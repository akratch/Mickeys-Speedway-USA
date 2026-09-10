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
 * the frame to 0x60. Volatile and address-exposed copy-propagation barriers,
 * each composed with the source-line tie, add one instruction and trigger a
 * structural cascade. All 21 candidate relocations remain present. Preserve
 * the assembly fallback pending an instruction-neutral copy-propagation
 * barrier or authenticated original declaration shape.
 *
 * A 2026-09-10 pass (lane c4-o11) closes the slot family arithmetically and
 * names the barrier class for the first residual. The debug local table gives
 * a seven-name declared block of 44 bytes, contact at the top and planes at
 * its bottom, and the pooled temporary that carries the propagated pointer
 * sits immediately below it. Pooled temporaries are laid strictly below the
 * declared block and never take a declared local's home, and planes has to
 * stay a declared volatile local or the plane-table address is rematerialised,
 * so the block bottom is pinned and no census edit can move that temporary up
 * to the target's slot. The value at the target's slot must therefore be a
 * declared local that cfe does not copy-propagate.
 *
 * The first form measured that actually defeats that copy propagation is a
 * variable array index: indexing the object as an array of the position type
 * by a variable moves the value out of the pool and into the declared home at
 * the target's slot. It costs two instructions, an index reload and a
 * multiply, and it collapses straight back to the pooled slot as soon as the
 * index is constant-foldable by cfe, so it is evidence rather than a
 * candidate. Four further constant spellings of the same address, including
 * the indexed and pointer-increment forms, are flat, which extends the
 * spelling-inertness finding to the indexed family. The open lever for the
 * first residual is now an index expression cfe cannot fold and uopt can, at
 * zero net instructions; the second residual is unchanged. */
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
 * summary: Pooled temporaries sit strictly below the declared block whose bottom is pinned by the volatile plane carrier, so the target slot must be a declared local cfe does not copy-propagate; a variable array index is the first form that defeats it, at two instructions.
 * PLATEAU-HANDOFF:func_overlay_022_F0000000_1878108:end
 */
