#include "PR/ultratypes.h"
#include "game/math.h"

extern f32 D_80083F60;
extern f32 func_8002A8BC(s32 angle);
extern f32 func_8002A8C0(s32 angle);

/* PROVENANCE: adapted from Jet Force Gemini's public libultra decomp,
 * libultra/src/gu/perspective.c:guPerspectiveF; Mickey's angle helpers,
 * constants, and linked bytes remain authoritative. */
/* verdict: structure-mismatch; 29/84 words differ; frame size is exact */
/* first mismatch: +0x44 helper-result spill home; target has one extra nop */
/* blocker: IDO spill placement and one pipeline gap remain unresolved */
#ifdef NON_MATCHING
void func_8004FAD0(MtxF mf, u16 *perspNorm, f32 fovy, f32 aspect,
                   f32 nearPlane, f32 farPlane, f32 scale) {
    f32 sp20;
    s32 sp1C;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f8_2;
    s32 temp_f8;

    temp_f8 = (s32)(fovy * D_80083F60);
    sp1C = temp_f8;
    sp20 = func_8002A8BC(temp_f8);
    temp_f12 = (sp20 / func_8002A8C0(temp_f8)) * scale;
    temp_f8_2 = temp_f12 / aspect;
    temp_f14 = scale / (nearPlane - farPlane);
    mf[0][0] = temp_f8_2;
    mf[0][1] = 0.0f;
    mf[0][2] = 0.0f;
    mf[0][3] = 0.0f;
    mf[1][0] = 0.0f;
    mf[1][1] = temp_f12;
    mf[1][2] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][0] = 0.0f;
    mf[2][1] = 0.0f;
    temp_f16 = nearPlane + farPlane;
    mf[2][2] = temp_f16 * temp_f14;
    mf[2][3] = -1.0f;
    mf[3][0] = 0.0f;
    mf[3][1] = 0.0f;
    mf[3][2] = (2.0f * nearPlane * farPlane) * temp_f14;
    mf[3][3] = 0.0f;

    if (perspNorm != (u16 *)NULL) {
        if (temp_f16 <= 2.0f) {
            *perspNorm = (u16)0xFFFF;
        } else {
            *perspNorm = (s32)(131072.0f / temp_f16);
            if (*perspNorm <= 0) {
                *perspNorm = (u16)0x0001;
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/block_506D0/func_8004FAD0.s")
#endif

/* PLATEAU-HANDOFF:func_8004FAD0:start
 * symbol: func_8004FAD0
 * score: 29/84 words
 * frame: 0x38
 * relocations: 4
 * first-mismatch: +0x44
 * summary: Best candidate has exact frame and register lanes; helper-result spill home and one pipeline gap remain after bounded source-shape sweep.
 * PLATEAU-HANDOFF:func_8004FAD0:end
 */
