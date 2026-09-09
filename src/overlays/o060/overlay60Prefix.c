#include "PR/ultratypes.h"
#include "game/menu.h"
#include "game/font.h"
#include "game/charControl.h"
#include "game/anim.h"
#include "game/math.h"
#include "game/gameVi.h"
#include "n_audio/mbi.h"
extern int sprintf(char *buffer, const char *format, ...);

/* Tier B: the call sites and runtime relocation records identify these
 * resident interfaces. The local views below use Mickey's menu, save-record,
 * and RCP field layouts; unknown fields retain their offsets. */
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
typedef struct Overlay60Point {
    s16 x;
    s16 y;
} Overlay60Point;

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
extern void func_8004B0F8(Gfx **commands, s32 x, s32 y, char *text, s32 flags);
extern void func_8002F618(Gfx **commands, RcpTextureNode *texture,
                        s32 x, s32 y, s32 red, s32 green, s32 blue, s32 alpha);
extern void func_8002FB34(Gfx **commands, RcpTextureNode *texture,
                        f32 x, f32 y, f32 scaleX, f32 scaleY, s32 alpha, s32 flags);
extern void func_800367A4(void *texture, s32 *state, s32 speed, f32 *frame, s32 ticks);
extern void func_8003A520(s32 split);
extern s32 func_8003A700(u8 initial);
extern void func_800336A8(s32 screenMode);
extern void func_80033FE0(void);
extern void func_80021504(f32 fov, s32 force);
extern MtxF *func_8002468C(void);
extern void func_80029198(void);
extern SavesSlot *func_800291C4(void);
extern s32 levelGetBlurEffect(s32 level);
extern void func_80034920(Gfx **commands);
extern void func_800349A4(Gfx **commands, s32 arg1, s32 arg2, s32 arg3);
extern void camStandardPersp(Gfx **commands, Mtx **matrices);
extern void camStandardOrtho(Gfx **commands, Mtx **matrices);
extern void rcpClearZBuffer(Gfx **commands, s32 width, s32 height,
                           s32 left, s32 top, s32 right, s32 bottom);
extern void func_8005AD64(ControlActor *object, s32 frame, s32 arg2, f32 value);
extern s32 func_8005ABA8(ControlActor *object, f32 scale, f32 ticks);
extern void func_80020D8C(MenuSpawnInner *model, s32 index, s32 frame);
extern void func_80009E78(Gfx **commands, Mtx **matrices,
                        void **vertices, ControlActor *object);
