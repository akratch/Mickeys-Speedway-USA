#!/usr/bin/env python3
"""Check a planned lane fan-out, and emit the facts each brief should carry.

    tools/dispatch_check.py <lane>=<sym>[,<sym>...] [<lane>=<sym>...]
    tools/dispatch_check.py --plan plan.json

Two jobs, both aimed at waste that has actually happened rather than waste
that might.

**Refuse an unsound plan.** A symbol assigned to two lanes gets worked twice
and merged twice, and the second merge is the one that conflicts. A symbol that
is no longer queued gets a lane spent re-deriving a match that already landed.
Both occurred while planning campaign 2; both were caught by hand, which is not
a control.

**Emit the measured facts.** Every brief so far was hand-assembled from the
ranking and the handoff shards, and that is where the errors came from: two
lanes were told to spend their first phase partitioning relocation artifacts
out of a residual, when the artifact count was already stored and turned out to
be 0.4% of the number. A brief that carries measured values does not invite a
lane to re-derive them, and a lane that is handed `masked=1834, artifact=7`
does not spend a phase discovering `7`.

Exit status is 0 when the plan is sound, 1 when it is not.
"""
from __future__ import annotations

import argparse
import collections
import json
import pathlib
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

RANKING = pathlib.Path(__file__).resolve().parents[1] / "config" / "nonmatching-ranking.us.json"


def queued_rows() -> dict[str, dict]:
    document = json.loads(RANKING.read_text(encoding="utf-8"))
    return {row["name"]: row for row in document["functions"]}


def closure_facts() -> dict[str, dict]:
    """Closure date and newly-applicable laws, from the re-open lister."""
    try:
        import reopen_candidates as rc
        return {row["symbol"]: row for row in rc.candidates(since="2026-09-10")}
    except Exception:
        return {}


def parse_plan(args: list[str]) -> dict[str, list[str]]:
    plan: dict[str, list[str]] = {}
    for item in args:
        if "=" not in item:
            raise ValueError(f"expected <lane>=<symbols>, got {item!r}")
        lane, symbols = item.split("=", 1)
        plan.setdefault(lane.strip(), []).extend(
            s.strip() for s in symbols.split(",") if s.strip())
    return plan


def check(plan: dict[str, list[str]]) -> tuple[list[str], list[str]]:
    rows, closures = queued_rows(), closure_facts()
    problems, notes = [], []

    # Distinct lanes, not occurrences: a symbol listed twice inside one lane is
    # sloppy input, not a planning fault, and refusing on it would make the
    # message ("assigned to 2 lanes") untrue. A guard that says something false
    # gets ignored, which costs more than the case it was catching.
    owner: dict[str, set[str]] = collections.defaultdict(set)
    for lane, symbols in plan.items():
        for symbol in symbols:
            owner[symbol].add(lane)

    for symbol, lanes in sorted(owner.items()):
        if len(lanes) > 1:
            problems.append(
                f"{symbol} is assigned to {len(lanes)} lanes ({', '.join(sorted(lanes))}). "
                f"It will be worked twice and the second merge will conflict.")
        if symbol not in rows:
            problems.append(
                f"{symbol} is not in the NON_MATCHING queue: already matched, or misspelled. "
                f"A lane pointed at it re-derives a landed match.")

    for lane, symbols in sorted(plan.items()):
        notes.append(f"\n=== {lane} ===")
        for symbol in symbols:
            row = rows.get(symbol)
            if row is None:
                notes.append(f"  {symbol}: NOT QUEUED")
                continue
            raw = row.get("differing_words")
            masked = row.get("relocation_masked_differing_words")
            artifact = (raw - masked) if (raw is not None and masked is not None) else None
            line = (f"  {symbol}: {row['size_bytes']} B, masked={masked}, "
                    f"artifact={artifact}, delta={row.get('size_delta')}, "
                    f"class={row.get('category')}")
            closure = closures.get(symbol)
            if closure:
                line += (f"\n      closure predates the current laws "
                         f"({closure['closed']}); newly applicable: "
                         f"{', '.join(law.split()[0] for law in closure['laws'])}")
            notes.append(line)
    return problems, notes


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("assignments", nargs="*")
    parser.add_argument("--plan", type=pathlib.Path,
                        help="JSON object mapping lane name to a list of symbols")
    args = parser.parse_args(argv)

    if args.plan:
        plan = {k: list(v) for k, v in json.loads(args.plan.read_text()).items()}
    else:
        try:
            plan = parse_plan(args.assignments)
        except ValueError as error:
            parser.error(str(error))
    if not plan:
        parser.error("no assignments given")

    problems, notes = check(plan)
    print("\n".join(notes).lstrip("\n"))
    if problems:
        print("\nPLAN REFUSED:", file=sys.stderr)
        for problem in problems:
            print(f"  {problem}", file=sys.stderr)
        return 1
    print(f"\nplan sound: {sum(len(v) for v in plan.values())} symbol(s) "
          f"across {len(plan)} lane(s), no overlap, all queued")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
