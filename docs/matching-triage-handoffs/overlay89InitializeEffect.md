<!-- plateau-handoff:overlay89InitializeEffect:start -->
### `overlay89InitializeEffect` plateau handoff

- source: `src/overlays/o089/overlay89InitializeEffect.c`
- score: 21 differing words
- frame: 0x58
- relocations: 5
- first mismatch: +0x40
- summary: 58 fell to 21 in three separable steps. Assigning `colors` before `source` closes a coherent v0/v1 cycle at zero width ([L127]) and is 58 to 45, while swapping their declarations is byte-identical, so definition order is what decides. Dropping both re-reads of `object->state` is 45 to 27 although each half alone regresses, -4 bytes and 69 words for one and +4 and 77 for the other ([L100]). Declaration order is then 27 to 21 by moving the a2 spill home from sp+0x54 to sp+0x38, and 11,520 orders put 21 at the floor with the frame never leaving 0x58. Of the 21 left, 17 are one float ring phase: the instrumented uopt shows exactly two class-2 webs, both taking colour 24 with an EMPTY forbidden mask, which is what makes the reading the table and not a constraint, and this procedure offers c24 to c29 only. c24 is f0, so the `size` local is coloured f0 and the float ring starts one position after the target's. Inlining the expression reaches the colour, naming 18 to 13, but hoists the scale load ahead of the conversion for 8 structural words; nineteen forms of the inline family, covering both operand orders, a pointer-arithmetic spelling, a dead trailing assignment, two statement positions, four region boundaries and a block-scoped scale local, bottom out at 26 while the local family holds 21, and there is nothing between the two minima. Forcing that web off c24 is worth at most 2 words, so the lever is not colour. The last 2 are the maintain call, where the target reuses the reloaded state pointer and we reload from the object.
<!-- plateau-handoff:overlay89InitializeEffect:end -->
