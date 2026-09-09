#include "PR/ultratypes.h"

extern s32 D_30;
extern f32 D_34;

/* Workbench: structure-mismatch, 61 relocation-masked / 62 raw differences, first +0x4.
 * Target has 79 instructions versus candidate 80; D_34 sites align, while D_30 adds two relocation entries.
 * Candidate is not permuter-ready; branch-likely and pointer/register structure remain divergent. */
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
 * score: 18/79 words
 * frame: frameless
 * relocations: 6
 * first-mismatch: +0x4
 * summary: Target 79 versus candidate 80 instructions; D_34 sites align but D_30 adds two relocations and branch-likely structure remains
 * PLATEAU-HANDOFF:overlay2ClassifyBoundary:end
 */
