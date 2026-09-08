#include "PR/ultratypes.h"

typedef struct O50Glyph {
    s32 texture;
    s32 alternate;
    s32 glyph;
    s16 x;
    s16 y;
} O50Glyph;

/* Tier D: field widths and offsets from this caller's loads and stores. */
typedef struct O50Racer {
    u8 pad000[4];
    f32 speed;
    u8 pad008[0x18A];
    u8 value192;
    u8 pad193[7];
    u8 item;
    u8 itemCount;
    s32 itemState;
    u8 pad1A0[8];
    u16 flags;
    u8 pad1AA[0x1D9];
    s8 laps;
    u8 pad384;
    u8 value385;
    u8 pad386[2];
    u8 value388;
    u8 pad389[0x31];
    s16 value3BA;
    u8 pad3BC[0x44];
    s32 raceTime;
    s32 lapTimes[20];
    s16 timeDifference;
    s16 differenceTimer;
    u8 pad458[4];
    u8 finished;
} O50Racer;

typedef struct O50Object {
    u8 pad00[0x64];
    O50Racer *racer;
} O50Object;

typedef struct O50Level {
    u8 pad00[0x86];
    s8 laps;
} O50Level;

/* Tier B: callee identities decoded from overlay 50 runtime exports.
 * Signatures follow Mickey callees and the O32 argument surface. */
u8 *func_80028F54(void); /* 0:+0x28B04 */
void camStandardOrtho(void **, void **); /* 0:+0x22600 */
void overlay45ReleaseDescriptor(void *); /* 45:+0x270 */
void overlay45SetMode(void *, s32); /* 45:+0x1BE0 */
void overlay56SplitTime(s32, s32 *, s32 *, s32 *); /* 56:+0xB8 */
O50Level *levelGetLevel(void); /* 0:+0x2634C */
s32 func_800290A0(void); /* 0:+0x28C50 */
s32 func_8003A7D0(void *); /* 0:+0x3A380 */
void func_8002F618(void **, void *, s32, s32, s32, s32, s32, s32); /* 0:+0x2F1C8 */
void func_80034920(void **); /* 0:+0x344D0 */
void func_80039E34(s32); /* 0:+0x399E4 */
void freeFrontEndItem(s32); /* 0:+0x3964C */
void loadFrontEndItem(s32); /* 0:+0x39794 */
s32 overlay59Interpolate(s32, s32, s32, s32, s32, s32 *, s32 *, s32); /* 59:+0x784 */
void overlay59BuildList(s32, void *); /* 59:+0x84C */
void overlay59DrawFrame(void **, s32, s32, s32); /* 59:+0x8EC */
s32 func_80036544(void *, s32 *, s32, f32 *, s32); /* 0:+0x360F4 */
void amSndPlay(u16, void **); /* 0:+0xB44 */
u32 joyGetPressed(s32); /* 0:+0x250FC */
void func_8004A4B0(s32, s32, s32, s32, s32); /* 0:+0x4A060 */
void func_8004B0A4(s32); /* 0:+0x4AC54 */
void func_8004B0DC(s32, s32, s32, s32); /* 0:+0x4AC8C */
void fontColour(s32, s32, s32, s32, s32); /* 0:+0x4AC68 */
void func_8004B0F8(void **, s32, s32, char *, s32); /* 0:+0x4ACA8 */
s32 mainGetMode(void); /* 0:+0x288C8 */
void func_800016EC(s32); /* 0:+0x129C */
void func_8003A590(void); /* 0:+0x3A140 */
void func_80037414(s32, f32, f32, s32, s32, s32, s32); /* 0:+0x36FC4 */
void mainChangeLevel(s32, s32, s32, s32, s32, s32); /* 0:+0x27F24 */
void func_800005CC(f32, u8); /* 0:+0x17C */

void overlay50SubmitTimeGlyphs(s32, s32, s32, s32); /* extern */
extern O50Glyph D_12C[10];
extern O50Glyph D_1CC[5];
extern s16 D_21C;
extern O50Glyph D_230[8];
extern O50Glyph D_2C0[2];
extern O50Glyph D_2E0[2];
extern O50Glyph D_300[2];
extern s32 D_328;
extern s8 D_32C;
extern s32 D_334;
extern s32 D_33C;

