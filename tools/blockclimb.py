#!/usr/bin/env python3
"""Hill-climb the STATEMENT ORDER of one function, a run at a time.

    tools/blockclimb.py <symbol> --write <out.c> [--passes N] [--min-run N]
                        [--probe-unguarded]

Statement order is a real axis on this compiler and it is the one lanes keep
retiring by accident. The call test (`docs/LANE_BRIEF.md`) retires **L106** --
ugen's p2 web-number ordering -- and nothing else; statement order still
reaches as1's scheduling and ugen's emission order, neither of which a phase
census touches. Three plateau shards here carried a note saying otherwise, and
on the largest of them this sweep was worth 45 words against a residual that
four previous passes had characterised as pure p1 colour.

WHAT IT DOES

Find every *run* -- three or more consecutive single-line statements at one
indentation level, none of which contains a call -- inside the function's line
range. For each run, try every one-statement move (take statement i, insert it
at position j) and keep any that lowers the score. Repeat until a whole pass
over every run proposes no move; that is a move-one local optimum, not a global
one, and it is reported as such.

The score comes from `nm_ranking`, the same relocation-masked positional count
`tools/score_symbol.py` reports, so a run of this agrees with the ranking by
construction. The scorer is injected, so the climb itself is testable without a
baserom, a toolchain or a build.

THE AUDIT REQUIREMENT -- READ THIS BEFORE ADOPTING ANY DIFF

**This tool optimises a score. A score is not a proof of equivalence.** Every
diff it proposes has to be read for semantics before it is adopted, and the
measured base rate of bad proposals is not small:

  - On the 14,456-byte overlay 58 function the unguarded optimum was 672 words
    against the guarded 688, and it got there by moving `nodes[1].texture = 0;`
    to AFTER the `func_8002F618` call at two sites. That callee walks the node
    list until it reads a NULL `texture`, so the terminator has to be stored
    first: the "better" order walks off the end of a local array.
  - The same run also moved `x = -x;` in front of a call taking `x + 0xA0` as
    an argument, which changes what is drawn.
  - On the 4,832-byte overlay 57 function it offered one further word for
    swapping `func_80028D24(0);` with `func_80028540(...)` -- two calls, in the
    other order.

All four are rejected. The guard below (a statement containing a call is not
movable, and a call terminates the run it appears in, so nothing can be moved
*across* one either) removes exactly this family, which is why the tool is
adoptable at all. It does not remove every semantic hazard: a store the callee
of a *later* call observes, two stores to the same aliased address, or a
statement whose value another statement in the run reads, are all still yours
to check. Read the diff.

`--probe-unguarded` re-climbs with the guard off and prints what that reaches,
WITHOUT writing it. The point is to make an unsafe winner visible rather than
silently discarded: if the unguarded number is much better, that is evidence
about the target's shape (on overlay 58 it says the original probably
initialises `nodes[1].texture` somewhere this reconstruction does not) and it
is worth writing into the shard even though the order itself cannot be taken.

WHAT IT COSTS, AND WHAT AN EMPTY RESULT MEANS

Measured on this tree: 0.14 s per candidate on a resident TU, 0.55 s on the
overlay 58 TU, and about 3,000 compiles for one full sweep of that function's
67 runs. Budget ten to twenty minutes for a whale and under a minute for a
small function.

It returned 45 words on the overlay 58 whale, 12 on the overlay 57 function and
**nothing at all** on `func_8000590C`. That last one is the result that makes
the other two worth believing: a sweep that never comes back empty is not
measuring anything. Record an empty run in the shard as a closed axis -- with
the pass count, so the next lane knows it was climbed to a fixed point and does
not pay for it again.
"""
from __future__ import annotations

import argparse
import contextlib
import pathlib
import re
import shutil
import sys
import tempfile
import time

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

ROOT = pathlib.Path(__file__).resolve().parent.parent

# A statement may not move if it can call something. Two different hazards are
# closed by the one rule: reordering two calls against each other, and moving a
# plain store across a call that observes it. Casts are spelled `(type) x`, so
# an identifier immediately followed by `(` is the tell and a cast never is.
CALL_RE = re.compile(r"[A-Za-z_]\w*\s*\(")

# A run is built from plain statements only. Anything that opens or closes a
# block, or continues onto another line, ends the run rather than joining it --
# the climb permutes whole lines and a half-statement is not a whole line.
BLOCK_STARTS = ("if", "else", "for", "while", "do", "switch", "case",
                "default", "return", "break", "continue", "goto", "}", "{",
                "/*", "*", "//", "#")


