#!/usr/bin/env python3
"""Decide each TU's multiply-scheduler flag from the ROM's own bytes.

IDO under `-Wab,-r4300_mul` separates two adjacent single-precision
multiplies with a scheduler nop. Without the flag it cannot emit one there
at all. So the flag is not a thing to search for by compiling: it is
*readable off the target*, before a line of source is written.

Two translation units reached that conclusion independently on 2026-09-09,
each after a day of hand permutation against the wrong scheduler --
`src/main/anim.c`, where the flag settled a two-word tie about twenty source
rewrites could not reach, and `src/main/shadows.c`, whose targets carry
fourteen such nops across three functions. Only one `src/main` TU carried
the flag before today, so every other unit's candidates had never been
measured against the right scheduler, and their plateaus record a defect
that is partly the build's.

This scan costs no compilation. It reads the disassembly splat already
emits for unmatched functions and reports, per TU:

    pairs   adjacent single-precision multiply pairs in the target
    nops    how many of those pairs are separated by a scheduler nop

A TU with nops>0 needs the flag. A TU with pairs>0 and nops==0 must not
have it. A TU with pairs==0 carries no evidence either way and is left
alone. Disagreements with the Makefile are what the exit code reports.

    python3 tools/mul_scheduler_scan.py              # disagreements only
    python3 tools/mul_scheduler_scan.py --all        # every TU with evidence

CLEAN ROOM: `asm/` is splat output, gitignored and never committed. This
tool reads it at run time and emits counts, TU names and flag verdicts --
never instruction text, and nothing it prints is written back into the
tree. It is the same posture as `check-fixtures`, which needs a baserom
nobody ships.
"""

from __future__ import annotations

import argparse
import re
import sys
from collections import defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
ASM = REPO / "asm" / "nonmatchings"
MAKEFILE = REPO / "Makefile"

FLAG = "-Wab,-r4300_mul"

# splat's rows are `/* ROM VRAM WORD */  mnemonic operands`. Take the
# mnemonic only; operands are never needed here and never printed.
ROW = re.compile(r"^\s*/\*.*?\*/\s+(\S+)")

# The delay-slot rows splat indents by one extra space are still ordinary
# instructions for this purpose; what matters is only that a nop sitting
# between two multiplies is not itself a delay slot. It cannot be: the
# instruction before it is a multiply, which never has one.
MULTIPLY = "mul.s"
NOP = "nop"


def mnemonics(path: Path) -> list[str]:
    out = []
    for line in path.read_text(errors="replace").splitlines():
        found = ROW.match(line)
        if found:
            out.append(found.group(1))
    return out


def scan(path: Path) -> tuple[int, int]:
    """(adjacent multiply pairs, pairs separated by a scheduler nop)."""
    ops = mnemonics(path)
    pairs = nops = 0
    for i in range(len(ops) - 1):
        if ops[i] != MULTIPLY:
            continue
        if ops[i + 1] == MULTIPLY:
            pairs += 1
        elif (
            i + 2 < len(ops) and ops[i + 1] == NOP and ops[i + 2] == MULTIPLY
        ):
            pairs += 1
            nops += 1
    return pairs, nops


def translation_unit(path: Path) -> str:
    """asm/nonmatchings/<group>/<tu>/<fn>.s -> src/<group>/<tu>.c"""
    rel = path.relative_to(ASM)
    parts = rel.parts
    if len(parts) < 3:
        return str(rel.parent)
    return f"src/{parts[0]}/{'/'.join(parts[1:-1])}.c"


def makefile_flagged() -> set[str]:
    """TUs the build already compiles with the flag."""
    flagged = set()
    for line in MAKEFILE.read_text().splitlines():
        if FLAG not in line or not line.startswith("$(BUILD_DIR)"):
            continue
        found = re.search(r"\$\(SRC_DIR\)/(\S+?)\.c\.o", line)
        if found:
            flagged.add(f"src/{found.group(1)}.c")
    return flagged


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--all", action="store_true", help="every TU with evidence")
    args = parser.parse_args()

    if not ASM.is_dir():
        print("no asm/ -- run `gmake extract` first; skipping, not failing")
        return 0

    totals: dict[str, list[int]] = defaultdict(lambda: [0, 0])
    for path in sorted(ASM.rglob("*.s")):
        pairs, nops = scan(path)
        if pairs:
            tu = translation_unit(path)
            totals[tu][0] += pairs
            totals[tu][1] += nops

    flagged = makefile_flagged()
    wants, forbids, agrees = [], [], []
    for tu, (pairs, nops) in sorted(totals.items()):
        has = tu in flagged
        if nops and not has:
            wants.append((tu, pairs, nops))
        elif not nops and has:
            forbids.append((tu, pairs, nops))
        else:
            agrees.append((tu, pairs, nops, has))

    if wants:
        print(f"NEEDS {FLAG} -- target carries scheduler nops, build does not:")
        for tu, pairs, nops in wants:
            print(f"  {tu:44s} {pairs:4d} pairs  {nops:4d} nops")
    if forbids:
        print(f"\nMUST NOT have {FLAG} -- build sets it, target has no nop:")
        for tu, pairs, nops in forbids:
            print(f"  {tu:44s} {pairs:4d} pairs  {nops:4d} nops")
    if args.all and agrees:
        print("\nagrees with the build:")
        for tu, pairs, nops, has in agrees:
            print(
                f"  {tu:44s} {pairs:4d} pairs  {nops:4d} nops  "
                f"{'flagged' if has else 'unflagged'}"
            )

    scanned = len(totals)
    if not wants and not forbids:
        print(f"\nmultiply scheduler OK -- {scanned} TU(s) with evidence agree")
        return 0
    print(
        f"\n{len(wants) + len(forbids)} of {scanned} TU(s) with evidence "
        f"disagree with the build.\nEach one's candidates were measured "
        f"against the wrong scheduler, so its recorded\nplateaus overstate "
        f"the source defect. Set the flag per TU in the Makefile,\nconfirm "
        f"`gmake verify` still passes, then re-measure."
    )
    return 1


if __name__ == "__main__":
    sys.exit(main())
