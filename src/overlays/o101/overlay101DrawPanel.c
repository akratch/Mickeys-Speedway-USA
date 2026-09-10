#include "PR/ultratypes.h"

typedef struct Overlay101Gfx {
    u32 w0;
    u32 w1;
} Overlay101Gfx;

typedef struct Overlay101PanelRect {
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    u32 color;
} Overlay101PanelRect;

typedef struct Overlay101Panel {
    u8 pad00[8];
    u8 mode;
    s8 phase;
    s16 currentX;
    s16 currentY;
    s16 x;
    s16 y;
    s16 width;
    s16 height;
    u8 intensity;
    u8 alpha;
    void *content;
} Overlay101Panel;

typedef struct Overlay101PanelWork {
    Overlay101PanelRect records[20];
    u32 padF0;
    u32 darkest;
    volatile u32 dim;
    volatile u32 dimmer;
    u32 full;
    s32 height;
    s32 width;
} Overlay101PanelWork;

extern u8 D_1F4[];

void func_overlay_101_F0000000_18DB820();

#define overlay101SetScissor func_overlay_101_F0000000_18DB820
#define func_80039584 func_overlay_101_F0000000_18DB820
#define overlay101BuildIntensityColors func_overlay_101_F0000000_18DB820
#define overlay101BuilderCreateReloc func_overlay_101_F0000000_18DB820
#define overlay101BuildBorder func_overlay_101_F0000000_18DB820
#define overlay101SelectElementReloc func_overlay_101_F0000000_18DB820
#define overlay101SetRenderModeReloc func_overlay_101_F0000000_18DB820
#define overlay101SetColorReloc func_overlay_101_F0000000_18DB820
#define overlay101DrawElementReloc func_overlay_101_F0000000_18DB820
#define overlay101DrawDefaultAssetReloc func_overlay_101_F0000000_18DB820

/* Pinned DKR v77/v80 and JFG scans classify overlay 101 as no donor. */
/* Lane c3-o101 (2026-09-10): 79 -> 36 relocation-masked words, 53 -> 25 of
 * them register-blind, with size, frame and every store offset unchanged. The
 * lever is the uopt REGION boundary proved out on overlay101BuildBorder, where
 * it closed that function outright: only a control-flow construct opens a
 * region, a bare compound statement is byte-inert, and an interior boundary
 * renumbers the webs downstream of it. The `if (1) { ... }` span over the first
 * nine records is worth 11 masked and 26 register-blind words on its own; the
 * empty `if (1) { }` join inside record 1 is worth a further 20 masked; moving
 * `right = x + work.width` below record 0's colour store -- work.width is not
 * written again and `right` is not read before, so this is order, not meaning
 * -- is worth the last 12. Falsified, each measured on this baseline: 1,770
 * region spans per round over three rounds converge here, and dropping
 * `volatile` from work.dim costs 52 words and from work.dimmer 16, so unlike
 * BuildBorder's trailingColor these two pairs have no join to break. The
 * residue is the mode-1/3 tail at +0xB4 and the record-0 prologue.
 *
 * Lane c6-band-b (2026-09-10) exonerated the allocator and named the residue.
 * It is NOT a colouring decision: 240 single-web forces on the instrumented
 * globalcolor profile -- all 20 of this procedure's allocator webs crossed
 * with the twelve caller-saved colours -- leave the object at 36 words or
 * make it worse, and not one improves it. What the target has and this
 * candidate does not is one more web in the expression ring. The register
 * census says so directly: the shipped code spends t1 twelve times and this
 * candidate four, with t6, t7 and t8 each one or two higher here, and the
 * ring runs exactly one place behind from the first mismatch at +0xB4
 * onwards. The eight spill words are the same fact at the pool: both sides
 * use exactly three of the eleven pool cells at the same 0x178 frame, this
 * one at 80/84/92 and the target at 72/92/96.
 *
 * The obvious way to supply that web -- a seventh declared local -- is not
 * free here. Ten placements of an `out = (out2 = ...)` nested-assignment
 * carrier (the idiom that closed the same ring question on
 * overlay101DrawTransformed) all move the first mismatch to word 0 at
 * unchanged size: the extra declaration moves the frame. Also measured flat
 * at 36: all 720 declaration orders of the six locals, the type and qualifier
 * lattice on x/y/right/bottom, `register`/`volatile`/`register volatile` on
 * `out`, and seven L59 line folds and blank-line placements around the
 * BuildIntensityColors call and its two neighbouring assignments. Two carrier
 * locals for the fifth mode-1/3 argument regress from +0x80, and the
 * one-step, two-step and `32 +` spellings of such a carrier are
 * byte-identical to each other, so that lattice is one point, not three. */
