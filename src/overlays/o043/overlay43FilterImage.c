#include "PR/ultratypes.h"

typedef struct Overlay43Image {
    u8 pad0[0x20];
    u8 *pixels;
} Overlay43Image;

/* DKR v77/v80 and JFG contain no exact donor for this image filter. */
/* Plateau evidence reviewed 2026-08-29: retained configured full-TU C has the
 * exact frameless 0xAC/43-word shape, no relocations, and four pixel-web
 * register differences at +0xC/+0x10/+0x20/+0x28. The isolated ranking has
 * those four plus a +0x7C/+0x80 schedule swap because import changed physical
 * line placement; its AST is equivalent but its source-line mapping is not
 * current. The earlier zero-word claim compared fallback assembly. Historical
 * flag, permutation, and source-probe outcomes lack attributable artifacts and
 * do not prove exhaustion. Reproduce configured V0, then use one allocation
 * trace to select at most one physical-line and one web-birth form, combining
 * only independent strict gains within five steps. Linked C proof is absent. */
#ifdef NON_MATCHING
void overlay43FilterImage(Overlay43Image *image) {
    u8 *pixel;
    u32 *word;
    register s32 row;
    register s32 column;
    register u16 sum;

    pixel = image->pixels;
    row = 0x3D;
    do {
        column = 0x3D;
        do {
            if (!pixel) {
            }
            if (!image->pixels) {
            }
            sum = (((((((0, pixel[1] + pixel[0])) + pixel[0x40]) +
                    pixel[0x42]) + pixel[0x80]) + pixel[0x81]) +
                    pixel[0x82]) + pixel[2];
            pixel[0x41] = sum >> 3;
            pixel++;
        } while (column--);
        pixel += 2;
    } while (row--); word = (u32 *)image->pixels; row = 0x3FF; do {
        *word = (*word & 0xF0F0F0F0) >> 4;
        word++;
    } while (row--);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o043/overlay43FilterImage/func_overlay_043_F0001378_188B348.s")
#endif
