#include "PR/ultratypes.h"

typedef struct Overlay20RemoveOwner {
    u8 pad0[0x84];
    void *entry;
} Overlay20RemoveOwner;

extern void *gOverlay20Entries[];
extern void *gOverlay20ShiftEntries[];
extern s32 gOverlay20EntryCount;
extern u8 gOverlay20MarkerEnd;
extern u32 gOverlay20ActiveBits;

/* Bounded plateau (2026-08-29): exact pinned DKR v77/v80/JFG scans found no
 * donor. Configured C is 36/53 raw and runtime-normalized words, frameless,
 * first +0x4C, with exact 0xD4 ownership and all ten LOCAL relocation tuples
 * and identities. The caller passes an Overlay20RemoveOwner pointer in a0;
 * this guarded diagnostic retains the ABI-equivalent s32 parameter. A fresh
 * fidelity-clean proc-0 UGEN trace proves a valid temporary FIFO with seven
 * sequential births and no decrement-result birth in the old carrier-reuse
 * form. Writing the decremented expression to the global count and reading it
 * for compaction introduces the target's t7 carrier and improves the plateau
 * from 36/53 to 39/53 words. Reordered, comma-grouped, and explicit-base
 * compaction forms are flat or regress; the remaining base web stays a2 where
 * the target needs the next FIFO birth at t9. All ten relocation identities
 * remain exact. IDO's trailing 0xC is section alignment, not target padding. */
#ifdef NON_MATCHING
void overlay20RemoveEntry(s32 owner) {
    void *entry;
    s32 i;
    void **cursor;
    void **end;

    entry = ((Overlay20RemoveOwner *)owner)->entry;
    if (entry == NULL) {
        return;
    }
    owner = gOverlay20EntryCount;
    i = 0;
    cursor = gOverlay20Entries;
    if (owner > 0) {
        do {
            if (entry == *cursor) {
                break;
            }
            i++;
            cursor++;
            if (i < owner) {
                continue;
            }
            break;
        } while (1);
    }
    if (i >= owner) {
        return;
    }
    gOverlay20EntryCount = owner - 1;
    if (i < gOverlay20EntryCount) {
        cursor = &gOverlay20ShiftEntries[i];
        end = &gOverlay20ShiftEntries[gOverlay20EntryCount];
        do {
            *cursor = cursor[1];
            cursor++;
        } while (cursor < end);
    }

    owner = (s32)&gOverlay20MarkerEnd;
    i = 31;
    do {
        if (owner != 0) {
            gOverlay20ActiveBits &= ~(1U << i);
            return;
        }
        owner -= 0x24;
    } while (i--);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o020/overlay20RemoveEntry/func_overlay_020_F0001018_18775F0.s")
#endif

/* PLATEAU-HANDOFF:overlay20RemoveEntry:start
 * symbol: overlay20RemoveEntry
 * score: 39/53 words
 * frame: frameless
 * relocations: 10
 * first-mismatch: +0x5C
 * summary: Fidelity-clean FIFO trace exposes a missing decrement birth; the global-count form gains 3 words, but the compaction base remains one birth behind.
 * PLATEAU-HANDOFF:overlay20RemoveEntry:end
 */
