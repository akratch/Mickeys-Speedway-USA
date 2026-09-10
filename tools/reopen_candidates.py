#!/usr/bin/env python3
"""List still-queued functions whose closure predates a compiler law.

    tools/reopen_candidates.py [--since YYYY-MM-DD] [--top N] [--json]

A plateau handoff records that a search came back flat. It is evidence about
**the lever set it was measured against**, not about the function. When a new
law lands, every closure written before it becomes a re-open candidate on its
face -- and that is computable rather than arguable.

It stopped being hypothetical on 2026-09-10: `func_overlay_041_F0000854`
carried a handoff recording its C-body search as closed, no reopen
authorization existed for it, and it matched exactly, in one lane, on the first
attempt after three new laws landed. The winning edit was never in the space
the closure searched. Its own evidence was never wrong -- only its scope was.
Reaching it took a hunch; this makes it a query.

A row here is a prompt to re-read a closure, never permission to skip it. The
useful output of working one is often "this closure was correct and remains
correct under the new laws", which is worth recording as explicitly as a match.
"""
from __future__ import annotations

import argparse
import json
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
HANDOFFS = ROOT / "docs" / "matching-triage-handoffs"
RANKING = ROOT / "config" / "nonmatching-ranking.us.json"

# Constructs each law reaches. A closure whose text mentions one was searching
# in territory that law now describes, so it is worth re-reading.
LAW_CONSTRUCTS = {
    "L90 induction exit test": re.compile(
        r"\bloop\b|\bcounter\b|induction|exit test|\bfor\b|\bwhile\b", re.I),
    "L92 operand weight": re.compile(
        r"commut|operand order|address (?:sum|arith)|\bindex(?:ed)?\b|scaled", re.I),
    "L93 loop shape": re.compile(
        r"do\s*\{|do/while|top-tested|bottom-tested|loop shape|\blatch\b", re.I),
    "L94 co-region reads": re.compile(
        r"\bextern\b|\bimport\b|global address|two reads|%hi|materiali", re.I),
}


def queued() -> dict[str, dict]:
    document = json.loads(RANKING.read_text(encoding="utf-8"))
    return {row["name"]: row for row in document["functions"]}


def closure_date(path: pathlib.Path) -> str:
    """The date the handoff's evidence last actually changed."""
    return subprocess.run(
        ["git", "log", "-1", "--format=%ad", "--date=short", "--", str(path)],
        cwd=ROOT, capture_output=True, text=True).stdout.strip()


def candidates(since: str) -> list[dict]:
    open_functions = queued()
    rows = []
    for path in sorted(HANDOFFS.glob("*.md")):
        symbol = path.stem
        row = open_functions.get(symbol)
        if row is None:
            continue                      # matched already; nothing to re-open
        date = closure_date(path)
        if not date or date >= since:
            continue                      # written with the laws already in hand
        text = path.read_text(encoding="utf-8", errors="replace")
        laws = [name for name, rx in LAW_CONSTRUCTS.items() if rx.search(text)]
        if not laws:
            continue
        masked = row.get("relocation_masked_differing_words")
        rows.append({
            "symbol": symbol,
            "size_bytes": row["size_bytes"],
            "masked_differing_words": masked,
            "size_delta": row.get("size_delta"),
            "closed": date,
            "laws": laws,
            "bytes_per_word": row["size_bytes"] / masked if masked else None,
        })
    rows.sort(key=lambda r: -(r["bytes_per_word"] or 0))
    return rows


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--since", default="2026-09-10",
                        help="closures strictly before this date are candidates "
                             "(default: the date L90-L94 landed)")
    parser.add_argument("--top", type=int, default=20)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args(argv)

    rows = candidates(args.since)
    if args.json:
        print(json.dumps({"since": args.since, "candidates": rows},
                         indent=2, sort_keys=True))
        return 0
    if not rows:
        print(f"no still-queued closures predate {args.since}")
        return 0
    print(f"{len(rows)} candidate(s), {sum(r['size_bytes'] for r in rows)} bytes, "
          f"closures before {args.since}\n")
    print("%-42s %7s %7s %6s %-11s %s" % (
        "symbol", "bytes", "masked", "delta", "closed", "laws now applicable"))
    for row in rows[:args.top]:
        print("%-42s %7d %7s %6s %-11s %s" % (
            row["symbol"][:42], row["size_bytes"],
            row["masked_differing_words"], row["size_delta"], row["closed"],
            ", ".join(law.split()[0] for law in row["laws"])))
    if len(rows) > args.top:
        print(f"\n... and {len(rows) - args.top} more (--top to widen)")
    print("\nA row is a prompt to re-read a closure, not permission to skip it.")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
