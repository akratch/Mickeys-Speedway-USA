<!-- plateau-handoff:overlay89InitializeEffect:start -->
### `overlay89InitializeEffect` plateau handoff

- source: `src/overlays/o089/overlay89InitializeEffect.c`
- score: 21 differing words
- frame: 0x58
- relocations: 5
- first mismatch: +0x40
- summary: 58 fell to 21 in three separable steps. Assigning `colors` before `source` closes a coherent v0/v1 cycle at zero width ([L127]) and is 58 to 45, while swapping their declarations is byte-identical, so definition order is what decides. Dropping both re-reads of `object->state` is 45 to 27 although each half alone regresses, -4 bytes and 69 words for one and +4 and 77 for the other ([L100]). Declaration order is then 27 to 21 by moving the a2 spill home from sp+0x54 to sp+0x38, and 11,520 orders put 21 at the floor with the frame never leaving 0x58. Of the 21 left, 17 are one float ring phase: the instrumented uopt shows exactly two class-2 webs, both taking colour 24 with an EMPTY forbidden mask, which is what makes the reading the table and not a constraint, and this procedure offers c24 to c29 only. c24 is f0, so the `size` local is coloured f0 and the float ring starts one position after the target's. Inlining the expression reaches the colour, naming 18 to 13, but hoists the scale load ahead of the conversion for 8 structural words; nineteen forms of the inline family, covering both operand orders, a pointer-arithmetic spelling, a dead trailing assignment, two statement positions, four region boundaries and a block-scoped scale local, bottom out at 26 while the local family holds 21, and there is nothing between the two minima. Forcing that web off c24 is worth at most 2 words, so the lever is not colour. The last 2 are the maintain call, where the target reuses the reloaded state pointer and we reload from the object. Lane p8-close floored the JOINT force grid, both class-2 webs against all six colours plus both splits, at 17, so colour is worth at most 4 and cannot close it. The inline form already has the target's ring and loses only on evaluating the scale load before the conversion; both source operand orders compile byte-identically, so IDO canonicalises the multiply and no spelling chooses.

#### 2026-09-12, lane `p8-close`: the colour half is bounded at 17 and the blocker is renamed

Baseline reproduces: 820 bytes, 205 of 205 instructions, delta 0, frame 0x58,
21 relocation-masked words, first mismatch +0x40. Aligner: 185 byte-exact, 18
register naming, 1 immediate-only, 2 really-different. `register_census` reads
two integer substitution sites and a float mapping over three windows at 91 per
cent with one closed five-cycle, which is the ring phase the record describes.

**Colour is bounded harder than the record says, and still cannot close it.**
The earlier entry priced colour from a one-web sweep. The JOINT grid -- both
class-2 webs against all six offered colours, 36 cells, plus both split paths
-- floors at **17**, at web 42 on colour 24 with web 16 on colour 28. So the
colour axis is worth at most 4 of the 21, not 2. It cannot close the function
for a reason the grid makes plain: both class-2 webs take the same colour, and
the ugen float ring only shifts when NO web holds that register.

**The ring fact, read from the object rather than the record.** In the size
block the target draws five scratch float registers and we draw four, because
our conversion result lands in the coloured web instead of the ring; every
later float register in the procedure is one ring position behind as a result.
Deleting the `size` local is the whole 17 words, which is what the record
already said, now with the count on both sides.

**The blocker is an evaluation order, not a colour and not a hoist.** The
inline form already has the right ring -- five scratch draws, and the
multiply's destination register is the target's -- and loses on exactly one
thing: it evaluates the scale load before the conversion, so the load takes the
first ring slot and everything else moves up one. The two source operand orders
compile BYTE-IDENTICALLY, so IDO canonicalises the commutative multiply and the
source cannot choose the order by writing it.

**Flat at 26, on top of the nineteen forms already recorded:** `const` and
sized array declarations of the scale symbol, a pointer declaration of it
(which is worse), `*(g + 1)`, `(g + 1)[0]`, an extra parenthesisation, an
or-with-zero and an add-with-zero folded into the conversion, and `if (1) { }`, `do { } while (0)` and
a bare brace before or around the statement.

**Also refuted this pass, none at 21 or better:** the assignment-in-expression
and comma forms, 55 each; `object->size` as its own intermediate, 187; a
ternary, 200; a second use of `size`, 197; a signed-conversion cast, 194 at
delta -20. Reusing the existing `range` local as the intermediate, and an
initialiser-style declaration, are byte-identical to the incumbent, so the
identity of the local does not matter -- only whether one exists.

**Decision variable for the next lane:** a source form in which the multiply's
conversion operand is evaluated before its memory operand while both remain in
one statement. Nothing measured moves IDO's canonicalisation, so this is a
weight question inside ugen and not a spelling question.
<!-- plateau-handoff:overlay89InitializeEffect:end -->
