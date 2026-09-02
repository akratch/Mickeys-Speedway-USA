#include "PR/ultratypes.h"

extern s8 D_8007D640;
extern s32 D_800D6AB0;
extern s16 D_800D6AB4;
extern s16 D_800D6AB6;
extern s16 D_800D6AB8;
extern s16 D_800D6ABC;
extern s16 D_800D6ABE;
extern s16 D_800D6AC0;
extern s16 D_800D6AC2;
extern s16 D_800D6AC4;
extern s16 D_800D6AC8;
extern s32 D_800D6AD8[];
extern s32 D_800D6AE4;
extern s32 piRomGetFileSize(s32 fileId);
extern s32 func_8002B280(s32 size, s32 tag);

/* verdict: register-ring-only; 12 differing words; first mismatch +0x34 */
/* shape: exact 47 instructions and 0x18 frame; permuter-ready */
/* canonical fallback retained until the temporary allocation closes */
#ifdef NON_MATCHING
void func_8004E8E0(void) {
    s32 temp_t9;
    s32 temp_v0;

    D_800D6AB4 = (piRomGetFileSize(5) >> 2) - 2;
    temp_v0 = func_8002B280(0x790, 0x8F);
    temp_t9 = temp_v0 + 0x3C0;
    D_800D6AD8[0] = temp_v0;
    D_800D6AD8[1] = temp_t9;
    D_800D6AB0 = temp_t9 + 0x3C0;
    D_800D6AE4 = 0;
    D_800D6AC4 = 0;
    D_800D6AB6 = 0;
    D_800D6AB8 = 0x20;
    D_800D6AC8 = 0;
    D_800D6ABC = 0x20;
    D_800D6ABE = 0xCA;
    D_800D6AC0 = 0x120;
    D_800D6AC2 = 0xDE;
    D_8007D640 = 1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/block_4F4E0/func_8004E8E0.s")
#endif

extern void mmFree(void *ptr);
extern void func_8004BFD8(s32 windowId);
extern void func_8004BF64(s32 windowId);

void func_8004E99C(void) {
    if (D_8007D640 != 0) {
        mmFree((void *)D_800D6AD8[0]);
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

/* PLATEAU-HANDOFF:func_8004E8E0:start
 * symbol: func_8004E8E0
 * score: 35/47 words
 * frame: 0x18
 * relocations: 28
 * first-mismatch: +0x34
 * summary: Exact shape; residual ugen temp-ring allocation is permuter-ready; target registers rotate after the second temporary.
 * PLATEAU-HANDOFF:func_8004E8E0:end
 */