#ifdef NON_MATCHING
void overlay101DrawPanel(Overlay101Gfx **displayList, Overlay101Panel *panel) {
    Overlay101PanelRect *out;
    s32 x;
    Overlay101PanelWork work;
    s32 y;
    s32 right;
    s32 bottom;

    overlay101SetScissor(displayList, 0, 0, 1000, 1000);

    if ((panel->mode == 1) || (panel->mode == 3)) {
        func_80039584(
            displayList,
            panel->currentX +
                (((panel->x - panel->currentX) * panel->phase) >> 6),
            panel->currentY +
                (((panel->y - panel->currentY) * panel->phase) >> 6),
            32 + (((panel->width - 32) * panel->phase) >> 6),
            32 + (((panel->height - 32) * panel->phase) >> 6),
            0xC0C0C0FF);
        return;
    }

    if (panel->mode == 2) {
        x = panel->x;
        y = panel->y;
        work.width = panel->width;
        work.height = panel->height;

        overlay101BuildIntensityColors(panel->intensity + 1, panel->alpha,
                                       &work.full, &work.dim, &work.dimmer,
                                       &work.darkest);

        bottom = y + work.height;
        out = work.records;

        if (1) {
            out->x0 = x + 1;
            out->y0 = y + 12;
            out->x1 = x + 3;
            out->y1 = bottom - 1;
            out->color = work.dimmer;
            right = x + work.width;
            out++;

            out->x0 = x + 1;
            out->y0 = bottom - 3;
            out->x1 = right - 1;
            if (1) {
            }
            out->y1 = bottom - 1;
            out->color = work.dimmer;
            out++;

            out->x0 = right - 3;
            out->y0 = y + 12;
            out->x1 = right - 1;
            out->y1 = bottom - 1;
            out->color = work.dimmer;
            out++;

            out->x0 = x;
            out->y0 = y + 12;
            out->x1 = x + 1;
            out->y1 = bottom;
            out->color = work.dim;
            out++;

            out->x0 = x + 1;
            out->y0 = bottom - 1;
            out->x1 = right;
            out->y1 = bottom;
            out->color = work.dim;
            out++;

            out->x0 = right - 4;
            out->y0 = y + 12;
            out->x1 = right - 3;
            out->y1 = bottom - 3;
            out->color = work.dim;
            out++;

            out->x0 = x + 3;
            out->y0 = y + 12;
            out->x1 = x + 4;
            out->y1 = bottom - 3;
            out->color = work.full;
            out++;

            out->x0 = x + 4;
            out->y0 = bottom - 4;
            out->x1 = right - 3;
            out->y1 = bottom - 3;
            out->color = work.full;
            out++;

            out->x0 = right - 1;
            out->y0 = y + 12;
            out->x1 = right;
            out->y1 = bottom;
        }
        out->color = work.full;
        out++;

        out->x0 = x + 4;
        out->y0 = y + 12;
        out->x1 = right - 4;
        out->y1 = bottom - 4;
        out->color = work.darkest;

        overlay101BuilderCreateReloc(displayList, 10, work.records, 0);

        overlay101BuildBorder(displayList, x, y, 12, 12,
                              panel->intensity, panel->alpha, 0);
        overlay101BuildBorder(displayList, x + 12, y, work.width - 36, 12,
                              panel->intensity, panel->alpha, 0);
        overlay101BuildBorder(displayList, right - 24, y, 12, 12,
                              panel->intensity, panel->alpha, 0);
        overlay101BuildBorder(displayList, right - 12, y, 12, 12,
                              panel->intensity, panel->alpha, 0);

        if (panel->content != NULL) {
            overlay101SetScissor(displayList, x + 14, y + 2, right - 26,
                                  y + 10);
            overlay101SelectElementReloc(2);
            overlay101SetRenderModeReloc(0, 0, 0, 0);
            overlay101SetColorReloc(0, 0, 0, 255, panel->alpha);
            overlay101DrawElementReloc(displayList, x + 14, y + 2,
                                       panel->content, 0);
            overlay101SetScissor(displayList, 0, 0, 1000, 1000);
        }
        return;
    }

    if (panel->mode == 0) {
        overlay101DrawDefaultAssetReloc(displayList, D_1F4,
                                        panel->currentX, panel->currentY,
                                        255, 255, 255, panel->alpha);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/overlay101DrawPanel/func_overlay_101_F0002EFC_18DE71C.s")
#endif

/* PLATEAU-HANDOFF:overlay101DrawPanel:start
 * symbol: overlay101DrawPanel
 * score: 36 differing words
 * frame: 0x178
 * relocations: 18
 * first-mismatch: +0xB4
 * summary: Exact 268 words and 0x178 frame; two uopt region boundaries and one order-only move took 79 masked words to 36. The residue is NOT colouring: 240 single-web globalcolor forces leave it at 36 or worse. It is one missing expression-ring web, visible as a t1 census of 4 against the target's 12 and a ring running one place behind from +0xB4; a seventh declared local would supply it but moves the frame.
 * PLATEAU-HANDOFF:overlay101DrawPanel:end
 */
