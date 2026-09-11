#include "PR/ultratypes.h"

/* NON_MATCHING structural reconstruction from Mickey's runtime relocations.
 * Resident identities use the ROM-table exports and reserved section bases;
 * D_o058 names identify byte offsets in overlay 58, including its BSS.
 * The guarded body retains the target call graph and typed data accesses.
 */
/*
 * Plateau (2026-09-10): 733 of 3,614 relocation-masked words differ, down from
 * 894.  Size delta 0, frame 0x138.  Under a shape-tolerant alignment 3,034
 * rows are byte-exact (up from 2,874) and 631 of 848 saved-register operand
 * slots agree (up from 434); the s7-with-s8 transposition that three grinds
 * named is GONE -- 0 transposed slots against 187.  Report all three numbers
 * for anything measured here: at 894 words the positional count is dominated
 * by displacement and does not order configurations on its own.
 *
 * The residual is p1 colour, not shape.  Every one of this procedure's 428
 * allocator decisions is phase one, so declaration order, definition position
 * and statement order are the wrong axis (165 earlier forms were flat on the
 * colour for that reason).  The only axis is L100's ratio,
 * `save = totalsave / nocs`, and there are two source dials on it:
 *
 *   1. Discarded-expression probes at loop depth 2 (three of them in case 9's
 *      grid inner loop, below).  100 of totalsave each, no instructions.
 *   2. WHICH DECLARED SCALAR PLAYS A LOOP'S INDEX.  A loop whose index is a
 *      carrier of its own takes its occurrences out of `i`'s web, and the
 *      target's own shape says it did this: it emits no `move <saved>,zero`
 *      in either the case-2 decrement loop's preheader or case 8's, which is
 *      what a loop whose index dies at strength reduction looks like.  Case 2
 *      is carried by `savedPosition` and case 8 by `opponent`; both are dead
 *      in their case, so the rename carries no meaning.  Worth 894 -> 755 and
 *      755 -> 733 respectively, and 197 saved-register slots between them.
 *
 * Still open, largest first: 119 words in case 9's grid-loop exit test (the
 * target tests `slti $at,<saved>,10` and the natural `opponent < 0xA` puts 10
 * in a saved register instead -- a second p1 contest, this one between the
 * constant's web and a global address web, and NOT reachable by respelling
 * either loop's bound: seven erase-loop spellings crossed with three grid
 * spellings are all exactly inert); 72 words in case 8; 40 in case 12; 34 in
 * case 1; 34 in case 2's draw loop; 29 in case 3; 22 at the case 1/2 boundary.
 *
 * Falsified: spelling a frame address as a cached pointer local (`char
 * *textPtr = &text[0];`, and the same for `nodes` and `character`) does not
 * reproduce the target's cached form -- all three measure one word WORSE at
 * unchanged size and frame, and caching `&character[0]` for the +0x3704 site
 * is byte-identical to not doing it.
 *
 * The `if (i != 0);` statements below are discarded-expression probes
 * (ido-5.3 L37) -- zero instructions, one web occurrence each.  They were
 * found by a two-pass climb over 11,304 variants and are a local optimum;
 * a further 144-variant sweep (one probe at the top of each case body, for
 * twelve different locals) found every size-preserving one inert.
 */
#ifdef NON_MATCHING
#include "game/anim.h"
#include "game/menu.h"
#include "game/font.h"
#include "overlays/overlay_056.h"

struct MenuCommand { u32 w0; u32 w1; };
typedef struct RcpTextureInfo RcpTextureInfo;
typedef struct RcpTextureNode {
    RcpTextureInfo *texture;
    RcpTextureInfo *alternate;
    u32 packedOffset;
    s16 x;
    s16 y;
} RcpTextureNode;

typedef struct Overlay58RaceEntry {
    u8 character;
    u8 variant;
    u8 variantCopy;
    u8 gap;
    s32 value;
    s32 lapTimes[3];
    u8 pad14[8];
    u8 counters[6];
    u16 rank;
    u8 flags[4];
} Overlay58RaceEntry;

typedef struct Overlay58RaceState {
    u8 mode;
    u8 active;
    u8 player;
    u8 countdown;
    Overlay58RaceEntry entries[6];
} Overlay58RaceState;

typedef struct SavesPackedEntry {
    s32 value;
    u8 name[3];
    u8 character;
} SavesPackedEntry;

typedef struct SavesSlot {
    SavesPackedEntry records[4];
} SavesSlot;

typedef struct ColourCycle {
    s32 frame;
    s32 time;
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    struct ColourCycle *table;
} ColourCycle;

typedef union Overlay58AnimationState {
    s32 word;
    u8 bytes[4];
} Overlay58AnimationState;

typedef struct Overlay58LanguageText {
    char *text[182];
} Overlay58LanguageText;

extern Overlay58RaceState *func_80028F54(void);
extern void func_80036AB0(void *cycle, s32 updateRate);
extern void func_8004B0A4(s32 font);
extern void func_8004B0F8(MenuCommand **displayList, s32 x, s32 y, char *text, s32 flags);
extern void func_8002F618(MenuCommand **displayList, RcpTextureNode *node, s32 x, s32 y, u8 red, u8 green, u8 blue, u8 alpha);
extern void func_8005055C(s32 pathIndex);
extern s32 sprintf(char *buffer, const char *format, ...);
extern void amSndPlay(u16 sound, void **handle);
extern void mainChangeCameras(s32 mode);
extern void joyCreateMap(s8 *activePlayers);
extern void mainChangeLevel(s32 nextLevel, s32 nextCharacter, s32 nextAnimGroup, s32 nextMenu, s32 arg4, s32 arg5);
extern void mainSetAnimGroup(s32 group);
extern void func_8003A754(void);
extern char *func_8003A5A0(s32 value);
extern SavesSlot *func_800291C4(void);
extern s32 levelGetBlurEffect(s32 level);
extern s32 func_8003A6B0(u8 index);
extern void func_80029120(u32 flags);
extern void func_8002917C(void);
extern s32 func_8003A700(u8 index);

extern u8 D_8007BEF8;
extern u8 D_8007BEFC;
extern u8 D_8007BF0C;
extern s32 D_8007BF44;
extern s32 D_8007C1A0;
extern Overlay58LanguageText *D_8007C0B8;
extern MenuCommand *D_800D3140;
extern s32 D_800D31B8;
extern s16 D_800D31BC;
extern s16 D_800D31BE;
extern u8 D_800D31C4[4];
extern RcpTextureInfo *D_800D31C8[];
extern s16 D_8007C0C0[4][4];

extern s32 D_o058_5E50[6];
extern char *D_o058_5E68[4];
extern Overlay58AnimationState D_o058_5E84;
extern Overlay58RaceEntry *D_o058_5EC8[6];
extern Overlay58RaceEntry *D_o058_5EE0[6];
extern s32 D_o058_5EF8[6];
extern s32 D_o058_5F10[6];
extern ColourCycle D_o058_5F38;
extern s32 D_o058_5B28[6];
extern char *D_o058_5C5C[3];
extern s16 D_o058_5C68[6];
extern s16 D_o058_5C74[6];
extern s16 D_o058_5C80[6];
extern s16 D_o058_5C8C[6];
extern char *D_o058_5C98[6];
extern s16 D_o058_5CB0[12];


