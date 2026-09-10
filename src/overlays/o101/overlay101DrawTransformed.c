#include "PR/ultratypes.h"

typedef struct Overlay101Gfx {
    u32 w0;
    u32 w1;
} Overlay101Gfx;

typedef struct Overlay101DrawNode {
    u8 pad00[8];
    u8 type;
    u8 pad09[5];
    s16 x;
    s16 y;
} Overlay101DrawNode;

typedef struct Overlay101TransformElement {
    u8 pad00[8];
    s16 x;
    s16 y;
    f32 scale;
    s16 rotation;
    u8 color;
    u8 pad13;
    f32 depth;
    u8 pad18[4];
    void *object;
} Overlay101TransformElement;

typedef struct Overlay101Transform {
    s16 rotateZ;
    s16 rotateY;
    s16 rotateX;
    s16 pad06;
    f32 scale;
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[0x10];
    f32 depth;
} Overlay101Transform;

void overlay101GetDimensions2Reloc(s32 *width, s32 *height);
void overlay101GetBounds2Reloc(Overlay101DrawNode *node, s32 *left, s32 *top,
                               s32 *right, s32 *bottom);
void overlay101SetScissor2Reloc(Overlay101Gfx **displayList, s32 left, s32 top,
                                s32 right, s32 bottom);
void overlay101SetTransformModeReloc(s32 mode);
void overlay101SubmitTransformReloc(Overlay101Gfx **displayList, void *matrix,
                                    void *vertices,
                                    Overlay101Transform *transform,
                                    void *object, s32 rotated, s32 color);

/*
 * Overlay 101 text +0x29A4..+0x2C3C. Natural source supplies the exact size,
 * 0x90 frame, ABI, CFG, FP topology, stack homes, and seven call sites. A
 * scoped decoded ledger selects two retail command schedules and complete
 * equivalent private temporary-register webs. Earlier lanes reached a 62-word
 * basin and then 43 by an exhaustive placement sweep that put the rotation
 * test BEFORE the third command build, recording honestly that this was a
 * measurement optimum and that the target emits the branch AFTER that build.
 *
 * Lane c6-band-b (2026-09-10) closed that contradiction and took the score to
 * 36. The move to the structurally-correct placement is worth nothing on its
 * own -- it measures 62 -- because it changes a register allocation as well as
 * an order. In the 43-word form the 0xFFFFFF00 constant is materialised in the
 * delay slot of the `overlay101GetDimensions2Reloc` call, so a0..a3 are all
 * forbidden to it and it lands on t0; the instrumented globalcolor profile
 * refuses `p1:w112=c4` outright, so a1 is not colour-reachable there at all.
 * Moving the branch shortens that live range, the constant takes a1 as the
 * target has it, and the a1 census goes 6 -> 9 exactly. What that costs is the
 * temp ring: with t0 no longer held by the constant, every expression temp in
 * the function shifts one place. Restoring the ring needs one more web parked
 * on t0, and one nested-assignment carrier at the fourth command supplies it
 * -- the same `command = (x = (*displayList)++)` idiom the second command
 * already carries, invisible in the object because the copy is peepholed away.
 * Both edits are regressions alone (62 and 66) and 36 together.
 *
 * The residue is a uniform ring rotation from +0x128 on, beginning at the
 * 0xFB000000 constant: the target holds one further web that this candidate
 * does not. It is not colour-reachable -- 36 of 36 single-web forces over the
 * eight allocator webs leave the object unchanged or worse -- so it is a web
 * count, not a colour. Flat at 36 or worse: all 16 subsets of carriers over
 * the four plain command sites, both orders of two distinct carriers crossed
 * with dropping either frame-filler local, five source-level ring nudges
 * (read-backs of screenWidth/screenHeight/transform fields/the unused locals),
 * naming the constant in a local (byte-identical), five region boundaries
 * (L97; the bare-brace control is byte-identical, as the law predicts), and
 * folding every command's word pair onto one line (45).
 */
#ifdef NON_MATCHING
void overlay101DrawTransformed(Overlay101Gfx **displayList, void *matrix,
                               void *vertices, Overlay101DrawNode *node,
                               Overlay101TransformElement *element) {
    s32 bounds0;
    s32 bounds1;
    s32 bounds2;
    s32 bounds3;
    s32 screenWidth;
    s32 screenHeight;
    s32 rotated;
    Overlay101Transform transform;
    Overlay101Gfx *new_var;
    Overlay101Gfx *new_var2;
    s32 right;
    s32 bottom;
    Overlay101Gfx *command;

    if (((element->color != 0) && (!(element->scale <= 0.0f))) &&
        ((node->type == 2) || (node->type == 4))) {
        overlay101GetDimensions2Reloc(&screenWidth, &screenHeight);
        transform.rotateZ = 0;
        transform.rotateY = 0;
        transform.rotateX = element->rotation;
        transform.scale = element->scale;
        transform.x = ((f32)(node->x + element->x)) -
                      ((f32)(((u32)screenWidth) >> 1));
        transform.y = ((f32)(((u32)screenHeight) >> 1)) -
                      ((f32)(node->y + element->y));
        transform.z = 0.0f;
        transform.depth = element->depth;
        command = (*displayList)++;
        command->w1 = 0;
        command->w0 = 0xE7000000;
        command = (new_var = (*displayList)++);
        command->w0 = 0xFA000000;
        command->w1 = element->color | 0xFFFFFF00;
        command = (*displayList)++;
        command->w1 = 0xFFFFFF00;
        command->w0 = 0xFB000000;
        if (element->rotation != 0)
            rotated = 1;
        else
            rotated = 0;
        overlay101GetBounds2Reloc(node, &bounds0, &bounds1, &bounds2, &bounds3);
        overlay101SetScissor2Reloc(displayList, bounds0, bounds1, bounds2,
                                   bounds3);
        if (element->scale == 1.0f)
            overlay101SetTransformModeReloc(0);
        overlay101SubmitTransformReloc(displayList, matrix, vertices,
                                       &transform, element->object, rotated,
                                       element->color);
        if (element->scale == 1.0f)
            overlay101SetTransformModeReloc(1);
        command = (new_var2 = (*displayList)++);
        command->w1 = 0;
        command->w0 = 0xE7000000;
        command = (*displayList)++;
        command->w1 = 0xFFFFFFFF;
        command->w0 = 0xFA000000;
        overlay101SetScissor2Reloc(displayList, 0, 0, 1000, 1000);
        (void)right;
        (void)bottom;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/overlay101DrawTransformed/func_overlay_101_F00029A4_18DE1C4.s")
#endif

/* PLATEAU-HANDOFF:overlay101DrawTransformed:start
 * symbol: overlay101DrawTransformed
 * score: 36 differing words
 * frame: 0x90
 * relocations: 7
 * first-mismatch: +0x128
 * summary: Exact 664 bytes, 166/166 words, frame 0x90 and CFG. Putting the rotation test where the target has it (after the third command build) frees the 0xFFFFFF00 constant onto a1, and one nested-assignment carrier at the fourth command restores the temp ring the move rotates; each is a regression alone and together they give 36. The residue is a uniform ring rotation from +0x128, proved to be a missing web rather than a colour.
 * PLATEAU-HANDOFF:overlay101DrawTransformed:end
 */
