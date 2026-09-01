#include "PR/ultratypes.h"
#include "PR/os_internal.h"

#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_80001740.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_80001A84.s")
extern OSThread D_800C7A50;

void func_80001BA0(void) {
    osStartThread(&D_800C7A50);
}
void func_80001BC4(void) {
    osStopThread(&D_800C7A50);
}
extern s32 D_80078DEC;

s32 func_80001BE8(void) {
    return D_80078DEC;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_80001BF4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_80002134.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_80002188.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/audioManager_DMAInitProc.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_8000238C.s")
