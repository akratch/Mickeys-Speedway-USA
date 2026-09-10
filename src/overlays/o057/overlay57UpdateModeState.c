#include "PR/ultratypes.h"

typedef struct Overlay57SetupObject {
    u8 reserved00[0x0C];
    s16 x0C;
    s16 y0E;
} Overlay57SetupObject;

typedef struct Overlay57MarkedEntry {
    s16 value00;
    u8 reserved02[2];
    u32 flags04;
} Overlay57MarkedEntry;

typedef struct Overlay57CountOwner {
    u8 reserved00[0x2C];
    u8 count2C;
} Overlay57CountOwner;

typedef struct Overlay57Selection {
    Overlay57CountOwner *countOwner00;
    u8 reserved04[0x48];
    Overlay57MarkedEntry *entries4C;
} Overlay57Selection;

typedef struct Overlay57SelectionChild {
    u8 reserved00[0x3A];
    s8 selector3A;
    u8 reserved3B[0x2D];
    Overlay57Selection **selections68;
} Overlay57SelectionChild;

typedef struct Overlay57LookupResult {
    u8 reserved00[8];
    Overlay57SelectionChild *child08;
} Overlay57LookupResult;

#define O57_SUB_WRAP(left, right) ((s32)((u32)(left) - (u32)(right)))

extern u32 gOverlay57Flags3B6CReloc;
extern s32 gOverlay57Mode3B88Reloc;
extern u8 gOverlay57Byte3D1CReloc;
extern u8 gOverlay57Byte3EF4Reloc;

extern void overlay57SetNodeValue(s32 id, s32 argument, f32 value);
extern void overlay57UpdateNode(void);
extern void overlay57ApplyTable(void);

extern s32 gO57ModeCountdown120;
extern s32 gO57ModeTimer114;
extern s32 gO57ModeState144BaseReloc[];
extern s32 gO57ModeState11CBaseReloc[];
extern s32 gO57ModeState118BaseReloc[];
extern s32 gO57ModeState160BaseReloc[];
extern s32 gO57ModeState164BaseReloc[];
extern s32 gO57ModeState184BaseReloc[];
extern s32 gO57ModeChoice4F8;
extern s32 gO57ModeGate50C[];
extern s32 gO57ModePrimaryIds134[];
extern s32 gO57ModeSecondaryIds1A0[];
extern Overlay57SetupObject gO57ModeSetup21C;
extern s32 gO57ModeTimerEarlyStoreBaseReloc[];
extern s32 gO57ModeTimerReloadBaseReloc[];
extern s32 gO57ModeTimerA5BaseReloc[];
extern s32 gO57ModeTimerA6BaseReloc[];
extern s32 gO57ModeTimerA7BaseReloc[];
extern s32 gO57ModeTimerB5BaseReloc[];
extern s32 gO57ModeTimerB6BaseReloc[];

#undef O57_S32
#define D_0x118 gO57ModeState118BaseReloc[0x118 / 4]
#define D_0x11C gO57ModeState11CBaseReloc[0x11C / 4]
#define D_0x120 gO57ModeCountdown120
#define D_0x144 gO57ModeState144BaseReloc[0x144 / 4]
#define D_0x160 gO57ModeState160BaseReloc[0x160 / 4]
#define D_0x164 gO57ModeState164BaseReloc[0x164 / 4]
#define D_0x184 gO57ModeState184BaseReloc[0x184 / 4]
#define D_0x4F8 gO57ModeChoice4F8
#define D_0x50C gO57ModeGate50C[0]
#define O57_JOIN_RAW(left, right) left##right
#define O57_JOIN(left, right) O57_JOIN_RAW(left, right)
#define O57_S32(offset) O57_JOIN(D_, offset)

