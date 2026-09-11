#include "PR/ultratypes.h"

typedef struct Overlay58OrderEntry {
    u8 pad00[3];
    u8 gap03;
    s32 value04;
    u8 pad08[0x14];
    u8 progress1C;
    u8 pad1D[5];
    u16 rank22;
    u8 pad24[4];
} Overlay58OrderEntry;

typedef struct Overlay58OrderState {
    u8 mode;
    u8 active;
    u8 player;
    u8 countdown;
    Overlay58OrderEntry entries[6];
} Overlay58OrderState;

typedef struct Overlay58TagSource {
    u8 pad00[0x2A];
    s8 tag2A;
    u8 pad2B[9];
} Overlay58TagSource;

typedef struct Overlay58SnapshotSource {
    u8 pad00[0xC];
    s32 value0C;
    s32 value10;
    s32 value14;
    u8 pad18[0x1D4];
    s32 value1EC;
} Overlay58SnapshotSource;

typedef struct Overlay58GameState {
    u8 pad00[0x50];
    s32 value50;
    u8 pad54[0x3C];
    s32 value90;
    s32 value94;
    s32 value98;
    u8 pad9C[0x124];
    s32 value1C0;
} Overlay58GameState;

extern Overlay58SnapshotSource gOverlay58SnapshotSourceReloc;
extern Overlay58TagSource gOverlay58TagSourcesReloc[];
extern Overlay58TagSource gOverlay58TagSourcesEndReloc[];
extern Overlay58GameState *gOverlay58GameStateReloc;
extern u8 gOverlay58EntryCountReloc;
extern u8 gOverlay58UpdateGateReloc;
extern u16 gOverlay58PackedFlagsReloc;
extern s32 gOverlay58ModeReloc;
extern s32 gOverlay58FrameCounterReloc;

extern s32 D_18[4];
extern s32 D_28;
extern s32 D_2C;
extern s32 D_30;
extern s32 D_6C;
extern s32 D_74;
extern Overlay58OrderEntry *D_78[6];
extern Overlay58OrderEntry *D_90[6];
extern Overlay58OrderEntry *D_7C[];
extern Overlay58OrderEntry *D_94[];
extern s32 D_A8[6];
extern s32 D_C0[6];
extern s32 D_AC[];
extern s32 D_C4[];
extern u8 D_C8[];
extern s8 D_F8[6];
extern s32 D_108[];
extern s32 D_120;
extern s32 D_140;
extern s32 D_160;
extern s32 D_180;
extern s32 D_D8;
extern s32 D_DC;
extern s32 D_E0;
extern u8 D_E8[];

extern Overlay58OrderState *overlay58GetOrderStateReloc(void);
extern void overlay58ResetOrderStateReloc(void *state);
extern void overlay58BeginOrderUpdateReloc(void);
extern void overlay58SplitOrderValueReloc(s32 value, s32 *part0, s32 *part1,
                                          s32 *part2);
extern void overlay58BuildOrderReloc(void *state, s32 count, s32 gap);
extern void overlay58FinalizePackedStatus(void);

/* Every Reloc name is provisional; normalized assembly does not bind it. */
/* Declaration order set to the target's home ladder 2026-09-11, byte-inert
 * (295 masked, 109 aligned-exact, 165 naming, 18 immediate before and after).
 * `count` is second only to carry the ladder; any single local does, and four
 * choices measured identical. */
