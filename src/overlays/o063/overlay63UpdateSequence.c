#include "PR/ultratypes.h"

extern void *o63PollReloc(s32 arg0);
extern void o63TransitionReloc(void);
extern void o63SetModeReloc(s32 mode);
extern void o63SetColorReloc(s32 red, s32 green, s32 blue, s32 alpha, s32 intensity);
extern void o63DrawReloc(void *resource, s32 x, s32 y, void *state, s32 size);

extern void **gO63Sequence;
extern u8 gO63PendingStart[];
extern void *gO63SequenceStart[];
extern void *gO63PendingState;
extern void *gO63ActiveState;
extern s32 gO63SequenceIndex;
extern s32 gO63Timer;
extern s32 gO63TransitionEnabled;
extern u32 gO63ToggleReloc;
extern u8 gO63GraphicReloc[];

/* Pinned DKR v77/v80 and JFG donor scans classify overlay 63 as none. */
/*
 * The whole deficit is one instruction and the census names it: `move -1`,
 * with every other opcode count equal at the configured flags. It is the
 * target's `move v1,v0` immediately after the poll result's zero test. The
 * target copies the poll result into v1 and then reuses v0 for the sequence
 * pointer it loads next; the candidate leaves the poll result in v0 and takes
 * v1 for the sequence pointer, so no copy is materialised. Everything after
 * that is the same two carriers with their names exchanged, which is why 88
 * of 107 words already align shift-tolerantly and the rest of the diff is
 * relocation immediates and the four-byte branch displacements the missing
 * word causes.
 *
 * Twelve source forms leave it at `move -1`: a named sequence local declared
 * before or after `token` and with or without `register`, that local read
 * before the guard or before the poll call, an explicit `result`/`token`
 * copy pair in either declaration order, the comparison reversed, a cast on
 * the compared value, `*(gO63Sequence + i)` instead of the subscript, an
 * empty then-arm, a goto skip, and a short-circuit split of the guard. The
 * flag lattice is flat too: -mips1 is two words long, -mips3 trades an `lw`
 * for an `lwu`, and -O3 is identical to -O2.
 */
#ifdef NON_MATCHING
void overlay63UpdateSequence(s32 delta) {
    void *token;

    token = o63PollReloc(0);
    if (token != 0) {
        if (gO63Sequence == 0) {
            gO63PendingState = gO63PendingStart;
            gO63Sequence = gO63SequenceStart;
            gO63SequenceIndex = 1;
        } else if (token == gO63Sequence[gO63SequenceIndex]) {
            gO63SequenceIndex++;
            if (gO63Sequence[gO63SequenceIndex] == 0) {
                gO63ToggleReloc = (gO63ToggleReloc ^ 1) & 1;
                gO63Sequence = 0;
                gO63Timer = 0xB4;
                gO63ActiveState = gO63PendingState;
            }
        } else {
            gO63Sequence = 0;
        }
    }

    if (gO63Timer > 0) {
        gO63Timer -= delta;
        if (gO63Timer <= 0 && gO63TransitionEnabled != 0) {
            o63TransitionReloc();
        }
        o63SetModeReloc(2);
        o63SetColorReloc(0, 0, 0, 0xFF, 0xFF);
        o63DrawReloc(gO63GraphicReloc, 0xA0, 0x3C, gO63ActiveState, 0xC);
        o63SetColorReloc(0, 0xFF, 0xFF, 0xFF, 0xFF);
        o63DrawReloc(gO63GraphicReloc, 0xA1, 0x3D, gO63ActiveState, 0xC);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o063/overlay63UpdateSequence/func_overlay_063_F000077C_18C3304.s")
#endif

/* PLATEAU-HANDOFF:overlay63UpdateSequence:start
 * symbol: overlay63UpdateSequence
 * score: 102 differing words
 * frame: 0x20
 * relocations: 39
 * first-mismatch: +0xC
 * summary: Census is exactly `move -1`: the target copies the poll result into v1 and reuses v0 for the sequence pointer, and the candidate exchanges the two carriers and materialises no copy. 88 of 107 words already align. Twelve source forms and the whole flag lattice are flat, so the lever has to make the sequence pointer take v0.
 * PLATEAU-HANDOFF:overlay63UpdateSequence:end
 */