def is_movable(line: str, allow_calls: bool = False) -> bool:
    """True if `line` is one complete statement this sweep may reorder.

    `allow_calls` exists for `--probe-unguarded` and for the tests. It is never
    what writes a file.
    """
    s = line.strip()
    if not s.endswith(";"):
        return False
    if s.startswith(BLOCK_STARTS):
        return False
    if "{" in s or "}" in s:
        return False
    if not allow_calls and CALL_RE.search(s):
        return False
    return True


def find_runs(lines: list[str], lo: int, hi: int, min_run: int = 3,
              allow_calls: bool = False) -> list[tuple[int, int]]:
    """Maximal [start, stop) spans of movable statements at one indentation.

    `lo`/`hi` are 0-based and half-open. A line that is not movable -- a call,
    a brace, a comment -- terminates the run it meets rather than being skipped
    over, which is what stops a statement being moved *across* a call.
    """
    out: list[tuple[int, int]] = []
    i = max(lo, 0)
    hi = min(hi, len(lines))
    while i < hi:
        if not is_movable(lines[i], allow_calls):
            i += 1
            continue
        indent = len(lines[i]) - len(lines[i].lstrip())
        j = i
        while (j < hi and is_movable(lines[j], allow_calls)
               and len(lines[j]) - len(lines[j].lstrip()) == indent):
            j += 1
        if j - i >= min_run:
            out.append((i, j))
        i = j
    return out


def moves(n: int):
    """Every one-statement move over `n` statements, as reordered index lists."""
    base = list(range(n))
    for i in range(n):
        for j in range(n):
            if i == j:
                continue
            order = base[:]
            order.insert(j, order.pop(i))
            if order != base:
                yield i, j, order


class Result:
    """What one climb reached, and how it got there."""

    def __init__(self) -> None:
        self.base_score: int = 0
        self.best_score: int = 0
        self.lines: list[str] = []
        self.adopted: list[tuple[int, int, int, int]] = []  # start, stop, i, j
        self.passes: int = 0
        self.evaluations: int = 0
        self.runs: list[tuple[int, int]] = []
        self.hit_pass_cap: bool = False

    @property
    def gain(self) -> int:
        return self.base_score - self.best_score


def climb(lines: list[str], score, lo: int, hi: int, min_run: int = 3,
          max_passes: int = 8, allow_calls: bool = False) -> Result:
    """Move-one hill climb to a fixed point.

    `score` takes the joined source text and returns an integer to minimise.

    Termination is not left to the scorer. Every order that has been evaluated
    is cached by its key, so an order is scored once and only once; a scorer
    that contradicts itself across calls therefore cannot make the climb
    ping-pong between two orders, because the second visit never re-measures.
    `max_passes` is a second, independent bound on top of that.
    """
    result = Result()
    result.lines = list(lines)
    cache: dict[tuple, int] = {}
    seen: set[tuple] = set()

    def measure(candidate: list[str]) -> int:
        key = tuple(candidate)
        if key in cache:
            return cache[key]
        value = score("\n".join(candidate))
        cache[key] = value
        result.evaluations += 1
        return value

    result.base_score = result.best_score = measure(result.lines)
    seen.add(tuple(result.lines))
    result.runs = find_runs(result.lines, lo, hi, min_run, allow_calls)

    for _ in range(max_passes):
        result.passes += 1
        improved = False
        # Runs are recomputed each pass: a move inside one run never changes
        # another run's bounds (runs are disjoint and moves are within a run),
        # but recomputing keeps this honest if that ever stops being true.
        for (start, stop) in find_runs(result.lines, lo, hi, min_run,
                                       allow_calls):
            n = stop - start
            for i, j, order in moves(n):
                candidate = (result.lines[:start]
                             + [result.lines[start + k] for k in order]
                             + result.lines[stop:])
                key = tuple(candidate)
                if key in seen:
                    continue
                value = measure(candidate)
                seen.add(key)
                if value < result.best_score:
                    result.best_score = value
                    result.lines = candidate
                    result.adopted.append((start, stop, i, j))
                    improved = True
        if not improved:
            return result
    result.hit_pass_cap = True
    return result


# --------------------------------------------------------------------------
# CLI. Everything below needs the build; nothing above it does.
# --------------------------------------------------------------------------

