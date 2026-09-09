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

/* Mickey-local reconstruction; pinned DKR v77/v80 are negative and JFG's
 * Overlay 36 hits occur only at the unrelated +0x1470/+0x1490 wrappers. */
/* Workbench verdict: operand mismatch, 56/63 words exact, frame 0x80 versus
 * 0x70; all opcodes and register lanes are exact after the bound source copy.
 * The 119-flag lattice and ten historical source hypotheses are nonexact;
 * removing the nearby-value home reaches only 0x78 and breaks the exact
 * allocation. A fidelity-gated whole-itable frame ladder identifies the
 * address-taken 52-byte results array, the state home live across the call,
 * homes for nearby/i/center/low/high, and the post-decrement temp. Direct
 * nearby access removes its home and reaches 0x78 but changes the integer
 * allocation; direct center access remains 0x80 and changes FP allocation;
 * removing both still stops at 0x78 and combines both regressions. The second
 * aligned eight-byte quantum therefore needs a producer mechanism that keeps
 * the exact integer/FP web topology. A 2026-09-04 declared-local/carrier pass
 * tested ten current-canonical forms: removing the state home, inlining one or
 * both height bounds, direct nearby reads, and reusing the center as a bound.
 * The reduced forms either remain at 0x80 or bottom out at 0x78 while adding
 * structural/register drift; none reaches the target's 0x70 frame with the
 * baseline's exact register lanes. The relocation synthesizer derives the
 * unresolved overlay-data pair consistently as LOCAL value 0x150; assembly
 * fallback stays canonical.
 *
 * 2026-09-10, lane nm-ovlsmall: the residual is exactly the frame and nothing
 * else. All four register lanes are already exact (pool 13/13, temp 7/7,
 * shared 7/7, fp 6/6); the seven words are the two `addiu sp` and five
 * sp-relative displacements. A new instrument measures the layout directly:
 * `cc -g3` keeps the same .text here and emits an .mdebug local table whose
 * entries are each declared local's home, as an offset from the frame top. The
 * frame reads as arg area (24 rounded to 32) + saves (8) + a declared block
 * that ends at the frame top + compiler temps below it. The candidate homes
 * seven locals at 76/72/68/64/60/56/52 -- results, state, nearby, i, center,
 * low, high -- with 12 bytes of temps, so block+temps is 88. The target's
 * frame of 112, with the array at 60 and the state spill at 56, means
 * block+temps must be 72.
 *
 * Declaration surgery bottoms out at 80. Dropping any single name frees eight
 * bytes; dropping more frees nothing, because each value that loses its name
 * becomes a compiler temp and the temps absorb what the block gives up. The
 * one arrangement that does reach 72 moves three of the five scratch values
 * out of the frame entirely, as parameters, whose homes live in the caller's
 * frame: with only `nearby` and `i` left as locals and center/low/high
 * declared as trailing parameters, the frame is exactly 112, the homes are
 * exactly 60/56/52/48, and every instruction matches except two -- IDO
 * unconditionally homes a2 and a3 for any function declaring three or more
 * parameters, and the target stores only a1. The target therefore has exactly
 * two parameters, which falsifies the extra-parameter mechanism while
 * confirming the layout the source has to reach.
 *
 * Newly falsified, all byte-flat: `register` on all five scratch locals;
 * declaring them in an inner block; and an entirely unused extra local, which
 * is eliminated outright. Adding two pure-copy locals raises the frame by
 * exactly eight bytes with identical code, which is how the per-home cost was
 * measured. Three top-tested rewrites of the scan loop leave the frame at 128.
 * The bounds cannot be unnamed: every form that drops `low`/`high` sinks the
 * `sub.s`/`add.s` into the loop instead of hoisting them. */













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
    ((Overlay36WorldState *)gOverlay36WorldStateReloc[0x54])->changed = 1;
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
 * summary: Indexed pointer removes one raw mismatch; frame remains 0x80 vs target 0x70 and needs a new producer topology.
 * PLATEAU-HANDOFF:func_overlay_036_F0000818_1883CD0:end
 */
