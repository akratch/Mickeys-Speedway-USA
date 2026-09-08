#include "PR/ultratypes.h"
#include "game/menu.h"
#include "game/font.h"
#include "game/charControl.h"
#include "game/anim.h"
#include "game/math.h"
#include "game/gameVi.h"
extern int sprintf(char *buffer, const char *format, ...);

/* Tier B: the call sites and runtime relocation records identify these
 * resident interfaces. The local views below use Mickey's menu, save-record,
 * and RCP field layouts; unknown fields retain their offsets. */
struct MenuCommand { u32 w0; u32 w1; };
typedef struct RcpTextureInfo RcpTextureInfo;
typedef struct RcpTextureNode {
    RcpTextureInfo *texture;
    RcpTextureInfo *alternate;
    u32 packedOffset;
    s16 x;
    s16 y;
} RcpTextureNode;
typedef struct SavesPackedEntry {
    s32 time;
    u8 initials[3];
    u8 character;
} SavesPackedEntry;
typedef struct SavesSlot { SavesPackedEntry records[4]; } SavesSlot;
typedef struct MenuSpawnInner {
    u8 pad00[8];
    s16 mode;
} MenuSpawnInner;

extern ControlActor **func_8000572C(s32 *start, s32 *end);
extern void amSndPlay(u16 sound, void **handle);
extern s32 mathRnd(s32 minimum, s32 maximum);
extern void func_80000510(u8 sequence);
extern void func_800005CC(f32 fade, u8 volume);
extern void func_8004B0A4(s32 font);
extern void func_8004B0DC(s32 red, s32 green, s32 blue, s32 alpha);
extern void func_8004B0F8(MenuCommand **commands, s32 x, s32 y, char *text, s32 flags);
extern void func_8002F618(MenuCommand **commands, RcpTextureNode *texture,
                        s32 x, s32 y, s32 red, s32 green, s32 blue, s32 alpha);
extern void func_8002FB34(MenuCommand **commands, RcpTextureNode *texture,
                        f32 x, f32 y, f32 scaleX, f32 scaleY, s32 alpha, s32 flags);
extern void func_800367A4(void *texture, s32 *state, s32 speed, f32 *frame, s32 ticks);
extern void func_8003A520(s32 split);
extern u8 func_8003A700(u8 initial);
extern void func_800336A8(s32 screenMode);
extern void func_80033FE0(void);
extern void func_80021504(f32 fov, s32 force);
extern MtxF *func_8002468C(void);
extern void func_80029198(void);
extern SavesSlot *func_800291C4(void);
extern s32 levelGetBlurEffect(s32 level);
extern void func_80034920(MenuCommand **commands);
extern void func_800349A4(MenuCommand **commands, s32 arg1, s32 arg2, s32 arg3);
extern void camStandardPersp(MenuCommand **commands, void **matrices);
extern void camStandardOrtho(MenuCommand **commands, void **matrices);
extern void rcpClearZBuffer(MenuCommand **commands, s32 width, s32 height,
                           s32 left, s32 top, s32 right, s32 bottom);
extern void func_8005AD64(ControlActor *object, s32 arg1, s32 arg2, s32 arg3);
extern s32 func_8005ABA8(ControlActor *object, f32 scale, f32 ticks);
extern void func_80020D8C(MenuSpawnInner *model, s32 index, s32 frame);
extern void func_80009E78(MenuCommand **commands, void **matrices,
                        void **vertices, ControlActor *object);
extern void *func_800355A0(s32 asset, s32 flags);
extern void func_800359D4(void *sprite);
extern void func_80023F84(MenuCommand **commands, void **matrices,
                        void **vertices, void *transform, void *sprite,
                        s32 flags, s32 alpha);
extern void mainChangeLevel(s32 level, s32 entrance, s32 cutscene,
                            s32 arg3, s32 arg4, s32 arg5);
extern s32 func_overlay_041_F000124C_1888584(s32 index);
extern void func_overlay_048_F0000000_1895408(void);
extern void func_overlay_056_F00000B8_18A2E30(s32 time, s32 *minutes,
                                           s32 *seconds, s32 *hundredths);
extern s32 func_overlay_068_F000146C_18C85CC(s32 index);
extern s32 func_overlay_082_F0000498_18CF618(ControlActor *object);
extern s32 func_overlay_082_F00004A4_18CF624(ControlActor *object);
extern void func_overlay_082_F00004B0_18CF630(ControlActor *object);
extern void func_overlay_082_F00004C0_18CF640(ControlActor *object);
extern s32 func_overlay_060_F0002F54_18BCD2C(s32 left, s32 bottom, s32 width,
                                         s32 height, s32 value, s32 alpha, s32 ticks);
extern void overlay60DrawBorder(s32 left, s32 top, s32 right, s32 bottom);
extern void overlay60DrawLine(s32 x1, s32 y1, s32 x2, s32 y2);
extern void overlay60ReassignChoiceSlots(void);

extern u16 D_8007BF14;
extern u16 D_8007BF1C;
extern char **D_8007C0B8;
extern s16 D_8007C0E8[];
extern s16 D_8007C11C[];
extern s32 D_800D2FC0;
extern MenuScreenModeState D_800D3128;
extern u16 D_800D312C;
extern u16 D_800D312E;
extern u16 D_800D3130[];
extern u8 D_800D313B;
extern u16 D_800D313C;
extern MenuCommand *D_800D3140;
extern void *D_800D3144;
extern void *D_800D3148;
extern s32 D_800D31B4;
extern s32 D_800D31B8;
extern s16 D_800D31BC;
extern s16 D_800D31BE;
extern RcpTextureInfo *D_800D31C8[];

/* Tier B: overlay-local identities come from overlay 60's LOCAL records.
 * Data and BSS are distinct even where their stored addends coincide. */
