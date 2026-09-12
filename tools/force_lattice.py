#!/usr/bin/env python3
"""Search COMBINATIONS of allocator forces, not one force at a time.

    tools/force_lattice.py <symbol> --proc N \
        --force p1:w27=c4 --force p1:w75=c3 [...] [--greedy|--subsets]

WHY THIS EXISTS.

Every force receipt this campaign has taken on a large function was measured
ALONE. On the tree's biggest function that produced five separately accepted
colours, each improving the score in isolation:

    web 27 -> s3   227 -> 224      web 379 -> s6  227 -> 217
    web 75 -> s2   227 -> 212      web 956 -> s5  227 -> 220
    web 225 -> s0  227 -> 220

Seven passes recorded those singly and none measured them together, while the
handoff hedged that they "are global effects and are not additive credits".
That is a reason to MEASURE the combination, not to assume it.

The campaign already has the counterexample that makes this urgent: L100's
receipt is a function that went 21 -> 5 on a PAIR of edits where *each edit
alone was a 31-word regression*. Measuring one decision at a time does not
merely under-report a combination -- it can invert its sign. A lattice search
is the only honest way to price a set of interacting decisions.

WHAT IT REPORTS, and why each part matters.

* the singles, re-measured here, so an inherited receipt is confirmed rather
  than trusted -- a force taken on an older source may not reproduce;
* the best combination found, with the subset that reaches it;
* the INTERACTION of each pair: additive, synergistic, or antagonistic.
  Antagonism is the interesting verdict. Two colours that each help alone but
  hurt together are competing for one register, which says the target's
  assignment is not the union of the individual wins and names the contest.

ACCEPTANCE IS CHECKED, NOT ASSUMED. `CDX_FORCE` is silently ignored unless
`CDX_PROC` is also set, recording `forced=-2` and returning a byte-identical
object that is indistinguishable from a legitimate decline (Trap 20). Every
cell here reads the forced value back out of the records and refuses to score
a cell whose forces did not all apply.

THE OBJECT IS SCORED DIRECTLY. score_symbol/align_symbol/frame_census all
recompile the TU with the *configured* command before measuring, which
overwrites the forced object and reports the unforced build -- one lane read
the same number for twelve consecutive forces before noticing. This reads the
forced object's own text.
"""
from __future__ import annotations

import argparse
import dataclasses
import itertools
import os
import pathlib
import re
import subprocess
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

ROOT = pathlib.Path(__file__).resolve().parents[1]
INSTRUMENTED = pathlib.Path.home() / "Desktop" / "dev" / "ido-instrumented"
FORCE_RE = re.compile(r"^p[12]:w\d+=c\d+$")


@dataclasses.dataclass(frozen=True)
class Cell:
    """One measured point: a set of forces and what it scored."""

    forces: tuple[str, ...]
    score: int | None
    accepted: bool
    note: str = ""

    @property
    def label(self) -> str:
        return "+".join(f.split(":", 1)[1] for f in self.forces) or "(base)"


def validate_forces(forces: list[str]) -> None:
    """Refuse a malformed spec before spending a compile on it."""
    for force in forces:
        if not FORCE_RE.match(force):
            raise SystemExit(
                f"force {force!r} is not of the form p1:wN=cM. "
                f"A malformed spec is silently ignored by the compiler, which "
                f"reads as a declined force and proves nothing."
            )
    webs = [f.split("=")[0] for f in forces]
    duplicated = {w for w in webs if webs.count(w) > 1}
    if duplicated:
        raise SystemExit(
            f"web(s) {', '.join(sorted(duplicated))} appear twice with different "
            f"colours; the compiler takes one and the result would be attributed "
            f"to both."
        )


def subsets(forces: list[str], max_size: int | None = None) -> list[tuple[str, ...]]:
    """Every non-empty subset, smallest first.

    Smallest first so the singles are measured before any combination: a single
    that fails to reproduce its inherited receipt invalidates every combination
    containing it, and finding that out first saves the rest of the sweep.
    """
    limit = len(forces) if max_size is None else min(max_size, len(forces))
    out: list[tuple[str, ...]] = []
    for size in range(1, limit + 1):
        out.extend(itertools.combinations(forces, size))
    return out


def interaction(pair_score: int, a_score: int, b_score: int, base: int) -> str:
    """Classify how two forces combine, relative to acting independently.

    `expected` is what the pair would score if the two were independent: each
    contributes its own delta from the base. The verdict is about the gap
    between that and what the pair actually scored.
    """
    expected = base - ((base - a_score) + (base - b_score))
    if pair_score < expected:
        return "SYNERGISTIC"
    if pair_score > expected:
        return "ANTAGONISTIC"
    return "additive"


