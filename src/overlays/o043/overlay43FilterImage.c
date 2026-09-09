#include "PR/ultratypes.h"

typedef struct Overlay43Image {
    u8 pad0[0x20];
    u8 *pixels;
} Overlay43Image;

/* PROVENANCE: no code adapted. The pinned DKR/JFG exact-donor scan is negative.
 * A function-specific structural scan found JFG assembly-only
 * func_overlay_4_000015A8_1EF7898 as a 0.574 masked-skeleton relative; it
 * supplies no donor C. */
/* Bounded reproof 2026-09-05: initializing sum from pixel[0] coalesces the
 * former first/sum webs and improves the configured candidate from 10/43 to
 * 27/43 words. The 0xAC/43-instruction extent remains exact and frameless,
 * with no relocations; first mismatch is +0x4. The linked trial reports 16
 * owned differing words and no collateral. A fidelity-clean UOPT receipt maps
 * one procedure with eight integer decisions, all in p2, while the complete
 * 18-slot UGEN temporary lane is exact. Four accepted color forces rotate the
 * row, column, sum, and post-decrement scratch webs to their target registers
 * and leave only the +0x7C/+0x80 schedule swap; the forced object is diagnostic
 * only. Unsigned counters, condition casts, and two explicit scratch-carrier
 * forms are byte-neutral; narrow counters add five instructions. No flags or
 * permutation were repeated. The owner remains overlay +0x1378..+0x1424 / ROM
 * 0x188B348..0x188B3F4; separate +0x1424..+0x1430 padding follows. Two local
 * JUMPs at +0x218/+0x24C are its only inbounds, with no export or outbound
 * relocation. Preserve the fallback; resume only with new source-authentic p2
 * web-renumbering evidence for the post-decrement scratch. */
#ifdef NON_MATCHING
void overlay43FilterImage(Overlay43Image *image) {
    u8 *pixel;
    u32 *word;
    s32 row;
    s32 column;
    u16 sum;

    pixel = image->pixels;
    row = 0x3D;
    do {
        column = 0x3D;
        do {
            sum = pixel[0];
            sum += pixel[1];
            sum += pixel[2];
            sum += pixel[0x40];
            sum += pixel[0x42];
            sum += pixel[0x80];
            sum += pixel[0x81];
            sum += pixel[0x82];
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

/* PLATEAU-HANDOFF:overlay43FilterImage:start
 * symbol: overlay43FilterImage
 * score: 27/43 words
 * frame: frameless
 * relocations: 0
 * first-mismatch: +0x4
 * summary: Coalescing pixel[0] into sum makes the temp lane exact and cuts 33 to 16 words; four accepted p2 color forces leave only a two-word schedule swap.
 * PLATEAU-HANDOFF:overlay43FilterImage:end
 */
