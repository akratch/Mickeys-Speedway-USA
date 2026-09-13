<!-- plateau-handoff:overlay60ReassignChoiceSlots:start -->
### `overlay60ReassignChoiceSlots` plateau handoff

- source: `src/overlays/o060/overlay60ReassignChoiceSlots.c`
- score: 35 differing words
- frame: 0x20
- relocations: 8
- first mismatch: 0x4
- summary: Authenticated nine-draw baseline; prior cursor/type/extent differentials leave the earlier p2 web-creation requirement unresolved.

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

#### 2026-09-12 lane p19-laws re-test

The configured baseline remains 35 masked words out of 53, exact size and
frame 0x20, with eight relocations. Replacing the first initialization walk
with an indexed loop scored 55 and grew the frame to 0x28. A separate pointer
carrier reached the target extent but retained a 0x28 frame and scored 37;
changing only the carrier type normalized back to the 35-word baseline.

These tests support the handoff's earlier-web-formation diagnosis but do not
provide a frame-preserving source form. The accepted `p2:w0=c2` force remains
diagnostic only at 13 words; the guarded baseline is retained.
#### 2026-09-13, lane h1: array-extent and stack-address differential

A fresh configured compile reproduces 53 words, 212 bytes, frame 0x20,
35 raw/masked differences and first +0x4. Paired aligned buckets are 19
exact and 33 naming, with no immediate or structural paired rows, plus one
candidate-only and one target-only word. The nine-draw census contains 101
emission records. Stock and traced full-TU text agree. Static relocations
are eight candidate and four extracted target records, with no matching
symbol tuples; this is not a resolved runtime relocation surface.

The earlier indexed-initialization probe grew the frame. One extent control
tests whether the two bytes above the explicit masked-index domain provide
frame slack: available has 16 elements instead of 18. All nine draws, their
sequence, and every line's emission count remain unchanged. The frame also
stays 0x20, while the array base moves from offset 0xC to 0x10 and its loop
bound moves from 0x16 to 0x1A. Aligned buckets become 18 exact, 32 naming,
one immediate and one structural paired row, plus one gap pair. Thus this
control fails the target stack-address requirement despite the unchanged
35-word scalar score. The original 18-element array is restored.

The smaller array was diagnostic only; no broader search-bound claim is
made for the unmasked slot-search loop. The measured frame behavior gives
no reason to repeat the already-failed indexed initialization on this extent.
ADR 0018 early stop applies to this frame-slack hypothesis and the recorded
carrier/initialization controls. Earlier web formation remains unresolved;
no colour sweep or known-flat cursor lattice was repeated and no bytes count
as matched. Evidence remains ignored under build/h1/overlay60ReassignChoiceSlots.
Commands: configured compilation, draw_census.py --save/--compare,
residual_map.py --object/--against, finalize_plateau.py and tools/gates.sh.

#### 2026-09-13, lane k1: authenticated draw-census follow-up

Fresh configured stock compilation reproduces 212 target bytes,
size delta 0, 35 raw and
35 masked differences, first +0x4.
Candidate frame is 0x20; the procedure-0 census records
9 draws and 101 emission records. Stock and traced full-TU text
compare identically. Static relocation counts are 8 candidate and
4 extracted target, with 0 identical offset/type/symbol tuples.
These are static measurements; overlay runtime identity proof remains separate.

Paired aligned buckets are 19 exact and 33 naming, with no paired immediate
or structural rows and one candidate-only/target-only gap pair. All observed
stack offsets agree, while the target takes the array base twice and the
candidate once. The first p2 web still reaches a free lowest colour; this is
the recorded earlier-web-formation question, not a ring-phase explanation.

The complete shard and source were reviewed, including the indexed initializer,
separate cursor, carrier-type and 16-element frame-slack controls. The latter
already measures unchanged draws despite different array-base traffic and no
frame reduction. No new frame-preserving source hypothesis was identified
outside those controls. ADR 0018 early evidence stop applies with zero new
source attempts. The nine-draw baseline is independently reproduced; no colour
sweep or known-flat cursor/extent probe was repeated. This is confirmation of
the open pre-colour web-creation requirement, not a new exhaustion claim.

The original guarded body and assembly fallback are retained. Sources, stock
and traced objects, frame and scalar measurements, draw profiles and aligned
maps remain ignored under build/k1/overlay60ReassignChoiceSlots.
Commands: lane_status.py, configured stock compilation, draw_census.py,
residual_map.py --object/--against where compared, finalize_plateau.py and
tools/gates.sh. No executable bytes are newly credited.

<!-- plateau-handoff:overlay60ReassignChoiceSlots:end -->
