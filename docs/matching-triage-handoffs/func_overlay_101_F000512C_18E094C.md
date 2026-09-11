<!-- plateau-handoff:func_overlay_101_F000512C_18E094C:start -->
### `func_overlay_101_F000512C_18E094C` plateau handoff

- source: `src/overlays/o101/func_overlay_101_F000512C_18E094C.c`
- score: 169/380 words
- frame: 0x38
- relocations: 49
- first mismatch: +0x8C
- summary: 211 masked words; the diagnosed sibling's two levers transferred whole, structure 131 to 40 and insertions 23 to 7.

Measured 2026-09-11, lane `lane/o11-triplet`. This function is one of three
byte-identical-sized siblings in the overlay-101 builder family; the mechanism
was diagnosed on `func_overlay_101_F00063F8_18E1C18` and transferred here, and
the two levers are stated in full in that symbol's shard.

Before: 298 masked words, aligned 162 byte-exact, 105 register naming, 5
immediate only, 131 really different, 23 insertion and 23 deletion words.
After: 211 masked, 178 byte-exact, 169 register naming, 0 immediate only, 40
really different, 7 insertions and 7 deletions. First mismatch moved from
+0x2C to +0x8C.

The transfer was mechanical and complete. The same two edits -- folding each
per-element assignment group onto one physical line (L59), and partitioning the
counter local so that `index` carries the panel-order counter D_1C4 and the
text-row counter D_1D0 while `slot` carries the two node counters (L100, L115)
-- were applied by line range with no per-function tuning, and this sibling
landed two words better than the function they were diagnosed on. Nothing was
re-searched here; the negative results recorded on the diagnosed sibling cover
this function by construction, since the bodies differ only in constants,
symbol names and one local declaration order.

Next lever is the same one, and is named in the diagnosed sibling's shard: the
first divergence is `slot`'s register, the ROM splitting it across two scratch
registers where the candidate holds one dedicated caller-saved register, and
the whole naming residual is the ugen ring phase downstream of that.

<!-- plateau-handoff:func_overlay_101_F000512C_18E094C:end -->
