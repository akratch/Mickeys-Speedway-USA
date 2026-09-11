#include "PR/ultratypes.h"

typedef struct O101TailC6E8Root {
    s32 chainType;
    void *chain;
    u8 kind;
    u8 pad09;
    s16 value0A;
    s16 value0C;
    s16 x;
    s16 y;
    s16 width;
    s16 height;
    u8 color0;
    u8 color1;
    void *asset;
} O101TailC6E8Root;

typedef struct O101TailC6E8Node32 {
    s32 previousType;
    void *previous;
    s16 x;
    s16 y;
    f32 scale;
    s16 value10;
    u8 color0;
    u8 color1;
    f32 value14;
    s32 value18;
    void *handle;
} O101TailC6E8Node32;

typedef struct O101TailC6E8Node20 {
    s32 previousType;
    void *previous;
    s16 x;
    s16 y;
    f32 scale;
    void *handle;
} O101TailC6E8Node20;

extern s32 gO101TailC6E8QueueCount;
extern u8 gO101TailC6E8Selector;
extern u8 gO101TailC6E8QueueBytes[];
extern s32 gO101TailC6E8PresentationActive;
extern s32 gO101TailC6E8PresentationDone;
extern O101TailC6E8Root gO101TailC6E8Root;
extern s32 gO101TailC6E8OrderCount;
extern O101TailC6E8Root *gO101TailC6E8OrderSlots[];
extern s32 gO101TailC6E8Node32Count;
extern O101TailC6E8Node32 gO101TailC6E8Nodes32[];
extern s32 gO101TailC6E8Node20Count;
extern O101TailC6E8Node20 gO101TailC6E8Nodes20[];
extern u8 gO101TailC6E8AssetDF0;
extern void *gO101TailC6E8Handle1D4;
extern void *gO101TailC6E8Handle1F4;
extern void *gO101TailC6E8Handle33C;
extern void *gO101TailC6E8Handle338;

extern s32 o101TailC6E8StateReloc(void);
extern void o101TailC6E8SpecialReloc();
extern void o101TailC6E8Block00Reloc();
extern void o101TailC6E8Block01Reloc();
extern void o101TailC6E8Block02Reloc();
extern void o101TailC6E8Block03Reloc();
extern void o101TailC6E8Block04Reloc();
extern void o101TailC6E8Block05Reloc();
extern void o101TailC6E8Block06Reloc();
extern void o101TailC6E8Block07Reloc();
extern void o101TailC6E8Block08Reloc();
extern void o101TailC6E8Block09Reloc();
extern void overlay101BuildPresentationA(void);
extern void o101TailC6E8Block11Reloc();
extern void o101TailC6E8Block12Reloc();
extern void o101TailC6E8Block13Reloc();
extern void o101TailC6E8Block14Reloc();
extern void o101TailC6E8Block15Reloc();
extern void o101TailC6E8Block16Reloc();
extern void o101TailC6E8Block17Reloc();
extern void o101TailC6E8Block18Reloc();
extern void o101TailC6E8Block19Reloc();
extern void o101TailC6E8Block20Reloc();
extern void o101TailC6E8Block21Reloc();
extern void o101TailC6E8Block22Reloc();
extern void o101TailC6E8Block23Reloc();
extern void o101TailC6E8Block24Reloc();
extern void o101TailC6E8DefaultReloc();
extern void *o101TailC6E8Create8FReloc();
extern void *o101TailC6E8Create90Reloc();
extern void *o101TailC6E8CreateCompactReloc();
extern void *o101TailC6E8Acquire5F1Reloc();
extern void *o101TailC6E8Acquire5F2Reloc();
extern void *o101TailC6E8Acquire58Reloc();
extern void *o101TailC6E8Acquire338Reloc();
extern void o101TailC6E8Tail27Reloc();
extern void o101TailC6E8TailFinalReloc();

