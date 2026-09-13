# Whale address-lifetime experiment: lane wv-b, 2026-09-13

Target: `func_overlay_058_F000138C_18B0574` in overlay 58. The
[per-function handoff](matching-triage-handoffs/func_overlay_058_F000138C_18B0574.md)
retains the earlier attempts and the current guarded-source metrics.

**Positive diagnostic: one naturally coloured address web at `numintf=39`.**
The retained reconstruction is still **187 masked / 358 raw**, 14,456 bytes,
delta zero, frame 0x138, 1,253 relocations, first mismatch +0x50. No executable
bytes are promoted. The pressure witness changes other switch cases and is
280 bytes short; it is preserved privately, not substituted for the better
guarded body.

### Baseline and identity controls

The assignment gate returned `base-only`. The configured full-TU build and
direct stock build reproduce the retained score. The actual preprocessed
compiler input passes its self-context comparison. Stock, captured and
instrumented outputs pass section, relocation and symbol fidelity; the stock
baseline also passes fidelity against the shared `ugen187/candidate.o`.
Every source cell has its own stock object, named Ucode/procedure-index map,
instrumented census, and both index/detail fidelity checks. The fresh index
contains 436 decision records; the supplied census summarizes 395 distinct
webs. Repeated decisions for a web are not distinct webs.

The supplied 187 landscape, census and residual report were used as resources;
none was regenerated. Source experiments use `lineage_census.census` with the
configured full-TU command redirected to the private source and capture
compiler. The ordinary `--compare` output is retained alongside the pressure
histograms. That comparison reports membership/split-count changes but does
not compare `numintf`; an unchanged split count alone can hide a degree change.
The tool was not modified.

The decisive attribution is a controlled source differential, not a
producer-emitted C name. Replacing the four literal `&D_800D3140` operands in
case 12 with an address cached in one nonescaping array element changes
baseline event 37, table 1162/chain 0, from **70 members and 29 split webs** to
**67 members and 26 split webs**. Its four original case-12 blocks are replaced
by one cache-initialization block; the other member blocks agree. This locates
the intervention on the invariant display-list **address**, not on a saved
coordinate value. No inference from the constant lineage `line` field is used.
Creation-event numbers are not a count of successive calls to the splitting
algorithm.

### The pressure witness

Attempt 9 caches the address of the same global object at entry and uses that
cached address at all 70 original operands. It never snapshots the mutable
display-list pointer itself. The address lineage has only the entry member
and no allocator web. Because first-use order changed, its run-local identity
is now event 2, table 938/chain 0; baseline table numbers cannot simply be
carried across this edit.

Attempt 13 starts from that exact control and restores the four original
literal-address operands in case 12. Thus case 12's original calculations,
loads, stores and calls are unchanged, while other cases still use the cache.
Comparing **attempt 9 with attempt 13** identifies the same table 938/chain 0
growing from one member to five: entry plus the four original case-12 blocks.
It now has **one web, zero splits, one natural colour**, with interference
**39**, eight registers left, and colour 20. No force was applied.

This is the requested reachability witness. It also shows that the large
interference degree depends on the address's uses in disjoint switch cases;
it must not be read as that many simultaneously live C scalars at the capture.
It does **not** prove repair of the earlier two-component coordinate-reuse
defect or closure of the remaining instruction gap.

The witness is **3,534 masked / 3,534 raw**, 14,176 owned bytes, delta -280,
frame 0x138, 1,116 relocations, first mismatch +0x4. It is neither object-exact
nor linked/ROM-proved. There is only one coloured web in this shortened
address family, so it supplies no pair of coloured children for a joint force.
The baseline event-45 family still has 28 webs, 26 split and two coloured.

### Source attempts

All arrays below are private automatic storage whose address does not escape.
Address caches preserve the exact global-object address passed to each call.
Union controls use disjoint switch-arm lifetimes or finish reading the array
member before replacing it with the pointer member; they never read an
inactive member. Original global access timing, signed arithmetic, bounds and
call order are preserved. The seven exhausted coordinate-capture families
were not repeated.

