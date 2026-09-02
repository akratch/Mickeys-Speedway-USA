#include "PR/ultratypes.h"

#pragma GLOBAL_ASM("asm/nonmatchings/main/block_4F4E0/func_8004E8E0.s")
extern s8 D_8007D640;
extern s16 D_800D6AC4;
extern s32 D_800D6AD8;
extern void mmFree(void *ptr);
extern void func_8004BFD8(s32 windowId);
extern void func_8004BF64(s32 windowId);

void func_8004E99C(void) {
    if (D_8007D640 != 0) {
        mmFree((void *)D_800D6AD8);
        func_8004BFD8(6);
        func_8004BF64(6);
        D_8007D640 = 0;
        D_800D6AC4 = 0;
    }
}

extern s32 D_8007D648;

void func_8004E9EC(s32 arg0) {
    D_8007D648 = arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/block_4F4E0/func_8004E9F8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/block_4F4E0/func_8004EC60.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/block_4F4E0/func_8004EDA8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/block_4F4E0/func_8004EED0.s")
s32 func_8004F020(void) {
    return 0;
}
