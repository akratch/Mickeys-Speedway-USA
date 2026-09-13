# Whale target-width pressure witness: lane wv-d, 2026-09-13

**The four bytes can come off without new stack traffic or lost address
pressure.** Attempt 11 takes the previous lane's attempt 23 to **14,456 owned
bytes, delta zero, frame 0x138**, with the same 35 stack homes and exactly the
same load, store, address-take and width counts: 435 touched instruction rows.
The original display-list address family retains 66 members and 21 split
webs. Its interference range improves from **101–119 to 98–119**.

This is the assignment's width/traffic/pressure result, not a match or a
residual improvement over the retained source. The witness scores **1,446
masked / 1,570 raw**, first mismatch +0x50, with 1,257 owned relocations.
It changes temporary-register draws and has more aligned residual than attempt
23. The guarded canonical body remains the better residual at **187 masked /
358 raw**, delta zero, frame 0x138 and 1,253 relocations. Both complete source
positions are preserved; no executable bytes are promoted.

Target identity is `func_overlay_058_F000138C_18B0574`, overlay 58, text
starting at offset 0x138C. See the [interpolation report](whale-address-interpolation.md)
and [per-function shard](matching-triage-handoffs/func_overlay_058_F000138C_18B0574.md)
for the inherited positions and campaign history.

## The source repair

Keep all four late case-9 address conversions from attempt 23. In the cases
7/11 transition path, change only the later equality-to-four test:

```c
if (*(u32 *)&D_o058_5EB0 == 4U) {
```

Its original form reads the declared signed object and compares with signed
four. The replacement reads through the corresponding unsigned integer type.
Both are four-byte accesses to the same aligned object. Equality to four has
the same truth value for every original 32-bit representation; no range
assumption about the state is needed. The first equality-to-zero test remains
original. No source storage, dummy instruction, extra access, global snapshot,
call, or change to call order is introduced. The four existing address
round-trips preserve their pointer values under the configured 32-bit ABI.

The unsigned equality makes the known value four available in a saved
register across the calls inside that branch. Two subsequent byte stores can
reuse that value, removing two separate constant materializations. The unsigned
equality first needs one additional load because its identity is distinct from
the earlier signed zero test. Those three changes give the net one-instruction
reduction.
The initial level-address materializations are not individually deleted by
this final source form.

Attempt 10 also casts the first zero-test value to unsigned. Removing that
cast in attempt 11 reproduces its complete content sections, relocations and
symbol surface. Thus the first test needs no edit. Attempt 7, which changes
both tests' lvalue types, is another target-width witness but has a worse
residual, 1,485 masked / 1,603 raw.

## What the censuses actually say

The original address family remains table 1162/chain 0, event 37, with the
same member blocks as attempt 23. Its complete descendant-degree histograms
are:

| Source | Interference degree: descendant count |
|---|---|
| Original 187 | 131:1, 132:10, 133:5, 135:1, 139:3, 140:9 |
| Previous attempt 23 | 101:1, 103:1, 111:2, 112:13, 118:2, 119:2 |
| New attempt 11 | 98:1, 100:1, 111:2, 112:13, 118:2, 119:2 |

The first source has 70 members and 29 splits; the two later sources have 66
members and 21 splits. No descendant is naturally coloured. The event-45
family still has 28 webs, 26 split and two coloured; its pressure histogram
is unchanged. There is therefore no newly coloured pair in the original
address family for the requested joint-force experiment.

The supplied 187 draw profile was reused. Restored attempt 23 has **720 draws
in exactly the same order**, but **5,056 emission records**, down from 5,072
across 34 changed source lines. Its final function is nevertheless one
instruction longer. The premise that one final extra instruction necessarily
appears as one extra emission at one line does not hold for this comparison.

Attempt 11 has **719 draws and 5,061 emission records**. Its net final-word
reduction coexists with an increase in emission events relative to attempt 23.
The changed draw sequence changes later temporary-register choices. The
repository's aligned target comparison grows from 755 differing paired rows
for attempt 23 to 1,015 for attempt 11, a **260-row regression**; gap rows are
reported separately. The lower positional score does not turn that into an
aligned improvement. This costs the new width/pressure position substantially
more residual than the retained 187 body and is why both positions are kept.

## The new colour measurement

A fresh landscape on attempt 23 completes **1,920 probes over 142 webs**,
including both save kinds. This does not repeat the supplied 187 landscape.
Of 54 accepted target-width forces, 38 also preserve the full stack-traffic
signature. The best such diagnostic is **453 masked**, at either
`p1:w1006=c7` or `p1:w1006=c2`; the original address family has degree 100–119
and 21 splits. These are trace-local IDs on attempt 23.