extern O50Glyph D_6C[4];
extern f32 D_A4;
extern f32 D_A8;
extern O50Glyph D_AC[3];
extern s32 D_B0[];
extern s32 D_BC;
extern s32 D_C0;
extern s16 D_C8;
extern s16 D_CA;
extern O50Glyph D_DC[2];
extern O50Glyph D_FC[3];

typedef struct O50MenuObject {
    s16 value00;
    s16 value02;
    s16 angle;
    s16 index;
    f32 value08;
    f32 x;
    f32 y;
    f32 value14;
    f32 frame;
    u8 pad1C[4];
} O50MenuObject;

/* Tier B: runtime export identities, kept distinct from stored addends. */
extern void *D_800D3140;
extern void *D_800D3144;
extern s32 D_800C947C;
extern s16 D_8007C180[];
extern s32 D_800D31C8[];
extern O50MenuObject D_800D3550[];
extern s32 D_8007C1B0;
extern u16 D_8007BF1C;
extern s32 D_8007C0BC;
extern s32 o50Overlay1StateReloc; /* overlay 1 +0x83E0 */

/* Tier B: local data, read-only data, and BSS ownership from runtime records. */
extern s8 o50Data320[];
extern s8 o50Data330;
extern s32 o50BssC4;
extern O50Glyph o50LapGlyphs[10];
extern char o50SpeedText[];
extern f32 o50ReverseScale;
extern f32 o50ForwardScale;

/* NON_MATCHING: runtime call/global identities and record layouts are recovered.
 * The configured candidate still has a structural and allocation residual;
 * the assembly fallback remains the only ROM-exact implementation. */
