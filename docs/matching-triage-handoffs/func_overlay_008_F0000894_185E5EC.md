<!-- plateau-handoff:func_overlay_008_F0000894_185E5EC:start -->
### `func_overlay_008_F0000894_185E5EC` plateau handoff

- source: `src/overlays/o008/overlay_008.c`
- score: 333/381 words
- frame: 0x70
- relocations: 48
- first mismatch: +0x6C
- summary: Linked trial proves 48 in-range/zero out-of-range words; ten 0.8.0-guided tail reshapes are nonexact, so resume only with a new temp-slot-54/pool-slot-64 web mechanism.
2026-09-09 lane `lane/tu2-o008` maintenance evidence (base `304d363e`, score unchanged at 48
relocation-masked words / 57 raw, 381 of 381 instructions, exact `0x70` frame):

- The residual is entirely integer allocation. Both floating-point lanes are now
  measured identical (pool 19/19, temp 22/22) and the frame is exact, so no FP or
  frame mechanism remains in this function.
- The first divergence is a temp-ring state difference at aligned row 266, at the
  head of the owner-flag branch chain. The two objects emit the same shape there,
  the same number of distinct carriers and the same stores; the target keeps the
  middle carrier in a block temp while the candidate is forced to spend a pool
  register on it, and every later integer difference follows from that one
  displacement.
- Carrier scoping is eliminated as the mechanism. Declaring each branch arm's flag
  carrier inside its own block, and giving each arm its own distinct function-scope
  carrier, are both byte-inert at 48 words. IDO already separates the disjoint live
  ranges, so the shared name in the inherited source was never the cause and
  respelling it cannot be the fix.
- Combined with the previously recorded result that direct compound assignments
  regress to 56, the source-spelling space around this branch chain is now
  bounded on both sides: removing the carriers is worse and splitting them is inert.
  Resume only with evidence about the ring state entering that block.
<!-- plateau-handoff:func_overlay_008_F0000894_185E5EC:end -->
