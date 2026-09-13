#include "PR/ultratypes.h"

typedef struct Overlay14Entry {
    s16 count;
    s16 selected;
    u8 *text;
    s16 kind;
    u8 padA[6];
} Overlay14Entry;

extern s32 gOverlay14ValueC0;
extern s32 gOverlay14DataBase;
extern s32 gOverlay14Args34;
extern Overlay14Entry *gOverlay14Entries;
extern void overlay14BuildPanel(s32, void *, s32, s32, s32, s32, s32);
extern s32 overlay14Dispatch();
extern s32 overlay14ValidateEntry(s16);

/* Workbench naming residual: 188/188 words, 12 differing words, first +0xDC.
 * Alignment: 176 exact and 12 naming rows; exact frame and no surplus words.
 * The while loop and generated entry stride retain the target temporary schedule. */
#ifdef NON_MATCHING
void func_overlay_014_F0001540_1870E18(s32 context) {
    u8 saved;
    s32 index;
    s32 cellWidth;
    s32 remaining;
    s32 x;
    s32 y;
    s32 opacity;
    u8 *cursor;
    void *drawArg;
    s32 first;

    overlay14BuildPanel(context, &gOverlay14DataBase, 0x5C, 0x14, 0xD0, 0x58,
                        (gOverlay14ValueC0 * 0xA0) >> 8);
    index = 0;
    cellWidth = overlay14Dispatch(2);
    remaining = (0x58 / cellWidth) - 1;
    overlay14Dispatch(2);
    overlay14Dispatch(0, 0, 0, 0);
    y = ((0x58 - (remaining * cellWidth)) >> 1) + 0x14;
    opacity = (gOverlay14ValueC0 * 0xFF) >> 8;
    while ((index < gOverlay14Entries->count) && (remaining > 0)) {


        first = 1; x = 0x60;
        if (index == gOverlay14Entries->selected)
            overlay14Dispatch(0xFF, 0xFF, 0xFF, 0xFF, opacity);
        else
            overlay14Dispatch(0, 0xC0, 0xC0, 0xFF, opacity);
        cursor = gOverlay14Entries[index].text;
        if (overlay14ValidateEntry(
                gOverlay14Entries[index].kind) == 0) cursor = 0;
        if ((cursor != 0) && (remaining > 0)) {
            do {
                cursor = (u8 *)overlay14Dispatch(2, cursor, 0xC8, &drawArg, 0);
                if (cursor != 0) {
                    saved = *cursor; *cursor = 0;
                    overlay14Dispatch(context, x, y, drawArg, 0);
                    *cursor = saved; y += cellWidth;
                    if (first != 0) { x += 8; first = 0; }
                    remaining--;
                }
            } while ((cursor != 0) && (remaining > 0));
        }
        index++;


    }
    overlay14Dispatch(0, 0, 0, 0);
    overlay14Dispatch(0xFF, 0xC0, 0, 0xFF, opacity);
    if ((index < gOverlay14Entries->count) || (cursor != 0))
        overlay14Dispatch(context, 0xC4, 0x6C, &gOverlay14Args34, 0xC);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o014/func_overlay_014_F0001540_1870E18/func_overlay_014_F0001540_1870E18.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_014_F0001540_1870E18:start
 * symbol: func_overlay_014_F0001540_1870E18
 * score: 12/188 words
 * frame: 0x80
 * relocations: 26
 * first-mismatch: +0xDC
 * summary: exact geometry remains; caller-pool ring is unchanged by source-order probes and needs a new zero-width web mechanism
 * PLATEAU-HANDOFF:func_overlay_014_F0001540_1870E18:end
 */
