# Whale address interpolation: lane wv-c, 2026-09-13

Target: `func_overlay_058_F000138C_18B0574`, overlay 58, owned text offset
0x138C, 14,456 bytes. The [per-function handoff](matching-triage-handoffs/func_overlay_058_F000138C_18B0574.md)
retains the campaign history; [the previous lane](whale-address-lifetime.md)
supplies the address-cache endpoints.

**The measured interpolation is nonempty, but there is no residual improvement.**
Attempt 19 has target extent, frame 0x138 and exactly the baseline stack traffic.
The original address family's descendant-web mean interference falls from
135.45 to 134.52, its minimum from 131 to 121, and its split count from 29 to
21. The distribution is mixed: its maximum rises from 140 to 143 and its median
from 133 to 136. This is a modest family-level movement, not a pointwise
reduction for every old web or a crossing of the colouring threshold.

The stronger no-stack witness, attempt 23, reaches **numintf 101–119** across
21 split webs, but is **four bytes too long**. The canonical guarded body stays
**187 masked / 358 raw**, delta zero, frame 0x138, 1,253 relocations and first
mismatch +0x50. No executable bytes are promoted; all new source variants remain
private evidence. The finalizer banks the measured finding with that better
retained body.

## What changes the address identity without an array cache

Converting an invariant global-object address through an ABI-width integer and
back to its original pointer type creates a separate type-4 expression lineage
in this compilation. A plain `(MenuCommand **)(u32)&D_800D3140` is sufficient:
removing an initial OR-zero probe reproduces the same code and pressure. A
signed 32-bit intermediate also reproduces the unsigned case-6 result. This is
measured behavior on this function, not a general compiler reachability theorem.

Applied at every operand outside case 12, the plain conversion leaves the
original type-1 address lineage with 16 members, one naturally coloured web,
no splits and numintf 43. It is 188 bytes short. The replacement expression
family has 66 members, 19 split webs and 12 coloured webs. Thus some pressure
has moved into a different family; reporting only the original family's
single coloured web would overstate the result. This supplies a second positive
control without explicitly caching the address in an array.

These conversions preserve the original object address under the configured
32-bit IDO ABI. They never snapshot the mutable display-list pointer, dereference
an invented object, change global-read timing, or change the original calls.
The array-cache controls share a nonescaping automatic union with a cursor
whose uses are in disjoint switch arms. No inactive union member is read.

## Target-width witness and the adjacent stronger control

Attempt 19 converts only three case-9 address operands: the nested-grid draw,
the option title after that grid, and the input-display loop. It leaves the
initial title and final conditional character call literal. Its score is
**615 masked / 772 raw**, extent 14,456, frame 0x138, first mismatch +0x50,
and 1,263 relocations. It retains all 35 stack homes and exactly each home's
load, store, address-take and width counts: 435 touched instruction rows.
There is no new address-cache stack traffic. Display-list address relocation
pairs fall from 70 to 68; other allocation changes raise the overall relocation
count, so target relocation identity is not claimed.

The invariant-address attribution is a controlled source differential. In
this run the original address remains table 1162/chain 0, event 37. Compared
with the preserved baseline it loses exactly member blocks 556, 563 and 568,
and gains none. Its lineage has 67 members and 21 split descendants. The
replacement conversion family is run-local event 952, table 647/chain 1:
four members, one coloured web at numintf 18. The baseline event-45 family
still has 26 splits and two coloured webs. Neither low degree nor a fresh
web number is assumed to repair the separate coordinate-reuse components.

Attempt 23 adds the final conditional character call to that same conversion
family, removing original address member block 575 as well. It reaches 66
members and 21 splits at numintf 101–119, with the same 35 stack homes and
435 touched rows as the baseline. Its score is **1,813 masked / 1,921 raw**,
extent 14,460, frame 0x138, first mismatch +0x50, and 1,257 relocations.
The owned function has 3,615 words; its padded text section is larger and is
not counted as function width. Address relocation pairs fall to 67.

Attempt 21, converting just the case-9 header and grid calls, is a separate
warning. It holds target width and lowers mean interference to 132.15, but
adds one four-byte stack home with one store and one load: 36 homes and 437
touched rows. Its 546 masked score is not accepted as the no-stack endpoint.
Source containing no array cache does not prove that the compiler added no spill.

Aligned candidate comparisons also reject a scalar-score adoption. Attempt 19
adds 198 aligned differing rows relative to the retained source's target
comparison and introduces eight additional gap pairs. The prior coordinate
gap remains. Its target width therefore does not mean unchanged instruction
shape. Attempt 21 adds 101 aligned rows and two gap pairs. The workbench
classifies the stronger attempt 23 as structural with a one-instruction surplus;
its `structure-buckets` guidance calls for resolving geometry before allocation.
The surplus is not merely compiler section padding or one added no-op.

## Measured source cells

Scores are masked differing words; deltas are owned bytes. The pressure column
is the original address family's run-local descendant range. Ranges summarize
different split populations, not old web numbers paired across changed source.
All 28 cells retain their sources, hypotheses, objects, first mismatch,
procedure mapping, compiler fidelity and complete pressure histograms privately.

