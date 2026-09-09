#!/usr/bin/env python3
"""Print the target .s a friendly-named candidate is paired with.

A candidate that has been given a real name keeps its auto-named assembly
fallback, so the target's basename no longer matches the C function's:

    #ifdef NON_MATCHING
    void overlay1FindBestRecord(...) { ... }
    #else
    #pragma GLOBAL_ASM(".../func_overlay_001_F0004F70_1853350.s")
    #endif

The pragma inside that function's own guard IS its target, whatever else the
translation unit contains. `tools/permute.sh` previously resolved this by
requiring the whole unit to name exactly one fallback, which gives up on every
multi-function overlay unit -- `overlay_001_tail.c` has twelve -- and left the
permuter unusable on them. A lane worked around it by copying the assembly
under a renamed path.

    python3 tools/resolve_target_asm.py <function> <c file>

Prints the pragma path, or nothing (exit 1) when the pairing is not decidable.
Reads source only; the assembly itself is never opened, so nothing ROM-derived
passes through here.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path


def conditional_blocks(lines: list[str]) -> list[tuple[int, int]]:
    """Every `#if...#endif` span, as (start, end) line indices.

    Depth is tracked rather than assumed: a nested `#if` inside a candidate's
    body must not be mistaken for the candidate's own guard.
    """
    blocks: list[tuple[int, int]] = []
    stack: list[int] = []
    for number, line in enumerate(lines):
        stripped = line.lstrip()
        if stripped.startswith("#if"):
            stack.append(number)
        elif stripped.startswith("#endif") and stack:
            blocks.append((stack.pop(), number))
    return blocks


def paired_target(text: str, symbol: str) -> str | None:
    """The GLOBAL_ASM path in the same guard as this symbol's definition."""
    lines = text.splitlines()
    definition = re.compile(r"^[A-Za-z_].*\b" + re.escape(symbol) + r"\s*\(")
    pragma = re.compile(r'^\s*#pragma GLOBAL_ASM\("([^"]*)"\)')

    found = next(
        (n for n, line in enumerate(lines) if definition.match(line)), None
    )
    if found is None:
        return None

    # The innermost enclosing block is the candidate's own guard.
    enclosing = [
        (start, end) for start, end in conditional_blocks(lines)
        if start < found < end
    ]
    if not enclosing:
        return None
    start, end = max(enclosing, key=lambda pair: pair[0])

    for line in lines[start:end]:
        hit = pragma.match(line)
        if hit:
            return hit.group(1)
    return None


def main() -> int:
    if len(sys.argv) != 3:
        print(f"usage: {sys.argv[0]} <function> <c file>", file=sys.stderr)
        return 2
    symbol, path = sys.argv[1], Path(sys.argv[2])
    target = paired_target(path.read_text(errors="replace"), symbol)
    if target is None:
        return 1
    print(target)
    return 0


if __name__ == "__main__":
    sys.exit(main())
