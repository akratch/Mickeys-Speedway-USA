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
 *       delay slot of the timer branch.  This is a MEMORY-RESIDENCY question,
 *       not a placement one, and it is answered: forcing savedEligible to
 *       memory (`volatile`, a one-element array, or storing through a taken
 *       address) puts the store exactly where the target has it.  All three
 *       cost a frame cell -- 0x30 becomes 0x38 and the home moves 0x28 -> 0x2C
 *       -- which costs more than the 3 words it buys (45 masked).  Freeing a
 *       cell by scoping `timer` into the timer block keeps the frame at 0x30
 *       and lands the store, but re-colours the eligibility and address webs in
 *       the timer region for 18 words (34 masked); merging `timer` and
 *       `eligible` into one variable is the best of that family at 28.  So a
 *       cell has to come free WITHOUT perturbing the timer block.
 *    2  the two address materialisations at +0x108 are emitted in the opposite
 *       order.
 *
 * Flags were screened on the old plateau and all tie or lose: -mips1 (234),
 * -O1 (431), -Olimit 0 (412), -O2 -g3 (89), loopunroll 0 and 4 (74). */
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
 * summary: The 43-word ring rotation and the 10-word savedEligible reload colour are closed by reading the global back in the byte store (u8)gO57ModeChoice4F8 instead of the local choice, which makes ugen number a ring temp for a load it then forwards from the store one line above, so the pop costs zero instructions and 74 falls to 21 at delta 0; what is left is 16 words of a globalcolor v0/a0 swap between entries and the dead post-decrement copy, 3 words of a store uopt sinks unless savedEligible is memory-resident which costs a frame cell, and 2 words of address order.
 * PLATEAU-HANDOFF:overlay57UpdateModeState:end
 */
