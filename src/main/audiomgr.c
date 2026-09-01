#include "PR/ultratypes.h"
#include "PR/os_internal.h"
#include "n_audio/libaudio.h"

typedef struct AudioManagerDMAState {
    u8 initialized;
    u8 pad1[3];
    void *firstUsed;
    void *firstFree;
} AudioManagerDMAState;

extern AudioManagerDMAState D_800C7DF8;
extern u8 D_800C7E08[];
extern s32 func_80002188(s32 addr, s32 len, void *state);

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
/* PROVENANCE: body adapted from Banjo-Kazooie's public decomp,
 * src/core1/code_1D00.c::audioManager_DMAInitProc. */
ALDMAproc audioManager_DMAInitProc(void *state) {
    if (!D_800C7DF8.initialized) {
        D_800C7DF8.firstUsed = NULL;
        D_800C7DF8.firstFree = D_800C7E08;
        D_800C7DF8.initialized = 1;
    }
    *(void **)state = &D_800C7DF8;
    return func_80002188;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_8000238C.s")
