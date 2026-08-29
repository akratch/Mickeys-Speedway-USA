#include "PR/ultratypes.h"

typedef struct Overlay43Image {
    u8 pad0[0x20];
    u8 *pixels;
} Overlay43Image;

/* PROVENANCE: no code adapted. The pinned DKR/JFG exact-donor scan is negative.
 * A function-specific structural scan found JFG assembly-only
 * func_overlay_4_000015A8_1EF7898 as a 0.574 masked-skeleton relative; it
 * supplies no donor C. */
/* Diagnostic evidence reviewed 2026-08-29: historical pre-cleanup configured
 * full-TU C was measured at the frameless 0xAC/43-word shape, no relocations,
 * and four pixel-web register differences at +0xC/+0x10/+0x20/+0x28. No
 * candidate object/report survives. The stale isolated ranking measured 37/43,
 * adding a +0x7C/+0x80 schedule swap because import changed physical line
 * placement; its object is also absent. The earlier zero-word claim compared
 * fallback assembly. Historical
 * flag, permutation, and source-probe outcomes lack attributable artifacts and
 * do not prove exhaustion. That 39/43 body also used two invented empty guards,
 * an inert comma operand, and allocation-sensitive physical-line packing; all
 * are removed, so policy-clean current V0 is uncompiled and its score, size,
 * frame, and emitted relocation table are unknown. It owns overlay
 * +0x1378..+0x1424 / ROM 0x188B348..0x188B3F4 with no target padding; separate
 * +0x1424..+0x1430 padding follows. Two local JUMPs at +0x218/+0x24C are its
 * only inbounds, with no export or outbound relocation. Compile configured and
 * isolated V0, retain 119 configurations including configured V0, take one
 * allocation trace, then try at most one natural sum/line form and one web-
 * birth form, combining only strict independent gains. Hard cap 123
 * deterministic builds plus one trace; permit build 124 only for a regression-
 * triggered historical control. No generic batch. Linked C proof is absent. */
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
            sum = ((((((pixel[0] + pixel[1]) + pixel[0x40]) +
                    pixel[0x42]) + pixel[0x80]) + pixel[0x81]) +
                    pixel[0x82]) + pixel[2];
            pixel[0x41] = sum >> 3;
            pixel++;
        } while (column--);
        pixel += 2;
    } while (row--);

    word = (u32 *)image->pixels;
    row = 0x3FF;
    do {
        *word = (*word & 0xF0F0F0F0) >> 4;
        word++;
    } while (row--);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o043/overlay43FilterImage/func_overlay_043_F0001378_188B348.s")
#endif
