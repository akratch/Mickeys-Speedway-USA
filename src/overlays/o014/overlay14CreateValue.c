#include "PR/ultratypes.h"

typedef struct Overlay14ValueSlot {
    s32 key;
    void *volatile value;
} Overlay14ValueSlot;

extern Overlay14ValueSlot gOverlay14Slots28[];
extern Overlay14ValueSlot gOverlay14FreeSlots28[];
extern Overlay14ValueSlot gOverlay14ChosenSlots28[];
extern Overlay14ValueSlot gOverlay14SlotsEnd128[];
extern void *gOverlay14SlotsActive2C;
extern s32 gOverlay14SlotCountE8;

extern s32 overlay14SelectKind(void);
extern void *overlay14LoadRelocatedValue(s32 key, s32 kind);
extern void *func_overlay_014_F00009F4_18702CC(s32 key, s32 kind);

/* Plateau (scheduler trace reopen): exact 0x180 size and 0x28 frame;
 * 82/96 relocation-aware words match. A fidelity-clean as1 -R capture proved
 * that the initial slot/end address chains are independent and selected by
 * source line. Keeping the assignment and loop label on one physical line
 * closes the two low-half schedule words and their four data identities.
 * All 15 relocation sites align and 14 identities resolve; only the selector
 * call identity at +0xAC remains open. The residual begins in the chosen/slot
 * pointer web after the active-slot load. */
#ifdef NON_MATCHING
void *overlay14CreateValue(s32 key, s32 alternate) {
    Overlay14ValueSlot *slot;
    void *value;
    s32 index;
    Overlay14ValueSlot *volatile chosen;
    s32 kind;

    slot = gOverlay14Slots28; scan_loop:
    value = slot->value;
    if ((value != 0) && (slot->key == key)) {
        return value;
    }
    slot++;
    if (slot < gOverlay14SlotsEnd128) {
        goto scan_loop;
    }

    index = 0;
    slot = gOverlay14FreeSlots28;
    if (gOverlay14SlotsActive2C != 0) {
        do {
            index++;
            if (index >= 32) {
                break;
            }
            slot = &gOverlay14FreeSlots28[index];
        } while (slot->value != 0);
    }
    if (index >= 32) {
        return 0;
    }
    chosen = &gOverlay14ChosenSlots28[index];
    kind = overlay14SelectKind();
    slot = chosen;

    switch (kind) {
        case 1:
            kind = 0xC;
            break;
        case 2:
            kind = 0xE;
            break;
        case 3:
            kind = 0x10;
            break;
        case 5:
            kind = 0x12;
            break;
        default:
            kind = 0xA;
            break;
    }

    if (alternate != 1) {
        slot->value = overlay14LoadRelocatedValue(key, kind);
    } else {
        slot->value = func_overlay_014_F00009F4_18702CC(key, kind);
    }
    value = slot->value;
    if (value != 0) {
        slot->key = key;
        value = slot->value;
        gOverlay14SlotCountE8++;
    }
    return value;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o014/overlay14CreateValue/func_overlay_014_F00006FC_186FFD4.s")
#endif

/* PLATEAU-HANDOFF:overlay14CreateValue:start
 * symbol: overlay14CreateValue
 * score: 82/96 words
 * frame: 0x28
 * relocations: 15
 * first-mismatch: +0x54
 * summary: Line-tag scheduling closes the initial LO16 pair; 14 positional words and one selector-call identity remain.
 * PLATEAU-HANDOFF:overlay14CreateValue:end
 */
