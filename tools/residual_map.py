#!/usr/bin/env python3
"""Map one function's residual by ADDRESS, and census its registers per region.

    tools/residual_map.py <symbol> [--window 0x200] [--lo 0x0] [--hi 0x4000]
                          [--json]

`tools/align_symbol.py` says how many rows disagree and in which of the four
buckets. `tools/register_census.py` says which registers disagree over the
whole function. Neither says *where*, and on a large function that is the
question that picks the lever.

WHY THE ADDRESS MATTERS, MEASURED

On the 14,456-byte overlay 58 function this reading changed the diagnosis
outright. Whole-function, its 371 naming rows looked like a flat per-web colour
residual across twenty-odd windows -- exactly what four passes had recorded.
Split at the one candidate-only instruction, it reads:

  - after that word, 150 of 212 differing register slots form ONE closed
    nine-cycle over the temp ring
  - before it, no cycle at all; the largest temp substitution is 8 slots

So a single extra instruction had rotated the free list for the remaining 1,196
bytes, and the residual it caused was worth about 150 register slots rather
than the one word it costs. A whole-function census cannot show that, because
the un-rotated head averages the rotated tail away. The brief's rule -- read a
register census in address order -- is this tool.

WHAT IT PRINTS

  - the four aligned buckets, agreeing with `align_symbol.py` by construction
  - one row per window: differing rows, split naming / immediate / structural,
    running total
  - the candidate-only and target-only instruction offsets, which are where a
    +1 or -1 run begins and ends
  - per window, the (ours -> theirs) register substitutions and any closed
    cycle among them

A closed cycle inside ONE window with no cycle in its neighbours is a ring
phase with a cause at the window boundary (L127, L149). A cycle that persists
across every window is a whole-function phase. Scattered substitutions with no
cycle are per-web colour and the address does not help.

Everything above `main` is pure and takes word lists, so the tests need no
baserom, no toolchain and no build.
"""
from __future__ import annotations

import argparse
import contextlib
import json
import pathlib
import shutil
import sys
import tempfile

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

GPR = ("zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
       "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
       "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
       "t8", "t9", "k0", "k1", "gp", "sp", "s8", "ra")


def register_fields(word: int) -> list[int]:
    """The register slots this word actually names, in a fixed order.

    The same case split `nm_ranking.instr_reg_mask` makes, read the other way
    round: that function erases these fields, this one extracts them. J-type
    names none; SPECIAL names rs, rt and rd; COP1's register fields are float
    and are not GPR slots, so they are skipped here rather than mis-decoded;
    everything else is I-type and names rs and rt.
    """
    op = (word >> 26) & 0x3F
    if op in (0x02, 0x03) or op == 0x11:
        return []
    rs = (word >> 21) & 0x1F
    rt = (word >> 16) & 0x1F
    if op == 0x00:
        return [rs, rt, (word >> 11) & 0x1F]
    return [rs, rt]


def substitutions(base_word: int, target_word: int) -> list[tuple[str, str]]:
    """(ours, theirs) for each register slot the two words disagree on."""
    out = []
    for ours, theirs in zip(register_fields(base_word),
                            register_fields(target_word)):
        if ours != theirs:
            out.append((GPR[ours], GPR[theirs]))
    return out


def dominant_mapping(pairs: dict[tuple[str, str], int]) -> dict[str, str]:
    """For each source register, the target it is most often replaced by.

    Ties are broken by the register's own table order so the result is
    deterministic rather than dict-insertion-ordered.
    """
    by_source: dict[str, dict[str, int]] = {}
    for (ours, theirs), n in pairs.items():
        by_source.setdefault(ours, {})[theirs] = \
            by_source.setdefault(ours, {}).get(theirs, 0) + n
    out = {}
    for ours, targets in by_source.items():
        out[ours] = max(sorted(targets), key=lambda t: targets[t])
    return out


