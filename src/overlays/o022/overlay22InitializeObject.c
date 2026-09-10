#include "PR/ultratypes.h"

#define FIELD(base, type, off) (*(type *)((u8 *)(base) + (off)))

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

extern f32 D_EE0;
extern u8 D_A7C[];
extern void *D_0[];
extern s32 D_30;
extern void func_80029A24(void *, Vec3f *);
extern void func_800150F0(s32, Vec3f *, Vec3f *, f32 *, void *, s32);
extern s32 func_800104B0(Vec3f *, Vec3f *, f32, void *, void *);
extern void func_8003E99C(void *, s32);
extern void func_80006A50(void *);
extern s16 func_8000F690(f32, f32, f32);
extern void func_overlay_022_F0000D30_1878E38(void *, s32, s32 *);

/* Configured remeasurement retains the exact 172-word/0x58-frame shape with
 * five instruction differences: objectPosition uses sp+0x28 instead of target
 * sp+0x30, and the D_A7C home/outgoing stores are reversed. The prior ten
 * declared-local/carrier forms remain exhausted. Stock IDO's itable listing
 * now identifies objectPosition's declared home at the target offset; CFE
 * instead copy-propagates the pointer expression, whose surviving value spills
 * at sp+0x28. A byte-array spelling and a source-line tie are flat. Ordered
 * three- and five-member aggregates regress, while a late D_0 carrier grows
 * the frame to 0x60. All 21 candidate relocations remain present.
 *
 * A 2026-09-10 pass (lane c4-o11) closed the slot family arithmetically for
 * the seven-local block: pooled temporaries are laid strictly below the
 * declared block, `planes` has to stay a declared volatile local or the
 * plane-table address is rematerialised, so the block bottom is pinned and no
 * census edit moves the pooled cell up to the target's slot. The first form
 * measured that defeats the copy propagation is a variable array index, and it
 * costs two instructions.
 *
 * 2026-09-10, lane o7-ovl: the two residuals are ONE residual, and the
 * five-local configuration is the frame in which to see it. Drop both
 * `objectPosition` and `planes` and inline their expressions. Normalise the
 * resulting object by adding 8 to every sp displacement at or above 40 (the
 * argument and save slots below that are already exact) and the first 55
 * instructions, +0x0 through +0xDC, are byte-identical to the target. The
 * first divergence is the target's `&D_A7C` home store, which the candidate
 * does not emit at all; after it the two streams carry the same instructions
 * one position apart, with the integer ring rotated by exactly one temp.
 *
 * So a single decision explains all of it: whether the plane-table address is
 * a spilled web or a rematerialised temp. Being a web supplies the missing
 * store, supplies the pool cell that lifts every frame slot by 8 at once
 * (block+pool 36+8 -> 36+12, frame 0x50 -> 0x58 after rounding), and supplies
 * the v0 colour that un-rotates the ring for the remaining words. That is the
 * whole open lever, and it is the same one the fin-near closure named.
 *
 * Newly falsified this pass, each byte-flat unless noted. Declaration order:
 * ALL 5,040 orders of the seven declarations were compiled; only the shipped
 * order and the one that swaps `contact` with `objectPosition` reach 5, the
 * next is 6, and the median is 30. The block is 44 bytes in every order --
 * three Vec3f and four 4-byte scalars leave no alignment slack -- so the block
 * bottom is 0x2C and the first pool cell 0x28 in all 5,040. That axis is
 * closed by exhaustion and by arithmetic. uopt region boundaries: `if (1) {}`
 * and `do {} while (0)`, empty or wrapping the definition, on either side of
 * `objectPosition` and of `planes` and both together -- eleven placements, all
 * flat. The region boundary that broke an address reassociation in overlay 38
 * does not reach cfe's copy propagation. A second, LIVE definition of
 * `objectPosition` with a different value (the else branch's `object + 0x1C`,
 * feeding the second func_80029A24) is flat, which retires the duplicate- and
 * dead-definition family as a class: the propagation is not single-assignment
 * driven. Also flat: self-assignment, `+ 0`, an `(s32)` round trip,
 * `&FIELD(object, Vec3f, 0xC)`, `register`, a const-initialised declaration,
 * and the definition hoisted to the first statement. On the store order,
 * folding `planes = D_A7C;` onto the `if` line, folding the whole condition
 * onto one line, and both together are flat, so the reversal is not an as1
 * line-number tie. A non-volatile `planes`, and no `planes` at all, are 62
 * words with the store dropped, and that is independent of where the
 * assignment is written -- four positions measured (shipped site, before
 * `distance`, before `keep`, before the first call), all 62. Position does not
 * open the web.
 *
 * 2026-09-10, lane w8-tu: the census is now read off the target, and the
 * lever the closure names is measured not to exist. Listing every sp
 * displacement the shipped function touches shows 0x28 and 0x54 are never
 * touched, so `contact` is register-allocated with a dead home, the declared
 * block's bottom is 0x34, and the block is 36 bytes -- five declared locals
 * exactly. That confirms the five-local reading from the target's own bytes.
 * Against it: a standalone probe at these flags shows uopt never merges an
 * address constant across a basic-block boundary -- three reads in three
 * blocks give three materialisations, two reads in one block give one for that
 * block -- so there is no web for it to spill, and only a volatile local
 * produces the single materialisation the target shows. The two facts are in
 * tension, and the next pass should attack the census side rather than look
 * for a spelling that spills. See the handoff shard for the rest.
 *
 * Preserve the assembly fallback. */
