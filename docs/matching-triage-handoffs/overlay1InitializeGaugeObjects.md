<!-- plateau-handoff:overlay1InitializeGaugeObjects:start -->
### `overlay1InitializeGaugeObjects` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 61/74 words
- frame: 0x60
- relocations: 3
- first mismatch: +0x30
- summary: one cursor for both loops takes 16 to 13; residual is the objects copy and the shared count read, reachable only through a union at a worse schedule

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

#### lm-o1tail: one cursor, and the shape of the declaration budget

16 to 13 words with one deletion: the separate `firstCursor` declaration. The
two loops walk the same list downward, and giving them one `O1GaugeObject **`
puts the second loop's cursor arithmetic back on the target's registers. 74
instructions and the 0x60 frame hold.

The frame is a declaration budget, and it is not a simple count. Ten or eleven
declarations give 0x60; a twelfth gives 0x68, and so does swapping a pointer
declaration for an `s32` at eleven. Every attempt to hold the count in a named
`s32` therefore pays 8 bytes of frame before it buys anything, which is why the
five carrier forms below all lose.

The count identity IS reachable, and this is the new causal finding. The target
loads the count slot once for both `initialIndex = count - 1` and the emptiness
test, and again after the first loop. `volatile` gives the reload but forbids
the sharing (three reads, three loads); plain gives the sharing but not the
reload (three reads, one load, and 0x68 of frame). A union of a plain and a
volatile member at the same address gives both -- 74 instructions, the 0x60
frame, and the first mismatch moves from +0x30 to +0x34 with the count block
exact -- but it costs the `li s3,40` placement and turns the second `beqz` into
a `beqz`-likely, landing at 14. It is retained as evidence that the identity is
source-reachable, not adopted: it is one word worse and it is not plausible
original source.

Eliminated from this base, do not repeat: the count carried in `loopValue`,
`index`, `maximum` or a fresh `s32` (each 75 instructions, the extra being a
copy of `initialIndex`); `maximum = 0` moved to three positions; reading the
count through `*(s32 *)&count` while `&count` is also the call argument (the
address CSEs into a saved register: +1 instruction at the call site); an
explicit `objectBase = objects` copy in either direction and at two declaration
positions (IDO coalesces it away); `register` on `objects`; `&objects[index]`,
`objects + index` and the byte-cast form for either cursor; and the first loop
written as `objects[index]` with no cursor at all (16). A 10-minute permuter run
from the 13-word base was flat at its base score of 295.

The residual is now one mechanism: the target copies the call result into a
second live carrier (`move a2,v0`) and uses the copy for the second loop's base
while the first loop uses the raw `v0`. That is two webs for one value, and no
source-level copy produces it because IDO coalesces every copy whose operands
are equal. The next lever has to make the two uses different expressions, not
the same local spelled twice.
<!-- plateau-handoff:overlay1InitializeGaugeObjects:end -->
