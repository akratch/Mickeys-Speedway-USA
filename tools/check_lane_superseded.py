#!/usr/bin/env python3
"""Report whether a lane branch has been overtaken before merging it.

    tools/check_lane_superseded.py <lane-branch> [--base REF]

Merging a superseded lane is not cheap. Each attempt costs a full build and
gate cycle, and because the lane's edits and the work that overtook them sit
in different regions, the merge usually reports no conflict and produces a
tree that fails several gates later for reasons that read like defects in the
lane's own work. Two lanes on 2026-09-10 cost two such cycles between them:
one had solved a pair of functions a different way than the version already
landed and matched a third that was also already matched, and the other
carried a copy of a shared tool half the size of the current one, with its
headline feature already upstream.

Both were detectable in seconds without building anything:

  matches   every function the lane's unmerged commits claim to match, which
            no longer has a #pragma GLOBAL_ASM fallback on the base -- that
            work has landed, by this lane or another route
  tools     every shared file the lane touches whose copy on the base has
            grown substantially since the lane's merge-base, which means the
            lane is editing a version that has moved on

Exit status is 0 when nothing is superseded, 1 when something is. This is
advice, not a verdict: a superseded lane can still carry documentation or a
falsification worth keeping, and the caller decides. It never edits anything.
"""
from __future__ import annotations

import argparse
import pathlib
import re
import subprocess
import sys

SYMBOL_RE = re.compile(r"\b((?:func_|overlay)[A-Za-z0-9_]+)")
# A lane's copy being this much smaller than the base's means the base has
# moved substantially, not that the lane trimmed a few lines.
SHRINK_RATIO = 0.75


def git(*args: str) -> str:
    return subprocess.run(["git", *args], capture_output=True, text=True).stdout


def worked_symbols(branch: str, base: str) -> dict[str, str]:
    """Symbols named by the lane's unmerged commit subjects, to the first sha.

    Reads every subject rather than only those starting "Match"/"Promote".
    Lanes routinely lead a subject with the symbol itself -- one that cost a
    wasted merge cycle was titled "func_X and func_Y: the reservation is a
    spilltemps temporary" -- and a narrower pattern silently found nothing.
    """
    worked: dict[str, str] = {}
    for line in git("log", "--format=%H\x01%s", f"{base}..{branch}").splitlines():
        if "\x01" not in line:
            continue
        sha, subject = line.split("\x01", 1)
        for symbol in SYMBOL_RE.findall(subject):
            worked.setdefault(symbol, sha[:8])
    return worked


def live_queue() -> set[str]:
    """The functions still behind #ifdef NON_MATCHING, from the project's own
    discovery.

    Do NOT re-derive this by grepping for a GLOBAL_ASM pragma named after the
    symbol. An overlay function carrying a friendly name has its fallback
    written under the GENERATED name -- overlay1UpdateRangeFlags is queued
    behind a pragma naming func_overlay_001_F...s -- so a name-matched grep
    finds nothing and reports every such function as already landed. That is
    precisely the class this check exists to reason about.
    """
    sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
    import permute_batch as pb
    return {item.func for item in pb.discover_queue()}


def shrunk_shared_files(branch: str, base: str) -> list[tuple[str, int, int]]:
    """Files the lane touches whose base copy has grown well past the lane's."""
    merge_base = git("merge-base", base, branch).strip()
    if not merge_base:
        return []
    changed = [p for p in git("diff", "--name-only", f"{merge_base}..{branch}").splitlines()
               if p.endswith((".py", ".sh", ".mk")) or p == "Makefile"]
    grown = []
    for path in changed:
        lane = git("show", f"{branch}:{path}").count("\n")
        head = git("show", f"{base}:{path}").count("\n")
        if head and lane and lane < head * SHRINK_RATIO:
            grown.append((path, lane, head))
    return grown


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("branch")
    parser.add_argument("--base", default="HEAD")
    args = parser.parse_args(argv)

    queued = live_queue()
    landed = {symbol: sha for symbol, sha in worked_symbols(args.branch, args.base).items()
              if symbol not in queued}
    grown = shrunk_shared_files(args.branch, args.base)

    if not landed and not grown:
        print(f"{args.branch}: nothing superseded")
        return 0
    if landed:
        print(f"{args.branch}: {len(landed)} function(s) it worked on are already matched:")
        for symbol, sha in sorted(landed.items()):
            print(f"  {symbol} (worked by {sha}; no longer in the NON_MATCHING queue)")
    if grown:
        print(f"{args.branch}: {len(grown)} shared file(s) have moved on since this lane forked:")
        for path, lane, head in grown:
            print(f"  {path}: lane has {lane} lines, {args.base} has {head}")
    print("This lane may be superseded. Its documentation or falsifications may "
          "still be worth keeping; its code changes probably are not.")
    return 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
