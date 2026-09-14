# RowBase's saved colour, lane wv-o

**Retained source unchanged: 160 masked / 331 raw differing words, 14,456
bytes, delta zero, frame 0x138, first mismatch +0x50.** Aligned buckets remain
3485 exact, 119 naming, one immediate and eight structural rows. The original
gap pair, complete 720-draw sequence and 1,253 owned text relocations remain.
Cases 3 and 12, their reset placement and position-address reuse are untouched.
No executable bytes are promoted; the C remains guarded NON_MATCHING.

### The additional neighbours, measured directly

Focused, stock-faithful neighbour captures compare the inherited attempt 35
with bank160. Identity joins use the recorded lineage and expression identity,
not a comparison of stale web numbers. Entry keeps its saving of 152/16.
It gains the independent index, both generated coordinate expressions and
rowBase, while one old row-coordinate fragment disappears. Its decision count
rises from 47 to 50; the new coordinate and index neighbours occupy s0 and s1,
so entry takes s2.

RowBase keeps saving 7/13. It gains entry, the two generated coordinates and
two other generated array cursors, while losing one old coordinate fragment.
Its decision count rises from 51 to 55. The new edge to entry closes s2,
which was rowBase's sole remaining saved colour. Available-register count
falls from seven to six, and rowBase splits. This identifies the lost colour;
a raw interference count alone does not establish the cause.

The original rowBase use is before the positive-count guard. The generated
coordinate moves that use into the next allocator block, where the generated
entry cursor is already live. Moving the independent node-Y store before the
texture load does not change those two allocator decisions. Reassociating the
node's subtraction into its invariant base changes some aligned rows but also
leaves both decisions unchanged.

### An edge-removal witness, with a different cost

A one-element array representation of the captured row coordinate removes the
rowBase-entry edge and restores the earlier rowBase use. It lowers rowBase's
interference count to 50, but changes the other row-coordinate loops and
introduces a different s2 neighbour. A scalar/array union view is identical to
that array output; it does not confine the representation change to case 2.

Attempt 5 instead captures the initial base in the existing minutes cell,
represented as a one-element array. Minutes is otherwise unused on this
case-2 path, and its original time-result uses retain the same four-byte
storage and addresses in other cases. RowY's other recurrences remain scalar.
**Entry and rowBase now both take s2 naturally**, with no interference edge
between them: entry has 48 neighbours and rowBase 50. RowBase again has seven
available registers and its original sole saved choice. Their lineage member
blocks match the retained baseline. No colour force is used in this source
witness.

This meets the requested saved-colour condition, but does not meet acceptance:
**2497 masked / 2543 raw, delta -4, frame 0x138, 724 draws and 1,249 text
relocations**. Direct array-base uses add coordinate reload draws across calls.
The changed draw order and width reject it. Using a nonescaping countdown
array also retains both s2 colours, but keeps the local cost and adds changes
in its other case. Thus removing the edge is possible; doing so without the
memory-form coordinate cost remains unresolved.

### Source controls and stopping evidence

All nine source attempts preserve their full source, stock object, score,
first mismatch, aligned comparison, draw census and compiler-fidelity evidence
in the private packet. Every frame is 0x138; none preserves the complete draw
order. Counts below describe compiled diagnostics, not adopted candidates.

| Attempt | Source control | Masked / raw | Byte delta | Draws | Text relocations |
|---|---|---:|---:|---:|---:|
| 1 | generated node-Y store before texture | 3527 / 3532 | -16 | 719 | 1251 |
| 2 | subtraction associated with invariant base | 3527 / 3532 | -16 | 719 | 1251 |
| 3 | captured row coordinate in one-element array | 3504 / 3513 | +36 | 715 | 1241 |
| 4 | scalar/array union view for that cell | 3504 / 3513 | +36 | 715 | 1241 |
| 5 | base in existing addressable minutes cell | 2497 / 2543 | -4 | 724 | 1249 |
| 6 | base in nonescaping countdown array | 2490 / 2534 | +16 | 728 | 1249 |
| 7 | one scalar reload after the array capture | 3497 / 3505 | -8 | 720 | 1251 |
| 8 | signed-16-bit round-trip in scalar capture | 2312 / 2358 | 0 | 723 | 1249 |
| 9 | rowBase declared at its table's signed width | 3224 / 3233 | +84 | 722 | 1296 |

Attempt 5 is the last new colour result. Attempt 6 controls for address escape
without removing the cost. Attempts 7-9 then fail to improve the retained
residual or supply a usable new allocation mechanism: loading the array base
once into rowY reintroduces rowBase's later-block use and split; the range
round-trip also retains that later use and split; narrowing rowBase itself
changes the menu induction and produces broad width/relocation costs. These
three consecutive failed repairs are the ADR 0018 stall. Neither 720 draws
without their order nor delta zero without the other guards is progress.
This is a plateau of these mechanisms, not an unrestricted impossibility claim.

### Reproduction and preservation

The assignment gate returns base-only. The configured full-TU object, private
baseline, detailed trace and named-Ucode capture pass fidelity; the actual
baseline's self-comparison is exact. Every source attempt has its own named
Ucode procedure mapping and indexed-output fidelity. The inherited oracle is
JFG frontKeyboard at 0.081; no donor material is adopted.

A fresh exhaustive same-kind and cross-kind scan reproduces **all 1,915
inherited probe rows exactly**: 520 target-width acceptances, 1,146 different
widths and 249 fail-closed declines. There are nine single-force winners and
no omitted or deferred pairs. The source is pinned independently of the
ranking fingerprint and the trace baseline passes stock fidelity. The existing
four-force lattice is not repeated; its 125 diagnostic is inherited evidence.

Fresh analysis-only preflight reproduces 1,253 records on each side, 1,238
offset/type agreements and 675 stable/effective identity agreements. Candidate
resolution remains 862 resolved and 391 unresolved; exact relocation identity
is not claimed. Regenerating overlay aliases repairs the fresh-lane link
without changing a tracked alias file. ROM verification uses the assembly
fallback, not the nonexact C.

The closing gates pass: `verify`, `cleanroom`, `check-docs`,
`check-scoreboard`, `check-overlay-syms` and `check-nonmatching-builds`, all
run through `tools/gates.sh`. Final comparison again gives 160 masked and
331 raw differences. The source body before EOF metadata is unchanged.

The private packet is `matching-evidence/wv-o-rowbase-colour-20260914` under
Git's common directory. It includes the neighbour captures, both-colour
witness, all nine attempts, fresh landscape, receipts, final gates and handoff.
`tools/finalize_plateau.py` preserves the original body and updates only its
EOF metadata. No merge, rebase or push is performed.

A future source packet should start with attempt 5's **absent entry edge**,
not merely its low interference count or correct colour. It must retain that
earlier base use while removing the repeated memory-coordinate draws, with
full width, relocation, reset/reuse and draw-order guards. The single scalar
reload, scalar/array union view and signed-width repairs above are already
measured and should not be repeated.
