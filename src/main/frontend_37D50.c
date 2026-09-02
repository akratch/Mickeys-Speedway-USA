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
extern void func_800378A4(f32 arg0, s32 arg1);
extern f32 func_8002A8C0(s32 angle);
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
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_800378A4.s")
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
