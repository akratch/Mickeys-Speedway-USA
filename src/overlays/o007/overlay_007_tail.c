#define OVERLAY_007_DEFINE_BSS
#include "overlays/overlay_007.h"

/* Overlay 7's local BSS through +0x2a0; the final 0x10 remains atlas-owned. */
static Overlay7Entry gOverlay7EntryStorage[32];
static s32 gOverlay7Reserved280;
static Overlay7Entry *gOverlay7ActiveHead;
static Overlay7Entry *gOverlay7FreeHead;
static Overlay7Entry *gOverlay7ActiveTail;
static Overlay7Entry *gOverlay7Selected;

typedef struct Overlay7SelectionRow {
    u16 values[3];
} Overlay7SelectionRow;

/* Overlay 7, ADR 0006 consolidation: C after the middle assembly island. */

/*
 * Reproof lead (evidence reviewed 2026-08-29): stale local configured full-TU
 * and isolated objects from the historical masked spelling have the exact 131-
 * word size and 0x20 frame but survive only outside canonical Git and predate
 * the corrected mathRnd identities. They measured 128/131 raw and 129/131
 * after runtime-relocation normalization: one t4-versus-t3 value web
 * differs at +0x44/+0x64, while the raw +0xA4 difference is the owned switch
 * table's LO16 addend. That score used a redundant low-ten-bit mask, now
 * removed, so clean current-source V0 remains uncompiled and its score, frame,
 * size, and tuples are unknown. The stale objects preserve all 30 offsets/types
 * but incorrectly name both +0x124/+0x1BC calls as overlay7LookupReloc rather
 * than runtime-authenticated mathRnd; they cannot support promotion. ORT 1471
 * has six callers. An exact-range structural scan found no credible donor,
 * best similarity 0.101. Compile current V0 and re-prove all 23 text plus seven
 * switch-table records. If shape regresses, use one identity-correct masked
 * diagnostic control; otherwise retain 119 configurations including V0, trace
 * the value web once, and try at most two trace-selected natural scalar/scope
 * forms. Hard cap 121 deterministic builds plus one trace, 122 only for the
 * control; no combination or generic batch.
 */
/* Ownership trial (2026-08-28): fixed the TU's +0x934..+0x950 .rodata range;
 * its historical 455-word linked failure measured the whole trial surface, not this
 * function's isolated plateau. Module growth is cleared; exact function/TU,
 * relocation, linked-range, and ROM proof remain outstanding. */