Web 1006 is the one-member grid-format address family, table 1095/chain 1,
member block 550. Its caller-colour controls stop hoisting that format address
and change the allocation of the level-ID address family. A source-level
integer round-trip on the format pointer changes four instruction words but
retains the score, geometry and pressure: it does not reproduce the force.

A second useful diagnostic is `p1:w172=c14`: 1,806 masked, target width,
identical stack traffic and the exact attempt-23 pressure histogram. This is
the transition-state load family, table 190/chain 0. Its branch scheduling
suggested the unsigned-load experiments, although the final source removes
different words through the known-four reuse described above.

The landscape's unforced source has delta +4. Its positional radii against
forced delta-zero objects span different geometry, so no additive packing or
joint-colour prediction is inferred from those radii. Direct aligned candidate
comparisons, owned extents and stack/pressure measurements guide the source
experiments. All forces remain private diagnostics.

## Source cells and limits

Scores are masked differing words; deltas are owned bytes. Every cell retains
its source, hypothesis, stock/index/detail objects, actual compiler input,
procedure mapping, score, first mismatch and full pressure census privately.

| Attempt | Change relative to previous attempt 23 unless noted | Masked | Delta |
|---|---|---:|---:|
| 1 | Integer round-trip at first level-ID load | 1813 | +4 |
| 2 | Same load with constant OR-zero address identity | 1813 | +4 |
| 3 | Same load with defined index-times-zero identity | 1813 | +4 |
| 4 | Integer round-trip on grid-format pointer | 1813 | +4 |
| 5 | Case-2 row initialization moved inside positive-count branch | 2933 | +4 |
| 6 | Unsigned lvalue for cases-7/11 zero test | 1885 | +4 |
| 7 | Unsigned lvalues for zero and four tests | 1485 | 0 |
| 8 | Unsigned zero-test lvalue, value cast at four test | 1591 | -4 |
| 9 | Unsigned value casts at both tests | 1565 | -4 |
| 10 | Unsigned zero-test value, unsigned four-test lvalue | 1446 | 0 |
| 11 | Only unsigned four-test lvalue | 1446 | 0 |

Attempts 1–3 retain the same score and pressure but each changes seven
instruction words around argument preparation; they are not byte-identical
to attempt 23. Attempt 4 changes four words and relocation offsets. Their own
stock/instrumented fidelity passes; cross-source differences are expected.
Attempt 5 removes the targeted row-base copy but adds a replacement elsewhere.
Attempt 6 removes a branch-delay no-op but adds a reload for the later signed
test. The paired load-type measurements separate that trade-off from the
known-four reuse. The early identity probes remain ignored diagnostics.

This packet ends at its explicitly assigned width/traffic/pressure milestone.
It is not a three-attempt exhaustion claim or a proof that no better source
exists. The source result still needs substantial allocation and structural
work before matching. In particular, retaining the original draw count/order
while preserving the new width and pressure is an unproved stronger bar.

## Preservation and validation

Private packet: `matching-evidence/wv-d-20260913.tar.gz` under Git's common
directory. It includes the original 187 source and object, previous attempt 23,
all eleven new cells, the complete fresh force landscape, per-force width and
stack reviews, draw profiles, aligned comparisons, procedure receipts,
compiler fidelity and gate logs. The exact new source is
`wv-d/a11_single_unsigned_transition/source.c` inside that packet. The inherited
wv-c packet is referenced separately rather than duplicated in the archive.

The assignment gate returns base-only. The configured full-TU baseline and
private baseline pass content-section, relocation and symbol fidelity against
each other and the supplied 187 resource object. The actual preprocessed
baseline self-context comparison passes. Each new source has a named Ucode
stream and complete index receipt mapping it to procedure zero; its stock,
index and detailed-trace objects pass fidelity. No compiler or object bytes
are patched.

`tools/finalize_plateau.py` banks the finding in the source metadata and shard
while retaining the 187 guarded body. Closing gates and the commit receipt
accompany the handoff. The full ROM gate validates the assembly fallback,
not the nonexact pressure witness. No merge, rebase, push or release occurs.

Next action: start from preserved attempt 11 for further pressure work, or
use it as a controlled counterpoint to 187 when investigating its draw-order
cost. Re-map any coloured webs on the chosen source before forcing them;
attempt 23's landscape is not a fresh landscape for attempt 11.