def compile_command(symbol: str) -> list[str]:
    """The configured command for the symbol's TU, with the compiler swapped.

    Derived from the build rather than retyped: a hand-written line that drops
    a per-file flag makes BOTH sides of an identity gate wrong in the same way,
    so they agree with each other and disagree with the tree (one such slip
    read 33 against the configured 31).
    """
    import nm_ranking as nr
    import permute_batch as pb

    items = [i for i in pb.queue_items() if i.func == symbol]
    if not items:
        raise SystemExit(f"{symbol} is not in the NON_MATCHING queue")
    commands = nr.configured_compile_commands(items)
    command = commands.get(symbol)
    if not command:
        raise SystemExit(f"no configured compile command for {symbol}")
    return list(command)


def run_cell(symbol: str, proc: int, forces: tuple[str, ...],
             dry_run: bool = False) -> Cell:
    """Compile under `forces`, verify they applied, and score the object."""
    if dry_run:
        return Cell(forces, None, True, "dry run")
    env = dict(os.environ)
    if forces:
        env["CDX_FORCE"] = ",".join(forces)
    env["CDX_PROC"] = str(proc)
    env["CDX_OUT"] = str(ROOT / "build" / "force_lattice.log")
    try:
        command = compile_command(symbol)
    except SystemExit as exc:
        return Cell(forces, None, False, str(exc))
    command = [str(INSTRUMENTED / "cc") if i == 0 else a
               for i, a in enumerate(command)]
    result = subprocess.run(command, env=env, capture_output=True, text=True,
                            cwd=ROOT)
    if result.returncode != 0:
        return Cell(forces, None, False, f"compile failed: {result.stderr[:120]}")
    log = pathlib.Path(env["CDX_OUT"])
    text = log.read_text(errors="replace") if log.exists() else ""
    if "forced=-2" in text:
        return Cell(forces, None, False,
                    "a force was NOT applied (forced=-2); the object is "
                    "byte-identical to the unforced build and proves nothing")
    return Cell(forces, _score(symbol), True)


def _score(symbol: str) -> int | None:
    """Masked differing words for the object currently on disk."""
    import nm_ranking as nr
    import permute_batch as pb

    items = [i for i in pb.queue_items() if i.func == symbol]
    if not items:
        return None
    item = items[0]
    base_o = ROOT / "build" / pb.object_for(item) if hasattr(pb, "object_for") else None
    if base_o is None or not base_o.exists():
        return None
    result, _ = nr.process_item(item, base_o)
    return None if result is None else result.relocation_masked_differing_words


def render(base: Cell, cells: list[Cell], forces: list[str]) -> str:
    out = [f"base (no force): {base.score}", ""]
    singles = {c.forces[0]: c for c in cells if len(c.forces) == 1}
    out.append("singles:")
    for force in forces:
        cell = singles.get(force)
        if cell is None or cell.score is None:
            out.append(f"  {force:<16} UNMEASURED  {cell.note if cell else ''}")
        else:
            delta = (base.score - cell.score) if base.score is not None else 0
            out.append(f"  {force:<16} {cell.score:>5}   {delta:+d}")
    scored = [c for c in cells if c.score is not None]
    if scored:
        best = min(scored, key=lambda c: c.score)
        out += ["", f"BEST: {best.score} with {best.label}"]
        if base.score is not None:
            out.append(f"      {base.score - best.score:+d} against the base")
    if base.score is not None and len(forces) > 1:
        out += ["", "pairwise interaction:"]
        by = {c.forces: c for c in cells if c.score is not None}
        for a, b in itertools.combinations(forces, 2):
            pair = by.get((a, b)) or by.get((b, a))
            ca, cb = singles.get(a), singles.get(b)
            if not (pair and ca and cb and ca.score and cb.score):
                continue
            verdict = interaction(pair.score, ca.score, cb.score, base.score)
            out.append(f"  {a.split(':')[1]:<12} + {b.split(':')[1]:<12} "
                       f"{pair.score:>5}  {verdict}")
    return "\n".join(out)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("symbol")
    parser.add_argument("--proc", type=int, required=True,
                        help="procedure ordinal; CDX_PROC must be an ordinal, "
                             "obtained by first running with a non-numeric "
                             "value to print the procindex table")
    parser.add_argument("--force", action="append", default=[], dest="forces",
                        help="p1:wN=cM, repeatable")
    parser.add_argument("--max-size", type=int, default=None,
                        help="largest subset to measure (default: all)")
    parser.add_argument("--dry-run", action="store_true",
                        help="list the cells without compiling")
    args = parser.parse_args(argv)

    if not args.forces:
        parser.error("give at least one --force")
    validate_forces(args.forces)

    plan = subsets(args.forces, args.max_size)
    if args.dry_run:
        print(f"{len(plan)} cell(s) would be measured, smallest first:")
        for forces in plan:
            print("  " + "+".join(f.split(":", 1)[1] for f in forces))
        return 0

    base = run_cell(args.symbol, args.proc, ())
    cells = [run_cell(args.symbol, args.proc, forces) for forces in plan]
    print(render(base, cells, args.forces))
    refused = [c for c in cells if not c.accepted]
    if refused:
        print(f"\n{len(refused)} cell(s) did not apply and were not scored:",
              file=sys.stderr)
        for cell in refused[:5]:
            print(f"  {cell.label}: {cell.note}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
