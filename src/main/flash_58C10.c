#include "PR/ultratypes.h"

extern s32 D_8007A1CC;
extern s32 D_800D789C;
extern s32 D_800D78A4;
extern s32 D_800D78AC;

#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_58C10/func_80058010.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_58C10/func_8005807C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_58C10/func_800580F0.s")
void func_800581BC(void) {
    D_8007A1CC |= 0x04000000;
}
void func_800581D8(s32 arg0, s32 arg1, s32 arg2) {
    D_8007A1CC |= 0x02000000;
    D_800D789C = arg0;
    D_800D78A4 = arg1;
    D_800D78AC = arg2;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_58C10/func_8005820C.s")

extern s32 D_8007F7A0;

s32 func_80058240(void) {
    return D_8007F7A0;
}
