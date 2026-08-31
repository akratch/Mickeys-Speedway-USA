<!-- plateau-handoff:overlay74Update:start -->
### `overlay74Update` plateau handoff

- source: `src/overlays/o074/overlay74Update.c`
- score: 61/100 words
- frame: 0x70
- relocations: 8
- first mismatch: +0x0
- summary: Target frame is 0x60; outer-array, late-carrier, and combined lexical scopes are byte-flat, leaving the 16-byte frame/lifetime blocker.

Fresh configured evidence on 2026-08-31 reproduces an exact-sized 400-byte
candidate with 61/100 exact positional words, 39 masked/raw residual words,
and the first mismatch at function offset `+0x0`. The candidate frame remains
`0x70` versus the target's `0x60`; all 8 relocation offsets and types align.

The bounded new mechanism pass tested three coherent lifetime forms: placing
the 13-pointer result array inside the outer conditional, placing the late
`count`/`flagBits`/`mask` carriers inside their use block, and combining those
two scopes. Each compiled byte-identically to V0. No natural strict gain was
available to gate a permutation batch, and the previously exhaustive flag
lattice was not repeated. Resume only with a genuinely new producer/frame
allocation mechanism.
<!-- plateau-handoff:overlay74Update:end -->
