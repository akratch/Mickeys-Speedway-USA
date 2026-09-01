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
 * Bounded reproof: the repaired configured and isolated V0 objects are
 * byte-identical and retain the exact 0xC8 function symbol and 0x30 frame at
 * 45/50 raw and 46/50 relocation-normalized words, first mismatch +0x24.
 * The fifth raw difference at +0xC4 is the standalone target object's baked
 * local-data +8 addend; V0 emits all eight authoritative runtime identities.
 * All 119 flag recipes were nonexact, with the configured -O2 -mips2 family
 * tied for best. The target assigns the reused size spill to sp+0x1C while IDO
 * assigns this body to sp+0x18 at four sites. The single stack-home trace saw
 * 11 allocator webs but no producer-emitted virtual or final home evidence, so
 * it exposed no source-visible lever and did not justify the one optional
 * natural declaration/scope/lifetime form. The trailing eight section-
 * alignment bytes remain outside target ownership. Park as W without dummy
 * volatile state, forced registers, or generic permutation.
 *
 * A later exact lexical-block precedent justified one bounded scope recheck.
 * Giving the two allocation phases independent natural local scopes retained
 * 50 words but regressed to 41/50 with a 0x40 frame. Moving only each loop's
 * countdown into its consuming branch emitted 49 words with a 0x38 frame and
 * a structural/register cascade. Neither changed the target's four-site
 * stack-home mechanism in isolation, so the exact-sized 46/50 V0 remains the
 * best policy-clean source.
 *
 * A fidelity-gated whole-itable capture later identified the abstract -0x14
 * home as the synthesized old-value carrier for `countdown--`: copy the
 * countdown, decrement/store it, then compare the old value. The byte-size
 * expression is instead the multiply/conversion web colored to `a2`; its
 * caller-save slot is assigned after the captured frontend homes. Making the
 * old value explicit moved `size` to the -0x14 home while the four emitted
 * saves and reloads remained at sp+0x18, proving those mechanisms are
 * decoupled. Assignment-in-call optimized back to V0, while carrying a second
 * byte-count alias across each call reproduced the known 48-word structural
 * regression.
 *
 * A fresh current-canonical reproof retains 45/50 raw and 46/50 relocation-
 * normalized words with the exact 0x30 frame. The authorized final caller-
 * save-slot capture cannot be produced by the pinned workbench: its supported
 * globalcolor grammar emits no calibrated final_offset field, and the receipt
 * contract explicitly forbids inferring one from raw words, source lines,
 * registers, or ordinals. The missing final-home hook is therefore a shared
 * tooling dependency, not permission for another source-form sweep.
 */
#ifdef NON_MATCHING
/* PLATEAU-HANDOFF
 * symbol: overlay34InitStorage
 * score: 45/50 words
 * frame: 0x30
 * relocations: 8
 * first-mismatch: +0x24
 * summary: Fresh V0 is exact-sized at 45/50 raw and 46/50 relocation-normalized words with frame 0x30; four sites remain sp+0x18 versus target sp+0x1C. The pinned producer emits no calibrated final_offset and the receipt contract forbids inference, so the authorized final-save-slot mechanism requires new global tooling; prior source forms remain closed.
 */
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
