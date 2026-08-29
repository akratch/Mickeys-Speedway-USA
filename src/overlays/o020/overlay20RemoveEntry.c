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
 * this guarded diagnostic retains the ABI-equivalent s32 parameter only so
 * IDO can reuse its carrier for the count. A pointer-typed local regresses to
 * 33 differences. All 119 flags are nonexact; a fidelity-clean proc-0 trace
 * records 13 caller-saved decisions. Narrowing newCount is flat, independent
 * compaction cursors regress to 19 differences, and reusing owner for the
 * decremented count is the retained one-word gain. No independent combination
 * or generic batch qualifies within the 122-build route. IDO's trailing 0xC
 * is section alignment, not target padding; fallback linkage alone is exact. */
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
    owner--;
    gOverlay20EntryCount = owner;
    if (i < owner) {
        cursor = &gOverlay20ShiftEntries[i];
        end = &gOverlay20ShiftEntries[owner];
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
