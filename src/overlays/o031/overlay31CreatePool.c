#include "PR/ultratypes.h"

typedef struct Overlay31PoolRecord {
    s32 state[16];
    u8 reserved40[0x64];
    u8 active;
    u8 reservedA5[0x1B];
} Overlay31PoolRecord;

extern void *overlay31AllocateReloc(s32 size, s32 tag);
extern void *overlay31CreateConfig(s32 kind, void *source, s32 width, s32 height,
                                   s32 slotCount);
extern void *D_10;

/* DKR v77/v80 and JFG contain no exact donor for this pool allocator. */
/*
 * Plateau, remeasured 2026-09-12 by lane p9-tight. 11 relocation-masked words
 * at 50/50 and frame 0x38 exact; the declaration order above is load-bearing
 * and is the whole of the frame.
 *
 * The residual is now ONE allocator decision, priced. An instrumented uopt
 * whose object is byte-identical to the configured build records seven p1
 * decisions here: state save 305 -> c1 v0, j 155 -> c2 v1, the literal 15
 * 50 -> c7 t0 (forbidden c1-c6, it spans the four-argument config call),
 * record 40.5 -> c3 a0, i 15.5 -> c4 a1, count 4.33 -> c14 s0, records 0.67
 * -> c8 t1. Forcing record to c4 and i to c5 -- both accepted, forced=4 and
 * forced=5 in the records -- gives 0 masked words at delta 0 against this
 * declaration order. So the function is exactly one colour from a match: the
 * target skips c3 for both webs and nothing in the target's stream uses $a0
 * between the two calls.
 *
 * Decision variable: what puts c3 in record's `forbidden` mask. It is not the
 * ratio -- c3 is on record's offer list at every save measured -- and it is
 * not a call denial, because neither call loads a0 alone; a web spanning the
 * config call is forbidden c3-c6 together, as the literal-15 web shows. So it
 * has to be interference with a pre-coloured a0, and the only pre-coloured a0
 * in the function is the incoming `count`, whose range ends at the size
 * computation four instructions in, in the target as well as here.
 */
#ifdef NON_MATCHING
Overlay31PoolRecord *overlay31CreatePool(s32 count) {
    s32 i;
    s32 j;
    Overlay31PoolRecord *records;
    Overlay31PoolRecord *record;
    s32 *state;

    records = (Overlay31PoolRecord *)overlay31AllocateReloc(
        count * sizeof(Overlay31PoolRecord), 0x8C);
    record = records;

    i = 0;
    if (count > 0) {
        do {
            record->state[15] = 0;
            record->active = 0;
            record->state[0] = 0;
            record->state[1] = 0;
            record->state[2] = 0;

            j = 3;
            state = &record->state[3];
            do {
                j += 4;
                state[1] = 0;
                state[2] = 0;
                state[3] = 0;
                state += 4;
                state[-4] = 0;
            } while (j != 15);
            i++;
            record++;
        } while (i != count);
    }

    D_10 = overlay31CreateConfig(0, 0, 0, 0, count * 15);
    return records;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o031/overlay31CreatePool/func_overlay_031_F0000E7C_188039C.s")
#endif

/* PLATEAU-HANDOFF:overlay31CreatePool:start
 * symbol: overlay31CreatePool
 * score: 11/50 words
 * frame: 0x38
 * relocations: 4
 * first-mismatch: +0x28
 * summary: Frame closed and the residual priced to one allocator decision. Declaring i and j first gives the target's 0x38 frame exactly, which retires the 8-byte gap the previous note could not source from an unused local of any type, and takes 13 masked words to 11 with the aligner's immediate-only bucket going 2 to 0 and byte-exact 37 to 39. The 11 that remain are one colour on two webs. An instrumented uopt whose object is byte-identical to the configured build records seven p1 decisions: state 305 to c1 v0, j 155 to c2 v1, the literal 15 at 50 to c7 t0 forbidden c1 through c6, record 40.5 to c3 a0, i 15.5 to c4 a1, count 4.33 to c14 s0, records 0.67 to c8 t1. Forcing record to c4 and i to c5, both recorded accepted, scores 0 masked at delta 0 on this declaration order, so the function is one colour from a match. Decision variable: what puts c3 in record's forbidden mask. Not the ratio, since c3 is on record's offer list at every measured save; not a call denial, since neither call loads a0 alone and a web spanning the config call is forbidden c3 through c6 together as the literal-15 web shows; so it must be interference with a pre-coloured a0, and the only one is the incoming count, whose range ends four instructions in on both sides. Flat at 11 on this base: all 32 if(1) region placements across five sites, four empty trailing compares to keep a web live, L109 probes on i and count, a header pointer aliasing record->state, a kind local for the config call's four zero arguments, an index form, and a trailing record reset. The empty trailing compare idiom that moved func_8003A754 reaches 9 here but by the wrong mechanism: it forbids c1 to state, so state and j each shift up one as well.
 * PLATEAU-HANDOFF:overlay31CreatePool:end
 */
