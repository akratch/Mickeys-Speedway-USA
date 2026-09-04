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
    s32 z;
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
 * Resident call surfaces authenticate these prototypes. */
s32 overlay74QueryReloc(f32 x, f32 y, s32 z, f32 strength, s32 enabled,
                        Overlay74HitObject **results);
void overlay74HitReloc(void);
void overlay74SoundReloc(u16 soundId, void **handle);
void overlay74RewardReloc(s32 count);

/* NON_MATCHING plateau: Mickey's resident ABI evidence and a bounded producer-
 * lifetime reconstruction recover the exact 100-word schedule, 0x60 frame,
 * stack homes, and integer/FP register webs. The configured C is 99/100 words;
 * its only ordinary difference is the commutative OR at +0x124. A linked
 * promotion trial reports one in-range word and no outside differences. */
#ifdef NON_MATCHING
/* PLATEAU-HANDOFF
 * symbol: overlay74Update
 * score: 99/100 words
 * frame: 0x60
 * relocations: 8
 * first-mismatch: +0x124
 * summary: Exact frame and register webs; only the commutative OR operand order differs after bounded source forms.
 */
void overlay74Update(Overlay74UpdateObject *object, s32 amount) {
    Overlay74HitObject *results[13];
    f32 delta;
    Overlay74UpdateState *state;
    Overlay74HitObject *hitObject;

    if (!(object->flags & 0x400)) {
        object->angle += amount << 8;
        state = object->state;
        if (overlay74QueryReloc(object->x, object->y, object->z,
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
                overlay74HitReloc();
                overlay74SoundReloc(0x27C, 0);

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
                    overlay74RewardReloc((s32)object);
                }
            }
        }
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o074/overlay74Update/func_overlay_074_F00000B8_18CBD58.s")
#endif
