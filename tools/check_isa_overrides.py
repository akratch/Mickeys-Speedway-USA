#!/usr/bin/env python3
"""Fail a per-object ISA override the target's own instructions contradict.

A per-file `MIPSISET := -mips1` override is a claim about how the shipped
code was built. It is falsifiable: IDO cannot emit a branch-likely
instruction (`beql`, `bnel`, `bc1tl`, ...) below `-mips2`, so a target that
contains one cannot have been compiled at `-mips1`, whatever a flag sweep
scored.

This is not hypothetical. `src/overlays/o058/func_overlay_058_F000138C_...`
carried a `-mips1 -32` override adopted on its first draft by a *positional*
flag sweep -- the metric that renumbers every row after an insertion, and
which lever 48 exists to warn about. Its target holds 38 branch-likely
instructions. The wrong flag manufactured 532 bytes of surplus and 180 nops
against the target's 38, and every later lane inherited that phantom as the
function's defining problem. Removing the override took the size delta from
+532 to +8 bytes and closed the frame in one step.

    python3 tools/check_isa_overrides.py

Exit 1 when an override contradicts its target, 0 otherwise. Reads splat's
disassembly, which is gitignored, and reports counts and symbol names only --
never instruction text. Skips rather than fails when `asm/` is absent, as
`check-fixtures` does without a baserom.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
ASM = REPO / "asm" / "nonmatchings"
MAKEFILES = [REPO / "Makefile"] + sorted((REPO / "mk").glob("*.mk"))

OVERRIDE = re.compile(
    r"^\$\(BUILD_DIR\)/\$\(SRC_DIR\)/(?P<obj>\S+)\.c\.o:\s*MIPSISET\s*:?=\s*(?P<isa>-mips\d)"
)
ROW = re.compile(r"^\s*/\*.*?\*/\s+(\S+)")

# Emitted only at -mips2 and above. Two families, both fatal to a -mips1
# claim, and the second was added after it let a real one through.
BRANCH_LIKELY = {
    "beql", "bnel", "blezl", "bgtzl", "bltzl", "bgezl",
    "bltzall", "bgezall", "bc1tl", "bc1fl",
}

# Float-to-integer conversion with an explicit rounding mode, and square root.
# MIPS I has no such instruction: at -mips1 IDO converts by saving the FPU
# control word, forcing a rounding mode through it, converting, and restoring
# -- roughly eleven words where -mips2 spends three. So a target holding one of
# these was not built at -mips1, exactly as a branch-likely target was not.
FP_MIPS2_ONLY = {
    "trunc.w.s", "trunc.w.d", "round.w.s", "round.w.d",
    "ceil.w.s", "ceil.w.d", "floor.w.s", "floor.w.d",
    "sqrt.s", "sqrt.d",
}

MIPS2_ONLY = BRANCH_LIKELY | FP_MIPS2_ONLY


def overrides() -> list[tuple[str, str, Path]]:
    found = []
    for path in MAKEFILES:
        if not path.is_file():
            continue
        for line in path.read_text(errors="replace").splitlines():
            hit = OVERRIDE.match(line)
            if hit and "%" not in hit.group("obj"):
                found.append((hit.group("obj"), hit.group("isa"), path))
    return found


def mips2_only_count(symbol: str) -> tuple[int, int] | None:
    """(branch-likely, fp-conversion) counts in the target, or None if absent."""
    matches = list(ASM.rglob(f"{symbol}.s"))
    if not matches:
        return None
    branch = fp = 0
    for path in matches:
        for line in path.read_text(errors="replace").splitlines():
            row = ROW.match(line)
            if not row:
                continue
            mnemonic = row.group(1)
            if mnemonic in BRANCH_LIKELY:
                branch += 1
            elif mnemonic in FP_MIPS2_ONLY:
                fp += 1
    return branch, fp


def main() -> int:
    if not ASM.is_dir():
        print("no asm/ -- run `gmake extract` first; skipping, not failing")
        return 0

    contradicted = []
    checked = 0
    for obj, isa, makefile in overrides():
        if isa != "-mips1":
            continue
        symbol = Path(obj).name
        counts = mips2_only_count(symbol)
        if counts is None:
            continue
        branch, fp = counts
        checked += 1
        if branch or fp:
            contradicted.append((obj, branch, fp, makefile.name))

    if not contradicted:
        print(f"ISA overrides OK -- {checked} -mips1 override(s) checked "
              f"against their targets")
        return 0

    print("a -mips1 override contradicts its own target:", file=sys.stderr)
    for obj, branch, fp, makefile in contradicted:
        carried = []
        if branch:
            carried.append(f"{branch} branch-likely")
        if fp:
            carried.append(f"{fp} rounding-mode float conversion")
        print(
            f"  {obj}: target carries {' and '.join(carried)} instruction(s), "
            f"which IDO cannot emit below -mips2 ({makefile})",
            file=sys.stderr,
        )
    print(
        "\nDrop the override so the file takes its group default, rebuild, and "
        "re-measure.\nA flag chosen by a positional differing-word score can be "
        "wrong in exactly this way;\nsee lever 48 in the workbench field guide.",
        file=sys.stderr,
    )
    return 1


if __name__ == "__main__":
    sys.exit(main())
