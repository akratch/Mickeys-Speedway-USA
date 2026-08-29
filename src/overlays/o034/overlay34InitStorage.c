#include "PR/ultratypes.h"

typedef struct Overlay34Record {
    u8 bytes[0x68];
} Overlay34Record;

extern Overlay34Record *gOverlay34Records;
extern s32 **gOverlay34Pointers;
extern s32 gOverlay34Count;
extern void *func_8002B280(s32 size, s32 tag);

/* The donor ledger classifies pinned DKR v77/v80 and JFG candidates negative. */
/*
 * Plateau: exact 0xC8 size and 0x30 frame, 46/50 relocation-normalized words,
 * first mismatch +0x24. The surviving isolated ranking has a fifth raw
 * difference at +0xC4 because its assembled target bakes the local-data +8
 * addend; the candidate and runtime table require eight relocation sites.
 * The target assigns the reused size spill to sp+0x1C; IDO assigns this body
 * to sp+0x18. Historical declaration, lifetime, type, flag, trace, and
 * permutation outcomes have no surviving attributable result set and do not
 * prove exhaustion. Preserve a fresh unchanged V0 and runtime-annotated
 * target, retain the complete 119-recipe flag lattice, then take one
 * CFE/UOPT/UGEN trace. The smallest established mechanism to test, only when
 * trace-supported, is an unused volatile word declared immediately after
 * `size`: descending declaration-home allocation may consume sp+0x18 and move
 * `size` to sp+0x1C. Stop if the trace exposes no lever or that one variant
 * changes anything beyond the four home offsets. The F route is provisional;
 * park as W if no source-visible lever survives.
 */
#ifdef NON_MATCHING
void overlay34InitStorage(s32 count) {
    s32 *word;
    s32 countdown;
    s32 remaining;
    s32 size;

    size = count * sizeof(Overlay34Record);
    gOverlay34Records = func_8002B280(size, 0x87);
    word = (s32 *)gOverlay34Records;
    remaining = size >> 2;
    countdown = remaining - 1;
    if (remaining != 0) {
        do {
            *word++ = 0;
        } while (countdown--);
    }

    size = count * sizeof(*gOverlay34Pointers);
    gOverlay34Pointers = func_8002B280(size, 0x87);
    word = (s32 *)gOverlay34Pointers;
    remaining = size >> 2;
    countdown = remaining - 1;
    if (remaining != 0) {
        do {
            *word++ = 0;
        } while (countdown--);
    }
    gOverlay34Count = count;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o034/overlay34InitStorage/func_overlay_034_F0000000_18811A8.s")
#endif
