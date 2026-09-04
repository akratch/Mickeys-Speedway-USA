#include "PR/ultratypes.h"

typedef struct Overlay43RotationInput {
    s16 pad00;
    s16 angle;
} Overlay43RotationInput;

typedef struct Overlay43MotionOutput {
    f32 unk00;
    u8 pad04[0x0C];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 pad1C[0x0C];
    f32 unk28;
    u8 pad2C[0x18];
    s32 owner;
} Overlay43MotionOutput;

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

extern u8 D_0[];
#define D_24 (*(f32 *)(D_0 + 0x24))
extern void func_80029FE4(Overlay43RotationInput *input, Vec3f *direction);
extern void func_8002A82C(Overlay43MotionOutput *output);

/* The configured candidate retains the exact 55-word/0x38-frame geometry and
 * nine differing words; +0x5C is call metadata and the first code-bit mismatch
 * is +0x74. Moving the D_24 carrier declaration behind the saved components
 * in a 2026-09-04 structural pass shifts otherwise-exact homes and regresses
 * to 19 words. Preserve the original FP pool order and assembly fallback. */
#ifdef NON_MATCHING
void func_overlay_043_F00010A8_188B078(Overlay43RotationInput *input,
                                      s32 owner,
                                      Overlay43MotionOutput *output) {
    f32 temp_f2;
    Vec3f direction;
    f32 sp24;
    f32 sp20;
    f32 sp1C;

    if (input->angle < 0) {
        input->angle = 0;
    }
    input->angle -= 0x4000;
    input->angle >>= 1;
    input->angle += 0x4000;

    direction.x = 0.0f;
    direction.y = 0.0f;
    direction.z = -1.0f;
    func_80029FE4(input, &direction);
    sp24 = direction.x;
    sp1C = direction.z;
    sp20 = direction.y;
    output->owner = owner;
    func_8002A82C(output);

    temp_f2 = D_24;
    output->unk00 = temp_f2;
    output->unk10 = -(sp24 / sp20);
    output->unk14 = 0.0f;
    output->unk18 = -(sp1C / sp20);
    output->unk28 = temp_f2;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o043/overlay43ComputeMotion/func_overlay_043_F00010A8_188B078.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_043_F00010A8_188B078:start
 * symbol: func_overlay_043_F00010A8_188B078
 * score: 46/55 words
 * frame: 0x38
 * relocations: 4
 * first-mismatch: +0x5C
 * summary: Moving the D_24 carrier behind saved components regresses 9 to 19 words by shifting exact homes. Preserve the original FP pool order.
 * PLATEAU-HANDOFF:func_overlay_043_F00010A8_188B078:end
 */
