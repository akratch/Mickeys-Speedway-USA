#include "PR/ultratypes.h"

typedef struct Overlay79Vector {
    f32 x;
    f32 y;
    f32 z;
} Overlay79Vector;

typedef struct Overlay79Object Overlay79Object;
typedef struct Overlay79MotionState Overlay79MotionState;

struct Overlay79MotionState {
    /* 0x00 */ s16 targetAngle;
    /* 0x02 */ s8 mode;
    /* 0x03 */ s8 active;
    /* 0x04 */ s16 event;
    /* 0x06 */ s16 step;
    /* 0x08 */ u32 collisionFlags;
    /* 0x0C */ s32 effectTimer;
    /* 0x10 */ f32 targetX;
    /* 0x14 */ f32 targetZ;
    /* 0x18 */ f32 acceleration;
    /* 0x1C */ f32 speed;
    /* 0x20 */ f32 heightOffset;
    /* 0x24 */ f32 homeX;
    /* 0x28 */ f32 homeY;
    /* 0x2C */ f32 homeZ;
    /* 0x30 */ f32 radiusSquared;
    /* 0x34 */ f32 travelRadius;
    /* 0x38 */ f32 previousHeight;
    /* 0x3C */ void *effect;
    /* 0x40 */ Overlay79Object *target;
};

struct Overlay79Object {
    /* 0x00 */ s16 angle;
    u8 pad02[6];
    /* 0x08 */ f32 scale;
    /* 0x0C */ f32 x;
    /* 0x10 */ f32 y;
    /* 0x14 */ f32 z;
    u8 pad18[4];
    /* 0x1C */ f32 velocityX;
    /* 0x20 */ f32 velocityY;
    /* 0x24 */ f32 velocityZ;
    /* 0x28 */ f32 floorHeight;
    u8 pad2C[0xF];
    /* 0x3B */ s8 mode;
    /* 0x3C */ s32 field3C;
    u8 pad40[0x24];
    /* 0x64 */ Overlay79MotionState *state;
    u8 pad68[0x18];
    /* 0x80 */ u32 flags;
};

typedef struct Overlay79SpawnDesc {
    /* 0x00 */ s16 objectId;
    /* 0x02 */ u8 kind;
    /* 0x03 */ u8 flags;
    /* 0x04 */ s16 x;
    /* 0x06 */ s16 y;
    /* 0x08 */ s16 z;
    /* 0x0A */ s16 angle;
    /* 0x0C */ Overlay79Object *parent;
    /* 0x10 */ f32 scale;
    u8 pad14[4];
} Overlay79SpawnDesc;

/*
 * Overlay 79's initialized image (ROM 0x18CE480..0x18CE4E0, module offsets
 * +0x14E0..+0x1540).  The shipped relocation table splits it at +0x1500: the
 * first eight words are one runtime-indexed table (LOCAL records patch a base
 * of +0x14E0 with a zero addend and a scaled `object->mode`), and the sixteen
 * words after it are the constant pool the module's code loads with LOCAL
 * records whose base is +0x1500 and whose addends run +0x4..+0x2C.  Tier A for
 * the split and for the addends; the byte values are unchanged from the
 * previously adopted image, so the module's data bytes are identical.
 */
f32 gOverlay79ModeFactors[8] = {
    0.04f, 0.01f, 0.02f, 0.1f, 0.01f, 0.0f, 0.0f, 0.0f,
};

f32 gOverlay79Constants[16] = {
    0.05f,  -0.2f, 0.2f, 0.9f, -0.1f, 0.707f, 0.1f, 0.9f,
    0.46f,  0.9f,  -0.1f, -0.2f, 0.707f, 0.1f, 0.01f, 0.01f,
};

/*
 * The module reaches its own data through the runtime linker rather than
 * through a link-time address, so the adopted C spells each base as the raw
 * stored addend the shipped record carries (`docs/reloc-surface.md`).
 * `gOverlay79FlagsReloc` is the reserved-BSS selector 0xFFF entry at
 * +0x4D6E8 that `func_overlay_079_F0001290_18CE230` already names; ROM-table
 * index 1579 covers both uses, so this is the same object (Tier A).
 */