def cycles(mapping: dict[str, str]) -> list[list[str]]:
    """Closed cycles of length >= 2 in a register mapping, each listed once.

    A cycle is reported starting from its alphabetically first member so two
    runs of the same data print the same thing.
    """
    seen: set[str] = set()
    found: list[list[str]] = []
    for start in sorted(mapping):
        if start in seen:
            continue
        path: list[str] = []
        node = start
        while node in mapping and node not in path:
            path.append(node)
            node = mapping[node]
        if node in path:
            cycle = path[path.index(node):]
            if len(cycle) >= 2:
                head = cycle.index(min(cycle))
                rotated = cycle[head:] + cycle[:head]
                if rotated not in found:
                    found.append(rotated)
        seen.update(path)
    return found


def windows(rows: list[tuple[int, str]], width: int) -> list[dict]:
    """Bucket classified rows by target offset.

    `rows` is [(target_offset, bucket)] with bucket in
    {"naming", "immediate", "structural"}. Empty windows are omitted, because
    a whale has long stretches with nothing in them and printing those buries
    the signal.
    """
    buckets: dict[int, dict[str, int]] = {}
    for offset, kind in rows:
        slot = buckets.setdefault((offset // width) * width,
                                  {"naming": 0, "immediate": 0,
                                   "structural": 0})
        slot[kind] += 1
    out = []
    running = 0
    for start in sorted(buckets):
        slot = buckets[start]
        total = slot["naming"] + slot["immediate"] + slot["structural"]
        running += total
        out.append({"offset": start, "total": total, "cumulative": running,
                    **slot})
    return out


# --------------------------------------------------------------------------
# CLI. Everything below needs the build; nothing above it does.
# --------------------------------------------------------------------------

@contextlib.contextmanager
def _isolated_workdir(nr):
    """Own scratch, so this can run beside a build or another harness.

    Same reasoning as `score_symbol.py` and `align_symbol.py`.
    """
    previous = nr.WORK_DIR
    scratch = pathlib.Path(tempfile.mkdtemp(prefix="residual-map-"))
    nr.WORK_DIR = scratch
    try:
        yield
    finally:
        nr.WORK_DIR = previous
        shutil.rmtree(scratch, ignore_errors=True)


def measure(symbol: str, object_path: pathlib.Path | None = None) -> dict:
    """Map `symbol`'s residual, from the configured build or a supplied object.

    WHY --object EXISTS. Every map this tool drew came from recompiling the
    tree, so the only object it could ever read was the one the working
    directory happens to produce. The overlay 58 force lattice retains 31
    forced objects, and the question that mattered -- which windows still carry
    naming rows once the five known forces are applied -- is a map of an object
    this tool could not open. Reading a retained object costs no compile and
    keeps the map identical in every other respect.
    """
    import align_symbol as als
    import nm_ranking as nr
    import permute_batch as pb

    queue = {entry.func: entry for entry in pb.discover_queue()}
    if symbol not in queue:
        raise SystemExit(f"residual_map: {symbol} is not in the NON_MATCHING "
                         f"queue (already matched, or never queued)")
    item = queue[symbol]
    with _isolated_workdir(nr):
        if object_path is None:
            commands = nr.configured_compile_commands([item])
            obj, error = nr.compile_configured_tu(item.rel_c_file,
                                                  commands[item.rel_c_file])
            if obj is None:
                raise SystemExit(f"residual_map: {error}")
        else:
            obj = pathlib.Path(object_path)
            if not obj.is_file():
                raise SystemExit(f"residual_map: no such object {obj}")
        streams, error = nr.word_streams(item, obj)
        if streams is None:
            raise SystemExit(f"residual_map: {error}")
        result, error = nr.process_item(item, obj)
        if result is None:
            raise SystemExit(f"residual_map: {error}")

        base, target = streams.base_words, streams.target_words
        b_reloc, t_reloc = streams.base_reloc, streams.target_reloc
        masked = als.reloc_masked

        def key(words, reloc, other):
            return [nr.instr_reg_mask(masked(w, i * 4, reloc, other))
                    for i, w in enumerate(words)]

        script = als._banded_edit_script(key(base, b_reloc, t_reloc),
                                        key(target, t_reloc, b_reloc))
        rows: list[tuple[int, str]] = []
        pairs_at: list[tuple[int, tuple[str, str]]] = []
        exact = 0
        insertions: list[int] = []
        deletions: list[int] = []
        for op, i, j in script:
            if op in ("equal", "replace"):
                bw = masked(base[i], i * 4, b_reloc, t_reloc)
                tw = masked(target[j], j * 4, t_reloc, b_reloc)
                if op == "equal" and bw == tw:
                    exact += 1
                elif nr.instr_reg_mask(bw) == nr.instr_reg_mask(tw):
                    rows.append((j * 4, "naming"))
                    for pair in substitutions(bw, tw):
                        pairs_at.append((j * 4, pair))
                elif als.instr_imm_mask(bw) == als.instr_imm_mask(tw):
                    rows.append((j * 4, "immediate"))
                else:
                    rows.append((j * 4, "structural"))
            elif op == "delete":
                insertions.append(i * 4)
            elif op == "insert":
                deletions.append(j * 4)
        return {
            "symbol": symbol,
            "file": item.rel_c_file,
            "object": str(object_path) if object_path else "configured build",
            "size_bytes": result.size_bytes,
            "size_delta": result.size_delta,
            "positional_masked": result.relocation_masked_differing_words,
            "aligned_exact": exact,
            "rows": rows,
            "pairs": pairs_at,
            "insertions": insertions,
            "deletions": deletions,
        }


def render(data: dict, width: int, lo: int, hi: int) -> str:
    rows = [r for r in data["rows"] if lo <= r[0] < hi]
    pairs = [p for p in data["pairs"] if lo <= p[0] < hi]
    counts = {"naming": 0, "immediate": 0, "structural": 0}
    for _, kind in rows:
        counts[kind] += 1
    out = [f"{data['symbol']}  ({data['file']})",
           f"  {data['size_bytes']} bytes, size delta {data['size_delta']:+d}, "
           f"positional masked {data['positional_masked']}",
           f"  byte-exact {data['aligned_exact']}, naming {counts['naming']}, "
           f"immediate {counts['immediate']}, structural {counts['structural']}"]
    ins = [f"+0x{o:X}" for o in data["insertions"] if lo <= o < hi]
    dels = [f"+0x{o:X}" for o in data["deletions"] if lo <= o < hi]
    if ins or dels:
        # align_symbol.py counts a word present on only one side inside its
        # "really different" bucket. Here they are listed separately, because
        # their OFFSETS are the whole point: each pair brackets a run where
        # one stream is a word ahead of the other. So this tool's structural
        # count plus len(insertions) plus len(deletions) is that bucket.
        out.append(f"  structural excludes {len(ins)} candidate-only and "
                   f"{len(dels)} target-only words, listed next")
    if ins:
        out.append("  candidate-only words: " + " ".join(ins))
    if dels:
        out.append("  target-only words:    " + " ".join(dels))
    out.append("")
    out.append("  offset      rows   nam   imm   str    cum")
    for w in windows(rows, width):
        out.append(f"   +0x{w['offset']:05X} {w['total']:6d} {w['naming']:5d} "
                   f"{w['immediate']:5d} {w['structural']:5d} "
                   f"{w['cumulative']:6d}")
    out.append("")
    out.append("  per-window register substitutions (ours -> theirs)")
    by_window: dict[int, dict[tuple[str, str], int]] = {}
    for offset, pair in pairs:
        slot = by_window.setdefault((offset // width) * width, {})
        slot[pair] = slot.get(pair, 0) + 1
    for start in sorted(by_window):
        slot = by_window[start]
        top = sorted(slot.items(), key=lambda kv: (-kv[1], kv[0]))[:6]
        spelled = ", ".join(f"{a}->{b} x{n}" for (a, b), n in top)
        out.append(f"   +0x{start:05X}  {spelled}")
        for cycle in cycles(dominant_mapping(slot)):
            out.append(f"            cycle: " + " -> ".join(cycle + [cycle[0]]))
    return "\n".join(out)


def compare(before: dict, after: dict, width: int, lo: int, hi: int) -> str:
    """Per-window aligned delta between two maps of the same function.

    `tools/force_lattice.py` reports a blast radius over POSITIONAL words,
    which is what its score counts but which inflates any window bracketed by
    a one-word insertion: every word after the insertion mismatches by
    position while aligning perfectly. On overlay 58 two such windows carried
    81 positional words and 22 aligned rows, and reading the positional number
    as register residual is what made those windows look like the densest
    colour problem in the function when they are two extra instructions.
    This view is aligned, so a window's number moves only when its content does.
    """
    def binned(data):
        out: dict[int, dict[str, int]] = {}
        for offset, kind in data["rows"]:
            if not lo <= offset < hi:
                continue
            slot = out.setdefault((offset // width) * width,
                                  {"naming": 0, "immediate": 0, "structural": 0})
            slot[kind] += 1
        return out

    a, b = binned(before), binned(after)
    out = [f"{before['symbol']}",
           f"  before: {before['object']}",
           f"  after:  {after['object']}",
           "",
           "  offset        nam        imm        str      total",
           "                a->b       a->b       a->b       delta"]
    total = 0
    for start in sorted(set(a) | set(b)):
        ra = a.get(start, {"naming": 0, "immediate": 0, "structural": 0})
        rb = b.get(start, {"naming": 0, "immediate": 0, "structural": 0})
        delta = sum(rb.values()) - sum(ra.values())
        total += delta
        cells = "  ".join(f"{ra[k]:>3}->{rb[k]:<3}"
                          for k in ("naming", "immediate", "structural"))
        mark = "" if delta else "   unmoved"
        out.append(f"   +0x{start:05X}  {cells}  {delta:+8d}{mark}")
    out.append(f"   {'total':<10} {'':<33}{total:+8d}")

    for name, key in (("candidate-only", "insertions"), ("target-only", "deletions")):
        was = {o for o in before[key] if lo <= o < hi}
        now = {o for o in after[key] if lo <= o < hi}
        if was != now:
            out.append(f"  {name} words changed: "
                       f"gone {sorted(was - now)}, new {sorted(now - was)}")
        elif was:
            out.append(f"  {name} words unchanged at "
                       + " ".join(f"+0x{o:X}" for o in sorted(was)))
    return "\n".join(out)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Map a function's residual by address and census its "
                    "registers per region.")
    parser.add_argument("symbol")
    parser.add_argument("--window", type=lambda v: int(v, 0), default=0x200)
    parser.add_argument("--lo", type=lambda v: int(v, 0), default=0)
    parser.add_argument("--hi", type=lambda v: int(v, 0), default=1 << 30)
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--object", type=pathlib.Path, default=None,
                        help="map this object instead of recompiling the tree")
    parser.add_argument("--against", type=pathlib.Path, default=None,
                        help="also map this object and print the aligned "
                             "per-window delta from it to --object/the build")
    args = parser.parse_args(argv)
    data = measure(args.symbol, args.object)
    if args.against is not None:
        before = measure(args.symbol, args.against)
        print(compare(before, data, args.window, args.lo, args.hi))
        return 0
    if args.json:
        data["rows"] = [{"offset": o, "bucket": k} for o, k in data["rows"]]
        data["pairs"] = [{"offset": o, "ours": a, "theirs": b}
                         for o, (a, b) in data["pairs"]]
        print(json.dumps(data, indent=2))
    else:
        print(render(data, args.window, args.lo, args.hi))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
