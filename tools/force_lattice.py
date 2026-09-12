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
    # Differing words per 0x200 window, keyed by window base. This is what
    # makes an interaction explicable rather than merely observed: two forces
    # whose blast radii are disjoint should compose, and two that move the same
    # window are contending for something in it.
    windows: dict[int, int] = dataclasses.field(default_factory=dict)

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


def _queue_item(symbol: str):
    """The queue entry for `symbol`, or a SystemExit naming why there is none."""
    import permute_batch as pb

    queue = {item.func: item for item in pb.discover_queue()}
    item = queue.get(symbol)
    if item is None:
        raise SystemExit(
            f"{symbol} is not in the NON_MATCHING queue: already matched, or "
            f"misspelled."
        )
    return item


def _instrumented(command: list[str]) -> list[str]:
    """`command` with the IDO driver swapped for the instrumented build.

    The compiler is located by matching the path rather than assuming argv[0]:
    this project's configured command runs the compile through asm-processor,
    so the driver is NOT the first word. Swapping argv[0] would replace the
    wrapper and silently compile with the stock compiler -- which reads as
    every force being declined.
    """
    out, swapped = [], False
    for arg in command:
        if not swapped and arg.endswith("ido/cc"):
            out.append(str(INSTRUMENTED / "cc"))
            swapped = True
        else:
            out.append(arg)
    if not swapped:
        raise SystemExit(
            "could not find the IDO driver in the configured command; "
            "refusing to guess, because guessing compiles with the stock "
            "compiler and every force then reads as declined."
        )
    return out


def run_cell(symbol: str, proc: int, forces: tuple[str, ...],
             dry_run: bool = False) -> Cell:
    """Compile under `forces`, verify they applied, and score the object."""
    if dry_run:
        return Cell(forces, None, True, "dry run")

    import nm_ranking as nr

    try:
        item = _queue_item(symbol)
        commands = nr.configured_compile_commands([item])
        command = commands.get(item.rel_c_file)
        if not command:
            return Cell(forces, None, False,
                        f"no configured compile command for {item.rel_c_file}")
        command = _instrumented(list(command))
    except SystemExit as exc:
        return Cell(forces, None, False, str(exc))

    work = ROOT / "build" / "force_lattice"
    work.mkdir(parents=True, exist_ok=True)
    obj = work / "forced.o"
    obj.unlink(missing_ok=True)
    log = work / "cdx.log"
    log.unlink(missing_ok=True)

    # Redirect only the object, exactly as compile_configured_tu does. The
    # source path is kept: its quoted includes, __FILE__ and __LINE__ are
    # compiler inputs. We do not call compile_configured_tu itself because it
    # warns that a forced object is about to be discarded -- true for the
    # measuring tools, and precisely backwards here, where the forced object is
    # the thing being measured.
    actual = list(command)
    try:
        actual[actual.index("-o") + 1] = str(obj)
    except (ValueError, IndexError):
        return Cell(forces, None, False, "configured command has no -o to redirect")

    env = dict(os.environ)
    env["CDX_PROC"] = str(proc)
    env["CDX_OUT"] = str(log)
    if forces:
        env["CDX_FORCE"] = ",".join(forces)
    else:
        env.pop("CDX_FORCE", None)

    result = subprocess.run(actual, env=env, cwd=ROOT, capture_output=True,
                            text=True, timeout=300)
    if result.returncode != 0 or not obj.is_file():
        return Cell(forces, None, False,
                    f"compile failed: {(result.stderr or result.stdout)[:160]}")

    text = log.read_text(errors="replace") if log.exists() else ""
    if forces and "forced=-2" in text:
        return Cell(forces, None, False,
                    "a force did NOT apply (forced=-2). The object is identical "
                    "to the unforced build, so scoring it would record the base "
                    "as though the force had been tried and declined.")
    if forces and not text:
        return Cell(forces, None, False,
                    "no records were written to CDX_OUT; acceptance cannot be "
                    "verified, and an unverified force proves nothing")

    scored, error = nr.process_item(item, obj)
    if scored is None:
        return Cell(forces, None, False, f"scoring failed: {error}")
    return Cell(forces, scored.relocation_masked_differing_words, True,
                windows=window_residual(item, obj))


def window_residual(item, obj: pathlib.Path, width: int = 0x200) -> dict[int, int]:
    """Differing words per address window, from the streams already on disk.

    Read here rather than through residual_map.measure(), which recompiles the
    TU with the CONFIGURED command and would therefore report the unforced
    build -- the same trap that had a lane reading one score for twelve
    consecutive forces.
    """
    import nm_ranking as nr

    streams, error = nr.word_streams(item, obj)
    if streams is None:
        return {}
    counts: dict[int, int] = {}
    pairs = zip(streams.base_words, streams.target_words)
    for index, (base_word, target_word) in enumerate(pairs):
        if base_word != target_word:
            counts[(index * 4) // width * width] = \
                counts.get((index * 4) // width * width, 0) + 1
    return counts


def blast_radius(base: Cell, cell: Cell) -> dict[int, int]:
    """Which windows a force MOVED, and by how much, against the base.

    Negative means the window improved. A force whose radius is one window is
    a local decision; one that moves eight windows is a global reallocation,
    and two of those will contend however good each looks alone.
    """
    if not base.windows or not cell.windows:
        return {}
    keys = set(base.windows) | set(cell.windows)
    moved = {k: cell.windows.get(k, 0) - base.windows.get(k, 0) for k in keys}
    return {k: v for k, v in moved.items() if v}


def collides(base: Cell, a: Cell, b: Cell) -> set[int]:
    """Windows that BOTH forces move -- where contention would show up."""
    return set(blast_radius(base, a)) & set(blast_radius(base, b))


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
            shared = collides(base, ca, cb)
            where = ("disjoint" if not shared
                     else "collide@" + ",".join(f"+0x{w:X}" for w in sorted(shared)))
            out.append(f"  {a.split(':')[1]:<12} + {b.split(':')[1]:<12} "
                       f"{pair.score:>5}  {verdict:<13} {where}")
    radii = [(f, blast_radius(base, c)) for f, c in singles.items() if c.windows]
    if radii:
        out += ["", "blast radius per force (windows moved, - is better):"]
        for force, radius in radii:
            shown = ", ".join(f"+0x{w:X}:{d:+d}" for w, d in sorted(radius.items()))
            out.append(f"  {force.split(':')[1]:<12} {len(radius):>2} window(s)  {shown}")
        out.append("  A force moving one window is a local decision; one moving "
                   "many is a global reallocation,")
        out.append("  and two of those contend however good each looks alone.")
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