extern s32 o57ModeOpaqueCallReloc();
extern void o57ModeOpaqueVoidCallReloc();
extern Overlay57LookupResult *o57ModeOpaquePtrCallReloc();
#define overlay57Call3AA4Reloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3AE0Reloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3B4CReloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3BC4Reloc o57ModeOpaqueCallReloc
#define overlay57Call3BECReloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3BF8Reloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3C08Reloc o57ModeOpaqueCallReloc
#define overlay57Call3C4CReloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3C5CReloc o57ModeOpaqueCallReloc
#define overlay57Call3C84Reloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3C94Reloc o57ModeOpaqueCallReloc
#define overlay57Call3CDCReloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3D08Reloc o57ModeOpaqueCallReloc
#define overlay57Call3D24Reloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3D34Reloc o57ModeOpaquePtrCallReloc
#define overlay57Call3DC4Reloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3DD4Reloc o57ModeOpaqueCallReloc
#define overlay57Call3DFCReloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3E08Reloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3E18Reloc o57ModeOpaqueCallReloc
#define overlay57Call3E5CReloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3E6CReloc o57ModeOpaqueCallReloc
#define overlay57Call3EB4Reloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3EE0Reloc o57ModeOpaqueCallReloc
#define overlay57Call3EFCReloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3F0CReloc o57ModeOpaquePtrCallReloc
#define overlay57Call3FA4Reloc o57ModeOpaqueVoidCallReloc
#define overlay57Call3FB8Reloc o57ModeOpaqueVoidCallReloc

