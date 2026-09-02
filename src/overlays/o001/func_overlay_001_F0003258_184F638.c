#include "overlays/overlay_001.h"

typedef struct Overlay1C1bState {
    s8 index;
    u8 pad01[0x1A7];
    u16 flags;
    u8 pad1AA[0x1D2];
    s16 path;
    u8 selector;
    u8 selectorCopy;
    u8 pad380[3];
    s8 modeIndex;
    s8 maxIndex;
    u8 eventMode;
    u8 pad386[0x12];
    s32 word398;
    f32 offset;
    u8 pad3A0[0x12];
    s8 mode3B2;
    u8 pad3B3[7];
    s16 lastMode;
    u8 pad3BC[0x44];
    s32 word400;
    s32 word404;
    s32 word408;
    s32 word40C;
    u8 pad410[0x44];
    u8 changed;
    u8 nextMode;
    u8 pad45E[2];
} Overlay1C1bState;

typedef struct Overlay1C1bObject {
    u8 pad00[0x64];
    Overlay1C1bState *state;
} Overlay1C1bObject;

typedef struct Overlay1C1bTableEntry {
    u8 pad00[6];
    u8 rank;
    u8 pad07;
    s32 value;
    u8 pad0C[0x1C];
} Overlay1C1bTableEntry;

extern s32 func_overlay_001_F0000000_184C3E0();
extern u8 D_0[];
extern s32 D_1D58;
extern s32 D_1D8C;
extern f32 D_E4;

/* promotion_trial: text-size-differs, 0 words; linked candidate is 48 bytes short (no text comparison).
 * workbench: structure-mismatch, 182 differing-word floor, first mismatch +0x0; 188 candidate instructions versus 200 target, frame 0xF0 versus 0xD8.
 * Structural gap: 12 instructions and 24 frame bytes remain; candidate fits 0x320 owner but is not shape-exact. */
#ifdef NON_MATCHING
void func_overlay_001_F0003258_184F638(void) {
    s32 objectCount;
    volatile s32 initialized;
    s32 index;
    s32 loopIndex;
    s32 innerIndex;
    s32 changed;
    s32 value;
    s32 otherValue;
    s32 *score;
    s32 *scratch;
    Overlay1C1bObject **objects;
    Overlay1C1bObject **cursor;
    Overlay1C1bObject *object;
    Overlay1C1bState *state;
    Overlay1C1bState *context;
    Overlay1C1bTableEntry *table;
    s32 scoreStack[8];
    Overlay1C1bState *stateStack[8];
    f32 threshold;
    f32 current;
    f32 firstScore;
    f32 secondScore;
    u8 orderValue;

    initialized = 0;
    objects = (Overlay1C1bObject **)(s32)
        func_overlay_001_F0000000_184C3E0(&objectCount);
    context = (Overlay1C1bState *)(s32)
        func_overlay_001_F0000000_184C3E0();
    table = (Overlay1C1bTableEntry *)(s32)
        func_overlay_001_F0000000_184C3E0();
    if (objectCount == 0 || D_1D58 == 0) {
        return;
    }

    index = objectCount - 1;
    cursor = &objects[index];
    threshold = D_E4;
    do {
        object = *cursor;
        state = object->state;
        if (state->modeIndex < *(s8 *)((u8 *)context + 0x86)) {
            value = 0;
            if (objectCount > 0) {
                if (state->index != D_0[0]) {
                    do {
                        value++;
                    } while ((value < objectCount) &&
                             (state->index != D_0[value]));
                }
            }
            if (initialized == 0) {
                initialized = 1;
                func_overlay_001_F0000000_184C3E0(object, 0x11, context);
            }
            if (state->index == D_0[value]) {
                current = *(f32 *)&D_0[value * 4];
                if (current == threshold) {
                    current = (f32)(state->modeIndex * D_1D8C) + state->offset;
                }
                value = state->word400;
                state->word400 = value +
                    (s32)((f32)value *
                          (1.0f - (current /
                                   (f32)(*(s8 *)((u8 *)context + 0x86) *
                                         D_1D8C))));
            }
            table[state->index].value = state->word400;
        }
        index--;
        scoreStack[state->index] = table[state->index].value;
        stateStack[state->index] = state;
        cursor--;
    } while (index != 0);

    changed = 0;
    do {
        changed = 0;
        innerIndex = objectCount - 2;
        if (index != 0) {
            do {
                secondScore = ((f32 *)D_0)[innerIndex + 1];
                firstScore = ((f32 *)D_0)[innerIndex];
                if (firstScore < secondScore) {
                    ((f32 *)D_0)[innerIndex] = secondScore;
                    ((f32 *)D_0)[innerIndex + 1] = firstScore;
                    changed = 1;
                    orderValue = D_0[innerIndex + 1];
                    D_0[innerIndex + 1] = D_0[innerIndex];
                    D_0[innerIndex] = orderValue;
                }
                innerIndex--;
            } while (innerIndex != 0);
        }
    } while (changed != 0);

    index = 0;
    if (objectCount > 0) {
        do {
            state = stateStack[index];
            table[state->index].rank = (u8)index;
            state->lastMode = (s16)index;
            index++;
        } while (index < objectCount);
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/func_overlay_001_F0003258_184F638/func_overlay_001_F0003258_184F638.s")
#endif
