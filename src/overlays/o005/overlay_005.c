#include "overlays/overlay_005.h"

/* Overlay 5, ADR 0006 consolidation: the -O2 game-code portion. */

void overlay5InitSequence(void *owner, s32 value) {
    Overlay5SequenceHeader header;

    header.count = 14;
    header.value = value;
    overlay5SequenceInitReloc((u8 *)owner + 0x48, &header, 0);
}

/*
 * 230/233 words (2026-09-09, was 209/233).  Exact 0x3A4 size, exact 0x98 frame,
 * every stack home and all 71 relocations agree.  The three remaining words are
 * one colour fact: the short-lived address web for `gOverlay5Span1Size` -- live
 * only from its %hi at +0x9C to the store at +0xC8, while $s0 still carries
 * `&gOverlay5Span0Size` -- takes v0 here and v1 in the target.  Same schedule,
 * same %hi/%lo split points, same spill points.
 *
 * What closed the other twenty-one words, and the frame law behind them:
 * IDO packs the declared locals of *every* scope into one chain, top-down in
 * declaration order, from T = frame; frame = align8(below + S) where S is the
 * summed local bytes and `below` (0x38 here) is fixed by the out-argument and
 * saved-register regions.  Reading the target's three visible homes back
 * through that law pins the whole declaration list: 8 bytes above soundConfig,
 * soundConfig at 0x70 sized 0x24, resource, bankSize at 0x68, maxValue, one
 * more 4-byte local, sequenceConfig at 0x4C, then five loop locals.  That is
 * exactly the DKR `audio_init` declaration list (see the PROVENANCE note on
 * `Overlay5SoundConfig`), `pad` included -- an unused local that still takes a
 * home.  The 0x24 sound config is the load-bearing half: at 0x20 the frame is
 * 0x90 and every home is wrong.  The +0x214/+0x218 swap is the initialiser
 * order `destinationOffset` before `destination`; the six orders of those three
 * initialisers separate 3/5/6.
 *
 * Flat at three, measured on the exact-size candidate: all 64 spellings of the
 * span1 store and its two reads (`*(&g)`, `(&g)[0]`, `*(u32 *)&g`); all 256
 * physical line groupings of the nine span statements; every legal placement of
 * the two ScaleValue statements (24 cells, the rest change size); all 24 orders
 * of the four head statements; `maxValue = 0` at seven later anchors; and a
 * dead `pad = <expr>` at 96 statement/expression points -- uopt kills a store
 * that is never read, so it reserves no colour and cannot be used as a spacer.
 * Naming the alloc results or the span sizes as locals shortens the function.
 */
