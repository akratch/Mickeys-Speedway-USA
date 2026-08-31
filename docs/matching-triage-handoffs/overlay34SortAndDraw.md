<!-- plateau-handoff:overlay34SortAndDraw:start -->
### `overlay34SortAndDraw` plateau handoff

- source: `src/overlays/o034/overlay34SortAndDraw.c`
- score: 128 differing words
- frame: 0x1A0
- relocations: 16
- first mismatch: +0x14
- summary: A fidelity-gated phase-local block moved ten home operands but left the 128-word target distance flat; the one-word deficit remains.

#### Whole-itable phase-local reproof (2026-08-31)

- assignment base: `d63836a7`; configured V0 reproduces a 760-byte target
  against a 756-byte / 189-word candidate, 129 raw and 128 relocation-masked
  differences, first mismatch `+0x14`, and the exact target frame `0x1A0`.
- IDO 5.3 `uopt.c` was regenerated from static-recompiler revision
  `9c242adc` at the pinned `b0058f15...` source digest, instrumented with the
  shipped global-color profile and preserved whole-itable patch, and built in
  a lane-local toolchain copy. With tracing enabled, `.text`, `.data`,
  `.rodata`, relocations, and symbols are identical to stock output.
- the baseline ladder contains 17 slots and separately exposes the large
  distance-array region, the sort swap home, and the later address-taken color
  and interpolation-scalar homes. This validates the phase-local hypothesis
  without inferring names from raw compiler records.
- the single authorized source form placed the distance/swap locals and the
  render-only colors/scalars in disjoint lexical blocks. It changes ten
  stack-home operands: the distance family moves only four bytes while the
  render homes cross the array region, rather than producing the measured
  20/24-byte reuse. Against retail it retains 189 words, frame `0x1A0`, all
  128 masked differences, and the seven relocation sites that remain four
  bytes early.
- the alias-splitting follow-up was not attempted because the scoped form
  produced no strict geometry gain. Preserve the retained source and reopen
  only when producer evidence attributes a specific home or identifies the
  missing instruction; do not repeat scopes, declaration shuffles, flags, or
  broad permutation.
<!-- plateau-handoff:overlay34SortAndDraw:end -->
