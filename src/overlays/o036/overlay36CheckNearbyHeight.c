#include "PR/ultratypes.h"

typedef struct Overlay36State {
    u8 pad0[4];
    u16 active;
} Overlay36State;

typedef struct Overlay36Object {
    u8 pad0[6];
    s16 flags;
    u8 pad8[4];
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[0x4C];
    Overlay36State *state;
} Overlay36Object;

typedef struct Overlay36Nearby {
    u8 pad0[0x10];
    f32 y;
} Overlay36Nearby;

typedef struct Overlay36WorldState {
    u8 pad0[0xA];
    u8 changed;
} Overlay36WorldState;

extern s32 func_8005776C(f32, f32, f32, f32, s32,
                         Overlay36Nearby **);
extern Overlay36WorldState *gOverlay36WorldStateReloc;

/* Mickey-local reconstruction; pinned DKR v77/v80 are negative and JFG's
 * Overlay 36 hits occur only at the unrelated +0x1470/+0x1490 wrappers. */
/* Workbench verdict: operand mismatch, 56/63 words exact, frame 0x80 versus
 * 0x70; all opcodes and register lanes are exact after the bound source copy.
 * The 119-flag lattice and ten historical source hypotheses are nonexact;
 * removing the nearby-value home reaches only 0x78 and breaks the exact
 * allocation. A later fidelity-gated UOPT trace reproduced all 63 instruction
 * words and all three relocations under the traced static compiler, then found
 * eleven naturally colored webs but no producer-emitted final stack-home
 * evidence. Pointer/FP register qualifiers and block-scoping the post-call
 * locals are byte-identical to this 0x80-frame V0. The relocation synthesizer
 * derives the unresolved overlay-data pair consistently as LOCAL value 0x150;
 * assembly fallback stays canonical. */













#ifdef NON_MATCHING
void func_overlay_036_F0000818_1883CD0(Overlay36Object *object,
                                       s32 remaining) {
    Overlay36Nearby *results[13];
    Overlay36State *state;
    Overlay36Nearby *nearby;
    s32 i;
    f32 center;
    f32 low;
    f32 high;

    state = object->state;
    if (state->active == 0) {
        remaining = func_8005776C(object->x, object->y, object->z,
                                  64.0f, 1, results);
        if (remaining != 0) {
            high = 45.0f;
            center = object->y;
            i = remaining - 1;
            low = center - 45.0f;
            high = low;
            low = center + 45.0f;
            if (remaining != 0) {
                do {
                    nearby = results[i];
                    center = nearby->y;
                    if ((center < high) || (low < center)) {
                        remaining--;
                    }
                } while (i--);
            }
        }
        if (remaining != 0) {
            state->active = 1;
        } else {
            object->flags &= ~0x400;
            gOverlay36WorldStateReloc->changed = 1;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o036/overlay36CheckNearbyHeight/func_overlay_036_F0000818_1883CD0.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_036_F0000818_1883CD0:start
 * symbol: func_overlay_036_F0000818_1883CD0
 * score: 56/63 words
 * frame: 0x80
 * relocations: 3
 * first-mismatch: +0x0
 * summary: Fidelity-gated UOPT leaves final stack-home ownership unresolved; three authentic home-shaping forms are flat
 * PLATEAU-HANDOFF:func_overlay_036_F0000818_1883CD0:end
 */
