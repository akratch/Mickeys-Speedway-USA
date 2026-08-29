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
import json
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
SCHEMA_VERSION = 1
ASSIGNABLE_STATE = "base-only"
SKIPPED_STATES = (
    "active",
    "already-integrated/exhausted",
    "stale-ledger",
    "not-live",
)


class ReadyQueueError(ValueError):
    """The ready queue cannot be produced without guessing."""


@dataclass(frozen=True)
class LiveIdentity:
    file: str
    symbol: str


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


def _ranking_details(row: dict[str, object], rank: int) -> dict[str, object]:
    return {
        "rank": rank,
        "file": row["file"],
        "symbol": row["name"],
        "category": row["category"],
        "size_bytes": row["size_bytes"],
        "differing_words": row["differing_words"],
        "first_mismatch_offset": row["first_mismatch_offset"],
        "size_delta": row["size_delta"],
        "objdiff_match_pct": row["objdiff_match_pct"],
    }


def build_report(
    document: object,
    live_items: Iterable[object],
    *,
    base: str,
    base_commit: str,
    ranking_name: str,
    scan: int,
    top: int,
    classify: AssignmentClassifier = lane_status.assignment_status,
) -> dict[str, object]:
    """Join ranking, live source identities, and assignment verdicts."""
    validated = nm_ranking.validate_ranking_document(document)
    functions = validated["functions"]
    unresolved = validated["unresolved_functions"]
    assert isinstance(functions, list)
    assert isinstance(unresolved, list)

    live = live_identities(live_items)
    live_keys = {(item.file, item.symbol) for item in live}
    live_paths_by_symbol: dict[str, set[str]] = {}
    for item in live:
        live_paths_by_symbol.setdefault(item.symbol, set()).add(item.file)

    ready: list[dict[str, object]] = []
    skipped: list[dict[str, object]] = []
    skipped_counts = {state: 0 for state in SKIPPED_STATES}
    scanned = 0

    for rank, raw_row in enumerate(functions, 1):
        if scanned >= scan or len(ready) >= top:
            break
        assert isinstance(raw_row, dict)
        scanned += 1
        file_name = str(raw_row["file"])
        symbol = str(raw_row["name"])
        key = (file_name, symbol)

        if key not in live_keys:
            other_paths = sorted(live_paths_by_symbol.get(symbol, set()))
            if other_paths:
                raise ReadyQueueError(
                    "source-path disagreement for "
                    f"{symbol}: ranking={file_name}, live={','.join(other_paths)}"
                )
            skipped_counts["not-live"] += 1
            skipped.append({
                **_ranking_details(raw_row, rank),
                "state": "not-live",
                "reason": "exact identity is absent from the live NON_MATCHING queue",
                "active_lanes": [],
                "source_commit": None,
                "ledger_commit": None,
            })
            continue

        assignment = classify(base, symbol)
        if assignment.source_path is not None and assignment.source_path != file_name:
            raise ReadyQueueError(
                "source-path disagreement for "
                f"{symbol}: ranking={file_name}, base={assignment.source_path}"
            )
        if assignment.state == ASSIGNABLE_STATE:
            if assignment.source_path != file_name:
                raise ReadyQueueError(
                    f"assignable {symbol} has no exact base source-path agreement"
                )
            ready.append({
                **_ranking_details(raw_row, rank),
                "state": assignment.state,
                "reason": assignment.reason,
            })
            continue
        if assignment.state not in skipped_counts:
            raise ReadyQueueError(
                f"lane_status returned unknown state {assignment.state!r} for {symbol}"
            )
        skipped_counts[assignment.state] += 1
        skipped.append({
            **_ranking_details(raw_row, rank),
            "state": assignment.state,
            "reason": assignment.reason,
            "active_lanes": list(assignment.active_lanes),
            "source_commit": assignment.source_commit,
            "ledger_commit": assignment.ledger_commit,
        })

    return {
        "schema_version": SCHEMA_VERSION,
        "base": base,
        "base_commit": base_commit,
        "ranking": ranking_name,
        "limits": {"scan": scan, "top": top},
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
            str(raw["symbol"]),
            str(raw["file"]),
            str(raw["category"]),
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
    headers = ["rank", "symbol", "file", "category", "diff/words"]
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
    headers = ["Rank", "Symbol", "File", "Category", "Diff/words"]
    rows = _display_rows(report)
    rendered = [
        "| " + " | ".join(headers) + " |",
        "| " + " | ".join("---" for _ in headers) + " |",
    ]
    for row in rows:
        escaped = [cell.replace("|", "\\|") for cell in row]
        escaped[1] = f"`{escaped[1]}`"
        escaped[2] = f"`{escaped[2]}`"
        rendered.append("| " + " | ".join(escaped) + " |")
    if not rows:
        rendered.append("| — | No assignable targets | — | — | — |")
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
        report = build_report(
            document,
            permute_batch.discover_queue(),
            base=args.base,
            base_commit=base_commit,
            ranking_name=portable_path(args.ranking),
            scan=args.scan,
            top=args.top,
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
