#include "PR/ultratypes.h"

typedef struct Overlay41Descriptor {
    u8 reserved00[0x14];
    u16 flags14;
    s8 reserved16[0xBE];
    f32 scaleD4;
} Overlay41Descriptor;

typedef struct Overlay41State {
    u8 reserved00[6];
    s16 flags06;
    u8 reserved08[0x31];
    u8 value39;
    u8 reserved3A[6];
    Overlay41Descriptor *descriptor40;
    u8 reserved44[0x4F];
    u8 selector93;
} Overlay41State;

typedef struct Overlay41Input {
    u8 reserved00[0x0C];
    f32 amount0C;
    u8 reserved10[4];
    u8 negate14;
    u8 reserved15;
    u8 flags16;
    u8 reserved17[0x0D];
    u8 start24;
    u8 end25;
    u8 limit26;
    u8 current27;
} Overlay41Input;

extern Overlay41State *gOverlay41CurrentState;
extern void overlay41ApplyAmount(Overlay41State *state, f32 amount, f32 step);

/* 14/115 differing words, exact 115 instructions, 0x20 frame, size delta 0 (lane p9-mid, 2026-09-12;
 * was 34).  Two edits, both measured:
 *
 *   Hoisting the start read to the top of the block is worth 4.  All 32 orders of the divisor copy
 *   and the start read against the three core statements were measured; start at the top is the
 *   unique optimum and the divisor copy's position is inert.
 *
 *   The redundant 16-bit mask on the flags06 read-modify-write is worth 16.  It is byte-inert --
 *   flags06 is s16, so masking to 0xFFFF before the OR cannot change the stored value -- but it
 *   draws one ugen ring temp, and that one draw is what the tail was missing.  Before it the
 *   register census read a closed ten-cycle t9->t0->t1->...->t8->t9 at 100 percent coherence in a
 *   single window, i.e. every temp from +0x150 on sat exactly one ring position early; after it the
 *   cycle is gone.  This is L127/L129 used as an instrument rather than a description.
 *
 * globalcolor is exonerated here: a 35-cell force sweep (seven p1 webs x the five lowest colours,
 * every acceptance verified from the record's forced field) leaves the object at 30 or worse in
 * every cell on the pre-mask baseline.  The residual was never a colouring decision.
 *
 * What is left is one region, +0xE8..+0x160.  The shipped code emits a real `move` for
 * divisor = limit before the clamp block and then reuses limit's register for the start read after
 * it; this candidate coalesces divisor into limit and therefore needs a second register for start,
 * which it copies.  Twelve spellings of the divisor copy and the value expression -- a second read
 * of input->limit26, a cast, a redundant mask, a self round-trip, a redefinition inside the clamp,
 * a definition in both arms, an explicit delta local, a two-statement split, dividing by limit
 * directly, and three declaration orders -- are all flat at 14 or change the size.  A further
 * 16-toggle greedy subset search over redundant masks elsewhere in the function finds nothing. */
#ifdef NON_MATCHING
void func_overlay_041_F0001298_18885D0(Overlay41Input *input,
                                        Overlay41State *state, s32 step) {
    Overlay41Descriptor *descriptor;
    s32 active;
    s32 value;
    s32 current;
    s32 limit;
    s32 divisor;
    u8 start;

    descriptor = state->descriptor40;
    active = descriptor->scaleD4 != 0.0f;
    if ((active && descriptor->reserved16[state->selector93 + 8] == 0) ||
        (!active && descriptor->reserved16[8] == 0)) {
        if (input->negate14 != 0) {
            overlay41ApplyAmount(state, -input->amount0C, (f32)step);
        } else {
            overlay41ApplyAmount(state, input->amount0C, (f32)step);
        }
    }

    current = input->current27;
    limit = input->limit26;
    if (current < limit) {
        start = input->start24;
        divisor = limit;
        input->current27 = current + step;
        current = input->current27;
        if (current >= limit) {
            input->current27 = limit;
            current = limit & 0xFF;
        }
        value = start + ((input->end25 - start) * current) / divisor;
        state->value39 = value;
        if ((u8)value < 0xFF) {
            state->flags06 = (state->flags06 & 0xFFFF) | 4;
        } else if (!(state->descriptor40->flags14 & 4)) {
            state->flags06 &= ~4;
        }
    } else {
        state->value39 = input->end25;
    }

    if (input->flags16 & 2) {
        gOverlay41CurrentState = state;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o041/overlay41UpdateProgress/func_overlay_041_F0001298_18885D0.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_041_F0001298_18885D0:start
 * symbol: func_overlay_041_F0001298_18885D0
 * score: 14/115 words
 * frame: 0x20
 * relocations: 4
 * first-mismatch: +0x108
 * summary: One redundant 16-bit mask supplies the ring draw the tail was missing; what is left is the unfolded divisor copy.
 * PLATEAU-HANDOFF:func_overlay_041_F0001298_18885D0:end
 */