extern u16 D_8007BF1C;
extern s32 D_8007BF48;
extern s32 D_8007BF4C;
extern s32 D_8007BF50;
extern s32 D_8007BF54;
extern u8 D_8007BF74;
extern s32 D_8007C1B4;
extern s16 D_800D304E;
/* Placeholder name for a second, distinct halfword import, NOT a claim about
 * its address.  The ROM relocates all six of this function's reads of this
 * kind at load time -- their address fields are zero in the image -- so no
 * read's identity is evidenced by the bytes.  What the target's code does fix
 * is that the two reads inside one case are not the same object: IDO opens a
 * global address web for a symbol as soon as two of its reads land in the same
 * region, and then spends an extra instruction materialising that web at the
 * single-read sites as well.  Every grouping of the six that separates the two
 * same-region pairs compiles to the identical object, so the split is
 * evidenced and this particular grouping is not.  Tier D; see the handoff. */
extern s16 D_800D3050;
extern RcpTextureNode D_o058_5BA0[];
extern s32 D_o058_5CD8;
extern char D_o058_5D2C[];
extern char D_o058_5D30[];
extern char D_o058_5D34[];
extern char D_o058_5D38[];
extern char D_o058_5D3C[];
extern char D_o058_5D44[];
extern char D_o058_5D48[];
extern char D_o058_5D50[];
extern char D_o058_5D54[];
extern char D_o058_5D5C[];
extern char D_o058_5D60[];
extern char D_o058_5D64[];
extern char D_o058_5D68[];
extern char D_o058_5D6C[];
extern char D_o058_5D70[];
extern char D_o058_5D74[];
extern char D_o058_5D78[];
extern char D_o058_5D7C[];
extern char D_o058_5D84[];
extern char D_o058_5D88[];
extern char D_o058_5D90[];
extern char D_o058_5D94[];
extern char D_o058_5D98[];
extern char D_o058_5DA0[];
extern char D_o058_5DA4[];
extern char D_o058_5DAC[];
extern char D_o058_5DB0[];
extern char D_o058_5DB4[];
extern char D_o058_5DB8[];
extern char D_o058_5DC4[];
extern s32 D_o058_5E78;
extern s32 D_o058_5E7C;
extern s32 D_o058_5E80;
extern s32 D_o058_5E88;
extern s32 D_o058_5E8C;
extern s32 D_o058_5E90;
extern s32 D_o058_5E94;
extern s32 D_o058_5E98;
extern s32 D_o058_5E9C;
extern s32 D_o058_5EA0;
extern s32 D_o058_5EA4;
extern s32 D_o058_5EA8;
extern s32 D_o058_5EAC;
extern s32 D_o058_5EB0;
extern s32 D_o058_5EB4;
extern s32 D_o058_5EC0;
extern s32 D_o058_5EC4;
extern s32 D_o058_5F28;
extern s32 D_o058_5F2C;
extern s32 D_o058_5F30;

extern s8 D_o058_5F48[4];