/*
 * Plateau (2026-09-10): 295 of 368 relocation-masked words differ, down from
 * 296, at exact 368-word size.  One safe change: the two stores in the seeding
 * loop are swapped.  They go through disjoint arrays and write the same value,
 * so the order is inert.
 *
 * The frame, not the words, is this function's live problem: 0x78 against the
 * target's 0x60, a 24-byte surplus at EXACT instruction count.  Measured, not
 * inferred: adding N unused `s32` declarations grows the frame in an 8-byte
 * step every second declaration and moves no word at all (0/1 pads 0x78, 2/3
 * 0x80, 4/5 0x88, 6 0x90), so the declared block is a rounded quantum charged
 * per declaration whether or not the local is ever used or coloured.  Removing
 * the three `right*` out-parameters buys 16 of the 24 bytes (0x78 -> 0x68).
 * So the target's declared block is five or six cells smaller than this one,
 * and the six address-taken `left*`/`right*` cells are where to look: `&x` in
 * an argument list is what forces a memory home.  Re-spelling them as two
 * `s32[3]` arrays or one `s32[6]` is byte-flat -- same cells, same frame --
 * and a form that keeps both calls but only three cells lands at 0x68 with a
 * 52-byte code deficit, so the reconstruction has to remove declarations
 * without removing the second call.
 *
 * Also measured here: `register` is inert.  Stripping it from all 37 locals
 * changes neither the words nor the frame (ido-5.3 L16).
 */
