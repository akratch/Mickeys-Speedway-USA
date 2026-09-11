<!-- plateau-handoff:func_overlay_101_F00063F8_18E1C18:start -->
### `func_overlay_101_F00063F8_18E1C18` plateau handoff

- source: `src/overlays/o101/func_overlay_101_F00063F8_18E1C18.c`
- score: 203/380 words
- frame: 0x38
- relocations: 49
- first mismatch: +0x9C
- summary: 203 masked words from 213; size, frame and instruction multiset all exact. Naming fell 167 to 104 once the pre-call counter read became a ring temp and the node pointer kept s0; the residual is as1 schedule, led by the counter-bump store.

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

<!-- plateau-handoff:func_overlay_101_F00063F8_18E1C18:end -->