/*
 * The retained 25-entry table at overlay-local +0xEAC maps selector values
 * 0..24 to these lexical cases in order. Atlas runtime records preserve the
 * distinct opaque call identities even though the split object uses one raw
 * static carrier for all calls.
 */
/* P2 plateau: workbench mixed(constant:7, structural:48, schedule:33, register:81).
 * Reversing the queue-end pointer addition closes one commutative/register word; mips2 retains the exact size and 0x18 frame.
 * First raw mismatch is the opaque call at +0x8.
 *
 * Lane c3-o101 (2026-09-10): 124 -> 119 relocation-masked words, first masked
 * mismatch +0x34 -> +0x4C, size and frame untouched. Both edits are uopt
 * REGION boundaries -- the mechanism that closed overlay101BuildBorder
 * outright -- and each empty `if (1) { }` is worth 3 and 2 words respectively.
 * This function's residue is a global colouring shift, not a shape problem:
 * the target holds `selector` in a1 and the queue-byte base in t7 where this
 * candidate holds them in a2 and a1, and the root-initialisation region's
 * three hoisted webs sit at t3/t4/t5 against the target's t0/t1/t2, which is
 * what makes ugen recycle t6..t9 across the seven root constants and leaves
 * as1 the anti-dependences that produce the interleaved li/sh pattern.
 * Falsified, each measured: seven orderings of the twelve root-field stores
 * all measure exactly 124 masked (the store order is a degenerate dimension
 * for this metric even though it moves the register-blind count between 45
 * and 49); six head rewrites of the selector/queue block are all >= 124, and
 * `end = cursor + count` and `end = count + gO101TailC6E8QueueBytes` both cost
 * one word against the retained `count + cursor`. Reopen on the colouring, not
 * on statement order.
 *
 * Lane p2-close (2026-09-11): 119 -> 106 masked at unchanged size and 0x18
 * frame, immediate-only words 2 -> 0 and structural words 31 -> 26.  Two
 * independent levers, both cheap and both reusable:
 *
 * 1. L59 group folds.  The builders here emit each per-element assignment
 *    group in a fixed order and as1 breaks the resulting ties on physical
 *    line number, so folding a whole group onto ONE line retires the tie.  All
 *    1,024 subsets of the ten foldable groups were measured twice, once on the
 *    plain baseline and once on top of lever 2; the paying set is the second
 *    node32 assignment group, the second chain fixup, the node20 assignment
 *    group and the node20 chain fixup.  The root-field group, the order-slot
 *    pair, the first node32 group and the first chain fixup are inert, and the
 *    two handle pairs regress.  A five-pattern greedy refinement over each
 *    group -- none, fold all, fold pairs, fold first half, fold second half --
 *    finds nothing better than fold-all on those four.
 * 2. `*cursor = cursor[1] & 0xFF;`.  The mask is a no-op on a `u8 *`, the
 *    `andi` after a `lbu` is removed by the peephole, and the object is the
 *    same size -- but cfe still emits the operation, so ugen allocates one
 *    more expression temporary in that block and the whole caller-saved free
 *    list moves one place.  Worth 5 words on its own.  Doubling or tripling
 *    the mask regresses (118, 119), and the same trick on `selector` or on the
 *    switch costs four bytes.
 *
 * The residue is still register naming: 75 of the 106 words.  Reopen on the
 * root-region colouring, and re-derive the web numbers before forcing. */
