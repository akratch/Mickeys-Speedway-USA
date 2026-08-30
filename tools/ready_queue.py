#!/usr/bin/env python3
"""Select ranked NON_MATCHING targets that are safe to assign.

This is a read-only coordination tool. It validates the retained ranking,
cross-checks each examined exact ``(file, symbol)`` identity against the live
source queue, and accepts only lane_status's fail-closed ``base-only`` state.
It reads committed Git objects and source text; it never builds or reads ROM
text.
"""

from __future__ import annotations

import argparse
import concurrent.futures
import json
import math
import subprocess
from dataclasses import dataclass
from pathlib import Path
import sys
from typing import Callable, Iterable


ROOT = Path(__file__).resolve().parent.parent
TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import lane_status  # noqa: E402
import integration_base  # noqa: E402
import nm_ranking  # noqa: E402
import permute_batch  # noqa: E402


DEFAULT_RANKING = ROOT / "config" / "nonmatching-ranking.us.json"
DEFAULT_TOP = 10
MAX_SCAN = 1000
DEFAULT_SCAN = MAX_SCAN
MAX_TOP = 100
MAX_JOBS = 16
DEFAULT_JOBS = 4
SCHEMA_VERSION = 5
ASSIGNABLE_STATE = "base-only"
SKIPPED_STATES = (
    "dirty-worktree",
    "active",
    "already-integrated/exhausted",
    "stale-ledger",
    "not-live",
)
STALE_EVIDENCE_PENALTY = 40
HIGH_CONFIDENCE_MAX_RESIDUAL_RATIO = 0.35
HIGH_CONFIDENCE_MIN_YIELD_SCORE = 70.0

CATEGORY_PENALTY = {
    "register-only": 0,
    # Two recent exact overlay promotions showed that an equal-sized candidate
    # whose residual words are all relocation-bearing can be an ownership or
    # retained-data problem rather than a hard codegen problem.  Keep this
    # class near allocator/schedule work instead of burying it below `other`.
    "reloc-mismatch": 1,
    "allocation-mismatch": 2,
    "schedule-only": 2,
    "other": 4,
    "structure-mismatch": 16,
    "size-mismatch": 32,
}

# Additive points make the expected-yield heuristic inspectable instead of
# presenting a pseudo-probability.  The six components sum to 100 points:
# geometry 20, residual 35, mechanism 20, bounded size 15, freshness 5, and
# relocation-masked evidence 5.  A high-confidence tier additionally requires
# every fail-closed predicate below; a high score alone is never sufficient.
CATEGORY_YIELD_POINTS = {
    "register-only": 20.0,
    "schedule-only": 19.0,
    "reloc-mismatch": 19.0,
    "allocation-mismatch": 18.0,
    "other": 10.0,
    "structure-mismatch": 5.0,
    "size-mismatch": 2.0,
}
HIGH_CONFIDENCE_CATEGORIES = frozenset({
    "register-only",
    "schedule-only",
    "reloc-mismatch",
    "allocation-mismatch",
})
SELECTION_MODES = (
    "default",
    "expected-yield",
    "high-confidence",
    "broad-structural",
)

FOCUS_CATEGORIES = {
    "default": None,
    "retained-data": frozenset({"reloc-mismatch"}),
}


class ReadyQueueError(ValueError):
    """The ready queue cannot be produced without guessing."""


@dataclass(frozen=True)
class LiveIdentity:
    file: str
    symbol: str


@dataclass(frozen=True)
class RankingEvidence:
    commit: str
    fresh: bool


@dataclass(frozen=True)
class YieldEvidence:
    score: float
    residual_ratio: float
    residual_basis: str
    exact_geometry: bool
    selection_class: str
    blockers: tuple[str, ...]


FreshnessMap = dict[tuple[str, str], RankingEvidence]


AssignmentClassifier = Callable[[str, str], lane_status.Assignment]


def positive_bounded(value: str, *, name: str, maximum: int) -> int:
    try:
        parsed = int(value, 10)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(f"{name} must be an integer") from exc
    if not 1 <= parsed <= maximum:
        raise argparse.ArgumentTypeError(
            f"{name} must be between 1 and {maximum}"
        )
    return parsed


def scan_value(value: str) -> int:
    return positive_bounded(value, name="--scan", maximum=MAX_SCAN)


def top_value(value: str) -> int:
    return positive_bounded(value, name="--top", maximum=MAX_TOP)


