#!/usr/bin/env python3
"""Census which register each side uses where, and find the permutation.

    tools/register_census.py <symbol> [<symbol> ...] [--json]

The fourth instrument to be rebuilt in scratch and thrown away, after the shape
aligner, the frame census and a fast scorer. A lane wrote one to get a
four-cycle out of a 279-word residual, and that cycle *was* the residual: 195 of
the 279 words were one permutation over the integer temp ring, and naming it
turned a "structural" plateau into a one-edit fix that closed 188 words.

`align_symbol.py` says how many rows differ only by register. This says **which
registers**, and whether they form a cycle.

That distinction decides the toolkit. A handful of scattered substitutions is a
colouring question per web. A clean cycle over `t6`-`t9` is one ring-phase fact
with a single cause, and by L127 a phase error is fixable from source at zero
byte cost by consuming one more ring temp. Reading a cycle as N independent
colour problems is how a lane spends a day on the wrong axis.

Pairs are counted only where the two words agree on everything except their
register fields, so a genuinely different instruction never contributes. Both
streams come from `nm_ranking.word_streams`, the path the ranking scores by.

**The two register banks are reported separately, and that correction matters.**
The integer temp ring and the floating-point pool are different allocators, so a
residual made entirely of float rows says nothing about integer colouring and
vice versa. Until 2026-09-11 this tool had no bank tag: it treated `lwc1`/`swc1`
as plain I-type and printed their float datum under a GPR name, and returned no
fields at all for COP1 register format. On `func_8003F154` that dropped nine of
thirteen substitution sites and renamed the other four, so a three-window float
rotation printed as a coherent integer cycle with L127 beside it -- pointing a
lane at the expression ring for a residual that is entirely fp.
"""
from __future__ import annotations

import argparse
import collections
import contextlib
import json
import pathlib
import shutil
import sys
import tempfile

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))

import align_symbol as al  # noqa: E402
import nm_ranking as nr  # noqa: E402
import permute_batch as pb  # noqa: E402

GPR = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
       "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
       "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
       "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"]
FPR = [f"f{n}" for n in range(32)]
BANKS = {"int": GPR, "fpr": FPR}

# Where each format keeps its register fields, as (bank, shift, width) triples.
R_FIELDS = (("int", 21, 0x1F), ("int", 16, 0x1F), ("int", 11, 0x1F))  # rs,rt,rd
I_FIELDS = (("int", 21, 0x1F), ("int", 16, 0x1F))                     # rs, rt
# lwc1/swc1/ldc1/sdc1: the base is a GPR, `ft` is a FLOAT register.
FP_MEM_FIELDS = (("int", 21, 0x1F), ("fpr", 16, 0x1F))
# COP1 register format: fmt(25-21) ft(20-16) fs(15-11) fd(10-6) function(5-0).
FP_R_FIELDS = (("fpr", 16, 0x1F), ("fpr", 11, 0x1F), ("fpr", 6, 0x1F))
# mfc1/mtc1/cfc1/ctc1 move between banks: `rt` is a GPR, `fs` a float register.
FP_MOVE_FIELDS = (("int", 16, 0x1F), ("fpr", 11, 0x1F))
FP_MEM_OPS = {0x31, 0x35, 0x39, 0x3D}          # lwc1, ldc1, swc1, sdc1
FP_MOVE_FMTS = {0x00, 0x02, 0x04, 0x06}        # mfc1, cfc1, mtc1, ctc1


