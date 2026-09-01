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
 * Plateau remeasured 2026-08-30: the candidate and target are both 50 words,
 * with 14 raw and 13 relocation-masked differences. The first mismatch is the
 * 0x38-byte target frame versus 0x30; the remaining executable differences are
 * one pointer/counter register family. All four relocation offsets and types
 * align, but the absolute D_10 pair lacks a complete static-to-runtime identity
 * proof. The 119-point flag lattice was neutral. The nearest permitted
 * skeleton remains JFG overlay 26 +0xF1C (0.727), itself GLOBAL_ASM. Natural
 * size-local, prototype, declaration-order, cast, assignment, and indexed-loop
 * spellings did not close the allocator gap; fabricated dead webs were rejected.
 */
#ifdef NON_MATCHING
Overlay31PoolRecord *overlay31CreatePool(s32 count) {
    Overlay31PoolRecord *records;
    Overlay31PoolRecord *record;
    s32 *state;
    s32 i;
    s32 j;

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
 * score: 37/50 words
 * frame: 0x30
 * relocations: 4
 * first-mismatch: +0x0
 * summary: Exact-sibling proxy proof left D_10 and the allocation call unresolved; no authenticated carrier/schedule lever, so source code stayed unchanged.
 * PLATEAU-HANDOFF:overlay31CreatePool:end
 */
