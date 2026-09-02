#include "PR/ultratypes.h"

extern s8 D_8007D640;
extern s32 D_800D6AB0;
extern s16 D_800D6AB4;
extern s16 D_800D6AB6;
extern s16 D_800D6AB8;
extern s16 D_800D6ABA;
extern s16 D_800D6ABC;
extern s16 D_800D6ABE;
extern s16 D_800D6AC0;
extern s16 D_800D6AC2;
extern s16 D_800D6AC4;
extern s16 D_800D6AC6;
extern s16 D_800D6AC8;
extern s32 D_800D6AD8[];
extern s32 D_800D6AE4;
extern s32 piRomGetFileSize(s32 fileId);
extern s32 func_8002B280(s32 size, s32 tag);

extern char *D_800D6AD0[];
extern u8 *D_800D6AE0;
extern void func_8004BB44(s32 windowId, s32 x1, s32 y1, s32 x2, s32 y2);
extern void func_8004BBE0(s32 windowId, s32 font);
extern void *func_8004BCC4(s32 windowId, s32 posX, s32 posY, char *text,
                           s32 number, s32 flags);
extern void fontWindowColour(s32 windowId, s32 red, s32 green, s32 blue,
                             s32 alpha);
extern void fontWindowFontBackground(s32 windowId, s32 red, s32 green,
                                     s32 blue, s32 alpha);
extern void fontWindowFontColour(s32 windowId, s32 red, s32 green, s32 blue,
                                 s32 alpha, s32 opacity);
extern void func_8004BFB0(s32 windowId);

/* verdict: register-ring-only; 12 differing words; first mismatch +0x34 */
/* shape: exact 47 instructions and 0x18 frame; permuter-ready */
/* canonical fallback retained until the temporary allocation closes */
#ifdef NON_MATCHING
void func_8004E8E0(void) {
    s32 temp_t9;
    s32 temp_v0;

    D_800D6AB4 = (piRomGetFileSize(5) >> 2) - 2;
    temp_v0 = func_8002B280(0x790, 0x8F);
    temp_t9 = temp_v0 + 0x3C0;
    D_800D6AD8[0] = temp_v0;
    D_800D6AD8[1] = temp_t9;
    D_800D6AB0 = temp_t9 + 0x3C0;
    D_800D6AE4 = 0;
    D_800D6AC4 = 0;
    D_800D6AB6 = 0;
    D_800D6AB8 = 0x20;
    D_800D6AC8 = 0;
    D_800D6ABC = 0x20;
    D_800D6ABE = 0xCA;
    D_800D6AC0 = 0x120;
    D_800D6AC2 = 0xDE;
    D_8007D640 = 1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/block_4F4E0/func_8004E8E0.s")
#endif

extern void mmFree(void *ptr);
extern void func_8004BFD8(s32 windowId);
extern void func_8004BF64(s32 windowId);

void func_8004E99C(void) {
    if (D_8007D640 != 0) {
        mmFree((void *)D_800D6AD8[0]);
        func_8004BFD8(6);
        func_8004BF64(6);
        D_8007D640 = 0;
        D_800D6AC4 = 0;
    }
}

extern s32 D_8007D648;

void func_8004E9EC(s32 arg0) {
    D_8007D648 = arg0;
}

/* PROVENANCE: adapted from Diddy Kong Racing's public decomp, src/game_text.c:
 * render_subtitles; Mickey's own globals, calls, and linked bytes remain authoritative. */
void func_8004E9F8(void) {
    s32 textX;
    s32 textY;
    s32 i;
    s32 textFlags;
    char **textData;

    func_8004BF64(6);
    func_8004BB44(6, D_800D6ABC, D_800D6ABE, D_800D6AC0,
                  (s32)D_800D6AC2);
    fontWindowColour(6, 0, 0x60, 0,
                     (s32)(D_800D6AB6 * 0x64) >> 8);
    fontWindowFontBackground(6, 0, 0, 0, 0);
    textY = (s32)((((D_800D6AC2 - D_800D6ABE) -
                    (D_800D6AC6 * 0xC)) - (D_800D6AC6 * 2)) + 2) >> 1;
    for (i = 0; i < D_800D6AC6; i++) {
        textData = &D_800D6AD0[0];
        func_8004BBE0(6, (s32)textData[i][5]);
        textFlags = textData[i][6];
        if (textFlags == 4) {
            textX = (D_800D6AC0 - D_800D6ABC) >> 1;
        } else {
            if (textFlags == 1) {
                textX = (D_800D6AC0 - D_800D6ABC) - 8;
            } else {
                textX = 8;
            }
        }
        fontWindowFontColour(6, textData[i][1], textData[i][2],
                             textData[i][3], 0xFF,
                             (textData[i][4] * D_800D6AB6) >> 8);
        func_8004BCC4(6, textX, textY, textData[i] + 8, 1, textFlags);
        fontWindowFontColour(6, 0, 0, 0, 0xFF,
                             (D_800D6AB6 * 0xFF) >> 8);
        func_8004BCC4(6, textX + 1, textY + 1, textData[i] + 8, 1,
                      textFlags);
        textY += 0xE;
    }
    func_8004BFB0(6);
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/block_4F4E0/func_8004EC60.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/block_4F4E0/func_8004EDA8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/block_4F4E0/func_8004EED0.s")
s32 func_8004F020(void) {
    return 0;
}

/* PLATEAU-HANDOFF:func_8004E8E0:start
 * symbol: func_8004E8E0
 * score: 35/47 words
 * frame: 0x18
 * relocations: 28
 * first-mismatch: +0x34
 * summary: Exact shape; residual ugen temp-ring allocation is permuter-ready; target registers rotate after the second temporary.
 * PLATEAU-HANDOFF:func_8004E8E0:end
 */
