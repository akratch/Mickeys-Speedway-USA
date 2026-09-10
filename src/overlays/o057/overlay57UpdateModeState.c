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

/* Plateau: exact-size at 0x588, 5 masked words, first at +0x108.
 *
 * 74 -> 21 -> 5.  The 16-word globalcolor cluster is CLOSED, and it took two
 * edits that are each a regression alone ([L88]): `entries` declared ONCE at
 * function scope instead of once per dispatch arm (alone: 31), and the dead
 * post-decrement copy given an explicit per-arm name `prev` (alone: 31).
 * Together: 5.
 *
 * The decision variable is uopt's colouring ORDER, and it is computable.  Every
 * `globalcolor` candidate carries `save = totalsave / nocs`, where `totalsave`
 * is the web's reference count with references inside a loop weighted x10 and
 * `nocs` is a bucket that grows with the web's span; webs are coloured by
 * DESCENDING save, and each takes the lowest colour not held by an already
 * coloured interfering web.  In this loop the three webs are `entries`, `count`
 * and the dead copy, and the target's assignment (entries v0, count v1, dead
 * copy a0) is exactly definition order -- which means the target's saves must
 * run entries > count > dead copy.  Measured, per arm: entries 42/3 = 14,
 * count 34/2 = 17, dead copy 44/2 = 22, because ONE cfe temporary serves the
 * post-decrement in BOTH arms while `entries` and `count` are separate
 * block-scoped symbols in each.  That is the exact reverse of what the target
 * needs, which is why 20 spellings of the loop moved nothing.
 *
 * Both halves of the reversal are bought by moving a symbol boundary:
 *
 *   - Hoisting `entries` to function scope merges the two arms' pointers into
 *     one web: totalsave 42 -> 84, nocs 3 -> 4, save 14 -> 21.
 *   - Naming the dead copy `prev` inside each arm splits the shared cfe
 *     temporary into two per-arm webs: totalsave 44 -> 22, save 22 -> 11.
 *
 * 21 > 17 > 11 is the target's order, and the colours follow without a force.
 * Neither edit alone reorders the trio: hoisting alone gives entries 21 > dead
 * copy 22 -> entries v1; splitting alone gives count 17 > entries 14 > dead
 * copy 11 -> count v0.  Hoisting `count` as well is inert (its merged web is
 * 68/4 = 17, the same save), and hoisting all six arm locals is 31.
 *
 * This is the general lever for a globalcolor residual on a pressure-free
 * procedure and it costs no width at all: a web's save is changed by changing
 * which SYMBOL its references belong to.  Merging two block-scoped copies of a
 * variable into one function-scope declaration raises save; giving a compiler
 * temporary an explicit per-block name lowers it.  Both are free.
 *
 * The 5 that remain are the two terms this pass did not reach:
 *
 *    3  uopt sinks `savedEligible = eligible` past the early return and
 *       duplicates the store into both arms' first `jal` delay slot; the target
 *       stores once, in the `blez` delay slot at +0x15c.  savedEligible is
 *       already memory-resident at sp+0x28 with the target's own home, so this
 *       is partial-dead-store sinking, not a missing home.  Forcing residency
 *       lands the store exactly (`volatile s32 savedEligible` declared between
 *       `timer` and `eligible`) but costs 18: `eligible`'s web stops spanning
 *       the dispatch, its nocs falls 19 -> 2, its save rises 0.26 -> 2.5, and it
 *       is then coloured BEFORE the gO57ModeTimer114 address web and takes its
 *       v1.  Raising that address web above 2.5 needs six more references to it
 *       and there are only three sites, so the volatile route is closed by the
 *       same save arithmetic that opened the loop.  Inert at 5: all 24
 *       declaration orders of the four function-scope locals, merging
 *       savedEligible into `eligible`, reading savedEligible in the guard,
 *       folding the two tests into one `&&`, storing inside the timer test, a
 *       read-back, and seven L97 region boundaries around the assignment, the
 *       eligibility block, the timer test and the whole dispatch.
 *    2  the two address materialisations at +0x108 are emitted in the opposite
 *       order.  Both are in the one basic block after the `jal`: `la $3,
 *       gO57ModeTimer114` (uopt's rematerialisation of the caller-saved address
 *       web, .loc = the call's line) and `lw $2, gO57ModeTimerReloadBaseReloc +
 *       276` (.loc = the next line).  as1 breaks the scheduling tie on physical
 *       source line ([L59]) and emits the lower line first, so the target needs
 *       the reload to carry a line <= the call's -- which no legal statement
 *       order supplies.  Putting both on ONE line does move the scheduler (the
 *       `lw` then beats the `addiu`) but not the two `lui`s: 7.  Also measured:
 *       a comma expression 7, nested ifs 5, a blank line 5, an L97 region
 *       around the reload 5, a multi-line call 5, a multi-line reload 5, and an
 *       explicit `s32 *timerPtr` (with or without a post-call reassignment)
 *       delta -8 and 276.
 *
 * Earlier, and still true: 74 fell to 21 by reading the global back in the byte
 * store (`(u8)gO57ModeChoice4F8` instead of the local `choice`), so ugen numbers
 * a ring temp for a load it forwards from the store above it and the pop costs
 * zero instructions.  That is the read-back-what-you-just-wrote lever already
 * recorded twice in docs/ido-learnings.md; what was new was the carrier, a plain
 * 32-bit global scalar read back one line below its own store.  Its safety
 * condition holds here: no call, no volatile access and no aliasing write
 * separates the store from the read.
 *
 * Flags were screened on the old plateau and all tie or lose: -mips1 (234),
 * -O1 (431), -Olimit 0 (412), -O2 -g3 (89), loopunroll 0 and 4 (74). */
#ifdef NON_MATCHING
void overlay57UpdateModeState(s32 updateRate) {
    s32 timer;
    s32 eligible;
    s32 savedEligible;
    Overlay57MarkedEntry *entries;

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
            s32 choice;
            s32 count;
            s32 prev;

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
                            prev = count--;
                            if (prev) {
                                do {
                                    if ((entries->flags04 & 0x00100000) != 0) {
                                        entries->value00 = 0x100;
                                    }
                                    entries++;
                                    prev = count--;
                                } while (prev);
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
            s32 choice;
            s32 count;
            s32 prev;

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
                            prev = count--;
                            if (prev) {
                                do {
                                    if ((entries->flags04 & 0x00100000) != 0) {
                                        entries->value00 = 0x100;
                                    }
                                    entries++;
                                    prev = count--;
                                } while (prev);
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
 * score: 5/354 words
 * frame: 0x30
 * relocations: 59
 * first-mismatch: +0x108
 * summary: 21 fell to 5 by reordering uopt's colouring, not by respelling the loop: globalcolor takes webs in descending save = totalsave/nocs (references, x10 inside a loop, over a span bucket), and the target's entries v0 / count v1 / dead-copy a0 is exactly the order entries > count > dead copy, while the candidate measured 14 < 17 < 22 because one cfe temporary served the post-decrement in BOTH arms and entries and count were separate block-scoped symbols in each; declaring entries ONCE at function scope merges its two webs (save 14 -> 21) and naming the dead copy prev per arm splits its shared web (save 22 -> 11), each a 31-word regression alone and 5 together; the 5 left are 3 words of a store uopt sinks past the early return (forcing residency lands it but re-colours 18 by raising eligible's save above the timer address web's) and 2 words of an as1 line-order tie at +0x108 that no legal statement order can reverse.
 * PLATEAU-HANDOFF:overlay57UpdateModeState:end
 */
