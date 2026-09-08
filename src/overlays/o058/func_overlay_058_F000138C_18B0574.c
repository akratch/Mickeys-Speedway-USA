#include "PR/ultratypes.h"

/* NON_MATCHING structural reconstruction from Mickey's runtime relocations.
 * Resident identities use the ROM-table exports and reserved section bases;
 * D_o058 names identify byte offsets in overlay 58, including its BSS.
 * The guarded body retains the target call graph and typed data accesses.
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
    RcpTextureNode nodes[4];
    char text[24];
    char character[2];
    s32 x;
    s32 seconds;
    s32 minutes;
    s32 centiseconds;
    s32 rowY;
    s32 rowHeight;
    s32 savedX;
    s32 savedPosition;
    s32 savedOffset;
    Overlay58RaceState *state;
    SavesPackedEntry *record;
    AnimPath *path;
    Overlay58RaceEntry **orderCursor;
    RcpTextureInfo **textureCursor;
    SavesSlot *saves;
    SavesSlot *slot;
    s16 *characterX;
    s32 columnStep;
    s32 portraitX;
    s32 *pointsCursor;
    s32 *rankCursor;
    s32 letter0;
    s32 pointsTotal;
    s32 letter1;
    s32 textY;
    s32 delta;
    s32 countdownX;
    s32 columnX;
    s32 portraitIndex;
    s32 opponent;
    s32 i;
    s32 highlighted;
    s32 columnCount;
    s32 erase;
    char **textCursor;
    char *characterCursor;
    u8 *erasedCharacter;
    u8 *nameCursor;

    state = func_80028F54();
    func_80036AB0(&D_o058_5F38, arg0);
    func_8004B0A4(0);
    if (((D_o058_5E94 == 1) || (D_o058_5E94 == 2)) &&
        (D_o058_5E50[0] == -1) && (D_8007BEF8 > 0)) {
        rankCursor = D_o058_5EF8;
        pointsCursor = D_o058_5E50;
        do {
            *pointsCursor = D_o058_5B28[*rankCursor];
            rankCursor += 1;
            pointsCursor += 1;
        } while ((u32) rankCursor < (u32) &D_o058_5EF8[D_8007BEF8]);
    }

    rowY = D_o058_5C74[D_8007BEF8 - 1];
    rowHeight = (s32) D_o058_5C68[D_8007BEF8 - 1];
    switch (D_o058_5E94) {
    case 1:
        i = 0;
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5EA4 + 0xA0, 0x1E, D_8007C0B8->text[0x27], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        x = D_o058_5E98 + D_o058_5EA8;
        if ((s32) D_8007BEF8 > 0) {
            orderCursor = D_o058_5EC8;
            rankCursor = D_o058_5EF8;
            do {
                x = -x;
                nodes[0].alternate = NULL;
                nodes[0].x = x + 0x4E;
                nodes[0].y = rowY - 4;
                nodes[0].packedOffset = 0;
                nodes[1].texture = 0;
                nodes[0].texture = D_800D31C8[0x51 + (*orderCursor)->character];
                func_8002F618(&D_800D3140, &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8004B0F8(&D_800D3140, x + 0x28, rowY, D_o058_5C98[*rankCursor], 0);
                if ((D_8007C1A0 == 1) && (state->entries == *orderCursor)) {
                    fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
                } else {
                    fontColour(0, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                func_8004B0F8(&D_800D3140, x + 0x71, rowY, D_8007C0B8->text[(*orderCursor)->character + 0x1A], 0);
                if (state->mode == 5) {
                    if (i == 0) {
                        overlay56SplitTime((*orderCursor)->value, &minutes, &seconds, &centiseconds);
                    } else {
                        overlay56SplitTime((*D_o058_5EC8)->value - (*orderCursor)->value, &minutes, &seconds, &centiseconds);
                    }
                } else if (i == 0) {
                    overlay56SplitTime((*orderCursor)->value, &minutes, &seconds, &centiseconds);
                } else {
                    overlay56SplitTime((*orderCursor)->value - (*D_o058_5EC8)->value, &minutes, &seconds, &centiseconds);
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
                rankCursor += 1;
                orderCursor += 1;
                rowY += rowHeight;
            } while (i < (s32) D_8007BEF8);
        }
        delta = arg0 * 0xF;
        D_o058_5E98 = D_o058_5E98 - delta;
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
                D_o058_5EA4 += delta;
                if ((D_o058_5EB0 == 5) || (D_o058_5EB0 == 6)) {
                    D_o058_5EA8 = D_o058_5EA8 + delta;
                    if (D_o058_5EA8 >= 0x141) {
                        D_o058_5E94 = D_o058_5EB0;
                        amSndPlay(0x1FAU, NULL);
                        return;
                    }
                } else {
                    D_o058_5EAC = D_o058_5EAC + delta;

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
                    pointsCursor = D_o058_5E50;
                    do {

                        if (*pointsCursor > 0) {
                            *pointsCursor = *pointsCursor - 1;
                        }
                        pointsCursor += 1;
                    } while ((u32) pointsCursor < (u32) &D_o058_5E50[D_8007BEF8]);
                    i = 0;
                }
            }
        }
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5EA0 + D_o058_5EA4 + 0xA0, 0x1E, D_8007C0B8->text[0x28], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        savedX = D_o058_5E98;
        savedOffset = D_o058_5EA0;
        if ((s32) D_8007BEF8 > 0) {
            orderCursor = D_o058_5EC8;
            rankCursor = D_o058_5EF8;
            pointsCursor = D_o058_5E50;
                do {
                D_o058_5E98 = -D_o058_5E98;
                D_o058_5EA0 = -D_o058_5EA0;

                nodes[0].texture = D_800D31C8[0x51 + (*orderCursor)->character];
                nodes[0].alternate = NULL;
                nodes[0].x = D_o058_5E98 + D_o058_5EA0 + 0x4E;
                nodes[0].y = rowY - 4;
                nodes[0].packedOffset = 0;
                nodes[1].texture = 0;
                func_8002F618(&D_800D3140, (RcpTextureNode *) &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5EA0 + 0x28, rowY, D_o058_5C98[*rankCursor], 0);
                if ((D_8007C1A0 == 1) && (state->entries == *orderCursor)) {
                    fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
                } else {
                    fontColour(0, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5EA0 + 0x71, rowY, D_8007C0B8->text[(*orderCursor)->character + 0x1A], 0);
                pointsTotal = (*orderCursor)->rank - *pointsCursor;
                fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
                sprintf(&text[0], D_o058_5D50, pointsTotal);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + D_o058_5EA0 + 0xBE, rowY, &text[0], 0);
                sprintf(&text[0], D_o058_5D54, *pointsCursor);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + D_o058_5EA0 + 0xEB, rowY, &text[0], 0);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + D_o058_5EA0 + 0x113, rowY, D_o058_5D5C, 0);
                i += 1;
                pointsCursor += 1;
                rankCursor += 1;
                orderCursor += 1;
                rowY += rowHeight;
            } while (i < (s32) D_8007BEF8);
        }
        D_o058_5E98 = savedX;
        D_o058_5EA0 = savedOffset;
        delta = arg0 * 0xF;
        D_o058_5E9C -= delta;
        D_o058_5EA0 = D_o058_5EA0 - delta;
        D_o058_5EA4 = D_o058_5EA4 - delta;

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
                D_o058_5E98 = D_o058_5E98 + delta;
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
        i = 0;
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0xA0, 0x1E, D_8007C0B8->text[0x29], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
        savedX = D_o058_5E98;
        savedPosition = D_o058_5E9C;
        if ((s32) D_8007BEF8 > 0) {
                pointsCursor = D_o058_5F10;
            orderCursor = D_o058_5EE0;
            do {
                D_o058_5E98 = -D_o058_5E98;
                D_o058_5E9C = -D_o058_5E9C;

                nodes[0].texture = D_800D31C8[0x51 + (*orderCursor)->character];
                nodes[0].alternate = NULL;
                nodes[0].x = D_o058_5E98 + D_o058_5E9C + 0x4E;
                nodes[0].y = rowY - 4;
                nodes[0].packedOffset = 0;
                nodes[1].texture = 0;
                func_8002F618(&D_800D3140, (RcpTextureNode *) &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0x28, rowY, D_o058_5C98[*pointsCursor], 0);
                if ((D_8007C1A0 == 1) && (state->entries == *orderCursor)) {
                    fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
                } else {
                    fontColour(0, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0x71, rowY, D_8007C0B8->text[(*orderCursor)->character + 0x1A], 0);
                sprintf(&text[0], D_o058_5D60, (*orderCursor)->rank);
                fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
                func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0xD2, rowY, &text[0], 0);
                if ((*orderCursor)->rank == 1) {
                    func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0x104, rowY, D_o058_5D64, 0);
                } else {
                    func_8004B0F8(&D_800D3140, D_o058_5E98 + D_o058_5E9C + 0x104, rowY, D_o058_5D68, 0);
                }
                i += 1;
                orderCursor += 1;
                pointsCursor += 1;
                rowY += rowHeight;
            } while (i < (s32) D_8007BEF8);
        }
        delta = arg0 * 0xF;
        D_o058_5E98 = savedX;
        D_o058_5E98 = savedX - delta;

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
                D_o058_5E9C = D_o058_5E9C + delta;
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
        i = 0;
        fontColour(0xFF, 0x80, 0, 0xFF, 0xFF);
        func_8004B0F8(&D_800D3140, D_o058_5E9C + D_o058_5EA0 + 0xA0, 0x1E, D_8007C0B8->text[0x6E], 4);
        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);


        columnStep = D_o058_5C8C[D_8007BEF8 - 1];
        columnX = D_o058_5C80[D_8007BEF8 - 1] + D_o058_5E9C + D_o058_5EA0;
        if ((s32) D_8007BEF8 > 0) {
            textCursor = D_o058_5C98;
            do {
                func_8004B0F8(&D_800D3140, columnX, 0x37, *textCursor, 4);
                i += 1;
                textCursor += 1;
                columnX += columnStep;
            } while (i < (s32) D_8007BEF8);
            i = 0;
        }
        savedPosition = D_o058_5E9C;
        savedOffset = D_o058_5EA0;
        if ((s32) D_8007BEF8 > 0) {
            orderCursor = D_o058_5EE0;
            do {
                D_o058_5E9C = -D_o058_5E9C;
                D_o058_5EA0 = -D_o058_5EA0;

                nodes[0].texture = D_800D31C8[0x51 + (*orderCursor)->character];
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
                        sprintf(&text[0], D_o058_5D6C, (*orderCursor)->counters[opponent]);
                        func_8004B0F8(&D_800D3140, columnX, rowY + 0x16, &text[0], 4);
                        opponent += 1;
                        columnX += columnStep;
                    } while (opponent < (s32) D_8007BEF8);
                }
                i += 1;
                orderCursor += 1;
                rowY += rowHeight;
            } while (i < (s32) D_8007BEF8);
        }
        delta = arg0 * 0xF;
        D_o058_5EA0 = savedOffset;
        D_o058_5EA0 = savedOffset - delta;

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
                D_o058_5E9C = D_o058_5E9C + delta;
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
            textureCursor = D_800D31C8;
            do {
                nodes[0].alternate = NULL;
                nodes[0].x = portraitX;
                nodes[0].y = 0x37;
                nodes[0].packedOffset = 0;
                nodes[1].texture = 0;
                nodes[0].texture = textureCursor[0x51];
                func_8002F618(&D_800D3140, (RcpTextureNode *) &nodes[0], 0, 0, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF, (u8) 0xFF);
                i += 1;
                textureCursor += 1;
                portraitX += columnStep;
            } while (i != columnCount);
            i = 0;
        }
        savedPosition = D_o058_5E9C;
        savedOffset = D_o058_5EA0;
        if ((s32) D_8007BEF8 > 0) {
            orderCursor = D_o058_5EE0;
            do {
                D_o058_5E9C = -D_o058_5E9C;
                D_o058_5EA0 = -D_o058_5EA0;

                nodes[0].texture = D_800D31C8[0x51 + (*orderCursor)->character];
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
                        sprintf(&text[0], D_o058_5D70, (*orderCursor)->flags[opponent]);
                        func_8004B0F8(&D_800D3140, columnX + 8, rowY + 0x16, &text[0], 4);
                        opponent += 1;
                        columnX += columnStep;
                    } while (opponent != columnCount);
                }
                i += 1;
                orderCursor += 1;
                rowY += rowHeight;
            } while (i < (s32) D_8007BEF8);
        }
        delta = arg0 * 0xF;
        D_o058_5EA0 = savedOffset;
        D_o058_5EA0 = savedOffset - delta;

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
                D_o058_5E9C = D_o058_5E9C + delta;
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
        textCursor = D_o058_5E68;
        textY = 0x50;
        do {
            if (i == D_o058_5F28) {
                fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
            } else {
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
            }
            func_8004B0F8(&D_800D3140, 0xA0 - x, textY, *textCursor, 4);
            i += 1;
            textY += 0x1E;
            textCursor += 1;
            x = -x;
        } while (i < 4);
        delta = arg0 * 0xF;
        D_o058_5EA0 = D_o058_5EA0 - delta;

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
                D_o058_5E9C = D_o058_5E9C + delta;
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
                            state->entries[0].value = 0;
                            state->entries[1].value = 0;

                            state->entries[3].value = 0;
                            state->entries[4].value = 0;
                            state->entries[5].value = 0;
                            state->entries[2].value = 0;
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
                do {
                    nodes[i].alternate = 0;
                    nodes[i].x = portraitX;
                    nodes[i].y = 0xAA;
                    nodes[i].packedOffset = 0;
                    nodes[i].texture = (RcpTextureInfo *) D_800D31C8[0x51 + state->entries[0].character];
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
        do {
            x = -x;
            textY = D_o058_5EAC + i * 0x1B + 0x5B;
            if (i == D_o058_5E8C) {
                fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
            } else {
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
            }
            sprintf(&text[0], D_8007C0B8->text[0x30], i + 1);
            func_8004B0F8(&D_800D3140, x + 0x82, textY, &text[0], 1);
            overlay56SplitTime(state->entries[0].lapTimes[i], &minutes, &seconds, &centiseconds);
            if (i != D_o058_5E8C) {
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
            i += 1;
        } while (i != 3);
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
        delta = arg0 * 0xF;
        D_o058_5E98 = D_o058_5E98 - delta;
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
                D_o058_5EA8 = D_o058_5EA8 + delta;
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
        delta = arg0 * 0xF;
        D_o058_5E98 = D_o058_5E98 - delta;
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
                            slot = &saves[levelGetBlurEffect(D_800D304E)];
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
                        erasedCharacter = &D_800D31C4[D_8007C1B4 - 1];
                        D_8007C1B4 -= 1;
                        textCursor = D_o058_5C5C;
                        i = 0;
                        do {
                            opponent = 0;
                            characterCursor = *textCursor;
                            do {
                                if ((u8) *characterCursor == *erasedCharacter) {
                                    D_o058_5E78 = i;
                                    D_o058_5E7C = opponent;
                                }
                                opponent += 1;
                                characterCursor += 1;
                            } while (opponent != 0xA);
                            i += 1;
                            textCursor += 1;
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
                D_o058_5EA8 = D_o058_5EA8 + delta;
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
        textCursor = D_o058_5C5C;
        i = 0;
        do {
            opponent = 0;
            x = -x;
            columnX = 0x34 + x;
            do {
                sprintf(&text[0], D_o058_5DAC, (u8) (*textCursor)[opponent]);
                if ((opponent == D_o058_5E7C) && (i == D_o058_5E78)) {
                    fontColour((s32) D_o058_5F38.red, (s32) D_o058_5F38.green, (s32) D_o058_5F38.blue, 0xFF, 0xFF);
                } else {
                    fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                func_8004B0F8(&D_800D3140, columnX, textY, &text[0], 4);
                opponent += 1;
                columnX += 0x18;
            } while (opponent < 0xA);
            i += 1;
            textCursor += 1;
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
            nameCursor = D_800D31C4;
            do {
                sprintf(&text[0], D_o058_5DB0, *nameCursor);
                func_8004B0F8(&D_800D3140, columnX, 0x50, &text[0], 4);
                i += 1;
                nameCursor += 1;
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
        slot = &saves[levelGetBlurEffect(D_800D304E)];

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
        record = slot->records;
        do {
            x = -x;
            overlay56SplitTime(record->value, &minutes, &seconds, &centiseconds);
            if (record->value == 0) {
                portraitIndex = 0x4A;
                sprintf(&text[0], D_o058_5DB8);
            } else {
                letter0 = func_8003A700(record->name[0]) & 0xFF;
                letter1 = func_8003A700(record->name[1]) & 0xFF;
                sprintf(&text[0], D_o058_5DC4, letter0, letter1, func_8003A700(record->name[2]), minutes, seconds, centiseconds);
                portraitIndex = record->character + 0x51;
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
            characterX = D_o058_5CB0;
            characterCursor = &text[0];
            do {
                if (highlighted == 0) {
                    if (characterX == D_o058_5CB0) {
                        fontColour(0, 0xFF, 0xFF, 0xFF, 0xFF);
                    }
                    if (characterX == &D_o058_5CB0[3]) {
                        fontColour(0xFF, 0xFF, 0, 0xFF, 0xFF);
                    }
                }
                character[1] = 0;
                character[0] = (s8) (u8) *characterCursor;
                func_8004B0F8(&D_800D3140, *characterX + x, textY, &character[0], 0);
                characterX += 1;
                characterCursor += 1;
            } while (characterX != &D_o058_5CB0[11]);
            if (i == 2) {
                textY += 0x1B;
                fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                func_8004B0F8(&D_800D3140, x + 0xA0, textY, D_8007C0B8->text[0x35], 4);
            }
            i += 1;
            record += 1;
            textY += 0x1B;
        } while (i != 4);
        delta = arg0 * 0xF;
        D_o058_5E98 = D_o058_5E98 - delta;
        if (D_o058_5E98 < 0) {
            D_o058_5E98 = 0;
            if ((D_o058_5EB0 == 0) && (D_800D31B8 & 0x9000)) {
                D_o058_5EB0 = 0xB;
                amSndPlay(0xCU, NULL);
            }
            if (D_o058_5EB0 != 0) {
                D_o058_5EA8 = D_o058_5EA8 + delta;
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