def fields(word: int) -> tuple[tuple[str, int, int], ...]:
    """The register selectors of one word, each tagged with its register bank.

    **The bank tag is the whole point, and its absence was a real fault.** The
    first version returned bare shifts and treated every non-SPECIAL word as
    I-type, so `lwc1 $f2,60(a3)` against `lwc1 $f6,60(a3)` was read as the GPR
    substitution `v0 -> a2` -- and a float-ring rotation over f0/f2/f4/f6 came
    out as an integer ring cycle with L127 printed beside it. It also returned
    nothing at all for COP1 register format, so `add.s`/`mul.s`/`neg.s` sites
    were invisible: on `func_8003F154` nine of thirteen substitution sites were
    dropped and the remaining four were mislabelled. `nm_ranking.instr_reg_mask`
    already gets COP1 right, so the aligner counted those rows as naming while
    this tool named the wrong registers for them.
    """
    op = (word >> 26) & 0x3F
    if op in (0x02, 0x03):          # j / jal carry no register
        return ()
    if op == 0x00:                  # SPECIAL
        return R_FIELDS
    if op in FP_MEM_OPS:            # float load/store: GPR base, FPR datum
        return FP_MEM_FIELDS
    if op == 0x11:                  # COP1
        fmt = (word >> 21) & 0x1F
        if fmt == 0x08:             # BC1 names no register
            return ()
        if fmt in FP_MOVE_FMTS:
            return FP_MOVE_FIELDS
        return FP_R_FIELDS
    return I_FIELDS


def census(streams: "nr.WordStreams") -> dict:
    """Count register-for-register substitutions between *aligned* words.

    The pairs must come from the alignment, not from matching indices. A
    function with a size delta has its streams shifted, so past the shift point
    a positional comparison pairs unrelated instructions and invents
    substitutions that are not there -- which is exactly the noise this tool
    exists to cut through. The first draft did that and reported a muddled
    two-cycle on a function whose streams are 32 bytes out of step.
    """
    pairs: collections.Counter = collections.Counter()
    ordered: list[tuple[int, str, str]] = []
    fpairs: collections.Counter = collections.Counter()
    fordered: list[tuple[int, str, str]] = []
    sites = 0
    base, target = streams.base_words, streams.target_words
    b_reloc, t_reloc = streams.base_reloc, streams.target_reloc

    def key(words, reloc, other):
        return [nr.instr_reg_mask(al.reloc_masked(w, i * 4, reloc, other))
                for i, w in enumerate(words)]

    script = al._banded_edit_script(key(base, b_reloc, t_reloc),
                                    key(target, t_reloc, b_reloc))
    for op, i, j in script:
        if op not in ("equal", "replace"):
            continue
        a = al.reloc_masked(base[i], i * 4, b_reloc, t_reloc)
        b = al.reloc_masked(target[j], j * 4, t_reloc, b_reloc)
        if a == b or nr.instr_reg_mask(a) != nr.instr_reg_mask(b):
            continue                # identical, or not a pure register difference
        sites += 1
        for bank, shift, mask in fields(a):
            ra, rb = (a >> shift) & mask, (b >> shift) & mask
            if ra == rb:
                continue
            names = BANKS[bank]
            if bank == "fpr":
                fpairs[(names[ra], names[rb])] += 1
                fordered.append((j * 4, names[ra], names[rb]))
            else:
                pairs[(names[ra], names[rb])] += 1
                ordered.append((j * 4, names[ra], names[rb]))
    return {"sites": sites, "pairs": pairs, "ordered": ordered,
            "fpairs": fpairs, "fordered": fordered}


def windows(sites: list[tuple[int, str, str]]) -> dict:
    """Does ONE mapping hold across the function, or does it change by region?

    Coherence counts fan-out but not *where* it happens, and that hides the
    distinction that matters. A census printed a clean four-cycle whose dominant
    mapping discarded the fact that one source register went three ways and
    another four; offset-resolved, only 60% of its pairs fit a single global
    permutation and six windows were needed. Two functions away the same
    instrument saw one clean shift at 83%, so it does discriminate -- but only
    once the sites are read in address order.

    A permutation that holds function-wide is one ring-phase fact. A mapping
    that changes every few sites is per-iteration or per-region consumption,
    and each window is its own question.

    Returns the share of sites explained by the single best global mapping, and
    the offsets where a greedy scan has to start a new window.
    """
    if not sites:
        return {"global_share": 1.0, "windows": 1, "boundaries": []}
    best: dict[str, str] = {}
    counts: collections.Counter = collections.Counter()
    for _, src, dst in sites:
        counts[(src, dst)] += 1
    for (src, dst), n in counts.items():
        if src not in best or n > counts[(src, best[src])]:
            best[src] = dst
    fits = sum(1 for _, src, dst in sites if best.get(src) == dst)

    # Greedy: extend a window while its own mapping stays consistent.
    boundaries: list[int] = []
    current: dict[str, str] = {}
    for offset, src, dst in sites:
        if current.get(src, dst) != dst:
            boundaries.append(offset)
            current = {}
        current[src] = dst
    return {"global_share": fits / len(sites),
            "windows": len(boundaries) + 1,
            "boundaries": boundaries}


