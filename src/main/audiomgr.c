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

typedef struct Overlay5SoundConfig {
    void *field00;
    s32 field04;
    s32 field08;
    s32 field0C;
    s32 field10;
    void *field14;
    s32 field18;
    s8 field1C;
} Overlay5SoundConfig;

typedef struct AudioManagerConfig {
    void *field00;
    s32 field04;
    s32 field08;
    s32 maxFXbusses;
    ALDMANew dmaproc;
    ALHeap *heap;
    s32 outputRate;
    u8 fxType[4];
    void *params[2];
} AudioManagerConfig;

typedef struct AudioManagerState {
    u8 pad000[0x280];
    u8 *bufferStart;
    u8 *bufferEnd;
    u8 *altBufferStart;
    u8 *altBufferEnd;
    void *largeBufferStart;
    void *largeBufferEnd;
    OSScTask task;
    s16 frameSamples[3];
    u16 pad30E;
    u8 *cmdLists[3];
    u8 *cmdListsAlt[3];
    u8 *largeData[2];
} AudioManagerState;

extern AudioManagerDMAState D_800C7DF8;
extern AudioManagerDMABuffer D_800C7E08[];
extern s32 func_80002188(s32 addr, s32 len, void *state);
extern OSThread D_800C7A50;
extern s16 D_800C7A56[];
extern ALHeap *D_800BFA34;
extern u8 D_80078DF4[];
extern u32 D_80078DD0;
extern u32 D_80078DD4;
extern AudioManagerDMABuffer *D_80078DC0;
extern s32 D_80078DC4;
extern s32 D_80078DC8;
extern s32 D_80078DCC;
extern s32 D_80078DD8;
extern s32 D_80078DDC;
extern s32 D_80078DE0;
extern s32 D_80078DE4;
extern s32 D_80078DE8;
extern s32 D_80078DEC;
extern volatile u32 D_80078DF0;
extern OSMesgQueue D_800C9020;
extern OSIoMesg D_800C8648[];
extern OSSched *D_800BFA30;
extern OSScClient D_800BFA38;
extern OSMesgQueue D_800C7D84;
extern OSMesgQueue D_800C7D9C;
extern OSMesg D_800C7DB4[];
extern OSMesg D_800C7DD4[];
extern N_ALGlobals D_800C7C80;
extern u64 D_800C7A48[];
extern OSMesg D_800C9038[];
extern s32 D_800D2FB0;
extern s32 D_800C863C;
extern s32 D_800C8640;
extern s32 D_800C8644;
extern s32 D_800C91DC;
extern OSIoMesg D_800C8648[];
extern OSScTask D_800C7CE8;
extern u32 D_A4500004;
extern u64 D_80076110[];
extern u64 D_80077950[];
extern u64 D_80077AD0[];
extern u64 D_80084B00[];
extern void *func_8002B280(s32 size, s32 tag);
extern void mmFree(void *address);
extern s32 osAiSetFrequency(s32 frequency);
extern s32 osAiSetNextBuffer(void *bufPtr, u32 size);
extern void osScAddClient(OSSched *sc, OSScClient *client, OSMesgQueue *msgQ, u8 id);
extern OSMesgQueue *osScGetCmdQ(OSSched *scheduler);
extern ALDMAproc audioManager_DMAInitProc(void *state);
extern void func_80001A84(void *arg);
extern void func_8000238C(void);
extern void func_80001BF4(void);
extern void func_80002134(void);

/* PROVENANCE: body adapted from Diddy Kong Racing's public decomp,
 * src/audiomgr.c::amCreateAudioMgr; Mickey's config, queues, and heap globals remain authoritative. */
/* Verdict: structure-mismatch; 154 differing sites, 203 candidate vs 209 target instructions. */
/* First mismatch: function offset +0x00; candidate frame -344 vs target -336. */
/* Gap: initializer stack, field, and copy shapes remain displaced from the target. */
#ifdef NON_MATCHING
typedef struct AudioManagerEffectParams {
    u32 words[0x108 / sizeof(u32)];
} AudioManagerEffectParams;

