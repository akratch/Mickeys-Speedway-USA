#include "PR/ultratypes.h"

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
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37680/func_80036CAC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37680/func_80036DD0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37680/func_80036F08.s")