extern u8 gOverlay79FlagsReloc[];
extern f32 gOverlay79ModeFactorsReloc[];
extern f32 gOverlay79ConstantsReloc[];
extern u8 D_FA0[];

#define O79_ACTIVE_ACCELERATION   gOverlay79ConstantsReloc[1]
#define O79_INACTIVE_ACCELERATION gOverlay79ConstantsReloc[2]
#define O79_TURN_POWER            gOverlay79ConstantsReloc[3]
#define O79_FORWARD_ACCELERATION  gOverlay79ConstantsReloc[4]
#define O79_TARGET_DOT            gOverlay79ConstantsReloc[5]
#define O79_BRAKE_ACCELERATION    gOverlay79ConstantsReloc[6]
#define O79_APPROACH_POWER        gOverlay79ConstantsReloc[7]
#define O79_LAUNCH_HEIGHT         gOverlay79ConstantsReloc[8]
#define O79_TURN_IN_POWER         gOverlay79ConstantsReloc[9]
#define O79_GRAVITY_HALF          gOverlay79ConstantsReloc[10]
#define O79_GRAVITY               gOverlay79ConstantsReloc[11]

extern Overlay79Object *overlay79FindNearby(Overlay79Vector *position,
                                            f32 radiusSquared);
extern void func_8005AD64(Overlay79Object *object, s32 mode, s32 index,
                          f32 value);
extern f32 sqrtf(f32 value);
extern void func_800031E8(void *handle);
extern s32 mathRnd(s32 lower, s32 upper);
extern void func_80002FE0(u16 id, f32 x, f32 y, f32 z, s32 priority,
                          void **handle);
extern s32 Arctanf(f32 y, f32 x);
extern f32 Powerf(f32 value, s32 exponent);
extern s16 dAngle(s16 current, s16 target, f32 fraction);
extern void mathOneFloatRPY(Overlay79Object *object, Overlay79Vector *vector);
extern f32 func_8002A8C0(s32 angle);
extern f32 func_8002A8BC(s32 angle);
extern Overlay79Object *func_8000590C(Overlay79SpawnDesc *desc, s32 count);
extern void func_80008128(Overlay79Object *object, f32 x, f32 y, f32 z);
extern s32 func_8005ABA8(Overlay79Object *object, f32 factor, f32 updateRate);
extern void func_800031C0(void *handle, f32 x, f32 y, f32 z);
extern void partUpdateTriggers(Overlay79Object *object, s32 updateRate);
extern void trackMakePolylist(s32 mode, Overlay79Vector *start,
                              Overlay79Vector *end, f32 *height, void *unused,
                              s32 flags);
extern s32 func_80010900(Overlay79Vector *start, Overlay79Vector *end,
                         f32 height, Overlay79Object *object, void *callback);

/*
 * Plateau (2026-09-08).  The body below is the shipped relocation surface's
 * own structure: 883 candidate instructions against 882, frame 0xB8 exact,
 * 574/882 positional words, first differing word +0x58.  The candidate's 88
 * static relocations match the module's 88 shipped records in count and type
 * histogram (58 R_MIPS_26, 15 HI16, 15 LO16) and in per-callee multiplicity;
 * 79/88 also agree by offset, and the nine that do not all sit past +0xBC0
 * and are displaced by exactly one instruction.
 *
 * Two residuals remain, both allocator-phase rather than structural:
 *
 *   1. One extra instruction.  At the displacement call the target retires
 *      the third component's spill in the branch delay slot; the candidate
 *      emits it ahead of the branch and pads the slot.  Every source spelling
 *      of that statement group tried (assign all three, assign two and pass
 *      the third as an expression, reorder the assignments, or drop the
 *      locals and let common-subexpression elimination re-derive them) gives
 *      the identical object, so the choice is made below the C level.
 *
 *   2. A one-step phase offset in both register rings.  The integer ring
 *      first diverges at +0x58 and every later integer difference is the
 *      single uniform rotation of that one step; the FP ring rotates over
 *      five registers from the mode-0 block onward.  The stack-home census is
 *      otherwise exact: every frame slot the target uses is used at the same
 *      offset here except the race-state flag, which sits one word high
 *      because IDO reserves one more spill word for this body than for the
 *      target's.
 *
 * Next lever: find the source shape that makes IDO reserve one fewer spill
 * word (which lands the last frame slot and, with it, the integer ring
 * phase).  Ruled out with measurement: the flag lattice (-O1/-O2 x
 * -mips1/-mips2 x -g3 all give 874 instructions and frame 0xB8, i.e. flat),
 * bitfield spellings of the race-state test, hoisting the flag word into a
 * local, splitting the test into nested ifs, and every displacement-call
 * spelling above.
 */