#ifdef NON_MATCHING
void func_overlay_050_F0000334_1896CA4(O50Object *arg0, s32 arg1) {
    s32 sp10C;
    s32 sp108;
    O50Glyph messageGlyphs[5];
    O50Glyph itemGlyphs[2];
    O50Level *level;
    O50Racer *racer;
    u8 *modeFlag;
    s32 timeMagnitude;
    s32 minutes;
    s32 seconds;
    s32 centiseconds;
    s32 hudY;
    s32 hudX;
    s32 loadedItem;
    O50Glyph *glyphCursor;
    O50Glyph *templateCursor;
    f32 stepY;
    f32 deltaX;
    s16 itemId;
    s16 remaining;
    s32 glyphTexture;
    s32 t6;
    s32 t7;
    s32 t8;
    s32 t9;
    s32 remainder;
    s32 glyphAlternate;
    s32 i;
    s32 lapIndex;
    s32 opacity;
    s32 var_v0_3;
    s32 targetAngle;
    s8 lapCount;

    modeFlag = (u8 *)func_80028F54();
    if (arg0 != NULL) {
        racer = arg0->racer;
        camStandardOrtho(&D_800D3140, &D_800D3144);
        if (o50BssC4 != 0) {
            D_C8 += arg1;
            if (D_C8 >= 0x3D) {
                if (D_C8 >= 0xF1) {
                    D_CA -= arg1 * 4;
                    if (D_CA < 0) {
                        overlay45ReleaseDescriptor((void *) o50BssC4);
                        o50BssC4 = 0;
                    } else {
                        overlay45SetMode((void *) o50BssC4, D_CA);
                    }
                } else {
                    D_CA += arg1 * 4;
                    if (D_CA >= 0x100) {
                        D_CA = 0xFF;
                    }
                    overlay45SetMode((void *) o50BssC4, D_CA);
                }
            }
        }
        if (D_800C947C == 0) {
            i = 0;
            if (arg1 > 0) {
                remainder = arg1 & 3;
                if (remainder != 0) {
                    i = 1;
                    deltaX = 0.0f - D_A8;
                    stepY = (0.0f - D_A4) * 0.125f;
                    if (remainder != 1) {
                        do {
                            i += 1;
                            D_A4 += stepY;
                            D_A8 += deltaX * 0.125f;
                            stepY = (0.0f - D_A4) * 0.125f;
                            deltaX = 0.0f - D_A8;
                        } while (remainder != i);
                    }
                    D_A4 += stepY;
                    D_A8 += deltaX * 0.125f;
                    if (i != arg1) {
                        goto block_16;
                    }
                } else {
block_16:
                    i = i + 4;
                    deltaX = 0.0f - D_A8;
                    stepY = (0.0f - D_A4) * 0.125f;
                    if (i != arg1) {
                        do {
                            i += 4;
                            D_A4 += stepY;
                            D_A8 += deltaX * 0.125f;
                            D_A4 += (0.0f - D_A4) * 0.125f;
                            D_A8 += (0.0f - D_A8) * 0.125f;
                            D_A4 += (0.0f - D_A4) * 0.125f;
                            D_A8 += (0.0f - D_A8) * 0.125f;
                            D_A4 += (0.0f - D_A4) * 0.125f;
                            D_A8 += (0.0f - D_A8) * 0.125f;
                            stepY = (0.0f - D_A4) * 0.125f;
                            deltaX = 0.0f - D_A8;
                        } while (i != arg1);
                    }
                    D_A4 += stepY;
                    D_A8 += deltaX * 0.125f;
                    D_A4 += (0.0f - D_A4) * 0.125f;
                    D_A8 += (0.0f - D_A8) * 0.125f;
                    D_A4 += (0.0f - D_A4) * 0.125f;
                    D_A8 += (0.0f - D_A8) * 0.125f;
                    D_A4 += (0.0f - D_A4) * 0.125f;
                    D_A8 += (0.0f - D_A8) * 0.125f;
                }
            }
        }
        hudY = (s32) D_A4;
        hudX = (s32) D_A8;
        D_FC[0].glyph = (s32) (((s32) racer->value192 / 10) << 0x10);
        D_FC[1].glyph = (s32) (((s32) racer->value192 % 10) << 0x10);
        if (racer->value3BA != 0xFF) {
            D_AC[0].glyph = racer->value3BA << 0x10;
            D_AC[1].glyph = (s32) (o50Data320[racer->value3BA] << 0x10);
        } else {
            D_AC[0].glyph = (s32) (racer->value385 << 0x10);
            D_AC[1].glyph = (s32) (o50Data320[racer->value385] << 0x10);
        }
        overlay56SplitTime(racer->raceTime, &minutes, &seconds, &centiseconds);
        level = levelGetLevel();
        if ((D_800C947C == 0) && (level->laps != racer->laps) && (func_800290A0() == 0) && (func_8003A7D0(arg0) != racer->raceTime)) {
            centiseconds = centiseconds - centiseconds % 10 + D_32C++;
            D_32C = (s8) ((s8) D_32C % 10);
        }
        D_12C[0].glyph = (s32) ((minutes / 10) << 0x10);
        D_12C[1].glyph = (s32) ((minutes % 10) << 0x10);
        D_12C[3].glyph = (s32) ((seconds / 10) << 0x10);
        D_12C[4].glyph = (s32) ((seconds % 10) << 0x10);
        D_12C[6].glyph = (s32) ((centiseconds / 10) << 0x10);
        D_12C[7].glyph = (s32) ((centiseconds % 10) << 0x10);
        func_8002F618(&D_800D3140, D_12C, 0,  hudY, 0xFF, 0xFF, 0xFF, 0xFF);
        func_8002F618(&D_800D3140, D_FC, 0,  hudY, 0xFF, 0xFF, 0xFF, 0xFF);
        if (*modeFlag != 1) {
            if (racer->flags & 8) {
                func_8002F618(&D_800D3140, D_DC, 0,  hudY, 0xFF, 0xFF, 0xFF, 0xFF);
            } else {
                func_8002F618(&D_800D3140, D_AC, 0,  hudY, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
        func_80034920(&D_800D3140);
        D_800D3550[4].y = (f32) (0x54 - hudY);
        D_800D3550[4].angle = (s16) ((s32) (racer->raceTime * -0x10000) / 300);
        func_80039E34(4);
        D_800D3550[1].y = (f32) (0x43 - hudY);
        func_80039E34(1);
        func_8002F618(&D_800D3140, D_2C0, 0,  hudY, 0xFF, 0xFF, 0xFF, 0xFF);
        func_80034920(&D_800D3140);
        if (racer->item != 0xFF) {
            opacity = D_334 + (arg1 * 0x10);
            D_334 = opacity;
            if (opacity >= 0x100) {
                opacity = 0xFF;
                D_334 = 0xFF;
            }
        } else {
            opacity = D_334 - (arg1 * 8);
            D_334 = opacity;
            if (opacity < 0) {
                D_334 = 0;
                opacity = 0;
            }
        }
        if (opacity > 0) {
            if (racer->itemState != 0) {
                itemId = 0x35;
                loadedItem = (s32) o50Data330;
            } else {
                if (racer->item != 0xFF) {
                    itemId = D_8007C180[racer->item];
                    loadedItem = (s32) o50Data330;
                } else {
                    itemId = (s16) o50Data330;
                    loadedItem = (s32) itemId;
                }
            }
            if (itemId != loadedItem) {
                if (loadedItem != -1) {

                    freeFrontEndItem(loadedItem);
                }
                o50Data330 = (s8) itemId;
                loadedItem = (s32) o50Data330;
                if (o50Data330 != -1) {
                    loadFrontEndItem((s32) o50Data330);
                    loadedItem = (s32) o50Data330;
                }
            }
            if (loadedItem != -1) {
                if (loadedItem == 0x35) {
                    itemGlyphs[0].x = 0x8A;
                    itemGlyphs[0].y = 0xF;
                } else {
                    itemGlyphs[0].x = 0x90;
                    itemGlyphs[0].y = 0x15;
                }
                itemGlyphs[0].glyph = 0;
                itemGlyphs[0].alternate = 0;
                itemGlyphs[1].texture = 0;
                itemGlyphs[0].texture = D_800D31C8[loadedItem];
                func_8002F618(&D_800D3140, itemGlyphs, 0, 0, 0xFF, 0xFF, 0xFF, D_334);
                if (o50Data330 != 0x35) {
                    if ((s32) racer->itemCount >= 2) {
                        D_300[0].glyph = (s32) (racer->itemCount << 0x10);
                        func_8002F618(&D_800D3140, D_300, 0xAB, 0x2E, 0, 0, 0, D_334);
                        func_8002F618(&D_800D3140, D_300, 0xAC, 0x30, 0, 0, 0, D_334);
                        func_8002F618(&D_800D3140, D_300, 0xAC, 0x2F, 0xFF, 0xFF, 0xFF, D_334);
                    }
                }
            }
        } else {
            if (o50Data330 != -1) {
                freeFrontEndItem((s32) o50Data330);
                o50Data330 = -1;
            }
        }
        if (racer->value388 != 0) {
            i = 0;
            if (arg1 > 0) {
                remainder = arg1 & 3;
                if (remainder != 0) {
                    do {
                        i += 1;
                        D_21C += (s32) (0x800 - D_21C) >> 3;
                    } while (remainder != i);
                    if (i != arg1) {
                        goto block_62;
                    }
                } else {
block_62:
                    do {
                        i += 4;
                        D_21C += (s32) (0x800 - D_21C) >> 3;
                        D_21C += (s32) (0x800 - D_21C) >> 3;
                        D_21C += (s32) (0x800 - D_21C) >> 3;
                        D_21C += (s32) (0x800 - D_21C) >> 3;
                    } while (i != arg1);
                }
            }
            var_v0_3 = 1;
        } else if (D_21C == -0x420) {
            var_v0_3 = 0;
        } else {
            i = 0;
            if (arg1 > 0) {
                remainder = arg1 & 3;
                if (remainder != 0) {
                    do {
                        i += 1;
                        D_21C += (s32) (0x1820 - D_21C) >> 3;
                    } while (remainder != i);
                    if (i != arg1) {
                        goto block_72;
                    }
                } else {
block_72:
                    do {
                        i += 4;
                        D_21C += (s32) (0x1820 - D_21C) >> 3;
                        D_21C += (s32) (0x1820 - D_21C) >> 3;
                        D_21C += (s32) (0x1820 - D_21C) >> 3;
                        D_21C += (s32) (0x1820 - D_21C) >> 3;
                    } while (i != arg1);
                }
            }
            var_v0_3 = 1;
            if (((s16) D_21C >> 6) == 0x60) {
                D_21C = -0x420;
                var_v0_3 = 0;
            }
        }
        if (var_v0_3 != 0) {
            t9 = (s16) D_21C >> 4;
            D_1CC[0].x = (s16) t9;
            D_1CC[1].x = (s16) t9;
            D_1CC[2].x = (s16) t9;
            D_1CC[3].x = (s16) t9;
            func_8002F618(&D_800D3140, D_1CC, 0, 0, 0xFF, 0xFF, 0xFF, 0xC0);
        }
        if (overlay59Interpolate(0, -0x18, 0xBE, 0x30, 0xBE, &sp10C, &sp108, 1) != 0) {
            overlay59BuildList(0, messageGlyphs);
            if (messageGlyphs[0].texture != 0) {
                func_8002F618(&D_800D3140, messageGlyphs, sp10C, sp108, 0xFF, 0xFF, 0xFF, 0xFF);
            }
            overlay59DrawFrame(&D_800D3140, 0, sp10C, sp108);
        }
        func_80036544((void *) D_800D31C8[0], &D_328, 0x14, &D_800D3550[1].frame, arg1);
        if (racer->laps < level->laps) {
            remaining = racer->differenceTimer;
            if (remaining >= arg1) {
                i = 0;
                if ((remaining == 0xB4) && (racer->timeDifference >= 0)) {

                    amSndPlay(0x1F8, 0);
                    i = 0;
                    remaining = racer->differenceTimer;
                }
                racer->differenceTimer = (s16) (remaining - arg1);
                if (arg1 > 0) {
                    remainder = arg1 & 3;
                    if (remainder != 0) {
                        do {
                            i += 1;
                            D_C0 += (s32) (0x550 - D_C0) >> 3;
                            D_BC += (s32) (0x830 - D_BC) >> 3;
                        } while (remainder != i);
                        if (i != arg1) {
                            goto block_93;
                        }
                    } else {
block_93:
                        do {
                            i += 4;
                            t9 = D_BC + ((s32) (0x830 - D_BC) >> 3);
                            D_BC = t9;
                            t6 = D_C0 + ((s32) (0x550 - D_C0) >> 3);
                            t7 = t9 + ((s32) (0x830 - t9) >> 3);
                            D_C0 = t6;
                            t8 = t6 + ((s32) (0x550 - t6) >> 3);
                            D_BC = t7;
                            t9 = t7 + ((s32) (0x830 - t7) >> 3);
                            D_C0 = t8;
                            t6 = t8 + ((s32) (0x550 - t8) >> 3);
                            D_BC = t9;
                            D_C0 = t6;
                            D_C0 = t6 + ((s32) (0x550 - t6) >> 3);
                            D_BC = t9 + ((s32) (0x830 - t9) >> 3);
                        } while (i != arg1);
                    }
                }
            } else if (D_BC != -0x500) {
                i = 0;
                if (remaining != -1) {

                    amSndPlay(0x1F9, 0);
                    i = 0;
                    racer->differenceTimer = -1;
                }
                remainder = arg1 & 3;
                if (arg1 > 0) {
                    if (remainder != 0) {
                        do {
                            i += 1;
                            D_C0 += (s32) (-0x140 - D_C0) >> 3;
                            D_BC += (s32) (0x1900 - D_BC) >> 3;
                        } while (remainder != i);
                        if (i != arg1) {
                            goto block_104;
                        }
                    } else {
block_104:
                        do {
                            i += 4;
                            t8 = D_BC + ((s32) (0x1900 - D_BC) >> 3);
                            D_BC = t8;
                            t9 = D_C0 + ((s32) (-0x140 - D_C0) >> 3);
                            t6 = t8 + ((s32) (0x1900 - t8) >> 3);
                            D_C0 = t9;
                            t7 = t9 + ((s32) (-0x140 - t9) >> 3);
                            D_BC = t6;
                            t8 = t6 + ((s32) (0x1900 - t6) >> 3);
                            D_C0 = t7;
                            t9 = t7 + ((s32) (-0x140 - t7) >> 3);
                            D_BC = t8;
                            D_C0 = t9;
                            D_C0 = t9 + ((s32) (-0x140 - t9) >> 3);
                            D_BC = t8 + ((s32) (0x1900 - t8) >> 3);
                        } while (i != arg1);
                    }
                }
                if (D_BC >= 0x1861) {
                    D_BC = -0x500;
                    D_C0 = -0x140;
                }
            }
            if (racer->timeDifference <= 0) {
                o50LapGlyphs[0].glyph = 12 << 16;
                glyphAlternate = D_800D31C8[21];
                glyphTexture = D_800D31C8[20];
                timeMagnitude = (s32) -racer->timeDifference;
                glyphCursor = &o50LapGlyphs[1];
                o50LapGlyphs[0].alternate = glyphAlternate;
                o50LapGlyphs[0].texture = glyphTexture;
                do {
                    glyphCursor += 4;
                    glyphCursor[-3].texture = glyphTexture;
                    glyphCursor[-3].alternate = glyphAlternate;
                    glyphCursor[-2].texture = glyphTexture;
                    glyphCursor[-2].alternate = glyphAlternate;
                    glyphCursor[-1].texture = glyphTexture;
                    glyphCursor[-1].alternate = glyphAlternate;
                    glyphCursor[-4].texture = glyphTexture;
                    glyphCursor[-4].alternate = glyphAlternate;
                } while (glyphCursor != &o50LapGlyphs[9]);
            } else {
                o50LapGlyphs[0].glyph = 13 << 16;
                glyphAlternate = D_800D31C8[21];
                glyphTexture = D_800D31C8[80];
                glyphCursor = &o50LapGlyphs[1];
                o50LapGlyphs[0].alternate = glyphAlternate;
                o50LapGlyphs[0].texture = glyphTexture;
                timeMagnitude = (s32) racer->timeDifference;
                do {
                    glyphCursor += 4;
                    glyphCursor[-3].texture = glyphTexture;
                    glyphCursor[-3].alternate = glyphAlternate;
                    glyphCursor[-2].texture = glyphTexture;
                    glyphCursor[-2].alternate = glyphAlternate;
                    glyphCursor[-1].texture = glyphTexture;
                    glyphCursor[-1].alternate = glyphAlternate;
                    glyphCursor[-4].texture = glyphTexture;
                    glyphCursor[-4].alternate = glyphAlternate;
                } while (glyphCursor != &o50LapGlyphs[9]);
            }
            overlay56SplitTime(timeMagnitude, &minutes, &seconds, &centiseconds);
            o50LapGlyphs[1].glyph = (minutes / 10) << 0x10;
            o50LapGlyphs[2].glyph = (minutes % 10) << 0x10;
            o50LapGlyphs[4].glyph = (seconds / 10) << 0x10;
            o50LapGlyphs[5].glyph = (seconds % 10) << 0x10;
            o50LapGlyphs[7].glyph = (centiseconds / 10) << 0x10;
            o50LapGlyphs[8].glyph = (centiseconds % 10) << 0x10;
            templateCursor = D_230;
            glyphCursor = &o50LapGlyphs[1];
            do {
                if (((s32) glyphCursor[0].glyph >> 0x10) == 1) {
                    if ((templateCursor == D_230) || (templateCursor == &D_230[3]) || (templateCursor == &D_230[6])) {
                        glyphCursor[0].x = (s16) (templateCursor[0].x + 1);
                    } else {
                        glyphCursor[0].x = (s16) (templateCursor[0].x - 1);
                    }
                } else {
                    glyphCursor[0].x = (s16) templateCursor[0].x;
                }
                templateCursor++;
                glyphCursor++;
            } while (templateCursor != &D_230[8]);
            func_8002F618(&D_800D3140, o50LapGlyphs,  ((s32) D_BC >> 4),  ((s32) D_C0 >> 4), 0xFF, 0xFF, 0xFF, 0xFF);
        }

        lapIndex = 0;
        if (joyGetPressed(0) & 1) {
            D_8007C1B0 ^= 1;
        }
        if (D_8007C1B0 != 0) {
            if (D_8007BF1C & 1) {
                timeMagnitude = (s32) (racer->speed * 6.25f);
                if (timeMagnitude < 0) {
                    timeMagnitude = -timeMagnitude;
                }
                func_8004A4B0(0xE6, 0xB4,  timeMagnitude, 3, 0);
                func_8004B0A4(2);
                func_8004B0DC(0, 0, 0, 0);
                fontColour(0x40, 0xFF, 0x40, 0xFF, 0xE0);
                func_8004B0F8(&D_800D3140, 0x106, 0xBC, o50SpeedText, 0);
            } else {
                func_8002F618(&D_800D3140, D_6C,  hudX, 0, 0xFF, 0xFF, 0xFF, 0xFF);
                lapIndex = lapIndex;
                remainder = arg1 & 3;
                if (racer->speed < 0.0f) {
                    targetAngle = (s32) (16384.0f - (-racer->speed * o50ReverseScale));
                } else {
                    targetAngle = (s32) (16384.0f - (racer->speed * o50ForwardScale));
                }
                if (arg1 > 0) {
                    if (remainder != 0) {
                        do {

                            lapIndex += 1;
                            D_800D3550[0].angle = (s16) (D_800D3550[0].angle + ((s32) (targetAngle - D_800D3550[0].angle) >> 2));
                        } while (remainder != lapIndex);
                        if (lapIndex != arg1) {
                            goto loop_138;
                        }
                    } else {
                        do {
loop_138:

                            lapIndex += 4;
                            D_800D3550[0].angle = (s16) (D_800D3550[0].angle + ((s32) (targetAngle - D_800D3550[0].angle) >> 2));

                            D_800D3550[0].angle = (s16) (D_800D3550[0].angle + ((s32) (targetAngle - D_800D3550[0].angle) >> 2));

                            D_800D3550[0].angle = (s16) (D_800D3550[0].angle + ((s32) (targetAngle - D_800D3550[0].angle) >> 2));

                            D_800D3550[0].angle = (s16) (D_800D3550[0].angle + ((s32) (targetAngle - D_800D3550[0].angle) >> 2));
                        } while (lapIndex != arg1);
                    }
                    lapIndex = 0;
                }

                D_800D3550[0].y = -85.0f;
                D_800D3550[0].x = (f32) (hudX + 0x77);
                func_80034920(&D_800D3140);
                D_8007C0BC = 0xFF;
                func_80039E34(0);
                lapIndex = lapIndex;
                D_8007C0BC = 0xFF;
            }
        }
        lapCount = racer->laps;
        if (racer->finished != 0) {
            lapCount += 1;
        }
        if (lapCount > 0) {
            do {
                i = 0;
                if (arg1 > 0) {
                    remainder = arg1 & 3;
                    if (remainder != 0) {
                        do {
                            t6 = D_B0[lapIndex];
                            i += 1;
                            D_B0[lapIndex] = t6 + ((s32) -t6 >> 2);
                        } while (remainder != i);
                        if (i != arg1) {
                            goto loop_148;
                        }
                    } else {
                        do {
loop_148:
                            t6 = D_B0[lapIndex];
                            i += 4;
                            t9 = t6 + ((s32) -t6 >> 2);
                            t8 = t9 + ((s32) -t9 >> 2);
                            D_B0[lapIndex] = t9;
                            t6 = t8 + ((s32) -t8 >> 2);
                            D_B0[lapIndex] = t8;
                            D_B0[lapIndex] = t6;
                            D_B0[lapIndex] = t6 + ((s32) -t6 >> 2);
                        } while (i != arg1);
                    }
                }

                overlay50SubmitTimeGlyphs(lapIndex + 1, D_B0[lapIndex] + 0xD6, 0x3C + lapIndex * 10, racer->lapTimes[lapIndex]);
                lapIndex++;
            } while (lapIndex != lapCount);
        }
        if (lapCount > 0) {
            func_8002F618(&D_800D3140, D_2E0, D_B0[0], 0, 0xFF, 0xFF, 0xFF, 0xFF);
        }
        if (mainGetMode() == 0) {
            switch (*modeFlag) {                   /* irregular */
            case 0:
                if ((o50Overlay1StateReloc == 0) && (joyGetPressed(0) & 0x9000) && (D_33C == 0)) {
                    func_800016EC(1);
                    func_8003A590();
                    func_80037414(2, 4.0f, -1.0f, 0, 0, 0, 0);
                    mainChangeLevel(0x12, 0, 0, 7, 1, 1);
                    func_800005CC(3.0f, 0);
                    D_33C = 1;
                    return;
                }
                break;
            case 1:
                if ((o50Overlay1StateReloc == 0) && (joyGetPressed(0) & 0x9000) && (D_33C == 0)) {
                    func_800016EC(1);
                    func_8003A590();
                    func_80037414(2, 4.0f, -1.0f, 0, 0, 0, 0);
                    mainChangeLevel(0x12, 0, 0, 7, 1, 0);
                    func_800005CC(3.0f, 0);
                    D_33C = 1;
                }
                break;
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o050/func_overlay_050_F0000334_1896CA4/func_overlay_050_F0000334_1896CA4.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_050_F0000334_1896CA4:start
 * symbol: func_overlay_050_F0000334_1896CA4
 * score: 31/1575 words
 * frame: 0x1A0
 * relocations: 315
 * first-mismatch: +0x0
 * summary: Fresh V0 is 12 bytes short with a 136-byte frame surplus; flag, context, constant, and first-call families are exhausted; 204 proxy identities remain.
 * PLATEAU-HANDOFF:func_overlay_050_F0000334_1896CA4:end
 */
