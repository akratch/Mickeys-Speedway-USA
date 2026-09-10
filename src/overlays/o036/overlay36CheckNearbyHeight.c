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
extern u8 *gOverlay36WorldStateReloc[];

/* Count nearby objects whose height is outside a 45-unit band around this
 * object's, and latch the world "changed" flag when none are left.
 * PROVENANCE: Mickey-local reconstruction; pinned DKR v77/v80 are negative and
 * JFG's Overlay 36 hits occur only at the unrelated +0x1470/+0x1490 wrappers.
 * No external C is adapted here.
 *
 * 2026-09-10, lane o7-ovl: seven words to three, and the residual is now the
 * frame SIZE and nothing else. Every displacement inside the frame is exact --
 * the results array at sp+60 and the state spill at sp+56 both land on the
 * target's slot -- so the only three differing words are the two `addiu sp`
 * and the incoming-argument home, which is frame+4 and follows from them.
 *
 * The lever was declaration ORDER, an axis no earlier pass swept. cfe lays
 * declared locals from the frame top in declaration order, so a local's home
 * is `frameTop - (bytes declared at or before it)` and the array's base is
 * `frame - 52 - (bytes declared before it)`. All 5,040 orders were compiled:
 * 120 of them score 3, 1,080 score 5, and 3,840 score 7. The 3-word set is
 * exactly the orders with four scalars ahead of the array and `state` sixth,
 * which puts the array at 60 and `state` at 56 while the frame is still 128.
 *
 * That also closes the frame arithmetically. Only six sp displacements are
 * ever touched (16/20 outgoing arguments, 32/36 saves, 56 state, frame+4 the
 * argument home), so every other home is reserved and never read: the frame is
 * pure bookkeeping. It is `round8(40 + block + temps)` with 40 = argument area
 * plus saves, block = 52 + 4 * (scalars declared after the array), and temps
 * the compiler's spill area. The target's 112 needs block + temps = 72, and a
 * seven-name block is 76 on its own. **The target therefore declares at most
 * five scalars beside the array**, and a six-scalar source cannot reach 112 in
 * any order -- which retires the whole ordering axis as a route to the frame
 * and names the next one.
 *
 * Six-name sources bottom out at 120. Dropping any single name frees eight
 * bytes of block but the compiler takes some back as temps, and each drop also
 * moves registers: nearby 20 words, center 11, low 23, nearby+center 22. A
 * five-name source needs temps to be exactly zero. Earlier passes also
 * falsified, all byte-flat or regressing: `register` on all five scratch
 * locals, declaring them in an inner block, an entirely unused extra local
 * (eliminated outright), three top-tested rewrites of the scan loop (frame
 * 128), and every form that drops low/high, which sinks the sub.s/add.s into
 * the loop instead of hoisting them. An explicit `!=` exit test reaches 0x78
 * by dropping the counter but unrolls the loop, adding 224-236 bytes against a
 * zero-delta target. Moving three scratch values out as trailing parameters
 * reaches the exact 112 frame and the exact homes, and fails by exactly two
 * instructions: IDO unconditionally homes a2 and a3 once a function declares
 * three or more parameters, and the target stores only a1 -- which is what
 * proves the target has exactly two parameters. */













#ifdef NON_MATCHING
void func_overlay_036_F0000818_1883CD0(Overlay36Object *object,
                                       s32 remaining) {
    f32 center;
    f32 low;
    f32 high;
    s32 i;
    Overlay36Nearby *results[13];
    Overlay36State *state;
    Overlay36Nearby *nearby;

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
    ((Overlay36WorldState *)gOverlay36WorldStateReloc[0x54])->changed = 1;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o036/overlay36CheckNearbyHeight/func_overlay_036_F0000818_1883CD0.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_036_F0000818_1883CD0:start
 * symbol: func_overlay_036_F0000818_1883CD0
 * score: 60/63 words
 * frame: 0x80
 * relocations: 3
 * first-mismatch: +0x0
 * summary: Declaration order fixes every in-frame displacement; the residual is the frame SIZE alone, and block arithmetic proves a six-scalar source can never reach 0x70.
 * PLATEAU-HANDOFF:func_overlay_036_F0000818_1883CD0:end
 */
