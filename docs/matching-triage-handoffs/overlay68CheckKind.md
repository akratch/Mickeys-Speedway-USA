<!-- plateau-handoff:overlay68CheckKind:start -->
### `overlay68CheckKind` plateau handoff

- source: `src/overlays/o068/overlay68CheckKind.c`
- score: 70/80 words
- frame: 0x48
- relocations: 9
- first mismatch: +0x50
- summary: All 101 legal colour forces are non-improving; isolate the UGEN emission-order identity.
#### 2026-09-11, lane `p6-small`: still ten, and two more mechanisms are closed

Unchanged at ten relocation-masked words, size delta 0, frame 0x48, first
mismatch +0x50. The 2026-09-10 decomposition holds exactly: one `bne` operand
order at +0x50, and a nine-word cluster whose root is that the target forms the
value cursor as a shift of the ZERO REGISTER followed by an add, which consumes
a temp-ring position and shifts every later temp by one (L127/L129). Reading
`register_census.py` after the float-bank fix confirms the shape: six integer
substitution sites, 100% coherent, one window, with a closed `v1`/`a1` cycle
for the `bne` and a straight `t4 -> t5 -> t6 -> t7 -> t8` run for the cluster.

New negatives, all measured at this base, all size delta 0 unless stated:

- **Block placement of the zero does not stop the fold.** 78 points: the
  `cursorIndex = 0` statement at each of twelve positions from function entry
  to the loop guard, each bare and each wrapped in an L97 region (`if (1)` and
  `do { } while (0)`), times the cursor assignment at each position at or after
  the division, plus both statements wrapped together. Every point that
  compiles to the same geometry scores ten; a region at function entry costs
  4 bytes. So the L97 lever that moved `func_8004BA8C` and `debug_text_width`
  on the same day does NOT reach this one, and the reason is that the fold is
  constant propagation, not interference.
- **A merge of two zero definitions does not defeat constant propagation.**
  Fifteen zero-provenance shapes: the assignment duplicated into both arms of
  the `threshold == 0` guard (four spellings), a ternary with zero on both
  arms, a single-trip `while`, a single-trip `do { } while (0)` around the use,
  the zero taken from `index` and from `result`, and byte-offset and
  address-of-index spellings of the cursor. The two-arm forms score 14: uopt
  still folds to a register move, merely later in the block. IDO's constant
  propagation meets across the arms.
- **Strength reduction does not supply the shift either.** Eight loop shapes
  that remove the explicit cursor and index the probe directly, so that the
  preheader address would be created by the induction-variable rewrite rather
  than by source (`probe->values[index]`, `((s16 *)probe)[index + 0x10]`, with
  the index incremented before and after the load, and with the cursor and
  index locals present, dropped, or both dropped). All ten or twelve.
- **A named -1 costs more than the operand order is worth.** The `bne` at +0x50
  is the same lever that worked on `func_8004BA8C`: a constant that is a
  VARIABLE keeps its source operand order, and a literal is canonicalised. Here
  the constant is shared by three sites (the initial `amount`, the map-entry
  test and the loop test), and naming it for any subset of those three, in any
  of three declaration slots, costs one word to materialise (21 points, every
  one 73 or worse). Six literal spellings of the loop test, including
  `-1 != currentKind` and `(currentKind + 1) != 0`, are byte-flat.

**Decision variable, unchanged and now better bounded:** what supplies an index
that is zero at run time and unknown to uopt's constant propagation, while
still colouring to the zero register. The target's instruction proves the value
reached the allocator as zero, so the requirement is a propagation barrier that
survives into ugen, not a runtime-unknown value.

#### 2026-09-12, lane `p10-near`: an exhaustive force sweep retires the colour axis

Baseline reproduces: 320 bytes, delta 0, 10 masked, first mismatch +0x50, frame
0x48. The instrumented toolchain's `.text` is byte-identical to the tree's for
this TU.

**The residual is not a colouring decision, and that is now an existence proof
rather than an inference** (L140). This procedure emits thirteen p1 decisions;
forcing every one of them onto every colour from c1 to c15 gives **146 accepted
forces** -- acceptance read from the `forced` field of each `p1color` record,
never from whether the object changed -- and the floor across all 146 is 10,
reached only by the incumbent colour of each web. So no move of any existing
web's colour touches these ten words, and the two previous lanes' reading of
the cluster as an address-folding fact rather than an allocation fact is
confirmed from the records.

**The L127 probe family does not reach it either.** The nine-word cluster is a
temp-ring phase difference downstream of the target's `sll`-of-zero, and L127
says a peephole-deleted no-op still draws a ring temp, so a probe placed between
the division and the loop should advance the ring one position and close five of
the nine. Twelve probes -- or-with-zero, and-with-minus-one and xor-with-zero on
the threshold numerator, the quotient, the value constant, the loop index, the
cursor index at its definition and at its use, the guard test and the guard's
arm -- are **all byte-identical**. L135 explains it: uopt folds identity
operations before the web builder, so on an integer that uopt already knows is a
constant, or that it can fold at the tree level, the probe never reaches ugen
and never draws.

Read off the objects, the ten words are exactly: the `bne` operand order at
+0x50; the cursor's two-instruction form at +0xC0 and +0xD4, where the ROM
emits a shift of the zero register and an add into the delay slot of an ordinary
branch while the candidate emits a register copy and a branch-likely whose delay
slot duplicates the following load; and five ring-phase words at +0x100..+0x120
that follow from it, ours running one position ahead of the ROM's.

**Reopen condition, unchanged in kind and narrowed in means:** the propagation
barrier must survive into ugen, and it cannot be built from an identity
operation (L135) or reached by a colour force (this pass). What is left is a
source form in which the element index is a variable at address-lowering time
and the zero register at allocation time.

#### 2026-09-12, lane `p23-lastmile4`: complete single-force colour control

The current baseline remains 320 bytes, 80 words, delta 0, frame 0x48, nine
relocations, and 10 masked words. Alignment splits the residual into six
naming and four structural rows over three windows. The 101 legal single-force
probes across eleven webs have no winner at delta 0; every changing force is a
regression or changes geometry.

The named source question remains upstream of allocation: identify the UGEN
ring or emission-order identity that creates the four structural rows, then
re-evaluate the naming component on that geometry.
<!-- plateau-handoff:overlay68CheckKind:end -->