void func_80001740(Overlay5SoundConfig *configArg, s32 priority, void *context) {
    s32 i;
    f32 fsize;
    u8 *dmaMemory;
    AudioManagerDMABuffer *dmaPtr;
    AudioManagerDMABuffer *prevDmaPtr;

    D_800BFA30 = (OSSched *)context;
    D_800BFA34 = (ALHeap *)configArg->field14;
    configArg->field18 = osAiSetFrequency(0x5604);
    ((AudioManagerConfig *)configArg)->dmaproc = audioManager_DMAInitProc;

    for (i = 0; i < configArg->field0C; i++) {
        if (((AudioManagerConfig *)configArg)->fxType[i] == AL_FX_CUSTOM) {
            AudioManagerEffectParams customParams;

            customParams = *(AudioManagerEffectParams *)D_80078DF4;
            ((AudioManagerConfig *)configArg)->params[i] = &customParams;
        }
    }

    n_alInit(&D_800C7C80, (ALSynConfig *)configArg);
    fsize = ((f32)configArg->field18 * 2) / (f32)D_800D2FB0;
    i = (s32)fsize;
    D_800C863C = i;
    if ((f32)i < fsize) {
        i++;
        D_800C863C = i;
    }
    i = ((i / 0xB8) * 0xB8) + 0xB8;
    D_800C863C = i;
    D_800C8640 = i - 0xB8;
    D_800C8644 = 0x1000;

    ((AudioManagerState *)&D_800C7A50)->bufferStart =
        alHeapDBAlloc(0, 0, (ALHeap *)configArg->field14, 1, 0x7580);
    ((AudioManagerState *)&D_800C7A50)->bufferEnd =
        ((AudioManagerState *)&D_800C7A50)->bufferStart + 0x3AC0;
    ((AudioManagerState *)&D_800C7A50)->altBufferStart =
        ((AudioManagerState *)&D_800C7A50)->bufferStart;
    ((AudioManagerState *)&D_800C7A50)->altBufferEnd =
        ((AudioManagerState *)&D_800C7A50)->bufferEnd;

    dmaMemory = alHeapDBAlloc(0, 0, (ALHeap *)configArg->field14, 1,
                              D_800C8644 * 0xC);
    for (i = 0; i < 3; i++) {
        ((AudioManagerState *)&D_800C7A50)->cmdLists[i] = dmaMemory;
        ((AudioManagerState *)&D_800C7A50)->cmdListsAlt[i] = dmaMemory;
        ((AudioManagerState *)&D_800C7A50)->frameSamples[i] = 0;
        dmaMemory += D_800C8644 * 4;
    }

    dmaMemory = alHeapDBAlloc(0, 0, (ALHeap *)configArg->field14, 1, 0xD200);
    D_800C7E08[0].node.prev = NULL;
    D_800C7E08[0].node.next = NULL;
    dmaPtr = &D_800C7E08[1];
    prevDmaPtr = &D_800C7E08[0];
    do {
        alLink(&dmaPtr->node, &prevDmaPtr->node);
        dmaPtr++;
        prevDmaPtr->ptr = dmaMemory;
        prevDmaPtr++;
        dmaMemory += 0x200;
    } while (dmaPtr != (AudioManagerDMABuffer *)&D_800C863C);
    prevDmaPtr->ptr = dmaMemory;

    osCreateMesgQueue(&D_800C7D9C, D_800C7DD4, 8);
    osCreateMesgQueue(&D_800C7D84, D_800C7DB4, 8);
    osCreateMesgQueue(&D_800C9020, D_800C9038, 0x69);
    osCreateThread((OSThread *)&D_800C7A50, -4, func_80001A84, NULL,
                   D_800C7A48, priority);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/audiomgr/func_80001740.s")
#endif

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
