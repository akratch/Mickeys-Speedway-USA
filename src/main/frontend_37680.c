#include "PR/ultratypes.h"

/* PROVENANCE: body adapted from Jet Force Gemini's public decompilation,
 * src/textures.c:resetColourCycle. Mickey's layout and compiler output remain
 * authoritative. */
typedef struct ColourCycle {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    struct ColourCycle *unkC;
} ColourCycle;

void func_80036A80(ColourCycle *cycle) {
    ColourCycle *temp_v0;

    temp_v0 = cycle->unkC;
    cycle->unk0 = 0;
    cycle->unk4 = 0;
    cycle->unk8 = temp_v0->unk8;
    cycle->unk9 = temp_v0->unk9;
    cycle->unkA = temp_v0->unkA;
    cycle->unkB = temp_v0->unkB;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37680/func_80036AB0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37680/func_80036C60.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37680/func_80036CAC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37680/func_80036DD0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37680/func_80036F08.s")
