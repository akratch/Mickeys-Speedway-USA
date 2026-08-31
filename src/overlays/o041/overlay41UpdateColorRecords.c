#include "PR/ultratypes.h"

typedef struct Overlay41ColorRecord {
    s32 handle;
    u8 targetRed;
    u8 red;
    u8 targetGreen;
    u8 green;
    u8 targetBlue;
    u8 blue;
    u8 targetAlpha;
    u8 alpha;
    s16 remaining;
    s16 duration;
} Overlay41ColorRecord;

extern Overlay41ColorRecord gOverlay41ColorRecords[12];
extern void overlay41SetColor(s32 handle, s32 red, s32 green, s32 blue);
extern void overlay41SetAlpha(s32 handle, s32 alpha);

/* Workbench: allocation-mismatch, exact 98/-48 shape, 15-word floor from +0x40.
 * Lever: constant/relocation audit after the direct-field spelling left the source schedule intact but not the color web.
 * Remains: 15 allocation, 5 constant, 2 schedule, and nine overlay-relocation residuals; assembly fallback stays canonical. */
#ifdef NON_MATCHING
/* Ownership trial (2026-08-28): removed the TU's private 0x20-byte .data island;
 * linked promotion is text-differs with 98 in-range words, first at +0x0.
 * Module growth is cleared; the remaining gap is codegen/register allocation. */
void func_overlay_041_F0000124_188745C(s32 amount) {
    Overlay41ColorRecord *record;
    s32 i;
    s32 handle;
    s32 initialRed;
    s32 initialGreen;
    s32 initialBlue;
    s32 initialAlpha;
    s32 red;
    s32 green;
    s32 blue;
    s32 alpha;
    s32 remaining;
    s32 factor;
    u8 *direct;

    record = gOverlay41ColorRecords;
    i = 11;
    do {
        handle = record->handle;
        if (handle != 0) {
            red = record->red;
            green = record->green;
            blue = record->blue;
            alpha = record->alpha;
            initialRed = red;
            initialGreen = green;
            initialBlue = blue;
            initialAlpha = alpha;
            remaining = record->remaining;
            if (amount >= remaining) {
                record->handle = 0;
            } else {
                record->remaining = remaining - amount;
                factor = (record->remaining << 16) / record->duration;
                red += ((record->targetRed - initialRed) * factor) >> 16;
                green += ((record->targetGreen - initialGreen) * factor) >> 16;
                blue += ((record->targetBlue - initialBlue) * factor) >> 16;
                alpha += ((record->targetAlpha - initialAlpha) * factor) >> 16;
            }
            if (handle < 0) {
                overlay41SetColor(handle, red & 0xFF, green & 0xFF, blue & 0xFF);
                overlay41SetAlpha(handle, alpha & 0xFF);
            } else {
                direct = (u8 *)((u32)handle | 0x80000000U);
                direct[0x38] = red;
                direct[0x39] = green;
                direct[0x3A] = blue;
                direct[0x3B] = alpha;
            }
        }
        record++;
    } while (i--);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o041/overlay41UpdateColorRecords/func_overlay_041_F0000124_188745C.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_041_F0000124_188745C:start
 * symbol: func_overlay_041_F0000124_188745C
 * score: 83/98 words
 * frame: 0x30
 * relocations: 4
 * first-mismatch: +0x30
 * summary: 119 flags and ten fresh natural forms are nonexact; the color/snapshot web remains allocator-blocked and four static identities are unresolved
 * PLATEAU-HANDOFF:func_overlay_041_F0000124_188745C:end
 */