extern void *func_800355A0(s32 asset, s32 flags);
extern void func_800359D4(void *sprite);
extern void func_80023F84(Gfx **commands, Mtx **matrices,
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
/* Tier B/D: the shared menu view names language and stereo mode. The
 * remaining fields below are identified by their bit positions: their reads
 * and byte-preserving writes belong to the same resident settings word. */
typedef struct Overlay60SettingsBits {
    u32 upper : 5;
    u32 field26_23 : 4;
    u32 stereoMode : 2;
    u32 field20 : 1;
    u32 field19 : 1;
    u32 field18 : 1;
    u32 gap17_16 : 2;
    u32 language : 6;
    u32 field9 : 1;
    u32 field8 : 1;
    u32 lower : 8;
} Overlay60SettingsBits;
extern Overlay60SettingsBits D_800D3128;
extern u16 D_800D312C;
extern u16 D_800D312E;
extern u16 D_800D3130[];
extern u8 D_800D313B;
extern u16 D_800D313C;
extern Gfx *D_800D3140;
extern Mtx *D_800D3144;
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
extern Overlay60Point gOverlay60Data0D8[];
extern Overlay60Point gOverlay60Data0E8[];
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
/* Tier B: this relocation denotes the local list at data +0xB0 with its
 * physical-address bias. It is a link-bound address alias, not a resident
 * global. Canonical relocation binding remains part of promotion proof. */
extern Gfx gOverlay60PhysicalList0B0[];

#define O60_TEXT(offset) D_8007C0B8[(offset) / 4]

/* Tier B/D: menu update and drawing reconstructed from Mickey's call graph,
 * field accesses, and ten-case dispatch. NON_MATCHING retains the ROM body. */
#ifdef NON_MATCHING
/* Declaration order below is load-bearing, not stylistic. IDO reserves a home
 * for every declared local in declaration order from the top of the local
 * block down, so the list's order alone fixes each array's frame offset. With
 * this order the arrays land on the target's own homes (enabled at
 * frame_top-0x5C, text at frame_top-0xDC, glyph at frame_top-0xE4) and the
 * whole stack census agrees with the target apart from the compiler's own
 * spill slots. Moving `icon`, `initial0` or `initial1` back above `enabled`
 * shifts all three by 12 bytes and loses that agreement. */
void func_overlay_060_F0000334_18BA10C(s32 ticks) {
    s32 panel;
    s32 first;
    s32 end;
    ControlActor **objects;
    ControlActor *menuObject;
    ControlActor *object;
    s32 i;
    s32 count;
    s32 value;
    s32 stereoMode;
    s32 screenMode;
    s32 previewMode;
    s32 showArrows;
    s32 limit;
    s32 minutes;
    s32 seconds;
    s32 hundredths;
    s32 row;
    s32 y;
    u8 enabled[16];
    char text[128];
    s32 icon;
    char glyph[2];
    s32 initial0;
    s32 initial1;
    s8 *rank;
    SavesSlot *slots;
    SavesPackedEntry *record;
    MtxF *projection;
    AnimPath *path;
    MenuSpawnInner *model;
    s32 left;
    s32 right;

    if (gOverlay60Data0A8 == NULL) {
        objects = func_8000572C(&first, &end);
        while (first < end) {
            menuObject = objects[first++];
            if (menuObject->kind == 0x54) {
                gOverlay60Data0A8 = menuObject;
                first = end;
            }
        }
    }
    if (gOverlay60Data160 != 0) {
        frontSetLanguage(D_800D3128.language);
        frontSetStereoMode(D_800D3128.stereoMode);
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
                        D_800D3128.field9 ^= 1;
                        amSndPlay(0xF, NULL);
                    }
                } else {
                    fontColour(0, 0xBE, 0, 0xFF, gOverlay60Data2A4);
                }
                sprintf(text, gOverlay60Data1F0, O60_TEXT(0x254),
                        gOverlay60Data298[D_800D3128.field9]);
                func_8004B0F8(&D_800D3140, 0x91, 0x96, text, 0xC);
                if (gOverlay60Data2B0 == 1) {
                    fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                    if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0 &&
                        D_800D31BC != 0) {
                        D_800D3128.field8 ^= 1;
                        amSndPlay(0xF, NULL);
                    }
                } else {
                    fontColour(0, 0xBE, 0, 0xFF, gOverlay60Data2A4);
                }
                sprintf(text, gOverlay60Data1F8, O60_TEXT(0x258),
                        gOverlay60Data298[D_800D3128.field8]);
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
                stereoMode = frontGetStereoMode();
                if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0) {
                    if (D_800D31BC < 0) {
                        stereoMode--;
                        if (stereoMode < 0) {
                            stereoMode = 3;
                        }
                        amSndPlay(0xF, NULL);
                    } else if (D_800D31BC > 0) {
                        stereoMode++;
                        if (stereoMode >= 4) {
                            stereoMode = 0;
                        }
                        amSndPlay(0xF, NULL);
                    }
                }
                frontSetStereoMode(stereoMode);
                if (stereoMode == 3) {
                    func_8002F618(&D_800D3140, &gOverlay60Data020,
                                  0x68, 0x8C, 0, 0xFF, 0, gOverlay60Data2A4);
                    fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x91, 0xB9, O60_TEXT(0x298), 0xC);
                    func_8004B0F8(&D_800D3140, 0x91, 0xC3, O60_TEXT(0x29C), 0xC);
                    func_8004B0F8(&D_800D3140, 0x91, 0xCD, O60_TEXT(0x2A0), 0xC);
                } else {
                    fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                    func_8004B0F8(&D_800D3140, 0x91, 0x9B, gOverlay60Data288[stereoMode], 0xC);
                }
                break;
            case 6:
                screenMode = frontGetScreenMode();
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
                            if (screenMode == 0) {
                                animseqStopPath(2);
                                animseqStartPath(3);
                                path = func_800508B4(3);
                                if (path != NULL) {
                                    path->flags |= 2;
                                }
                                for (i = 0; i < 4; i++) {
                                    gOverlay60Data0C8[i]->x = gOverlay60Data0E8[i].x;
                                    gOverlay60Data0C8[i]->y = gOverlay60Data0E8[i].y;
                                }
                                (*projection)[0][0] *= 0.75f;
                            } else {
                                animseqStopPath(3);
                                animseqStartPath(2);
                                path = func_800508B4(2);
                                if (path != NULL) {
                                    path->flags |= 2;
                                }
                                for (i = 0; i < 4; i++) {
                                    gOverlay60Data0C8[i]->x = gOverlay60Data0D8[i].x;
                                    gOverlay60Data0C8[i]->y = gOverlay60Data0D8[i].y;
                                }
                                (*projection)[0][0] /= 0.75f;
                            }
                            gOverlay60Data158 = 2;
                        }
                    } else {
                        if (screenMode == 0) {
                            screenMode = 1;
                        } else {
                            screenMode = 0;
                        }
                        func_80033FE0();
                        D_800D2FC0 = 1;
                        func_800336A8(screenMode);
                        func_80021504(60.0f, 1);
                        gOverlay60Data158 = 0;
                        func_overlay_082_F00004C0_18CF640(gOverlay60Data0A8);
                    }
                }
                if (screenMode == 1) {
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
                func_8003A2C8(screenMode);
                break;
            case 7:
                showArrows = 0;
                if (gOverlay60Data14C == 0) {
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
                            if (D_800D3128.field26_23 == 0xF) {
                                if (D_800D3128.field18) {
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
                        slots += levelGetBlurEffect(D_8007C0E8[gOverlay60Data150]);
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
                        for (row = 0; row < 4; row++) {
                            record = &slots->records[row];
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
                                gOverlay60Data1D0[row], gOverlay60Data1A4[row], 0xC);
                            func_8002FB34(&D_800D3140, &gOverlay60Data060,
                                gOverlay60Data1B4[1], gOverlay60Data1D0[row], 0.5f, 0.5f, -1, 0);
                            for (i = 0; i < 11; i++) {
                                glyph[1] = '\0';
                                glyph[0] = text[i];
                                func_8004B0F8(&D_800D3140, gOverlay60Data1B4[i + 2],
                                    gOverlay60Data1D0[row], glyph, 0xC);
                            }
                        }
                        showArrows = 1;
                        break;
                    case 1:
                        func_80034920(&D_800D3140);
                        camStandardPersp(&D_800D3140, &D_800D3144);
                        previewMode = gOverlay60Data150;
                        gOverlay60Data260[7] = D_8007C0B8[0x220 / 4 + previewMode];
                        if (((D_800D3130[0] & 0x1C0) >> 6) >= 3 &&
                            ((D_800D3130[1] & 0x1C0) >> 6) >= 3 &&
                            ((D_800D3130[2] & 0x1C0) >> 6) >= 3) {
                            if (D_800D3128.field26_23 == 0xF) {
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
                            gOverlay60Data150 = previewMode - 1;
                            if (gOverlay60Data150 < 0) {
                                gOverlay60Data150 = limit - 1;
                            }
                            amSndPlay(0xF, NULL);
                        } else if (D_800D31BC > 0) {
                            gOverlay60Data150 = previewMode + 1;
                            if (gOverlay60Data150 >= limit) {
                                gOverlay60Data150 = 0;
                            }
                            amSndPlay(0xF, NULL);
                        }
                        gOverlay60Data130 = previewMode;
                        fontColour(0, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                        func_8004B0F8(&D_800D3140, 0x3C, 0x8C, O60_TEXT(0x234), 8);
                        fontColour(0xFF, 0, 0, 0xFF, gOverlay60Data2A4);
                        func_8004B0F8(&D_800D3140, 0x3C, 0x9B, O60_TEXT(0x238), 8);
                        fontColour(0xFF, 0xFF, 0, 0xFF, gOverlay60Data2A4);
                        func_8004B0F8(&D_800D3140, 0x3C, 0xAA, O60_TEXT(0x23C), 8);
                        fontColour(0, 0xFF, 0xFF, 0xFF, gOverlay60Data2A4);
                        func_8004B0F8(&D_800D3140, 0x3C, 0xB9, O60_TEXT(0x240), 8);
                        func_800349A4(&D_800D3140, 0, 0, 0);
                        gDma1p(D_800D3140++, 7, gOverlay60PhysicalList0B0, 0x10, 2);
                        gDPSetPrimColor(D_800D3140++, 0, 0, 0, 255, 0,
                                        gOverlay60Data2A4);
                        overlay60DrawBorder(0x39, 0x86, 0x82, 0x92);
                        overlay60DrawBorder(0x91, 0x74, 0xAF, 0x9D);
                        overlay60DrawLine(0x82, 0x8D, 0x91, 0x8D);
                        gDPSetPrimColor(D_800D3140++, 0, 0, 255, 0, 0,
                                        gOverlay60Data2A4);
                        overlay60DrawBorder(0x39, 0x95, 0x82, 0xA1);
                        overlay60DrawBorder(0xB5, 0x74, 0xD4, 0x9D);
                        overlay60DrawLine(0xC7, 0x9E, 0xC7, 0xA2);
                        overlay60DrawLine(0x82, 0xA1, 0xC7, 0xA1);
                        gDPSetPrimColor(D_800D3140++, 0, 0, 255, 255, 0,
                                        gOverlay60Data2A4);
                        overlay60DrawBorder(0x39, 0xA4, 0x82, 0xB0);
                        overlay60DrawBorder(0x91, 0xA6, 0xAF, 0xCF);
                        overlay60DrawLine(0x82, 0xAB, 0x91, 0xAB);
                        gDPSetPrimColor(D_800D3140++, 0, 0, 0, 255, 255,
                                        gOverlay60Data2A4);
                        overlay60DrawBorder(0x39, 0xB3, 0x82, 0xBF);
                        overlay60DrawBorder(0xB5, 0xA6, 0xD4, 0xCF);
                        overlay60DrawLine(0xC5, 0xD0, 0xC5, 0xD3);
                        overlay60DrawLine(0x80, 0xD3, 0xC6, 0xD3);
                        overlay60DrawLine(0x80, 0xC0, 0x80, 0xD3);
                        func_80034920(&D_800D3140);
                        rcpClearZBuffer(&D_800D3140, 0x140, 0xF0, 0x8C, 0x64, 0xD7, 0xC8);
                        for (i = 0; i < 4; i++) {
                            object = gOverlay60Data0C8[i];
                            previewMode = gOverlay60Data150;
                            if (previewMode != object->unk3A) {
                                object->unk3A = previewMode;
                                func_8005AD64(gOverlay60Data0C8[i], 0, 0, 0.0f);
                            }
                            previewMode = gOverlay60Data150;
                            object = gOverlay60Data0C8[i];
                            object->unk8 = gOverlay60Data0F8[previewMode];
                            model = (MenuSpawnInner *)(gOverlay60Data0C8[i])->unk68[gOverlay60Data150];
                            model->mode = ticks;
                            rank = gOverlay60Data12B + 1 +
                                ((u32)(D_800D3130[gOverlay60Data150] & gOverlay60Data10C[i]) >> gOverlay60Data11C[i]);
                            func_80020D8C(model, 0, rank[-1] * 256);
                            func_8005ABA8(gOverlay60Data0C8[i], 0.003f, ticks);
                            if (rank != gOverlay60Data12B + 1) {
                                (gOverlay60Data0C8[i])->alpha = gOverlay60Data2A4;
                                func_80009E78(&D_800D3140, &D_800D3144,
                                    &D_800D3148, gOverlay60Data0C8[i]);
                            }
                        }
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
                        if (gOverlay60Data14C != 0 && gOverlay60Data174 == NULL) {
                            gOverlay60Data174 = func_800355A0(
                                gOverlay60Data168[gOverlay60Data150], 0);
                        }
                        if (gOverlay60Data174 != NULL) {
                            camStandardOrtho(&D_800D3140, &D_800D3144);
                            gDPPipeSync(D_800D3140++);
                            gDPSetPrimColor(D_800D3140++, 0, 0, 255, 255, 255, 255);
                            gDPSetEnvColor(D_800D3140++, 255, 255, 255, 0);
                            func_80023F84(&D_800D3140, &D_800D3144, &D_800D3148,
                                gOverlay60Data178, gOverlay60Data174, 0, 0xFF);
                            if (gOverlay60Data14C == 0) {
                                func_800359D4(gOverlay60Data174);
                                gOverlay60Data174 = NULL;
                            }
                        }
                        for (i = 0; i < 6; i++) {
                            if (D_800D313B & (1 << i)) {
                                showArrows++;
                            }
                        }
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
                        D_800D3128.field20 ^= 1;
                        amSndPlay(0xF, NULL);
                    }
                } else {
                    fontColour(0, 0xBE, 0, 0xFF, gOverlay60Data2A4);
                }
                sprintf(text, gOverlay60Data220, O60_TEXT(0x1D4),
                    gOverlay60Data298[D_800D3128.field20]);
                func_8004B0F8(&D_800D3140, 0x91, 0x96, text, 0xC);
                if (gOverlay60Data2B0 == 1) {
                    fontColour(0x64, 0xFF, 0x64, 0xFF, gOverlay60Data2A4);
                    if (func_overlay_082_F00004A4_18CF624(gOverlay60Data0A8) != 0 &&
                        D_800D31BC != 0) {
                        D_800D3128.field19 ^= 1;
                        amSndPlay(0xF, NULL);
                    }
                } else {
                    fontColour(0, 0xBE, 0, 0xFF, gOverlay60Data2A4);
                }
                sprintf(text, gOverlay60Data228, O60_TEXT(0x1D8),
                    gOverlay60Data298[D_800D3128.field19]);
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
        func_8004B0F8(&D_800D3140, 0x91, gOverlay60Data2A8,
            gOverlay60Data260[panel], 0xC);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o060/overlay60Prefix/func_overlay_060_F0000334_18BA10C.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_060_F0000334_18BA10C:start
 * symbol: func_overlay_060_F0000334_18BA10C
 * score: 2510 differing words
 * frame: 0x198
 * relocations: 864
 * first-mismatch: +0x4
 * summary: NON_MATCHING: frame and every declared-local home now byte-exact (enabled 0x13C, text 0xBC, glyph 0xB4); mnemonic census within 45 of 2789 and the -12 is 45 insertions against 48 deletions, not one hole; the whole remaining residual is one uopt address-CSE that keeps &gOverlay60Data0A8 in a saved register for the entire target and rematerializes it 24 times here.
 * PLATEAU-HANDOFF:func_overlay_060_F0000334_18BA10C:end
 */