#ifdef NON_MATCHING
void func_overlay_101_F000C6E8_18E7F08(void) {
    s32 count;
    s32 orderIndex;
    s32 nodeIndex;
    s32 priorNodeIndex;
    u8 selector;
    void *handle;
    O101TailC6E8Node32 *node32;
    O101TailC6E8Node20 *node20;
    s32 previousType;
    void *previous;
    u8 *cursor;
    u8 *end;

    if (o101TailC6E8StateReloc() == 0x12) {
        o101TailC6E8SpecialReloc(0);
        goto common_tail;
    }

    selector = gO101TailC6E8Selector;
    count = gO101TailC6E8QueueCount;
    if (count > 0) {
        gO101TailC6E8QueueCount--;
        if (gO101TailC6E8QueueCount > 0) {
            cursor = gO101TailC6E8QueueBytes;
            if (1) {
            }
            end = gO101TailC6E8QueueCount + cursor;
            do {
                *cursor = cursor[1] & 0xFF;
                cursor++;
            } while (cursor < end);
        }
    }

    switch (selector) {
        case 0:  o101TailC6E8Block00Reloc(); break;
        case 1:  o101TailC6E8Block01Reloc(); break;
        case 2:  o101TailC6E8Block02Reloc(); break;
        case 3:  o101TailC6E8Block03Reloc(); break;
        case 4:  o101TailC6E8Block04Reloc(); break;
        case 5:  o101TailC6E8Block05Reloc(); break;
        case 6:  o101TailC6E8Block06Reloc(); break;
        case 7:  o101TailC6E8Block07Reloc(); break;
        case 8:  o101TailC6E8Block08Reloc(); break;
        case 9:  o101TailC6E8Block09Reloc(); break;
        case 10: overlay101BuildPresentationA(); break;
        case 11: o101TailC6E8Block11Reloc(); break;
        case 12: o101TailC6E8Block12Reloc(); break;
        case 13: o101TailC6E8Block13Reloc(); break;
        case 14: o101TailC6E8Block14Reloc(0); break;
        case 15: o101TailC6E8Block15Reloc(1); break;
        case 16: o101TailC6E8Block16Reloc(2); break;
        case 17: o101TailC6E8Block17Reloc(3); break;
        case 18: o101TailC6E8Block18Reloc(4); break;
        case 19: o101TailC6E8Block19Reloc(5); break;
        case 20: o101TailC6E8Block20Reloc(); break;
        case 21: o101TailC6E8Block21Reloc(); break;
        case 22: o101TailC6E8Block22Reloc(0); break;
        case 23: o101TailC6E8Block23Reloc(1); break;
        case 24: o101TailC6E8Block24Reloc(); break;
        default: o101TailC6E8DefaultReloc(0); break;
    }

    if (gO101TailC6E8PresentationActive != 0) {
        orderIndex = gO101TailC6E8OrderCount;
        gO101TailC6E8Root.height = 0x120;
        gO101TailC6E8Root.width = 0x180;
        gO101TailC6E8Root.y = -0x18;
        gO101TailC6E8Root.x = -0x20;
        gO101TailC6E8Root.kind = 2;
        gO101TailC6E8Root.asset = &gO101TailC6E8AssetDF0;
        gO101TailC6E8Root.color0 = 0xFF;
        gO101TailC6E8Root.color1 = 0xFF;
        gO101TailC6E8Root.value0A = 0;
        gO101TailC6E8Root.value0C = 0;
        gO101TailC6E8Root.chainType = 0;
        gO101TailC6E8Root.chain = NULL;
        gO101TailC6E8OrderSlots[orderIndex] = &gO101TailC6E8Root;
        gO101TailC6E8OrderCount = orderIndex + 1;

        nodeIndex = gO101TailC6E8Node32Count;
        node32 = &gO101TailC6E8Nodes32[nodeIndex];
        node32->x = 0x35;
        node32->y = 0xE0;
        node32->scale = 1.0f;
        node32->value10 = 0;
        node32->color0 = 0xFF;
        node32->color1 = 0;
        node32->value14 = 0.0f;
        node32->value18 = 0;
        handle = o101TailC6E8Create8FReloc(
            0x8F, NULL, &gO101TailC6E8Root,
            &gO101TailC6E8OrderCount);
        nodeIndex = gO101TailC6E8Node32Count;
        node32 = &gO101TailC6E8Nodes32[nodeIndex];
        previousType = gO101TailC6E8Root.chainType;
        previous = gO101TailC6E8Root.chain;
        gO101TailC6E8Root.chainType = 2;
        gO101TailC6E8Root.chain = node32;
        node32->previousType = previousType;
        node32->previous = previous;
        node32->handle = handle;
        gO101TailC6E8Node32Count = nodeIndex + 1;

        priorNodeIndex = nodeIndex;
        nodeIndex = gO101TailC6E8Node32Count;
        node32 = &gO101TailC6E8Nodes32[nodeIndex];
        node32->x = 0xC0; node32->y = 0xE0; node32->scale = 1.0f; node32->value10 = 0; node32->color0 = 0xFF; node32->color1 = 0; node32->value14 = 0.0f; node32->value18 = 0;
        handle = o101TailC6E8Create90Reloc(
            0x90, NULL, &gO101TailC6E8Root, priorNodeIndex);
        nodeIndex = gO101TailC6E8Node32Count;
        node32 = &gO101TailC6E8Nodes32[nodeIndex];
        previousType = gO101TailC6E8Root.chainType; previous = gO101TailC6E8Root.chain; gO101TailC6E8Root.chainType = 2; gO101TailC6E8Root.chain = node32; node32->previousType = previousType; node32->previous = previous; node32->handle = handle; gO101TailC6E8Node32Count = nodeIndex + 1;

        priorNodeIndex = nodeIndex;
        nodeIndex = gO101TailC6E8Node20Count;
        node20 = &gO101TailC6E8Nodes20[nodeIndex];
        node20->x = 0x20; node20->y = 0x18; node20->scale = 1.0f;
        handle = o101TailC6E8CreateCompactReloc(
            1, node20, &gO101TailC6E8Root, priorNodeIndex);
        if (1) {
        }
        nodeIndex = gO101TailC6E8Node20Count;
        node20 = &gO101TailC6E8Nodes20[nodeIndex];
        previousType = gO101TailC6E8Root.chainType; previous = gO101TailC6E8Root.chain; gO101TailC6E8Root.chainType = 1; gO101TailC6E8Root.chain = node20; node20->previousType = previousType; node20->previous = previous; node20->handle = handle; gO101TailC6E8Node20Count = nodeIndex + 1;

        gO101TailC6E8Handle1D4 = o101TailC6E8Acquire5F1Reloc(0x5F1);
        gO101TailC6E8Handle1F4 = o101TailC6E8Acquire5F2Reloc(0x5F2);
    }

    gO101TailC6E8Handle33C = o101TailC6E8Acquire58Reloc(0x58);
    gO101TailC6E8Handle338 =
        o101TailC6E8Acquire338Reloc(0, 0x278D00);

common_tail:
    gO101TailC6E8PresentationDone = 0;
    o101TailC6E8Tail27Reloc(0x27);
    o101TailC6E8TailFinalReloc();
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/overlay101TailC6E8/func_overlay_101_F000C6E8_18E7F08.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_101_F000C6E8_18E7F08:start
 * symbol: func_overlay_101_F000C6E8_18E7F08
 * score: 106 differing words
 * frame: 0x18
 * relocations: 91
 * first-mismatch: +0x4C
 * summary: Two uopt region boundaries took 124 masked words to 119 at exact size and 0x18 frame. 2026-09-11, lane p2-close, 119 -> 106 with immediate-only words 2 -> 0 and structural 31 -> 26: four L59 group folds, chosen by measuring all 1,024 subsets of the ten foldable assignment groups, plus a no-op `& 0xFF` on the queue copy, which folds away at zero byte cost but still makes ugen allocate one more expression temporary and so moves the caller-saved free list one place. The shard's older claim that the target holds `selector` in a1 and this candidate a2 is stale: both hold it in a1 now, and the visible difference is the queue-byte base, t7 there against a2 here. The residue is 75 register-naming words, and it is p1 colouring in the root-initialisation region.
 * PLATEAU-HANDOFF:func_overlay_101_F000C6E8_18E7F08:end
 */
