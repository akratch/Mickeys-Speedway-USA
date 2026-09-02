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
    u8 pad00[0x0C];
    f32 x;
    u8 pad10[4];
    f32 y;
    u8 pad18[0x4C];
    Overlay1C1bState *state;
} Overlay1C1bObject;

typedef struct Overlay1C1bController {
    u8 pad00[0x86];
    s8 count;
} Overlay1C1bController;

typedef struct Overlay1C1bPair {
    f32 distance;
    f32 value;
    s32 valid;
} Overlay1C1bPair;

typedef f32 (*Overlay1F32Call)(f32 value);

extern s32 func_overlay_001_F0000000_184C3E0();
extern f32 func_overlay_001_F00000E4_184C4C4(f32 first, f32 second);
extern s32 *func_overlay_001_F00004B4_184C894(Overlay1C1bObject *object);
extern void func_overlay_001_F0001D78_184E158(s32 index, void *objects,
                                               s32 count);
extern void func_overlay_001_F00028D4_184ECB4(s32 index, void *objects,
                                               s32 count);
extern void func_overlay_001_F000293C_184ED1C(s32 index, void *objects,
                                               s32 count);
extern void func_overlay_001_F000296C_184ED4C(void *context, s32 *arg0);
extern void func_overlay_001_F0002AA4_184EE84(s32 *arg0);

extern s32 D_0;
extern s32 D_1D78;
extern s32 D_1D8C;
extern s32 D_1D94;
extern s32 D_1D98;
extern s32 *D_1D74;
extern u8 D_1DCC[];
extern u8 D_1DD4[];
extern u8 D_1DC0[];
extern Overlay1C1bPair D_1BA8[][6];
extern f32 D_1DAC;
extern f32 D_1DB0;
extern f32 D_1DB4;
extern f32 D_1DB8;

#define O1_MODE D_0
#define O1_BYTES ((u8 *)&D_0)
#define O1_FLOATS ((f32 *)&D_0)

/* promotion_trial: build-error, 0 words; schedule divergence at D_1D78, D_1D98, D_1DB8 (no linked text).
 * workbench: structure-mismatch, 433 differing-word floor, first mismatch +0x0; 447 candidate instructions versus 451 target, frame 0xC8 versus 0xB0.
 * Structural gap: four instructions and 24 frame bytes remain, plus unresolved overlay relocation layout; candidate fits 0x70C but is not shape-exact. */
