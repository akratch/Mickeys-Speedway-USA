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

/* PROVENANCE: indexed search and list-compaction loops adapted from Diddy Kong
 * Racing's published src/weather.c::lensflare_override_remove. Mickey's owner
 * offset, arrays, marker cleanup, relocations, and target bytes remain
 * authoritative. */
/* Bounded plateau (2026-09-04): configured C is 47/53 words, frameless, with
 * exact 0xD4 ownership and all ten relocation tuples and identities. The
 * donor-style indexed compaction removes two pool webs and makes the 12-web
 * temporary lane exact. The remaining six words are a two-web pool rotation:
 * the target uses v0/a1 for the end/cursor pair while IDO chooses a1/a0.
 * Fidelity-clean proc-0 tracing identifies an invisible v0 web that conflicts
 * with the end web. A diagnostic split plus forced cursor color reaches the
 * target pool assignments but introduces a stack frame, so it is not a valid
 * promotion. Pointer-, index-, cursor-relative-, explicit-base-, register-,
 * byte-offset-, and association variants were exhausted. IDO's trailing 0xC
 * is section alignment, not target padding. */
#ifdef NON_MATCHING
void overlay20RemoveEntry(s32 owner) {
    void *entry;
    s32 i;

    entry = ((Overlay20RemoveOwner *)owner)->entry;
    if (entry == NULL) {
        return;
    }
    owner = gOverlay20EntryCount;
    i = 0;
    if (owner > 0) {
        do {
            if (entry == gOverlay20Entries[i]) {
                break;
            }
            i++;
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
        do {
            gOverlay20ShiftEntries[i] = gOverlay20ShiftEntries[i + 1];
            i++;
        } while (i < gOverlay20EntryCount);
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
 * score: 47/53 words
 * frame: frameless
 * relocations: 10
 * first-mismatch: +0x6C
 * summary: Indexed compaction makes the temporary lane exact and cuts the deficit to six words; an invisible interfering web blocks the remaining two-web pool rotation.
 * PLATEAU-HANDOFF:overlay20RemoveEntry:end
 */
