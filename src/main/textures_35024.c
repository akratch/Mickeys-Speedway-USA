#include "PR/ultratypes.h"

/*
 * PROVENANCE: this texture-TU sequence was compared with Jet Force Gemini's
 * public src/textures.c. The two state setters are JFG-shaped starting
 * points; Mickey's globals and final byte/relocation results decide adoption.
 */

extern u8 D_8007BD94;
extern s32 D_8007BD80;

void func_80034424(u8 enabled) {
    D_8007BD94 = enabled;
}
#ifdef NON_MATCHING
void func_80034434(s32 value, ...) {
    D_8007BD80 = value & 0xFF;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_35024/func_80034434.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_35024/func_80034448.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_35024/func_800347A0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_35024/func_8003484C.s")
