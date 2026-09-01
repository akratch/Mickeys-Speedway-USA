#include "PR/ultratypes.h"

/*
 * PROVENANCE: the texture-TU placement and the small mode setter below were
 * compared with Jet Force Gemini's public src/textures.c. Mickey's globals,
 * boundaries, and compiler output remain authoritative.
 */

extern s32 D_8007BD90;

#ifdef NON_MATCHING
typedef struct TextureRomTable {
    s32 *first;
    s32 *second;
} TextureRomTable;

extern void *func_8002B280(s32 size, s32 tag);
extern s32 *piRomLoad(s32 assetId);
extern TextureRomTable D_800D2FD0;
extern s32 *D_800D2FD8;
extern s32 *D_800D2FDC;
extern s32 D_800D2FE0;
extern s32 D_800D2FE8[2];
extern s32 D_800D2FF0;
extern s32 *D_800D2FF4;
extern s32 *D_800D2FF8;
extern s32 *D_800D2FFC;
extern s32 *D_800D3000;
extern s32 D_800D3004;
extern s32 D_800D3008;
extern s32 *D_800D301C;

void func_80034260(void) {
    s32 *temp_v0;
    s32 *var_v0;
    s32 *var_v0_2;
    s32 temp_t3;
    s32 temp_t7;
    s32 temp_t9;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;

    D_800D2FD8 = func_8002B280(0x15E0, 0x90);
    D_800D2FDC = func_8002B280(0x280, 0x90);
    D_800D2FE0 = 0;
    D_800D2FF0 = 0;
    D_800D2FD0.first = piRomLoad(3);
    D_800D2FD0.second = piRomLoad(1);
    var_v1 = 0;
    if (D_800D2FD0.first != NULL && D_800D2FD0.first[0] != -1) {
        var_v0 = D_800D2FD0.first;
        do {
            temp_t7 = *((s32 *)((u8 *)var_v0 + 4));
            var_v1++;
            var_v0 = (s32 *)((u8 *)var_v0 + 4);
        } while (temp_t7 != -1);
    }
    D_800D2FE8[0] = var_v1 - 1;
    var_v1_2 = 0;
    var_v0_2 = D_800D2FD0.second;
    if (D_800D2FD0.second != NULL && D_800D2FD0.second[0] != -1) {
        do {
            temp_t9 = *((s32 *)((u8 *)var_v0_2 + 4));
            var_v1_2++;
            var_v0_2 = (s32 *)((u8 *)var_v0_2 + 4);
        } while (temp_t9 != -1);
    }
    D_800D2FE8[1] = var_v1_2 - 1;
    D_800D2FFC = func_8002B280(0x320, 0x90);
    D_800D3000 = func_8002B280(0x200, 0x90);
    D_800D3008 = 0;
    temp_v0 = piRomLoad(0x16);
    D_800D2FF8 = temp_v0;
    D_800D3004 = 0;
    var_v1_3 = 0;
    if (temp_v0 != NULL && temp_v0[0] != -1) {
        do {
            temp_t3 = var_v1_3 + 1;
            D_800D3004 = temp_t3;
            var_v1_3 = temp_t3;
        } while (temp_v0[var_v1_3] != -1);
    }
    D_800D3004 = var_v1_3 - 1;
    D_800D301C = func_8002B280(0x28, 0x90);
    D_800D2FF4 = 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_34E60/func_80034260.s")
#endif

void func_800343F0(s32 flags) {
    D_8007BD90 |= flags;
}