def jobs_value(value: str) -> int:
    return positive_bounded(value, name="--jobs", maximum=MAX_JOBS)


def portable_path(path: Path) -> str:
    """Keep repository-local report identities independent of the checkout."""
    try:
        return path.resolve().relative_to(ROOT.resolve()).as_posix()
    except ValueError:
        return path.as_posix()


def live_identities(items: Iterable[object]) -> list[LiveIdentity]:
    """Normalize source discovery while refusing malformed/duplicate rows."""
    identities: list[LiveIdentity] = []
    seen: set[tuple[str, str]] = set()
    for index, item in enumerate(items):
        file_name = getattr(item, "rel_c_file", None)
        symbol = getattr(item, "func", None)
        if not isinstance(file_name, str) or not isinstance(symbol, str):
            raise ReadyQueueError(
                f"live queue row {index} lacks an exact file/symbol identity"
            )
        key = (file_name, symbol)
        if key in seen:
            raise ReadyQueueError(
                f"live queue contains duplicate identity {file_name}:{symbol}"
            )
        seen.add(key)
        identities.append(LiveIdentity(file_name, symbol))
    return identities


def proof_quality(row: dict[str, object]) -> str:
    if int(row["size_delta"]) != 0:
        return "size"
    category = str(row["category"])
    if category in {"register-only", "allocation-mismatch", "schedule-only"}:
        return "allocator"
    if category == "reloc-mismatch":
        return "relocation"
    if category == "structure-mismatch":
        return "structural"
    return "codegen"


def actionable_differing_words(row: dict[str, object]) -> int:
    """Prefer linker-field-masked evidence when the ranking has measured it."""
    masked = row.get("relocation_masked_differing_words")
    return int(masked) if masked is not None else int(row["differing_words"])


