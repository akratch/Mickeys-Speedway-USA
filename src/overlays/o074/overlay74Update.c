#include "PR/ultratypes.h"

typedef struct Overlay74UpdateState {
    u8 strength;
    u8 channel;
    s8 minimum;
    s8 maximum;
} Overlay74UpdateState;

typedef struct Overlay74UpdateObject {
    s16 angle;
    u8 pad02[4];
    s16 flags;
    u8 pad08[4];
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[0x4C];
    Overlay74UpdateState *state;
} Overlay74UpdateObject;

typedef struct Overlay74HitState {
    s8 strength;
} Overlay74HitState;

typedef struct Overlay74HitObject {
    u8 pad00[0x10];
    f32 y;
    u8 pad14[0x50];
    Overlay74HitState *state;
} Overlay74HitObject;

extern u32 gOverlay74Flags;

/* Runtime identities: func_8005776C, func_800291B4, amSndPlay, func_8003A680.
 * Resident ROM and matched callers authenticate the floating-point z ABI. */
s32 func_8005776C(f32 x, f32 y, f32 z, f32 strength, s32 enabled,
                        Overlay74HitObject **results);
void func_800291B4(void);
void amSndPlay(u16 soundId, void **handle);
void func_8003A680(s32 count);

/* NON_MATCHING plateau: configured C is 99/100 words with a 0x60 frame and
 * eight relocations; the sole residual is the commutative OR at +0x124. All
 * register lanes match; CFE spelling owns the residual. */
#ifdef NON_MATCHING
void overlay74Update(Overlay74UpdateObject *object, s32 amount) {
    Overlay74HitObject *results[13];
    f32 delta;
    Overlay74UpdateState *state;
    Overlay74HitObject *hitObject;

    if (!(object->flags & 0x400)) {
        object->angle += amount << 8;
        state = object->state;
        if (func_8005776C(object->x, object->y, object->z,
                                (f32)state->strength, 1, results) != 0) {
            hitObject = results[0];
            amount = (s32)hitObject->state;
            delta = hitObject->y - object->y;
            if ((((Overlay74HitState *)amount)->strength == 0) &&
                ((f32)state->minimum < delta) &&
                (delta < (f32)state->maximum)) {
                object->flags |= 0x400;
                *(u16 *)&gOverlay74Flags =
                    ((((((gOverlay74Flags << 5) >> 28) |
                        (1 << state->channel)) << 1) << 6) & 0x780) |
                    (*(u16 *)&gOverlay74Flags & 0xF87F);
                func_800291B4();
                amSndPlay(0x27C, 0);

                object = (Overlay74UpdateObject *)5;
                state = (Overlay74UpdateState *)((gOverlay74Flags << 5) >> 28);
                hitObject = (Overlay74HitObject *)8;
                do {
                    if ((s32)state & (s32)hitObject) {
                        object = (Overlay74UpdateObject *)((s32)object + 1);
                    }
                    hitObject = (Overlay74HitObject *)((s32)hitObject >> 1);
                } while (hitObject != NULL);
                if ((s32)object >= 6) {
                    func_8003A680((s32)object);
                }
            }
        }
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o074/overlay74Update/func_overlay_074_F00000B8_18CBD58.s")
#endif

/* PLATEAU-HANDOFF:overlay74Update:start
 * symbol: overlay74Update
 * score: 99/100 words
 * frame: 0x60
 * relocations: 8
 * first-mismatch: +0x124
 * summary: All register lanes match; CFE spelling owns the OR residual. Next lever: a new C spelling reversing OR without operand swap or named carrier.
 * PLATEAU-HANDOFF:overlay74Update:end
 */