extern RcpTextureNode gOverlay60Data020;
extern RcpTextureNode gOverlay60Data040;
extern RcpTextureNode gOverlay60Data060;
extern RcpTextureNode gOverlay60Data080;
extern s32 gOverlay60Data0A0;
extern f32 gOverlay60Data0A4;
extern ControlActor *gOverlay60Data0A8;
extern char gOverlay60Data0C0[];
extern char gOverlay60Data0C4[];
extern ControlActor *gOverlay60Data0C8[];
extern s16 gOverlay60Data0D8[];
extern s16 gOverlay60Data0E8[];
extern f32 gOverlay60Data0F8[];
extern s32 gOverlay60Data10C[];
extern s32 gOverlay60Data11C[];
extern s8 gOverlay60Data12B[];
extern s32 gOverlay60Data130;
extern void *gOverlay60Data134;
extern u16 gOverlay60Data138[];
extern s32 gOverlay60Data14C;
extern s32 gOverlay60Data150;
extern s32 gOverlay60Data154;
extern s32 gOverlay60Data158;
extern s32 gOverlay60Data15C;
extern s32 gOverlay60Data160;
extern s32 gOverlay60Data164;
extern s16 gOverlay60Data168[];
extern void *gOverlay60Data174;
extern u8 gOverlay60Data178[];
extern char *gOverlay60Data1A4[];
extern s16 gOverlay60Data1B4[];
extern s16 gOverlay60Data1D0[];
extern char gOverlay60Data1F0[];
extern char gOverlay60Data1F8[];
extern char gOverlay60Data200[];
extern char gOverlay60Data20C[];
extern char gOverlay60Data220[];
extern char gOverlay60Data228[];
extern f32 gOverlay60Data258;
extern char *gOverlay60Data260[];
extern char *gOverlay60Data288[];
extern char *gOverlay60Data298[];
extern s32 gOverlay60Data2A0;
extern s32 gOverlay60Data2A4;
extern s32 gOverlay60Data2A8;
extern s32 gOverlay60Data2AC;
extern s32 gOverlay60Data2B0;
extern s32 gOverlay60Data2B4;
extern s32 gOverlay60Data2B8;
extern u8 D_800000B0[];

/* Tier D: these are byte views of fields not named by MenuScreenModeBits. */
#define O60_SETTINGS_WORD (*(u32 *)&D_800D3128)
#define O60_SETTINGS_BYTE(n) (((u8 *)&D_800D3128)[n])
#define O60_TEXT(offset) D_8007C0B8[(offset) / 4]

/* Tier B/D: menu update and drawing reconstructed from Mickey's call graph,
 * field accesses, and ten-case dispatch. NON_MATCHING retains the ROM body. */
