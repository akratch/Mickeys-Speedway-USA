<!-- plateau-handoff:overlay60ReassignChoiceSlots:start -->
### `overlay60ReassignChoiceSlots` plateau handoff

- source: `src/overlays/o060/overlay60ReassignChoiceSlots.c`
- score: 18/53 words
- frame: 0x20
- relocations: 8
- first mismatch: +0x4
- summary: Fidelity-clean CDX maps the choice pointer to first p2 web w0; forcing its zero-cost v1 tie cuts 35 to 13 words, proving retail needs earlier web formation, not a priority change.

#### Epoch 15 allocator-colour pass (2026-09-04)

- assignment base: `2d82a090`. Fresh configured C reproduces 53 target and
  candidate words, the exact `0x20` frame, 35 relocation-masked/raw positional
  differences, first mismatch `+0x4`, and an identical 15-entry temporary
  lane. Candidate and target each expose eight relocation records, but only
  six offset/type sites align and the four pass aliases remain unresolved by
  static identity evidence.
- a fresh linked promotion trial classifies the stock C as `text-differs`,
  with 35 in-range words, zero outside words, and 4/8 aligned relocation
  sites. This is not equality; the linked overlay and ROM still use the
  assembly fallback.
- the pinned instrumented IDO 5.3 drop-in carries the global-colour hooks.
  Tracing-off, index, and detailed-capture objects all pass `.text`, data,
  relocation, and symbol fidelity against the stock object. The TU maps to
  procedure 0 with ten phase-two allocator decisions; the trace producer
  exposes no direct `source_semantic` handle.
- candidate `p2:w0` owns the long choice-pointer rotation. It is the first p2
  web, has no recorded interference, and every caller-saved colour from
  `v0` through `t5` has equal zero cost. Stock therefore takes the lowest
  colour, `v0`; `v1` is available and not underpriced.
- the diagnostic `p2:w0=c2` force is accepted. It changes 32 object rows,
  cascades the downstream pool allocation toward retail, and reduces the
  positional residual from 35 to 13 words. The remaining aligned residual is
  six structural sites plus one register site around the hoisted
  `available`-base move and the two address-materialization groups. A forced
  object is diagnostic only and was not promoted.
- this rules out priority/cost tuning: the first web cannot naturally choose
  `v1` while equal-cost `v0` is free. Retail must form or number another web
  earlier, or split the initial walker from the later `available` base. The
  earlier one-pointer folding probe is already disproved, and the trace gives
  no new source-attributed spelling, so the retained C body is unchanged.
<!-- plateau-handoff:overlay60ReassignChoiceSlots:end -->