def coherence(pairs: collections.Counter) -> dict:
    """How functional the mapping is: does each source register go to ONE target?

    Cycle presence alone is not a usable tell, which was learned by acting on
    it. A lead whose residual was a temp-ring four-cycle closed on one edit; the
    sibling's census also printed a closed cycle, the transfer was applied, and
    it refused. The difference is that the lead's mapping was *functional* --
    each source register went to a single target and the cycle covered 195 of
    279 words -- while the sibling maps one source register to three different
    targets and the cycle covers a minority of its residual.

    So report the share of each source's substitutions that follow its dominant
    target. Near 1.0 is a permutation, which is one ring-phase fact. Far below
    is scattered colouring that happens to contain a loop.
    """
    by_source: dict[str, collections.Counter] = collections.defaultdict(
        collections.Counter)
    for (src, dst), count in pairs.items():
        by_source[src][dst] += count
    total = sum(pairs.values())
    dominant = sum(counts.most_common(1)[0][1] for counts in by_source.values())
    worst = min(
        (counts.most_common(1)[0][1] / sum(counts.values()), src)
        for src, counts in by_source.items()) if by_source else (1.0, "")
    return {
        "share_following_dominant": (dominant / total) if total else 0.0,
        "sources": len(by_source),
        "least_coherent_source": worst[1],
        "least_coherent_share": worst[0],
    }


def cycles(pairs: collections.Counter) -> list[list[str]]:
    """Closed cycles in the dominant mapping, which is what a ring phase looks like."""
    best: dict[str, str] = {}
    for (src, dst), count in pairs.items():
        if src not in best or count > pairs[(src, best[src])]:
            best[src] = dst
    found, seen = [], set()
    for start in best:
        if start in seen:
            continue
        chain, node = [], start
        while node in best and node not in chain:
            chain.append(node)
            node = best[node]
        if node == start and len(chain) > 1:
            found.append(chain)
            seen.update(chain)
    return sorted(found, key=len, reverse=True)


@contextlib.contextmanager
def _isolated_workdir():
    previous = nr.WORK_DIR
    scratch = pathlib.Path(tempfile.mkdtemp(prefix="register-census-"))
    nr.WORK_DIR = scratch
    try:
        yield
    finally:
        nr.WORK_DIR = previous
        shutil.rmtree(scratch, ignore_errors=True)


def measure(symbols: list[str]) -> tuple[list[dict], list[str]]:
    queue = {item.func: item for item in pb.discover_queue()}
    wanted, errors = [], []
    for symbol in symbols:
        item = queue.get(symbol)
        if item is None:
            errors.append(f"{symbol}: not in the NON_MATCHING queue")
        else:
            wanted.append(item)
    if not wanted:
        return [], errors
    rows = []
    with _isolated_workdir():
        commands = nr.configured_compile_commands(wanted)
        compiled = {s: nr.compile_configured_tu(s, commands[s]) for s in commands}
        for item in wanted:
            candidate, error = compiled[item.rel_c_file]
            if candidate is None:
                errors.append(f"{item.func}: {error}")
                continue
            streams, error = nr.word_streams(item, candidate)
            if streams is None:
                errors.append(f"{item.func}: {error}")
                continue
            out = census(streams)
            rows.append({
                "symbol": item.func,
                "substitution_sites": out["sites"],
                "pairs": [{"ours": a, "theirs": b, "count": c}
                          for (a, b), c in out["pairs"].most_common()],
                "cycles": cycles(out["pairs"]),
                "coherence": coherence(out["pairs"]),
                "windows": windows(out["ordered"]),
                "float_pairs": [{"ours": a, "theirs": b, "count": c}
                                for (a, b), c in out["fpairs"].most_common()],
                "float_cycles": cycles(out["fpairs"]),
                "float_coherence": coherence(out["fpairs"]),
                "float_windows": windows(out["fordered"]),
            })
    return rows, errors


