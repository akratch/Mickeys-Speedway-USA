#include "PR/ultratypes.h"
#include "game/memory.h"

extern s32 D_8007BEA8;
extern s32 D_8007BE90;
extern s32 D_8007BEB8;
extern s32 D_8007BE80;
typedef struct FrontendBufferPointers {
    void *unk0;
    void *unk4;
    void *unk8;
} FrontendBufferPointers;
extern FrontendBufferPointers D_8007BE88;
typedef struct FrontendVertex {
    s16 x;
    s16 y;
    s16 unk4;
    s8 r;
    s8 g;
    s8 b;
    s8 a;
} FrontendVertex;
extern s32 D_8007BE84;
extern s32 D_8007BEB0;
extern s32 D_8007BEB4;
extern f32 D_800826A0;
extern void func_800378A4(f32 arg0, s32 arg1);
extern f32 func_8002A8C0(s32 angle);
extern f32 sqrtf(f32 value);
extern void func_80037AEC(f32 arg0, s32 arg1);

void func_80037150(void) {
    D_8007BEA8 = 0;
    if (D_8007BE88.unk0 != NULL) {
        mmFree(D_8007BE88.unk0);
        D_8007BE88.unk0 = NULL;
    }
    if (D_8007BE88.unk4 != NULL) {
        mmFree(D_8007BE88.unk4);
        D_8007BE88.unk4 = NULL;
    }
    D_8007BE80 = 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_800371BC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037414.s")
void func_80037658(void) {
    D_8007BEA8 = 0;
}
s32 func_80037664(void) {
    if ((D_8007BEA8 == 0) && (D_8007BEB8 == 0)) {
        return 0;
    }
    if ((D_8007BEA8 != 2) || ((D_8007BE90 & 1) != 0) ||
        (D_8007BEB8 != 0)) {
        return 1;
    }
    return 2;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_800376CC.s")
/* Workbench verdict: structure-mismatch, 103 differing words. */
/* First mismatch: +0x14; target 117 instructions/candidate 112, both 0x68 frames. */
/* Structural gap: radial-loop FP lifetimes and clamp/control-flow shape remain. */
#ifdef NON_MATCHING
void func_800378A4(f32 arg0, s32 arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 var_f20;
    s32 temp_s3;
    s32 var_a0;
    s32 var_s1;
    s32 var_s6;
    s32 var_v0;
    FrontendVertex *var_s0;

    var_a0 = (s32) arg0;
    arg0 *= D_800826A0;
    var_s0 = ((FrontendVertex **) &D_8007BE88)[D_8007BE84];
    if (var_s0 != NULL) {
        temp_s3 = 0xFF - var_a0;
        var_s6 = 0;
        do {
            var_s1 = 0;
            temp_f0 = (f32) (var_s6 - 8) * 15.0f;

            do {
                temp_f0_2 = (f32) (var_s1 - 8) * 20.0f;
                temp_f0_3 = sqrtf((temp_f0_2 * temp_f0_2) +
                                  (temp_f0 * temp_f0));
                var_f20 = (200.0f - temp_f0_3) * arg0;
                if (var_f20 < 0.0f) {
                    var_f20 = 0.0f;
                }
                var_a0 = (s32) (func_8002A8C0(
                    (s32) (temp_f0_3 * 1000.0f) + D_8007BEB4) * var_f20);
                var_v0 = (s32) ((temp_s3 + var_a0) * arg1) >> 8;
                if (var_v0 < 0) {
                    var_v0 = 0;
                }
                if (var_v0 >= 0x100) {
                    var_v0 = 0xFF;
                }
                var_s0->unk4 = (s16) (var_a0 + 5);
                var_s0->r = (s8) var_v0;
                var_s0->g = (s8) var_v0;
                var_s0->b = (s8) var_v0;
                var_s1 += 1;
                var_s0 += 1;
            } while (var_s1 != 0x11);
            var_s6 += 1;
        } while (var_s6 != 0x11);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_800378A4.s")
#endif
void func_80037A78(void) {
    D_8007BEB4 = 0x8000 - (D_8007BEB0 << 8);
    if (D_8007BEB0 < 0x200) {
        func_800378A4((f32) ((s32) D_8007BEB0 >> 3), 0x100);
        return;
    }
    func_800378A4(64.0f, (s32) (0x400 - D_8007BEB0) >> 1);
}
/* Workbench verdict: allocation-mismatch, 7 differing words. */
/* First mismatch: +0x54; target and candidate are 66 words with 0x40 frames. */
/* Structural gap: only register allocation remains. */
#ifdef NON_MATCHING
void func_80037AEC(f32 arg0, s32 arg1) {
    FrontendVertex *vertex;
    s32 row;
    s32 phase;

    vertex = ((FrontendVertex **) &D_8007BE88)[D_8007BE84];
    if (vertex != NULL) {
        s32 contrast = 0xFF - ((s32) arg0 * 2);

        phase = D_8007BEB4;
        row = 0;
        do {
            s32 column = 0;
            s32 angle = phase;

            do {
                f32 value = func_8002A8C0(angle) * arg0;
                s32 integerValue;
                s32 intensity;

                column += 1;
                angle += 0x2000;
                vertex += 1;
                integerValue = (s32) value;
                vertex[-1].unk4 = (s16) (integerValue + 5);
                intensity = (s32) (((integerValue * 2) + contrast) * arg1) >> 8;
                vertex[-1].r = (s8) intensity;
                vertex[-1].g = (s8) intensity;
                vertex[-1].b = (s8) intensity;
            } while (column != 0x11);
            row += 1;
            phase += 0x800;
        } while (row != 0x11);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037AEC.s")
#endif
/* Workbench verdict: structure-mismatch, with a four-byte size deficit. */
/* First mismatch: +0x40; target multiplies before the call and has a nop delay slot. */
/* Structural gap: the interpolation multiply/call scheduling shape is unresolved. */
#ifdef NON_MATCHING
void func_80037BF4(void) {
    D_8007BEB4 = (D_8007BEB0 << 8) + 0x8000;
    if (D_8007BEB0 < 0x200) {
        func_80037AEC((f32) D_8007BEB0 * 0.0625f, 0x100);
    } else {
        func_80037AEC(32.0f, (s32) (0x400 - D_8007BEB0) >> 1);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037BF4.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037C74.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80038190.s")
