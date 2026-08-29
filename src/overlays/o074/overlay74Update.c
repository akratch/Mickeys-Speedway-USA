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

extern u32 gOverlay74Flags;

/* Runtime identities: func_8005776C, func_800291B4, amSndPlay, func_8003A680.
 * Resident call surfaces authenticate these prototypes. */
s32 overlay74QueryReloc(f32 x, f32 y, s32 z, f32 strength, s32 enabled,
                        Overlay74UpdateObject **results);
void overlay74HitReloc(void);
void overlay74SoundReloc(u16 soundId, void **handle);
void overlay74RewardReloc(s32 count);

/* NON_MATCHING diagnostic (re-reviewed 2026-08-29): retained full-TU and
 * isolated C compile the former policy-invalid two-empty-guard body at 94/100
 * raw and relocation-normalized words, first +0xC, frame 0x60. Five sites swap
 * its result aggregate's address/object web and +0x124 reverses an OR. Current
 * source removes the guards, adopts the authenticated resident ABIs, uses a
 * natural 13-pointer hit array, and uses integer flag/count carriers; this V0
 * is uncompiled, so its score/frame/mechanism are unknown. Its eight
 * offsets/types agree with the retained runtime records,
 * including both gOverlay74Flags pairs resolved through reserved selector
 * 0xFFF/addend 0x4D6E8 to D_800D3128; the assembled target retains only four
 * static call relocations. Exact pinned DKR v77/v80/JFG range scans found no
 * donor; no attributable near-match oracle survives.
 * Historical flag, structural, forced-color, and permuter scores have no
 * surviving attributable variant objects and are scheduling context, not
 * exhaustion proof. Compile ABI-correct V0, then run the 119-flag lattice
 * including V0; reverse the outer OR only if +0x124 remains. Cap 120 unique
 * stock candidates. Linked equality proves fallback only. */
#ifdef NON_MATCHING
void overlay74Update(Overlay74UpdateObject *object, s32 amount) {
    Overlay74UpdateObject *results[13];
    Overlay74UpdateObject *hitObject;
    f32 delta;
    Overlay74UpdateState *state;
    s32 flagBits;
    s32 count;
    s32 mask;

    if (!(object->flags & 0x400)) {
        object->angle += amount << 8;
        state = object->state;
        if (overlay74QueryReloc(object->x, object->y, object->z,
                                (f32)state->strength, 1, results) > 0) {
            hitObject = results[0];
            delta = hitObject->y - object->y;
            if ((hitObject->state->strength == 0) &&
                ((f32)state->minimum < delta) &&
                (delta < (f32)state->maximum)) {
                object->flags |= 0x400;
                *(u16 *)&gOverlay74Flags =
                    (*(u16 *)&gOverlay74Flags & 0xF87F) |
                    ((((((gOverlay74Flags << 5) >> 28) |
                        (1 << state->channel)) << 1) << 6) & 0x780);
                overlay74HitReloc();
                overlay74SoundReloc(0x27C, 0);

                count = 5;
                flagBits = (gOverlay74Flags << 5) >> 28;
                mask = 8;
                do {
                    if (flagBits & mask) {
                        count++;
                    }
                    mask >>= 1;
                } while (mask != 0);
                if (count >= 6) {
                    overlay74RewardReloc(count);
                }
            }
        }
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o074/overlay74Update/func_overlay_074_F00000B8_18CBD58.s")
#endif