void func_overlay_058_F000138C_18B0574(s32 arg0) {
    s32 i;
    s32 opponent;
    s32 textY;
    s32 columnX;
    s32 x;
    void **cursor;   /* one cell, exactly as the scalar it replaces */
    s32 portraitX;
    s32 columnStep;
    s32 portraitIndex;
    s32 countdownX;
    s32 seconds;
    s32 minutes;
    s32 centiseconds;
    s32 rowY;
    s32 highlighted;
    s32 rowHeight;
    s32 letter0;
    s32 letter1;
    s32 savedX;
    s32 savedPosition;
    s32 savedOffset;
    s32 rowBase;
    s32 columnCount;
    char character[2];
    char text[24];
    s32 erase;
    Overlay58RaceState *state;
    RcpTextureNode nodes[4];
    AnimPath *path;
    SavesSlot *saves;
    SavesSlot *slot;

    state = func_80028F54();
    func_80036AB0(&D_o058_5F38, arg0);
    func_8004B0A4(0);
    switch (D_o058_5E94) {
    case 1:
    case 2:
        if ((D_o058_5E50[0] == -1) && (D_8007BEF8 > 0)) {
            opponent = 0;
            do {
                D_o058_5E50[opponent] = D_o058_5B28[D_o058_5EF8[opponent]];
                opponent++;
            } while (opponent < D_8007BEF8);
        }
        break;
    }

    rowHeight = D_o058_5C74[D_8007BEF8 - 1];
    rowBase = D_o058_5C68[D_8007BEF8 - 1];
    switch (D_o058_5E94) {
    case 1:
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5EA4 + 0xA0, 0x1E, D_8007C0B8->text[0x27], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        /* `i = 0;` ahead of `x`, not after it: one word.  Both spellings put
         * the def in the loop's own block, so the strength-reduced cursor
         * bases stay folded either way (L98); only the schedule moves. */
        i = 0;
        x = D_o058_5E98 + D_o058_5EA8;
        rowY = rowBase;
        if ((s32) D_8007BEF8 > 0) {
            do {
                x = -x;
                nodes[0].texture = D_800D31C8[0x51 + D_o058_5EC8[i]->character];
                nodes[0].alternate = NULL;
                nodes[0].x = x + 0x4E;
                nodes[0].y = rowY - 4;
                nodes[0].packedOffset = 0;
                nodes[1].texture = 0;
                func_8002F618(&D_800D3140, &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8004B0F8(&D_800D3140, x + 0x28, rowY, D_o058_5C98[D_o058_5EF8[i]], 0);
                if ((D_8007C1A0 == 1) && (state->entries == D_o058_5EC8[i])) {
                    fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
                } else {
                    fontColour(0, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                func_8004B0F8(&D_800D3140, x + 0x71, rowY, D_8007C0B8->text[D_o058_5EC8[i]->character + 0x1A], 0);
                if (state->mode == 5) {
                    if (i == 0) {
                        overlay56SplitTime(D_o058_5EC8[i]->value, &minutes, &seconds, &centiseconds);
                    } else {
                        overlay56SplitTime(D_o058_5EC8[0]->value - D_o058_5EC8[i]->value, &minutes, &seconds, &centiseconds);
                    }
                } else if (i == 0) {
                    overlay56SplitTime(D_o058_5EC8[i]->value, &minutes, &seconds, &centiseconds);
                } else {
                    overlay56SplitTime(D_o058_5EC8[i]->value - D_o058_5EC8[0]->value, &minutes, &seconds, &centiseconds);
                }
                fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
                textY = rowY + D_o058_5EAC;
                if ((i == 0) || (minutes != 0)) {
                    sprintf(&text[0], D_o058_5D2C, minutes);
                    func_8004B0F8(&D_800D3140, x + 0xDC, textY, &text[0], 1);
                    func_8004B0F8(&D_800D3140, x + 0xDF, textY, D_o058_5D30, 0);
                }
                if (i != 0) {
                    if (state->mode == 5) {
                        func_8004B0F8(&D_800D3140, x + 0xBA, textY, D_o058_5D34, 0);
                    } else {
                        func_8004B0F8(&D_800D3140, x + 0xBA, textY, D_o058_5D38, 0);
                    }
                }
                sprintf(&text[0], D_o058_5D3C, seconds);
                func_8004B0F8(&D_800D3140, x + 0xE6, textY, &text[0], 0);
                func_8004B0F8(&D_800D3140, x + 0xFD, textY, D_o058_5D44, 0);
                sprintf(&text[0], D_o058_5D48, centiseconds);
                func_8004B0F8(&D_800D3140, x + 0x104, textY, &text[0], 0);
                i += 1;
                rowY += rowHeight;
            } while (i < (s32) D_8007BEF8);
        }
        D_o058_5E98 -= arg0 * 0xF;
        if (D_o058_5E98 < 0) {
            D_o058_5E98 = 0;
            if ((D_o058_5EB0 == 0) && (D_800D31B8 & 0x9000)) {
                if ((D_o058_5EC4 >= 4) && (D_8007BF0C == 0)) {
                    if (state->countdown == 0) {
                        D_o058_5EB0 = 5;
                    } else {
                        D_o058_5EB0 = 6;
                    }
                } else {
                    *D_o058_5E50 = -1;
                    D_o058_5EB0 = 2;
                }
                amSndPlay(0xCU, NULL);
            }
            if (D_o058_5EB0 != 0) {
                D_o058_5EA4 += arg0 * 0xF;
                if ((D_o058_5EB0 == 5) || (D_o058_5EB0 == 6)) {
                    D_o058_5EA8 += arg0 * 0xF;
                    if (D_o058_5EA8 >= 0x141) {
                        D_o058_5E94 = D_o058_5EB0;
                        amSndPlay(0x1FAU, NULL);
                        return;
                    }
                } else {
                    D_o058_5EAC += arg0 * 0xF;

                    if ((D_o058_5EAC >= 0xB5) && (D_o058_5EA4 >= 0x141)) {
                        D_o058_5E9C = 0x6E;
                        D_o058_5E94 = D_o058_5EB0;
                        amSndPlay(0x1FAU, NULL);
                        D_o058_5EB0 = 0;
                        D_o058_5EB4 = 0x4B;
                        D_o058_5EA0 = 0;
                        return;
                    }
                }
            }
        }
        break;
    case 2:
        i = 0;
        if (*D_o058_5E50 > 0) {
            D_o058_5EB4 = D_o058_5EB4 - arg0;
            if (D_o058_5EB4 < 0) {
                D_o058_5EB4 = 0xF;
                amSndPlay(0x1BU, NULL);
                if ((s32) D_8007BEF8 > 0) {
                    /* This loop's index is a carrier of its own, not `i`.
                     * The target emits no `move <saved>,zero` in this
                     * preheader at all, which is what a loop whose index dies
                     * at strength reduction looks like; `i` does not die there
                     * because the draw loop below reads it.  Any scalar dead
                     * in case 2 can carry it (L?: a declared scalar's identity
                     * is free), but only `savedPosition` keeps the size delta
                     * at 0 -- every other carrier measured reads -4.  Worth
                     * 894 -> 755 masked words on its own. */
                    savedPosition = 0;
                    do {
                        if (D_o058_5E50[savedPosition] > 0) {
                            D_o058_5E50[savedPosition] -= 1;
                        }
                        savedPosition++;
                    } while (savedPosition < D_8007BEF8);
                    i = 0;
                }
            }
        }
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5EA0 + D_o058_5EA4 + 0xA0, 0x1E, D_8007C0B8->text[0x28], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        savedX = D_o058_5E98;
        if (i != 0);
        savedOffset = D_o058_5EA0;
        i = 0;
        rowY = rowBase;
        if ((s32) D_8007BEF8 > 0) {
            do {
                D_o058_5E98 = -D_o058_5E98;
                D_o058_5EA0 = -D_o058_5EA0;

                nodes[0].texture = D_800D31C8[0x51 + D_o058_5EC8[i]->character];
                nodes[0].alternate = NULL;
                nodes[0].x = D_o058_5E98 + D_o058_5EA0 + 0x4E;
                nodes[0].y = rowY - 4;
                nodes[0].packedOffset = 0;
                nodes[1].texture = 0;
                func_8002F618(&D_800D3140, (RcpTextureNode *) &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5EA0 + 0x28, rowY, D_o058_5C98[D_o058_5EF8[i]], 0);
                if ((D_8007C1A0 == 1) && (state->entries == D_o058_5EC8[i])) {
                    fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
                } else {
                    fontColour(0, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5EA0 + 0x71, rowY, D_8007C0B8->text[D_o058_5EC8[i]->character + 0x1A], 0);
                textY = D_o058_5EC8[i]->rank - D_o058_5E50[i];
                fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
                sprintf(&text[0], D_o058_5D50, textY);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + D_o058_5EA0 + 0xBE, rowY, &text[0], 0);
                sprintf(&text[0], D_o058_5D54, D_o058_5E50[i]);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + D_o058_5EA0 + 0xEB, rowY, &text[0], 0);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + D_o058_5EA0 + 0x113, rowY, D_o058_5D5C, 0);
                i += 1;
                rowY += rowHeight;
            } while (i < (s32) D_8007BEF8);
        }
        D_o058_5E98 = savedX;
        D_o058_5EA0 = savedOffset;
        D_o058_5E9C -= arg0 * 0xF;
        D_o058_5EA0 -= arg0 * 0xF;
        D_o058_5EA4 -= arg0 * 0xF;

        if ((D_o058_5E9C < 0) && (D_o058_5EA0 < 0) && (D_o058_5EA4 < 0)) {
            if (D_o058_5EB0 == 0) {
                if (D_800D31B8 & 0x9000) {
                    D_o058_5EB0 = 3;
                    amSndPlay(0xCU, NULL);
                } else if ((D_800D31B8 & 0x4000) && (state->mode != 5)) {
                    D_o058_5EB0 = 1;
                    amSndPlay(0xDU, NULL);
                }
            } else {
                D_o058_5E98 += arg0 * 0xF;
                if (D_o058_5E98 >= 0x141) {
                    D_o058_5EAC = 0;
                    D_o058_5E94 = D_o058_5EB0;
                    amSndPlay(0x1FAU, NULL);
                    D_o058_5EB0 = 0;
                    D_o058_5EA4 = 0;
                }
            }
        }
        if (D_o058_5E9C < 0) {
            D_o058_5E9C = 0;
        }
        if (D_o058_5EA0 < 0) {
            D_o058_5EA0 = 0;
        }
        if (D_o058_5EA4 < 0) {
            D_o058_5EA4 = 0;
            return;
        }
        break;
    case 3:
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0xA0, 0x1E, D_8007C0B8->text[0x29], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        savedX = D_o058_5E98;
        if (i != 0);
        savedPosition = D_o058_5E9C;
        i = 0;
        rowY = rowBase;
        if ((s32) D_8007BEF8 > 0) {
            do {
                D_o058_5E98 = -D_o058_5E98;
                D_o058_5E9C = -D_o058_5E9C;

                nodes[0].texture = D_800D31C8[0x51 + D_o058_5EE0[i]->character];
                nodes[0].alternate = NULL;
                nodes[0].x = D_o058_5E98 + D_o058_5E9C + 0x4E;
                nodes[0].y = rowY - 4;
                nodes[0].packedOffset = 0;
                nodes[1].texture = 0;
                func_8002F618(&D_800D3140, (RcpTextureNode *) &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0x28, rowY, D_o058_5C98[D_o058_5F10[i]], 0);
                if ((D_8007C1A0 == 1) && (state->entries == D_o058_5EE0[i])) {
                    fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
                } else {
                    fontColour(0, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0x71, rowY, D_8007C0B8->text[D_o058_5EE0[i]->character + 0x1A], 0);
                sprintf(&text[0], D_o058_5D60, D_o058_5EE0[i]->rank);
                fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0xD2, rowY, &text[0], 0);
                if (D_o058_5EE0[i]->rank == 1) {
                    func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0x104, rowY, D_o058_5D64, 0);
                } else {
                    func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0x104, rowY, D_o058_5D68, 0);
                }
                i += 1;
                rowY += rowHeight;
            } while (i < (s32) D_8007BEF8);
        }
        D_o058_5E98 = savedX;
        D_o058_5E98 -= arg0 * 0xF;

        D_o058_5E9C = savedPosition;
        if (D_o058_5E98 < 0) {
            D_o058_5E98 = 0;
            if (D_o058_5EB0 == 0) {
                if (D_800D31B8 & 0x9000) {
                    if (state->mode == 0) {
                        D_o058_5EB0 = 4;
                    } else {
                        D_o058_5EB0 = 0xC;
                    }
                    amSndPlay(0xCU, NULL);
                    return;
                }
                if (D_800D31B8 & 0x4000) {
                    D_o058_5EB0 = 2;
                    *D_o058_5E50 = -1;
                    amSndPlay(0xDU, NULL);
                    return;
                }
            } else {
                D_o058_5E9C += arg0 * 0xF;
                if (D_o058_5E9C >= 0x141) {
                    if (D_o058_5EB0 == 4) {
                        animseqStopPath(D_o058_5E84.bytes[3]);
                        D_o058_5E84.word = 1;
                        path = func_800508B4(D_o058_5E84.bytes[3]);
                        if (path != NULL) {
                            func_8005055C(D_o058_5E84.bytes[3]);
                            animseqStartPath(D_o058_5E84.bytes[3]);
                            path->flags |= 2;
                        }
                        D_o058_5E80 = 2;
                        D_o058_5E88 = 0x50;
                        return;
                    }
                    D_o058_5E94 = D_o058_5EB0;
                    amSndPlay(0x1FAU, NULL);
                    D_o058_5EB0 = 0;
                    D_o058_5E9C = 0;
                    D_o058_5EA0 = 0x140;
                    return;
                }
            }
        }
        break;
    case 12:
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5E9C + D_o058_5EA0 + 0xA0, 0x1E, D_8007C0B8->text[0x6E], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);


        columnStep = D_o058_5C8C[D_8007BEF8 - 1];
        columnX = D_o058_5C80[D_8007BEF8 - 1] + D_o058_5E9C + D_o058_5EA0;
        i = 0;
        if ((s32) D_8007BEF8 > 0) {
            do {
                func_8004B0F8(&D_800D3140, columnX, 0x37, D_o058_5C98[i], 4);
                i += 1;
                columnX += columnStep;
            } while (i < (s32) D_8007BEF8);
        }
        savedPosition = D_o058_5E9C;
        savedOffset = D_o058_5EA0;
        i = 0;
        rowY = rowBase;
        if ((s32) D_8007BEF8 > 0) {
            do {
                D_o058_5E9C = -D_o058_5E9C;
                D_o058_5EA0 = -D_o058_5EA0;

                nodes[0].texture = D_800D31C8[0x51 + D_o058_5EE0[i]->character];
                nodes[0].alternate = NULL;
                nodes[0].x = D_o058_5E9C + D_o058_5EA0 + 0x28;
                nodes[0].y = rowY + 0x12;
                nodes[0].packedOffset = 0;
                nodes[1].texture = 0;
                opponent = 0;
                func_8002F618(&D_800D3140, (RcpTextureNode *) &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
                fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);

                columnX = D_o058_5C80[D_8007BEF8 - 1] + D_o058_5E9C + D_o058_5EA0;
                if ((s32) D_8007BEF8 > 0) {
                    do {
                        sprintf(&text[0], D_o058_5D6C, D_o058_5EE0[i]->counters[opponent]);
                        func_8004B0F8(&D_800D3140, columnX, rowY + 0x16, &text[0], 4);
                        opponent += 1;
                        columnX += columnStep;
                    } while (opponent < (s32) D_8007BEF8);
                }
                i += 1;
                rowY += rowHeight;
            } while (i < (s32) D_8007BEF8);
        }
        D_o058_5EA0 = savedOffset;
        D_o058_5EA0 -= arg0 * 0xF;

        D_o058_5E9C = savedPosition;
        if (D_o058_5EA0 < 0) {
            D_o058_5EA0 = 0;
            if (D_o058_5EB0 == 0) {
                if (D_800D31B8 & 0x9000) {
                    if (state->mode == 3) {
                        D_o058_5EB0 = 4;
                    } else {
                        D_o058_5EB0 = 7;
                    }
                    amSndPlay(0xCU, NULL);
                    return;
                }
                if (D_800D31B8 & 0x4000) {
                    D_o058_5EB0 = 3;
                    *D_o058_5E50 = -1;
                    amSndPlay(0xDU, NULL);
                    return;
                }
            } else {
                D_o058_5E9C += arg0 * 0xF;
                if (D_o058_5E9C >= 0x141) {
                    if (D_o058_5EB0 == 4) {
                        animseqStopPath(D_o058_5E84.bytes[3]);
                        D_o058_5E84.word = 1;
                        path = func_800508B4(D_o058_5E84.bytes[3]);
                        if (path != NULL) {
                            func_8005055C(D_o058_5E84.bytes[3]);
                            animseqStartPath(D_o058_5E84.bytes[3]);
                            path->flags |= 2;
                        }
                        D_o058_5E80 = 2;
                        D_o058_5E88 = 0x50;
                        return;
                    }
                    D_o058_5E94 = D_o058_5EB0;
                    amSndPlay(0x1FAU, NULL);
                    D_o058_5EB0 = 0;
                    D_o058_5E98 = 0x140;
                    D_o058_5E9C = 0;
                    D_o058_5EA0 = 0x140;
                    return;
                }
            }
        }
        break;
    case 13:
        i = 0;
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5E9C + D_o058_5EA0 + 0xA0, 0x1E, D_8007C0B8->text[0x6F], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

        if (D_8007BEF8 == 4) {
            columnCount = 4;
        } else {
            columnCount = D_8007BEF8 + 1;
        }

        columnStep = D_o058_5C8C[columnCount - 1];
        portraitX = D_o058_5C80[D_8007BEF8 - 1] + D_o058_5E9C + D_o058_5EA0;
        if (columnCount > 0) {
            /* Cursor, not `D_800D31C8[0x51 + i]`: an explicit cursor is what
             * lets the index def move to the top of the case.  IDO folds a
             * known-zero index into a strength-reduced cursor base only from
             * the loop's own block, so an indexed loop pins its `i = 0` next
             * to the loop, while the target's is in the first call's delay
             * slot.  With the subscript spelled as a cursor there is nothing
             * left to fold and the def is free to sit where the target has
             * it.  See the handoff. */
            /* Base the cursor at D_800D31C8 and carry the 0x51 in the
             * subscript, not `&D_800D31C8[0x51]`: the target's cursor register
             * holds the bare symbol (`%lo` zero) and spends the 0x144 in the
             * load's displacement.  Same instruction count, one more exact
             * row and a shorter shape block. */
            cursor = (void **) D_800D31C8;
            do {
                nodes[0].alternate = NULL;
                nodes[0].x = portraitX;
                nodes[0].y = 0x37;
                nodes[0].packedOffset = 0;
                nodes[1].texture = 0;
                nodes[0].texture = (RcpTextureInfo *) cursor[0x51];
                func_8002F618(&D_800D3140, (RcpTextureNode *) &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
                i += 1;
                cursor++;
                portraitX += columnStep;
            /* `!=`, not `<`: with the cursor carrying the subscript, uopt
             * no longer normalises `i < columnCount` into the target's
             * `bne`, and the `<` spelling costs an extra slt.  The converse
             * of what the indexed form wanted; see the handoff. */
            } while (i != columnCount);
        }
        savedPosition = D_o058_5E9C;
        savedOffset = D_o058_5EA0;
        i = 0;
        rowY = rowBase;
        if ((s32) D_8007BEF8 > 0) {
            do {
                D_o058_5E9C = -D_o058_5E9C;
                D_o058_5EA0 = -D_o058_5EA0;

                nodes[0].texture = D_800D31C8[0x51 + D_o058_5EE0[i]->character];
                nodes[0].alternate = NULL;
                nodes[0].x = D_o058_5E9C + D_o058_5EA0 + 0x28;
                nodes[0].y = rowY + 0x12;
                nodes[0].packedOffset = 0;
                nodes[1].texture = 0;
                opponent = 0;
                func_8002F618(&D_800D3140, (RcpTextureNode *) &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
                fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);

                columnX = D_o058_5C80[D_8007BEF8 - 1] + D_o058_5E9C + D_o058_5EA0;
                if (columnCount > 0) {
                    do {
                        sprintf(&text[0], D_o058_5D70, D_o058_5EE0[i]->flags[opponent]);
                        func_8004B0F8(&D_800D3140, columnX + 8, rowY + 0x16, &text[0], 4);
                        opponent += 1;
                        columnX += columnStep;
                    } while (opponent != columnCount);
                }
                i += 1;
                rowY += rowHeight;
            } while (i < (s32) D_8007BEF8);
        }
        D_o058_5EA0 = savedOffset;
        D_o058_5EA0 -= arg0 * 0xF;

        D_o058_5E9C = savedPosition;
        if (D_o058_5EA0 < 0) {
            D_o058_5EA0 = 0;
            if (D_o058_5EB0 == 0) {
                if (D_800D31B8 & 0x9000) {
                    if (state->mode == 3) {
                        D_o058_5EB0 = 4;
                    } else {
                        D_o058_5EB0 = 7;
                    }
                    amSndPlay(0xCU, NULL);
                    return;
                }
                if (D_800D31B8 & 0x4000) {
                    D_o058_5EB0 = 0xC;
                    *D_o058_5E50 = -1;
                    amSndPlay(0xDU, NULL);
                    return;
                }
            } else {
                D_o058_5E9C += arg0 * 0xF;
                if (D_o058_5E9C >= 0x141) {
                    if (D_o058_5EB0 == 4) {
                        animseqStopPath(D_o058_5E84.bytes[3]);
                        D_o058_5E84.word = 1;
                        path = func_800508B4(D_o058_5E84.bytes[3]);
                        if (path != NULL) {
                            func_8005055C(D_o058_5E84.bytes[3]);
                            animseqStartPath(D_o058_5E84.bytes[3]);
                            path->flags |= 2;
                        }
                        D_o058_5E80 = 2;
                        D_o058_5E88 = 0x50;
                        return;
                    }
                    D_o058_5E94 = D_o058_5EB0;
                    amSndPlay(0x1FAU, NULL);
                    D_o058_5EB0 = 0;
                    D_o058_5E9C = 0;
                    D_o058_5EA0 = 0x140;
                    return;
                }
            }
        }
        break;
    case 7:
    case 11:
        i = 0;
        if (D_o058_5EB0 == 0) {
            if ((D_800D31BE >= 0x11) && (D_o058_5F28 > 0)) {
                D_o058_5F28 -= 1;
                amSndPlay(0xFU, NULL);
            } else if ((D_800D31BE < -0x10) && (D_o058_5F28 < 3)) {
                D_o058_5F28 += 1;
                amSndPlay(0xFU, NULL);
            }
        }
        x = D_o058_5E9C + D_o058_5EA0;
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        if (D_o058_5E94 == 7) {
            if (D_8007C1A0 == 1) {
                func_8004B0F8(&D_800D3140, x + 0xA0, 0x1E, D_8007C0B8->text[0xB5], 4);
            } else {
                func_8004B0F8(&D_800D3140, x + 0xA0, 0x1E, D_8007C0B8->text[0x2A], 4);
            }
        } else {
            func_8004B0F8(&D_800D3140, x + 0xA0, 0x1E, D_8007C0B8->text[0x2B], 4);
        }
        /* Cursor, not `D_o058_5E68[i]`; and the def of `i` therefore stays
         * at the top of the case, where the target puts it.  See case 13. */
        cursor = (void **) D_o058_5E68;
        textY = 0x50;
        do {
            if (i == D_o058_5F28) {
                fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
            } else {
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
            }
            func_8004B0F8(&D_800D3140, 0xA0 - x, textY, (char *) *cursor, 4);
            i += 1;
            cursor++;
            textY += 0x1E;
            x = -x;
        } while (i < 4);
        D_o058_5EA0 -= arg0 * 0xF;

        if (D_o058_5EA0 < 0) {
            if (D_o058_5EB0 == 0) {
                if (D_800D31B8 & 0x9000) {
                    D_o058_5EB0 = 4;
                    amSndPlay(0xCU, NULL);

                } else if (D_800D31B8 & 0x4000) {
                    if (D_o058_5E94 == 7) {
                        D_o058_5EB0 = 0xC;
                    } else {
                        D_o058_5EB0 = 0xA;
                    }
                    amSndPlay(0xDU, NULL);

                }
            } else {
                D_o058_5E9C += arg0 * 0xF;
                if (D_o058_5E9C >= 0x141) {
                    if (D_o058_5EB0 == 4) {
                        switch (D_o058_5F28) {
                        case 0:
                            mainChangeCameras(D_8007C1A0);
                            joyCreateMap(D_o058_5F48);
                            if (((D_8007C1A0 == 2) || (D_8007C1A0 == 3)) && (D_8007BF74 != 0)) {
                                D_8007BEFC = 4 - D_8007C1A0;
                                D_8007BEF8 = 4;
                            } else if ((D_8007C1A0 == 1) && (state->mode == 5)) {
                                D_8007BEFC = 3;
                                D_8007BEF8 = 4;
                            } else {
                                D_8007BEFC = 0;
                                D_8007BEF8 = (u8) D_8007C1A0;
                            }
                            for (i = 0; i < 6; i++) {
                                state->entries[i].value = 0;
                            }
                            if (D_o058_5F2C != 0) {
                                mainChangeLevel((s32) D_800D304E, 0, 0, 5, 1, 0);
                                D_o058_5F2C = 0;
                            }

                            break;
                        case 1:
                            if (D_o058_5F2C != 0) {
                                mainChangeLevel(0x1D, 0, 0, 0xB, 1, 0);
                                D_o058_5F2C = 0;

                            }
                            break;
                        case 2:
                            if (D_o058_5F2C != 0) {
                                if (state->mode == 5) {
                                    mainChangeLevel(0xC, 0, 0, 0x12, 1, 0);
                                } else {
                                    mainChangeLevel(0xC, 0, 0, 0x11, 1, 0);
                                }
                                D_o058_5F2C = 0;

                            }
                            break;
                        case 3:
                            if (D_o058_5F2C != 0) {
                                mainChangeLevel(0xC, 0, 0, 0xC, 1, 0);
                                D_o058_5F2C = 0;

                            }
                            break;
                        }
                    } else {
                        D_o058_5EAC = 0;
                        D_o058_5E94 = D_o058_5EB0;
                        amSndPlay(0x1FAU, NULL);
                        D_o058_5EB0 = 0;
                        D_o058_5E98 = 0x140;
                        D_o058_5E9C = 0;
                        D_o058_5EA0 = 0x140;

                        D_o058_5EA8 = 0;
                    }
                }
            }
        }
        if (D_o058_5EA0 < 0) {
            D_o058_5EA0 = 0;
            return;
        }
        break;
    case 5:
        fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5EA8 + 0xA0, 0x50, D_8007C0B8->text[0x2C], 4);
        D_o058_5EA8 = -D_o058_5EA8;
        fontColour(0xFF, 0, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5EA8 + 0xA0, 0x8C, D_8007C0B8->text[0x2D], 4);
        D_o058_5EA8 = -D_o058_5EA8;

        D_o058_5EA8 = D_o058_5EA8 - (arg0 * 0xF);
        if (D_o058_5EA8 < 0) {
            if (D_800D31B8 & 0x9000) {
                func_8003A754();
                if ((D_8007BF44 > 0) && (state->mode == 0)) {
                    D_8007BF48 = -1;
                    if (D_o058_5F2C != 0) {
                        mainChangeLevel(0x12, 0, 0, 0xF, 1, 0);
                        D_o058_5F2C = 0;
                    }
                    mainSetAnimGroup(1);
                } else if (D_o058_5F2C != 0) {
                    mainChangeLevel(0xC, 0, 0, 0xC, 1, 0);
                    D_o058_5F2C = 0;
                }
            }
            D_o058_5EA8 = 0;
            return;
        }
        break;
    case 6:
        fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5EA8 + 0xA0, 0x50, D_8007C0B8->text[0x2C], 4);
        D_o058_5EA8 = -D_o058_5EA8;
        if (state->active == 0) {
            fontColour(0, 0xFF, 0, 0xFF, 0xFF);
            func_8004B0F8(&D_800D3140, D_o058_5EA8 + 0xA0, 0x8C, D_8007C0B8->text[0x6C], 4);
            D_o058_5EA8 = -D_o058_5EA8;
        } else {
            i = 0;
            if (D_8007BF1C & 0x100) {
                sprintf(&text[0], D_8007C0B8->text[0xB3]);
                countdownX = 0x6C;
            } else {

                if (state->countdown == 1) {
                    sprintf(&text[0], D_8007C0B8->text[0x2E], state->countdown);
                    countdownX = 0x94;
                } else {
                    sprintf(&text[0], D_8007C0B8->text[0x2F], state->countdown);
                    countdownX = 0x80;
                }
            }
            fontColour(0, 0xFF, 0, 0xFF, 0xFF);
            func_8004B0F8(&D_800D3140, D_o058_5EA8 + 0xA0, 0x8C, &text[0], 4);
            D_o058_5EA8 = -D_o058_5EA8;

            if ((s32) state->countdown > 0) {
                portraitX = countdownX + D_o058_5EA8;
                /* Adjacent index def; see the note in case 7/11. */
                i = 0;
                do {
                    nodes[i].texture = (RcpTextureInfo *) D_800D31C8[0x51 + state->entries[0].character];
                    nodes[i].alternate = 0;
                    nodes[i].x = portraitX;
                    nodes[i].y = 0xAA;
                    nodes[i].packedOffset = 0;
                    portraitX += 0x28;
                    i += 1;
                } while (i < (s32) state->countdown);
            }
            nodes[i].texture = 0;
            func_8002F618(&D_800D3140, (RcpTextureNode *) &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
        }
        D_o058_5EA8 = D_o058_5EA8 - (arg0 * 0xF);
        if (D_o058_5EA8 < 0) {
            if (D_800D31B8 & 0x9000) {
                if ((D_8007BF44 > 0) && (state->mode == 0)) {
                    D_8007BEF8 = 6;
                    D_8007BEFC = 5;
                    joyCreateMap(D_o058_5F48);
                    D_8007BF48 = (s32) D_8007C0C0[state->player][state->active];
                    D_8007BF4C = (s32) state->entries[0].character;
                    D_8007BF50 = 5;
                    D_8007BF54 = 0;
                    mainChangeLevel(0x12, 0, 0, 0xF, 1, 0);
                    mainSetAnimGroup(1);
                } else {
                    D_o058_5EC0 = 1;
                }
            }
            D_o058_5EA8 = 0;
            return;
        }
        break;
    case 8:
        x = D_o058_5E98 + D_o058_5EA8;
        i = 0;
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, x + 0xA0, 0x1E, func_8003A5A0((s32) D_800D304E), 4);
        x = -x;
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, x + 0xA0, 0x39, D_8007C0B8->text[0x27], 4);

        if (D_o058_5CD8 != 0) {
            if ((D_o058_5CD8 >= 2) && (x == 0)) {
                amSndPlay(0x27CU, NULL);
                D_o058_5CD8 = 1;
            }
            if (D_o058_5CD8 == 1) {
                func_8002F618(&D_800D3140, D_o058_5BA0, x + 0x30, 0x24, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
            }
        }
        /* Adjacent index def; see the note in case 7/11.  Carried by
         * `opponent`, not `i`, for the same reason as case 2's decrement
         * loop: it takes this loop's occurrences out of `i`'s web.  Of the
         * eighteen carriers measured here `opponent` is the best by a clear
         * margin (733 masked against 761 for the next), and `i` is dead after
         * this loop in case 8, so the rename is free of meaning. */
        opponent = 0;
        do {
            x = -x;
            textY = D_o058_5EAC + opponent * 0x1B + 0x5B;
            if (opponent == D_o058_5E8C) {
                fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
            } else {
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
            }
            sprintf(&text[0], D_8007C0B8->text[0x30], opponent + 1);
            func_8004B0F8(&D_800D3140, x + 0x82, textY, &text[0], 1);
            overlay56SplitTime(state->entries[0].lapTimes[opponent], &minutes, &seconds, &centiseconds);
            if (opponent != D_o058_5E8C) {
                fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
            }
            sprintf(&text[0], D_o058_5D74, minutes);
            func_8004B0F8(&D_800D3140, x + 0xB4, textY, &text[0], 1);
            func_8004B0F8(&D_800D3140, x + 0xB7, textY, D_o058_5D78, 0);
            sprintf(&text[0], D_o058_5D7C, seconds);
            func_8004B0F8(&D_800D3140, x + 0xBE, textY, &text[0], 0);
            func_8004B0F8(&D_800D3140, x + 0xD5, textY, D_o058_5D84, 0);
            sprintf(&text[0], D_o058_5D88, centiseconds);
            func_8004B0F8(&D_800D3140, x + 0xDC, textY, &text[0], 0);
            opponent += 1;
        } while (opponent != 3);
        x = -x;
        textY = textY + 0x2C;
        if (D_o058_5E90 != -1) {
            fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
        } else {
            fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        }
        func_8004B0F8(&D_800D3140, x + 0x82, textY, D_8007C0B8->text[0x31], 1);
        overlay56SplitTime(state->entries[0].value, &minutes, &seconds, &centiseconds);
        if (D_o058_5E90 == -1) {
            fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
        }
        sprintf(&text[0], D_o058_5D90, minutes);
        func_8004B0F8(&D_800D3140, x + 0xB4, textY, &text[0], 1);
        func_8004B0F8(&D_800D3140, x + 0xB7, textY, D_o058_5D94, 0);
        sprintf(&text[0], D_o058_5D98, seconds);
        func_8004B0F8(&D_800D3140, x + 0xBE, textY, &text[0], 0);
        func_8004B0F8(&D_800D3140, x + 0xD5, textY, D_o058_5DA0, 0);
        sprintf(&text[0], D_o058_5DA4, centiseconds);
        func_8004B0F8(&D_800D3140, x + 0xDC, textY, &text[0], 0);
        D_o058_5E98 -= arg0 * 0xF;
        if (D_o058_5E98 < 0) {
            D_o058_5E98 = 0;
            if ((D_o058_5EB0 == 0) && (D_800D31B8 & 0x9000)) {
                if ((D_o058_5E8C != -1) || (D_o058_5E90 != -1)) {
                    D_o058_5EB0 = 9;
                } else {
                    D_o058_5EB0 = 0xA;
                }
                amSndPlay(0xCU, NULL);
            }
            if (D_o058_5EB0 != 0) {
                D_o058_5EA8 += arg0 * 0xF;
                if (D_o058_5EA8 >= 0x141) {
                    D_o058_5EA8 = 0;
                    D_o058_5E98 = 0x140;
                    D_o058_5E94 = D_o058_5EB0;
                    D_o058_5EB0 = 0;
                    amSndPlay(0x1FAU, NULL);
                    return;
                }
            }
        }
        break;
    case 9:
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        x = D_o058_5E98 + D_o058_5EA8;
        func_8004B0F8(&D_800D3140, x + 0xA0, 0x1E, D_8007C0B8->text[0x32], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        D_o058_5E98 -= arg0 * 0xF;
        if (D_o058_5E98 < 0) {
            D_o058_5E98 = 0;
            if (D_o058_5EB0 == 0) {
                erase = 0;
                if (D_800D31B8 & 0x4000) {
                    erase = 1;
                } else if (D_800D31B8 & 0x9000) {
                    if ((D_o058_5E78 == 2) && (D_o058_5E7C == 9)) {
                        erase = 1;
                    } else if ((D_8007C1B4 == 3) || (D_o058_5E78 == 3)) {
                        if (D_o058_5E78 == 3) {
                            amSndPlay(0xCU, NULL);
                            D_o058_5EB0 = 0xA;
                            saves = func_800291C4();
                            slot = &saves[levelGetBlurEffect(D_800D3050)];
                            if (D_o058_5E8C != -1) {
                                slot->records[3].name[0] = func_8003A6B0(D_800D31C4[0]);
                                slot->records[3].name[1] = func_8003A6B0(D_800D31C4[1]);
                                slot->records[3].name[2] = func_8003A6B0(D_800D31C4[2]);
                            }
                            if (D_o058_5E90 != -1) {
                                slot->records[D_o058_5E90].name[0] = func_8003A6B0(D_800D31C4[0]);
                                slot->records[D_o058_5E90].name[1] = func_8003A6B0(D_800D31C4[1]);
                                slot->records[D_o058_5E90].name[2] = func_8003A6B0(D_800D31C4[2]);
                            }
                            func_80029120(levelGetBlurEffect(D_800D304E));
                            if (D_o058_5F30 != 0) {
                                func_8002917C();
                            }
                        } else {
                            amSndPlay(0xEU, NULL);
                        }
                    } else {
                        D_800D31C4[D_8007C1B4] = (u8) D_o058_5C5C[D_o058_5E78][D_o058_5E7C];
                        if (D_8007C1B4 == 2) {
                            D_o058_5E78 = 3;
                        }
                        D_8007C1B4 += 1;
                        amSndPlay(0xCU, NULL);
                    }
                }
                if (erase != 0) {
                    if (D_8007C1B4 == 0) {
                        amSndPlay(0xEU, NULL);
                    } else {
                        D_800D31C4[D_8007C1B4] = 0x20;
                        D_8007C1B4 -= 1;
                        i = 0;
                        do {
                            opponent = 0;
                            do {
                                if ((u8) D_o058_5C5C[i][opponent] == D_800D31C4[D_8007C1B4]) {
                                    D_o058_5E78 = i;
                                    D_o058_5E7C = opponent;
                                }
                                opponent += 1;
                            } while (opponent != 0xA);
                            i += 1;
                        } while (i != 3);
                        amSndPlay(0xDU, NULL);
                    }
                }

                if (D_800D31BE >= 0x11) {
                    if (D_o058_5E78 == 0) {
                        amSndPlay(0xEU, NULL);
                    } else {
                        amSndPlay(0xFU, NULL);
                        D_o058_5E78 -= 1;
                    }

                }
                if (D_800D31BE < -0x10) {
                    if ((D_o058_5E78 == 3) || (D_o058_5E78 == 3)) {
                        amSndPlay(0xEU, NULL);
                    } else {
                        amSndPlay(0xFU, NULL);
                        D_o058_5E78 += 1;
                    }
                }

                if (D_800D31BC < -0x10) {
                    if ((D_o058_5E7C == 0) || (D_o058_5E78 == 3)) {
                        amSndPlay(0xEU, NULL);
                    } else {
                        amSndPlay(0xFU, NULL);
                        D_o058_5E7C -= 1;
                    }

                }
                if (D_800D31BC >= 0x11) {
                    if ((D_o058_5E7C == 9) || (D_o058_5E78 == 3)) {
                        amSndPlay(0xEU, NULL);
                    } else {
                        amSndPlay(0xFU, NULL);
                        D_o058_5E7C += 1;
                    }
                }
            }
            if (D_o058_5EB0 != 0) {
                D_o058_5EA8 += arg0 * 0xF;
                if (D_o058_5EA8 >= 0x141) {
                    D_o058_5EA8 = 0;
                    D_o058_5E98 = 0x140;
                    D_o058_5E94 = D_o058_5EB0;
                    D_o058_5EB0 = 0;
                    amSndPlay(0x1FAU, NULL);
                }
            }
        }
        textY = 0x78;
        i = 0;
        do {
            opponent = 0;
            x = -x;
            columnX = 0x34 + x;
            do {
                /* Three discarded-expression probes on the index, at loop
                 * depth 2 -- zero instructions, and the only lever measured
                 * that moves this procedure's saved-register colouring.  Every
                 * one of its 428 allocator decisions is phase one, so the only
                 * axis is L100's ratio `save = totalsave / nocs`: a depth-2
                 * probe adds 100 to the index web's totalsave and nothing to
                 * its nocs, which lifts it past the text buffer's address temp
                 * and transposes the pair.  Two probes suffice for the tree as
                 * it stood; case 8's carrier above needs three, which is why
                 * three are written.  Six restructure the web and cost words.
                 * A probe at the top of a case (depth 0) is NOT free here. */
                if (i);
                if (i);
                if (i);
                sprintf(&text[0], D_o058_5DAC, (u8) D_o058_5C5C[i][opponent]);
                if ((opponent == D_o058_5E7C) && (i == D_o058_5E78)) {
                    fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
                } else {
                    fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                func_8004B0F8(&D_800D3140, columnX, textY, &text[0], 4);
                opponent += 1;
                columnX += 0x18;
                /* Spelled so the bound is not the literal 10: with `opponent < 0xA`
                 * uopt shares the constant with the erase loop's `!= 0xA` above,
                 * keeps 10 in a saved register through this loop and rewrites the
                 * exit test as bne, which shifts every colour after it. The target
                 * tests `slti $at, opponent, 0xA` with no shared register. */
            } while ((opponent - 1) < (0xA - 1));
            i += 1;
            textY += 0x1B;
        } while (i < 3);
        x = -x;
        if (D_o058_5E78 == 3) {
            fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
        } else {
            fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        }
        func_8004B0F8(&D_800D3140, x + 0xA0, textY, D_8007C0B8->text[0x33], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

        i = 0;
        columnX = x + 0x78;
        if (D_8007C1B4 > 0) {
            do {
                sprintf(&text[0], D_o058_5DB0, D_800D31C4[i]);
                func_8004B0F8(&D_800D3140, columnX, 0x50, &text[0], 4);
                i += 1;
                columnX += 0x1E;
            } while (i < D_8007C1B4);
        }
        if ((D_8007C1B4 != 3) && (D_o058_5E78 != 3)) {
            sprintf(&text[0], D_o058_5DB4, (u8) D_o058_5C5C[D_o058_5E78][D_o058_5E7C]);
            fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
            func_8004B0F8(&D_800D3140, columnX, 0x50, &text[0], 4);
            return;
        }
        break;
    case 10:
        x = D_o058_5E98 + D_o058_5EA8;
        i = 0;
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, x + 0xA0, 0x1E, func_8003A5A0((s32) D_800D304E), 4);
        x = -x;
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, x + 0xA0, 0x39, D_8007C0B8->text[0x34], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        saves = func_800291C4();
        if (i != 0);
        slot = &saves[levelGetBlurEffect(D_800D3050)];

        if (D_o058_5CD8 != 0) {
            if ((D_o058_5CD8 >= 2) && (x == 0)) {
                amSndPlay(0x27CU, NULL);
                D_o058_5CD8 = 1;
            }
            if (D_o058_5CD8 == 1) {
                func_8002F618(&D_800D3140, D_o058_5BA0, x + 0x30, 0x24, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
            }
        }
        textY = 0x5B;
        /* Adjacent index def; see the note in case 7/11. */
        i = 0;
        do {
            x = -x;
            overlay56SplitTime(slot->records[i].value, &minutes, &seconds, &centiseconds);
            if (slot->records[i].value == 0) {
                portraitIndex = 0x4A;
                sprintf(&text[0], D_o058_5DB8);
            } else {
                letter0 = func_8003A700(slot->records[i].name[0]) & 0xFF;
                letter1 = func_8003A700(slot->records[i].name[1]) & 0xFF;
                sprintf(&text[0], D_o058_5DC4, letter0, letter1, func_8003A700(slot->records[i].name[2]), minutes, seconds, centiseconds);
                portraitIndex = slot->records[i].character + 0x51;
            }
            highlighted = (i == D_o058_5E90) ||
                       ((i == 3) && (D_o058_5E8C != -1));
            if (highlighted != 0) {
                fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
            } else {
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
            }
            if (i < 3) {
                func_8004B0F8(&D_800D3140, x + 0x28, textY, D_o058_5C98[i], 0);
            }
            nodes[0].alternate = NULL;
            nodes[0].x = x + 0x58;
            nodes[0].y = textY - 4;
            nodes[0].packedOffset = 0;
            nodes[1].texture = 0;
            nodes[0].texture = D_800D31C8[portraitIndex];
            func_8002F618(&D_800D3140, (RcpTextureNode *) &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
            opponent = 0;
            do {
                if (highlighted == 0) {
                    if (opponent == 0) {
                        fontColour(0, 0xFF, 0xFF, 0xFF, 0xFF);
                    }
                    if (opponent == 3) {
                        fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
                    }
                }
                character[0] = text[opponent];
                character[1] = 0;
                func_8004B0F8(&D_800D3140, D_o058_5CB0[opponent] + x, textY, &character[0], 0);
                opponent += 1;
            } while (opponent != 11);
            if (i == 2) {
                textY += 0x1B;
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8004B0F8(&D_800D3140, x + 0xA0, textY, D_8007C0B8->text[0x35], 4);
            }
            i += 1;
            textY += 0x1B;
        } while (i != 4);
        D_o058_5E98 -= arg0 * 0xF;
        if (D_o058_5E98 < 0) {
            D_o058_5E98 = 0;
            if ((D_o058_5EB0 == 0) && (D_800D31B8 & 0x9000)) {
                D_o058_5EB0 = 0xB;
                amSndPlay(0xCU, NULL);
            }
            if (D_o058_5EB0 != 0) {
                D_o058_5EA8 += arg0 * 0xF;
                if (D_o058_5EA8 >= 0x141) {
                    D_o058_5E94 = D_o058_5EB0;
                    D_o058_5E9C = 0;
                    D_o058_5EA0 = 0x140;
                    D_o058_5EB0 = 0;
                    amSndPlay(0x1FAU, NULL);
                }
            }
        }
        break;
    default:
        break;
    }
}


#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o058/func_overlay_058_F000138C_18B0574/func_overlay_058_F000138C_18B0574.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_058_F000138C_18B0574:start
 * symbol: func_overlay_058_F000138C_18B0574
 * score: 733/3614 words, size delta 0
 * frame: 0x138
 * relocations: 1266
 * first-mismatch: +0x50
 * summary: 3614 instructions per side; the whole residual is p1 colour, and the axis was L100's save ratio because every one of the procedure's 428 allocator decisions is phase one. The s7-with-s8 transposition three grinds named is closed (0 transposed saved-register slots against 187) by two dials on that ratio: depth-2 discarded-expression probes on the index, and giving two inner loops index carriers of their own. Both dials are now exhausted -- 1,604 carrier x probe-count cells and 288 probe-site cells, no adoption -- and what is left is two web PARTITION problems, the prologue and case 10's `&character[0]` temp (web 978), each force-declined on all nine callee-saved colours
 * PLATEAU-HANDOFF:func_overlay_058_F000138C_18B0574:end
 */
