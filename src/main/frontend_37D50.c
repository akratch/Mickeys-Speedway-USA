#include "PR/ultratypes.h"

extern s32 D_8007BEA8;
extern s32 D_8007BE90;
extern s32 D_8007BEB8;

#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037150.s")
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
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037A78.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037AEC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037BF4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037C74.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80038190.s")