#ifdef NON_MATCHING
void func_overlay_079_F0000134_18CD0D4(Overlay79Object *object,
                                       s32 updateRate) {
    Overlay79MotionState *state;
    Overlay79Object *nearby;
    Overlay79Object *spawned;
    Overlay79SpawnDesc desc;
    Overlay79Vector start;
    Overlay79Vector end;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distance;
    f32 dot;
    f32 update;
    f32 range;
    f32 factor;
    Overlay79Vector forward;
    s8 mode;
    s32 delta;
    s32 angle;
    s32 raceActive;

    state = object->state;
    update = updateRate;
    if (((*(u16 *)&gOverlay79FlagsReloc[0xE] & 0x1C0) >> 6) >= 3 &&
        (((*(u32 *)&gOverlay79FlagsReloc[0] << 5) >> 28) == 0xF) &&
        !(*(u32 *)&gOverlay79FlagsReloc[0] & 0x40000)) {
        raceActive = 1;
    } else {
        raceActive = 0;
    }

    nearby = overlay79FindNearby((Overlay79Vector *)&object->x, 22500.0f);
    if (nearby != NULL) {
        state->active = 1;
    }

    if (state->active != 0) {
        if (state->collisionFlags & 2) {
            if (object->mode != 0) {
                func_8005AD64(object, 0, -1, 0.0f);
            }
        } else if (((state->collisionFlags & 2) == 0) &&
                   (object->mode != 3)) {
            func_8005AD64(object, 3, -1, 0.0f);
        }

        if (nearby != NULL) {
            if (state->speed > -25.0f) {
                state->acceleration = O79_ACTIVE_ACCELERATION;
            } else {
                state->acceleration = 0.0f;
            }
            if (nearby != NULL) {
                dx = nearby->x - object->x;
                dz = nearby->z - object->z;
                if ((sqrtf((dx * dx) + (dz * dz)) < 70.0f) &&
                    (state->collisionFlags & 2)) {
                    object->velocityY = 5.0f;
                    if (state->effect != NULL) {
                        func_800031E8(state->effect);
                    }
                    func_80002FE0(mathRnd(0x21F, 0x226), object->x, object->y,
                                  object->z, 4, &state->effect);
                    state->effectTimer = mathRnd(0x78, 0xF0);
                }
            }
        } else if (state->speed < 0.0f) {
            state->acceleration = O79_INACTIVE_ACCELERATION;
        } else {
            state->active = 0;
            state->step = 0;
            state->mode = 2;
            state->acceleration = 0.0f;
            state->speed = 0.0f;
        }

        dx = object->x - state->homeX;
        dz = object->z - state->homeZ;
        if ((state->radiusSquared <= ((dx * dx) + (dz * dz))) ||
            (nearby == NULL)) {
            state->targetAngle = Arctanf(dx, dz);
        } else {
            state->targetAngle = Arctanf(nearby->x - object->x,
                                         nearby->z - object->z);
        }
        object->angle = dAngle(object->angle, state->targetAngle,
                               1.0f - Powerf(O79_TURN_POWER, updateRate));
        delta = (object->angle - state->targetAngle) & 0xFFFF;
        if (((state->targetAngle - object->angle) & 0xFFFF) < delta) {
            delta = -((state->targetAngle - object->angle) & 0xFFFF);
        }
        if ((delta >= -99) && (delta < 100)) {
            object->angle = state->targetAngle;
        }
        if (state->target != NULL) {
            dx = state->target->x - object->x;
            dy = state->target->y - object->y;
            dz = state->target->z - object->z;
            if (sqrtf((dx * dx) + (dy * dy) + (dz * dz)) < 30.0f) {
                *(s32 *)state->target->state = 1;
            }
        }
        if (state->effectTimer != 0) {
            state->effectTimer -= updateRate;
            if (state->effectTimer < 0) {
                state->effectTimer = 0;
            }
        }
        if ((state->effect == NULL) && (state->effectTimer == 0)) {
            func_80002FE0(mathRnd(0x227, 0x229), object->x, object->y,
                          object->z, 4, &state->effect);
            state->effectTimer = mathRnd(0x78, 0xF0);
        }
    } else {
        mode = state->mode;
        if (mode == 0) {
            if (state->speed > -1.0f) {
                state->acceleration = O79_FORWARD_ACCELERATION;
            } else {
                state->acceleration = 0.0f;
            }
            forward.x = 0.0f;
            forward.y = 0.0f;
            forward.z = -1.0f;
            mathOneFloatRPY(object, &forward);
            dx = state->targetX - object->x;
            dz = state->targetZ - object->z;
            dot = (forward.z * dz) + (dx * forward.x);
            if ((dot < 0.0f) || (state->collisionFlags & 4)) {
                state->mode = 1;
            } else if (state->target != NULL) {
                dx = object->x - state->target->x;
                dz = object->z - state->target->z;
                distance = sqrtf((dx * dx) + (dz * dz));
                forward.x = 0.0f;
                forward.y = 0.0f;
                forward.z = -1.0f;
                mathOneFloatRPY(object, &forward);
                dx = state->target->x - object->x;
                dz = state->target->z - object->z;
                dot = (forward.z * dz) + (dx * forward.x);
                if ((distance < 50.0f) && (O79_TARGET_DOT < dot)) {
                    state->mode = 1;
                }
            }
            if (state->effectTimer != 0) {
                state->effectTimer -= updateRate;
                if (state->effectTimer < 0) {
                    state->effectTimer = 0;
                }
            }
            if ((state->effect == NULL) && (state->effectTimer == 0)) {
                func_80002FE0(mathRnd(0x21B, 0x21E), object->x, object->y,
                              object->z, 4, &state->effect);
                state->effectTimer = mathRnd(0x78, 0xF0);
            }
        } else if (mode == 1) {
            if (state->speed < 0.0f) {
                state->acceleration = O79_BRAKE_ACCELERATION;
            } else {
                state->acceleration = 0.0f;
                state->speed = 0.0f;
                state->mode = (state->target == NULL && raceActive != 0)
                                  ? 3
                                  : 2;
            }
        } else if (mode == 2) {
            if (state->step == 0) {
                func_8005AD64(object, 1, -1, 0.0f);
                state->step++;
            } else if ((state->step == 1) && (state->event != 0)) {
                if (state->effect == NULL) {
                    func_80002FE0(mathRnd(0x218, 0x21A), object->x, object->y,
                                  object->z, 4, &state->effect);
                }
                func_8005AD64(object, 2, -1, 0.0f);
                state->step++;
            } else if ((state->step == 2) && (state->event != 0)) {
                func_8005AD64(object, 1, -1, 0.0f);
                state->step++;
            } else if ((state->step == 3) && (state->event != 0)) {
                func_8005AD64(object, 0, -1, 0.0f);
                state->step = 0;
                angle = mathRnd(-0x7FFF, 0x8000);
                range = mathRnd(0, (s32)state->travelRadius);
                state->targetX = state->homeX - (func_8002A8C0(angle) * range);
                state->targetZ = state->homeZ - (func_8002A8BC(angle) * range);
                state->targetAngle = Arctanf(object->x - state->targetX,
                                             object->z - state->targetZ);
                state->mode = 4;
            }
        } else if (mode == 3) {
            if (state->step == 0) {
                angle = mathRnd(-0x7FFF, 0x8000);
                range = mathRnd(0, (s32)state->travelRadius);
                state->targetX = state->homeX - (func_8002A8C0(angle) * range);
                state->targetZ = state->homeZ - (func_8002A8BC(angle) * range);
                state->targetAngle = Arctanf(object->x - state->targetX,
                                             object->z - state->targetZ);
                state->step = 1;
            } else if (state->step == 1) {
                object->angle = dAngle(object->angle, state->targetAngle,
                                       1.0f - Powerf(O79_APPROACH_POWER,
                                                     updateRate));
                delta = (object->angle - state->targetAngle) & 0xFFFF;
                if (((state->targetAngle - object->angle) & 0xFFFF) < delta) {
                    delta = -((state->targetAngle - object->angle) & 0xFFFF);
                }
                if ((delta >= -99) && (delta < 100)) {
                    object->angle = state->targetAngle;
                    state->step = 2;
                }
            } else if (state->step == 2) {
                func_8005AD64(object, 4, -1, 0.0f);
                if (state->effect == NULL) {
                    func_80002FE0(mathRnd(0x270, 0x272), object->x, object->y,
                                  object->z, 4, &state->effect);
                }
                state->step = 3;
            } else if ((state->step == 3) &&
                       (state->previousHeight < O79_LAUNCH_HEIGHT) &&
                       (O79_LAUNCH_HEIGHT <= object->floorHeight)) {
                desc.objectId = 0x144;
                desc.kind = 0x14;
                desc.flags = 0;
                desc.x = (func_8002A8C0(object->angle) * 18.0f) + object->x;
                desc.y = object->y;
                desc.z = (func_8002A8BC(object->angle) * 18.0f) + object->z;
                desc.parent = object;
                desc.scale = object->scale;
                spawned = func_8000590C(&desc, 1);
                if (spawned != NULL) {
                    spawned->field3C = 0;
                    state->target = spawned;
                }
                func_80002FE0(0x273, object->x, object->y, object->z, 4, NULL);
                state->step = 4;
            } else if ((state->step == 4) && (state->event != 0)) {
                func_8005AD64(object, 0, -1, 0.0f);
                state->step = 0;
                state->mode = 0;
            }
        } else if (mode == 4) {
            object->angle = dAngle(object->angle, state->targetAngle,
                                   1.0f - Powerf(O79_TURN_IN_POWER,
                                                 updateRate));
            delta = (object->angle - state->targetAngle) & 0xFFFF;
            if (((state->targetAngle - object->angle) & 0xFFFF) < delta) {
                delta = -((state->targetAngle - object->angle) & 0xFFFF);
            }
            if ((delta >= -99) && (delta < 100)) {
                object->angle = state->targetAngle;
                state->mode = 0;
            }
        }
    }

    distance = (state->speed * update) +
               (0.5f * state->acceleration * update * update);
    state->speed += state->acceleration * update;
    dy = (object->velocityY * update) +
         (O79_GRAVITY_HALF * update * update);
    object->velocityY += O79_GRAVITY * update;
    dx = func_8002A8C0(object->angle) * distance;
    dz = func_8002A8BC(object->angle) * distance;

    start.x = object->x;
    start.y = object->y + state->heightOffset;
    start.z = object->z;
    end.x = start.x + dx;
    end.y = start.y + dy;
    end.z = start.z + dz;
    state->collisionFlags = 0;
    trackMakePolylist(1, &start, &end, &state->heightOffset, NULL, 1);
    if (func_80010900(&start, &end, state->heightOffset, object, D_FA0) != 0) {
        object->velocityY = 0.0f;
    }
    dx = end.x - start.x;
    dy = end.y - start.y;
    dz = end.z - start.z;
    func_80008128(object, dx, dy, dz);
    object->velocityX = dx / update;
    object->velocityZ = dz / update;
    if (object->mode == 0) {
        factor = gOverlay79ModeFactorsReloc[object->mode] * -state->speed;
    } else {
        factor = gOverlay79ModeFactorsReloc[object->mode];
    }
    state->previousHeight = object->floorHeight;
    state->event = func_8005ABA8(object, factor, update);
    if (state->effect != NULL) {
        func_800031C0(state->effect, object->x, object->y, object->z);
    }
    if (object->mode == 3) {
        object->flags |= 1;
        partUpdateTriggers(object, updateRate);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o079/func_overlay_079_F0000134_18CD0D4/func_overlay_079_F0000134_18CD0D4.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_079_F0000134_18CD0D4:start
 * symbol: func_overlay_079_F0000134_18CD0D4
 * score: 574/882 words
 * frame: 0xB8
 * relocations: 88
 * first-mismatch: +0x58
 * summary: 883 vs 882 instructions, frame exact, all 88 relocations match by count/type/callee count; residual is one delay-slot spill and a one-step register-ring phase
 * PLATEAU-HANDOFF:func_overlay_079_F0000134_18CD0D4:end
 */