#ifdef NON_MATCHING
void func_overlay_060_F0000334_18BA10C(s32 ticks) {
    s32 panel;
    s32 first;
    s32 end;
    ControlActor **objects;
    ControlActor *object;
    s32 i;
    s32 count;
    s32 value;
    s32 mode;
    s32 oldDetail;
    s32 showArrows;
    s32 limit;
    s32 minutes;
    s32 seconds;
    s32 hundredths;
    s32 initial0;
    s32 initial1;
    s32 icon;
    s32 row;
    s16 y;
    s8 enabled[16];
    char text[128];
    char glyph[2];
    char *letter;
    char **recordLabel;
    s16 *recordY;
    s16 *column;
    s32 *mask;
    s32 *shift;
    s8 *rank;
    SavesSlot *slots;
    SavesPackedEntry *record;
    MtxF *projection;
    AnimPath *path;
    MenuSpawnInner *model;
    MenuCommand *command;
    f32 elapsed;
    s32 left;
    s32 right;

    if (gOverlay60Data0A8 == NULL) {
        objects = func_8000572C(&first, &end);
        while (first < end) {
            object = objects[first++];
            if (object->kind == 0x54) {
                gOverlay60Data0A8 = object;
                first = end;
            }
        }
    }
    if (gOverlay60Data160 != 0) {
        frontSetLanguage(D_800D3128.bits.language);
        frontSetStereoMode(D_800D3128.bits.stereoMode);
        frontSetSfxVolume(D_800D312C);
        frontSetBgmVolume(D_800D312E);
        gOverlay60Data160 = 0;
    }
    if (gOverlay60Data0A8 != NULL &&
        func_overlay_041_F000124C_1888584(gOverlay60Data2B8) != 0) {
        func_8004B0A4(2);
        gOverlay60Data2A0 += ticks * 2;
        if (gOverlay60Data2A0 >= 0x100) {
            gOverlay60Data2A0 = 0xFF;
        }
        fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A0);
        panel = func_overlay_082_F0000498_18CF618(gOverlay60Data0A8);
        if (gOverlay60Data2AC == -1 &&
            func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0) {
            gOverlay60Data2AC = panel;
            gOverlay60Data14C = 0;
            gOverlay60Data2B0 = 0;
            gOverlay60Data2B4 = 0;
            gOverlay60Data150 = 0;
            gOverlay60Data154 = 1;
            gOverlay60Data158 = 0;
        }
        if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0 &&
            panel == gOverlay60Data2AC) {
            gOverlay60Data2A4 += ticks * 8;
            if (gOverlay60Data2A4 >= 0x100) {
                gOverlay60Data2A4 = 0xFF;
            }
        } else {
            gOverlay60Data2A4 -= ticks * 8;
            if (gOverlay60Data2A4 < 0) {
                gOverlay60Data2A4 = 0;
                gOverlay60Data2AC = -1;
            }
        }
        if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0) {
            gOverlay60Data2A8 -= ticks * 4;
            if (gOverlay60Data2A8 < 0x69) {
                gOverlay60Data2A8 = 0x69;
            }
        } else {
            gOverlay60Data2A8 += ticks * 4;
            if (gOverlay60Data2A8 >= 0x9C) {
                gOverlay60Data2A8 = 0x9B;
            }
        }
        if (gOverlay60Data2A4 != 0) {
            switch (gOverlay60Data2AC) {
            case 0:
                if (gOverlay60Data134 == NULL) {
                    amSndPlay(gOverlay60Data138[mathRnd(0, 9)],
                              &gOverlay60Data134);
                }
                frontSetSfxVolume(func_overlay_060_F0002F54_18BCD2C(
                    0x5F, 0xB9, 0x64, 0x28, frontGetSfxVolume(),
                    gOverlay60Data2A4, ticks));
                break;
            case 1:
                if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0) {
                    if (D_800D31BE < 0) {
                        if (gOverlay60Data2B0 == 0) {
                            gOverlay60Data2B0 = 1;
                            amSndPlay(0xF, NULL);
                        } else {
                            amSndPlay(0xE, NULL);
                        }
                    } else if (D_800D31BE > 0) {
                        if (gOverlay60Data2B0 == 1) {
                            gOverlay60Data2B0 = 0;
                            amSndPlay(0xF, NULL);
                        } else {
                            amSndPlay(0xE, NULL);
                        }
                    }
                }
                if (gOverlay60Data2B0 == 0) {
                    fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                    if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0 &&
                        D_800D31BC != 0) {
                        O60_SETTINGS_BYTE(2) = (O60_SETTINGS_BYTE(2) & ~2) |
                            ((((O60_SETTINGS_WORD >> 9) & 1) ^ 1) << 1);
                        amSndPlay(0xF, NULL);
                    }
                } else {
                    fontColour(0, 0xBE, 0, 0xFF, gOverlay60Data2A4);
                }
                sprintf(text, gOverlay60Data1F0, O60_TEXT(0x254),
                        gOverlay60Data298[(O60_SETTINGS_WORD >> 9) & 1]);
                func_8004B0F8(&D_800D3140, 0x91, 0x96, text, 0xC);
                if (gOverlay60Data2B0 == 1) {
                    fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                    if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0 &&
                        D_800D31BC != 0) {
                        O60_SETTINGS_BYTE(2) = (O60_SETTINGS_BYTE(2) & ~1) |
                            ((O60_SETTINGS_BYTE(2) ^ 1) & 1);
                        amSndPlay(0xF, NULL);
                    }
                } else {
                    fontColour(0, 0xBE, 0, 0xFF, gOverlay60Data2A4);
                }
                sprintf(text, gOverlay60Data1F8, O60_TEXT(0x258),
                        gOverlay60Data298[O60_SETTINGS_BYTE(2) & 1]);
                func_8004B0F8(&D_800D3140, 0x91, 0xAA, text, 0xC);
                break;
            case 2:
                value = frontGet2PlayerSplit();
                if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0) {
                    if (D_800D31BC != 0) {
                        value ^= 1;
                        func_8003A520(value);
                        amSndPlay(0xF, NULL);
                    }
                }
                fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                func_8004B0F8(&D_800D3140, 0x91, 0x9B,
                              D_8007C0B8[0x1E8 / 4 + value], 0xC);
                break;
            case 3:
                count = 0;
                for (i = 0; i < 16; i++) {
                    if (D_800D313C & (1 << i)) {
                        enabled[count++] = i;
                    }
                }
                if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0 &&
                    count > 0) {
                    if (D_800D31BE > 0 && gOverlay60Data2B0 > 0) {
                        gOverlay60Data2B0--;
                        if (gOverlay60Data2B0 < gOverlay60Data2B4) {
                            gOverlay60Data2B4 = gOverlay60Data2B0;
                        }
                        amSndPlay(0xF, NULL);
                    } else if (D_800D31BE < 0 && gOverlay60Data2B0 < count - 1) {
                        gOverlay60Data2B0++;
                        if (gOverlay60Data2B4 < gOverlay60Data2B0 - 5) {
                            gOverlay60Data2B4 = gOverlay60Data2B0 - 5;
                        }
                        amSndPlay(0xF, NULL);
                    } else if (D_800D31BC != 0) {
                        D_8007BF1C ^= 1 << enabled[gOverlay60Data2B0];
                        amSndPlay(0xF, NULL);
                    }
                }
                y = 0x8C;
                for (i = gOverlay60Data2B4;
                     i < gOverlay60Data2B4 + 6 && i < count; i++) {
                    if (i == gOverlay60Data2B0) {
                        fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                    } else {
                        fontColour(0, 0xBE, 0, 0xFF, gOverlay60Data2A4);
                    }
                    func_8004B0F8(&D_800D3140, 0x4D, y,
                                  D_8007C0B8[0x2A4 / 4 + enabled[i]], 8);
                    func_8004B0F8(&D_800D3140, 0xC1, y,
                        gOverlay60Data298[(D_8007BF1C >> enabled[i]) & 1], 8);
                    y += 0xC;
                }
                break;
            case 4:
                frontSetBgmVolume(func_overlay_060_F0002F54_18BCD2C(
                    0x5F, 0xB9, 0x64, 0x28, frontGetBgmVolume(),
                    gOverlay60Data2A4, ticks));
                if (D_8007BF1C & 0x80) {
                    if (gOverlay60Data150 > 0) {
                        gOverlay60Data150 -= ticks;
                        if (gOverlay60Data150 <= 0) {
                            gOverlay60Data150 = 0;
                            func_80000510(((u8 *)&gOverlay60Data15C)[3]);
                        }
                    }
                    if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0) {
                        if ((D_800D31B8 & 4) && gOverlay60Data2B0 > 0) {
                            gOverlay60Data2B0--;
                            amSndPlay(0xF, NULL);
                        } else if ((D_800D31B8 & 8) && gOverlay60Data2B0 < 0x2A) {
                            gOverlay60Data2B0++;
                            amSndPlay(0xF, NULL);
                        } else if (D_800D31B8 & 0x9000) {
                            if (gOverlay60Data150 == 0) {
                                func_800005CC(0.5f, 0);
                                gOverlay60Data150 = 0x1E;
                            }
                            gOverlay60Data15C = gOverlay60Data2B0 + 1;
                            amSndPlay(0xC, NULL);
                        }
                    }
                    sprintf(text, O60_TEXT(0x2C8), gOverlay60Data2B0 + 1);
                    fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x91, 0x7F, text, 0xC);
                    fontColour(0xFF, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x91, 0x75, gOverlay60Data0C0, 0xC);
                    func_8004B0F8(&D_800D3140, 0x91, 0x89, gOverlay60Data0C4, 0xC);
                }
                break;
            case 5:
                mode = frontGetStereoMode();
                if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0) {
                    if (D_800D31BC < 0) {
                        mode--;
                        if (mode < 0) {
                            mode = 3;
                        }
                        amSndPlay(0xF, NULL);
                    } else if (D_800D31BC > 0) {
                        mode++;
                        if (mode >= 4) {
                            mode = 0;
                        }
                        amSndPlay(0xF, NULL);
                    }
                }
                frontSetStereoMode(mode);
                if (mode == 3) {
                    func_8002F618(&D_800D3140, &gOverlay60Data020,
                                  0x68, 0x8C, 0, 0xFF, 0, gOverlay60Data2A4);
                    fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x91, 0xB9, O60_TEXT(0x298), 0xC);
                    func_8004B0F8(&D_800D3140, 0x91, 0xC3, O60_TEXT(0x29C), 0xC);
                    func_8004B0F8(&D_800D3140, 0x91, 0xCD, O60_TEXT(0x2A0), 0xC);
                } else {
                    fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x91, 0x9B, gOverlay60Data288[mode], 0xC);
                }
                break;
            case 6:
                mode = frontGetScreenMode();
                if (gOverlay60Data158 == 0 &&
                    func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0 &&
                    D_800D31BC != 0) {
                    func_overlay_082_F00004B0_18CF630(gOverlay60Data0A8);
                    gOverlay60Data158 = 1;
                    amSndPlay(0xF, NULL);
                }
                if (gOverlay60Data158 != 0) {
                    if (gOverlay60Data158 == 1) {
                        if (viDisplayingScreen0() == 0) {
                            projection = func_8002468C();
                            animseqStopPath(((u8 *)&gOverlay60Data2B8)[3]);
                            if (mode == 0) {
                                animseqStopPath(2);
                                animseqStartPath(3);
                                path = func_800508B4(3);
                                if (path != NULL) {
                                    path->flags |= 2;
                                }
                                gOverlay60Data0C8[0]->x = gOverlay60Data0E8[0];
                                gOverlay60Data0C8[0]->y = gOverlay60Data0E8[1];
                                gOverlay60Data0C8[1]->x = gOverlay60Data0E8[2];
                                gOverlay60Data0C8[1]->y = gOverlay60Data0E8[3];
                                gOverlay60Data0C8[2]->x = gOverlay60Data0E8[4];
                                gOverlay60Data0C8[2]->y = gOverlay60Data0E8[5];
                                gOverlay60Data0C8[3]->x = gOverlay60Data0E8[6];
                                gOverlay60Data0C8[3]->y = gOverlay60Data0E8[7];
                                (*projection)[0][0] *= 0.75f;
                            } else {
                                animseqStopPath(3);
                                animseqStartPath(2);
                                path = func_800508B4(2);
                                if (path != NULL) {
                                    path->flags |= 2;
                                }
                                gOverlay60Data0C8[0]->x = gOverlay60Data0D8[0];
                                gOverlay60Data0C8[0]->y = gOverlay60Data0D8[1];
                                gOverlay60Data0C8[1]->x = gOverlay60Data0D8[2];
                                gOverlay60Data0C8[1]->y = gOverlay60Data0D8[3];
                                gOverlay60Data0C8[2]->x = gOverlay60Data0D8[4];
                                gOverlay60Data0C8[2]->y = gOverlay60Data0D8[5];
                                gOverlay60Data0C8[3]->x = gOverlay60Data0D8[6];
                                gOverlay60Data0C8[3]->y = gOverlay60Data0D8[7];
                                (*projection)[0][0] /= 0.75f;
                            }
                            gOverlay60Data158 = 2;
                        }
                    } else {
                        if (mode == 0) {
                            mode = 1;
                        } else {
                            mode = 0;
                        }
                        func_80033FE0();
                        D_800D2FC0 = 1;
                        func_800336A8(mode);
                        func_80021504(60.0f, 1);
                        gOverlay60Data158 = 0;
                        func_overlay_082_F00004C0_18CF640(gOverlay60Data0A8);
                    }
                }
                if (mode == 1) {
                    fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x91, 0x91, O60_TEXT(0x1DC), 0xC);
                    value = frontGetWideAdjust();
                    if (D_800D31B4 & 8) {
                        value--;
                    }
                    if (D_800D31B4 & 4) {
                        value++;
                    }
                    frontSetWideAdjust(value);
                    fontColour(0xFF, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x6A, 0xB4, gOverlay60Data0C0, 4);
                    fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x97, 0xB4, O60_TEXT(0x34), 4);
                    fontColour(0xFF, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x6A, 0xC3, gOverlay60Data0C4, 4);
                    fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x97, 0xC3, O60_TEXT(0x38), 4);
                } else {
                    fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x91, 0x9B, O60_TEXT(0x1E0), 0xC);
                }
                func_8003A2C8(mode);
                break;
            case 7:
                oldDetail = gOverlay60Data14C;
                showArrows = 0;
                if (oldDetail == 0) {
                    func_overlay_082_F00004C0_18CF640(gOverlay60Data0A8);
                    if (gOverlay60Data154 == 0 &&
                        func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0) {
                        if (D_800D31B8 & 0x9000) {
                            gOverlay60Data14C = 1;
                            gOverlay60Data150 = 0;
                            func_overlay_082_F00004B0_18CF630(gOverlay60Data0A8);
                            amSndPlay(0xC, NULL);
                        } else if (D_800D31BE > 0 && gOverlay60Data2B0 > 0) {
                            gOverlay60Data2B0--;
                            if (gOverlay60Data2B0 == 2 && !(D_800D313B & 0x3F)) {
                                gOverlay60Data2B0 = 1;
                            }
                            amSndPlay(0xF, NULL);
                        } else if (D_800D31BE < 0) {
                            if (gOverlay60Data2B0 < 3) {
                                gOverlay60Data2B0++;
                                if (gOverlay60Data2B0 == 2 && !(D_800D313B & 0x3F)) {
                                    gOverlay60Data2B0 = 3;
                                }
                                amSndPlay(0xF, NULL);
                            }
                        }
                    }
                    gOverlay60Data154 = 0;
                    gOverlay60Data260[7] = O60_TEXT(0x278);
                    if (gOverlay60Data2B0 == 0) {
                        fontColour(0x80, 0xFF, 0x80, 0xFF, gOverlay60Data2A4);
                    } else {
                        fontColour(0, 0xD0, 0, 0xFF, gOverlay60Data2A4);
                    }
                    func_8004B0F8(&D_800D3140, 0x91, 0x8F, O60_TEXT(0x280), 0xC);
                    if (gOverlay60Data2B0 == 1) {
                        fontColour(0x80, 0xFF, 0x80, 0xFF, gOverlay60Data2A4);
                    } else {
                        fontColour(0, 0xD0, 0, 0xFF, gOverlay60Data2A4);
                    }
                    func_8004B0F8(&D_800D3140, 0x91, 0x9B, O60_TEXT(0x288), 0xC);
                    if (gOverlay60Data2B0 == 2) {
                        fontColour(0x80, 0xFF, 0x80, 0xFF, gOverlay60Data2A4);
                    } else if (D_800D313B != 0) {
                        fontColour(0, 0xD0, 0, 0xFF, gOverlay60Data2A4);
                    } else {
                        fontColour(0, 0x90, 0, 0xFF, gOverlay60Data2A4);
                    }
                    func_8004B0F8(&D_800D3140, 0x91, 0xA7, O60_TEXT(0x27C), 0xC);
                    if (gOverlay60Data2B0 == 3) {
                        fontColour(0x80, 0xFF, 0x80, 0xFF, gOverlay60Data2A4);
                    } else {
                        fontColour(0, 0xD0, 0, 0xFF, gOverlay60Data2A4);
                    }
                    func_8004B0F8(&D_800D3140, 0x91, 0xB3, O60_TEXT(0x28C), 0xC);
                } else {
                    switch (gOverlay60Data2B0) {
                    case 0:
                        gOverlay60Data260[7] = D_8007C0B8[D_8007C11C[gOverlay60Data150]];
                        if (((D_800D3130[0] & 0x1C0) >> 6) >= 3 &&
                            ((D_800D3130[1] & 0x1C0) >> 6) >= 3 &&
                            ((D_800D3130[2] & 0x1C0) >> 6) >= 3) {
                            if (((O60_SETTINGS_WORD >> 23) & 0xF) == 0xF) {
                                if (O60_SETTINGS_WORD & 0x40000) {
                                    limit = 0x15;
                                } else {
                                    limit = 0x14;
                                }
                            } else {
                                limit = 0x10;
                            }
                        } else {
                            limit = 0xC;
                        }
                        if (D_800D31B8 & 0x4000) {
                            gOverlay60Data14C = 0;
                            amSndPlay(0xD, NULL);
                        } else if (D_800D31BC < 0) {
                            gOverlay60Data150--;
                            if (gOverlay60Data150 < 0) {
                                gOverlay60Data150 = limit - 1;
                            }
                            amSndPlay(0xF, NULL);
                        } else if (D_800D31BC > 0) {
                            gOverlay60Data150++;
                            if (gOverlay60Data150 >= limit) {
                                gOverlay60Data150 = 0;
                            }
                            amSndPlay(0xF, NULL);
                        }
                        slots = func_800291C4();
                        record = slots[levelGetBlurEffect(D_8007C0E8[gOverlay60Data150])].records;
                        func_8004B0A4(2);
                        func_8004B0DC(0, 0, 0, 0);
                        fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                        func_8004B0F8(&D_800D3140, 0x91, 0x7A, O60_TEXT(0xD0), 4);
                        func_8004B0F8(&D_800D3140, 0x91, 0xBA, O60_TEXT(0xD4), 4);
                        if (func_overlay_068_F000146C_18C85CC(
                                D_8007C0E8[gOverlay60Data150]) != 0) {
                            func_8002FB34(&D_800D3140, &gOverlay60Data080,
                                81.0f, 115.0f, gOverlay60Data258, gOverlay60Data258, -2, 0);
                        }
                        fontColour(0xC0, 0xFF, 0, 0xFF, 0xFF);
                        recordY = gOverlay60Data1D0;
                        recordLabel = gOverlay60Data1A4;
                        do {
                            func_overlay_056_F00000B8_18A2E30(record->time,
                                &minutes, &seconds, &hundredths);
                            if (record->time == 0) {
                                icon = 0x4A;
                                sprintf(text, gOverlay60Data200);
                            } else {
                                initial0 = func_8003A700(record->initials[0]) & 0xFF;
                                initial1 = func_8003A700(record->initials[1]) & 0xFF;
                                sprintf(text, gOverlay60Data20C, initial0, initial1,
                                    func_8003A700(record->initials[2]),
                                    minutes, seconds, hundredths);
                                icon = record->character + 0x51;
                            }
                            gOverlay60Data060.texture = D_800D31C8[icon];
                            func_8004B0F8(&D_800D3140, gOverlay60Data1B4[0],
                                *recordY, *recordLabel, 0xC);
                            func_8002FB34(&D_800D3140, &gOverlay60Data060,
                                gOverlay60Data1B4[1], *recordY, 0.5f, 0.5f, -1, 0);
                            column = gOverlay60Data1B4;
                            letter = text;
                            do {
                                glyph[1] = '\0';
                                glyph[0] = *letter;
                                func_8004B0F8(&D_800D3140, column[2], *recordY,
                                    glyph, 0xC);
                                column++;
                                letter++;
                            } while (column != gOverlay60Data1B4 + 11);
                            recordLabel++;
                            record++;
                            recordY++;
                        } while (recordLabel != gOverlay60Data1A4 + 4);
                        showArrows = 1;
                        break;
                    case 1:
                        func_80034920(&D_800D3140);
                        camStandardPersp(&D_800D3140, &D_800D3144);
                        mode = gOverlay60Data150;
                        gOverlay60Data260[7] = D_8007C0B8[0x220 / 4 + mode];
                        if (((D_800D3130[0] & 0x1C0) >> 6) >= 3 &&
                            ((D_800D3130[1] & 0x1C0) >> 6) >= 3 &&
                            ((D_800D3130[2] & 0x1C0) >> 6) >= 3) {
                            if (((O60_SETTINGS_WORD >> 23) & 0xF) == 0xF) {
                                limit = 5;
                            } else {
                                limit = 4;
                            }
                        } else {
                            limit = 3;
                        }
                        if (D_800D31B8 & 0x4000) {
                            gOverlay60Data14C = 0;
                            amSndPlay(0xD, NULL);
                        } else if (D_800D31BC < 0) {
                            gOverlay60Data150 = mode - 1;
                            if (gOverlay60Data150 < 0) {
                                gOverlay60Data150 = limit - 1;
                            }
                            amSndPlay(0xF, NULL);
                        } else if (D_800D31BC > 0) {
                            gOverlay60Data150 = mode + 1;
                            if (gOverlay60Data150 >= limit) {
                                gOverlay60Data150 = 0;
                            }
                            amSndPlay(0xF, NULL);
                        }
                        gOverlay60Data130 = mode;
                        fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                        func_8004B0F8(&D_800D3140, 0x3C, 0x8C, O60_TEXT(0x234), 8);
                        fontColour(0xFF, 0, 0, 0xFF, gOverlay60Data2A4);
                        func_8004B0F8(&D_800D3140, 0x3C, 0x9B, O60_TEXT(0x238), 8);
                        fontColour(0xFF, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                        func_8004B0F8(&D_800D3140, 0x3C, 0xAA, O60_TEXT(0x23C), 8);
                        fontColour(0, 0xFF, 0xFF, 0xFF, gOverlay60Data2A4);
                        func_8004B0F8(&D_800D3140, 0x3C, 0xB9, O60_TEXT(0x240), 8);
                        func_800349A4(&D_800D3140, 0, 0, 0);
                        command = D_800D3140++;
                        command->w0 = 0x07020010;
                        command->w1 = (u32)D_800000B0;
                        command = D_800D3140++;
                        command->w0 = 0xFA000000;
                        command->w1 = 0x00FF0000 | (gOverlay60Data2A4 & 0xFF);
                        overlay60DrawBorder(0x39, 0x86, 0x82, 0x92);
                        overlay60DrawBorder(0x91, 0x74, 0xAF, 0x9D);
                        overlay60DrawLine(0x82, 0x8D, 0x91, 0x8D);
                        command = D_800D3140++;
                        command->w0 = 0xFA000000;
                        command->w1 = 0xFF000000 | (gOverlay60Data2A4 & 0xFF);
                        overlay60DrawBorder(0x39, 0x95, 0x82, 0xA1);
                        overlay60DrawBorder(0xB5, 0x74, 0xD4, 0x9D);
                        overlay60DrawLine(0xC7, 0x9E, 0xC7, 0xA2);
                        overlay60DrawLine(0x82, 0xA1, 0xC7, 0xA1);
                        command = D_800D3140++;
                        command->w0 = 0xFA000000;
                        command->w1 = 0xFFFF0000 | (gOverlay60Data2A4 & 0xFF);
                        overlay60DrawBorder(0x39, 0xA4, 0x82, 0xB0);
                        overlay60DrawBorder(0x91, 0xA6, 0xAF, 0xCF);
                        overlay60DrawLine(0x82, 0xAB, 0x91, 0xAB);
                        command = D_800D3140++;
                        command->w0 = 0xFA000000;
                        command->w1 = 0x00FFFF00 | (gOverlay60Data2A4 & 0xFF);
                        overlay60DrawBorder(0x39, 0xB3, 0x82, 0xBF);
                        overlay60DrawBorder(0xB5, 0xA6, 0xD4, 0xCF);
                        overlay60DrawLine(0xC5, 0xD0, 0xC5, 0xD3);
                        overlay60DrawLine(0x80, 0xD3, 0xC6, 0xD3);
                        overlay60DrawLine(0x80, 0xC0, 0x80, 0xD3);
                        func_80034920(&D_800D3140);
                        rcpClearZBuffer(&D_800D3140, 0x140, 0xF0, 0x8C, 0x64, 0xD7, 0xC8);
                        shift = gOverlay60Data11C;
                        mask = gOverlay60Data10C;
                        objects = gOverlay60Data0C8;
                        elapsed = ticks;
                        do {
                            object = *objects;
                            mode = gOverlay60Data150;
                            if (mode != object->unk3A) {
                                object->unk3A = mode;
                                func_8005AD64(*objects, 0, 0, 0);
                            }
                            (*objects)->unk8 = gOverlay60Data0F8[mode];
                            model = (MenuSpawnInner *)(*objects)->unk68[gOverlay60Data150];
                            model->mode = ticks;
                            rank = gOverlay60Data12B +
                                ((D_800D3130[gOverlay60Data150] & *mask) >> *shift);
                            func_80020D8C(model, 0, *rank * 256);
                            func_8005ABA8(*objects, 0.003f, elapsed);
                            if (rank != gOverlay60Data12B) {
                                (*objects)->alpha = gOverlay60Data2A4;
                                func_80009E78(&D_800D3140, &D_800D3144,
                                    &D_800D3148, *objects);
                            }
                            shift++;
                            objects++;
                            mask++;
                        } while (shift != gOverlay60Data11C + 4);
                        showArrows = 1;
                        break;
                    case 2:
                        gOverlay60Data260[7] = O60_TEXT(0x27C);
                        if (D_800D31B8 & 0x4000) {
                            gOverlay60Data14C = 0;
                            amSndPlay(0xD, NULL);
                        } else if (D_800D31BC < 0) {
                            value = gOverlay60Data150;
                            do {
                                value--;
                                if (value < 0) {
                                    value = 5;
                                }
                            } while (!(D_800D313B & (1 << value)));
                            if (value != gOverlay60Data150) {
                                gOverlay60Data150 = value;
                                if (gOverlay60Data174 != NULL) {
                                    func_800359D4(gOverlay60Data174);
                                    gOverlay60Data174 = NULL;
                                }
                                amSndPlay(0xF, NULL);
                            }
                        } else if (D_800D31BC > 0) {
                            value = gOverlay60Data150;
                            do {
                                value++;
                                if (value >= 6) {
                                    value = 0;
                                }
                            } while (!(D_800D313B & (1 << value)));
                            if (value != gOverlay60Data150) {
                                gOverlay60Data150 = value;
                                if (gOverlay60Data174 != NULL) {
                                    func_800359D4(gOverlay60Data174);
                                    gOverlay60Data174 = NULL;
                                }
                                amSndPlay(0xF, NULL);
                            }
                        }
                        if (oldDetail != 0 && gOverlay60Data174 == NULL) {
                            gOverlay60Data174 = func_800355A0(
                                gOverlay60Data168[gOverlay60Data150], 0);
                        }
                        if (gOverlay60Data174 != NULL) {
                            camStandardOrtho(&D_800D3140, &D_800D3144);
                            command = D_800D3140++;
                            command->w1 = 0;
                            command->w0 = 0xE7000000;
                            command = D_800D3140++;
                            command->w1 = 0xFFFFFFFF;
                            command->w0 = 0xFA000000;
                            command = D_800D3140++;
                            command->w1 = 0xFFFFFF00;
                            command->w0 = 0xFB000000;
                            func_80023F84(&D_800D3140, &D_800D3144, &D_800D3148,
                                gOverlay60Data178, gOverlay60Data174, 0, 0xFF);
                            if (gOverlay60Data14C == 0) {
                                func_800359D4(gOverlay60Data174);
                                gOverlay60Data174 = NULL;
                            }
                        }
                        if (D_800D313B & 1) { showArrows = 1; }
                        if (D_800D313B & 2) { showArrows++; }
                        if (D_800D313B & 4) { showArrows++; }
                        if (D_800D313B & 8) { showArrows++; }
                        if (D_800D313B & 0x10) { showArrows++; }
                        if (D_800D313B & 0x20) { showArrows++; }
                        if (showArrows < 2) { showArrows = 0; }
                        break;
                    case 3:
                        gOverlay60Data260[7] = O60_TEXT(0x28C);
                        if (D_800D31B8 & 0x9000) {
                            if (gOverlay60Data150 != 0) {
                                gOverlay60Data14C = 0;
                                gOverlay60Data160 = 1;
                                overlay60ReassignChoiceSlots();
                                D_8007BF1C = 0;
                                func_80029198();
                                amSndPlay(0xC, NULL);
                            } else {
                                gOverlay60Data14C = 0;
                                amSndPlay(0xD, NULL);
                            }
                        } else if (D_800D31B8 & 0x4000) {
                            gOverlay60Data14C = 0;
                            amSndPlay(0xD, NULL);
                        } else if (D_800D31BC < 0 && gOverlay60Data150 == 0) {
                            gOverlay60Data150 = 1;
                            amSndPlay(0xF, NULL);
                        } else if (D_800D31BC > 0 && gOverlay60Data150 != 0) {
                            gOverlay60Data150 = 0;
                            amSndPlay(0xF, NULL);
                        }
                        func_8004B0F8(&D_800D3140, 0x91, 0x8F, O60_TEXT(0x290), 0xC);
                        func_8004B0F8(&D_800D3140, 0x91, 0x99, O60_TEXT(0x294), 0xC);
                        func_8004B0F8(&D_800D3140, 0x91, 0xAD, O60_TEXT(0x54), 0xC);
                        if (gOverlay60Data150 != 0) {
                            fontColour(0x80, 0xFF, 0x80, 0xFF, gOverlay60Data2A4);
                        } else {
                            fontColour(0, 0xD0, 0, 0xFF, gOverlay60Data2A4);
                        }
                        func_8004B0F8(&D_800D3140, 0x71, 0xBC, O60_TEXT(0x58), 0xC);
                        if (gOverlay60Data150 == 0) {
                            fontColour(0x80, 0xFF, 0x80, 0xFF, gOverlay60Data2A4);
                        } else {
                            fontColour(0, 0xD0, 0, 0xFF, gOverlay60Data2A4);
                        }
                        func_8004B0F8(&D_800D3140, 0xB1, 0xBC, O60_TEXT(0x5C), 0xC);
                        break;
                    }
                }
                if (showArrows != 0) {
                    if (frontGetScreenMode() == 1) {
                        left = 0x2E;
                        right = 0xE4;
                    } else {
                        left = 0x24;
                        right = 0xF4;
                    }
                    func_8002FB34(&D_800D3140, &gOverlay60Data040, left,
                        155.0f, 1.0f, 1.0f, -2, 3);
                    func_8002FB34(&D_800D3140, &gOverlay60Data040, right,
                        155.0f, 1.0f, 1.0f, -2, 0x1003);
                    func_800367A4(gOverlay60Data040.texture, &gOverlay60Data0A0,
                        0xC, &gOverlay60Data0A4, ticks);
                    gOverlay60Data040.packedOffset = (s32)(gOverlay60Data0A4 * 65536.0f);
                }
                break;
            case 8:
                if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0) {
                    if (D_800D31BE < 0) {
                        if (gOverlay60Data2B0 == 0) {
                            gOverlay60Data2B0 = 1;
                            amSndPlay(0xF, NULL);
                        } else {
                            amSndPlay(0xE, NULL);
                        }
                    } else if (D_800D31BE > 0) {
                        if (gOverlay60Data2B0 == 1) {
                            gOverlay60Data2B0 = 0;
                            amSndPlay(0xF, NULL);
                        } else {
                            amSndPlay(0xE, NULL);
                        }
                    }
                }
                if (gOverlay60Data2B0 == 0) {
                    fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                    if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0 &&
                        D_800D31BC != 0) {
                        O60_SETTINGS_BYTE(1) = (O60_SETTINGS_BYTE(1) & ~0x10) |
                            ((((O60_SETTINGS_WORD >> 20) & 1) ^ 1) << 4);
                        amSndPlay(0xF, NULL);
                    }
                } else {
                    fontColour(0, 0xBE, 0, 0xFF, gOverlay60Data2A4);
                }
                sprintf(text, gOverlay60Data220, O60_TEXT(0x1D4),
                    gOverlay60Data298[(O60_SETTINGS_WORD >> 20) & 1]);
                func_8004B0F8(&D_800D3140, 0x91, 0x96, text, 0xC);
                if (gOverlay60Data2B0 == 1) {
                    fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                    if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0 &&
                        D_800D31BC != 0) {
                        O60_SETTINGS_BYTE(1) = (O60_SETTINGS_BYTE(1) & ~8) |
                            ((((O60_SETTINGS_WORD >> 19) & 1) ^ 1) << 3);
                        amSndPlay(0xF, NULL);
                    }
                } else {
                    fontColour(0, 0xBE, 0, 0xFF, gOverlay60Data2A4);
                }
                sprintf(text, gOverlay60Data228, O60_TEXT(0x1D8),
                    gOverlay60Data298[(O60_SETTINGS_WORD >> 19) & 1]);
                func_8004B0F8(&D_800D3140, 0x91, 0xAA, text, 0xC);
                break;
            case 9:
                if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0) {
                    if (D_800D31BE < 0) {
                        gOverlay60Data2B0++;
                        if (gOverlay60Data2B0 >= 0xC) {
                            gOverlay60Data2B0 = 0xB;
                            amSndPlay(0xE, NULL);
                        } else {
                            amSndPlay(0xF, NULL);
                        }
                        if (gOverlay60Data2B4 < gOverlay60Data2B0 - 5) {
                            gOverlay60Data2B4++;
                        }
                    } else if (D_800D31BE > 0) {
                        gOverlay60Data2B0--;
                        if (gOverlay60Data2B0 < 0) {
                            gOverlay60Data2B0 = 0;
                            amSndPlay(0xE, NULL);
                        } else {
                            amSndPlay(0xF, NULL);
                        }
                        if (gOverlay60Data2B0 < gOverlay60Data2B4) {
                            gOverlay60Data2B4 = gOverlay60Data2B0;
                        }
                    }
                    if (D_800D31BC != 0) {
                        D_8007BF14 ^= 1 << (gOverlay60Data2B0 + 2);
                        amSndPlay(0xF, NULL);
                    }
                }
                y = 0x8C;
                for (i = gOverlay60Data2B4; i < gOverlay60Data2B4 + 6; i++) {
                    if (i == gOverlay60Data2B0) {
                        fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                    } else {
                        fontColour(0, 0xBE, 0, 0xFF, gOverlay60Data2A4);
                    }
                    func_8004B0F8(&D_800D3140, 0x4D, y,
                        D_8007C0B8[0x1F0 / 4 + i], 8);
                    func_8004B0F8(&D_800D3140, 0xC1, y,
                        gOverlay60Data298[(D_8007BF14 >> (i + 2)) & 1], 8);
                    y += 0xC;
                }
                break;
            }
        }
        if ((D_800D31B8 & 0x4000) && gOverlay60Data2AC == -1 &&
            gOverlay60Data164 == 0) {
            gOverlay60Data164 = 1;
            if (D_8007BF1C & 0x40) {
                D_8007BF1C &= ~0x40;
                func_overlay_048_F0000000_1895408();
            } else {
                mainChangeLevel(0xC, 0, 0, 0xC, 1, 0);
            }
            amSndPlay(0xD, NULL);
        }
        fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A0);
        func_8004B0F8(&D_800D3140, 0x91, (s16)gOverlay60Data2A8,
            gOverlay60Data260[panel], 0xC);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o060/overlay60Prefix/func_overlay_060_F0000334_18BA10C.s")
#endif
