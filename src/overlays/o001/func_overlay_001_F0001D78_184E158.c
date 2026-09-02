#include "overlays/overlay_001.h"

typedef struct Overlay1Input {
    u8 pad00[0x0C];
    f32 x;
    u8 pad10[4];
    f32 y;
} Overlay1Input;

typedef struct Overlay1PathPoint {
    u8 bytes[0x10];
} Overlay1PathPoint;

typedef struct Overlay1PathRecord {
    u8 pad00[0x0E];
    u16 flags;
    u8 pad10[4];
    Overlay1PathPoint points[8];
} Overlay1PathRecord;

typedef struct Overlay1MotionState {
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
    u8 pad386;
    u8 timer;
    u8 timerState;
    u8 pad389[0x0F];
    s32 word398;
    f32 offset;
    u8 pad3A0[0x12];
    s8 mode3b2;
    u8 pad3B3[7];
    s16 lastMode;
    u8 pad3BC[0x44];
    s32 word400;
    s32 word404;
    s32 word408;
    s32 word40C;
    u8 pad410[0x4C];
    u8 changed;
    u8 nextMode;
    u8 pad45E[2];
} Overlay1MotionState;

typedef struct Overlay1Controller {
    u8 pad00[0x86];
    s8 count;
} Overlay1Controller;

typedef struct Overlay1Object {
    u8 pad00[0x64];
    Overlay1MotionState *state;
} Overlay1Object;

typedef struct Overlay1Result {
    s32 count;
    u8 pad04[4];
    s32 value;
    s32 values[8];
} Overlay1Result;

typedef s32 (*Overlay1NoArgCall)(void);
typedef s32 (*Overlay1PathCall)(void *value);
typedef s32 (*Overlay1CheckCall)(void *value, s32 x, s32 y);
typedef s32 (*Overlay1MetricCall)(f32 x, f32 y, void *state,
                                  void *record, s32 value);
typedef s32 (*Overlay1EventCall)(s32 event, void *state);
typedef s32 (*Overlay1IntEventCall)(s32 event, s32 value);
typedef s32 (*Overlay1BatchCall)(s32 event, s32 count, s32 value,
                                 s32 enabled);

extern s32 func_overlay_001_F0000000_184C3E0();
extern void func_overlay_001_F0000BD4_184CFB4(void);
extern s32 D_0;
extern s32 D_1D8C;
extern s32 D_1D94;
extern u8 D_1DC0[];
extern u8 D_1DC8[];
extern u8 D_1DD0[];
extern Overlay1PathRecord *D_1D58;
extern Overlay1PathRecord *D_1D60;
extern Overlay1PathRecord *D_1D68;
extern Overlay1PathRecord *D_1D68Read;
extern Overlay1PathRecord *D_1D6C;
extern Overlay1PathPoint *D_208;
extern Overlay1PathPoint *D_20C;
extern Overlay1PathPoint *D_210;
extern Overlay1PathPoint *D_214;
extern f32 D_D8;

#define O1_STATE (*(Overlay1MotionState * volatile *)(void *)&D_1DA0)

/* promotion_trial: build-error, schedule divergence at D_1D94; no linked text.
 * workbench: structure-mismatch, 584/627 differing words, frame 0x80 vs 0x70.
 * Structural gap: 28 instructions and relocation schedule remain unresolved. */