#ifdef NON_MATCHING
void func_overlay_058_F0000000_18AF1E8(void) {
    register s32 i;
    register s32 count;
    s32 left0;
    s32 right0;
    s32 left1;
    s32 right1;
    s32 left2;
    s32 right2;
    Overlay58OrderState *state;
    Overlay58OrderEntry *entry;
    Overlay58OrderEntry *next;
    register Overlay58OrderEntry **orderCursor;
    register Overlay58OrderEntry **rankCursor;
    Overlay58TagSource *tagSource;
    s8 *tag;
    s32 *classCursor;
    register s32 limit;
    register s32 swapped;
    register s32 gap;

    state = overlay58GetOrderStateReloc();
    overlay58ResetOrderStateReloc(D_C8);
    D_120 = gOverlay58SnapshotSourceReloc.value0C;
    D_140 = gOverlay58SnapshotSourceReloc.value10;
    D_160 = gOverlay58SnapshotSourceReloc.value14;
    D_180 = gOverlay58SnapshotSourceReloc.value1EC;
    D_30 = 0;
    overlay58BeginOrderUpdateReloc();

    tag = D_F8;
    tagSource = gOverlay58TagSourcesReloc;
    do {
        *tag++ = tagSource->tag2A;
        tagSource++;
    } while (tagSource < gOverlay58TagSourcesEndReloc);

    D_6C = state->active;
    count = gOverlay58EntryCountReloc;
    if (count > 0) {
        orderCursor = D_78;
        rankCursor = D_90;
        entry = &state->entries[0];
        do {
            rankCursor++;
            rankCursor[-1] = entry;
            *orderCursor = entry;
            entry++;
            orderCursor++;
        } while (rankCursor < &D_90[count]);
    }

    limit = count - 1;
    if (state->mode == 5) {
        if (limit > 0) {
            do {
                swapped = 0;
                orderCursor = D_78;
                rankCursor = &D_78[limit];
                do {
                    entry = orderCursor[0];
                    next = orderCursor[1];
                    if (entry->value04 < next->value04) {
                        orderCursor[0] = next;
                        orderCursor[1] = entry;
                        swapped = 1;
                    }
                    orderCursor++;
                } while (orderCursor < rankCursor);
            } while (swapped != 0);
        }
    } else {
        if (limit > 0) {
            do {
                swapped = 0;
                orderCursor = D_78;
                rankCursor = &D_78[limit];
                do {
                    entry = orderCursor[0];
                    next = orderCursor[1];
                    if (next->value04 < entry->value04) {
                        orderCursor[0] = next;
                        orderCursor[1] = entry;
                        swapped = 1;
                    }
                    orderCursor++;
                } while (orderCursor < rankCursor);
            } while (swapped != 0);
        }
    }

    D_A8[0] = 0;
    i = 1;
    if (i < gOverlay58EntryCountReloc) {
        orderCursor = D_7C;
        classCursor = D_AC;
        do {
            overlay58SplitOrderValueReloc(orderCursor[-1]->value04, &left0,
                                          &left1, &left2);
            overlay58SplitOrderValueReloc(orderCursor[0]->value04, &right0,
                                          &right1, &right2);
            if ((left0 == right0) && (left1 == right1) &&
                (left2 == right2)) {
                classCursor[0] = classCursor[-1];
            } else {
                classCursor[0] = i;
            }
            i++;
            orderCursor++;
            classCursor++;
        } while (i < gOverlay58EntryCountReloc);
    }

    i = 0;
    if (i < gOverlay58EntryCountReloc) {
        do {
            if (&state->entries[0] == D_78[i]) {
                D_74 = i;
                i = 6;
            }
            i++;
        } while (i < gOverlay58EntryCountReloc);
        i = 0;
    }
    if (((D_74 < 4) || (gOverlay58UpdateGateReloc != 0)) &&
        (state->mode != 1)) {
        if (i < gOverlay58EntryCountReloc) {
            orderCursor = D_78;
            classCursor = D_A8;
            do {
                entry = (Overlay58OrderEntry *)((u8 *)*orderCursor +
                                                *classCursor);
                ((u8 *)entry)[0x1C]++;
                entry = *orderCursor;
                entry->rank22 += D_108[*classCursor];
                entry = *orderCursor;
                if (entry->rank22 >= 10000) {
                    entry->rank22 = 9999;
                }
                count = gOverlay58EntryCountReloc;
                i++;
                orderCursor++;
                classCursor++;
            } while (i < count);
        }
        i = 0;
    } else if ((state->active != 0) &&
               ((gOverlay58PackedFlagsReloc & 0x100) == 0)) {
        state->countdown--;
    }

    limit = gOverlay58EntryCountReloc - 1;
    do {
        swapped = 0;
        if (limit > 0) {
            orderCursor = D_90;
            rankCursor = &D_90[limit];
            do {
                entry = orderCursor[0];
                next = orderCursor[1];
                if (entry->rank22 < next->rank22) {
                    orderCursor[0] = next;
                    orderCursor[1] = entry;
                    swapped = 1;
                }
                orderCursor++;
            } while (orderCursor < rankCursor);
            i = 0;
        }
    } while (swapped != 0);

    i = 0;
    if (i < gOverlay58EntryCountReloc) {
        orderCursor = D_90;
        do {
            entry = *orderCursor;
            gap = D_90[0]->rank22 - entry->rank22;
            if (gap >= 101) {
                gap = 100;
            }
            entry->gap03 = gap;
            count = gOverlay58EntryCountReloc;
            i++;
            orderCursor++;
        } while (i < count);
    }

    D_C0[0] = 0;
    i = 1;
    if (i < gOverlay58EntryCountReloc) {
        orderCursor = D_94;
        classCursor = D_C4;
        do {
            entry = orderCursor[-1];
            next = orderCursor[0];
            if (entry->rank22 == next->rank22) {
                classCursor[0] = classCursor[-1];
            } else {
                classCursor[0] = i;
            }
            i++;
            orderCursor++;
            classCursor++;
        } while (i < gOverlay58EntryCountReloc);
    }

    D_D8 = 0;
    overlay58BuildOrderReloc(D_E8, 10, gap);
    D_2C = 0;
    if (gOverlay58ModeReloc == 3) {
        D_28 = 3;
    } else {
        D_28 = 0;
    }

    if (state->mode != 5) {
        D_18[0] = gOverlay58GameStateReloc->value90;
    } else {
        D_18[0] = gOverlay58GameStateReloc->value1C0;
    }
    D_18[1] = gOverlay58GameStateReloc->value94;
    D_18[2] = gOverlay58GameStateReloc->value98;
    D_18[3] = gOverlay58GameStateReloc->value50;

    overlay58FinalizePackedStatus();
    D_DC = 1;
    D_E0 = 0;
    gOverlay58FrameCounterReloc++;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o058/func_overlay_058_F0000000_18AF1E8/func_overlay_058_F0000000_18AF1E8.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_058_F0000000_18AF1E8:start
 * symbol: func_overlay_058_F0000000_18AF1E8
 * score: 295/368 words
 * frame: 0x78
 * relocations: 95
 * first-mismatch: +0x0
 * summary: Target block solved at 52 bytes, thirteen declarations; six carriers must merge and the surviving positions are pinned.
 * PLATEAU-HANDOFF:func_overlay_058_F0000000_18AF1E8:end
 */