Scores below are masked differing words; deltas are bytes.

1. Case-12 stride carried by countdownX: 273 masked, delta 0; 29 split; pressure unchanged.
2. Case-12 stride in one array element: 1728 masked, delta 0; 29 split; pressure 130–141.
3. Entry state pointer in one array element: 2528 masked, delta +24; 29 split; pressure 129–139.
4. Row height in one array element: 2527 masked, delta -12; 29 split; pressure 129–139.
5. Combine state and row-height homes: 2105 masked, delta +36; 29 split; pressure 128–139.
6. Row base in one array element: 3525 masked, delta +32; 29 split; pressure 129–139.
7. Case-12 invariant address in an array: 2484 masked, delta -16; 26 split; pressure 128–139.
8. Same address in a new scalar: 195 masked, delta 0; Original 29 split; pressure unchanged.
9. Cache invariant address for all cases: 3552 masked, delta -280; Entry-only lineage; no allocator web.
10. Case-12 address in existing cursor: 187 masked, delta 0; Original 29 split; pressure unchanged.
11. Address array shares cursor storage in a union: 2511 masked, delta -4; 26 split; pressure 127–136.
12. Union cache restricted to the row nest: 2472 masked, delta +20; 28 split; pressure 129–139.
13. Attempt 9 with original case-12 operands restored: 3534 masked, delta -280; One coloured web; pressure 39.
14. Array seed, then scalar reload after first font call: 3306 masked, delta +40; 26 split; pressure 129–139.
15. Union seed, then existing slot pointer: 1727 masked, delta +4; 26 split; pressure 126–135.
16. Attempt 14 reload before the first font call: 3267 masked, delta +36; 26 split; pressure 130–139.
17. Attempt 15 using the same-typed saves pointer: 1728 masked, delta +4; Same pressure/splits as attempt 15.
18. Union seed and scalar share their home: 2511 masked, delta -4; Reproduces attempt 11's pressure/splits.

The compact controls therefore reach the original family too, but none colours
it. Attempt 15 is the smallest-pressure compact witness; its 14,460-byte
object has frame 0x138 and 1,264 relocations. Its four excess bytes and poor
residual are not accepted. An aligned comparison of attempt 11 also shows
substantial regressions, rather than merely a positional displacement penalty.

The last improvement in the compact series is attempt 15. Attempts 16–18
repeat the observed spill/scalar tradeoff, preserve the same split outcome,
and do not improve the retained residual or the best pressure witness.
This is the three-attempt stall record. No unrestricted impossibility claim
is made, and the function is not declared unmatchable.

### Preservation and next action

Private packet: `matching-evidence/wv-b-20260913.tar.gz` under Git's common
directory. It retains all 18 sources, hypotheses, configured commands,
compiler inputs, stock/index/detail objects, procedure maps, fidelity reports,
scores, censuses and comparisons. The supplied baseline census is copied into
the packet so comparisons survive lane removal. Reproduction uses the retained
`measure.py` adapter, `tools/lineage_census.py --compare`,
`tools/allocator_trace_receipt.py --map-only`, and workbench fidelity. Baseline
scoring also uses `tools/wb_compare.sh --summary-json`; direct object scores use
the repository's `force_lattice._score`/`nm_ranking` comparator.

`tools/finalize_plateau.py` refreshes the guarded-source handoff metadata and
its shard. This companion report keeps the shard below the tracked-file size
limit. The final gate/commit receipt accompanies the handoff. The ROM
gate exercises the assembly fallback, not any pressure diagnostic.

The next concrete investigation is to use attempts 9/13 as the positive
address-lifetime control and attempts 11/15 as the compact negative controls:
find a target-size spelling that reduces interference without replacing the
target's address-materialization surface with stack traffic. First distinguish
this address family from the historical coordinate-reuse split components;
do not assume a low degree alone repairs that separate gap. A new joint-force
packet needs two actually coloured, mapped webs on its own source, not the
baseline's stale web numbers. No merge, rebase or push was performed.