/* Plateau: exact-size at 0x588, 21 masked words, first at +0x108.
 * savedEligible is a plain s32: as `volatile` it forces the store into memory
 * but costs a frame cell.  `timer` is declared at function scope ahead of it so
 * that the eligibility spill takes the frame's first cell at sp+0x28, which is
 * where the target keeps it.
 *
 * The 43-word temp-ring rotation and the 10-word savedEligible reload colour
 * are CLOSED, by one edit of two characters' worth of meaning: the byte store
 * reads the global back (`(u8)gO57ModeChoice4F8`) instead of reusing the local
 * `choice`.  ugen numbers a ring temp for that load and then forwards the value
 * it has just stored, so the load emits NO instruction -- the object is
 * byte-for-byte the same width -- while the ring advances one position in each
 * dispatch arm.  Measured with `cc -S`: the function's ring-temp count goes
 * 59 -> 61, the arm-1 selector load moves t4 -> t5 and the arm-2 one t3 -> t5,
 * and every later temp follows; 74 -> 21 masked words at delta 0.
 *
 * This is NOT a new family.  It is the read-back-what-you-just-wrote lever
 * already recorded twice in docs/ido-learnings.md -- the named
 * common-subexpression carrier on `func_8004E8E0`, where spelling a just-written
 * pointer chain as `array[0] = call(); array[1] = array[0] + size;` consumed an
 * invisible ugen temporary at the same instruction shape, and the known-zero
 * byte read in overlay 47's release routine, where reading a status byte back
 * kept one pop after the value folded away.  What is new here is only the
 * carrier: a plain 32-bit GLOBAL SCALAR, read back one line below its own
 * store, with the pop taken in each of two arms.  It is worth pairing with the
 * four families that buy a pop WITH an instruction (L65 phantom mask, L76 field
 * read through a local, L77 index scaled twice, L85 truncation at the store):
 * all four were tried here first and all four cost width.  On a plateau that is
 * exact-size, try the read-back before any of them.
 *
 * The safety condition the recorded lever carries applies unchanged and is met
 * here: no call, no volatile access and no aliasing write separates the store
 * from the read.
 *
 * The 21 that remain decompose exactly, and none of them is a ring question:
 *
 *   16  a v0/a0 swap in the marked-entry loop of both arms.  `entries` is
 *       coloured v0 in the target and a0 here; the dead post-decrement copy of
 *       `count` (`if (count--)`, whose `move` as1 makes dead by testing the
 *       pre-decrement register) takes the other one.  Both are free at that
 *       point -- `result` (v0) died at the second `result->child08` read and a0
 *       died at the preceding call argument -- so this is globalcolor's choice
 *       between two available colours for two interfering webs, and the lever
 *       is an extra interfering web, not a spelling.  Inert across 20 measured
 *       forms: all six declaration orders, `entries` assigned inside the test,
 *       `count` scoped to the loop or typed u32/u8, `entries[0].` member form,
 *       a `for` loop, `entries + 1` / `++entries` / u8-cursor arithmetic,
 *       commuted mask operands, and an s16 cast on the stored value.  `count`
 *       as s16 costs 24 bytes; hoisting the count read above the entries test
 *       costs 8.
 *    3  uopt sinks `savedEligible = eligible` past the early return and
 *       duplicates the store into both arms; the target stores once, in the
 *       delay slot of the timer branch.  CORRECTION, measured: this is NOT a
 *       memory-residency question.  savedEligible is ALREADY memory-resident
 *       in the candidate -- `sw a0,40(sp)` at +0x17c and +0x38c and nine
 *       `lw t3,40(sp)` reloads, the same home sp+0x28 the target uses.  What
 *       differs is only WHERE the one store goes: the target puts it in the
 *       `blez` delay slot at +0x15c, before the early return, and the
 *       candidate sinks it below that return into the first `jal` delay slot
 *       of each arm.  It is partial-dead-store sinking, not a missing home.
 *
 *       And the frame cost of forcing residency is a DECLARATION-POSITION
 *       artifact, not intrinsic.  `volatile s32 savedEligible` (or the
 *       one-element array form -- the two measure identically at every
 *       position) declared BETWEEN `timer` and `eligible` keeps the frame at
 *       0x30, keeps the home at 0x28, and lands the store exactly where the
 *       target has it: 34 masked.  Declared last it is 0x38/0x2C for 45;
 *       declared first, 0x38/0x2C for 43.  So the cell was never the
 *       obstacle.  What the middle position costs instead is 18 words of
 *       re-colouring in the timer region -- `eligible` a0 -> v1 and the
 *       gO57ModeTimer114 address web v1 -> a2 -- because taking savedEligible
 *       out of the web pool shortens `eligible`'s live range so it is coloured
 *       BEFORE the timer address web, where the target colours the address
 *       web first.  That swap, not a frame cell, is what has to be undone.
 *
 *       Sinking survives every source form tried: reading savedEligible rather
 *       than eligible in the guard, moving the assignment below the timer
 *       test, folding the test into one `&&`, a read-back, an `if (1)` region
 *       around the test, and replacing the early `return` with
 *       `goto dispatch_done` so the store is live on EVERY path (37, and it
 *       also loses the `beqzl`).  Merging savedEligible into `eligible` is
 *       byte-identical to the baseline at 21; wrapping it in a one-field
 *       struct is register-allocated and also measures 21 in two of the six
 *       declaration orders.
 *    2  the two address materialisations at +0x108 are emitted in the opposite
 *       order.
 *
 * Flags were screened on the old plateau and all tie or lose: -mips1 (234),
 * -O1 (431), -Olimit 0 (412), -O2 -g3 (89), loopunroll 0 and 4 (74).
 *
 * Measured negatives added on the 21-word plateau, none of which moved a word:
 *
 *   - L97 region boundaries at 18 placements.  The split is sharp and is the
 *     useful finding: EVERY boundary inside the marked-entry loop costs
 *     exactly +10 (`if (1)`/`do {} while (0)` around the do-while, around the
 *     count read, around the body, around the increment, and an empty marker
 *     before the body or the increment), and EVERY placement outside it is
 *     inert (around the entries assignment, the entries test, the selection
 *     block, the selector store, or as an empty marker before any of them).
 *     So the construct is live here -- it is not that uopt ignores it -- but
 *     it never demotes the dead copy.  Reading the inside-the-loop variant
 *     shows why: it rotates the three loop webs to deadcopy v0, entries v1,
 *     count a0, where the baseline is deadcopy v0, count v1, entries a0 and
 *     the TARGET is entries v0, count v1, deadcopy a0.  The boundary permutes
 *     entries against count and leaves the dead copy on the lowest colour.
 *     The target's order is exactly definition order; every candidate colours
 *     the dead copy first, which by L93 is the top-tested numbering.
 *   - loop shape: `while (count--)` and `for (; count--;)` are byte-identical
 *     to the manual `if (count--) { do ... while (count--); }` (21); spelling
 *     either test `!= 0` costs 10.
 *   - splitting the dead copy into one or two explicit user variables (`prev`,
 *     or `g0`/`g1` for the guard and the loop), declared before or after
 *     `count`: 21 or 31, never lower.
 *   - four more declaration orders of the arm's six locals, including entries
 *     first and entries last: 21.
 *   - the 2-word address order at +0x108: an explicit `s32 *` for the store
 *     base, a separate `next` temp for the subtraction, an `if (1)` region
 *     around the reload, a different reload base symbol, and hoisting the
 *     store into both arms of the timer test -- 21, 21, 21, 21 and worse. */