def _function_line_span(text: str, symbol: str) -> tuple[int, int]:
    """0-based, half-open line range of `symbol`'s body.

    The definition is the line that names the symbol immediately before a `(`
    and is not a declaration, a call or a comment; the body starts after the
    first `{` at or below it -- several definitions here wrap their parameter
    list over two lines -- and ends at the first closing brace in column zero.
    """
    lines = text.split("\n")
    opener = re.compile(r"\b" + re.escape(symbol) + r"\s*\(")
    start = None
    for index, line in enumerate(lines):
        stripped = line.strip()
        if not opener.search(line):
            continue
        if (stripped.startswith(("*", "/*", "//", "#"))
                or stripped.endswith(";")
                or stripped.startswith(("return ", "extern "))):
            continue
        for scan in range(index, min(index + 4, len(lines))):
            if lines[scan].rstrip().endswith("{"):
                start = scan + 1
                break
        if start is not None:
            break
    if start is None:
        raise SystemExit(f"blockclimb: no definition of {symbol} found")
    for index in range(start, len(lines)):
        if lines[index].startswith("}"):
            return start, index
    raise SystemExit(f"blockclimb: {symbol} has no closing brace in column 0")


@contextlib.contextmanager
def _restored(path: pathlib.Path):
    """Put the source back however this exits: the tree is not ours to leave dirty."""
    original = path.read_text()
    try:
        yield original
    finally:
        path.write_text(original)


def _configured_scorer(symbol: str):
    """Compile the configured TU and return the relocation-masked word count.

    Returns `(item, source_path, score, scratch_dir)`. Imported lazily so the
    climb above -- and its tests -- stay free of the build, the toolchain and
    the baserom.
    """
    import nm_ranking as nr
    import permute_batch as pb

    queue = {entry.func: entry for entry in pb.discover_queue()}
    if symbol not in queue:
        raise SystemExit(
            f"blockclimb: {symbol} is not in the NON_MATCHING queue "
            f"(already matched, or never queued)")
    item = queue[symbol]
    source = ROOT / item.rel_c_file
    scratch = pathlib.Path(tempfile.mkdtemp(prefix="blockclimb-"))
    nr.WORK_DIR = scratch
    commands = {}

    def score(text: str) -> int:
        source.write_text(text)
        if not commands:
            commands.update(nr.configured_compile_commands([item]))
        obj, error = nr.compile_configured_tu(item.rel_c_file,
                                              commands[item.rel_c_file])
        if obj is None:
            return 10 ** 9          # a form that does not compile is not a win
        measured, error = nr.process_item(item, obj)
        if measured is None:
            return 10 ** 9
        return measured.relocation_masked_differing_words

    return item, source, score, scratch


def _report(result: Result, label: str, stream=sys.stdout) -> None:
    # The metric falls when it improves, so spell the direction out rather
    # than printing a signed number a reader has to interpret.
    gain = ("no change" if result.gain == 0
            else f"{result.gain} word(s) better")
    print(f"{label}: {result.base_score} -> {result.best_score} "
          f"({gain}, {len(result.adopted)} move(s), "
          f"{result.passes} pass(es), {result.evaluations} compiles)",
          file=stream)
    if result.hit_pass_cap:
        print(f"  NOT a fixed point: stopped at the {result.passes}-pass cap; "
              f"re-run from the output to continue", file=stream)
    elif not result.adopted:
        print("  no move improves; this is a move-one fixed point and the "
              "axis is closed for this shape", file=stream)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Hill-climb one function's statement order.")
    parser.add_argument("symbol")
    parser.add_argument("--write", type=pathlib.Path, required=True,
                        help="where to write the improved source; the tree's "
                             "own file is always restored")
    parser.add_argument("--min-run", type=int, default=3)
    parser.add_argument("--passes", type=int, default=8)
    parser.add_argument("--probe-unguarded", action="store_true",
                        help="also report what is reachable when statements "
                             "containing calls may move. Never written, and "
                             "never adoptable without reading the diff.")
    args = parser.parse_args(argv)

    item, source, score, scratch = _configured_scorer(args.symbol)
    try:
        with _restored(source) as original:
            lines = original.split("\n")
            lo, hi = _function_line_span(original, args.symbol)
            started = time.time()
            guarded = climb(lines, score, lo, hi, args.min_run, args.passes)
            _report(guarded, f"{args.symbol} guarded")
            args.write.write_text("\n".join(guarded.lines))
            print(f"  wrote {args.write}")

            if args.probe_unguarded:
                unguarded = climb(lines, score, lo, hi, args.min_run,
                                  args.passes, allow_calls=True)
                _report(unguarded, f"{args.symbol} UNGUARDED (not written)")
                if unguarded.best_score < guarded.best_score:
                    print(f"  the unguarded optimum is "
                          f"{guarded.best_score - unguarded.best_score} words "
                          f"better and moves statements containing calls. It "
                          f"is NOT adoptable as it stands -- read every hunk, "
                          f"and record the gap in the shard as evidence about "
                          f"the target's shape.")
            print(f"  {time.time() - started:.0f}s")
    finally:
        shutil.rmtree(scratch, ignore_errors=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