def render_bank(row: dict, prefix: str, title: str, law: str) -> list[str]:
    """One bank's section. The float bank gets its own because it is a different
    allocator: the integer temp ring and the fp pool rotate independently, and a
    residual that is entirely float rows says nothing about integer colouring."""
    pairs = row[prefix + "pairs"] if prefix else row["pairs"]
    if not pairs:
        return []
    out = [f"  {title} (ours -> theirs):"]
    for p in pairs[:10]:
        out.append(f"    {p['ours']:>4} -> {p['theirs']:<4} x{p['count']}")
    c = row[prefix + "coherence"] if prefix else row["coherence"]
    share = c["share_following_dominant"]
    out.append(f"  mapping coherence: {share:.0%} of substitutions follow their "
               f"source's dominant target ({c['sources']} source registers)")
    if c["least_coherent_source"]:
        out.append(f"    least coherent: {c['least_coherent_source']} at "
                   f"{c['least_coherent_share']:.0%}")
    w = row[prefix + "windows"] if prefix else row["windows"]
    out.append(f"  one global mapping explains {w['global_share']:.0%} of sites; "
               f"{w['windows']} window(s) needed")
    if w["windows"] > 1 and w["boundaries"]:
        shown = ", ".join(f"+0x{b:X}" for b in w["boundaries"][:6])
        out.append(f"    new mapping starts at {shown}"
                   + (" ..." if len(w["boundaries"]) > 6 else ""))
        out.append("    a mapping that changes by region is per-iteration"
                   " consumption, not one ring phase")
    found = row[prefix + "cycles"] if prefix else row["cycles"]
    if found:
        out.append("  cycles in the dominant mapping:")
        for cycle in found:
            out.append("    " + " -> ".join(cycle) + f" -> {cycle[0]}")
        if share >= 0.80:
            out.append(f"    coherent cycle: one {law} fact, not N colour"
                       " problems -- see L127")
        else:
            out.append("    NOTE: a cycle in an incoherent mapping is not a ring"
                       " phase. Cycle presence alone is not the tell -- a"
                       " transfer was applied on it and refused.")
    else:
        out.append("  no closed cycle: treat these as per-web colour questions")
    return out


def render(row: dict) -> str:
    out = [f"{row['symbol']}",
           f"  pure register-substitution sites: {row['substitution_sites']}"]
    if not row["pairs"] and not row["float_pairs"]:
        out.append("  no register-only differences")
        return "\n".join(out)
    if not row["pairs"]:
        out.append("  NO integer-register differences: every substitution is a"
                   " FLOAT register, so this is the fp pool/ring, not integer"
                   " colouring")
    out += render_bank(row, "", "most common substitutions", "ring-phase")
    if row["float_pairs"]:
        out.append("  float registers:")
        out += render_bank(row, "float_", "most common float substitutions",
                           "fp-ring-phase")
    return "\n".join(out)


def main(argv: list[str]) -> int:
    ap = argparse.ArgumentParser(
        description="Census register substitutions and find ring-phase cycles.")
    ap.add_argument("symbols", nargs="+")
    ap.add_argument("--json", action="store_true")
    args = ap.parse_args(argv)
    rows, errors = measure(args.symbols)
    if args.json:
        print(json.dumps({"functions": rows, "errors": errors}, indent=2))
    else:
        for row in rows:
            print(render(row))
            print()
        for e in errors:
            print(f"error: {e}", file=sys.stderr)
    return 1 if errors and not rows else 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