#ifdef NON_MATCHING
void overlay7DispatchModes(Overlay7ModeOwner *first, Overlay7ModeOwner *second) {
    Overlay7ModeState *firstState;
    Overlay7ModeState *secondState;
    Overlay7ModeRecord *record;
    Overlay7ModeRecord (*modes)[10];

    secondState = second->state;
    firstState = first->state;
    modes = secondState->alternate == 0 ? gOverlay7AlternateModes
                                        : gOverlay7PrimaryModes;
    record = &modes[firstState->index][secondState->index];

    if ((s32)(gOverlay7DispatchFlagsReloc << 22) < 0) {
        secondState->timer = 100;
        secondState->height += 5.0f;
        switch (record->mode) {
        case 1:
            overlay7CreateEntry(first, record->first, 3);
            break;
        case 2:
            overlay7CreateEntry(first, record->first, 3);
            overlay7AppendEntry(second, record->second, 3);
            break;
        case 3:
            overlay7CreateEntry(second, record->second, 3);
            overlay7AppendEntry(first, record->first, 3);
            break;
        case 4:
            if (mathRnd(1, 2) == 1) {
                overlay7CreateEntry(first, record->first, 3);
                overlay7AppendEntry(second, record->second, 3);
            } else {
                overlay7CreateEntry(second, record->second, 3);
                overlay7AppendEntry(first, record->first, 3);
            }
            break;
        case 0:
        case 5:
            overlay7CreateEntry(first, record->first, 3);
            overlay7CreateEntry(second, record->second, 3);
            break;
        case 6:
            if (mathRnd(1, 2) == 1) {
                overlay7CreateEntry(first, record->first, 3);
                break;
            }
            overlay7CreateEntry(second, record->second, 3);
            break;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o007/overlay_007_tail/func_overlay_007_F0000894_185C71C.s")
#endif

/*
 * Plateau: exact size and opcode schedule, but 81 words still differ from
 * +0x10. The remaining mismatch is a temp-FIFO/register-allocation phase;
 * failed and base now have the target stack homes, and the O2 flag lattice
 * does not change the result.
 */
#ifdef NON_MATCHING
void overlay7UpdateOwnerMode(Overlay7CheckOwner *owner, s32 previous) {
    Overlay7CheckState *state;
    s32 failed;
    Overlay7CheckEntry *entries;
    s32 reference;
    s32 index;
    Overlay7CheckEntry *base;
    s32 i;

    state = owner->state;
    if (*overlay7GetModeReloc() == 1) {
        failed = 0;
        base = (Overlay7CheckEntry *)overlay7GetCheckTableReloc();
        index = overlay7GetCheckIndexReloc(overlay7GetCurrentReloc());

        if (D_8 < state->field383 && !(state->flags1A8 & 1)) {
            if (D_8 == 2) {
                entries = base + index * 4;
                reference = entries[3].value;
                i = 0;
                do {
                    if (state->limits404[i] < reference || reference == 0 ||
                        state->field400 < entries[i].value ||
                        entries[i].value == 0) {
                        failed = 1;
                    }
                    i++;
                } while (i != 3);
                if (failed) {
                    overlay7RecordCheckReloc(5);
                } else {
                    overlay7RecordCheckReloc(D_8 + 2);
                }
            } else {
                overlay7RecordCheckReloc(D_8 + 2);
            }
            D_8++;
        }

        if (previous == state->field383) {
            if (failed) {
                overlay7SetOwnerModeReloc(owner, 6);
            } else {
                overlay7SetOwnerModeReloc(owner, 12);
            }
        } else {
            overlay7SetOwnerModeReloc(owner, state->field385);
        }
    } else {
        if (D_8 < state->field383 && !(state->flags1A8 & 1)) {
            overlay7RecordCheckReloc(D_8 + 2);
            D_8++;
        }
        if (previous == state->field383) {
            overlay7SetOwnerModeReloc(owner, state->field385 + 7);
        } else {
            overlay7SetOwnerModeReloc(owner, state->field385);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o007/overlay_007_tail/func_overlay_007_F0000AA0_185C928.s")
#endif

/* Plateau evidence reviewed 2026-08-29: retained pre-current-line-layout
 * full-TU C is 55/60 relocation-annotated, 53/60 literal, and 58/60 after
 * runtime normalization, exact size/frame, first substantive mismatch +0x4.
 * The two normalized sites at +0x4/+0x10 are one t7-versus-t6 flags-value web;
 * +0x40/+0x84/+0x88/+0x90/+0xC0 are relocation-only. It owns all 13 runtime
 * records; linked equality is fallback-only. Historical flag, trace, source,
 * and search outcomes are unretained. Compile identity-correct current V0,
 * retain 119 flags, trace once, and try at most two trace-selected natural
 * scalar forms; cap at 122 stock builds plus trace and stop on exact. */
#ifdef NON_MATCHING
void overlay7DispatchSelection(Overlay7DispatchOwner *owner, s32 selection) {
    Overlay7DispatchState *state;
    u16 *override;
    s8 mapped;

    state = owner->state;
    if ((s32)((gOverlay7DispatchFlagsReloc & 0x3FF) << 22) < 0) {
        if (selection >= 14 && selection < 17) {
            override = &gOverlay7DispatchOverride[state->index];
            if (*override == 0) {
                *override = 0x10E;
                goto create;
            }
        } else {
create:
            overlay7CreateEntry(owner,
                                gOverlay7DispatchValues[state->index][selection],
                                gOverlay7DispatchTypes[selection]);
            goto query;
        }
    } else {
query:
        if (camGetModeReloc() == 0) {
            mapped = gOverlay7DispatchMap[selection];
            if (mapped != -1) {
                overlay59AppendValueReloc(0, state->index, mapped,
                                          state->field45D);
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o007/overlay_007_tail/func_overlay_007_F0000CCC_185CB54.s")
#endif

/*
 * Plateau (identity audit 2026-08-29): retained pre-identity-repair C is 66/72
 * raw and 69/72 runtime-normalized, exact size/frame, first substantive +0xBC.
 * The three sites +0xBC/+0xC4/+0xD4 are one post-mathRnd u16 narrowing carrier;
 * +0x4/+0x88/+0xF8 are relocation-only. Current source correctly expresses all
 * 17 identities, including +0xF4's local JUMP to overlay7ReleaseEntry, but no
 * identity-current C object survives. Historical flags, trace, source forms,
 * and search are unretained. Compile V0, retain 119 flags, trace once, and try
 * at most two natural narrowing forms; cap at 121 stock builds plus one trace.
 */
#ifdef NON_MATCHING
void overlay7CommitSelection(s32 selection) {
    u16 value;
    Overlay7Pair *pair;
    s32 remaining;

    if (gOverlay7DispatchModeReloc & 1) {
        switch (selection) {
        case 29:
            value = 0xCF;
            break;
        case 30:
            value = 0xF5;
            break;
        case 31:
            value = 0x116;
            break;
        default:
            value = ((Overlay7SelectionRow *)&gOverlay7DispatchData[0x754])
                        [selection]
                            .values[mathRnd(0, 2)];
            break;
        }
        pair = (Overlay7Pair *)&gOverlay7DispatchData[0x8F4];
        remaining = 11;
        do {
            if (pair->key == value) {
                value += mathRnd(0, pair->value);
                break;
            }
            pair++;
        } while (remaining--);
        if (value != 0) {
            if (gOverlay7DispatchObject != 0) {
                func_800031E8(gOverlay7DispatchObject);
                overlay7ReleaseEntry(gOverlay7Selected);
            }
            amSndPlay(value, &gOverlay7CommitArgument);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o007/overlay_007_tail/func_overlay_007_F0000DBC_185CC44.s")
#endif

/* Pinned DKR v77/v80 and JFG object scans found no exact donor. Exact but
 * non-natural: the empty condition and dummy comma-expression operand are
 * semantically inert allocation aids preserving IDO's 11-word coloring.
 * Tracked in docs/cleanup-queue.md. */
s32 overlay7FillValues(s16 *value) {
    s32 remaining;

    if (((!value) & 0xFFFFU) && (!value)) {
    }
    value = &gOverlay7ValuesEnd;
    remaining = 9;
    do {
        *((0, value)) = 0xF0;
        value--;
    } while (remaining--);
}

/* Naturally exact under the overlay's ordinary IDO -O2 -mips2 flags. */
void overlay7InitPool(void) {
    Overlay7Entry *entry;
    s32 i;

    entry = gOverlay7FreeHead = gOverlay7EntryStorage;
    for (i = 0; i < 31; i++) {
        entry->next = entry + 1;
        entry->active = 0;
        entry++;
    }
    entry->next = 0;
    gOverlay7ActiveHead = 0;
    gOverlay7ActiveTail = 0;
    gOverlay7Selected = 0;
}
