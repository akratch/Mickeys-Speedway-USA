#!/usr/bin/env python3
"""Remeasure stale source plateaus through the canonical preflight pipeline.

The ready queue deliberately refuses source comments that lack an exact-symbol
handoff. This helper turns that maintenance class into a bounded, read-only
measurement batch. It emits scalar proof summaries only; candidate bytes,
assembly, and aligned rows remain in ignored build artifacts.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
from pathlib import Path
from typing import Sequence


ROOT = Path(__file__).resolve().parents[1]
PYTHON = ROOT / ".venv/bin/python"
READY_QUEUE = ROOT / "tools/ready_queue.py"
PREFLIGHT = ROOT / "tools/function_preflight.py"
SCHEMA = "mickey-plateau-remeasurement-v1"
SYMBOL_RE = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
MAINTENANCE_CLASS = "prose-needs-remeasurement"


class RemeasureError(ValueError):
    """The batch cannot continue without guessing about evidence."""


def bounded_int(value: str, *, label: str, minimum: int, maximum: int) -> int:
    try:
        parsed = int(value, 10)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(f"{label} must be an integer") from exc
    if not minimum <= parsed <= maximum:
        raise argparse.ArgumentTypeError(
            f"{label} must be between {minimum} and {maximum}"
        )
    return parsed


def _plain_int(value: object, label: str, *, minimum: int = 0) -> int:
    if isinstance(value, bool) or not isinstance(value, int) or value < minimum:
        raise RemeasureError(f"{label} must be an integer >= {minimum}")
    return value


def _optional_int(value: object, label: str) -> int | None:
    if value is None:
        return None
    return _plain_int(value, label)


def validate_symbol(value: object) -> str:
    if not isinstance(value, str) or not SYMBOL_RE.fullmatch(value):
        raise RemeasureError(f"invalid exact symbol {value!r}")
    return value


def select_stale_rows(document: object, limit: int) -> list[dict[str, object]]:
    if not isinstance(document, dict) or document.get("schema_version") != 4:
        schema = document.get("schema_version") if isinstance(document, dict) else None
        raise RemeasureError(f"unsupported ready-queue schema {schema!r}")
    skipped = document.get("skipped")
    if not isinstance(skipped, list):
        raise RemeasureError("ready-queue report lacks a skipped row list")
    selected: list[dict[str, object]] = []
    seen: set[str] = set()
    for index, row in enumerate(skipped):
        if not isinstance(row, dict):
            raise RemeasureError(f"skipped[{index}] is not an object")
        if row.get("maintenance_class") != MAINTENANCE_CLASS:
            continue
        symbol = validate_symbol(row.get("symbol"))
        source = row.get("file")
        if not isinstance(source, str) or not source.startswith("src/"):
            raise RemeasureError(f"{symbol} lacks a portable source path")
        if symbol in seen:
            raise RemeasureError(f"duplicate stale symbol {symbol}")
        seen.add(symbol)
        selected.append(
            {
                "symbol": symbol,
                "source": source,
                "queue_rank": _plain_int(row.get("rank"), f"{symbol}.rank", minimum=1),
                "retained_differing_words": _plain_int(
                    row.get("differing_words"), f"{symbol}.differing_words"
                ),
                "retained_masked_differing_words": _optional_int(
                    row.get("relocation_masked_differing_words"),
                    f"{symbol}.relocation_masked_differing_words",
                ),
            }
        )
        if len(selected) == limit:
            break
    return selected


def summarize_preflight(report: object) -> dict[str, object]:
    if not isinstance(report, dict) or report.get("schema") != "mickey-function-evidence-preflight-v1":
        schema = report.get("schema") if isinstance(report, dict) else None
        raise RemeasureError(f"unexpected preflight schema {schema!r}")
    symbol = validate_symbol(report.get("candidate_symbol"))
    source = report.get("source")
    if not isinstance(source, str) or not source.startswith("src/"):
        raise RemeasureError(f"{symbol} preflight lacks a portable source path")
    workbench = report.get("workbench")
    preflight = report.get("preflight")
    if not isinstance(workbench, dict) or not isinstance(preflight, dict):
        raise RemeasureError(f"{symbol} preflight lacks workbench/status evidence")
    counts = preflight.get("counts")
    if not isinstance(counts, dict):
        raise RemeasureError(f"{symbol} preflight lacks relocation counts")
    status = preflight.get("status")
    if status not in ("complete", "partial"):
        raise RemeasureError(f"{symbol} has invalid preflight status {status!r}")
    first = workbench.get("first_mismatch")
    if first is not None and (
        not isinstance(first, str) or not re.fullmatch(r"\+0x[0-9A-F]+", first)
    ):
        raise RemeasureError(f"{symbol} has invalid first mismatch {first!r}")
    return {
        "symbol": symbol,
        "source": source,
        "status": status,
        "verdict": workbench.get("verdict"),
        "differing_words": _plain_int(
            workbench.get("differing_words"), f"{symbol}.differing_words"
        ),
        "target_words": _plain_int(
            workbench.get("target_words"), f"{symbol}.target_words", minimum=1
        ),
        "candidate_words": _plain_int(
            workbench.get("candidate_words"), f"{symbol}.candidate_words", minimum=1
        ),
        "first_mismatch": first,
        "target_frame": _optional_int(workbench.get("target_frame"), f"{symbol}.target_frame"),
        "candidate_frame": _optional_int(
            workbench.get("candidate_frame"), f"{symbol}.candidate_frame"
        ),
        "target_relocations": _plain_int(
            counts.get("target_relocations"), f"{symbol}.target_relocations"
        ),
        "candidate_relocations": _plain_int(
            counts.get("candidate_static_relocations"), f"{symbol}.candidate_relocations"
        ),
        "resolved_identities": _plain_int(
            counts.get("candidate_identities_resolved"), f"{symbol}.resolved_identities"
        ),
        "unresolved_identities": _plain_int(
            counts.get("candidate_identities_unresolved"), f"{symbol}.unresolved_identities"
        ),
        "offset_type_aligned": _plain_int(
            counts.get("offset_type_aligned"), f"{symbol}.offset_type_aligned"
        ),
    }


def _run_json(command: Sequence[str], *, timeout: int, label: str) -> object:
    try:
        result = subprocess.run(
            command, cwd=ROOT, text=True, capture_output=True, timeout=timeout, check=False
        )
    except subprocess.TimeoutExpired as exc:
        raise RemeasureError(f"{label} exceeded {timeout}s") from exc
    if result.returncode:
        diagnostic = (result.stderr or result.stdout).strip().splitlines()
        tail = diagnostic[-1] if diagnostic else "no diagnostic"
        raise RemeasureError(
            f"{label} failed ({result.returncode}): {tail.replace(str(ROOT), '.')}"
        )
    try:
        return json.loads(result.stdout)
    except json.JSONDecodeError as exc:
        raise RemeasureError(f"{label} did not emit one JSON document") from exc


def discover(args: argparse.Namespace) -> list[dict[str, object]]:
    command = [
        str(PYTHON), str(READY_QUEUE), "--base", args.base,
        "--scan", str(args.scan), "--top", "100", "--jobs", str(args.queue_jobs),
        "--format", "json",
    ]
    return select_stale_rows(
        _run_json(command, timeout=args.timeout, label="ready queue"), args.top
    )


def measure(row: dict[str, object], args: argparse.Namespace) -> dict[str, object]:
    symbol = validate_symbol(row["symbol"])
    command = [
        "nice", "-n", "10", str(PYTHON), str(PREFLIGHT), symbol,
        "--analysis-only", "--json",
    ]
    if args.no_build:
        command.append("--no-build")
    report = _run_json(command, timeout=args.timeout, label=f"preflight {symbol}")
    measured = summarize_preflight(report)
    measured.update({key: value for key, value in row.items() if key not in measured})
    return measured


def render_table(rows: Sequence[dict[str, object]]) -> str:
    lines = [
        "rank  symbol                                        diff  words       frame       reloc  ids  status",
        "----  --------------------------------------------  ----  ----------  ----------  -----  ---  --------",
    ]
    for row in rows:
        frame = f"{row['candidate_frame']}/{row['target_frame']}"
        reloc = f"{row['candidate_relocations']}/{row['target_relocations']}"
        words = f"{row['candidate_words']}/{row['target_words']}"
        lines.append(
            f"{str(row.get('queue_rank', '-')):>4}  {str(row['symbol']):<44}  "
            f"{int(row['differing_words']):>4}  {words:<10}  {frame:<10}  "
            f"{reloc:<5}  {int(row['resolved_identities']):>3}  {row['status']}"
        )
    return "\n".join(lines)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("symbols", nargs="*", help="exact symbols; omit to discover stale queue rows")
    parser.add_argument("--base", default="origin/campaign/unchain")
    parser.add_argument("--scan", type=lambda value: bounded_int(value, label="--scan", minimum=1, maximum=1000), default=300)
    parser.add_argument("--top", type=lambda value: bounded_int(value, label="--top", minimum=1, maximum=100), default=10)
    parser.add_argument("--queue-jobs", type=lambda value: bounded_int(value, label="--queue-jobs", minimum=1, maximum=16), default=4)
    parser.add_argument("--timeout", type=lambda value: bounded_int(value, label="--timeout", minimum=10, maximum=1800), default=600, help="per-command timeout in seconds")
    parser.add_argument("--no-build", action="store_true", help="require current preflight artifacts")
    parser.add_argument("--format", choices=("table", "json"), default="table")
    return parser


def main(argv: Sequence[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    try:
        if args.symbols:
            seen: set[str] = set()
            selected = []
            for raw in args.symbols[: args.top]:
                symbol = validate_symbol(raw)
                if symbol in seen:
                    raise RemeasureError(f"duplicate requested symbol {symbol}")
                seen.add(symbol)
                selected.append({"symbol": symbol})
        else:
            selected = discover(args)
        if not selected:
            raise RemeasureError("no stale plateau rows selected")
        measured = [measure(row, args) for row in selected]
    except RemeasureError as error:
        parser.error(str(error))
    payload = {"schema": SCHEMA, "count": len(measured), "functions": measured}
    print(
        json.dumps(payload, indent=2, sort_keys=True)
        if args.format == "json" else render_table(measured)
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