#ifdef NON_MATCHING
s32 *func_overlay_001_F0002B4C_184EF2C(s32 *arg0) {
    s32 objectCount;
    Overlay1C1bObject **objects;
    Overlay1C1bObject **cursor;
    Overlay1C1bObject *outerObject;
    Overlay1C1bObject *innerObject;
    Overlay1C1bState *outerState;
    Overlay1C1bState *innerState;
    Overlay1C1bState *worldState;
    Overlay1C1bPair *pair;
    Overlay1C1bPair *reversePair;
    void *context;
    s32 index;
    s32 innerIndex;
    s32 loopValue;
    s32 value;
    s32 changed;
    s32 bestIndex;
    s32 remaining;
    s32 total;
    s32 sum;
    f32 first;
    f32 second;
    f32 current;
    f32 anchor;
    f32 step;
    f32 *weights;
    u8 *order;

    context = (void *)(s32)func_overlay_001_F0000000_184C3E0();
    objects = (Overlay1C1bObject **)(s32)
        func_overlay_001_F0000000_184C3E0(&objectCount);
    if (O1_MODE == 3) {
        func_overlay_001_F0002AA4_184EE84(arg0);
    } else {
        func_overlay_001_F000296C_184ED4C(context, arg0);
    }
    func_overlay_001_F0000000_184C3E0(arg0);
    if (objectCount == 0) {
        return (s32 *)0;
    }

    changed = O1_MODE;
    order = D_1DD4;
    if (changed != 0) {
        D_1D94 = (s32)arg0;
        *(f32 *)&O1_BYTES[4] = (f32)(s32)arg0;
        D_1D78 = (O1_MODE != 0);
        order = D_1DCC;
        loopValue = 4;
        do {
            order[loopValue] = order[loopValue - 1];
            D_1DD4[loopValue] = order[loopValue];
            loopValue--;
        } while (loopValue != 0);

        remaining = objectCount - 1;
        cursor = &objects[remaining];
        do {
            D_1DC0[remaining] = 0;
            if (func_overlay_001_F00004B4_184C894(*cursor) != NULL) {
                value = O1_MODE;
                if (value == 1) {
                    func_overlay_001_F0001D78_184E158(remaining, context,
                                                       objectCount);
                } else if (value == 2) {
                    func_overlay_001_F00028D4_184ECB4(remaining, context,
                                                       objectCount);
                } else if (value == 3) {
                    func_overlay_001_F000293C_184ED1C(remaining, context,
                                                       objectCount);
                }
            }
            cursor--;
            remaining--;
        } while (remaining >= 0);

        if ((changed == 1) && (objectCount >= 2)) {
            total = objectCount * objectCount;
            pair = &D_1BA8[0][0];
            loopValue = total - 1;
            if (total != 0) {
                do {
                    pair->valid = 0;
                    pair++;
                    loopValue--;
                } while (loopValue != 0);
            }

            bestIndex = objectCount - 1;
            remaining = bestIndex;
            if (objectCount != 0) {
                cursor = &objects[remaining];
                do {
                    outerObject = *cursor;
                    outerState = outerObject->state;
                    innerIndex = bestIndex;
                    if (objectCount != 0) {
                        do {
                            innerObject = objects[innerIndex];
                            innerState = innerObject->state;
                            pair = &D_1BA8[(u8)outerState->index]
                                             [(u8)innerState->index];
                            reversePair = &D_1BA8[(u8)innerState->index]
                                                    [(u8)outerState->index];
                            if (remaining != innerIndex) {
                                if (pair->valid == 0) {
                                    first = outerObject->x - innerObject->x;
                                    second = outerObject->y - innerObject->y;
                                    pair->distance =
                                        ((Overlay1F32Call)
                                             func_overlay_001_F0000000_184C3E0)(
                                            (first * first) +
                                            (second * second));
                                    pair->value = func_overlay_001_F00000E4_184C4C4(
                                        ((Overlay1C1bState *)D_1DA0)->offset,
                                        innerState->offset);
                                    reversePair->distance = pair->distance;
                                    reversePair->value = -pair->value;
                                    pair->valid = 1;
                                }
                            } else {
                                pair->distance = 0.0f;
                                reversePair->distance = 0.0f;
                                pair->value = 0.0f;
                                reversePair->value = 0.0f;
                            }
                            innerIndex--;
                        } while (innerIndex > 0);
                    }
                    cursor--;
                    value = remaining;
                    remaining--;
                } while (value != 0);
            }

            weights = O1_FLOATS;
            order = O1_BYTES;
            do {
                changed = 0;
                loopValue = objectCount - 2;
                if (remaining != 0) {
                    pair = &D_1BA8[0][0] + loopValue;
                    do {
                        if (weights[loopValue] < weights[loopValue + 1]) {
                            first = weights[loopValue];
                            weights[loopValue] = weights[loopValue + 1];
                            weights[loopValue + 1] = first;
                            value = order[loopValue];
                            order[loopValue] = order[loopValue + 1];
                            order[loopValue + 1] = value;
                            changed = 1;
                        }
                        pair--;
                        loopValue--;
                    } while (loopValue != 0);
                }
            } while (changed != 0);

            bestIndex = remaining;
            if (objectCount != 0) {
                index = bestIndex;
                do {
                    outerObject = objects[(u8)index];
                    outerState = outerObject->state;
                    if (!(outerState->flags & 1)) {
                        bestIndex = remaining;
                        anchor = (f32)(outerState->modeIndex * D_1D8C) +
                                 outerState->offset;
                    } else {
                        bestIndex--;
                        if (bestIndex == 0) {
                            bestIndex = remaining;
                        } else {
                            index--;
                            continue;
                        }
                    }
                    index--;
                } while (index >= 0);
            }

            step = D_1DB8 / (f32)D_1D8C;
            index = bestIndex;
            loopValue = bestIndex;
            if (objectCount != 0) {
                do {
                    outerObject = objects[(u8)index];
                    outerState = outerObject->state;
            if (O1_MODE == 2) {
                        current = (weights[index] - weights[0]) * step + D_1DB0;
                    } else {
                        current = (anchor - weights[index]) * step + D_1DB0;
                    }
                    outerState->offset = current;
                    outerState->maxIndex = (s8)bestIndex;
                    if (!(outerState->flags & 1) ||
                        (outerState->flags & 0x10)) {
                        outerState->offset = 1.0f;
                    } else {
                        current = outerState->offset;
                        if (current < D_1DAC) {
                            outerState->offset = D_1DAC;
                        } else if (D_1DB4 < current) {
                            outerState->offset = D_1DB4;
                        }
                    }
                    index--;
                    loopValue--;
                } while (loopValue != 0);
            }
            outerObject = objects[(u8)O1_MODE];
            value = outerObject->state->index;
            if (D_1D74 != (s32 *)(s32)value) {
                D_1D74 = (s32 *)(s32)value;
                D_1D98 = (s32)(s32)outerObject;
            }
        }

        if (O1_MODE == 0) {
            func_overlay_001_F0000000_184C3E0(
                (s32 *)0xA, (u8 *)changed);
        }
        remaining = objectCount - 1;
        if (objectCount != 0) {
            do {
                if (D_1DC0[remaining] != 0) {
                    outerState = objects[remaining]->state;
                    sum = 0;
                    total = 0;
                    if (outerState->eventMode != 0) {
                        innerState = objects[(u8)(outerState->eventMode - 1)]->state;
                        value = outerState->modeIndex;
                        while (total < value) {
                            sum += innerState->word404;
                            total++;
                            innerState++;
                        }
                        value = outerState->word400 - sum;
                        if ((value >= -0x7FFE) && (value < 0x7FFF)) {
                            if ((outerState->eventMode == 1) &&
                                (innerState->modeIndex < 3)) {
                                innerState->word400 = value;
                                innerState->lastMode = 0xB4;
                            }
                            outerState->word400 = -value;
                            outerState->lastMode = 0xB4;
                        }
                    }
                }
                remaining--;
            } while (remaining >= 0);
        }
    }
    return (s32 *)0;
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_middle_c1b_a/func_overlay_001_F0002B4C_184EF2C.s")
#endif
