#include "PR/ultratypes.h"

typedef struct Overlay74UpdateState {
    u8 strength;
    u8 channel;
    s8 minimum;
    s8 maximum;
} Overlay74UpdateState;

typedef struct Overlay74HitState {
    s8 strength;
} Overlay74HitState;

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

typedef struct Overlay74QueryResult {
    Overlay74UpdateObject *object;
    u8 pad04[0x30];
} Overlay74QueryResult;

extern u32 gOverlay74Flags;

/* Runtime identities: func_8005776C, func_800291B4, amSndPlay, func_8003A680.
 * The first two ABIs need a bounded configured probe before these declarations
 * can adopt the resident prototypes. */
Overlay74UpdateObject *overlay74QueryReloc(f32 x, f32 y, s32 z, f32 strength,
                                           s32 enabled,
                                           Overlay74QueryResult *result);
void overlay74HitReloc(Overlay74UpdateObject *object);
void overlay74SoundReloc(s32 soundId, s32 arg1);
void overlay74RewardReloc(s32 count);

/* NON_MATCHING diagnostic (re-reviewed 2026-08-29): retained full-TU and
 * isolated C agree on the former empty-guard candidate at six raw and
 * six relocation-normalized words at +0xC,+0x68,+0x84,+0x8C,+0x90,+0x124,
 * while preserving the 0x60 frame. Five words swap the result aggregate's
 * address/object register pair, and the last OR uses the opposite commutative
 * encoding. Its eight offsets/types
 * agree with the runtime records,
 * including both gOverlay74Flags pairs resolved through reserved selector
 * 0xFFF/addend 0x4D6E8 to D_800D3128; the assembled target retains only four
 * static call relocations. Pinned DKR v77/v80 and JFG scans found no donor.
 * Historical flag, structural, forced-color, and permuter scores have no
 * surviving attributable variant objects and are scheduling context, not
 * exhaustion proof. The invented empty guards are now removed. Compile a
 * source-faithful V0, probe both resident ABIs, retain the 119-flag lattice,
 * then reverse the outer OR once. Linked C proof does not survive. */
#ifdef NON_MATCHING
void overlay74Update(Overlay74UpdateObject *object, s32 amount) {
    Overlay74QueryResult result;
    f32 delta;
    Overlay74UpdateState *state;
    s32 mask;

    if (!(object->flags & 0x400)) {
        object->angle += amount << 8;
        state = object->state;
        if (overlay74QueryReloc(object->x, object->y, object->z,
                                (f32)state->strength, 1, &result)) {
            amount = (s32)result.object->state;
            delta = result.object->y - object->y;
            if ((((Overlay74HitState *)amount)->strength == 0) &&
                ((f32)state->minimum < delta) &&
                (delta < (f32)state->maximum)) {
                object->flags |= 0x400;
                *(u16 *)&gOverlay74Flags =
                    (*(u16 *)&gOverlay74Flags & 0xF87F) |
                    ((((((gOverlay74Flags << 5) >> 28) |
                        (1 << state->channel)) << 1) << 6) & 0x780);
                overlay74HitReloc(object);
                overlay74SoundReloc(0x27C, 0);

                object = (Overlay74UpdateObject *)5;
                state = (Overlay74UpdateState *)((gOverlay74Flags << 5) >> 28);
                mask = 8;
                do {
                    if ((s32)state & mask) {
                        object = (Overlay74UpdateObject *)((s32)object + 1);
                    }
                    mask >>= 1;
                } while (mask != 0);
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
