<!-- plateau-handoff:func_overlay_101_F000571C_18E0F3C:start -->
### `func_overlay_101_F000571C_18E0F3C` plateau handoff

- source: `src/overlays/o101/func_overlay_101_F000571C_18E0F3C.c`
- score: 222/443 words
- frame: 0x40
- relocations: 59
- first mismatch: +0x10
- summary: 222 masked words from 263; size, frame and the s0..s8 slot map all exact. Residual is 178 register naming, by L114 mostly ring phase.

Measured 2026-09-11, lane `lane/o12-quad`. Numbers are `tools/align_symbol.py`.

Before: 263 masked words, size exact at 443 words, aligned 203 byte-exact, 187
register naming, 5 immediate only, 57 really different, 6 insertion and 6
deletion sites. After: 222 masked, 228 byte-exact, 178 register naming, 1
immediate only, 38 really different, 2 insertions and 2 deletions. First
mismatch is at +0x10 and the first thirty-three instructions are byte-exact.
The frame was already exact at 0x40 with ten slots, and remains so.

Two levers.

- L59, the fold. The four non-macro assignment groups are ONE physical line
  each, for the reason stated in the F63F8 shard: folding retires as1's
  `lineno` key so the ready-list order supplies the reversal the ROM emits.
  Worth 263 to 236, structural 57 to 28, insertions 6 to 2.
- L110 and L115. The FIRST node group reads the pool counter inline,
  `node32 = &D_340[D_1CC]`, and only the post-call half names the index local.
  The ROM puts that pre-call read in a scratch register and the post-call one
  in the counter's callee-saved home; one local spanning both mis-colours the
  pre-call half. Worth 236 to 222. It is specific to the first group: the same
  change inside the image macro reads 297 and inside the text macro 364.

Refuted here, each measured against the folded 236 baseline. The call-spanning
edit that closed the missing sixth web on the F78F4 quadruplet -- storing
handle, previousType and previous through the pre-call pointer -- is a
regression on this function: 305 with all three moved, 242 with the handle
store alone, 248 with the other two. The reason is structural and worth
recording: this function's ladder already homes nine slots, its s0..s8 slot map
already equals the ROM's, and there is no web left to win, so the edit only
widens a live range that is already correctly coloured. The L100 counter
partition likewise does not pay -- splitting the node and text counters reads
346 at a size delta of 8, the F63F8 shape reads 383 at 8, one local for all
three reads 283, and giving the owner counter its own local reads 236, against
236 for the incumbent.

Measured inert: the x and y store order in the first node group, once the pool
read is inline.

Next lever, with the decision variable named. There is no missing web: the
instrumented uopt shows every remaining split web preferring a caller-saved
colour, and the nine coloured webs already land on the ROM's registers. The
residual is 178 naming words, and the first divergence at +0x10 together with
the surviving 38 structural words is ugen ring phase -- the pre-call node
pointer takes v0 where the ROM reuses s1. By L114 that is phase, not colour, so
the axis is the as1 dependence graph and ugen's ring rotation, not any save
ratio or spelling.

<!-- plateau-handoff:func_overlay_101_F000571C_18E0F3C:end -->
