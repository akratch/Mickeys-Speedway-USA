#include "PR/ultratypes.h"

typedef struct Overlay2Line {
    f32 x1;
    f32 y1;
    f32 x2;
    f32 y2;
    u16 value1;
    u16 value2;
} Overlay2Line;

typedef struct Overlay2Node {
    u16 type;
    u16 index;
    union {
        struct {
            u16 count;
            u16 pad6;
        } leaf;
        f32 boundary;
    } data;
    struct Overlay2Node *side1;
    struct Overlay2Node *side0;
} Overlay2Node;

extern s32 D_30;
extern f32 D_34;
extern Overlay2Line *D_3C;
extern f32 D_40;
extern f32 D_44;
extern f32 D_48;
extern f32 D_4C;
extern s32 D_50;
extern s32 D_54;
extern f32 D_58;
extern f32 D_5C;
extern s32 D_60;
extern f32 D_64;
extern f32 D_68;
extern f32 D_6C;
extern u16 D_70;
extern u16 D_72;

extern s32 overlay2IntersectSegments(f32 x0, f32 y0, f32 x1, f32 y1,
                                     f32 x2, f32 y2, f32 x3, f32 y3,
                                     f32 *outX, f32 *outY);
extern void overlay2IntersectBoundary(f32 x0, f32 y0, f32 x1, f32 y1,
                                      f32 *outX, f32 *outY);

/* Mickey-local reconstruction; pinned DKR/JFG object scans found no donor.
 *
 * 214/253 relocation-masked words, exact 0x3F4 size, exact 0x68 frame, 51 of
 * 51 relocations.  The 39 differing words are six sites, and every one of them
 * is a carrier colour rather than a structure: at each, the shipped code
 * spends `a0` (or one lower ring temp) where this candidate spends the next
 * one.  `a0` and `a1` are pure scratch in this function -- every call here
 * passes its first two floats in f12/f14, so no argument ever lands in them.
 *
 * The frame is closed.  T = frame = 0x68 and the declaration chain runs
 * count/remaining/leafResult at 0x64/0x60/0x5C, hitX 0x58, hitY 0x54, line
 * 0x50, recursiveResult 0x4C, so S = 0x1C and an eighth local moves the frame
 * to 0x70.  That is why every attempt to name the short-circuit boolean in a
 * fresh local collapses, and why the L88 "name it and drop another local"
 * composition has nowhere to go: the seven that are there are all live.
 *
 * Eliminated by earlier lanes: leaf-zero return (151), a scalar in place of
 * the one-element array (99), all four or-operand orders (byte-identical --
 * uopt normalises them), naming the short-circuit boolean in five carriers
 * (78-107), and a 48-cell composition cross flooring at 39.  Added here, all
 * flat at 39 or size-changing: the ternary and short-circuit-or spellings of
 * both short-circuit blocks (93-97 where they compile); 42 adjacent
 * source-line joins across the whole body; a 25-cell cross of five leaf-head
 * and five leaf-tail spellings (including `count = remaining--`, the comma
 * operator in the loop condition, and an explicit post-loop reset); 20
 * named-boolean carrier x branch-polarity forms, every one of which changes
 * the size; a 1,115-cell randomised cross of declaration order, local
 * qualifiers and types, five or-block spellings and four node-selection forms;
 * and the callee return-type lattice that closed overlay 5 -- making
 * `overlay2IntersectBoundary` non-void, `overlay2IntersectSegments` void, or
 * changing this function's own return type -- which is inert here.
 */
#ifdef NON_MATCHING
s32 overlay2QueryNode(f32 x0, f32 y0, f32 x1, f32 y1,
                      Overlay2Node *node) {
    register s32 count;
    s32 remaining;
    s32 leafResult;
    f32 hitX;
    f32 hitY;
    Overlay2Line *line;
    s32 recursiveResult[1];

    if (node->type == 1) {
        leafResult = 0;
        remaining = node->data.leaf.count;
        count = remaining;
        line = &D_3C[node->index];
        remaining--;
        if (count != 0) {
            do {
                if (overlay2IntersectSegments(D_40, D_44, D_48, D_4C,
                                              line->x1, line->y1,
                                              line->x2, line->y2,
                                              &hitX, &hitY) != 0) {
                    if (((((D_40 - hitX) * (D_40 - hitX)) +
                          ((D_44 - hitY) * (D_44 - hitY))) > 1.0f) &&
                        ((((D_40 - hitX) * (D_40 - hitX)) +
                          ((D_44 - hitY) * (D_44 - hitY))) < D_6C)) {
                        D_6C = ((D_40 - hitX) * (D_40 - hitX)) +
                               ((D_44 - hitY) * (D_44 - hitY));
                        D_64 = hitX;
                        D_68 = hitY;
                        D_70 = line->value1;
                        D_72 = line->value2;
                    }
                    if (D_60 != 0) {
                        leafResult = 1;
                    } else {
                        return 1;
                    }
                }
                count = remaining;
                line++;
            } while (remaining--);
        }
        if (D_60 != 0) {
            return leafResult;
        }
        return count;
    }

    D_50 = ((node->index == 0) ? y0 : x0) < node->data.boundary;
    D_54 = ((node->index == 0) ? y1 : x1) < node->data.boundary;

    if (D_50 == D_54) {
        if (D_50 != 0) {
            node = node->side1;
        } else {
            node = node->side0;
        }
        return overlay2QueryNode(x0, y0, x1, y1, node);
    }

    D_30 = node->index;
    D_34 = node->data.boundary;
    overlay2IntersectBoundary(x0, y0, x1, y1, &D_58, &D_5C);

    if (D_50 != 0) {
        if (D_60 != 0) {
            recursiveResult[0] =
                overlay2QueryNode(x0, y0, D_58, D_5C, node->side1);
            return overlay2QueryNode(D_58, D_5C, x1, y1, node->side0) |
                   recursiveResult[0];
        }
        if (overlay2QueryNode(x0, y0, D_58, D_5C, node->side1) != 0) {
            return 1;
        }
        return overlay2QueryNode(D_58, D_5C, x1, y1, node->side0) != 0;
    }

    if (D_60 != 0) {
        recursiveResult[0] =
            overlay2QueryNode(x0, y0, D_58, D_5C, node->side0);
        return overlay2QueryNode(D_58, D_5C, x1, y1, node->side1) |
               recursiveResult[0];
    }
    if (overlay2QueryNode(x0, y0, D_58, D_5C, node->side0) != 0) {
        return 1;
    }
    return overlay2QueryNode(D_58, D_5C, x1, y1, node->side1) != 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o002/overlay2QueryNode/func_overlay_002_F00016A0_1858498.s")
#endif

/* PLATEAU-HANDOFF:overlay2QueryNode:start
 * symbol: overlay2QueryNode
 * score: 214/253 words
 * frame: 0x68
 * relocations: 51
 * first-mismatch: +0x40
 * summary: Six carrier-colour sites; the frame admits exactly seven locals, so no boolean can be named. 1,200 new cells flat at 39.
 * PLATEAU-HANDOFF:overlay2QueryNode:end
 */
