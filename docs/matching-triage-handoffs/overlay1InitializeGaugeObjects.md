<!-- plateau-handoff:overlay1InitializeGaugeObjects:start -->
### `overlay1InitializeGaugeObjects` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 58/74 words
- frame: 0x60
- relocations: 3
- first mismatch: +0x1C
- summary: The reported +0x1C is an unresolved call identity; first instruction mismatch +0x30 is the count/object carrier after 119 flags and nine forms.

#### tu2-o1tail: the volatile count is required, and the objects carrier is the residual

Re-measured at the assigned base: 16 masked words, 74 of 74 instructions,
0x60 frame exact. The `volatile s32 count` is load-bearing -- dropping the
qualifier costs 38 words (16 to 54) and one instruction, because IDO then
CSE's the three reads into one.

The residual is two carriers. The target reads the count once into t6 and uses
it for both the `beqz` and the `addiu ...,-1`, then copies the object list from
v0 into a2 with a `move` and uses a2 for the second loop's cursor arithmetic;
the candidate emits two adjacent loads of the count slot, no copy of v0, and
propagates initialIndex into the second cursor where the target uses the
separate `index` web. Eliminated: reading the count through one named carrier
before the subtraction (flat at 16), and moving `maximum = 0` after the
initialIndex computation (flat at 16). Next lever is the objects carrier -- the
target's `move a2,v0` is the two-live-carriers shape, so the source spells the
second loop's base as the call result again rather than through the same local.
<!-- plateau-handoff:overlay1InitializeGaugeObjects:end -->
