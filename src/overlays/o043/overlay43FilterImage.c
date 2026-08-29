#include "PR/ultratypes.h"

typedef struct Overlay43Image {
    u8 pad0[0x20];
    u8 *pixels;
} Overlay43Image;

/* Historical notes report no exact DKR v77/v80 or JFG donor, but no retained
 * scan report authenticates that route; re-run the oracle before compilation. */
/* Diagnostic evidence reviewed 2026-08-29: retained configured full-TU C has
 * the exact frameless 0xAC/43-word shape, no relocations, and four pixel-web
 * register differences at +0xC/+0x10/+0x20/+0x28. The isolated ranking has
 * those four plus a +0x7C/+0x80 schedule swap because import changed physical
 * line placement; its AST is equivalent but its source-line mapping is not
 * current. The earlier zero-word claim compared fallback assembly. Historical
 * flag, permutation, and source-probe outcomes lack attributable artifacts and
 * do not prove exhaustion. That 39/43 body also used two invented empty guards;
 * they are removed, so clean current V0 is uncompiled. Compile it, retain the
 * 119-flag lattice, take one allocation trace, then try at most one natural
 * physical-line form and one web-birth form plus an improving-only combination.
 * Linked C proof is absent. */
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