def expected_yield_evidence(
    row: dict[str, object], evidence: RankingEvidence | None,
) -> YieldEvidence:
    """Score expected bounded-attempt yield and derive a fail-closed tier.

    This is a transparent scheduling heuristic, not a match probability.  A
    row reaches ``high-confidence`` only when all prerequisite evidence is
    present and current; point totals cannot compensate for a missing proof.
    """
    target_words = max(1, int(row["size_bytes"]) // 4)
    candidate_words = max(
        0, (int(row["size_bytes"]) + int(row["size_delta"])) // 4,
    )
    geometry_words = max(1, target_words, candidate_words)
    masked = row.get("relocation_masked_differing_words")
    differing = int(masked) if masked is not None else int(row["differing_words"])
    residual_ratio = min(1.0, differing / geometry_words)
    exact_geometry = int(row["size_delta"]) == 0

    delta_words = abs(int(row["size_delta"])) // 4
    geometry_points = 20.0 if exact_geometry else max(0.0, 10.0 - delta_words)
    residual_points = 35.0 * (1.0 - residual_ratio)
    category = str(row["category"])
    category_points = CATEGORY_YIELD_POINTS.get(category, 4.0)
    size_points = 15.0 * min(1.0, math.sqrt(64.0 / geometry_words))
    freshness_points = (
        5.0 if evidence is not None and evidence.fresh
        else 0.0 if evidence is not None
        else 2.5
    )
    masked_points = 5.0 if masked is not None else 0.0
    score = round(
        geometry_points + residual_points + category_points + size_points
        + freshness_points + masked_points,
        2,
    )

    blockers: list[str] = []
    if evidence is None:
        blockers.append("freshness-unproven")
    elif not evidence.fresh:
        blockers.append("stale-ranking-evidence")
    if not exact_geometry:
        blockers.append("non-exact-geometry")
    if masked is None:
        blockers.append("masked-evidence-missing")
    if category not in HIGH_CONFIDENCE_CATEGORIES:
        blockers.append("broad-mismatch-category")
    if residual_ratio > HIGH_CONFIDENCE_MAX_RESIDUAL_RATIO:
        blockers.append("residual-above-0.35")
    if score < HIGH_CONFIDENCE_MIN_YIELD_SCORE:
        blockers.append("yield-score-below-70")

    if evidence is not None and not evidence.fresh:
        selection_class = "needs-reproof"
    elif not blockers:
        selection_class = "high-confidence"
    else:
        selection_class = "broad-structural"
    return YieldEvidence(
        score=score,
        residual_ratio=round(residual_ratio, 6),
        residual_basis="relocation-masked" if masked is not None else "raw",
        exact_geometry=exact_geometry,
        selection_class=selection_class,
        blockers=tuple(blockers),
    )


def effort_score(row: dict[str, object]) -> int:
    """Estimate bounded matching effort from retained aggregate evidence."""
    differing = actionable_differing_words(row)
    words = max(1, int(row["size_bytes"]) // 4)
    ratio_penalty = math.ceil(20 * differing / words)
    category_penalty = CATEGORY_PENALTY.get(str(row["category"]), 20)
    size_penalty = min(abs(int(row["size_delta"])) // 4, 50) * 2
    return differing + ratio_penalty + category_penalty + size_penalty


def prioritized_rows(
    functions: list[object], freshness: FreshnessMap | None = None,
    selection: str = "default",
) -> list[tuple[int, int, dict[str, object], int]]:
    """Return (priority rank, snapshot rank, row, effort score)."""
    if selection not in SELECTION_MODES:
        raise ReadyQueueError(f"unknown selection {selection!r}")
    measured: list[
        tuple[int, int, dict[str, object], YieldEvidence]
    ] = []
    for snapshot_rank, raw in enumerate(functions, 1):
        assert isinstance(raw, dict)
        key = (str(raw["file"]), str(raw["name"]))
        evidence = freshness.get(key) if freshness is not None else None
        yield_evidence = expected_yield_evidence(raw, evidence)
        if (
            selection == "high-confidence"
            and yield_evidence.selection_class != "high-confidence"
        ):
            continue
        if (
            selection == "broad-structural"
            and yield_evidence.selection_class != "broad-structural"
        ):
            continue
        stale_penalty = (
            STALE_EVIDENCE_PENALTY
            if evidence is not None and not evidence.fresh else 0
        )
        measured.append((
            effort_score(raw) + stale_penalty,
            snapshot_rank,
            raw,
            yield_evidence,
        ))
    if selection == "default":
        measured.sort(key=lambda value: (value[0], value[1]))
    else:
        tier = {
            "high-confidence": 0,
            "broad-structural": 1,
            "needs-reproof": 2,
        }
        measured.sort(key=lambda value: (
            tier[value[3].selection_class],
            -value[3].score,
            value[0],
            value[1],
        ))
    return [
        (priority_rank, snapshot_rank, row, score)
        for priority_rank, (score, snapshot_rank, row, _yield_evidence)
        in enumerate(measured, 1)
    ]


def _ranking_details(
    row: dict[str, object], rank: int, snapshot_rank: int, score: int,
    evidence: RankingEvidence | None,
) -> dict[str, object]:
    yield_evidence = expected_yield_evidence(row, evidence)
    return {
        "rank": rank,
        "snapshot_rank": snapshot_rank,
        "effort_score": score,
        "proof_quality": (
            "reproof-" + proof_quality(row)
            if evidence is not None and not evidence.fresh
            else proof_quality(row)
        ),
        "requires_reproof": evidence is not None and not evidence.fresh,
        "ranking_evidence_commit": evidence.commit if evidence else None,
        "ranking_evidence_fresh": evidence.fresh if evidence else None,
        "file": row["file"],
        "symbol": row["name"],
        "category": row["category"],
        "size_bytes": row["size_bytes"],
        "differing_words": row["differing_words"],
        "first_mismatch_offset": row["first_mismatch_offset"],
        "relocation_masked_differing_words": row.get(
            "relocation_masked_differing_words"
        ),
        "relocation_masked_first_mismatch_offset": row.get(
            "relocation_masked_first_mismatch_offset"
        ),
        "size_delta": row["size_delta"],
        "objdiff_match_pct": row["objdiff_match_pct"],
        "expected_yield_score": yield_evidence.score,
        "residual_ratio": yield_evidence.residual_ratio,
        "residual_basis": yield_evidence.residual_basis,
        "exact_geometry": yield_evidence.exact_geometry,
        "selection_class": yield_evidence.selection_class,
        "selection_blockers": list(yield_evidence.blockers),
    }


def ranking_freshness(
    base: str, ranking_path: str, document: object,
) -> FreshnessMap:
    """Prove each metric row was measured against the current source blob."""
    validated = nm_ranking.validate_ranking_document(document)
    functions = validated["functions"]
    assert isinstance(functions, list)
    try:
        evidence_commits = nm_ranking.ranking_evidence_commits(base, ranking_path)
    except nm_ranking.RankingDocumentError as exc:
        raise ReadyQueueError(str(exc)) from exc
    expected = {
        (str(row["file"]), str(row["name"]))
        for row in functions if isinstance(row, dict)
    }
    if set(evidence_commits) != expected:
        missing = sorted(expected - set(evidence_commits))
        extra = sorted(set(evidence_commits) - expected)
        raise ReadyQueueError(
            "ranking blame identities disagree with validated rows: "
            f"missing={missing[:3]} extra={extra[:3]}"
        )

    source_cache: dict[tuple[str, str], str | None] = {}
    digest_cache: dict[tuple[str, str, str], str | None] = {}

    def source(ref: str, path: str) -> str | None:
        key = (ref, path)
        if key not in source_cache:
            source_cache[key] = lane_status.show_file(ref, path)
        return source_cache[key]

    def digest(ref: str, path: str, symbol: str) -> str | None:
        key = (ref, path, symbol)
        if key not in digest_cache:
            digest_cache[key] = nm_ranking.source_context_digest(
                source(ref, path), symbol
            )
        return digest_cache[key]

    result: FreshnessMap = {}
    rows_by_key = {
        (str(row["file"]), str(row["name"])): row
        for row in functions if isinstance(row, dict)
    }
    for key, commit in evidence_commits.items():
        current = digest(base, key[0], key[1])
        embedded = rows_by_key[key].get(nm_ranking.SOURCE_CONTEXT_FIELD)
        measured = (
            nm_ranking.normalize_source_context_digest(embedded)
            if embedded is not None
            else digest(commit, key[0], key[1])
        )
        result[key] = RankingEvidence(
            commit=commit,
            fresh=current is not None and current == measured,
        )
    return result


def primary_worktree() -> Path:
    """Resolve the first/main worktree from Git's shared worktree registry."""
    proc = subprocess.run(
        ["git", "worktree", "list", "--porcelain"],
        cwd=ROOT, capture_output=True, text=True,
    )
    if proc.returncode:
        raise ReadyQueueError(
            proc.stderr.strip() or "cannot resolve the primary Git worktree"
        )
    for line in proc.stdout.splitlines():
        if line.startswith("worktree "):
            path = Path(line.removeprefix("worktree "))
            if not path.is_absolute():
                raise ReadyQueueError("Git returned a non-absolute primary worktree")
            return path
    raise ReadyQueueError("Git worktree registry has no primary worktree")


def dirty_worktree_paths(root: Path | None = None) -> set[str]:
    """Return tracked paths dirty in the primary worktree, not this lane."""
    root = primary_worktree() if root is None else root
    paths: set[str] = set()
    for args in (("diff", "--name-only"), ("diff", "--cached", "--name-only")):
        proc = subprocess.run(
            ["git", "-C", str(root), *args], capture_output=True, text=True,
        )
        if proc.returncode:
            raise ReadyQueueError(
                proc.stderr.strip() or f"cannot inspect primary worktree {root}"
            )
        paths.update(line for line in proc.stdout.splitlines() if line)
    return paths


def maintenance_action(state: str, maintenance_class: str) -> str:
    """Return one bounded next action for a coordination maintenance row."""
    if maintenance_class == "ranking-reproof":
        return (
            "refresh configured measurements with nice -n 15 .venv/bin/python "
            "tools/nm_ranking.py --refresh-stale --jobs 2 before source edits"
        )
    if maintenance_class == "prose-needs-remeasurement":
        return (
            "remeasure the configured baseline, then replace prose-only "
            "evidence with tools/finalize_plateau.py"
        )
    if maintenance_class == "stale-structured-evidence":
        return (
            "review the source marker, then refresh its exact-symbol shard "
            "with tools/plateau_handoff_audit.py --write"
        )
    if maintenance_class in {
        "legacy-invalid", "shard-invalid", "shard-source-mismatch",
        "source-identity", "source-missing", "history-missing",
    }:
        return "repair the exact-symbol evidence, then rerun lane_status.py"
    if maintenance_class == "lane-owned" or state == "active":
        return "review and release the listed lane before reassignment"
    if maintenance_class == "dirty-worktree" or state == "dirty-worktree":
        return "coordinate or commit the primary-worktree source edit"
    if maintenance_class == "not-live" or state == "not-live":
        return "prune the retired identity with tools/nm_ranking.py --prune-stale"
    return "review the fail-closed evidence and rerun lane_status.py"


def build_report(
    document: object,
    live_items: Iterable[object],
    *,
    base: str,
    base_commit: str,
    ranking_name: str,
    scan: int,
    top: int,
    jobs: int = 1,
    focus: str = "default",
    selection: str = "default",
    collect_maintenance: bool = False,
    freshness: FreshnessMap | None = None,
    dirty_paths: set[str] | None = None,
    classify: AssignmentClassifier = lane_status.assignment_status,
) -> dict[str, object]:
    """Join ranking, live source identities, and assignment verdicts."""
    validated = nm_ranking.validate_ranking_document(document)
    functions = validated["functions"]
    unresolved = validated["unresolved_functions"]
    assert isinstance(functions, list)
    assert isinstance(unresolved, list)

    live = live_identities(live_items)
    dirty_paths = dirty_paths or set()
    live_keys = {(item.file, item.symbol) for item in live}
    live_paths_by_symbol: dict[str, set[str]] = {}
    for item in live:
        live_paths_by_symbol.setdefault(item.symbol, set()).add(item.file)

    ready: list[dict[str, object]] = []
    skipped: list[dict[str, object]] = []
    maintenance: list[dict[str, object]] = []
    skipped_counts = {state: 0 for state in SKIPPED_STATES}
    maintenance_counts: dict[str, int] = {}
    scanned = 0

    if focus not in FOCUS_CATEGORIES:
        raise ReadyQueueError(f"unknown focus {focus!r}")
    focus_categories = FOCUS_CATEGORIES[focus]
    focused_functions = [
        row for row in functions
        if focus_categories is None
        or (
            isinstance(row, dict)
            and str(row.get("category")) in focus_categories
        )
    ]
    selected_rows = prioritized_rows(
        focused_functions, freshness, selection=selection,
    )
    ranked_rows = selected_rows[:scan]
    chunk_size = max(1, jobs * 2)
    for chunk_start in range(0, len(ranked_rows), chunk_size):
        if len(ready) >= top and not collect_maintenance:
            break
        chunk = ranked_rows[chunk_start : chunk_start + chunk_size]
        futures: dict[int, concurrent.futures.Future[lane_status.Assignment]] = {}
        executor: concurrent.futures.ThreadPoolExecutor | None = None
        if jobs > 1:
            executor = concurrent.futures.ThreadPoolExecutor(max_workers=jobs)
            for rank, _snapshot_rank, raw_row, _score in chunk:
                assert isinstance(raw_row, dict)
                key = (str(raw_row["file"]), str(raw_row["name"]))
                evidence = freshness.get(key) if freshness is not None else None
                if key in live_keys and key[0] not in dirty_paths:
                    futures[rank] = executor.submit(classify, base, key[1])

        try:
            for rank, snapshot_rank, raw_row, score in chunk:
                if len(ready) >= top and not collect_maintenance:
                    break
                assert isinstance(raw_row, dict)
                scanned += 1
                file_name = str(raw_row["file"])
                symbol = str(raw_row["name"])
                key = (file_name, symbol)
                evidence = freshness.get(key) if freshness is not None else None

                if key not in live_keys:
                    other_paths = sorted(live_paths_by_symbol.get(symbol, set()))
                    if other_paths:
                        raise ReadyQueueError(
                            "source-path disagreement for "
                            f"{symbol}: ranking={file_name}, "
                            f"live={','.join(other_paths)}"
                        )
                    skipped_counts["not-live"] += 1
                    skipped_row = {
                        **_ranking_details(
                            raw_row, rank, snapshot_rank, score, evidence,
                        ),
                        "state": "not-live",
                        "maintenance_class": "not-live",
                        "reason": (
                            "exact identity is absent from the live "
                            "NON_MATCHING queue"
                        ),
                        "active_lanes": [],
                        "source_commit": None,
                        "ledger_commit": None,
                    }
                    skipped.append(skipped_row)
                    maintenance.append({
                        **skipped_row,
                        "action": maintenance_action("not-live", "not-live"),
                    })
                    maintenance_counts["not-live"] = (
                        maintenance_counts.get("not-live", 0) + 1
                    )
                    continue

                if file_name in dirty_paths:
                    skipped_counts["dirty-worktree"] += 1
                    skipped_row = {
                        **_ranking_details(
                            raw_row, rank, snapshot_rank, score, evidence,
                        ),
                        "state": "dirty-worktree",
                        "maintenance_class": "dirty-worktree",
                        "reason": (
                            "owning source path has uncommitted primary-"
                            "worktree changes"
                        ),
                        "active_lanes": [],
                        "source_commit": None,
                        "ledger_commit": None,
                    }
                    skipped.append(skipped_row)
                    maintenance.append({
                        **skipped_row,
                        "action": maintenance_action(
                            "dirty-worktree", "dirty-worktree",
                        ),
                    })
                    maintenance_counts["dirty-worktree"] = (
                        maintenance_counts.get("dirty-worktree", 0) + 1
                    )
                    continue

                assignment = (
                    futures[rank].result()
                    if executor is not None
                    else classify(base, symbol)
                )
                if (
                    assignment.source_path is not None
                    and assignment.source_path != file_name
                ):
                    raise ReadyQueueError(
                        "source-path disagreement for "
                        f"{symbol}: ranking={file_name}, "
                        f"base={assignment.source_path}"
                    )
                if assignment.state == ASSIGNABLE_STATE:
                    if assignment.source_path != file_name:
                        raise ReadyQueueError(
                            f"assignable {symbol} has no exact base "
                            "source-path agreement"
                        )
                    ready_row = {
                        **_ranking_details(
                            raw_row, rank, snapshot_rank, score, evidence,
                        ),
                        "state": assignment.state,
                        "reason": (
                            assignment.reason
                            if evidence is None or evidence.fresh
                            else assignment.reason + "; refresh configured "
                            "baseline before source edits"
                        ),
                    }
                    if len(ready) < top:
                        ready.append(ready_row)
                    if evidence is not None and not evidence.fresh:
                        maintenance_class = "ranking-reproof"
                        maintenance_counts[maintenance_class] = (
                            maintenance_counts.get(maintenance_class, 0) + 1
                        )
                        maintenance.append({
                            **ready_row,
                            "state": "reproof",
                            "maintenance_class": maintenance_class,
                            "action": maintenance_action(
                                "reproof", maintenance_class,
                            ),
                        })
                    continue
                if assignment.state not in skipped_counts:
                    raise ReadyQueueError(
                        "lane_status returned unknown state "
                        f"{assignment.state!r} for {symbol}"
                    )
                skipped_counts[assignment.state] += 1
                maintenance_class = assignment.reason_code or "unclassified"
                maintenance_counts[maintenance_class] = (
                    maintenance_counts.get(maintenance_class, 0) + 1
                )
                skipped_row = {
                    **_ranking_details(
                        raw_row, rank, snapshot_rank, score, evidence,
                    ),
                    "state": assignment.state,
                    "maintenance_class": maintenance_class,
                    "reason": assignment.reason,
                    "active_lanes": list(assignment.active_lanes),
                    "source_commit": assignment.source_commit,
                    "ledger_commit": assignment.ledger_commit,
                }
                skipped.append(skipped_row)
                if assignment.state in {"active", "stale-ledger"}:
                    maintenance.append({
                        **skipped_row,
                        "action": maintenance_action(
                            assignment.state, maintenance_class,
                        ),
                    })
        finally:
            if executor is not None:
                executor.shutdown(wait=True, cancel_futures=True)

    file_counts: dict[str, int] = {}
    for raw in ready:
        file_name = str(raw["file"])
        file_counts[file_name] = file_counts.get(file_name, 0) + 1
    file_positions: dict[str, int] = {}
    for raw in ready:
        file_name = str(raw["file"])
        file_positions[file_name] = file_positions.get(file_name, 0) + 1
        raw["file_batch_position"] = file_positions[file_name]
        raw["file_batch_size"] = file_counts[file_name]

    return {
        "schema_version": SCHEMA_VERSION,
        "base": base,
        "base_commit": base_commit,
        "ranking": ranking_name,
        "focus": focus,
        "selection": selection,
        "limits": {
            "scan": scan,
            "top": top,
            "jobs": jobs,
            "collect_maintenance": collect_maintenance,
        },
        "ranking_rows": len(functions),
        "focused_ranking_rows": len(focused_functions),
        "selected_ranking_rows": len(selected_rows),
        "unresolved_rows": len(unresolved),
        "scanned": scanned,
        "ready": ready,
        "skipped": skipped,
        "maintenance": maintenance,
        "summary": {
            "ready": len(ready),
            "skipped": skipped_counts,
            "maintenance_classes": dict(sorted(maintenance_counts.items())),
            "top_limit_reached": len(ready) >= top,
            "scan_limit_reached": scanned >= scan and (
                len(ready) < top
                or collect_maintenance and scanned < len(selected_rows)
            ),
            "ranking_exhausted": scanned == len(selected_rows),
            "maintenance_scan_complete": (
                collect_maintenance and scanned == len(selected_rows)
            ),
        },
    }


def _display_headers(report: dict[str, object]) -> list[str]:
    if report["selection"] == "default":
        return [
            "rank", "snap", "symbol", "file", "quality", "cost", "TU batch",
            "masked/raw/words",
        ]
    return [
        "rank", "snap", "symbol", "file", "tier", "yield", "residual",
        "geometry", "category", "TU batch", "masked/raw/words",
    ]


def _display_rows(report: dict[str, object]) -> list[list[str]]:
    ready = report["ready"]
    assert isinstance(ready, list)
    rows: list[list[str]] = []
    for raw in ready:
        assert isinstance(raw, dict)
        mismatch = raw["differing_words"]
        masked = raw.get("relocation_masked_differing_words")
        size = raw["size_bytes"]
        common = [
            str(raw["rank"]), str(raw["snapshot_rank"]), str(raw["symbol"]),
            str(raw["file"]),
        ]
        tail = [
            f"{raw['file_batch_position']}/{raw['file_batch_size']}",
            f"{'—' if masked is None else masked}/{mismatch}/{int(size) // 4}",
        ]
        if report["selection"] == "default":
            rows.append(common + [
                str(raw["proof_quality"]), str(raw["effort_score"]), *tail,
            ])
        else:
            geometry = (
                "exact" if raw["exact_geometry"]
                else f"delta {int(raw['size_delta']) // 4:+d}w"
            )
            rows.append(common + [
                str(raw["selection_class"]),
                f"{float(raw['expected_yield_score']):.2f}",
                f"{float(raw['residual_ratio']):.3f}",
                geometry,
                str(raw["category"]),
                *tail,
            ])
    return rows


def summary_line(report: dict[str, object]) -> str:
    summary = report["summary"]
    assert isinstance(summary, dict)
    skipped = summary["skipped"]
    assert isinstance(skipped, dict)
    skip_text = ", ".join(f"{state}={skipped[state]}" for state in SKIPPED_STATES)
    selection = (
        "" if report["selection"] == "default"
        else f" selection={report['selection']}"
    )
    return (
        f"ready={summary['ready']} scanned={report['scanned']}/"
        f"{report['limits']['scan']}{selection} skipped: {skip_text}; "
        f"unresolved-ranking-rows={report['unresolved_rows']}"
    )


def render_table(report: dict[str, object]) -> str:
    headers = _display_headers(report)
    rows = _display_rows(report)
    if not rows:
        return f"(no assignable targets)\n{summary_line(report)}\n"
    widths = [len(header) for header in headers]
    for row in rows:
        for index, cell in enumerate(row):
            widths[index] = max(widths[index], len(cell))

    def line(row: list[str]) -> str:
        return "  ".join(cell.ljust(widths[index]) for index, cell in enumerate(row))

    rendered = [line(headers), line(["-" * width for width in widths])]
    rendered.extend(line(row) for row in rows)
    rendered.append(summary_line(report))
    return "\n".join(rendered) + "\n"


def render_markdown(report: dict[str, object]) -> str:
    headers = [header.title() for header in _display_headers(report)]
    rows = _display_rows(report)
    rendered = [
        "| " + " | ".join(headers) + " |",
        "| " + " | ".join("---" for _ in headers) + " |",
    ]
    for row in rows:
        escaped = [cell.replace("|", "\\|") for cell in row]
        escaped[2] = f"`{escaped[2]}`"
        escaped[3] = f"`{escaped[3]}`"
        rendered.append("| " + " | ".join(escaped) + " |")
    if not rows:
        rendered.append(
            "| " + " | ".join(
                "No assignable targets" if index == 2 else "—"
                for index in range(len(headers))
            ) + " |"
        )
    rendered.extend(("", summary_line(report)))
    return "\n".join(rendered) + "\n"


def render_maintenance(report: dict[str, object]) -> str:
    """Render only queue blockers that require coordination maintenance."""
    maintenance = report["maintenance"]
    assert isinstance(maintenance, list)
    rows: list[list[str]] = []
    for raw in maintenance:
        assert isinstance(raw, dict)
        state = str(raw["state"])
        detail = str(raw["reason"])
        if state == "active":
            lanes = raw.get("active_lanes", [])
            assert isinstance(lanes, list)
            detail = f"{len(lanes)} lane ref(s): {detail}"
        rows.append([
            str(raw["rank"]), state, str(raw["maintenance_class"]),
            str(raw["symbol"]),
            str(raw["file"]), str(raw["action"]), detail,
        ])
    headers = [
        "rank", "state", "class", "symbol", "file", "next action", "evidence",
    ]
    if not rows:
        return f"(no maintenance blockers)\n{summary_line(report)}\n"
    widths = [len(header) for header in headers]
    for row in rows:
        for index, cell in enumerate(row):
            widths[index] = max(widths[index], len(cell))

    def line(row: list[str]) -> str:
        return "  ".join(
            cell.ljust(widths[index]) for index, cell in enumerate(row)
        )

    rendered = [line(headers), line(["-" * width for width in widths])]
    rendered.extend(line(row) for row in rows)
    rendered.append(summary_line(report))
    return "\n".join(rendered) + "\n"


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--base",
        help="canonical Git ref (default: freshest linear integration ref)",
    )
    parser.add_argument(
        "--ranking", type=Path, default=DEFAULT_RANKING,
        help=f"ranking JSON (default {DEFAULT_RANKING.relative_to(ROOT)})",
    )
    parser.add_argument(
        "--scan", type=scan_value, default=DEFAULT_SCAN,
        help=(
            "maximum ranked rows to examine before the requested queue is "
            f"full (1-{MAX_SCAN}; default {DEFAULT_SCAN})"
        ),
    )
    parser.add_argument(
        "--top", type=top_value, default=DEFAULT_TOP,
        help=f"maximum assignable rows to return (1-{MAX_TOP}; default {DEFAULT_TOP})",
    )
    parser.add_argument(
        "--jobs", type=jobs_value, default=DEFAULT_JOBS,
        help=(
            "parallel read-only assignment checks "
            f"(1-{MAX_JOBS}; default {DEFAULT_JOBS})"
        ),
    )
    parser.add_argument(
        "--focus", choices=tuple(FOCUS_CATEGORIES), default="default",
        help=(
            "queue view: default bounded-effort order, or retained-data for "
            "equal-sized rows whose residuals are all relocation-bearing"
        ),
    )
    parser.add_argument(
        "--selection", choices=SELECTION_MODES, default="default",
        help=(
            "selection order: unchanged default effort order, expected-yield, "
            "or a fail-closed high-confidence/broad-structural tier"
        ),
    )
    parser.add_argument(
        "--format", choices=("table", "markdown", "maintenance", "json"),
        default="table",
        help=(
            "stable JSON, concise ready output, or a maintenance-blocker "
            "view (default table)"
        ),
    )
    args = parser.parse_args(argv)
    if args.top > args.scan:
        parser.error("--top cannot exceed --scan")
    return args


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        if args.base is None:
            args.base = integration_base.resolve(ROOT)
        base_commit = lane_status.git(
            "rev-parse", "--verify", f"{args.base}^{{commit}}"
        ).strip()
        document = json.loads(args.ranking.read_text(encoding="utf-8"))
        ranking_name = portable_path(args.ranking)
        freshness = ranking_freshness(args.base, ranking_name, document)
        dirty_paths = dirty_worktree_paths()
        live_items = permute_batch.discover_queue()
        identities = live_identities(live_items)
        context = lane_status.AssignmentContext.build(
            args.base, [identity.symbol for identity in identities],
            jobs=args.jobs,
        )
        report = build_report(
            document,
            live_items,
            base=args.base,
            base_commit=base_commit,
            ranking_name=ranking_name,
            scan=args.scan, top=args.top, jobs=args.jobs,
            focus=args.focus,
            selection=args.selection,
            collect_maintenance=args.format == "maintenance",
            freshness=freshness,
            dirty_paths=dirty_paths,
            classify=context.classify,
        )
    except (
        OSError,
        json.JSONDecodeError,
        nm_ranking.RankingDocumentError,
        ReadyQueueError,
        RuntimeError,
    ) as exc:
        print(f"ready_queue: {exc}", file=sys.stderr)
        return 2

    if args.format == "json":
        print(json.dumps(report, indent=2, sort_keys=True))
    elif args.format == "markdown":
        sys.stdout.write(render_markdown(report))
    elif args.format == "maintenance":
        sys.stdout.write(render_maintenance(report))
    else:
        sys.stdout.write(render_table(report))
    return 0 if report["ready"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
