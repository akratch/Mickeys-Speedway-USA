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
import nm_ranking  # noqa: E402
import permute_batch  # noqa: E402


DEFAULT_RANKING = ROOT / "config" / "nonmatching-ranking.us.json"
DEFAULT_BASE = "campaign/unchain"
DEFAULT_SCAN = 50
DEFAULT_TOP = 10
MAX_SCAN = 1000
MAX_TOP = 100
MAX_JOBS = 16
DEFAULT_JOBS = 4
SCHEMA_VERSION = 3
ASSIGNABLE_STATE = "base-only"
SKIPPED_STATES = (
    "dirty-worktree",
    "active",
    "already-integrated/exhausted",
    "stale-ledger",
    "not-live",
)
STALE_EVIDENCE_PENALTY = 40

CATEGORY_PENALTY = {
    "register-only": 0,
    "allocation-mismatch": 2,
    "schedule-only": 2,
    "other": 4,
    "reloc-mismatch": 8,
    "structure-mismatch": 16,
    "size-mismatch": 32,
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


def effort_score(row: dict[str, object]) -> int:
    """Estimate bounded matching effort from retained aggregate evidence."""
    differing = int(row["differing_words"])
    words = max(1, int(row["size_bytes"]) // 4)
    ratio_penalty = math.ceil(20 * differing / words)
    category_penalty = CATEGORY_PENALTY.get(str(row["category"]), 20)
    size_penalty = min(abs(int(row["size_delta"])) // 4, 50) * 2
    return differing + ratio_penalty + category_penalty + size_penalty


def prioritized_rows(
    functions: list[object], freshness: FreshnessMap | None = None,
) -> list[tuple[int, int, dict[str, object], int]]:
    """Return (priority rank, snapshot rank, row, effort score)."""
    measured = []
    for snapshot_rank, raw in enumerate(functions, 1):
        assert isinstance(raw, dict)
        key = (str(raw["file"]), str(raw["name"]))
        evidence = freshness.get(key) if freshness is not None else None
        stale_penalty = (
            STALE_EVIDENCE_PENALTY
            if evidence is not None and not evidence.fresh else 0
        )
        measured.append((effort_score(raw) + stale_penalty, snapshot_rank, raw))
    measured.sort(key=lambda value: (value[0], value[1]))
    return [
        (priority_rank, snapshot_rank, row, score)
        for priority_rank, (score, snapshot_rank, row)
        in enumerate(measured, 1)
    ]


def _ranking_details(
    row: dict[str, object], rank: int, snapshot_rank: int, score: int,
    evidence: RankingEvidence | None,
) -> dict[str, object]:
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
        "size_delta": row["size_delta"],
        "objdiff_match_pct": row["objdiff_match_pct"],
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


def dirty_worktree_paths() -> set[str]:
    """Return tracked paths changed in either the index or working tree."""
    paths: set[str] = set()
    for args in (("diff", "--name-only"), ("diff", "--cached", "--name-only")):
        paths.update(
            line for line in lane_status.git(*args).splitlines() if line
        )
    return paths


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
    skipped_counts = {state: 0 for state in SKIPPED_STATES}
    scanned = 0

    ranked_rows = prioritized_rows(functions, freshness)[:scan]
    chunk_size = max(1, jobs * 2)
    for chunk_start in range(0, len(ranked_rows), chunk_size):
        if len(ready) >= top:
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
                if len(ready) >= top:
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
                    skipped.append({
                        **_ranking_details(
                            raw_row, rank, snapshot_rank, score, evidence,
                        ),
                        "state": "not-live",
                        "reason": (
                            "exact identity is absent from the live "
                            "NON_MATCHING queue"
                        ),
                        "active_lanes": [],
                        "source_commit": None,
                        "ledger_commit": None,
                    })
                    continue

                if file_name in dirty_paths:
                    skipped_counts["dirty-worktree"] += 1
                    skipped.append({
                        **_ranking_details(
                            raw_row, rank, snapshot_rank, score, evidence,
                        ),
                        "state": "dirty-worktree",
                        "reason": (
                            "owning source path has uncommitted primary-"
                            "worktree changes"
                        ),
                        "active_lanes": [],
                        "source_commit": None,
                        "ledger_commit": None,
                    })
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
                    ready.append({
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
                    })
                    continue
                if assignment.state not in skipped_counts:
                    raise ReadyQueueError(
                        "lane_status returned unknown state "
                        f"{assignment.state!r} for {symbol}"
                    )
                skipped_counts[assignment.state] += 1
                skipped.append({
                    **_ranking_details(
                        raw_row, rank, snapshot_rank, score, evidence,
                    ),
                    "state": assignment.state,
                    "reason": assignment.reason,
                    "active_lanes": list(assignment.active_lanes),
                    "source_commit": assignment.source_commit,
                    "ledger_commit": assignment.ledger_commit,
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
        "limits": {"scan": scan, "top": top, "jobs": jobs},
        "ranking_rows": len(functions),
        "unresolved_rows": len(unresolved),
        "scanned": scanned,
        "ready": ready,
        "skipped": skipped,
        "summary": {
            "ready": len(ready),
            "skipped": skipped_counts,
            "top_limit_reached": len(ready) >= top,
            "scan_limit_reached": scanned >= scan and len(ready) < top,
            "ranking_exhausted": scanned == len(functions),
        },
    }


def _display_rows(report: dict[str, object]) -> list[list[str]]:
    ready = report["ready"]
    assert isinstance(ready, list)
    rows: list[list[str]] = []
    for raw in ready:
        assert isinstance(raw, dict)
        mismatch = raw["differing_words"]
        size = raw["size_bytes"]
        rows.append([
            str(raw["rank"]),
            str(raw["snapshot_rank"]),
            str(raw["symbol"]),
            str(raw["file"]),
            str(raw["proof_quality"]),
            str(raw["effort_score"]),
            f"{raw['file_batch_position']}/{raw['file_batch_size']}",
            f"{mismatch}/{int(size) // 4}",
        ])
    return rows


def summary_line(report: dict[str, object]) -> str:
    summary = report["summary"]
    assert isinstance(summary, dict)
    skipped = summary["skipped"]
    assert isinstance(skipped, dict)
    skip_text = ", ".join(f"{state}={skipped[state]}" for state in SKIPPED_STATES)
    return (
        f"ready={summary['ready']} scanned={report['scanned']}/"
        f"{report['limits']['scan']} skipped: {skip_text}; "
        f"unresolved-ranking-rows={report['unresolved_rows']}"
    )


def render_table(report: dict[str, object]) -> str:
    headers = [
        "rank", "snap", "symbol", "file", "quality", "cost", "TU batch",
        "diff/words",
    ]
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
    headers = [
        "Rank", "Snapshot", "Symbol", "File", "Quality", "Cost", "TU batch",
        "Diff/words",
    ]
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
        rendered.append("| — | — | No assignable targets | — | — | — | — | — |")
    rendered.extend(("", summary_line(report)))
    return "\n".join(rendered) + "\n"


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", default=DEFAULT_BASE, help="canonical Git ref")
    parser.add_argument(
        "--ranking", type=Path, default=DEFAULT_RANKING,
        help=f"ranking JSON (default {DEFAULT_RANKING.relative_to(ROOT)})",
    )
    parser.add_argument(
        "--scan", type=scan_value, default=DEFAULT_SCAN,
        help=f"maximum ranked rows to examine (1-{MAX_SCAN}; default {DEFAULT_SCAN})",
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
        "--format", choices=("table", "markdown", "json"), default="table",
        help="stable JSON or concise human-readable output (default table)",
    )
    args = parser.parse_args(argv)
    if args.top > args.scan:
        parser.error("--top cannot exceed --scan")
    return args


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        base_commit = lane_status.git(
            "rev-parse", "--verify", f"{args.base}^{{commit}}"
        ).strip()
        document = json.loads(args.ranking.read_text(encoding="utf-8"))
        ranking_name = portable_path(args.ranking)
        freshness = ranking_freshness(args.base, ranking_name, document)
        dirty_paths = dirty_worktree_paths()
        report = build_report(
            document,
            permute_batch.discover_queue(),
            base=args.base,
            base_commit=base_commit,
            ranking_name=ranking_name,
            scan=args.scan, top=args.top, jobs=args.jobs,
            freshness=freshness,
            dirty_paths=dirty_paths,
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
    else:
        sys.stdout.write(render_table(report))
    return 0 if report["ready"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
