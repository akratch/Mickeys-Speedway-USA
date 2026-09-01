#include "PR/ultratypes.h"
#include "PR/os_internal.h"
#include "PR/os.h"
#include "PR/os_pi.h"
#include "n_audio/libaudio.h"
#include "n_libaudio.h"
#include "game/sched_internal.h"

typedef struct AudioManagerDMABuffer {
    ALLink node;
    u32 startAddr;
    u32 lastFrame;
    char *ptr;
} AudioManagerDMABuffer;

typedef struct AudioManagerDMAState {
    u8 initialized;
    u8 pad1[3];
    AudioManagerDMABuffer *firstUsed;
    AudioManagerDMABuffer *firstFree;
} AudioManagerDMAState;

extern AudioManagerDMAState D_800C7DF8;
extern AudioManagerDMABuffer D_800C7E08[];
extern s32 func_80002188(s32 addr, s32 len, void *state);
extern u32 D_80078DD0;
extern u32 D_80078DD4;
extern volatile u32 D_80078DF0;
extern OSMesgQueue D_800C9020;
extern OSIoMesg D_800C8648[];
extern OSSched *D_800BFA30;
extern OSScClient D_800BFA38;
extern OSMesgQueue D_800C7D84;
extern OSMesgQueue D_800C7D9C;
extern N_ALGlobals D_800C7C80;
extern void osScAddClient(OSSched *sc, OSScClient *client, OSMesgQueue *msgQ, u8 id);
extern void func_80001BF4(void);
extern void func_80002134(void);

#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_80001740.s")

/* PROVENANCE: body adapted from Diddy Kong Racing's public decomp,
 * src/audiomgr.c::__amMain; Mickey's queue globals and message flow remain authoritative. */
void func_80001A84(void *arg) {
    s16 *msg = NULL;
    s16 *doneMsg = NULL;
    s32 done = 0;

    (void)arg;
    osScAddClient(D_800BFA30, &D_800BFA38, &D_800C7D84, 1);
    do {
        osRecvMesg(&D_800C7D84, (OSMesg *)&msg, OS_MESG_BLOCK);
        switch (*msg) {
            case 4:
                break;
            case 1:
                func_80001BF4();
                osRecvMesg(&D_800C7D9C, (OSMesg *)&doneMsg, OS_MESG_BLOCK);
                func_80002134();
                break;
            case 10:
                done = 1;
                break;
        }
    } while (done == 0);
    n_alClose(&D_800C7C80);
}
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
