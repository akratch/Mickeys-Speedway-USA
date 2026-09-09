#include "PR/ultratypes.h"

extern s32 D_30;
extern f32 D_34;

/* The extra word is the unfilled delay slot of the shared `jr ra`, and it is
 * a consequence of two branch-likely selections, not of the source tree.
 *
 * Census against the target: `bc1f -2, bc1fl +2, li +2, nop -1`. Both arms
 * end with a conditional branch to the one shared `return 1;`. In the target
 * as1 has already scheduled that block's `li v0,1` into the `jr ra` delay
 * slot, so the block begins with `jr ra`, which cannot be copied into an
 * annulled slot and the two branches stay `bc1f` with `nop` delays. In the
 * candidate the block still begins with `li v0,1`, so as1 takes it as the
 * branch-likely fill, leaves the original in place, retargets both branches
 * one word later, and the `jr ra` is left with a `nop`: one extra word.
 *
 * The `li +2` is the same v0 question from the other side. The target keeps
 * both stack-passed pointers live in v0/v1, loaded together at the head of
 * each arm, and gives `first`/`second` fresh temporaries (t7/t8, t1/t2); v0
 * is therefore not free and each `return 1;` materialises its own constant.
 * The candidate colours `first`/`second` into v0/a0, defers the `side2` load,
 * and hoists a spare `li v0,1` above each `bc1fl`.
 *
 * Source shape does not select it. Flat at 80 words and the same census:
 * hoisting `second = 0`, grouping the two stores, pointer locals assigned at
 * the top or per arm, a named result variable, swapping the two arms, giving
 * the else-arm its own `return 1;` (which costs three more words), removing
 * the last `if`'s braces, and merging the trailing `return` onto the closing
 * brace line. The whole flag lattice is also flat: no row at any of -O1/-O2/
 * -O3 x -mips1/-mips2/-mips3 x r4300_mul/loopunroll/g3 reaches 79 words with
 * a smaller census than the configured -O2 -mips2 row.
 *
 * Next lever: make v0 unavailable for the hoisted return constant so both
 * pointers take v0/v1, which is what leaves the join block starting with
 * `jr ra`. */
#ifdef NON_MATCHING
s32 overlay2ClassifyBoundary(f32 x1, f32 y1, f32 x2, f32 y2, s32 *side1,
                             s32 *side2) {
    s32 first;
    s32 second;

    if (D_30 == 0) {
        first = 0;
        if (y1 < D_34) {
            first = 1;
        }
        *side1 = first;
        second = 0;
        if (y2 < D_34) {
            second = 1;
        }
        *side2 = second;
        if (y1 == D_34) {
            *side1 = second;
            return 1;
        }
        if (y2 == D_34) {
            *side2 = *side1;
            return 1;
        }
    } else {
        first = 0;
        if (x1 < D_34) {
            first = 1;
        }
        *side1 = first;
        second = 0;
        if (x2 < D_34) {
            second = 1;
        }
        *side2 = second;
        if (x1 == D_34) {
            *side1 = second;
            return 1;
        }
        if (x2 == D_34) {
            *side2 = *side1;
        }
    }
    return 1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o002/overlay2ClassifyBoundary/func_overlay_002_F00002C4_18570BC.s")
#endif

/* PLATEAU-HANDOFF:overlay2ClassifyBoundary:start
 * symbol: overlay2ClassifyBoundary
 * score: 62/79 words
 * frame: frameless
 * relocations: 6
 * first-mismatch: +0x4
 * summary: The +1 word is the shared jr ra's unfilled delay slot: two bc1fl selections steal the join block's li v0,1, which the target has already scheduled into that delay. Census is bc1f -2, bc1fl +2, li +2, nop -1. Ten source shapes and the whole flag lattice are flat; the lever is making v0 unavailable so both stack pointers take v0/v1.
 * PLATEAU-HANDOFF:overlay2ClassifyBoundary:end
 */
