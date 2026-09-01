#include "PR/ultratypes.h"
#include "game/memory.h"

/* PROVENANCE: body adapted from Jet Force Gemini's public decompilation,
 * src/textures.c:resetColourCycle. Mickey's layout and compiler output remain
 * authoritative. */
typedef struct ColourCycle {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    struct ColourCycle *unkC;
} ColourCycle;

/* PROVENANCE: body adapted from Jet Force Gemini's public decompilation,
 * src/textures.c:resetMixCycle. Mickey's layout and compiler output remain
 * authoritative. */
typedef struct PulsatingLightDataFrame {
    u16 value;
    u16 time;
} PulsatingLightDataFrame;

typedef struct PulsatingLightData {
    u16 numberFrames;
    u16 currentFrame;
    u16 time;
    u16 totalTime;
    s32 outColorValue;
    PulsatingLightDataFrame frames[1];
} PulsatingLightData;

extern MemoryPoolSlot *func_8002B280(s32 size, u32 tag);
extern u32 *piRomLoad(u32 assetIndex);
extern s32 piRomLoadSection(u32 assetIndex, u32 address, s32 assetOffset,
                             s32 size);
extern s32 byteswap32(u8 *address);
extern u8 *func_8004D7E0(u8 *compressed, u8 *output);
extern void mmFree(void *address);

void func_80036A80(ColourCycle *cycle) {
    ColourCycle *temp_v0;

    temp_v0 = cycle->unkC;
    cycle->unk0 = 0;
    cycle->unk4 = 0;
    cycle->unk8 = temp_v0->unk8;
    cycle->unk9 = temp_v0->unk9;
    cycle->unkA = temp_v0->unkA;
    cycle->unkB = temp_v0->unkB;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37680/func_80036AB0.s")

void func_80036C60(PulsatingLightData *data) {
    s32 i;

    data->currentFrame = 0;
    data->time = 0;
    data->totalTime = 0;
    data->outColorValue = data->frames[0].value;
    for (i = 0; i < data->numberFrames; i++) {
        data->totalTime += data->frames[i].time;
    }
}

/* PROVENANCE: body adapted from Jet Force Gemini's public decompilation,
 * src/textures.c:updateMixCycle. Mickey's layout and compiler output remain
 * authoritative. */
void func_80036CAC(PulsatingLightData *data, s32 timeDelta) {
    s32 thisFrameIndex;
    s32 nextFrameIndex;

    if (data->numberFrames > 1) {
        data->time += timeDelta;
        while (data->time >= data->totalTime) {
            data->time -= data->totalTime;
        }
        while (data->time >= data->frames[data->currentFrame].time) {
            data->time -= data->frames[data->currentFrame].time;
            data->currentFrame++;
            if (data->currentFrame >= data->numberFrames) {
                data->currentFrame = 0;
            }
        }
        thisFrameIndex = data->currentFrame;
        nextFrameIndex = thisFrameIndex + 1;
        if (nextFrameIndex >= data->numberFrames) {
            nextFrameIndex = 0;
        }

        data->outColorValue = data->frames[thisFrameIndex].value +
                              ((data->frames[nextFrameIndex].value * data->time) /
                               data->frames[thisFrameIndex].time);
    }
}
/* PROVENANCE: body adapted from Jet Force Gemini's public decompilation,
 * src/screen.c:screenLoad. Mickey's loader calls and bytes remain authoritative. */
s32 *func_80036DD0(s32 screenIndex) {
    s32 *screenTable;
    s32 start;
    s32 size;
    s32 count;
    s32 uncompressedSize;
    u8 *decompressedAddr;
    u8 *sp1C;
    u32 compressedAddr;

    screenTable = (s32 *) piRomLoad(0x14);
    for (count = 0; screenTable[count] != -1; count++) {}
    count--;
    if (count == 0) {
        mmFree(screenTable);
        return (u8 *) 0x80100000;
    }
    if ((screenIndex < 0) || (screenIndex >= count)) {
        screenIndex = 0;
    }
    size = screenTable[screenIndex + 1] - screenTable[screenIndex];
    start = screenTable[screenIndex];
    decompressedAddr = NULL;
    sp1C = (u8 *) func_8002B280(0x10, 0x90);
    if (sp1C != NULL) {
        piRomLoadSection(0x13, (u32) sp1C, start, 0x10);
        uncompressedSize = byteswap32(sp1C) + 0x80;
        mmFree(sp1C);
        decompressedAddr = (u8 *) func_8002B280(uncompressedSize, 0x90);
        if (decompressedAddr != NULL) {
            compressedAddr = (u32) ((decompressedAddr + uncompressedSize) - size);
            compressedAddr -= compressedAddr & 0xF;
            piRomLoadSection(0x13, compressedAddr, start, size);
            func_8004D7E0((u8 *) (compressedAddr & 0xFFFFFFFF), decompressedAddr);
        }
    }
    mmFree(screenTable);
    return decompressedAddr;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37680/func_80036F08.s")
