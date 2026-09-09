#!/usr/bin/env python3
"""Check that every donor-citing reopen authorization names a live donor.

A reopen authorization in `config/lane-reopen-authorizations.us.json` may
cite "new donor evidence": a permitted decomp (Jet Force Gemini, mostly)
advanced a shared-engine TU past the reference a plateau was filed against.
That is mechanism 2 of the four in ADR 0011's reopen discipline.

The claim is falsifiable, and on 2026-09-08 it was false for 26 of 83
authorizations. Twenty-six lanes were told to "re-derive from that donor"
against JFG translation units that contain no C at all: `src/fx.c` is 156
lines of which 74 are GLOBAL_ASM pragmas and *zero* are function bodies,
and `src/spranim.c` is the same shape. An agent handed that reason spends
its budget proving a negative before it can start the real work. One did.

So: a donor claim must point at a counterpart that actually has C in it.
This tool checks that, and nothing else. It does not judge whether the
specific function has a donor body -- only whether the file it would come
from exists and is more than a pragma list. That weaker check is enough to
catch the whole failure class and needs no symbol correspondence.

    python3 tools/check_donor_claims.py            # report and verdict
    python3 tools/check_donor_claims.py --list     # every claim, with counts

Exit 0 when every donor claim has a live counterpart (or the reference farm
is absent), 1 when a claim points at an empty or missing one.

THE REFERENCE FARM IS OUTSIDE THE REPO and is not a build dependency:
`~/Desktop/dev/decomp-refs/jfg`, overridable with JFG_ROOT, per
docs/CLEANROOM.md. A clone without it skips the check rather than failing,
the same way `check-fixtures` needs a baserom nobody ships. Nothing is read
out of the donor but line counts -- no names, no bodies, no instruction
text ever reaches this repository through this tool.
"""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
AUTHORIZATIONS = REPO / "config" / "lane-reopen-authorizations.us.json"
MAIN_SRC = REPO / "src" / "main"

JFG_ROOT = Path(
    os.path.expanduser(os.environ.get("JFG_ROOT", "~/Desktop/dev/decomp-refs/jfg"))
)

# A reason claims donor evidence if it *directs a lane to the donor*. Naming
# the donor is not enough: the restatements this tool provoked say "No donor
# here: JFG src/fx.c is all GLOBAL_ASM", and an earlier draft that matched a
# bare "JFG" flagged every one of those disclaimers as a claim. Anchor on the
# affirmative opener the authorizations actually use, and exclude the
# disclaimer explicitly so a restatement cannot read as a fresh claim.
#
# Prose is a poor carrier for a gate and this is the honest limit of the
# tool: a claim phrased some third way goes unchecked. The schema has no
# field for "mechanism" -- adding one means changing the validator in
# lane_status.parse_reopen_authorizations that every merge runs, which is
# not a change to make while lanes are in flight. Recorded in the workbench
# backlog instead.
DONOR_CLAIM = re.compile(r"New donor evidence|Re-derive .{0,20}from that donor", re.I)
DONOR_DISCLAIMER = re.compile(r"No donor here|donor lever is empty", re.I)


def claims_donor(reason: str) -> bool:
    """True when the reason sends a lane to a donor for a body."""
    return bool(DONOR_CLAIM.search(reason)) and not DONOR_DISCLAIMER.search(reason)


# A top-level `}` in column 0 closes a function body. Aggregate initializers
# end `};` and so do not count; verified against JFG src/camera.c (58 bodies,
# 0 pragmas) and src/fx.c (0 bodies, 74 pragmas).
BODY_LINE = re.compile(r"^\}$", re.MULTILINE)


# Translation units this project splits or renames relative to the donor.
# The basename rule below is right for the common case and wrong here, and
# refusing a *true* claim is as bad as accepting a false one: JFG keeps the
# whole front end in one `src/menu.c`, which is where every `front*` name this
# tree already carries came from. Mickey splits that code across a `menu.c`,
# a `menu_3B1A0.c` and two `frontend_*.c` files.
#
# An entry here is a claim in its own right, so it needs the same standard as
# any other: name the evidence. For these four it is the adopted symbol names
# -- frontDemoMessage, frontDrawRectangle, frontGet2PlayerSplit,
# frontGetLevelScreenMode, frontSetScreenMode -- which are JFG's own and are
# defined in its menu.c.
TU_COUNTERPARTS = {
    "src/main/frontend_37D50.c": "src/menu.c",
    "src/main/frontend_37680.c": "src/menu.c",
    "src/main/menu_3B1A0.c": "src/menu.c",
}


