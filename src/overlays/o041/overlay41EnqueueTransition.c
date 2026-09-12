#include "PR/ultratypes.h"

typedef struct Overlay41QueueEntry {
    s16 timer;
    s16 value2;
    s16 value4;
    s16 value6;
    s8 value8;
    s8 value9;
    s8 valueA;
    u8 active;
} Overlay41QueueEntry;

extern u8 gOverlay41QueueActive0Read[];
extern u8 gOverlay41QueueTimer0Read[];
extern u8 gOverlay41QueueTimer0Write[];
extern u8 gOverlay41QueueValue2_0[];
extern u8 gOverlay41QueueValue4_0[];
extern u8 gOverlay41QueueValue6_0[];
extern u8 gOverlay41QueueValue8_0[];
extern u8 gOverlay41QueueValue9_0[];
extern u8 gOverlay41QueueValueA_0[];
extern u8 gOverlay41QueueActive0Write[];
extern u8 gOverlay41QueueEntries[];

#define entry ((volatile u8 *)cursor)

/* 29/105 differing words, exact 105 instructions, frameless, size delta 0.  All 22 relocation
 * identities are proven.  Located 2026-09-12 (lane p9-mid): read positionally rather than through
 * the shape aligner, the two streams agree from +0xC4 on, and everything between +0x74 and +0xC0 is
 * the same sequence offset by one word.  The candidate emits the second cursor advance at +0x78,
 * ahead of the entry block; the shipped code emits it at +0xC0, after that block's return delay
 * slot.  Hoisted, the advance cannot be an in-place update, so this candidate carries a base cursor
 * and an advanced cursor in two registers where the shipped code carries one and updates it in
 * place -- and all 13 v1-to-v0 substitutions are that one fact.  The tell is the two delay-slot
 * loads at +0x7C and +0x88: the shipped code reads the next entry's active byte at displacement 23
 * off the un-advanced base, folding the +12 in, which is what leaves the advance free to sink.
 * So the decision variable is what makes uopt reassociate a pointer advance into the following
 * access's displacement instead of materialising the advanced pointer (L107 family), and it is not
 * a lifetime or a pool question.  This function is a leaf, its records are p2 only, and forcing the
 * two cursor webs to swap colours scores 56 against 29.  See the handoff for the axes covered. */
#ifdef NON_MATCHING
void func_overlay_041_F000195C_1888C94(s32 value2, s32 timer, s32 value4,
                                       s32 value6, s32 value8, s32 value9,
                                       s32 valueA) {
    u32 cursor;

    cursor = (u32)gOverlay41QueueEntries;
    if (gOverlay41QueueActive0Read[11] == 0 &&
        *(s16 *)(gOverlay41QueueTimer0Read + 0) <= 0) {
        *(s16 *)(gOverlay41QueueValue2_0 + 2) = value2;
        *(s16 *)(gOverlay41QueueTimer0Write + 0) = timer;
        *(s16 *)(gOverlay41QueueValue4_0 + 4) = value4;
        *(s16 *)(gOverlay41QueueValue6_0 + 6) = value6;
        gOverlay41QueueValue8_0[8] = value8;
        gOverlay41QueueValue9_0[9] = value9;
        gOverlay41QueueValueA_0[10] = valueA;
        gOverlay41QueueActive0Write[11] = 0;
        return;
    } else if (entry[11] == 0 && *(s16 *)(entry + 0) <= 0) {
        *(s16 *)(entry + 2) = value2;
        *(s16 *)(entry + 0) = timer;
        *(s16 *)(entry + 4) = value4;
        *(s16 *)(entry + 6) = value6;
        entry[8] = value8;
        entry[9] = value9;
        entry[11] = 0;
        entry[10] = valueA;
        return;
    }
    cursor += 12;
    if (entry[11] == 0 && *(s16 *)(entry + 0) <= 0) {
        *(s16 *)(entry + 2) = value2;
        *(s16 *)(entry + 0) = timer;
        *(s16 *)(entry + 4) = value4;
        *(s16 *)(entry + 6) = value6;
        entry[8] = value8;
        entry[9] = value9;
        entry[11] = 0;
        entry[10] = valueA;
        return;
    }
    cursor += 12;
    if (entry[11] == 0 && *(s16 *)(entry + 0) <= 0) {
        *(s16 *)(entry + 2) = value2;
        *(s16 *)(entry + 0) = timer;
        *(s16 *)(entry + 4) = value4;
        *(s16 *)(entry + 6) = value6;
        entry[8] = value8;
        entry[9] = value9;
        entry[11] = 0;
        entry[10] = valueA;
        return;
    }
    cursor += 12;
    if (entry[11] == 0 && *(s16 *)(entry + 0) <= 0) {
        *(s16 *)(entry + 2) = value2;
        *(s16 *)(entry + 0) = timer;
        *(s16 *)(entry + 4) = value4;
        *(s16 *)(entry + 6) = value6;
        entry[8] = value8;
        entry[9] = value9;
        entry[11] = 0;
        entry[10] = valueA;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o041/overlay41EnqueueTransition/func_overlay_041_F000195C_1888C94.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_041_F000195C_1888C94:start
 * symbol: func_overlay_041_F000195C_1888C94
 * score: 29/105 words
 * frame: frameless
 * relocations: 22
 * first-mismatch: +0x8
 * summary: Leaf cursor-definition placement regressed to 30 words; exact relocation identity leaves only an authentic cursor-lifetime decision.
 * PLATEAU-HANDOFF:func_overlay_041_F000195C_1888C94:end
 */
