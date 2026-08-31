#include "PR/ultratypes.h"

typedef struct Overlay40Entry {
    s8 state;
    s8 scales[3];
    u8 red;
    u8 green;
    u8 blue;
    u8 id;
} Overlay40Entry;

typedef struct Overlay40Node {
    u8 pad00[0x64];
    void *effect64;
} Overlay40Node;

typedef struct Overlay40Object {
    u8 pad00[8];
    Overlay40Node *node;
} Overlay40Object;

extern Overlay40Entry gOverlay40Entries[8];
extern Overlay40Object **gOverlay40Objects;

/* Plateau: exact 0xB8 extent and frameless CFG; 44/46 positional
 * words match, with the only residual at +0xC/+0x10: IDO schedules the loop
 * count before the gOverlay40Objects LO16 while the target completes that
 * address first. All 119 flag identities preserve or worsen the schedule.
 * A token-identical line-boundary probe left both words unchanged, ruling out
 * statement line ownership for this input. A bounded, gain-gated permutation
 * batch found the natural declaration order retained below; it improves the
 * byte-weighted diagnostic from 215 to 20 but preserves the same two-word
 * schedule residual. The direct shift remains required for the target's t7/t8
 * byte webs. The sole resident caller supplies amount in a0; remaining is
 * overwritten before use. Keep the assembly fallback until a new scheduler
 * mechanism closes the final swap and all four relocation tuples compare
 * exact. */
#ifdef NON_MATCHING
void overlay40UpdateEntries(s32 amount, s32 remaining) {
    Overlay40Entry *entry;
    s8 previous;
    Overlay40Object *object;

    entry = gOverlay40Entries;
    remaining = 7;
    do {
        if (entry->state != -1) {
            if (entry->state < 8) {
                entry->state++;
            }
            previous = entry->scales[0];
            entry->scales[2] = entry->scales[1];
            entry->scales[0] = previous - amount;
            entry->scales[1] = previous;
            if (entry->scales[0] < 0) {
                entry->scales[0] = 0;
            }
            object = gOverlay40Objects[entry->id];
            if (object != 0 && object->node != 0) {
                if (entry->state & 1) {
                    object->node->effect64 = &entry->red;
                } else {
                    object->node->effect64 = 0;
                }
            }
        }
        entry++;
    } while (remaining--);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o040/overlay40UpdateEntries/func_overlay_040_F00000E8_1886998.s")
#endif

/* PLATEAU-HANDOFF:overlay40UpdateEntries:start
 * symbol: overlay40UpdateEntries
 * score: 44/46 words
 * frame: frameless
 * relocations: 4
 * first-mismatch: +0xC
 * summary: Fresh listing, preprocessed-input, and token-identical line probes preserve 44/46 with the +0xC/+0x10 schedule residual. A bounded permutation batch identified the retained natural previous-before-object declaration order (byte-weighted diagnostic 215 -> 20), but positional exactness remains 44/46. Flags, comma forms, line ownership, and declaration-order families are exhausted; resume only with a genuinely new scheduler mechanism.
 * PLATEAU-HANDOFF:overlay40UpdateEntries:end
 */