def counterpart(tu: Path) -> Path:
    """The donor file this TU corresponds to.

    Our src/main/<name>.c is normally the donor's src/<name>.c; TU_COUNTERPARTS
    overrides that where this project has split or renamed a unit.
    """
    try:
        key = tu.resolve().relative_to(REPO).as_posix()
    except ValueError:
        key = tu.as_posix()
    override = TU_COUNTERPARTS.get(key)
    if override:
        return JFG_ROOT / override
    return JFG_ROOT / "src" / tu.name


def donor_shape(path: Path) -> tuple[int, int] | None:
    """(function bodies, GLOBAL_ASM pragmas), or None if there is no file."""
    if not path.is_file():
        return None
    text = path.read_text(errors="replace")
    return len(BODY_LINE.findall(text)), text.count("GLOBAL_ASM")


def locate(symbols: set[str]) -> tuple[dict[str, Path], dict[str, str]]:
    """Map each symbol to the TU that *defines* it, via lane_status.

    Locating by mention rather than by definition is what made the first
    draft of this tool under-report: a symbol called from an alphabetically
    earlier TU is attributed there, and its real TU -- whose donor may be
    empty -- is never checked. `lane_status.source_identity_index` is the
    tree's existing authority for "which committed file defines this", and
    it fails closed on an ambiguous or missing definition. Reuse it rather
    than keeping a second, weaker answer to the same question.
    """
    sys.path.insert(0, str(REPO / "tools"))
    import lane_status

    resolved = lane_status.source_identity_index("HEAD", sorted(symbols))
    found: dict[str, Path] = {}
    unresolved: dict[str, str] = {}
    for symbol, (path, why) in resolved.items():
        if path is None:
            unresolved[symbol] = why or "no committed definition"
        else:
            found[symbol] = REPO / path
    return found, unresolved


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--list", action="store_true", help="print every claim, not just failures"
    )
    args = parser.parse_args()

    document = json.loads(AUTHORIZATIONS.read_text())
    authorizations = document["authorizations"]
    claims = {
        symbol
        for symbol, row in authorizations.items()
        if claims_donor(row.get("reason", ""))
    }
    if not claims:
        print("donor claims: none to check")
        return 0

    if not JFG_ROOT.is_dir():
        print(
            f"donor claims: {len(claims)} unchecked -- no reference farm at "
            f"$JFG_ROOT (see docs/CLEANROOM.md); skipping, not failing"
        )
        return 0

    where, unresolved = locate(claims)
    # Group by TU: the check is per translation unit, so report it that way.
    by_tu: dict[Path, list[str]] = {}
    for symbol in sorted(claims):
        tu = where.get(symbol)
        if tu is not None:
            by_tu.setdefault(tu, []).append(symbol)

    dead: list[tuple[Path, list[str], str]] = []
    for tu, symbols in sorted(by_tu.items()):
        shape = donor_shape(counterpart(tu))
        if shape is None:
            dead.append((tu, symbols, "donor has no counterpart file"))
        elif shape[0] == 0:
            dead.append(
                (tu, symbols, f"donor counterpart is all asm ({shape[1]} pragmas)")
            )
        elif args.list:
            print(
                f"OK    {tu.relative_to(REPO)}: {len(symbols)} claim(s), "
                f"donor has {shape[0]} bodies / {shape[1]} pragmas"
            )

    # A claim whose symbol has no single committed definition is not
    # "passing" -- it is unchecked, and saying so is the whole point of a
    # fail-closed gate. Report it, but do not fail on it: an unresolvable
    # definition is a different defect with a different owner.
    for symbol, why in sorted(unresolved.items()):
        print(f"note  {symbol}: donor claim unchecked -- {why}", file=sys.stderr)

    if not dead:
        print(
            f"donor claims OK -- {len(claims) - len(unresolved)} checked "
            f"across {len(by_tu)} TU(s), every donor counterpart has C "
            f"bodies; {len(unresolved)} unchecked"
        )
        return 0

    stranded = sum(len(symbols) for _, symbols, _ in dead)
    print(
        f"donor claims: {stranded} of {len(claims)} cite a donor that "
        f"cannot supply anything\n",
        file=sys.stderr,
    )
    for tu, symbols, why in dead:
        print(f"  {tu.relative_to(REPO)}: {why}", file=sys.stderr)
        print(f"    {len(symbols)} authorization(s): {', '.join(symbols)}", file=sys.stderr)
    print(
        "\nRestate these under a mechanism that is true for them (ADR 0018's\n"
        "stall rule, or decompiler-assisted reconstruction), or drop them.\n"
        "A lane told to consult an empty donor spends its budget proving a\n"
        "negative. See docs/references.md.",
        file=sys.stderr,
    )
    return 1


if __name__ == "__main__":
    sys.exit(main())
