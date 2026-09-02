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
/* Verdict: structure-mismatch; 9 differing sites of 21 instructions with an exact frame. */
/* First mismatch: function offset +0x20; the target reloads the EFC address for its final store. */
/* Gap: the target's direct EFC load/store lifetimes are not reproduced by this volatile declaration. */
#ifdef NON_MATCHING
extern volatile u32 D_80078DF0;
extern volatile s32 D_80078EFC;

void func_80002134(void) {
    if ((osAiGetLength() >> 2) == 0) {
        if (D_80078EFC == 0) {
            D_80078DF0 |= 8;
            D_80078EFC = 0;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_80002134.s")
#endif
/* PROVENANCE: body adapted from Diddy Kong Racing's public decomp,
 * src/audiomgr.c::__amDMA; Mickey's DMA state and queue globals remain authoritative. */
/* Verdict: operand-mismatch; 7 differing words of 115 with exact frame and schedule. */
/* First mismatch: function offset +0x28; the remaining differences are stack spill offsets. */
/* Gap: the target assigns different homes to delta, lastDmaPtr, and foundBuffer. */
#ifdef NON_MATCHING
s32 func_80002188(s32 addr, s32 len, void *state) {
    void *foundBuffer;
    s32 delta, addrEnd, buffEnd;
    AudioManagerDMABuffer *dmaPtr, *lastDmaPtr;
    s32 pad;

    lastDmaPtr = NULL;
    delta = addr & 1;
    dmaPtr = D_800C7DF8.firstUsed;
    addrEnd = addr + len;

    while (dmaPtr != NULL) {
        buffEnd = dmaPtr->startAddr + 0x200;
        if (dmaPtr->startAddr > (u32) addr) {
            break;
        } else if (addrEnd <= buffEnd) {
            dmaPtr->lastFrame = D_80078DD0;
            foundBuffer = dmaPtr->ptr + addr - dmaPtr->startAddr;
            return osVirtualToPhysical(foundBuffer);
        }
        lastDmaPtr = dmaPtr;
        dmaPtr = (AudioManagerDMABuffer *) dmaPtr->node.next;
    }

    dmaPtr = D_800C7DF8.firstFree;
    if (dmaPtr == NULL) {
        D_80078DF0 |= 2;
        if (lastDmaPtr == NULL) {
            lastDmaPtr = D_800C7DF8.firstUsed;
        }
    }
    if (dmaPtr == NULL) {
        return osVirtualToPhysical(lastDmaPtr->ptr) + delta;
    }

    D_800C7DF8.firstFree = (AudioManagerDMABuffer *) dmaPtr->node.next;
    alUnlink(&dmaPtr->node);

    if (lastDmaPtr != NULL) {
        alLink(&dmaPtr->node, &lastDmaPtr->node);
    } else if (D_800C7DF8.firstUsed != NULL) {
        lastDmaPtr = D_800C7DF8.firstUsed;
        D_800C7DF8.firstUsed = dmaPtr;
        dmaPtr->node.next = &lastDmaPtr->node;
        dmaPtr->node.prev = NULL;
        lastDmaPtr->node.prev = &dmaPtr->node;
    } else {
        D_800C7DF8.firstUsed = dmaPtr;
        dmaPtr->node.next = NULL;
        dmaPtr->node.prev = NULL;
    }

    foundBuffer = dmaPtr->ptr;
    addr -= delta;
    dmaPtr->startAddr = addr;
    dmaPtr->lastFrame = D_80078DD0;
    osPiStartDma(&D_800C8648[D_80078DD4++], OS_MESG_PRI_HIGH, OS_READ,
                 addr, foundBuffer, 0x200, &D_800C9020);
    return osVirtualToPhysical(foundBuffer) + delta;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_80002188.s")
#endif
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
/* PROVENANCE: body adapted from Diddy Kong Racing's public decomp,
 * src/audiomgr.c::__clearAudioDMA; Mickey's DMA state and queue globals remain authoritative. */
void func_8000238C(void) {
    u32 i;
    OSIoMesg *iomsg = NULL;
    AudioManagerDMABuffer *dmaPtr;
    void *nextPtr;

    for (i = 0; i < D_80078DD4; i++) {
        if (osRecvMesg(&D_800C9020, (OSMesg *)&iomsg, OS_MESG_NOBLOCK) == -1) {
            D_80078DF0 |= 4;
        }
    }

    dmaPtr = D_800C7DF8.firstUsed;
    while (dmaPtr != NULL) {
        nextPtr = dmaPtr->node.next;
        if (dmaPtr->lastFrame + 1 < D_80078DD0) {
            if (D_800C7DF8.firstUsed == dmaPtr) {
                D_800C7DF8.firstUsed =
                    (AudioManagerDMABuffer *)dmaPtr->node.next;
            }
            alUnlink(&dmaPtr->node);
            if (D_800C7DF8.firstFree != NULL) {
                alLink(&dmaPtr->node, &D_800C7DF8.firstFree->node);
            } else {
                D_800C7DF8.firstFree = dmaPtr;
                dmaPtr->node.next = NULL;
                dmaPtr->node.prev = NULL;
            }
        }
        dmaPtr = (AudioManagerDMABuffer *) nextPtr;
    }

    D_80078DD4 = 0;
}