| Attempt | Source form | Masked | Delta | numintf | Splits |
|---|---|---:|---:|---|---:|
| 1 | Case 5 union cache | 2273 | +16 | 128–139 | 29 |
| 2 | Case 6 union cache | 2269 | +8 | 124–135 | 29 |
| 3 | Cases 7/11 union cache | 2263 | +8 | 127–134 | 26 |
| 4 | Unsigned OR-zero, all except case 12 | 3491 | -188 | 43–43 | 0 |
| 5 | Case 2 union cache | 3153 | -4 | 106–133 | 29 |
| 6 | Case 5 unsigned OR-zero | 213 | +0 | 132–141 | 29 |
| 7 | Plain unsigned cast, all except case 12 | 3491 | -188 | 43–43 | 0 |
| 8 | Case 6 unsigned OR-zero | 1547 | -8 | 128–137 | 29 |
| 9 | Cases 7/11 plain cast | 3526 | +32 | 129–138 | 28 |
| 10 | Case 6 first call pair | 216 | +0 | 132–141 | 29 |
| 11 | Case 2 header/portrait pair | 250 | +0 | 129–142 | 29 |
| 12 | Case 6 signed cast | 1547 | -8 | 128–137 | 29 |
| 13 | Case 2 first loop pair | 3089 | -4 | 126–141 | 29 |
| 14 | Case 2 final loop pair | 2973 | -4 | 126–141 | 29 |
| 15 | Case 6 wide cast | 1570 | -8 | 128–137 | 29 |
| 16 | Case 9 all five calls | 1002 | -12 | 101–120 | 21 |
| 17 | Case 12 address casts only | 2488 | -20 | 129–137 | 26 |
| 18 | Case 9 grid/exit pair | 987 | +4 | 110–140 | 22 |
| 19 | Case 9 grid/exit/input calls | 615 | +0 | 121–143 | 21 |
| 20 | Case 9 grid/exit/final calls | 1814 | +8 | 126–140 | 21 |
| 21 | Case 9 header/grid pair | 546 | +0 | 118–141 | 27 |
| 22 | Case 9 grid only | 1032 | +8 | 117–140 | 27 |
| 23 | Case 9 all four late calls | 1813 | +4 | 101–119 | 21 |
| 24 | Attempt 23 plus case 2 loop pair | 3103 | -16 | 112–119 | 19 |
| 25 | Attempt 23 plus case 10 row pair | 2486 | -12 | 113–120 | 19 |
| 26 | Attempt 23 via existing pointer | 1813 | +4 | 101–119 | 21 |
| 27 | Attempt 23 via existing integer | 1813 | +4 | 101–119 | 21 |
| 28 | Attempt 23 via fresh pointer | 1813 | +4 | 101–119 | 21 |

## Stopping evidence and preservation

The last substantial pressure finding is attempt 23. Disjoint source edits are
not additive: combining it with a case-2 pair measured at delta -4 produces
-16, not zero; using a later case-10 pair produces -12. Both combinations have
19 original-address splits but still miss target extent.

The final three carrier controls, attempts 26–28, do not improve the retained
residual, target-width witness or pressure tradeoff. The existing pointer and
integer carriers reproduce attempt 23's complete content sections, relocation
surface and symbol surface. A fresh pointer retains its size, scores and
pressure but shifts one existing stack home's accesses by four bytes, changing
eight instruction words. It is not byte-identical. This is the three-attempt
stall record for the available carrier mechanism, not an impossibility claim
about other source forms. No case-12 coordinate/capture family or case-3 cursor
experiment was repeated, and no forced-colour candidate is used or promoted.

The assignment gate returned base-only. The actual preprocessed baseline
self-context comparison passes; configured full-TU, private stock and supplied
ugen-resource objects pass content-section, relocation and symbol fidelity.
Every source cell has a named Ucode/procedure-index receipt mapping it to
ordinal zero, and its stock/index/detail objects pass fidelity. The supplied
187 landscape passes its freshness check and is read rather than regenerated.
Its census and residual resources are likewise reused.

Private packet: `matching-evidence/wv-c-20260913.tar.gz` under Git's common
directory. It includes the 28 cells, baseline, retained resource census,
measurement adapter, stock and instrumented objects, complete traces, context
checks, pressure comparisons, stack and aligned-delta reports, and gate logs.
`measure.py` derives from the previous lane's preserved adapter; its configured
command is freshly obtained from this checkout. Reproduction uses that adapter,
`lineage_census.py --compare`, direct object scoring, `residual_map.py --object
... --against ...`, and workbench `slots` and `fidelity`.

`tools/finalize_plateau.py` preserves the guarded 187 body and updates its
metadata and shard. Final gates are recorded with the commit receipt; ROM
verification exercises the assembly fallback, not any nonexact diagnostic.
A fresh-lane link failure was repaired by regenerating overlay aliases after
compilation; no generated alias change is retained. No merge, rebase or push
is performed.

The next source investigation should start from the exact three-call and
four-call case-9 controls above. A stronger resumption test is **target width,
unchanged stack traffic, and lower interference throughout the address family**,
with aligned regressions and relocation changes priced separately. The present
experiment proves a mixed target-width movement and a uniformly lower-pressure
near-width point. It does not establish that all three properties are reachable
together, and it supplies no coloured pair in the original address family for
a joint-force repair.
