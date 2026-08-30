#include "PR/ultratypes.h"

extern s32 gOverlay2BoundaryAxis;
extern f32 gOverlay2BoundaryValue;

/* Workbench: structure-mismatch, 61 differing words, first semantic gap +0x14.
 * Branch-likely/FP CFG is target-shaped; the candidate is one word oversized.
 * Fresh V0 is frameless at 80 versus 79 words (320 versus 316 bytes), with
 * 18/79 relocation-masked and 17/79 raw words exact.  All six runtime LOCAL
 * tuples retain the BSS +0x30/+0x34 identities.  A natural BSS-base struct
 * spelling makes the first load target-shaped but leaves 61 masked differences
 * and adds no identity gain, so the pointer/register web remains the blocker. */
#ifdef NON_MATCHING
s32 overlay2ClassifyBoundary(f32 x1, f32 y1, f32 x2, f32 y2, s32 *side1,
                             s32 *side2) {
    s32 first;
    s32 second;

    if (gOverlay2BoundaryAxis == 0) {
        first = 0;
        if (y1 < gOverlay2BoundaryValue) {
            first = 1;
        }
        *side1 = first;
        second = 0;
        if (y2 < gOverlay2BoundaryValue) {
            second = 1;
        }
        *side2 = second;
        if (y1 == gOverlay2BoundaryValue) {
            *side1 = second;
            return 1;
        }
        if (y2 == gOverlay2BoundaryValue) {
            *side2 = *side1;
            return 1;
        }
    } else {
        first = 0;
        if (x1 < gOverlay2BoundaryValue) {
            first = 1;
        }
        *side1 = first;
        second = 0;
        if (x2 < gOverlay2BoundaryValue) {
            second = 1;
        }
        *side2 = second;
        if (x1 == gOverlay2BoundaryValue) {
            *side1 = second;
            return 1;
        }
        if (x2 == gOverlay2BoundaryValue) {
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
 * score: 18/79 words
 * frame: frameless
 * relocations: 6
 * first-mismatch: +0x14
 * summary: Fresh V0 is 320 vs 316 bytes; all six runtime LOCAL tuples are exact. Resume only with a new natural pointer/register-web mechanism; BSS-base was flat.
 * PLATEAU-HANDOFF:overlay2ClassifyBoundary:end
 */
