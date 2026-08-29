#include "PR/ultratypes.h"

typedef struct Overlay34Record {
    u8 bytes[0x68];
} Overlay34Record;

extern Overlay34Record *gOverlay34Records;
extern Overlay34Record **gOverlay34Pointers;
extern s32 gOverlay34Count;
extern void *func_8002B280(s32 size, u32 colourTag);

/* Pinned exact-overlay scans found no exact DKR v77/v80 or JFG donor;
 * structural or semantic donors remain unqualified. */
/*
 * Plateau: exact 0xC8 function symbol and 0x30 frame, 45/50 raw and 46/50
 * relocation-normalized words,
 * first mismatch +0x24. The surviving isolated ranking has a fifth raw
 * difference at +0xC4 because its assembled target bakes the local-data +8
 * addend; the candidate and runtime table require eight relocation sites. The
 * genuine C section's trailing eight alignment bytes are not target ownership.
 * The target assigns the reused size spill to sp+0x1C; IDO assigns this body
 * to sp+0x18. Historical declaration, lifetime, type, flag, trace, and
 * permutation outcomes have no surviving attributable result set and do not
 * prove exhaustion. Preserve a fresh unchanged V0 and runtime-annotated
 * target, retain the complete 119-recipe flag lattice including V0, then take
 * one CFE/UOPT/UGEN trace. Only if it identifies a source-visible lever may one
 * declaration/scope/lifetime form using existing semantic locals be compiled;
 * no dummy volatile home, inert guard, forced register, or generic permutation.
 * Stop if the trace exposes no lever or that form changes size, frame, any
 * identity, any word outside the four home offsets, or remains nonexact. Park
 * as W.
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