#ifdef NON_MATCHING
void overlay57UpdateModeState(s32 updateRate) {
    s32 timer;
    s32 eligible;
    s32 savedEligible;

    O57_S32(0x144) = 1;
    {
        s32 *list;
        u8 id;

        if (gO57ModeCountdown120 > 0) {
            if ((gO57ModeCountdown120 = O57_SUB_WRAP(gO57ModeCountdown120, updateRate)) > 0) {
                return;
            }

            list = gO57ModePrimaryIds134;
            while (*list != -1) {
                id = (u8)*list;
                overlay57Call3AA4Reloc(id);
                list++;
            }

            list = gO57ModeSecondaryIds1A0;
            while (*list != -1) {
                id = (u8)*list;
                overlay57Call3AE0Reloc(id);
                overlay57SetNodeValue(*list, 1, 0.0079f);
                list++;
            }

            gO57ModeTimerEarlyStoreBaseReloc[0x114 / 4] = 60;
            O57_S32(0x11C) = 0;
            return;
        }
    }

    {
        timer = gO57ModeTimer114;
        if ((timer >= 31) && (O57_SUB_WRAP(timer, updateRate) < 31)) {
            overlay57Call3B4CReloc(0x10, 0);
            timer = gO57ModeTimerReloadBaseReloc[0x114 / 4];
        }
        gO57ModeTimer114 = O57_SUB_WRAP(timer, updateRate);
    }

    eligible = (gOverlay57Flags3B6CReloc & 0xD000) != 0;
    if (eligible != 0) {
        eligible = gO57ModeGate50C[0] == 0;
    }
    savedEligible = eligible;
    if (gO57ModeTimer114 > 0) {
        if (eligible == 0) {
            return;
        }
    }

    if (gOverlay57Mode3B88Reloc != 1) {
        goto mode_b;
    }
        if (overlay57Call3BC4Reloc() == 8) {
            overlay57Call3BECReloc(0x2A, 0, 0, 0xE, 1, 0);
            gO57ModeGate50C[0] = 1;
            overlay57Call3BF8Reloc();
        } else if (overlay57Call3C08Reloc() == 5) {
            O57_S32(0x118) = 9;
            O57_S32(0x164) = 0;
            O57_S32(0x160) = 0;
            O57_S32(0x184) = 0x2F;
            gO57ModeTimerA5BaseReloc[0x114 / 4] = 0xB4;
            overlay57Call3C4CReloc(1);
        } else if (overlay57Call3C5CReloc() == 6) {
            O57_S32(0x118) = 12;
            gO57ModeTimerA6BaseReloc[0x114 / 4] = 0xB4;
            overlay57Call3C84Reloc(2);
        } else if (overlay57Call3C94Reloc() == 7) {
            O57_S32(0x118) = 21;
            O57_S32(0x160) = 0;
            O57_S32(0x164) = 0;
            O57_S32(0x184) = 0x4B;
            gO57ModeTimerA7BaseReloc[0x114 / 4] = 0xB4;
            overlay57Call3CDCReloc(1);
        } else {
            Overlay57LookupResult *result;
            Overlay57SelectionChild *child;
            Overlay57Selection *selection;
            Overlay57MarkedEntry *entries;
            s32 choice;
            s32 count;

            O57_S32(0x118) = 15;
            gO57ModeSetup21C.x0C = 0x17C;
            gO57ModeSetup21C.y0E = 0xBE;
            choice = overlay57Call3D08Reloc();
            gO57ModeChoice4F8 = choice;
            gOverlay57Byte3D1CReloc = (u8)gO57ModeChoice4F8;
            overlay57Call3D24Reloc(0x4D);
            overlay57UpdateNode();
            result = overlay57Call3D34Reloc(0x4C);
            if (result != 0) {
                child = result->child08;
                if (child != 0) {
                    child->selector3A = (s8)gO57ModeChoice4F8;
                    child = result->child08;
                    selection = child->selections68[child->selector3A];
                    if (selection != 0) {
                        entries = selection->entries4C;
                        if (entries != 0) {
                            count = selection->countOwner00->count2C;
                            if (count--) {
                                do {
                                    if ((entries->flags04 & 0x00100000) != 0) {
                                        entries->value00 = 0x100;
                                    }
                                    entries++;
                                } while (count--);
                            }
                        }
                    }
                }
            }
            overlay57Call3DC4Reloc(3);
        }
    goto dispatch_done;

mode_b:
        if (overlay57Call3DD4Reloc() == 8) {
            overlay57Call3DFCReloc(0x2A, 0, 0, 0xE, 1, 0);
            gO57ModeGate50C[0] = 1;
            overlay57Call3E08Reloc();
        } else if (overlay57Call3E18Reloc() == 5) {
            O57_S32(0x118) = 9;
            O57_S32(0x164) = 0;
            O57_S32(0x160) = 0;
            O57_S32(0x184) = 0x2F;
            gO57ModeTimerB5BaseReloc[0x114 / 4] = 0xB4;
            overlay57Call3E5CReloc(1);
        } else if (overlay57Call3E6CReloc() == 6) {
            O57_S32(0x118) = 21;
            O57_S32(0x160) = 0;
            O57_S32(0x164) = 0;
            O57_S32(0x184) = 0x4B;
            gO57ModeTimerB6BaseReloc[0x114 / 4] = 0xB4;
            overlay57Call3EB4Reloc(1);
        } else {
            Overlay57LookupResult *result;
            Overlay57SelectionChild *child;
            Overlay57Selection *selection;
            Overlay57MarkedEntry *entries;
            s32 choice;
            s32 count;

            O57_S32(0x118) = 15;
            gO57ModeSetup21C.x0C = 0x17C;
            gO57ModeSetup21C.y0E = 0xBE;
            choice = overlay57Call3EE0Reloc();
            gO57ModeChoice4F8 = choice;
            gOverlay57Byte3EF4Reloc = (u8)gO57ModeChoice4F8;
            overlay57Call3EFCReloc(0x4D);
            overlay57UpdateNode();
            result = overlay57Call3F0CReloc(0x4C);
            if (result != 0) {
                child = result->child08;
                if (child != 0) {
                    child->selector3A = (s8)gO57ModeChoice4F8;
                    child = result->child08;
                    selection = child->selections68[child->selector3A];
                    if (selection != 0) {
                        entries = selection->entries4C;
                        if (entries != 0) {
                            count = selection->countOwner00->count2C;
                            if (count--) {
                                do {
                                    if ((entries->flags04 & 0x00100000) != 0) {
                                        entries->value00 = 0x100;
                                    }
                                    entries++;
                                } while (count--);
                            }
                        }
                    }
                }
                overlay57ApplyTable();
            }
            overlay57Call3FA4Reloc(3);
        }
dispatch_done:

    if (savedEligible != 0) {
        overlay57Call3FB8Reloc();
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o057/overlay57UpdateModeState/func_overlay_057_F0003A4C_18A7644.s")
#endif

/* PLATEAU-HANDOFF:overlay57UpdateModeState:start
 * symbol: overlay57UpdateModeState
 * score: 21/354 words
 * frame: 0x30
 * relocations: 59
 * first-mismatch: +0x108
 * summary: 74 fell to 21 by reading the global back in the byte store (u8)gO57ModeChoice4F8 instead of the local choice, so ugen numbers a ring temp for a load it forwards from the store above it and the pop costs zero instructions; the 21 that remain are 16 words of a globalcolor swap between entries and the dead post-decrement copy, 3 words of a store uopt sinks, and 2 words of address order, and this pass corrects the 3: savedEligible is already memory-resident at sp+0x28 in the candidate so it is partial-dead-store sinking rather than a missing home, and the frame cost of forcing residency is a declaration-position artifact -- volatile or a one-element array declared BETWEEN timer and eligible keeps frame 0x30 and home 0x28 and lands the store, paying instead 18 words of re-colouring in the timer region because eligible loses its long live range and is coloured before the address web.
 * PLATEAU-HANDOFF:overlay57UpdateModeState:end
 */