#ifdef NON_MATCHING
void func_overlay_022_F0000000_1878108(void *object, void *init) {
    void *contact;
    Vec3f position;
    Vec3f offset;
    f32 distance;
    s32 keep;
    Vec3f *objectPosition;
    void * volatile planes;

    contact = FIELD(object, void *, 0x64);
    FIELD(contact, f32, 4) = (f32)FIELD(init, s32, 0xC);
    FIELD(contact, s8, 0) = (s8)FIELD(init, s16, 0xA);
    FIELD(contact, void *, 0x38) = FIELD(init, void *, 0x14);
    FIELD(contact, s16, 2) = 1;
    position.x = FIELD(object, f32, 0xC);
    position.y = FIELD(object, f32, 0x10);
    position.z = FIELD(object, f32, 0x14);
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = FIELD(init, f32, 0x10);
    func_80029A24(FIELD(contact, void *, 0x38), &offset);
    objectPosition = (Vec3f *)((u8 *)object + 0xC);
    FIELD(object, f32, 0xC) += offset.x;
    FIELD(object, f32, 0x10) += offset.y;
    FIELD(object, f32, 0x14) += offset.z;
    keep = 1;
    distance = D_EE0;
    func_800150F0(1, &position, objectPosition, &distance, 0, 1);
    planes = D_A7C;
    if ((func_800104B0(&position, objectPosition, distance, object, D_A7C) != 0) &&
        (FIELD(contact, u8, 1) & 4)) {
        FIELD(object, s32, 0x80) |= 2;
        func_8003E99C(object, 1);
        func_80006A50(object);
        keep = 0;
    }
    if (keep != 0) {
        if (FIELD(contact, s8, 0) == 2) {
            if (!(FIELD(contact, u8, 1) & 2)) {
                FIELD(contact, u8, 1) = 0;
                position.x = FIELD(object, f32, 0xC);
                position.y = FIELD(object, f32, 0x10) - 5.0f;
                position.z = FIELD(object, f32, 0x14);
                if (func_800104B0(objectPosition, &position, distance, object,
                                  planes) == 0) {
                    FIELD(contact, s8, 0) = 1;
                    FIELD(object, s16, 4) = 0x4000;
                }
            }
        } else {
            FIELD(object, f32, 0x24) = -FIELD(contact, f32, 4);
            func_80029A24(FIELD(contact, void *, 0x38),
                          (Vec3f *)((u8 *)object + 0x1C));
            FIELD(object, s16, 4) = 0x4000;
        }
        FIELD(object, s16, 0x2E) = func_8000F690(
            FIELD(object, f32, 0xC), FIELD(object, f32, 0x10),
            FIELD(object, f32, 0x14));
        FIELD(FIELD(object, void *, 0x48), void *, 0x70) =
            FIELD(contact, void *, 0x38);
        FIELD(FIELD(object, void *, 0x48), u16, 6) |= 2;
        FIELD(contact, f32, 0x2C) = FIELD(object, f32, 0xC);
        FIELD(contact, f32, 0x30) = FIELD(object, f32, 0x10);
        FIELD(contact, f32, 0x34) = FIELD(object, f32, 0x14);
        FIELD(contact, s16, 2) = 0;
        if (D_30 < 12) {
            D_0[D_30++] = object;
            return;
        }
        func_overlay_022_F0000D30_1878E38(D_0[0], 5, &D_30);
        D_0[D_30++] = object;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o022/overlay22InitializeObject/func_overlay_022_F0000000_1878108.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_022_F0000000_1878108:start
 * symbol: func_overlay_022_F0000000_1878108
 * score: 167/172 words
 * frame: 0x58
 * relocations: 21
 * first-mismatch: +0xCC
 * summary: The two residuals are one: in a five-local source every word up to +0xDC is exact under a uniform 8-byte slot shift, and the sole open lever is making the plane-table address a spilled web rather than a rematerialised temp.
 * PLATEAU-HANDOFF:func_overlay_022_F0000000_1878108:end
 */
