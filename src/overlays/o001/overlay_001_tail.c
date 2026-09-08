#include "overlays/overlay_001.h"

/* ---- overlay1InitializeGaugeObjects ---- */


typedef struct O1GaugeState {
    s8 type; u8 pad001[0x1A7]; u16 flags; u8 pad1AA[0x250];
    s16 enabled; u8 pad3FC[4]; s32 value;
} O1GaugeState;
typedef struct O1GaugeObject { u8 pad00[0x64]; O1GaugeState *state; } O1GaugeObject;
typedef struct O1GaugeTableEntry { u8 pad00[8]; s32 value; u8 pad0C[0x1C]; } O1GaugeTableEntry;
extern O1GaugeTableEntry *overlay1GetGaugeTable(void);
extern O1GaugeObject **overlay1GetGaugeObjects(s32 *count);
extern s32 overlay1RandomRange(s32 minimum, s32 maximum);

/* Plateau: exact 74-word size and 0x60 frame; best is 16 words different,
 * first actionable mismatch +0x30. The second-slot count declaration matches
 * its target stack home; the first-loop count/object carrier web remains. */
#ifdef NON_MATCHING
void overlay1InitializeGaugeObjects(void) {
    O1GaugeTableEntry *table;
    volatile s32 count;
    O1GaugeObject **objects;
    O1GaugeObject **firstCursor;
    O1GaugeObject **secondCursor;
    O1GaugeObject *object;
    O1GaugeState *state;
    s32 initialIndex;
    s32 index;
    s32 loopValue;
    s32 maximum;

    table = overlay1GetGaugeTable();
    objects = overlay1GetGaugeObjects((s32 *)&count);
    maximum = 0;
    initialIndex = count - 1;
    index = initialIndex;
    if (count != 0) {
        firstCursor = (O1GaugeObject **)((u8 *)objects + (index * 4));
        do {
            object = *firstCursor--;
            state = object->state;
            loopValue = state->enabled;
            if ((loopValue != 0) && (maximum < state->value)) maximum = state->value;
            loopValue = index;
            index--;
        } while (loopValue != 0);
        index = initialIndex;
    }
    secondCursor = objects + index;
    if (count != 0) {
        do {
            state = (*secondCursor)->state;
            state->flags |= 1;
            if (table[state->type].value == 0) {
                table[state->type].value = overlay1RandomRange(100, 1000) + maximum;
            }
            loopValue = index;
            secondCursor--;
            index--;
        } while (loopValue != 0);
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F0003578_184F958.s")
#endif

/* ---- overlay1AssignRecordIndex ---- */


typedef struct O1VariableRecord { s16 type; u8 size; u8 pad03[9]; u16 index; } O1VariableRecord;
typedef struct O1RecordOwner { u8 pad00[0xC]; u16 index; } O1RecordOwner;
extern s32 D_1D8C;
extern void overlay1GetRomlistInfoReloc(O1VariableRecord **records, s32 *length,
                                        s32 enabled);

/* Fresh configured V0 was exactly 44 words but differed at 21, with frame
 * 0x30 versus target 0x38 and five of seven runtime records. A meaningful
 * D_1D8C pointer lifetime plus declaration order recovers the exact frame and
 * records/length homes at sp+0x34/sp+0x20, reducing the residual to eleven
 * words from +0x28. All 119 flag modes are nonexact. The remaining two target
 * LOCAL records and conditional-store shape require a separately materialized
 * symbolic D_1D8C lvalue; volatile, union, scalar-snapshot, operand-order, and
 * scope variants do not supply it naturally. Preserve this basin and do not
 * restore the rejected dummy, false argument, private alias, or literal write. */
#ifdef NON_MATCHING
void overlay1AssignRecordIndex(s32 unused, O1RecordOwner *owner) {
    O1VariableRecord *records;
    O1VariableRecord *record;
    s32 *recordIndex;
    s32 offset;
    s32 next;
    s32 length;
    u8 size;

    if (owner->index == 0xFFFF) {
        overlay1GetRomlistInfoReloc(&records, &length, 1);
        offset = 0;
        record = records;
        if (length > 0) {
            do {
                if (record->type == 0xCA) {
                    recordIndex = &D_1D8C;
                    next = record->index + 1;
                    if (*recordIndex < next) D_1D8C = next;
                }
                size = record->size;
                offset += size;
                record = (O1VariableRecord *)((u8 *)record + size);
            } while (offset < length);
        }
        owner->index = (u16)D_1D8C;
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F00036A0_184FA80.s")
#endif

/* ---- overlay1ChoosePath ---- */


typedef struct O1ChoiceState {
    s8 playerIndex;
    s8 relationIndex;
    u8 pad002[6];
    u16 objectValue;
    u16 scoreIndex;
    u8 pad00C[0x18E];
    u8 mode;
    u8 active;
    u8 pad19C[0x0C];
    u16 flags;
    u8 pad1AA[0x1D4];
    u8 selector;
    u8 previousSelector;
    u8 transition;
    u8 pad381[0x1B];
    f32 progress;
    u8 pad3A0[8];
    u8 relationModes[8];
} O1ChoiceState;

typedef struct O1ChoiceObject {
    u8 pad000[0x64];
    O1ChoiceState *state;
} O1ChoiceObject;

typedef struct O1ControlPoint {
    f32 x;
    f32 z;
    u8 pad008[4];
    s8 enabled;
    u8 pad00D[3];
} O1ControlPoint;

typedef struct O1ControlTable {
    u8 pad000[0x10];
    u16 flags;
    u8 pad012[2];
    O1ControlPoint points[8];
} O1ControlTable;

typedef struct O1Pair {
    f32 distance;
    f32 value;
    s32 valid;
} O1Pair;

typedef struct Overlay1ValueEntry {
    f32 value;
    u8 pad04[8];
} Overlay1ValueEntry;

typedef struct Overlay1ValueRow {
    Overlay1ValueEntry entries[6];
} Overlay1ValueRow;

extern O1ControlTable *D_1D60;
extern O1ControlTable *D_1D68;
extern O1ControlTable *D_1D68Read;
extern O1ControlTable *D_1D6C;
extern O1ControlPoint *D_208;
extern O1ControlPoint *D_20C;
extern O1ControlPoint *D_210;
extern O1ControlPoint *D_214;
extern s32 D_1D94;
extern f32 D_E8;
extern f32 D_EC;
extern f32 D_F0;
extern f32 D_F4;
extern Overlay1ValueRow D_1BA8[];

extern f32 overlay1RandomWave(s32 value);
extern O1ChoiceObject **overlay1GetChoiceObjects(s32 *count);
extern void overlay1SubmitChoice(O1ChoiceObject *object);
extern void overlay1InterpolatePath(f32 *outX, f32 *outZ, s32 path,
                                    f32 offset);
extern O1ChoiceObject *overlay1FindChoice(f32 progress,
                                         O1ControlTable *table,
                                         s32 minimum, s32 *scores);
extern f32 overlay1MeasureChoice(f32 first, f32 second);

#define CHOICE_WORLD ((O1ChoiceState *)D_1DA0)

/* Workbench plateau: 447/446 instructions, exact 0x90 frame, 341 masked/346 raw
 * differences, and first mismatch at +0xC.  Removing the redundant path carrier
 * changes only allocation and improves 19 positional words; the linked trial is
 * still 12 bytes long.  Target score and interpolation locals sit higher in the
 * same frame; the flag lattice and reverse score cursor remain eliminated. */
#ifdef NON_MATCHING
void func_overlay_001_F0003750_184FB30(f32 *outX, f32 *outZ) {
    O1ChoiceState *otherState;
    O1ChoiceState *chosenState;
    O1ChoiceObject **objects;
    O1ChoiceObject **cursor;
    O1ChoiceObject *object;
    O1ChoiceObject *found;
    O1ControlTable *table;
    s32 i;
    s32 scores[8];
    s32 selected;
    s32 loopValue;
    s32 step;
    s32 value;
    f32 weight;
    f32 difference;
    f32 temporaryX;
    f32 temporaryZ;

    if (CHOICE_WORLD->transition != 0) {
        weight = (overlay1RandomWave((CHOICE_WORLD->transition << 7) + 0x8000) +
                  1.0f) * 0.5f;
        overlay1InterpolatePath(outX, outZ, CHOICE_WORLD->previousSelector, 0.5f);
        overlay1InterpolatePath(&temporaryX, &temporaryZ, CHOICE_WORLD->selector,
                                0.5f);
        *outX = ((*outX - temporaryX) * weight) + temporaryX;
        *outZ = ((*outZ - temporaryZ) * weight) + temporaryZ;
        value = D_1D94 * 8;
        if (CHOICE_WORLD->transition >= value) {
            CHOICE_WORLD->transition -= value;
        } else {
            CHOICE_WORLD->transition = 0;
        }
        return;
    }

    if (((O1ControlTable *)D_1D64)->flags & 1) {
        table = D_1D68;
        if (table->points[CHOICE_WORLD->selector].enabled == 0) {
            i = 7;
            do {
                if (table->points[i].enabled != 0 &&
                    i != CHOICE_WORLD->selector &&
                    table->points[CHOICE_WORLD->selector].x == table->points[i].x &&
                    table->points[CHOICE_WORLD->selector].z == table->points[i].z) {
                    CHOICE_WORLD->previousSelector = i;
                    CHOICE_WORLD->selector = i;
                    CHOICE_WORLD->transition = 0;
                    break;
                }
                loopValue = i;
                i--;
            } while (loopValue != 0);
        }
        overlay1InterpolatePath(outX, outZ, CHOICE_WORLD->selector, 0.5f);
        return;
    }

    i = 7;
    do {
        difference = (f32)(i - CHOICE_WORLD->selector);
        if (difference < 0.0f) difference = -difference;
        scores[i] = (s32)(48.0f - difference * 6.0f);
        loopValue = i;
        i--;
    } while (loopValue != 0);

    objects = overlay1GetChoiceObjects(&i);
    if (i-- != 0) {
        cursor = objects + i;
        do {
            object = *cursor--;
            otherState = object->state;
            if (otherState != CHOICE_WORLD && !(otherState->flags & 8)) {
                difference =
                    ((O1Pair (*)[6])D_1BA8)[CHOICE_WORLD->playerIndex]
                                                [otherState->playerIndex].value;
                if (((difference > -2.0f) && (difference < 2.0f)) ||
                    ((D_E8 < difference) && (difference < 0.0f) &&
                     (CHOICE_WORLD->relationModes[otherState->relationIndex] == 1))) {
                    value = otherState->selector;
                    step = 1;
                    if (value < CHOICE_WORLD->selector) step = -1;
                    do {
                        if (difference > 0.0f) {
                            weight = difference;
                        } else {
                            weight = -difference;
                        }
                        scores[value] =
                            (s32)((f32)scores[value] -
                                  ((2.0f - weight) * 64.0f));
                        value += step;
                    } while (value >= 0 && value < 8);
                }
                if ((difference > -2.0f) && (difference < 0.0f) &&
                    CHOICE_WORLD->relationModes[otherState->relationIndex] >= 4) {
                    scores[otherState->selector] =
                        (s32)((f32)scores[otherState->selector] +
                              D_EC);
                }
            }
            loopValue = i;
            i--;
        } while (loopValue != 0);
    }

    table = D_1D68Read;
    i = 7;
    do {
        scores[i] += table->points[i].enabled;
        loopValue = i;
        i--;
    } while (loopValue != 0);

    i = 7;
    do {
        if (table->points[i].enabled == 0) scores[i] = -1000000;
        loopValue = i;
        i--;
    } while (loopValue != 0);

    value = -1000000;
    if (CHOICE_WORLD->active != 0 && CHOICE_WORLD->mode == 6) {
        found = overlay1FindChoice(CHOICE_WORLD->progress, table, value, scores);
        if (found != 0) {
            chosenState = found->state;
            weight = (f32)chosenState->objectValue * D_F0;
            difference = overlay1MeasureChoice(weight, CHOICE_WORLD->progress);
            if (difference < D_F4) {
                overlay1SubmitChoice(D_1D9C);
            } else if (difference < 3.0f) {
                scores[chosenState->scoreIndex] += 1000;
            }
        }
    }

    selected = -1;
    i = 0;
    do {
        if (value < scores[i]) {
            selected = i;
            value = scores[i];
            D_208 = &D_1D60->points[i];
            D_20C = &((O1ControlTable *)D_1D64)->points[i];
            D_210 = &D_1D68->points[i];
            D_214 = &D_1D6C->points[i];
        }
        i++;
    } while (i < 8);

    overlay1InterpolatePath(outX, outZ, CHOICE_WORLD->selector, 0.5f);
    if (selected != -1 && selected != CHOICE_WORLD->selector) {
        CHOICE_WORLD->previousSelector = CHOICE_WORLD->selector;
        CHOICE_WORLD->selector = selected;
        CHOICE_WORLD->transition = 0xFF;
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F0003750_184FB30.s")
#endif

#undef CHOICE_WORLD

/* ---- overlay1SubmitGlobals ---- */


/* Pinned DKR v77/v80 and JFG scans classify overlay 1 as no donor. */
extern s32 gOverlay1SubmitArg2;
extern s32 gOverlay1SubmitArg3;
extern void overlay1SubmitReloc(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void overlay1SubmitGlobals(s32 arg0, s32 arg1) {
    overlay1SubmitReloc(arg0, arg1, gOverlay1SubmitArg2, gOverlay1SubmitArg3);
}

/* ---- overlay1SubmitAll ---- */


/* Pinned DKR v77/v80 and JFG scans classify overlay 1 as no donor. */
extern s32 gOverlay1SubmitArg2;
extern s32 gOverlay1SubmitArg3;
extern s32 gOverlay1SubmitArg4;
extern s32 gOverlay1SubmitArg5;
extern void overlay1SubmitAllReloc(s32 arg0, s32 arg1, s32 arg2, s32 arg3,
                                   s32 arg4, s32 arg5);

void overlay1SubmitAll(s32 arg0, s32 arg1) {
    overlay1SubmitAllReloc(arg0, arg1, gOverlay1SubmitArg2,
                           gOverlay1SubmitArg3, gOverlay1SubmitArg4,
                           gOverlay1SubmitArg5);
}

/* ---- overlay1AngleBetweenSamples ---- */


typedef struct Overlay1SampleState {
    u8 pad0[0x37E];
    u8 selector;
} Overlay1SampleState;

/* Pinned DKR v77/v80 and JFG scans classify overlay 1 as no donor. */
extern Overlay1SampleState *gOverlay1SampleState;
extern void overlay1SampleReloc(f32 *x, f32 *y, s32 selector, f32 scale);
extern s32 overlay1SampleAngleReloc(f32 x, f32 y);

s32 overlay1AngleBetweenSamples(f32 unusedX, f32 unusedY) {
    f32 firstX;
    f32 firstY;
    f32 secondX;
    f32 secondY;

    overlay1SampleReloc(&firstX, &firstY, gOverlay1SampleState->selector, 1.0f);
    overlay1SampleReloc(&secondX, &secondY, gOverlay1SampleState->selector,
                        2.5f);
    return (s16)(overlay1SampleAngleReloc(firstX - secondX,
                                          firstY - secondY) - 0x8000);
}

/* ---- overlay1RelativeAngles ---- */


typedef struct Overlay1Position {
    u8 pad0[0xC];
    f32 x;
    u8 pad10[4];
    f32 y;
} Overlay1Position;

/* Pinned DKR v77/v80 and JFG scans classify overlay 1 as no donor. */
extern Overlay1Position *gOverlay1Position;
extern s32 overlay1AngleReloc(f32 x, f32 y);

s32 overlay1RelativeAngleA(f32 x, f32 y) {
    s32 angle;
    f32 deltaX;
    f32 deltaY;

    deltaX = x - gOverlay1Position->x;
    deltaY = y - gOverlay1Position->y;
    angle = overlay1AngleReloc(deltaX, deltaY);
    return (s16)(angle + 0x8000);
}

s32 overlay1RelativeAngleB(f32 x, f32 y) {
    s32 angle;
    f32 deltaX;
    f32 deltaY;

    deltaX = x - gOverlay1Position->x;
    deltaY = y - gOverlay1Position->y;
    angle = overlay1AngleReloc(deltaX, deltaY);
    return (s16)(angle + 0x8000);
}

/* ---- overlay1TransitionState ---- */


typedef struct Transform {
    s16 rotY;
    s16 rotX;
    s16 rotZ;
    u8 pad06[6];
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[0x21];
    u8 alpha;
    u8 pad3A[0xE];
    struct ObjectHeader *header;
} Transform;

typedef struct ObjectHeader {
    u8 pad00[6];
    u16 flags;
} ObjectHeader;

typedef struct Spawned {
    s16 angle;
    u8 pad02[2];
    f32 y;
    u8 pad08[4];
    f32 x;
    union {
        f32 y2;
        u16 flags;
    } at10;
    f32 z;
    u8 pad18[0x2C];
    f32 x2;
    f32 z2;
} Spawned;

typedef struct State {
    u8 pad000[0x16C];
    u8 done;
    u8 pad16D[3];
    u8 phase;
    u8 pad171[0x1F];
    u8 fade;
    u8 active;
    u8 pad192[0x16];
    u16 flags;
    u8 pad1AA[0x1D2];
    s16 pathId;
    u8 selectorA;
    u8 selectorB;
    u8 selectorC;
    u8 pad381[0x5F];
    Spawned *spawned;
} State;

extern s32 G_o1_83e4;
extern void ext_o7_ccc(Transform *, s32);
/* Runtime table-2 record 548 at module +0x4018 makes this the sole inbound to
 * Overlay 1 ORT 1485, +0x378 (overlay1FindType5ByKey). The local State-pointer
 * argument and Spawned-pointer result are an inconsistent caller-side typed
 * view; preserve them until a coordinated ABI reconstruction retains codegen. */
extern Spawned *local_378(State *);
extern void ext_o0_1bed0(Transform *, f32, f32, f32, s16, s16, s16);
extern void ext_o0_1c6bc(Transform *, State *);
extern void ext_o0_5a914(Transform *, s32, s32, s32);
extern Spawned *local_414(s16, Spawned **);
extern s16 local_c0(Spawned *);

/* PLATEAU (2026-09-04): configured full-TU C is 236/237 words with 160 raw
 * and relocation-masked differences from +0x20; both frames are 0x50. An
 * asymmetric selector assignment restored the narrow index mask/shift. Phase
 * carrier, register-hint, and pointer-advance forms were flat or regressed;
 * folding the existing point update into its load is also byte-flat. The
 * promotion trial's in=0/out=0 is a build-error classification caused by
 * schedule-divergent global sites, not linked byte equality. */
#ifdef NON_MATCHING
void overlay1TransitionState(Transform *obj, State *state, s32 updateRate) {
    Spawned *sp3C;
    s32 value;
    u8 phase;
    s32 phaseValue;
    u8 index;
    u8 *point;
    Spawned *spawned;

    if (G_o1_83e4 == 3) {
        phase = state->phase;
        if (phase == 0) {
            return;
        }
        if (phase == 1) {
                ext_o7_ccc(obj, 0x13);
                state->spawned = local_378(state);
                state->phase = 2;
                return;
            }
            phaseValue = phase;
            if (phaseValue == 2) {
                value = state->fade - (updateRate * 4);
                if (value <= 0) {
                    state->phase = 3;
                    return;
                }
                state->fade = value;
                return;
            }
            if (phaseValue == 3) {
                spawned = state->spawned;
                obj->x = spawned->x;
                obj->y = spawned->at10.y2 + 100.0f;
                obj->z = spawned->z;
                obj->rotX = 0;
                obj->rotZ = 0;
                obj->rotY = spawned->angle;
                ext_o0_1bed0(obj, obj->x, obj->y, obj->z, obj->rotY, obj->rotX, obj->rotZ);
                ext_o0_1c6bc(obj, state);
                state->flags &= ~8;
                obj->header->flags &= ~1;
                state->fade = 0;
                state->active = 1;
                state->phase = 4;
                return;
            }
            if (phaseValue == 4) {
                value = state->fade + (updateRate * 4);
                if (value >= 255) {
                    state->fade = 255;
                    state->phase = 5;
                    return;
                }
                state->fade = value;
                return;
            }
            if (phaseValue == 5) {
                obj->header->flags |= 1;
                state->phase = 0;
                state->active = 0;
                state->done = 1;
                ext_o0_5a914(obj, 12, -1, 0);
                state->spawned = 0;
        }
    } else {
        phase = state->phase;
        if (phase == 0) {
            return;
        }
        if (phase == 1) {
                ext_o7_ccc(obj, 0x13);
                state->spawned = local_414(state->pathId, &sp3C);
                state->pathId = local_c0(sp3C);
                state->spawned->at10.flags |= 8;
                state->phase = 2;
                return;
            }
            phaseValue = phase;
            if (phaseValue == 2) {
                value = obj->alpha - (updateRate * 4);
                if (value <= 0) {
                    state->phase = 3;
                    return;
                }
                obj->alpha = value;
                return;
            }
            if (phaseValue == 3) {
                spawned = state->spawned;
                index = state->selectorA = 3;
                state->selectorB = 3;
                state->selectorC = 0;
                point = (u8 *)spawned + (index << 4);
                obj->x = *(f32 *)(point + 0x14);
                obj->y = spawned->y + 100.0f;
                point += 0x14;
                obj->z = *(f32 *)(point + 4);
                obj->rotX = 0;
                obj->rotZ = 0;
                obj->rotY = *(s16 *)((u8 *)spawned + 0xC) + 0x4000;
                ext_o0_1bed0(obj, obj->x, obj->y, obj->z, obj->rotY, obj->rotX, obj->rotZ);
                ext_o0_1c6bc(obj, state);
                state->flags &= ~8;
                obj->header->flags &= ~1;
                obj->alpha = 0;
                state->active = 1;
                state->phase = 4;
                return;
            }
            if (phaseValue == 4) {
                value = obj->alpha + (updateRate * 4);
                if (value >= 255) {
                    obj->alpha = 255;
                    state->phase = 5;
                    return;
                }
                obj->alpha = value;
                return;
            }
            if (phaseValue == 5) {
                obj->header->flags |= 1;
                state->phase = 0;
                state->active = 0;
                state->done = 1;
                ext_o0_5a914(obj, 12, -1, 0);
                state->spawned->at10.flags &= 0xFFF7;
                state->spawned = 0;
        }
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F0003FD8_18503B8.s")
#endif

/* ---- overlay1UpdateObjectPhysics ---- */

#undef NULL
#define NULL 0

typedef struct O1PhysicsObject O1PhysicsObject;
typedef struct O1PhysicsState O1PhysicsState;

/* Partial layouts established by Mickey's field accesses. */
struct O1PhysicsObject {
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    u8 pad006[0x6];
    f32 x;
    f32 y;
    f32 z;
    u8 pad018[0x4];
    f32 velocityX;
    f32 velocityY;
    f32 velocityZ;
    f32 animationProgress;
    u8 pad02C[0x2];
    s16 positionTag;
    u8 pad030[0xB];
    s8 mode;
    u8 pad03C[0x28];
    O1PhysicsState * state;
    u8 pad068[0x18];
    s32 flags80;
};
struct O1PhysicsState {
    u8 pad000[0x2];
    u8 inSurface;
    u8 surfaceKind;
    f32 forwardVelocity;
    f32 sideVelocity;
    f32 speedLimit;
    u8 pad010[0x4];
    f32 field14;
    u8 pad018[0x20];
    f32 previousX;
    f32 previousY;
    f32 previousZ;
    u8 pad044[0x18];
    f32 slope;
    f32 normalX;
    f32 normalY;
    f32 surfaceHeight;
    f32 submergedHeight;
    f32 outputScale;
    f32 impulseX;
    f32 impulseY;
    f32 impulseZ;
    f32 impulseInitial;
    f32 impulseVelocity;
    f32 impulseAcceleration;
    u8 pad08C[0x8];
    f32 actualVelocityX;
    f32 actualVelocityY;
    f32 actualVelocityZ;
    u8 pad0A0[0x8];
    void * soundA8;
    void * soundAC;
    u8 pad0B0[0x24];
    O1PhysicsObject * linkedObject;
    u8 pad0D8[0x18];
    s16 heading;
    u8 pad0F2[0xE];
    s16 field100;
    s16 spinTimer;
    s16 spinAngle;
    u8 pad106[0x2];
    s16 steeringAngle;
    u8 pad10A[0x4E];
    s16 field158;
    u8 pad15A[0xC];
    s16 field166;
    u8 pad168[0x2];
    s16 field16A;
    u8 field16C;
    u8 pad16D[0x3];
    u8 reset170;
    u8 reverseTimer;
    u8 pad172[0xF];
    u8 impulseActive;
    u8 pad182[0x1];
    s8 field183;
    u8 pad184[0x1];
    u8 boostMode;
    u8 pad186[0x1];
    s8 field187;
    f32 boostScale;
    u8 pad18C[0x1];
    s8 disabled18D;
    u8 pad18E[0x4];
    u8 level192;
    u8 pad193[0x15];
    u16 flags1A8;
    u8 pad1AA[0x112];
    s32 collisionMode;
    u8 pad2C0[0x89];
    u8 field349;
    u8 pad34A[0x32];
    s16 pathIndex;
    u8 pad37E[0x3];
    u8 pathMode;
    u8 actionMode;
    u8 pad383[0x15];
    f32 progress398;
    u8 pad39C[0x4];
    f32 speedScale;
    u8 pad3A4[0x18];
    f32 targetX;
    f32 targetZ;
    u8 pad3C4[0x8];
    s16 previousPathIndex;
    s16 stuckTimer;
    f32 pathStartX;
    f32 pathStartZ;
    f32 pathEndX;
    f32 pathEndZ;
    u8 pad3E0[0x1A];
    s16 field3FA;
    u8 pad3FC[0x20];
    s32 controlKeys;
    s32 controlDkeys;
    u8 pad424[0x4];
    s32 controlXjoy;
    s32 controlYjoy;
    u8 pad430[0x8];
    s32 joypadDisabled;
};

typedef struct O1PhysicsSurface {
    f32 height;
    u32 flags;
} O1PhysicsSurface;

typedef struct O1PhysicsPathMode {
    s32 (*test)(void);
    void (*position)(f32 *x, f32 *z);
    s16 (*heading)(f32 x, f32 z);
    u16 mask;
    u16 unused;
} O1PhysicsPathMode;

typedef struct O1PhysicsActionMode {
    void (*test)(void);
    void (*update)(void);
    u16 mask;
    u16 unused;
} O1PhysicsActionMode;

extern O1PhysicsPathMode gO1PhysicsPaths[4];
extern O1PhysicsActionMode gO1PhysicsActions[6];

/* Call identities come from the shipped overlay relocation table. */
s32 func_overlay_001_F00004B4_184C894(void *object);
s32 func_overlay_001_F0007D6C_185414C(s16, s16, s16, s16,
                                     s16 *, s16 *, void *);
f32 *ext_o8_8(void *state);
void ext_o0_1ee0c(void *state, s32 disabled);
void ext_o8_49dc(s32 value);
void ext_o0_1d4c0(void *object, void *state);
void ext_o0_29adc(s16 *angles, f32 *vector);
s32 ext_o0_1312c(f32 x, f32 z, f32 *height, u32 flags, void *surfaces);
void ext_o0_1ecfc(void *object, void *state);
s32 ext_o2_123c(f32 x, f32 z, void *region);
s16 ext_o0_2a4c0(f32 x, f32 z);
s32 ext_o0_2a5bc(s16 first, s16 second);
void ext_o7_edc(void);
f32 ext_o8_1000(void *object, void *state, f32 limit);
s32 ext_o0_2630c(void);
s32 ext_o0_29598(s32 minimum, s32 maximum);
f32 ext_o0_2a46c(s16 angle);
void ext_o0_2d98(void *handle);
void ext_o0_2b90(s32 id, f32 x, f32 y, f32 z, s32 priority, void **handle);
void ext_o0_2d70(void *handle, f32 x, f32 y, f32 z);
f32 ext_o0_2a428(f32 base, s32 exponent);
f32 ext_o0_2a470(s16 angle);
s32 ext_o0_1e174(void *object, void *state, f32 update);
s32 ext_o0_1d920(void *object, void *state, f32 update);
s32 ext_o0_7cd8(void *object, f32 x, f32 y, f32 z);
void ext_o8_49a4(void *state);
f32 ext_o8_34a0(void *object, void *state, f32 limit, f32 update);
void ext_o8_49b4(void *unused);
void ext_o0_1cfcc(void *object, void *state, s32 update);
void ext_o8_3278(void *object, void *state, s32 update);
void ext_o0_1d510(void *object, void *state, s32 mode, s32 kind, s32 update);
void ext_o8_2ec0(void *object, void *state, s32 update);
void ext_o8_3018(void *object, void *state, f32 scale, s32 update);
void ext_o0_3e99c(void *object, s32 update);
extern f32 D_4;
extern s32 G_o1_83e4;
extern f32 G_rt_458c4;
extern s32 G_rt_43a3c;
extern u8 G_offd_31a4;
extern void *D_1BA4;
extern f32 LOCAL_DATA_4;
extern s32 LOCAL_BSS_1D78;
extern s32 LOCAL_BSS_1D94;
extern void *LOCAL_BSS_1BA4;
extern void *LOCAL_BSS_1D9C;
extern f32 LOCAL_RODATA_F8;
extern f32 LOCAL_RODATA_FC;
extern f32 LOCAL_RODATA_100;
extern f32 LOCAL_RODATA_104;
extern f32 LOCAL_RODATA_108;
extern f32 LOCAL_RODATA_10C;
extern f32 LOCAL_RODATA_110;
extern f32 LOCAL_RODATA_114;
extern f32 LOCAL_RODATA_118;
extern f32 LOCAL_RODATA_11C;
extern f32 LOCAL_RODATA_120;
extern f32 LOCAL_RODATA_124;
extern f32 LOCAL_RODATA_128;
extern f32 LOCAL_RODATA_12C;
extern f32 LOCAL_RODATA_130;
extern f32 LOCAL_RODATA_134;
extern f32 LOCAL_RODATA_138;
extern f32 LOCAL_RODATA_13C;
extern f32 LOCAL_RODATA_140;
extern f32 LOCAL_RODATA_144;
extern f32 LOCAL_RODATA_148;
extern f32 LOCAL_RODATA_14C;
extern f32 LOCAL_RODATA_150;
extern f32 LOCAL_RODATA_154;
extern f32 LOCAL_RODATA_158;
extern f32 LOCAL_RODATA_15C;

/* Typed reconstruction remains NON_MATCHING. The object/state and callback
 * layouts follow Mickey's runtime identities and access widths. Local
 * lifetime cleanup reduces the frame; remaining CFG and allocation work
 * is measured separately in the function handoff. */
#ifdef NON_MATCHING
void func_overlay_001_F000438C_185076C(O1PhysicsObject *object, s32 updateRate) {
    f32 *tuning;
    O1PhysicsSurface surfaces[8];
    f32 normal[3];
    s16 angles[3];
    f32 targetX;
    f32 targetZ;
    s32 remaining;
    f32 limit;
    f32 velocityX;
    f32 inverseUpdate;
    f32 deltaY;
    f32 impulseX;
    f32 impulseY;
    f32 impulseZ;
    f32 scale;
    s16 heading;
    s16 targetHeading;
    s16 pathHeading;
    f32 speed;
    s16 resolvedX;
    s16 resolvedZ;
    void (*actionTest)(void);
    void (*actionUpdate)(void);
    O1PhysicsPathMode *path;
    O1PhysicsActionMode *action;
    f32 value;
    f32 travelZ;
    f32 work;
    f32 deltaX;
    f32 verticalVelocity;
    f32 negativeSpeedLimit;
    f32 negativeLimit;
    f32 deltaZ;
    f32 cosine;
    f32 value2;
    f32 inverse;
    f32 ceiling;
    f32 acceleration;
    f32 fraction;
    f32 velocityZ;
    f32 angleMagnitude;
    void *sound;
    void *region;
    O1PhysicsState *state;
    s16 clampedAngle;
    s32 (*pathTest)(void);
    f32 *sample;
    s32 surfaceCount;
    s32 collision;
    s32 angleOffset;
    s32 mode;
    s32 keys;
    s32 surfaceIndex;
    s32 applySlope;
    s32 pathIndex;
    s32 sampleIndex;
    s32 steering;
    s32 actionIndex;
    u8 level;
    O1PhysicsObject *cachedObject;
    O1PhysicsObject *linkedObject;
    O1PhysicsSurface *surface;

    state = object->state;
    if (func_overlay_001_F00004B4_184C894(object) != 0) {
        tuning = ext_o8_8(state);
        mode = G_o1_83e4;
        G_rt_458c4 = *tuning;
        if (mode == 1) {
            if ((G_rt_43a3c == 0) && (state->joypadDisabled == 0) && (state->spinTimer == 0) && !(state->flags1A8 & 8)) {
                if (state->pathIndex != state->previousPathIndex) {
                    state->previousPathIndex = state->pathIndex;
                    state->stuckTimer = 0;
                } else {
                    state->stuckTimer = (s16) (state->stuckTimer + updateRate);
                }
                if ((f32) state->stuckTimer > 360.0f) {
                    if (state->reset170 == 0) {
                        state->reset170 = 1U;
                    }
                    goto block_13;
                }
            } else {
block_13:
                state->stuckTimer = 0;
            }
        }
        state->outputScale = 0.0f;
        if ((state->disabled18D != 0) || (state->field158 != 0) || (state->reset170 != 0) || (state->field3FA != 0)) {
            ext_o0_1ee0c(state, 1);
        }
        D_1D94 = updateRate;
        D_4 = (f32) updateRate;
        speed = -state->forwardVelocity;
        ext_o8_49dc(NULL);
        value = LOCAL_RODATA_F8;
        object->flags80 = 0;
        state->controlXjoy = 0;
        state->controlYjoy = 0;
        state->controlKeys = 0;
        state->controlDkeys = 0;
        if (state->forwardVelocity < value) {
            state->forwardVelocity = value;
        }
        work = LOCAL_RODATA_FC;
        if (work < state->forwardVelocity) {
            state->forwardVelocity = work;
        }
        if (state->sideVelocity < value) {
            state->sideVelocity = value;
        }
        if (work < state->sideVelocity) {
            state->sideVelocity = work;
        }
        ext_o0_1d4c0(object, state);
        angles[0] = -state->heading;
        angles[1] = -object->rotationY;
        normal[2] = 0.0f;
        normal[0] = 0.0f;
        angles[2] = -object->rotationZ;
        normal[1] = -1.0f;
        ext_o0_29adc(angles, normal);
        state->normalX = normal[0];
        state->normalY = normal[1];
        state->slope = normal[2];
        surfaceCount = ext_o0_1312c(object->x, object->z, NULL, 0x08010000, surfaces);
        ceiling = -32768.0f;
        surfaceIndex = surfaceCount - 1;
        state->surfaceHeight = -32768.0f;
        if (surfaceCount != NULL) {
            surface = &surfaces[surfaceIndex];
            do {
                if (surface->flags & 0x10000) {
                    state->surfaceHeight = (f32) surface->height;
                }
                if (surface->flags & 0x08000000) {
                    ceiling = surface->height;
                }
                surface -= 1;
                surfaceIndex -= 1;
            } while (surfaceIndex != 0);
        }
        value2 = state->surfaceHeight;
        if (object->y < value2) {
            state->inSurface = 1U;
            state->submergedHeight = value2;
        } else {
            state->inSurface = 0U;
            state->submergedHeight = 0.0f;
        }
        if (object->y < ceiling) {
            if (state->reset170 == 0) {
                state->reset170 = 1U;
            }
            if ((state->inSurface != 0) && (state->surfaceKind != 1)) {
                ext_o0_1ecfc(object, state);
            }
        }
        level = state->level192;
        if ((s32) level >= 0xB) {
            level = 0xA;
        }
        path = &gO1PhysicsPaths[1];
        pathIndex = 1;
        limit = (tuning[16] + ((f32) level * tuning[2])) * state->speedScale;
        do {
            pathTest = path->test;
            if ((pathTest != NULL) && (path->mask & (1 << state->pathMode))) {

                if (pathTest() != 0) {
                    state->pathMode = (u8) pathIndex;
                }
            }
            pathIndex += 1;
            path++;
        } while (pathIndex != 4);
        gO1PhysicsPaths[state->pathMode].position(&targetX, &targetZ);
        region = D_1BA4;
        if (region != NULL) {
            cachedObject = D_1D9C;
            if (ext_o2_123c(cachedObject->x, cachedObject->z, region) != NULL) {
                cachedObject = D_1D9C;
                ((O1PhysicsState *)D_1DA0)->pathStartX = cachedObject->x;
                ((O1PhysicsState *)D_1DA0)->pathStartZ = cachedObject->z;
            }
            if (ext_o2_123c(targetX, targetZ, D_1BA4) != NULL) {
                ((O1PhysicsState *)D_1DA0)->pathEndX = targetX;
                ((O1PhysicsState *)D_1DA0)->pathEndZ = targetZ;
            }
            func_overlay_001_F0007D6C_185414C(
                (s16)(s32)((O1PhysicsState *)D_1DA0)->pathStartX,
                (s16)(s32)((O1PhysicsState *)D_1DA0)->pathStartZ,
                (s16)(s32) ((O1PhysicsState *)D_1DA0)->pathEndX, (s16)(s32) ((O1PhysicsState *)D_1DA0)->pathEndZ,
                &resolvedX, &resolvedZ, D_1D9C);
            targetX = (f32) resolvedX;
            targetZ = (f32) resolvedZ;
        }
        targetHeading = ext_o0_2a4c0(targetX - object->x, targetZ - object->z) + 0x8000;
        pathHeading = gO1PhysicsPaths[state->pathMode].heading(targetX, targetZ);
        state->targetX = targetX;
        state->targetZ = targetZ;
        value = (f32) ext_o0_2a5bc(object->rotationX, targetHeading) * LOCAL_RODATA_100;
        angleMagnitude = value;
        state->controlXjoy = (s32) (value * LOCAL_RODATA_104);
        if (value < 0.0f) {
            angleMagnitude = -value;
        }
        if (G_o1_83e4 == 1) {
            if (angleMagnitude > 24576.0f) {
                clampedAngle = 0x6000;
            } else {
                clampedAngle = (s16) (s32) angleMagnitude;
            }
            angleOffset = (s32) (angleMagnitude - 8192.0f);
            if (speed < (25.0f - ((f32) clampedAngle * 0.0009765625f))) {
                state->controlKeys = (s32) (state->controlKeys | 0x8000);
            }
            clampedAngle = (s16) angleOffset;
            if ((s16) angleOffset < 0) {
                clampedAngle = (s16) angleOffset * -1;
            }
            if (((f32) clampedAngle * LOCAL_RODATA_108) < speed) {
                state->controlKeys = (s32) (state->controlKeys | 0x4000);
            }
            value = (f32) ext_o0_2a5bc(targetHeading, pathHeading);
            angleMagnitude = value;
            if (value < 0.0f) {
                angleMagnitude = -value;
            }
            if ((angleMagnitude * LOCAL_RODATA_10C) < speed) {
                state->controlKeys = (s32) (state->controlKeys | 0x4000);
            }
        } else {
            if (angleMagnitude > 16384.0f) {
                clampedAngle = 0x4000;
            } else {
                clampedAngle = (s16) (s32) angleMagnitude;
            }
            angleOffset = (s32) (angleMagnitude - 16384.0f);
            if (speed < (25.0f - ((f32) clampedAngle * 0.0014648438f))) {
                state->controlKeys = (s32) (state->controlKeys | 0x8000);
            }
            clampedAngle = (s16) angleOffset;
            if ((s16) angleOffset < 0) {
                clampedAngle = (s16) angleOffset * -1;
            }
            if (((f32) clampedAngle * 0.006713867f) < speed) {
                state->controlKeys = (s32) (state->controlKeys | 0x4000);
            }
            if (speed < LOCAL_RODATA_110) {
                state->controlKeys = (s32) (state->controlKeys | 0x8000);
            }
            if (state->reverseTimer != 0) {
                if (updateRate < (s32) state->reverseTimer) {
                    state->reverseTimer = (u8) (state->reverseTimer - updateRate);
                } else {
                    state->reverseTimer = 0U;
                }
                state->controlYjoy = -0x64;
                state->controlKeys = 0x4000;
            }
        }
        if (G_rt_43a3c != 0) {
            state->controlKeys = 0x4000;
            object->velocityX = 0.0f;
            object->velocityY = 0.0f;
            object->velocityZ = 0.0f;
            state->forwardVelocity = 0.0f;
        }
        if ((LOCAL_BSS_1D78 != 0) && (G_rt_43a3c == 0)) {
            state->field183 = 1;
            state->boostMode = 2U;
            state->field187 = 6;
            state->boostScale = 1.0f;
        }
        if (G_rt_43a3c != 0) {
            ext_o7_edc();
        }
        if (state->joypadDisabled != 0) {
            state->controlXjoy = 0;
            state->controlYjoy = 0;
            state->controlKeys = 0;
            state->controlDkeys = 0;
        }
        linkedObject = state->linkedObject;
        if (linkedObject != NULL) {
            limit *= 1.0f + (LOCAL_RODATA_114 * linkedObject->state->field14);
        }
        if (state->boostMode == 0) {
            value = state->slope;
            if (value != 0.0f) {
                scale = 1.0f - (value * 0.5f * tuning[3]);
                if (scale < LOCAL_RODATA_118) {
                    scale = LOCAL_RODATA_11C;
                }
                limit *= scale;
            }
        }
        if ((state->spinTimer != 0) && ((object->mode == 0x10) || (object->mode == 0xF))) {
            scale = object->animationProgress * 1.5f;
            if (scale > 1.0f) {
                scale = 1.0f;
            }
            if (state->spinTimer > 0) {
                scale = -scale;
            }
            state->spinAngle = (s16) (s32) (65536.0f * scale);
            if (object->animationProgress == 1.0f) {
                state->spinTimer = 0;
                state->spinAngle = 0;
            } else {
                state->boostMode = 0U;
                state->boostScale = 0.0f;
            }
        }
        if (D_1D94 != 0) {
            remaining = D_1D94 - 1;
            do {
                value = ext_o8_1000(object, state, limit);
                limit = value;
                keys = state->controlKeys;
                if (((keys & 0x4000) == 0) && (state->slope > 0.0f) && (state->forwardVelocity < -value)) {
                    applySlope = 1;
                } else if (((keys & 0x4000) == 0) && (state->slope < 0.0f)) {
                    applySlope = 1;
                } else {
                    applySlope = 0;
                    if (!(keys & 0xC000) && (state->slope > 0.0f)) {
                        applySlope = 1;
                    }
                }
                if ((applySlope != 0) && (state->boostMode == 0)) {
                    state->forwardVelocity = (f32) (state->forwardVelocity + (G_rt_458c4 * state->slope));
                    value2 = tuning[7];
                    if (value2 < state->forwardVelocity) {
                        state->forwardVelocity = value2;
                    }
                }
                if (state->spinTimer != 0) {
                    value2 = LOCAL_RODATA_120;
                    state->controlXjoy = 0;
                    state->controlYjoy = 0;
                    state->controlKeys = 0;
                    state->controlDkeys = 0;
                    state->forwardVelocity = (f32) (state->forwardVelocity * value2);
                    state->sideVelocity = (f32) (state->sideVelocity * value2);
                }
                keys = state->controlKeys;
                state->field100 = 0;
                if (keys & 0x4000) {
                    if (state->forwardVelocity < -5.0f) {
                        ext_o8_49dc(1);
                    }
                    if (state->forwardVelocity < 0.0f) {
                        state->forwardVelocity = (f32) (state->forwardVelocity + tuning[(s32) -state->forwardVelocity + 50]);
                        if ((state->forwardVelocity > 0.0f) && (state->controlYjoy >= -0x1E)) {
                            state->forwardVelocity = 0.0f;
                        }
                    } else if (state->controlYjoy < -0x1E) {
                        state->forwardVelocity = (f32) (state->forwardVelocity + tuning[(s32) state->forwardVelocity + 8]);
                        if (state->forwardVelocity > 6.0f) {
                            state->forwardVelocity = 6.0f;
                        }
                    } else {
                        state->forwardVelocity = (f32) (state->forwardVelocity - tuning[50]);
                        if (state->forwardVelocity <= 0.0f) {
                            state->forwardVelocity = 0.0f;
                        }
                    }
                } else if (keys & 0x8000) {
                    negativeLimit = -value;
                    if ((state->boostMode == 1) || (state->boostMode == 2)) {
                        if (G_offd_31a4 == 0) {
                            acceleration = LOCAL_RODATA_124;
                        } else {
                            acceleration = 0.5f;
                        }
                    } else {
                        value2 = state->forwardVelocity;
                        if (value2 > 0.0f) {
                            sampleIndex = (s32) value2;
                            fraction = value2 - (f32) sampleIndex;
                        } else {
                            work = -value2;
                            sampleIndex = (s32) work;
                            fraction = work - (f32) sampleIndex;
                        }
                        sample = &tuning[sampleIndex];
                        work = sample[17];
                        acceleration = ((sample[18] - work) * fraction) + work;
                    }
                    if (state->forwardVelocity < negativeLimit) {
                        state->forwardVelocity = (f32) (state->forwardVelocity * LOCAL_RODATA_128);
                        if (negativeLimit < state->forwardVelocity) {
                            goto block_160;
                        }
                    } else {
                        state->forwardVelocity = (f32) (state->forwardVelocity - (acceleration * state->speedScale));
                        if (state->forwardVelocity < negativeLimit) {
block_160:
                            state->forwardVelocity = negativeLimit;
                        }
                    }
                    if ((G_o1_83e4 == 1) && (G_offd_31a4 == 0) && (state->flags1A8 & 1) && (ext_o0_2630c() == (s32)0x21) && (state->pathIndex == 0x2A) && (LOCAL_RODATA_12C < state->progress398)) {
                        state->forwardVelocity = (f32) (state->forwardVelocity - 2.0f);
                    }
                } else {
                    value2 = state->forwardVelocity;
                    if ((LOCAL_RODATA_130 < value2) && (value2 < LOCAL_RODATA_134)) {
                        state->forwardVelocity = 0.0f;
                    } else {
                        state->forwardVelocity = (f32) (value2 * LOCAL_RODATA_138);
                    }
                }
                if ((LOCAL_RODATA_13C < state->forwardVelocity) && (state->forwardVelocity < LOCAL_RODATA_140)) {
                    ext_o0_29598(0, 127);
                }
                if (state->controlXjoy >= 0x42) {
                    steering = -0x1F4;
                } else if (state->controlXjoy < -0x41) {
                    steering = 0x1F4;
                } else {
                    steering = (s32) (state->controlXjoy * -0x1F4) / 65;
                }
                work = -2.0f - state->forwardVelocity;
                state->steeringAngle = (s16) (state->steeringAngle + ((s32) (steering - state->steeringAngle) >> 1));
                if (work > 0.0f) {
                    scale = (ext_o0_2a46c((s16) (s32) (work * LOCAL_RODATA_144)) * 0.25f) + 0.75f;
                } else {
                    if (work < 0.0f) {
                        work = -work;
                    }
                    if (work > 2.0f) {
                        work = 2.0f;
                    }
                    scale = (ext_o0_2a46c((s16) (s32) (work * 16384.0f)) + 1.0f) * 0.5f;
                }
                if (state->forwardVelocity > 0.0f) {
                    scale = -scale;
                }
                state->heading = (s16) (s32) ((f32) state->heading + ((f32) state->steeringAngle * scale));
                state->sideVelocity = (f32) (state->sideVelocity * LOCAL_RODATA_148);
                value2 = state->sideVelocity;
                if ((LOCAL_RODATA_14C < value2) && (value2 < LOCAL_RODATA_150)) {
                    state->sideVelocity = 0.0f;
                }
                remaining -= 1;
            } while (remaining != 0);
        }
        if (state->controlDkeys & 0x2000) {
            sound = state->soundA8;
            if (sound != NULL) {
                ext_o0_2d98(sound);
            }
            ext_o0_2b90(4, object->x, object->y, object->z, 4, &state->soundA8);
        }
        if ((state->controlKeys & 0x4000) && (state->forwardVelocity < 0.0f)) {
            sound = state->soundAC;
            if (sound == NULL) {
                ext_o0_2b90(3, object->x, object->y, object->z, 1, &state->soundAC);
            } else {
                ext_o0_2d70(sound, object->x, object->y, object->z);
            }
        }
        sound = state->soundAC;
        if (sound != NULL) {
            ext_o0_2d98(sound);
        }
        object->rotationX = (s16) (state->heading + state->spinAngle);
        heading = state->heading;
        if (state->joypadDisabled == 1) {
            value = ext_o0_2a428(LOCAL_RODATA_154, D_1D94);
            value2 = state->forwardVelocity;
            if ((value2 < -0.5f) || (value2 > 0.5f)) {
                state->forwardVelocity = (f32) (value2 * value);
            } else {
                state->forwardVelocity = 0.0f;
            }
            value2 = state->sideVelocity;
            if ((value2 < -0.5f) || (value2 > 0.5f)) {
                state->sideVelocity = (f32) (value2 * value);
            } else {
                state->sideVelocity = 0.0f;
            }
        }
        if (state->impulseActive != 0) {
            value = (state->impulseVelocity * D_4) + (0.5f * state->impulseAcceleration * D_4 * D_4);
            impulseX = state->impulseX * value;
            impulseY = state->impulseY * value;
            impulseZ = state->impulseZ * value;
            if (value < 0.0f) {
                state->impulseVelocity = 0.0f;
                state->impulseAcceleration = 0.0f;
                state->impulseActive = 0U;
                if (!(state->controlKeys & 0x8000)) {
                    state->forwardVelocity = 0.0f;
                    state->sideVelocity = 0.0f;
                }
            }
            value = state->impulseVelocity;
            scale = 1.0f - (value / state->impulseInitial);
            state->impulseVelocity = (f32) (value + (state->impulseAcceleration * D_4));
            velocityX = ext_o0_2a470(heading) * state->forwardVelocity * scale;
            velocityZ = ext_o0_2a46c(heading) * state->forwardVelocity * scale;
        } else {
            impulseX = 0.0f;
            impulseY = 0.0f;
            impulseZ = 0.0f;
            velocityX = ext_o0_2a470(heading) * state->forwardVelocity;
            velocityZ = ext_o0_2a46c(heading) * state->forwardVelocity;
        }

        velocityX += state->sideVelocity * ext_o0_2a46c(heading);
        cosine = ext_o0_2a470(heading);
        verticalVelocity = object->velocityY;
        deltaX = (velocityX * D_4) + impulseX;
        travelZ = (velocityZ - (state->sideVelocity * cosine)) * D_4;
        deltaY = ((verticalVelocity * D_4) - (0.5f * G_rt_458c4 * D_4 * D_4)) + impulseY;
        inverse = 1.0f / D_4;
        deltaZ = travelZ + impulseZ;
        inverseUpdate = inverse;
        object->velocityX = (f32) (deltaX * inverse);
        object->velocityY = (f32) (verticalVelocity - (G_rt_458c4 * D_4));
        object->x += deltaX;
        object->velocityZ = (f32) (deltaZ * inverseUpdate);
        object->y = (f32) (object->y + deltaY);
        object->z += deltaZ;
        if (state->collisionMode == 1) {
            collision = ext_o0_1e174(object, state, D_4);
        } else {
            collision = ext_o0_1d920(object, state, D_4);
        }

        if ((ext_o0_7cd8(object, 0.0f, 0.0f, 0.0f) != NULL) || (object->positionTag == -1)) {
            if (state->reset170 == 0) {
                state->reset170 = 1U;
            }
            object->x = state->previousX;
            object->y = (f32) state->previousY;
            object->z = state->previousZ;

            ext_o0_7cd8(object, 0.0f, 0.0f, 0.0f);
        }
        if (state->field166 != 0) {
            if (G_o1_83e4 == 3) {
                state->reverseTimer = 0x78U;
                state->field166 = 0;
            } else if (state->reset170 == 0) {
                state->reset170 = 1U;
            }
        }
        state->actualVelocityX = (object->x - state->previousX) * inverseUpdate;
        state->actualVelocityY = (f32) ((object->y - state->previousY) * inverseUpdate);
        state->actualVelocityZ = (object->z - state->previousZ) * inverseUpdate;
        if ((state->field16A == 0) && (collision != NULL)) {
            work = state->speedLimit;
            state->speedLimit = (f32) (work + ((3.0f - work) * (1.0f - ext_o0_2a428(LOCAL_RODATA_158, D_1D94))));
            work = state->speedLimit;
            negativeSpeedLimit = -work;
            if (state->forwardVelocity < negativeSpeedLimit) {
                state->forwardVelocity = negativeSpeedLimit;
            }
            if (work < state->forwardVelocity) {
                state->forwardVelocity = work;
            }
            if (state->sideVelocity < negativeSpeedLimit) {
                state->sideVelocity = negativeSpeedLimit;
            }
            if (work < state->sideVelocity) {
                state->sideVelocity = work;
            }
        } else {
            work = state->speedLimit;
            state->speedLimit = (f32) (work + ((25.0f - work) * (1.0f - ext_o0_2a428(LOCAL_RODATA_15C, D_1D94))));
        }
        ext_o8_49a4(state);
        state->outputScale = ext_o8_34a0(object, state, limit, D_4);
        ext_o8_49b4(state);
        ext_o0_1cfcc(object, state, D_1D94);
        action = &gO1PhysicsActions[2];
        actionIndex = 2;
        do {
            if (actionIndex != state->actionMode) {
                actionTest = action->test;
                if ((actionTest != NULL) && (action->mask & (1 << state->actionMode))) {

                    actionTest();
                }
            }
            actionIndex += 1;
            action++;
        } while (actionIndex != 6);
        actionUpdate = gO1PhysicsActions[state->actionMode].update;
        if (actionUpdate != NULL) {
            actionUpdate();
        }
        ext_o8_3278(object, state, D_1D94);
        ext_o0_1d510(object, state, 0, 3, D_1D94);
        ext_o8_2ec0(object, state, D_1D94);
        ext_o8_3018(object, state, state->outputScale, D_1D94);
        ext_o0_3e99c(object, D_1D94);
        if ((state->field349 != 0) && (state->field16C == 1)) {
            state->field16C = 0U;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F000438C_185076C.s")
#endif

/* ---- overlay1InitTimedState ---- */


/* DKR v77/v80 contains only generic object-state initialization patterns. */
typedef struct Overlay1TimedState {
    u8 pad0[0x193];
    u8 field193;
    u8 pad194[0x1EE];
    u8 enabled;
    u8 pad383[0xD];
    s16 timer;
    u8 pad392[0x52];
    s32 value3E4;
} Overlay1TimedState;

typedef struct Overlay1TimedObject {
    u8 pad0[0x64];
    Overlay1TimedState *state;
} Overlay1TimedObject;

void overlay1InitTimedState(Overlay1TimedObject *object, s32 timer) {
    Overlay1TimedState *state = object->state;

    state->enabled = 1;
    state->timer = timer;
    state->field193 = 0;
    state->value3E4 = 0;
}

/* ---- overlay1ConsumeTimer ---- */


typedef struct Overlay1TimerState {
    u8 pad000[0x382];
    u8 active;
    u8 pad383[0xD];
    u16 timer;
} Overlay1TimerState;

/* Fresh pinned DKR v77/v80 and JFG object scans found no exact donor. */
extern s32 gOverlay1TimerStep;

void overlay1ConsumeTimer(void) {
    if (((Overlay1TimerState *)gOverlay1TimerState)->timer <=
        gOverlay1TimerStep) {
        ((Overlay1TimerState *)gOverlay1TimerState)->active = 0;
    } else {
        ((Overlay1TimerState *)gOverlay1TimerState)->timer -=
            gOverlay1TimerStep;
    }
}

/* ---- overlay1StartTimerCallbacks ---- */


typedef void (*Overlay1Callback)(void);

typedef struct Overlay1CallbackState {
    u8 pad000[0x382];
    u8 mode;
} Overlay1CallbackState;

typedef struct Overlay1CallbackObject {
    u8 pad00[0x64];
    Overlay1CallbackState *state;
} Overlay1CallbackObject;

typedef struct Overlay1CallbackEntry {
    Overlay1Callback callback;
    s32 pad4;
    u16 modeMask;
    u16 padA;
} Overlay1CallbackEntry;

/* Fresh pinned DKR v77/v80 and JFG scans found no Overlay 1 donor.
 * Retained genuine C measures 36/56 words with all 13 runtime-backed records.
 * Lane 704a9f31 reached an apparent exact result only by adding an empty
 * pointer condition after the callback; that invented guard is rejected and
 * must not be promoted. Reproduce the natural baseline before a source-faithful
 * allocator/lifetime attempt. */
extern s32 overlay1IsObjectActive(void *object);
extern s32 gOverlay1TimerStep;
extern f32 gOverlay1CallbackStepFloat;
extern Overlay1CallbackEntry gOverlay1CallbackDescriptor[];
extern Overlay1CallbackEntry gOverlay1ModeCallbacks[];

void overlay1StartTimerCallbacks(Overlay1CallbackObject *object, s32 amount)
{
  Overlay1CallbackEntry *entry;
  Overlay1Callback callback;
  Overlay1Callback loadedCallback;
  s32 index;
  u8 mode;
  if (overlay1IsObjectActive(object) != 0)
  {
    gOverlay1TimerStep = amount;
    gOverlay1CallbackStepFloat = amount;
    entry = gOverlay1CallbackDescriptor;
    for (index = 5; index != 6; index++, entry++)
    {
      mode = ((Overlay1CallbackState *) gOverlay1TimerState)->mode;
      if (index != mode)
      {
        loadedCallback = entry->callback;
        callback = loadedCallback;
        if (callback != 0)
        {
          if ((entry->modeMask & (1 << mode)) != 0)
          {
            callback();
            if (entry->modeMask)
            {
            }
          }
        }
      }
    }

    mode = ((Overlay1CallbackState *) gOverlay1TimerState)->mode;
    callback = gOverlay1ModeCallbacks[mode].callback;
    if (callback != 0)
    {
      callback();
    }
  }
}

/* ---- overlay1FindDirectionalObject ---- */


#ifndef DOT_CONDITION
#define DOT_CONDITION (threshold < dot)
#endif

typedef struct Overlay1ObjectState { s8 tableIndex; } Overlay1ObjectState;
typedef struct Overlay1DirectionalObject {
    s16 angle; u8 pad02[0xA]; f32 x; u8 pad10[4]; f32 z;
    u8 pad18[0x4C]; Overlay1ObjectState *state;
} Overlay1DirectionalObject;
extern Overlay1DirectionalObject **overlay1GetObjectList(s32 *count);
extern f32 sqrtf(f32 value);
extern f32 overlay1TrigX(s32 angle);
extern f32 overlay1TrigY(s32 angle);
extern f32 D_160;

Overlay1DirectionalObject *overlay1FindDirectionalObject(
    Overlay1DirectionalObject *object, void *unused1, void *unused2,
    f32 threshold, f32 maxValue) {
    s32 count;
    Overlay1DirectionalObject **objects;
    Overlay1DirectionalObject *other;
    Overlay1DirectionalObject *best;
    Overlay1ObjectState *otherState;
    Overlay1ValueEntry *entry;
    s32 remaining;
    f32 dx, dz, distance, directionX, directionY, dot, value, bestValue;

    objects = overlay1GetObjectList(&count);
    best = 0;
    bestValue = D_160;
    if (overlay1IsObjectActive(object)) {
        goto active;
    }
    return 0;
active:
    while (remaining = count--) {
        other = objects[count];
        if (other == object) {
        } else {
            dx = other->x - object->x;
            otherState = other->state;
            dz = other->z - object->z;
            distance = sqrtf((dx * dx) + (dz * dz));
            if (distance > 0.0f) {
                dx /= distance;
                dz /= distance;
            }
            directionX = -overlay1TrigX(object->angle);
            directionY = -overlay1TrigY(object->angle);
            dot = (directionX * dx) + (directionY * dz);
            if (DOT_CONDITION) {
                entry = &D_1BA8[*(s8 *)D_1DA0].entries[otherState->tableIndex];
                value = entry->value;
                if ((value <= maxValue) && (value < bestValue)) {
                    bestValue = value;
                    best = other;
                }
            }
        }
    }
    return best;
}

/* ---- overlay1ReturnZero ---- */


/* DKR v77/v80 and JFG have no overlay-1 donor; this is a generic leaf. */
s32 overlay1ReturnZero(void) {
    return 0;
}

/* ---- overlay1DispatchMode ---- */


typedef struct Overlay1ModeState {
    u8 pad00;
    s8 index;
    u8 pad02[0x198];
    u8 mode;
    u8 timer;
    u8 pad19C[4];
    s32 task;
    u8 pad1A4[0x1DA];
    u8 group;
    u8 pad37F[0x1D];
    f32 angle;
    u8 pad3A0[8];
    u8 status[1];
} Overlay1ModeState;

typedef struct Overlay1ModeObject {
    u8 pad00[0x64];
    Overlay1ModeState *state;
} Overlay1ModeObject;
struct Overlay8ActivationOwner; struct Overlay36Object; struct Overlay36TickSource; struct Overlay36EffectSource;
#ifndef WORLD_GLOBAL_DECL
#define WORLD_GLOBAL_DECL extern Overlay1ModeState *D_1DA0_array[];
#define WORLD D_1DA0_array[0]
#endif
#ifndef CASE_END
#define CASE_END return 0
#endif
WORLD_GLOBAL_DECL
extern u8 D_6C[];
extern void func_overlay_008_F0000F1C_185EC74(struct Overlay8ActivationOwner *object, s32 arg);
extern s32 mathRnd(s32 minimum, s32 maximum);
extern Overlay1ModeObject *overlay1FindPreviousAngle(f32 angle);
extern Overlay1ModeObject *overlay1FindNextAngle(f32 angle);
extern void overlay36SpawnDirectional(struct Overlay36Object *object);
extern void overlay36SpawnLinked7F(struct Overlay36Object *object);
extern void overlay36SpawnOffsetA9(struct Overlay36Object *object);
extern void overlay36CallModeZero(void *object);
extern void overlay36TickState(struct Overlay36TickSource *object);
extern void overlay36UpdatePeers(struct Overlay36Object *object);
extern void overlay36SpawnFinalEffect(struct Overlay36EffectSource *object);
extern f32 overlay1WrapOffset(f32 first, f32 second);

/* Tier A: the ordinary IDO body and 0x28 frame are linked-ROM exact.
 * The 61 text relocation sites retain their shipped runtime identities.
 * External calls name the canonical definitions with their existing ABI;
 * local call aliases preserve the authentic pre-loader call addends.
 * No compiler instruction is edited by the metadata-only link recipe. */
/* The retained eight-entry table remains owned by initialized data.
 * Its compiler destinations agree relative to this function; the HI/LO
 * relocations bind to its stored addend without changing instructions. */

s32 overlay1DispatchMode(void) {
    Overlay1ModeState *world;
    Overlay1ModeObject *candidateObject;
    Overlay1ModeState *angleState;
    Overlay1ModeState *candidateState;
    Overlay1ModeObject *object;
    Overlay1ModeState *state;
    f32 difference;

    world = WORLD;
    switch (world->mode) {
        case 2:
            func_overlay_008_F0000F1C_185EC74(D_1D9C, 1);
            WORLD->timer--;
            if (WORLD->timer == 0) {
                WORLD->mode = 0xFF;
                WORLD->task = 0;
            }
            CASE_END;
        case 3:
            if (D_6C[WORLD->index] < mathRnd(1, 100)) {
                object = overlay1FindPreviousAngle(WORLD->angle);
                candidateObject = object;
                if (candidateObject != 0) {
                    state = object->state;
                    if (WORLD->status[state->index] >= 3) {
                        overlay36SpawnDirectional(D_1D9C);
                    }
                }
            }
            CASE_END;
        case 4:
            if (D_6C[WORLD->index] < mathRnd(1, 100)) {
                object = overlay1FindPreviousAngle(WORLD->angle);
                if (object != 0) {
                    state = object->state;
                    if (WORLD->status[state->index] >= 3) {
                        angleState = object->state;
                        difference = overlay1WrapOffset(WORLD->angle,
                                                        angleState->angle);
                        if ((0.5f <= difference) && (difference <= 4.0f)) {
                            overlay36SpawnLinked7F(D_1D9C);
                        }
                    }
                }
            }
            CASE_END;
        case 5:
            if (D_6C[WORLD->index] < mathRnd(1, 100)) {
                object = overlay1FindPreviousAngle(WORLD->angle);
                if (object != 0) {
                    state = object->state;
                    if (WORLD->status[state->index] >= 3) {
                        overlay36SpawnOffsetA9(D_1D9C);
                    }
                }
            }
            CASE_END;
        case 6:
            object = overlay1FindNextAngle(WORLD->angle);
            if (object != 0) {
                candidateState = object->state;
                state = candidateState;
                difference = overlay1WrapOffset(WORLD->angle, state->angle);
                if ((difference <= 3.0f) &&
                    (WORLD->status[state->index] >= 3) &&
                    (state->group == WORLD->group)) {
                    overlay36CallModeZero(D_1D9C);
                }
            }
            CASE_END;
        case 7:
            overlay36TickState(D_1D9C);
            CASE_END;
        case 8:
            overlay36UpdatePeers(D_1D9C);
            CASE_END;
        case 9:
            overlay36SpawnFinalEffect(D_1D9C);
            CASE_END;
    }
    return 0;
}





/* ---- overlay1HandleCachedMode ---- */

typedef struct W { u8 p0[0xD4]; void *object; u8 pD8[0xC3]; u8 enabled; s32 state; } W;
extern s32 D_83E4;
extern s32 overlay27CanUse(void *);
extern s32 overlay3RunCachedModeAction(void *, W *);
extern s32 overlay1DispatchMode(void);
/* Workbench verdict: structure-mismatch, 23 differing words, first mismatch +0x0.
 * Shape: one extra instruction (33/32) with an exact 0x18 frame; not shape-exact.
 * Remaining gap: callback-clear control flow and unresolved relocation identities. */
#ifdef NON_MATCHING
s32 overlay1HandleCachedMode(void) {
    if (((W *)D_1DA0)->enabled == 0) goto clear;
    if (overlay27CanUse(((W *)D_1DA0)->object) == 0) {
        if (D_83E4 == 3) {
            return overlay3RunCachedModeAction(D_1D9C, (W *)D_1DA0);
        }
        return overlay1DispatchMode();
    }
clear:
    ((W *)D_1DA0)->state = 0;
    return 0;
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F00061F0_18525D0.s")
#endif

/* ---- overlay1ChooseModeObject ---- */


typedef struct O1SelectState { s8 tableIndex; } O1SelectState;
typedef struct O1SelectObject { u8 pad00[0x64]; O1SelectState *state; } O1SelectObject;
typedef struct O1SelectEntry { f32 value; u8 pad04[8]; } O1SelectEntry;
typedef struct O1SelectRow { O1SelectEntry entries[6]; } O1SelectRow;
typedef struct O1Selection { O1SelectObject *object; s16 value; } O1Selection;
typedef struct O1SelectWorld {
    s8 row;
    u8 pad01[0x381];
    u8 mode;
    u8 pad383[0xD];
    O1Selection selection;
    u8 pad398[0x4C];
    O1SelectObject *selected;
} O1SelectWorld;

extern O1SelectObject **func_80005750(s32 *count);
extern s32 mathRnd(s32 minimum, s32 maximum);

s32 overlay1ChooseModeObject(void) {
    s32 count;
    s32 remaining;
    O1SelectObject *object;
    s32 choiceCount;
    O1Selection *selection;
    O1SelectObject *choices[5];
    O1SelectObject **objects;

    objects = func_80005750(&count);
    choiceCount = 0;
    remaining = count--;
    while (remaining != 0) {
        object = objects[count];
        {
            O1SelectState *state = object->state;
            if (object != D_1D9C) {
                if (D_1BA8[((O1SelectWorld *)D_1DA0)->row]
                        .entries[state->tableIndex]
                        .value < 600.0f) {
                    choices[choiceCount++] = object;
                }
            }
        }
        remaining = count--;
    }
    if (choiceCount != 0) {
        count = mathRnd(1, choiceCount) - 1;
        object = choices[count];
        selection = &((O1SelectWorld *)D_1DA0)->selection;
        selection->object = object;
        selection->value = mathRnd(0x5A, 0x84);
        ((O1SelectWorld *)D_1DA0)->mode = 5;
        ((O1SelectWorld *)D_1DA0)->selected = object;
        return 1;
    }
    return 0;
}

/* ---- overlay1UpdateCountdown ---- */


typedef struct Overlay1Countdown {
    u8 pad0[4];
    u16 value;
} Overlay1Countdown;

typedef struct Overlay1CountdownObject {
    u8 pad0[0x390];
    Overlay1Countdown countdown;
} Overlay1CountdownObject;

extern Overlay1CountdownObject *gOverlay1CountdownObject;
extern s32 gOverlay1CountdownAmount;
extern void *gOverlay1CountdownResource;
extern void overlay1CountdownReloc(void *, s32);

/* DKR v77/v80 and JFG contain no exact donor for this countdown update. */
void overlay1UpdateCountdown(void) {
    u8 *object;
    s32 amount;
    register s32 mode;
    u16 countdown;

    object = (u8 *)gOverlay1CountdownObject;
    amount = gOverlay1CountdownAmount;
    countdown = *(u16 *)(object + 0x394); mode = 0x78; object += 0x390;
    /* Legacy inert spelling present in the retained exact C producer; whether
     * it is codegen-required has not been isolated. Tracked in
     * docs/cleanup-queue.md. */
    if ((((u32)object & mode) != 0) && (object == 0)) {
    }
    if (countdown <= amount) {
        overlay1CountdownReloc(gOverlay1CountdownResource, mode);
    } else {
        *(u16 *)(object + 4) = countdown - amount;
    }
}

/* ---- overlay1ReadSelection ---- */


typedef struct Overlay1Vector {
    f32 x;
    f32 y;
    f32 z;
} Overlay1Vector;

typedef struct Overlay1Descriptor {
    u8 pad00[0x2D];
    u8 count;
} Overlay1Descriptor;

typedef struct Overlay1Selection {
    Overlay1Descriptor *descriptor;
    u8 pad04[4];
    s16 useObjectPosition;
    u8 pad0A[0x36];
    Overlay1Vector *vectors;
} Overlay1Selection;

typedef struct Overlay1Object {
    u8 pad00[0x0C];
    Overlay1Vector position;
    u8 pad18[0x22];
    s8 selectedIndex;
    u8 pad3B[0x0D];
    u8 *fallback;
    u8 pad4C[0x1C];
    Overlay1Selection **selections;
} Overlay1Object;

void overlay1ReadSelection(Overlay1Object *object, s32 index, f32 *outX,
                           f32 *outY, f32 *outZ) {
    Overlay1Selection *selection;
    Overlay1Descriptor *descriptor;
    Overlay1Vector *vectors;
    s32 offset;

    selection = object->selections[object->selectedIndex];
    if (selection != 0) {
        descriptor = selection->descriptor;
        if (selection->useObjectPosition != 0) {
            *outX = object->position.x;
            *outY = object->position.y;
            *outZ = object->position.z;
            return;
        }
        if (descriptor->count >= index) {
            vectors = selection->vectors;
            offset = index * sizeof(Overlay1Vector);
            if (vectors == 0) {
                return;
            }
            *outX = *(f32 *)((u8 *)vectors + offset + 0);
            *outY = *(f32 *)((u8 *)selection->vectors + offset + 4);
            *outZ = *(f32 *)((u8 *)selection->vectors + offset + 8);
            return;
        }
    }

    *outX = *(f32 *)(object->fallback + 0x24);
    *outY = *(f32 *)(object->fallback + 0x28);
    *outZ = *(f32 *)(object->fallback + 0x2C);
}

/* ---- overlay1SolveAngleCandidates ---- */


extern f32 overlay1SqrtReloc(f32 value);
extern s32 overlay1AngleReloc(f32 y, f32 x);

/* Fresh phase-5 plateau: configured C remains 137/139 words, frame 0xA8,
 * with 92 raw/masked differences and first relocation mismatch +0x50. The
 * target's root loop retains a two-instruction generic-count preheader. Nine
 * natural loop/count forms were identical or worse; the only exact-size form
 * added a saved-register web. -O2 -g3 reaches 139 words/86 differences but
 * perturbs the shared-TU prologue, so it is diagnostic rather than a flag fix. */
#ifdef NON_MATCHING
s16 overlay1SolveAngleCandidates(
    f32 x0, f32 y0, f32 x1, f32 y1,
    f32 y2, f32 x2, f32 radius, f32 slope, s32 chooseHigh) {
    f32 dx;
    f32 dy;
    f32 distance;
    f32 sum;
    f32 discriminant;
    f32 discriminantRoot;
    f32 denominator;
    f32 root;
    f32 angleX;
    s32 solutionCount;
    s16 solutions[2];
    s32 sign;

    solutionCount = 0;
    dx = x0 - y1;
    dy = x1 - x2;
    distance = overlay1SqrtReloc((dx * dx) + (dy * dy));
    dy = y2 - y0;
    sum = (dy * slope) + (radius * radius);
    discriminant = (sum * sum) -
        ((slope * slope) * ((distance * distance) + (dy * dy)));

    if (discriminant >= 0.0f) {
        discriminantRoot = overlay1SqrtReloc(discriminant);
        denominator = (((dy * dy) / (distance * distance)) + 1.0f) * 2.0f;

        sign = solutionCount + 2;
        while (sign--) {
            if (sign != 0) {
                root = discriminantRoot;
            } else {
                root = -discriminantRoot;
            }
            root = (root + sum) / denominator;
            if (root >= 0.0f) {
                angleX = overlay1SqrtReloc(root);
                if (distance < 0.0f) {
                    angleX = -angleX;
                }
                solutions[solutionCount] = overlay1AngleReloc(
                    ((dy / distance) * angleX) -
                    ((slope * distance) / (angleX + angleX)), angleX);
                solutionCount++;
            }
        }
    }

    if (solutionCount != 1) {
        if (solutionCount == 2) {
            if (solutions[0] < solutions[1]) {
                return chooseHigh ? solutions[1] : solutions[0];
            }
            return chooseHigh ? solutions[0] : solutions[1];
        }
        return 0x2000;
    }
    return solutions[0];
}

s32 overlay1LoopControlCarrier(s32 value) {
    if (value == 0) {
        return 2;
    }
    return value;
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F00064F8_18528D8.s")
#endif

/* ---- overlay1UpdateModeSound ---- */


typedef struct Overlay1SoundModeObject {
    u8 pad0[0x193];
    u8 mode;
    u8 pad194[0x14];
    u16 flags;
} Overlay1SoundModeObject;

extern Overlay1SoundModeObject *gOverlay1ModeObject;
extern void *gOverlay1ModeResource;
extern void overlay1ModeSoundReloc(void *, s32);

/* DKR v77/v80 and JFG contain no exact donor for this mode-gated wrapper. */
void overlay1UpdateModeSound(void) {
    Overlay1SoundModeObject *object;

    object = gOverlay1ModeObject;
    if (object->mode == 13) {
        if (object->flags & 2) {
            overlay1ModeSoundReloc(gOverlay1ModeResource, 0x78);
        }
    } else {
        overlay1ModeSoundReloc(gOverlay1ModeResource, 0x78);
    }
}

/* ---- overlay1CopyBytes ---- */


/* DKR v77/v80 contains only generic byte-copy initialization patterns. */
typedef struct Overlay1ByteState {
    u8 values[6];
} Overlay1ByteState;

typedef struct Overlay1ByteObject {
    u8 pad0[0x64];
    Overlay1ByteState *state;
} Overlay1ByteObject;

typedef struct Overlay1ByteInit {
    u8 pad0[0xA];
    u8 values[6];
} Overlay1ByteInit;

void overlay1CopyBytes(Overlay1ByteObject *object, Overlay1ByteInit *init) {
    Overlay1ByteState *state = object->state;

    state->values[0] = init->values[0];
    state->values[1] = init->values[1];
    state->values[2] = init->values[2];
    state->values[3] = init->values[3];
    state->values[4] = init->values[4];
    state->values[5] = init->values[5];
}

/* ---- overlay1UpdateRangeFlags ---- */


typedef struct Overlay1RangeConfig {
    u8 angleHigh;
    u8 horizontalScale;
    u8 verticalScale;
    u8 mode;
    u8 soundId;
} Overlay1RangeConfig;

typedef struct Overlay1RangeState {
    u8 pad000[0x1A8];
    u16 flags;
} Overlay1RangeState;

typedef struct Overlay1HeightData {
    u8 pad000[0x5C];
    f32 height;
} Overlay1HeightData;

typedef struct Overlay1RangeObject {
    u8 pad000[0xC];
    f32 x;
    f32 y;
    f32 z;
    u8 pad018[0x30];
    Overlay1HeightData *heightData;
    u8 pad04C[0x18];
    void *state;
} Overlay1RangeObject;

extern Overlay1RangeObject **overlay1GetObjectListReloc(s32 *count);
extern s32 overlay1GetAngleValueReloc(f32 dz, f32 dx);
extern void overlay1ActivateObjectReloc(Overlay1RangeObject *object);
extern void overlay1PlaySoundReloc(u8 soundId);

/* Workbench verdict=allocation-mismatch; 31 raw/masked words differ in the exact 120-word/0x70 frame, first +0x34.
 * Declaration order moves the count home to sp+0x68; instruction count, opcode order, and relocation sites are exact.
 * Remaining register-only residual is permuter-ready; four relocation names remain overlay-local aliases. */
#ifdef NON_MATCHING
void overlay1UpdateRangeFlags(Overlay1RangeObject *object, void *unused) {
    Overlay1RangeConfig *config;
    s32 count;
    Overlay1RangeObject **objects;
    register s32 clearMask;

    config = object->state;
    clearMask = ~8;
    objects = overlay1GetObjectListReloc(&count);
    if (count--) {
        do {
            Overlay1RangeObject *other;
            Overlay1RangeState *otherState;
            f32 dx;
            f32 dz;
            u32 horizontalRange;
            s16 angle;

            other = objects[count];
            otherState = other->state;
            dx = other->x - object->x;
            dz = other->z - object->z;
            horizontalRange = (u32)config->horizontalScale * 10U;
            if ((dx * dx + dz * dz) <
                (f32)(s32)(horizontalRange * horizontalRange)) {
                angle = (s16)(((u32)config->angleHigh << 8) +
                              overlay1GetAngleValueReloc(dz, dx));
                if ((angle < -0x4000) || (angle >= 0x4001)) {
                    if ((object->y <= other->y + other->heightData->height) &&
                        (other->y <= object->y +
                         (f32)(s32)((u32)config->verticalScale * 10U))) {
                        switch (config->mode) {
                            case 0: {
                                u16 flags;
                                flags = otherState->flags;
                                if (!(flags & 8)) {
                                    otherState->flags = flags | 8;
                                }
                                break;
                            }
                            case 1: {
                                u16 flags;
                                flags = otherState->flags;
                                if (flags & 8) {
                                    otherState->flags = flags & clearMask;
                                    overlay1ActivateObjectReloc(other);
                                    overlay1PlaySoundReloc(config->soundId);
                                }
                                break;
                            }
                        }
                    }
                }
            }
        } while (count--);
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F00067C0_1852BA0.s")
#endif

/* ---- overlay1InitMotion ---- */


/* No corresponding DKR/JFG source or object match was found. */
typedef struct Overlay1MotionState {
    f32 magnitude;
    s32 mode;
    u16 first;
    u16 second;
} Overlay1MotionState;

typedef struct Overlay1MotionObject {
    u8 pad0[8];
    f32 scaledMagnitude;
    u8 padC[0x58];
    Overlay1MotionState *state;
} Overlay1MotionObject;

typedef struct Overlay1MotionInit {
    u8 pad0[0xA];
    u8 magnitude;
    u8 mode;
    u16 first;
    u16 second;
} Overlay1MotionInit;

extern f32 gOverlay1MotionScale;

void overlay1InitMotion(Overlay1MotionObject *object, Overlay1MotionInit *init) {
    Overlay1MotionState *state;

    state = object->state;
    object->scaledMagnitude = (f32)init->magnitude * gOverlay1MotionScale;
    state->magnitude = (f32)init->magnitude;
    state->mode = init->mode;
    state->first = init->first;
    state->second = init->second;
}

/* ---- overlay1ConsumeNearbyPending ---- */


typedef struct Overlay1NearbyState {
    f32 radius;
    s32 mode;
    u16 kind;
} Overlay1NearbyState;

typedef struct Overlay1OtherState {
    s8 kind;
    u8 pad01[0x191];
    u8 pending;
    u8 pad193[0x221];
    s16 count;
} Overlay1OtherState;

typedef struct Overlay1NearbyObject {
    u8 pad00[0xC];
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[0x4C];
    void *state;
} Overlay1NearbyObject;

#ifdef NON_MATCHING
/* Workbench: structure-mismatch, exact 69-word extent and 0x48 frame, with
 * 31 differing words and first codegen mismatch +0x40. An explicit initial
 * countdown removes the old extra move; register lanes remain divergent. */
void overlay1ConsumeNearbyPending(void *objectArg, void *listArg) {
    Overlay1NearbyState *state;
    f32 radiusSquared;
    s32 count;
    Overlay1NearbyObject *other;
    Overlay1OtherState *otherState;

    state = ((Overlay1NearbyObject *)objectArg)->state;
    radiusSquared = state->radius * 4.0f;
    radiusSquared *= state->radius * 4.0f;
    {
        Overlay1NearbyObject *object = objectArg;
        listArg = overlay1GetObjectListReloc(&count);
        if (count != 0) {
            count--;
            do {
                other = ((Overlay1NearbyObject **)listArg)[count];
                otherState = other->state;
                if (state->kind == otherState->kind) {
                    f32 dx = other->x - object->x;
                    f32 dy = other->y - object->y;
                    f32 dz = other->z - object->z;
                    if (((dx * dx) + (dy * dy) + (dz * dz) < radiusSquared) &&
                        (state->mode == 2)) {
                        u8 pending = otherState->pending;
                        if (pending) {
                            otherState->pending = 0;
                            otherState->count += pending;
                        }
                    }
                }
            } while (count--);
        }
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F0006A14_1852DF4.s")
#endif

/* ---- overlay1InitRange ---- */


/* Compact range initializer; exact DKR and JFG scans are negative. */
typedef struct Overlay1InitRangeState { s16 a, b; s32 value; u8 c, d; } Overlay1InitRangeState;
typedef struct Overlay1InitRangeObject { u8 pad0[0x64]; Overlay1InitRangeState *state; } Overlay1InitRangeObject;
typedef struct Overlay1RangeInit { u8 pad0[0xA]; u8 a, b, value, c, d; } Overlay1RangeInit;
void overlay1InitRange(Overlay1InitRangeObject *object, Overlay1RangeInit *init) {
    Overlay1InitRangeState *state = object->state;
    state->a = init->a * 10; state->b = init->b * 10;
    state->value = init->value; state->c = init->c; state->d = init->d;
}

/* ---- overlay1SearchNearby ---- */


typedef struct Overlay1SearchState {
    u16 xRange;
    u16 zRange;
    u8 pad04[2];
    u8 flags;
    u8 pad07;
    u8 lookupKey;
    u8 pad09[2];
    u8 active;
    u8 pad0C[0x3A8];
    s16 counter;
} Overlay1SearchState;

typedef struct Overlay1SearchObject {
    u8 pad00[0xC];
    f32 x;
    u8 pad10[4];
    f32 z;
    u8 pad18[0x2C];
    s16 type;
    u8 pad46[0x1E];
    Overlay1SearchState *state;
} Overlay1SearchObject;

extern Overlay1SearchObject **func_8000572C(s32 *first, s32 *limit);
extern Overlay1SearchObject *func_80005820(u8 key);
extern void overlay4RemoveObject(Overlay1SearchObject *object);

void overlay1SearchNearby(Overlay1SearchObject *object, void *unused) {
    Overlay1SearchState *range;
    s32 first;
    s32 limit;
    s32 index;
    Overlay1SearchObject **objects;
    Overlay1SearchObject *candidate;
    Overlay1SearchState *state;
    Overlay1SearchObject *linked;
    f32 delta;
    f32 threshold;

    (void)unused;
    range = object->state;
    objects = func_8000572C(&first, &limit);
    index = first;
    if (index < limit) {
        do {
            candidate = objects[index];
            if (candidate->type == 0x21) {
                delta = candidate->x - object->x;
                state = candidate->state;
                threshold = range->xRange;
                if (delta < 0.0f) {
                    delta = -delta;
                }
                if (delta <= threshold) {
                    delta = candidate->z - object->z;
                    threshold = range->zRange;
                    if (delta < 0.0f) {
                        delta = -delta;
                    }
                    if (delta <= threshold) {
                        linked = func_80005820(range->lookupKey);
                        if (linked != 0) {
                            linked->state->counter++;
                        }
                        state->flags |= 4;
                        state->active = 1;
                        overlay4RemoveObject(candidate);
                        return;
                    }
                }
            }
            index++;
        } while (index != limit);
    }
}

/* ---- overlay1SelectMaskedMode ---- */


typedef struct Overlay1MaskedState {
    u8 pad0[0x382];
    u8 bitIndex;
    u8 pad383[0xD];
    u8 timer;
} Overlay1MaskedState;

typedef struct Overlay1MaskedObject {
    u8 pad0[0x64];
    Overlay1MaskedState *state;
} Overlay1MaskedObject;

extern u8 gOverlay1ModeMasks[];
extern void overlay1SelectModeReloc(void *, s32);

/* DKR v77/v80 and JFG contain no exact donor for this table-mask selector. */
s32 overlay1SelectMaskedMode(Overlay1MaskedObject *object, s32 index) {
    Overlay1MaskedState *state;

    state = object->state;
    if (*(u16 *)(gOverlay1ModeMasks + index * 12 + 0xC4) &
        (1 << state->bitIndex)) {
        state->bitIndex = index;
        overlay1SelectModeReloc(&state->timer, 8);
        return 1;
    }
    return 0;
}

/* ---- overlay1UpdateAimedTransient ---- */


typedef struct Overlay1TransientState {
    void *owner;
    s16 mode;
    u8 type;
    u8 active;
    u8 selector;
    s8 linkedIndex;
    u8 pad0A;
} Overlay1TransientState;

typedef struct Overlay1MotionSourceExtra {
    u8 pad00[0x5C];
    f32 height;
} Overlay1MotionSourceExtra;

typedef struct Overlay1MotionSourceState {
    u8 pad00;
    s8 index;
} Overlay1MotionSourceState;

typedef struct Overlay1MotionSource {
    u8 pad00[0xC];
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[4];
    f32 velocityX;
    f32 velocityY;
    f32 velocityZ;
    u8 pad28[0x20];
    Overlay1MotionSourceExtra *extra;
    u8 pad4C[0x18];
    Overlay1MotionSourceState *state;
} Overlay1MotionSource;

typedef struct Overlay1VelocityExtra {
    u8 pad00[0xE0];
    f32 *value;
} Overlay1VelocityExtra;

typedef struct Overlay1TransientObject {
    u8 pad00[8];
    f32 scale;
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[4];
    f32 velocityX;
    f32 velocityY;
    f32 velocityZ;
    u8 pad28[0x18];
    Overlay1VelocityExtra *extra;
    u8 pad44[0x20];
    Overlay1TransientState *state;
    u8 pad68[0x10];
    s16 *flags;
} Overlay1TransientObject;

typedef struct Overlay1TransientOwner {
    s16 angle;
    u8 pad02[0x26];
    f32 distance;
} Overlay1TransientOwner;

typedef struct Overlay1TransientWorld {
    u8 pad00[0x193];
    u8 mode;
    u8 pad194[0x14];
    u16 flags;
    u8 pad1AA[0x1E6];
    Overlay1MotionSource *source;
    Overlay1TransientObject *object;
    u8 pad398[0x84];
    u32 status;
} Overlay1TransientWorld;

extern f32 D_4;
extern f32 overlay1AimedScaleReloc;
extern f32 overlay1AimedThresholdReloc;
extern f32 overlay1AimedTrigReloc;
extern f32 overlay1AimedVelocityYReloc;

extern Overlay1TransientObject *func_overlay_036_F0000694_1883B4C(
    Overlay1TransientOwner *owner, Overlay1TransientWorld *world);
extern s16 func_overlay_001_F00064F8_18528D8(
    f32, f32, f32, f32, f32, f32, f32, f32, s32);
extern s32 func_8002A910(f32 y, f32 x);
extern f32 func_8002A8BC(s32 angle);
extern f32 func_8002A8C0(s32 angle);
extern f32 sqrtf(f32 value);

/* Plateau (2026-08-30): -O2 -mips2 -Wab,-r4300_mul emits the exact
 * 996-byte extent.  An O32 integer carrier for the shared-world address and
 * saved-state declaration order reduce the raw residual from 64 to 32 words.
 * The candidate frame remains 0x88 versus 0x80, and 38 of 43 runtime
 * relocation offsets/types align; strict identities remain ambiguous in the
 * shared Overlay 1 TU.  Ten coherent source forms and all 119 flags are
 * exhausted; the remaining early-load, stack-home, and constant-call schedule
 * needs new source-authentic evidence rather than generic permutation. */
#ifdef NON_MATCHING
void overlay1UpdateAimedTransient(void) {
    Overlay1TransientWorld *world;
    u32 worldAddress;
    Overlay1TransientOwner *owner;
    Overlay1TransientObject *object;
    Overlay1TransientState *savedState;
    Overlay1TransientState *state;
    Overlay1MotionSource *source;
    f32 factor;
    f32 predictedX;
    f32 predictedY;
    f32 predictedZ;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    f32 distance;
    f32 trig;
    s32 iteration;
    s16 sourceAngle;
    s16 objectAngle;

    worldAddress = (u32)&D_1DA0;
    world = *(Overlay1TransientWorld **)worldAddress;
    object = world->object;
    source = world->source;
    if (object == 0) {
        object = func_overlay_036_F0000694_1883B4C(D_1D9C, world);
        if (object != 0) {
            state = object->state;
            state->owner = D_1D9C;
            state->type = 3;
            state->active = 1;
            state->selector = 9;
            state->pad0A = 0;
            object->scale = overlay1AimedScaleReloc;
            overlay1ReadSelection(D_1D9C, 9, &object->x, &object->y,
                                  &object->z);
            (*(Overlay1TransientWorld **)worldAddress)->object = object;
            savedState = state;
        }
        world = D_1DA0;
        state = savedState;
    } else {
        state = object->state;
    }

    if ((world->mode == 0xD) && state->active &&
        (overlay1AimedThresholdReloc <=
         ((Overlay1TransientOwner *)D_1D9C)->distance)) {
        state->active = 0;
        state->mode = 0xC;
        if (source != 0) {
            state->linkedIndex = source->state->index;
            factor = 0.0f;
            iteration = 3;
            do {
                predictedX = source->x + (factor * source->velocityX);
                predictedY = source->y + (factor * source->velocityY) +
                    (source->extra->height * 0.5f);
                predictedZ = source->z + (factor * source->velocityZ);
                deltaX = predictedX - object->x;
                deltaY = predictedY - object->y;
                deltaZ = predictedZ - object->z;
                distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) +
                                 (deltaZ * deltaZ));
                factor = 30.0f / distance;
                if (factor > 0.0f) {
                    deltaX *= factor;
                    deltaZ *= factor;
                }
                factor = distance / (30.0f * D_4);
            } while (iteration--);

            sourceAngle = func_8002A910(deltaX, deltaZ);
            objectAngle = func_overlay_001_F00064F8_18528D8(
                object->x, object->y, object->z,
                predictedX, predictedY, predictedZ,
                30.0f, -*object->extra->value, 0);
            trig = func_8002A8BC(objectAngle);
            object->velocityX = func_8002A8C0(sourceAngle) * trig * 30.0f;
            object->velocityY = func_8002A8C0(objectAngle) * 30.0f;
            trig = func_8002A8BC(objectAngle);
            object->velocityZ = func_8002A8BC(sourceAngle) * trig * 30.0f;
        } else {
            state->linkedIndex = -1;
            trig = overlay1AimedTrigReloc;
            object->velocityX =
                func_8002A8C0(((Overlay1TransientOwner *)D_1D9C)->angle) * trig *
                -30.0f;
            object->velocityY = overlay1AimedVelocityYReloc;
            object->velocityZ =
                func_8002A8BC(((Overlay1TransientOwner *)D_1D9C)->angle) * trig *
                -30.0f;
        }
        *object->flags &= ~2;
        world = D_1DA0;
    }

    if (world->flags & 2) {
        if (world->mode == 0xD) {
            overlay1InitTimedState(D_1D9C, 0x78);
            world = D_1DA0;
        }
        if (world->mode == 0xD) {
            world->mode = 0xD;
            world = D_1DA0;
        }
    }
    if (!(world->status & 0x2000) && world->mode == 0xD) {
        world->mode = 0xD;
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F0006D4C_185312C.s")
#endif

/* ---- overlay1UpdateTransient ---- */


typedef struct Overlay1SimpleTransientState {
    void *owner;
    s16 mode;
    u8 type;
    u8 active;
    u8 selector;
    s8 linkedIndex;
    u8 pad0A;
} Overlay1SimpleTransientState;

typedef struct Overlay1SimpleTransientObject {
    u8 pad00[8];
    f32 scale;
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[4];
    f32 velocityX;
    f32 velocityY;
    f32 velocityZ;
    u8 pad28[0x3C];
    Overlay1SimpleTransientState *state;
    u8 pad68[0x10];
    s16 *flags;
} Overlay1SimpleTransientObject;

typedef struct Overlay1SimpleTransientOwner {
    u8 pad00[0x28];
    f32 distance;
} Overlay1SimpleTransientOwner;

typedef struct Overlay1SimpleTransientWorld {
    u8 pad00[0x193];
    u8 mode;
    u8 pad194[0x14];
    u16 flags;
    u8 pad1AA[0x1EA];
    Overlay1SimpleTransientObject *object;
} Overlay1SimpleTransientWorld;

extern f32 gOverlay1TransientScale;
extern f32 gOverlay1TransientThreshold;
extern f32 gOverlay1TransientVelocityY;
extern Overlay1SimpleTransientObject *overlay1TransientCallReloc();

void overlay1UpdateTransient(void) {
    Overlay1SimpleTransientObject *object;
    Overlay1SimpleTransientState *state;

    object = ((Overlay1SimpleTransientWorld *)D_1DA0)->object;
    if (object == 0) {
        object = overlay1TransientCallReloc(
            (Overlay1SimpleTransientOwner *)D_1D9C,
            (Overlay1SimpleTransientWorld *)D_1DA0);
        if (object != 0) {
            state = object->state;
            state->owner = D_1D9C;
            state->type = 2;
            state->active = 1;
            state->selector = 9;
            state->pad0A = 0;
            object->scale = gOverlay1TransientScale;
            overlay1TransientCallReloc(D_1D9C, 9, &object->x, &object->y,
                                       &object->z);
            ((Overlay1SimpleTransientWorld *)D_1DA0)->object = object;
        }
    } else {
        state = object->state;
    }

    if (((Overlay1SimpleTransientWorld *)D_1DA0)->flags & 2) {
        overlay1TransientCallReloc(D_1D9C, 0x78);
    }
    if ((((Overlay1SimpleTransientWorld *)D_1DA0)->mode == 0xD) &&
        (((Overlay1SimpleTransientOwner *)D_1D9C)->distance >= gOverlay1TransientThreshold) &&
        state->active) {
        state->active = 0;
        state->linkedIndex = -1;
        state->mode = 0xC;
        object->velocityX = 0.0f;
        object->velocityY = gOverlay1TransientVelocityY;
        object->velocityZ = 0.0f;
        *object->flags &= ~2;
    }
}

/* ---- overlay1AllocateRecord ---- */


typedef struct Overlay1PoolBits {
    u8 high;
    u8 group : 6;
    u8 unused : 1;
    u8 active : 1;
} Overlay1PoolBits;

typedef union Overlay1PoolFlags {
    u16 value;
    Overlay1PoolBits bits;
} Overlay1PoolFlags;

typedef struct Overlay1PoolRecord {
    u8 pad00[0xC0];
    Overlay1PoolFlags flags;
    u8 padC2[0xA];
} Overlay1PoolRecord;

extern Overlay1PoolRecord *D_218;
extern Overlay1PoolRecord D_220[];
extern Overlay1PoolRecord D_1BA0[];
extern s32 D_1D88;
extern s32 gOverlay1PoolExhausted;
extern s32 D_1D84;

/* The redundant width mask is semantically inert for the u16 value; it
 * preserves IDO's shipped temporary-FIFO phase. See docs/cleanup-queue.md. */
Overlay1PoolRecord *overlay1AllocateRecord(void) {
    Overlay1PoolRecord *cursor;
    Overlay1PoolRecord *result;

    cursor = D_218;
    result = cursor;
    do {
        cursor = (D_218 = cursor + 1);
        if (cursor >= D_1BA0) {
            D_218 = D_220;
            cursor = D_220;
        }
        if (result == cursor) {
            D_1D84 = 1;
            return 0;
        }
    } while ((((u32)*((u8 *)cursor + 0xC1) >> 2) == D_1D88) &&
             (((cursor->flags.value & 0xFFFF) & 1) == 0));

    result->flags.bits.group = D_1D88;
    return result;
}

/* ---- overlay1CloneRecord ---- */


extern void *overlay1AllocateRecordReloc(u32 *source);

/* DKR v77/v80 and JFG have generic copy loops, but no exact donor. */
/* Exact C: all 23 instruction words, the -24 frame, relocation identity, and
 * linked overlay range match after bounded permutation. */
void *overlay1CloneRecord(u32 *source)
{
  u32 *destination;
  register u32 remaining;
  void *result;
  result = overlay1AllocateRecordReloc(source);
  if (result == 0)
  {
    remaining = 0;
    return remaining;
  }
  remaining = 50;
  {
    u32 *input;
    input = source;
    destination = result;
    remaining = 50;
    do
    {
      *(destination++) = *(input++);
    }
    while (remaining--);
  }
  return result;
}

/* ---- overlay1UpdateValueCache ---- */


typedef struct Overlay1CacheValueEntry {
    s16 keyA;
    s16 keyB;
    u32 value;
} Overlay1CacheValueEntry;

extern Overlay1CacheValueEntry gOverlay1ValueCache[64];

/* Matched 2026-08-29 after a bounded permuter found the zero-valued counter
 * expression that preserves IDO's target caller-saved web order. The stock
 * mixed-TU object is exact for all 120 words and both local relocations. */
s32 overlay1UpdateValueCache(s16 keyA, s16 keyB, f32 value) {
    register s32 searchKeyA = keyA;
    register s32 searchKeyB = keyB;
    Overlay1CacheValueEntry *entry;
    s32 remaining;

    entry = gOverlay1ValueCache;
    remaining = 0x3F;
    do {
        if ((entry->value != 0) && (searchKeyA == entry->keyA) &&
            (searchKeyB == entry->keyB)) {
            if (value < (f32)entry->value) {
                entry->value = (u32)value;
                return 1;
            }
            return remaining * 0;
        }
        entry++;
    } while (remaining--);

    entry = gOverlay1ValueCache;
    remaining = 0x3F;
    do {
        if (entry->value == 0) {
            entry->keyA = searchKeyA;
            entry->keyB = searchKeyB;
            entry->value = (u32)value;
            return 1;
        }
        entry++;
    } while (remaining--);

    return 0;
}

/* ---- overlay1AppendPathPoint ---- */


typedef struct Overlay1PathState {
    s16 x[32];
    s16 y[32];
    u8 primary[32];
    u8 secondary[32];
    u8 count;
    u8 flags;
    u8 padC2[2];
    f32 length;
    u32 anchorDistanceSquared;
} Overlay1PathState;

extern f32 sqrtf(f32 value);
extern s32 overlay1UpdateValueCache(s16 x, s16 y, f32 value);
extern s16 overlay1AnchorX;
extern s16 overlay1AnchorY;

#ifdef NON_MATCHING
void overlay1AppendPathPoint(Overlay1PathState *state, s16 x, s16 y,
                             u8 primary, u8 secondary) {
    register s32 pointX = x;
    register s32 pointY = y;
    u8 index = state->count;
    s16 dx = pointX - state->x[index];
    s16 dy;
    s16 anchorX;
    s16 anchorDx;

    dy = pointY - state->y[index];
    state->count = index + 1;
    state->x[state->count] = pointX;
    state->y[state->count] = pointY;
    state->primary[state->count] = primary;
    state->secondary[state->count] = secondary;
    state->length += sqrtf((f32)((dx * dx) + (dy * dy)));
    state->flags = (state->flags & ~3) | 1;

    if ((state->count >= 2) &&
        (overlay1UpdateValueCache(pointX, pointY, state->length) == 0)) {
        state->flags &= ~3;
        return;
    }

    anchorX = overlay1AnchorX;
    anchorDx = pointX - anchorX;
    if ((pointX == anchorX) && (pointY == overlay1AnchorY)) {
        state->anchorDistanceSquared = 0;
    } else {
        s16 anchorDy = pointY - overlay1AnchorY;
        state->anchorDistanceSquared =
            (anchorDx * anchorDx) + (anchorDy * anchorDy);
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F0007580_1853960.s")
#endif

/* ---- overlay1BendPathPoint ---- */


typedef struct Overlay1PathPoint {
    s16 x;
    s16 y;
} Overlay1PathPoint;

typedef struct Overlay1Path {
    Overlay1PathPoint *points;
    u32 count;
} Overlay1Path;

/* Fresh pinned DKR v77/v80 and JFG scans found no Overlay 1 donor. */
extern Overlay1Path *overlay1GetPathReloc(u8 selector);
extern s32 overlay1AngleReloc(f32 y, f32 x);
extern s32 overlay1AngleDifferenceReloc(s16 first, s16 second);
extern f32 overlay1TrigXReloc(s32 angle);
extern f32 overlay1TrigYReloc(s32 angle);

/* Plateau: exact 107 words/frame; best is 25 words different, first +0xC.
 * Angle-local order and a split previous-index decrement improve allocation;
 * parameter stack homes and integer/pointer registers remain divergent. */
#ifdef NON_MATCHING
void overlay1BendPathPoint(s16 *x, s16 *y, u8 index, u8 selector) {
    Overlay1PathPoint *next, *previous, *current;
    Overlay1Path *path;
    s16 firstAngle, secondAngle, midpointAngle;
    volatile u8 localIndex;
    s32 nextIndex, previousIndex, currentIndex;

    localIndex = index;
    path = overlay1GetPathReloc(selector);
    index = localIndex;
    current = &path->points[index];
    if (index != 0) {
        currentIndex = index;
        previousIndex = index - 1;
    } else {
        previousIndex = path->count;
        previousIndex--;
        currentIndex = 0;
    }
    previous = &path->points[previousIndex];
    if (currentIndex >= path->count) {
        nextIndex = 0;
    } else {
        nextIndex = currentIndex + 1;
    }
    next = &path->points[nextIndex];
    firstAngle = (s16)(overlay1AngleReloc((f32)(current->y - previous->y),
                                         (f32)(current->x - previous->x)) -
                       0x8000);
    secondAngle = (s16)(overlay1AngleReloc((f32)(current->y - next->y),
                                          (f32)(current->x - next->x)) -
                        0x8000);
    midpointAngle = firstAngle +
        (overlay1AngleDifferenceReloc(firstAngle, secondAngle) >> 1);
    *x = (s16)((f32)*x - overlay1TrigXReloc(midpointAngle) * 50.0f);
    *y = (s16)((f32)*y - overlay1TrigYReloc(midpointAngle) * 50.0f);
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F0007730_1853B10.s")
#endif

/* ---- overlay1AdvancePath ---- */


typedef struct Overlay1Point {
    s16 x;
    s16 y;
} Overlay1Point;

typedef struct Overlay1PathEntry {
    Overlay1Point *points;
} Overlay1PathEntry;

typedef struct Overlay1TraceResult {
    s16 x;
    s16 y;
    u16 base;
    u16 first;
    u16 second;
    u16 secondary;
    s32 changed;
} Overlay1TraceResult;

extern s32 overlay2TracePath(f32 x, f32 y, f32 anchorX, f32 anchorY,
                             void *arg5, Overlay1TraceResult *result,
                             u8 primary, u8 secondary);
extern Overlay1PathEntry *overlay1GetEntry(u16 index);
extern void *overlay1CloneRecord(u32 *source);
extern void overlay1AppendPathPoint(Overlay1PathState *state, s16 x, s16 y,
                                    u8 primary, u8 secondary);
extern s16 overlay1AnchorX;
extern s16 overlay1AnchorY;
extern s32 gOverlay1PoolExhausted;

/* DKR v77/v80, JFG, and the five-reference skeleton scan found no credible
 * donor for this bounded path advance. */
/* Plateau (2026-08-31): the retained clean full-TU candidate is 163 versus
 * 162 target words, with an exact 0x58 frame, 121 positional differences,
 * and first mismatch +0x10. All 119 flags are nonexact; seven O2/MIPS-II
 * rows tie. Removing the widened endpoint carrier is the only strict natural
 * gain and leaves one extra result.x reload. The old 84-word diagnostic used
 * a conflicting wide call prototype and never compiled as a full TU; an
 * explicit call cast lowers indirectly and is inadmissible. Earlier guarded
 * candidates currently shift this symbol +0xFC, so preflight refuses static
 * relocation ownership. A later size-near pass re-proved the one-instruction
 * surplus: narrow and wide carriers add two instructions, a ternary adds
 * three, and operand, line, goto, and reused-local forms are flat. The callee
 * target explicitly sign-extends its two s16 arguments, ruling out a shared
 * wide prototype. Resume only with evidence for an original no-prototype/TU
 * boundary, or allocator evidence for retaining result.x in the argument
 * lane. */
#ifdef NON_MATCHING
s32 overlay1AdvancePath(Overlay1PathState *state) {
    s16 currentX;
    s16 currentY;
    Overlay1PathEntry *entry;
    Overlay1TraceResult result;
    Overlay1PathState *child;
    u8 count;

    count = state->count;
    currentX = state->x[count];
    currentY = state->y[count];

    state->flags = (state->flags & ~3) |
                   (*(u16 *)&state->count & 1);
    if (count >= 31) {
        return 1;
    }

    if (!overlay2TracePath((f32)currentX, (f32)currentY,
                           (f32)overlay1AnchorX, (f32)overlay1AnchorY,
                           (void *)gOverlay1SubmitArg5, &result,
                           state->primary[count], state->secondary[count]) ||
        ((result.x == overlay1AnchorX) && (result.y == overlay1AnchorY))) {
        overlay1AppendPathPoint(state, overlay1AnchorX, overlay1AnchorY, 0xFF, 0);
        return 1;
    }

    entry = overlay1GetEntry(result.secondary);
    if ((currentX != result.x) || (currentY != result.y)) {
        overlay1AppendPathPoint(state, result.x, result.y,
                                *((u8 *)&result + 5), result.secondary);
        if (result.changed != 0) {
            state->flags = (state->flags & ~3) |
                           ((*(u16 *)&state->count | 2) & 3);
        }
    }

    if ((result.base != result.first) && (gOverlay1PoolExhausted == 0)) {
        child = overlay1CloneRecord((u32 *)state);
        if (child != NULL) {
            overlay1AppendPathPoint(child, entry->points[result.first].x,
                                    entry->points[result.first].y,
                                    *((u8 *)&result + 7), result.secondary);
            child->flags = (child->flags & ~3) |
                           ((*(u16 *)&child->count | 2) & 3);
        }
    }

    if ((result.base != result.second) && (gOverlay1PoolExhausted == 0)) {
        child = overlay1CloneRecord((u32 *)state);
        if (child != NULL) {
            overlay1AppendPathPoint(child, entry->points[result.second].x,
                                    entry->points[result.second].y,
                                    *((u8 *)&result + 9), result.secondary);
            child->flags = (child->flags & ~3) |
                           ((*(u16 *)&child->count | 2) & 3);
        }
    }
    return 1;
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F00078DC_1853CBC.s")
#endif

/* ---- overlay1FindBestRecord ---- */


typedef struct Overlay1BestRecord {
    u8 pad00[0xC0];
    union {
        u16 flags;
        struct {
            u8 high;
            u8 type;
        } bytes;
    } header;
    u8 padC2[6];
    u32 value;
} Overlay1BestRecord;

extern Overlay1BestRecord gOverlay1BestRecords[32];
extern s32 gOverlay1SelectedType;

/* Mickey-only reconstruction; pinned DKR v77/v80 and JFG scans have no exact
 * byte donor. Bounded current-layout reproof preserves the exact 30-word,
 * frameless 18/30 basin: all twelve differences are one a1/a3 pool-register
 * exchange, while the temporary lane is exact. All 119 flag configurations
 * were attempted; seven O2/MIPS-II rows tie V0 and none is exact. A
 * fidelity-clean proc-38 allocator trace records ten caller-saved decisions.
 * The selectedType/value declaration swap, block-local value, and record/value
 * declaration-position forms are all byte-identical to V0, exhausting the
 * 122-form cap without a generic batch. Runtime records 884..887 prove both
 * HI16/LO16 pairs and distinct D_220/D_1D88 identities; local caller records
 * 889 and 895 are both in overlay1CreateRecord. Linked range/module/ROM
 * identity still proves fallback only. A historical exact claim rewrote the
 * twelve register fields after compilation and remains prohibited evidence.
 * Resume only with a new natural allocator-order mechanism. Exact promotion
 * must also share the record type with overlay1CreateRecord and
 * metadata-rebind distinct D_220 and D_1D88 identities; neither operation may
 * change candidate instruction fields. */
#ifdef NON_MATCHING
Overlay1BestRecord *overlay1FindBestRecord(void) {
    Overlay1BestRecord *record;
    Overlay1BestRecord *result;
    u32 bestValue;
    register u32 value;
    s32 remaining;
    register s32 selectedType;

    record = gOverlay1BestRecords;
    bestValue = (u32)-1;
    result = NULL;
    selectedType = gOverlay1SelectedType;
    remaining = 31;
    do {
        if (selectedType == ((u32)record->header.bytes.type >> 2)) {
            value = record->value;
            if ((value == 0) ||
                (((record->header.flags & 3) == 3) &&
                 (value < bestValue))) {
                bestValue = value;
                result = record;
            }
        }
        record++;
    } while (remaining--);
    return result;
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_tail/func_overlay_001_F0007B64_1853F44.s")
#endif

/* PLATEAU-HANDOFF:overlay1FindBestRecord:start
 * symbol: overlay1FindBestRecord
 * score: 18/30 words
 * frame: frameless
 * relocations: 4
 * first-mismatch: +0x4
 * summary: 119 flags and three natural declaration/scope forms are exhausted; one a1/a3 allocator bijection remains
 * PLATEAU-HANDOFF:overlay1FindBestRecord:end
 */

/* PLATEAU-HANDOFF:overlay1AppendPathPoint:start
 * symbol: overlay1AppendPathPoint
 * score: 22 differing words
 * frame: 0x28
 * relocations: 8
 * first-mismatch: +0x20
 * summary: 119 flags and six coherent forms exhausted; next lever is source evidence for pool formation or anchor temporary FIFO behavior.
 * PLATEAU-HANDOFF:overlay1AppendPathPoint:end
 */


/* PLATEAU-HANDOFF:overlay1HandleCachedMode:start
 * symbol: overlay1HandleCachedMode
 * score: 23 differing words
 * frame: 0x18
 * relocations: 11
 * first-mismatch: +0x0
 * summary: inverted callback condition removes one instruction; remaining gap is structural clear-flow and relocation identity
 * PLATEAU-HANDOFF:overlay1HandleCachedMode:end
 */

/* PLATEAU-HANDOFF:overlay1ConsumeNearbyPending:start
 * symbol: overlay1ConsumeNearbyPending
 * score: 31 differing words
 * frame: 0x48
 * relocations: 1
 * first-mismatch: +0x40
 * summary: 119 flags and ten coherent forms exhausted; next lever is source-authentic count/object/state allocator mapping, not the TU-wide diagnostic -g3 flag
 * PLATEAU-HANDOFF:overlay1ConsumeNearbyPending:end
 */

/* PLATEAU-HANDOFF:overlay1UpdateRangeFlags:start
 * symbol: overlay1UpdateRangeFlags
 * score: 31 differing words
 * frame: 0x70
 * relocations: 4
 * first-mismatch: +0x34
 * summary: Exact 120-word frame and opcode order; 31 register-only words and four unresolved overlay-local call aliases remain.
 * PLATEAU-HANDOFF:overlay1UpdateRangeFlags:end
 */

/* PLATEAU-HANDOFF:overlay1UpdateAimedTransient:start
 * symbol: overlay1UpdateAimedTransient
 * score: 32 differing words
 * frame: 0x88
 * relocations: 43
 * first-mismatch: +0x0
 * summary: O32 address carrier halves the raw residual; frame, early-load schedule, and five relocation offsets remain after ten forms and 119 flags; no permuter.
 * PLATEAU-HANDOFF:overlay1UpdateAimedTransient:end
 */

/* PLATEAU-HANDOFF:overlay1InitializeGaugeObjects:start
 * symbol: overlay1InitializeGaugeObjects
 * score: 58/74 words
 * frame: 0x60
 * relocations: 3
 * first-mismatch: +0x1C
 * summary: The reported +0x1C is an unresolved call identity; first instruction mismatch +0x30 is the count/object carrier after 119 flags and nine forms.
 * PLATEAU-HANDOFF:overlay1InitializeGaugeObjects:end
 */

/* PLATEAU-HANDOFF:overlay1AdvancePath:start
 * symbol: overlay1AdvancePath
 * score: 121/162 words
 * frame: 0x58
 * relocations: 22
 * first-mismatch: +0x10
 * summary: Size-near pass confirms one extra result.x reload; ten carrier/CFG/line forms are nonexact and the callee proves the s16 ABI
 * PLATEAU-HANDOFF:overlay1AdvancePath:end
 */

/* PLATEAU-HANDOFF:overlay1TransitionState:start
 * symbol: overlay1TransitionState
 * score: 160 differing words
 * frame: 0x50
 * relocations: 13
 * first-mismatch: +0x20
 * summary: The size-near pointer-update probe is byte-flat; promotion trial in=0/out=0 is a schedule-divergence build error, not equality. Retain the 160-word structural plateau.
 * PLATEAU-HANDOFF:overlay1TransitionState:end
 */

/* PLATEAU-HANDOFF:func_overlay_001_F000438C_185076C:start
 * symbol: func_overlay_001_F000438C_185076C
 * score: 1508 differing words
 * frame: 0x230
 * relocations: 65
 * first-mismatch: +0x0
 * summary: Target 0x138 frame versus m2c 0x230; recover typed ABI/stack layout and canonical symbol ownership before further CFG work.
 * PLATEAU-HANDOFF:func_overlay_001_F000438C_185076C:end
 */

/* PLATEAU-HANDOFF:overlay1AssignRecordIndex:start
 * symbol: overlay1AssignRecordIndex
 * score: 11 differing words
 * frame: 0x38
 * relocations: 5
 * first-mismatch: +0x28
 * summary: Exact geometry and frame with five of seven records and a conditional global store awaiting an authentic separate symbolic lvalue
 * PLATEAU-HANDOFF:overlay1AssignRecordIndex:end
 */

/* PLATEAU-HANDOFF:overlay1SolveAngleCandidates:start
 * symbol: overlay1SolveAngleCandidates
 * score: 47/139 words
 * frame: 0xA8
 * relocations: 4
 * first-mismatch: +0x50
 * summary: Two relocation offsets/types align; identities unresolved. -O2 -g3 gives 139 words/86 diffs but changes the TU prologue. Need a natural root-count preheader.
 * PLATEAU-HANDOFF:overlay1SolveAngleCandidates:end
 */

/* PLATEAU-HANDOFF:func_overlay_001_F0003750_184FB30:start
 * symbol: func_overlay_001_F0003750_184FB30
 * score: 105/446 words
 * frame: 0x90
 * relocations: 31
 * first-mismatch: +0xC
 * summary: Direct selector loads remove the redundant path web and improve 360 to 341 masked differences without changing the 447/446 size. Linked trial has zero measured diffs but remains 12 bytes long.
 * PLATEAU-HANDOFF:func_overlay_001_F0003750_184FB30:end
 */

/* PLATEAU-HANDOFF:overlay1BendPathPoint:start
 * symbol: overlay1BendPathPoint
 * score: 82/107 words
 * frame: 0x30
 * relocations: 6
 * first-mismatch: +0xC
 * summary: exact geometry; parameter-home scheduling and three coupled integer/pointer register webs remain after the full flag lattice
 * PLATEAU-HANDOFF:overlay1BendPathPoint:end
 */
