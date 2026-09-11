<!-- plateau-handoff:func_overlay_101_F00063F8_18E1C18:start -->
### `func_overlay_101_F00063F8_18E1C18` plateau handoff

- source: `src/overlays/o101/func_overlay_101_F00063F8_18E1C18.c`
- score: 136/380 words
- frame: 0x38
- relocations: 49
- first mismatch: +0x9C
- summary: 136 masked words; the byte-length local is u8, which collapsed the naming bucket by more than four to one and left a clean one-position ring shift from +0x2BC.

Measured 2026-09-11, lane `lane/s1-o101`. Every number is
`tools/align_symbol.py`.

Before: 213 masked, 178 byte-exact, 167 register naming, 1 immediate only, 41
really different, 7 insertions and 7 deletions.
After: 203 masked, 209 byte-exact, 104 register naming, 4 immediate only, 79
really different, 16 insertions and 16 deletions. First mismatch moved from
+0x8C to +0x9C. Size delta 0 and the frame ladder matches at all eight offsets
both before and after.

**The previous closure named the right decision variable and rejected the right
edit for the wrong reason.** It said the ROM splits the slot local across two
scratch registers, one per call side, where the candidate holds a single
dedicated caller-saved register, and it recorded "dropping `slot` for a direct
counter read anywhere" as regressive at 331 and "for the pre-call half only" as
215 with worse structure. Both halves do regress on their own. Doing BOTH halves
does not: `node32 = &D_340[D_1CC]` and `node20 = &D_200[D_1C8]` on both sides of
each call, with the `slot` local deleted, reads 203.

The two halves fix different things and each one alone breaks what the other
fixes. The pre-call direct read is what turns the pre-call counter value from a
globalcolor web into a ugen ring temp, which is what the ROM has; that alone
moves the first three instructions of the node block to byte-exact and fixes the
ring phase for the rest of the function, which is the whole 167-row naming
bucket. But on its own it also splits the node pointer, because one definition is
then a direct expression and the other is still through the local, and a split
pointer loses the ROM's callee-saved home. Making both definitions the same
expression keeps the pointer in the ROM's s0 and lets the post-call counter take
the ROM's a1. Pre-call only reads 215, post-call only reads 314, both together
203.

The counter bump must stay in front of every store, for the same reason as on the
F78F4 quadruplet: a bump written behind a store makes uopt re-read the counter.

**L114 does not classify anything here, and the boundary is per procedure.** This
procedure's `p1cost` rows decode as c2 `v1`, c4 `a1`, c5 `a2`, c6 `a3`, c7 `t0`,
c8 `t1`, c9 `t2`, c10 `t3`, c11 `t4`, c12 `t5`, c13 unnamed, c14-c22 `s0`-`s8`,
and globalcolor assigns three t-bank colours outright in this compilation -- two
webs take c7 `t0` and one takes c8 `t1` -- beside 22 webs it declines to ugen. So
a naming row spelled with a `t` register is not evidence of ring phase in this
function. The lever above rests on something narrower and measured: `t5`, the
register the ROM's pre-call counter needs, is not among the colours globalcolor
assigns here, so that value has to reach the ring, and the direct counter read is
what sends it there. Re-decode the table from your own procedure's records before
classifying any row.

Swept exhaustively and inert or worse: all 180 orders of the six post-call
statements with the bump spelled directly, all 540 orders with an index local
added, and both with and without the pre-call change -- 203 is the floor and
every order that reaches it puts the bump first. Dropping the now-unused `slot`
declaration is inert (L99), as is adding unused declarations.

Next lever, with the decision variable named. The residual is as1 schedule, and
the first divergence is the counter-bump STORE. The ROM computes the bump from
the post-call counter register and emits its store late, after the previous-link
and chain-type stores; as1 emits it immediately after the recompute here, because
that is where the source has to put the bump to keep the counter load from being
re-issued. So the axis is not statement order -- 540 orders say so -- it is
whether the bump's load can sit early while its store sinks, which needs either a
carrier uopt is willing to keep in the ring or an aliasing fact that lets as1
move the store. Read the dependence graph with `cc -Wa,-R` before trying another
spelling. Note also, against an assumption this lane made and then disproved: as1
DOES reorder stores whose base registers differ, observed here moving the handle
store past the chain-type store, so "store order is source order" is not a law to
build on.


Lane `lane/p4-pres`, 2026-09-11, re-measured this cluster and moved it twice.
Both edits are source shape and neither costs an instruction.

The byte-length local is `u8`, not `s32`. That is the whole of the second and
larger gain, and no lane had swept the axis. A 360-cell type lattice was
measured -- the callee's declared return type across six spellings, the local's
own type across five, the store cast across three and the opacity mask across
four -- and every cell that reaches the floor has the LOCAL declared `u8`,
while the return type is indifferent across all six spellings. It is
semantically exact rather than a coincidence: both uses of the value already
truncate to eight bits, the node field being `u8` and the opacity expression
masking with `0xFF`, so the mask becomes redundant and the declaration simply
tells the compiler what the code already guarantees.

The naming bucket is what moved, and it moved the way an offset-resolved
register census predicted. Before the edit the substitution sites split into
ten windows in which the permutation is internally consistent but changes at
each boundary, and the window sizes repeat -- 14 then 7, 14 then 7 -- once per
text row, which is a per-iteration ring-consumption difference rather than a
colour question. After it, naming falls by more than a factor of four and what
is left is a single clean one-position shift over the integer temp ring.

The other lever the dispatch named, the counter-bump STORE, is exhausted. 659
constrained permutations of the whole post-call block -- the bump, the three
node stores and the two root stores, under the two real dependences -- were
measured across four bump spellings (`x = x + 1`, `+=`, pre-increment and
post-increment), and every one of them is flat at the pre-type-lattice plateau.
The earlier shard's reading that "the axis is what lets the bump's load sit
early while its store sinks, not the statement order" is confirmed, now over
the whole block rather than the six statements it had swept.

Next lever, with the decision variable named and a measured boundary around it.
The residual is a ring PHASE: the candidate's integer temp ring runs exactly one
position behind the shipped code from +0x2BC onward, ours `t2` against their
`t3`, `t3` against `t4`, `t6` against `t7`, `t7` against `t8`, `t8` against `t9`
and `t9` back to `t2`, and 83 percent of all substitution pairs are consistent
with that one permutation. That is the exact shape L127 describes. **It is not
reachable by an L127 free-list consumer from this source, and that is measured,
not assumed.** 155 no-op cells were swept: every read of the four counter
globals and the loop index, wrapped with OR-zero, AND-minus-one, XOR-zero, a
byte mask and two doubled forms; then every read of a root struct field and of
the call result, pointer fields included through an integer round trip. All 155
are exactly flat. So either the phase fault is established before any source
point a no-op can reach, or the rows are globalcolor colour after all, which
L114 as corrected for this procedure permits -- this procedure assigns three
t-bank colours outright. Decide that first from the `p1color` records before
spending another lattice on no-ops.

<!-- plateau-handoff:func_overlay_101_F00063F8_18E1C18:end -->
