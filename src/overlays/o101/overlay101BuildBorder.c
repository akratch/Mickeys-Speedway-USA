#include "PR/ultratypes.h"

typedef struct Overlay101Gfx {
    u32 w0;
    u32 w1;
} Overlay101Gfx;

typedef struct Overlay101BorderRect {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    u32 color;
} Overlay101BorderRect;

void overlay101BuildIntensityColorsReloc(s32 intensity, s32 alpha, u32 *full,
                                         u32 *dim, u32 *dimmer, u32 *darkest);
void overlay101BuilderCreateReloc(Overlay101Gfx **displayList, s32 count,
                                  Overlay101BorderRect *rects, s32 flags);

/* Plateau: configured C is size/frame exact at 79 words/-136 but differs in 38
 * words; first non-relocation mismatch +0x44. All three relocation offset/type
 * sites align; two local R_MIPS_26 identities remain unresolved. */
/* Lane c2-o101 (2026-09-10): of those 38 words only NINE survive masking the
 * register fields, and every rectangle store already lands at the target's
 * offset in the target's order, so the whole residue is register colouring
 * plus two ready-order slips (the target loads the width home before the
 * height home and interleaves the two edge sums between the volatile colour
 * reads). The colouring difference is concrete: the target puts x, x+1,
 * x+width and y+height in a0..a3 and reuses a2 for y+1 once x is dead, while
 * this candidate reserves a2 for the `&rects` argument web that `rect = rects`
 * opens and pushes the geometry into t1..t5.
 *
 * Falsified levers, each measured: all sixteen commutative operand orders of
 * the four geometry sums measure exactly 38, so L92 spelling is byte-inert
 * for this shape; hoisting x+width and y+height into named locals regresses
 * to 40; writing the first four rectangles as `rects[i]` frees a2 but x still
 * takes a0 and the fifth-rectangle pointer folds away for a -4 delta and 52
 * words, so the `rect = rects; ... rect += 4;` idiom is load-bearing for the
 * size and is confirmed house style by the matched sibling
 * overlay101BuildFrame. */
#ifdef NON_MATCHING
void overlay101BuildBorder(Overlay101Gfx **displayList, s32 x, s32 y,
                           s32 width, s32 height, s32 intensity, s32 alpha,
                           s32 swapColors) {
    volatile u32 trailingColor;
    volatile u32 leadingColor;
    volatile u32 interiorColor;
    Overlay101BorderRect *rect;
    Overlay101BorderRect rects[5];

    if (swapColors != 0) {
        overlay101BuildIntensityColorsReloc(
            intensity + 1, alpha, (u32 *)&leadingColor,
            (u32 *)&trailingColor, (u32 *)&interiorColor, NULL);
    } else {
        overlay101BuildIntensityColorsReloc(
            intensity + 1, alpha, (u32 *)&trailingColor,
            (u32 *)&leadingColor, (u32 *)&interiorColor, NULL);
    }

    rect = rects;
    rect[0].x0 = x;
    rect[1].x0 = x;
    rect[0].y0 = y;
    rect[0].x1 = x + 1;
    rect[0].y1 = y + height - 1;

    rect[1].y0 = y + height - 1;
    rect[1].x1 = x + width;
    rect[1].y1 = y + height;

    rect[2].x0 = x + width - 1;
    rect[2].y0 = y + 1;
    rect[2].x1 = x + width;
    rect[2].y1 = y + height - 1;

    rect[3].x0 = x + 1;
    rect[3].y0 = y;
    rect[3].x1 = x + width;
    rect[3].y1 = y + 1;

    rect += 4;
    rects[0].color = leadingColor;
    rects[1].color = leadingColor;
    rects[2].color = trailingColor;
    rects[3].color = trailingColor;
    rect->x0 = x + 1;
    rect->y0 = y + 1;
    rect->x1 = x + width - 1;
    rect->y1 = y + height - 1;
    rect->color = interiorColor;

    overlay101BuilderCreateReloc(displayList, 5, rects, 0);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/overlay101BuildBorder/func_overlay_101_F0002DC0_18DE5E0.s")
#endif

/* PLATEAU-HANDOFF:overlay101BuildBorder:start
 * symbol: overlay101BuildBorder
 * score: 38 differing words
 * frame: 0x88
 * relocations: 3
 * first-mismatch: +0x44
 * summary: Exact 79 words and 0x88 frame; 38 masked and 38 raw differences, only nine of them register-blind. Every store already matches; the residue is register colouring plus two ready-order slips.
 * PLATEAU-HANDOFF:overlay101BuildBorder:end
 */