#ifdef NON_MATCHING
void overlay5InitializeAudio(void *context) {
    s32 index;
    Overlay5SoundConfig soundConfig;
    Overlay5Resource *resource;
    u32 bankSize;
    u32 maxValue;
    u32 pad;
    Overlay5SequenceConfig sequenceConfig;

    maxValue = 0;
    gOverlay5AudioOwner = gOverlay5OwnerSoundState;
    alHeapInit(gOverlay5HeapState, gOverlay5HeapMemory, 0x30D40);

    resource = func_8002E148(0x31);
    gOverlay5Span0Size = resource->end - resource->span1End;
    gOverlay5Span0 = func_8002B280(gOverlay5Span0Size, 0x82);
    func_8002E2E0(0x32, gOverlay5Span0,
                  (void *)resource->span1End, gOverlay5Span0Size);
    gOverlay5Span0ScaleValue = gOverlay5Span0Size / 10U;

    gOverlay5Span1Size = resource->span1End - resource->span1Start;
    gOverlay5Span1 = func_8002B280(gOverlay5Span1Size, 0x82);
    func_8002E2E0(0x32, gOverlay5Span1,
                  (void *)resource->span1Start, gOverlay5Span1Size);
    gOverlay5Span1ScaleValue = gOverlay5Span1Size / 3U;

    gOverlay5Span2 = func_8002B280(resource->span0Start, 0x82);
    func_8002E2E0(0x32, gOverlay5Span2, 0, resource->span0Start);
    alBnkfNew(gOverlay5Span2,
              func_8002E35C(0x32, (void *)resource->span0Start));

    gOverlay5Bank = alHeapDBAlloc(0, 0, gOverlay5HeapState, 1, 4);
    func_8002E2E0(0x32, gOverlay5Bank,
                  (void *)resource->span0End, 4);

    bankSize = (u32)gOverlay5Bank->count * sizeof(Overlay5BankEntry) + 4;
    gOverlay5Bank = func_8002B280(bankSize, 0x82);
    func_8002E2E0(0x32, gOverlay5Bank,
                  (void *)resource->span0End, bankSize);
    alSeqFileNew(gOverlay5Bank,
                 func_8002E35C(0x32, (void *)resource->span0End));

    gOverlay5EntryValues = func_8002B280(
        (u32)gOverlay5Bank->count * sizeof(*gOverlay5EntryValues), 0x82);
    {
        Overlay5Bank *bank = gOverlay5Bank;
        u32 *destination;
        u32 destinationOffset;
        u32 sourceOffset;

        index = 0;
        if (bank->count > 0) {
            destinationOffset = 0;
            destination = gOverlay5EntryValues;
            sourceOffset = 0;
            do {
                u32 value;

                *destination = *(u32 *)((u8 *)bank + sourceOffset + 8);
                destination = (u32 *)((u8 *)gOverlay5EntryValues +
                                      destinationOffset);
                value = *destination;
                if ((value & 1) != 0) {
                    *destination = value + 1;
                    destination = (u32 *)((u8 *)gOverlay5EntryValues +
                                          destinationOffset);
                    value = *destination;
                }
                if (maxValue < value) {
                    maxValue = value;
                }
                bank = gOverlay5Bank;
                index++;
                destinationOffset += 4;
                destination++;
                sourceOffset += 8;
            } while (index < bank->count);
        }
    }

    soundConfig.maxVVoices = 0x2C;
    soundConfig.maxPVoices = 0x28;
    soundConfig.maxUpdates = 0x80;
    soundConfig.dmaproc = NULL;
    soundConfig.fxType[0] = 6;
    soundConfig.maxFXbusses = 1;
    soundConfig.outputRate = 0;
    soundConfig.heap = gOverlay5HeapState;
    func_80001740(&soundConfig, 0x0C, context);

    gOverlay5Player0 = overlay5CreatePlayer(0x20, 0x96);
    gOverlay5Player1 = overlay5CreatePlayer(0x10, 0x32);

    sequenceConfig.maxEvents = 0xC8;
    sequenceConfig.maxSounds = 0x20;
    sequenceConfig.maxChannels = 0x10;
    sequenceConfig.numGroups = 5;
    sequenceConfig.heap = gOverlay5HeapState;
    gsSndpNew(&sequenceConfig);

    func_80001BA0();
    func_80000450(0);
    func_8002B768(resource);
    func_800039F0();
    alSurround_OutputType(4);
    alSurround_ReverbSetup(0, 3);
    osCreateMesgQueue(gOverlay5MessageQueue, gOverlay5MessageBuffer, 1);
    n_alCSPSetMessageQ(gOverlay5Player0, gOverlay5MessageQueue);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o005/overlay_005/func_overlay_005_F000031C_185B744.s")
#endif

void *overlay5CreatePlayer(s32 arg0, s32 arg1) {
    void *player;
    Overlay5PlayerConfig config;

    config.arg0 = arg0;
    config.arg1 = arg1;
    config.voiceCount = arg0;
    config.channels = 0x10;
    config.heap = gOverlay5AudioHeap;
    config.initQueue = gOverlay5InitQueue;
    config.eventQueue = gOverlay5EventQueue;
    config.frameCallback = gOverlay5FrameCallback;
    player = overlay5AllocPlayerReloc(0, 0, gOverlay5AudioHeap, 1, 0x90);
    overlay5InitPlayerReloc(player, &config);
    overlay5AttachBankReloc(player, gOverlay5AudioState->sequenceBank);
    return player;
}

/* PLATEAU-HANDOFF:overlay5InitializeAudio:start
 * symbol: overlay5InitializeAudio
 * score: 230/233 words
 * frame: 0x98
 * relocations: 71
 * first-mismatch: +0x9C
 * summary: Exact size, frame, every stack home and all 71 relocations; the three remaining words are one colour fact on the gOverlay5Span1Size address web.
 * PLATEAU-HANDOFF:overlay5InitializeAudio:end
 */
