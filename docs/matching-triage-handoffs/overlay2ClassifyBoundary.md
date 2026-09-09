<!-- plateau-handoff:overlay2ClassifyBoundary:start -->
### `overlay2ClassifyBoundary` plateau handoff

- source: `src/overlays/o002/overlay2ClassifyBoundary.c`
- score: 62/79 words
- frame: frameless
- relocations: 6
- first mismatch: +0x4
- summary: The +1 word is the shared jr ra's unfilled delay slot: two bc1fl selections steal the join block's li v0,1, which the target has already scheduled into that delay. Census is bc1f -2, bc1fl +2, li +2, nop -1. Ten source shapes and the whole flag lattice are flat; the lever is making v0 unavailable so both stack pointers take v0/v1.
<!-- plateau-handoff:overlay2ClassifyBoundary:end -->