#ifdef NON_MATCHING
void func_overlay_001_F0001D78_184E158(s32 index,
                                       Overlay1Controller *controller,
                                       s32 limit) {
    Overlay1Result *results;
    Overlay1Object **objects;
    Overlay1Object **objectCursor;
    Overlay1Object *object;
    Overlay1MotionState *state;
    Overlay1Input *input;
    Overlay1PathRecord *record;
    Overlay1PathRecord *temporaryRecord;
    s32 objectCount;
    s32 switched;
    s32 current;
    s32 value;
    s32 loopValue;
    s32 indexValue;
    s32 valid;
    s32 *stateValue;
    u8 *byteValue;
    f32 currentOffset;
    f32 *output;

    results = (Overlay1Result *)(s32)func_overlay_001_F0000000_184C3E0();
    objects = (Overlay1Object **)(s32)
        func_overlay_001_F0000000_184C3E0(&objectCount);
    state = O1_STATE;
    if (!(state->flags & 1)) {
        input = D_1D9C;
        state->selector = (u8)func_overlay_001_F0000000_184C3E0(
            input->x, input->y, state, D_1D64, state->word398);
        state->selectorCopy = state->selector;
        state = O1_STATE;
    }

    if (!(state->flags & 8)) {
        currentOffset = (f32)(state->modeIndex * D_1D8C) + state->offset;
    }
    switched = 0;
    if (!(state->flags & 8)) {
        input = D_1D9C;
        if (func_overlay_001_F0000000_184C3E0(
                D_1D68, *(s32 *)&input->x, *(s32 *)&input->y) != 0) {
            switched = 1;
            temporaryRecord = (Overlay1PathRecord *)
                func_overlay_001_F0000000_184C3E0(D_1D6C);
            D_1D60 = D_1D64;
            D_1D64 = D_1D68;
            D_1D68 = D_1D6C;
            D_1D6C = temporaryRecord;
            state = O1_STATE;
            state->path = (s16)(((u8 *)D_1D68 - (u8 *)D_1D58) / 0x94);
            if (((Overlay1PathRecord *)D_1D64)->flags == 0) {
                if (state->changed != 0) {
                    state->changed = 0;
                    state->modeIndex++;
                } else if (state->modeIndex < controller->count) {
                    state->modeIndex++;
                    D_1DC8[state->modeIndex]++;
                    state->nextMode = D_1DD0[state->modeIndex];
                    if (controller->count == state->modeIndex) {
                        state->word40C =
                            (((state->word400 / 3) - (state->word408 / 3) -
                              (state->word404 / 3)) * 3);
                    }
                    if (state->modeIndex < 0) {
                        state->modeIndex = 0;
                    }
                    if (!(state->flags & 1) && state->modeIndex > 0 &&
                        state->modeIndex < 3) {
                        func_overlay_001_F0000000_184C3E0(0x1FC, state);
                        state = O1_STATE;
                    }
                    if (state->modeIndex > 0) {
                        D_1DC0[state->index] = 1;
                    }
                    if (state->maxIndex < state->modeIndex) {
                        state->maxIndex = state->modeIndex;
                        state = O1_STATE;
                    }
                    if (controller->count == state->modeIndex &&
                        *(s32 *)&D_0 == 1 && !(state->flags & 1)) {
                        if (results->count == 1) {
                            valid = 0;
                            objectCursor = (Overlay1Object **)(s32)
                                func_overlay_001_F0000000_184C3E0();
                            value = func_overlay_001_F0000000_184C3E0();
                            objectCursor = (Overlay1Object **)
                                ((u8 *)objectCursor + (value << 5));
                            current = 0;
                            state = O1_STATE;
                                if (state->modeIndex > 0) {
                                value = *(s32 *)((u8 *)objectCursor + 0x18);
                                stateValue = &state->word404;
                                do {
                                    if ((*stateValue < value) || value == 0) {
                                        valid = 1;
                                    }
                                    stateValue++;
                                    current += 4;
                                } while (current < (state->modeIndex * 4));
                            }
                            value = state->word400;
                            current = 0;
                            byteValue = (u8 *)objectCursor;
                            do {
                                loopValue = *(s32 *)byteValue;
                                current += 8;
                                if ((value < loopValue) || loopValue == 0) {
                                    valid = 1;
                                }
                                byteValue += 8;
                            } while (current != 0x18);
                            func_overlay_001_F0000000_184C3E0(
                                valid ? 6 : 5, 0x18, value, valid);
                            state = O1_STATE;
                        } else if (state->eventMode == 0) {
                            func_overlay_001_F0000000_184C3E0(5, state);
                            state = O1_STATE;
                        } else if (state->eventMode < 4) {
                            func_overlay_001_F0000000_184C3E0(0x1B, state);
                            state = O1_STATE;
                        } else {
                            func_overlay_001_F0000000_184C3E0(6, state);
                            state = O1_STATE;
                        }
                    }
                    if (state->modeIndex > 0) {
                        if (controller->count == state->modeIndex + 1) {
                            func_overlay_001_F0000000_184C3E0(
                                D_1D9C, controller->count, 0);
                        } else if (controller->count == state->modeIndex) {
                            func_overlay_001_F0000000_184C3E0(
                                D_1D9C, controller->count, 0);
                            state = D_1DA0;
                            if (!(state->flags & 1)) {
                                (*(s32 *)&D_0)--;
                                state->flags |= 0x11;
                            }
                            value = *(s32 *)&D_0 + 1;
                            *(s32 *)&D_0 = value;
                            if (((u8 *)&D_0)[0] != 0 && value == 1 &&
                                (value + 1) == ((u8 *)&D_0)[0]) {
                                objectCount = *(s32 *)&D_0;
                                if (objectCount != 0) {
                                    objectCursor = (Overlay1Object **)
                                        ((u8 *)objects + (objectCount - 1) * 4);
                                    do {
                                        object = *objectCursor--;
                                        object->state->flags |= 1;
                                        objectCount--;
                                    } while (objectCount != 0);
                                }
                                *(s32 *)&D_0 = 0;
                                ((Overlay1IntEventCall)
                                    func_overlay_001_F0000000_184C3E0)(
                                        0x1B, objectCount);
                                state = O1_STATE;
                            }
                            current = 0;
                            indexValue = 0;
                            state->lastMode = state->nextMode;
                            output = (f32 *)((u8 *)results +
                                state->index * 0x28);
                            *(s32 *)((u8 *)output + 8) = state->word400;
                            if (state->modeIndex > 0) {
                                do {
                                    *(s32 *)((u8 *)output + 0xC + indexValue) =
                                        *(s32 *)((u8 *)state + 0x404 + indexValue);
                                    current++;
                                    indexValue += 4;
                                } while (current < state->modeIndex);
                            }
                            if (results->count == 1) {
                                func_overlay_001_F0000000_184C3E0(
                                    current, state);
                            }
                        }
                    }
                }
            }
            state = O1_STATE;
            state->mode3b2 = (s8)((Overlay1PathRecord *)D_1D64)->flags;
        } else {
            input = D_1D9C;
            if (func_overlay_001_F0000000_184C3E0(
                    D_1D64, *(s32 *)&input->x, *(s32 *)&input->y) == 0) {
                switched = 1;
                temporaryRecord = (Overlay1PathRecord *)
                    func_overlay_001_F0000000_184C3E0(D_1D60);
                D_1D6C = D_1D68;
                D_1D68 = D_1D64;
                D_1D64 = D_1D60;
                D_1D60 = temporaryRecord;
                state = O1_STATE;
                state->path = (s16)(((u8 *)D_1D68 - (u8 *)D_1D58) / 0x94);
                if (((Overlay1PathRecord *)D_1D68)->flags == 0 &&
                    state->lastMode == 0xFF &&
                    state->changed == 0) {
                    state->changed = 1;
                    if (state->modeIndex != -1) {
                        state->modeIndex--;
                    }
                }
                state->mode3b2 = (s8)((Overlay1PathRecord *)D_1D68)->flags;
            }
        }
        if (switched) {
            state = O1_STATE;
            D_208 = (Overlay1PathPoint *)((u8 *)D_1D60 +
                state->selector * 0x10 + 0x14);
            D_20C = (Overlay1PathPoint *)((u8 *)D_1D64 +
                state->selector * 0x10 + 0x14);
            D_210 = (Overlay1PathPoint *)((u8 *)D_1D68 +
                state->selector * 0x10 + 0x14);
            D_214 = (Overlay1PathPoint *)((u8 *)D_1D6C +
                state->selector * 0x10 + 0x14);
        }
        func_overlay_001_F0000BD4_184CFB4();
        state = O1_STATE;
    }

    output = &((f32 *)D_0)[index];
    if (!(state->flags & 8)) {
        *output = (f32)(state->modeIndex * D_1D8C) + state->offset;
    } else {
        *output = D_D8;
    }
    ((u8 *)D_0)[index] = (u8)state->index;
    if (!(state->flags & 8) && *output < currentOffset) {
        value = state->timer;
        if (value >= D_1D94) {
            state->timer = (u8)(value - D_1D94);
            return;
        }
        if (!(state->flags & 1)) {
            func_overlay_001_F0000000_184C3E0(8, state);
        }
        state->timerState = 1;
        O1_STATE->timer = 0xF0;
        return;
    }
    state->timer = 0xF0;
    O1_STATE->timerState = 0;
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_middle_c1a0/func_overlay_001_F0001D78_184E158.s")
#endif
