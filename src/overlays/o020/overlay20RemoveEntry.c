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

/* Exact pinned DKR v77/v80/JFG scans found no donor; no function-specific
 * near-match oracle survives. Retained configured/isolated current-body C is
 * 35/53 raw and runtime-normalized words, frameless, first +0x4C, with all ten
 * runtime relocation sites represented. It owns +0x1018..+0x10EC with no
 * target padding; IDO's trailing 0xC is section alignment. Eighteen sites are
 * the newCount, compaction cursor/end, and active-bit allocator web. The caller
 * passes a pointer, so the argument is typed below; fresh score is pending.
 * Historical 119-flag/forms/permuter claims are unretained. Run 119 flags
 * including V0, trace once, try one natural newCount form and one cursor/end
 * form, then only a strict-gain combination; cap 122 builds plus trace and one
 * <=2,000-candidate batch only after a legal gain. */
#ifdef NON_MATCHING
void overlay20RemoveEntry(Overlay20RemoveOwner *owner) {
    void *entry;
    s32 ownerValue;
    s32 i;
    s32 newCount;
    void **cursor;
    void **end;

    entry = owner->entry;
    if (entry == NULL) {
        return;
    }
    ownerValue = gOverlay20EntryCount;
    i = 0;
    cursor = gOverlay20Entries;
    if (ownerValue > 0) {
        do {
            if (entry == *cursor) {
                break;
            }
            i++;
            cursor++;
            if (i < ownerValue) {
                continue;
            }
            break;
        } while (1);
    }
    if (i >= ownerValue) {
        return;
    }
    newCount = ownerValue - 1;
    gOverlay20EntryCount = newCount;
    if (i < newCount) {
        cursor = &gOverlay20ShiftEntries[i];
        end = &gOverlay20ShiftEntries[newCount];
        do {
            *cursor = cursor[1];
            cursor++;
        } while (cursor < end);
    }

    ownerValue = (s32)&gOverlay20MarkerEnd;
    i = 31;
    do {
        if (ownerValue != 0) {
            gOverlay20ActiveBits &= ~(1U << i);
            return;
        }
        ownerValue -= 0x24;
    } while (i--);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o020/overlay20RemoveEntry/func_overlay_020_F0001018_18775F0.s")
#endif
