#include "PR/ultratypes.h"

#define O52_S8(base, offset) (*(s8 *)((u8 *)(base) + (offset)))
#define O52_U8(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define O52_S16(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define O52_U16(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define O52_S32(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define O52_U32(base, offset) (*(u32 *)((u8 *)(base) + (offset)))
#define O52_F32(base, offset) (*(f32 *)((u8 *)(base) + (offset)))
#define O52_PTR(base, offset) (*(u8 **)((u8 *)(base) + (offset)))

extern s32 func_overlay_052_F0000000_189A670();
extern void func_overlay_052_F0000540_189ABB0();

extern u8 D_0[];
extern u8 D_8[];
extern u8 D_24C[];
extern u8 D_2F4[];
extern u8 D_314[];
extern u8 D_320[];
extern u8 D_328[];
extern u8 D_340[];
extern u8 D_4A0[];
extern u8 D_4A8[];
extern u8 D_4B4[];
extern u8 D_4B8[];
extern u8 D_4C4[];
extern u8 D_4C8[];
extern u8 D_480[];
extern volatile s32 D_4CC;
extern s16 D_4D0;
extern s16 D_4D2;
extern f32 D_4B0;
extern u8 D_50[];
extern u8 D_54[];
extern u8 D_58[];
extern u8 D_60[];
extern u8 D_80[];
extern u8 D_A0[];
extern u8 D_100[];
extern u8 D_140[];
extern u8 D_160[];
extern u8 D_170[];
extern u8 D_180[];
extern u8 D_200[];
extern u8 D_240[];
extern u8 D_F0[];

/* promotion_trial: text-differs, 1686 words differ in-range; first mismatch +0x0. */
/* Preflight shape: target/candidate frame 0x118/0x118; candidate is 1568 of 1687 words. */
/* Structural gap remains: 119 candidate words are short; linked ROM text differs throughout. */
#ifdef NON_MATCHING
void func_overlay_052_F000063C_189ACAC(s32 arg0) {
    s32 i;
    s32 j;
    s32 k;
    s32 state;
    s32 mode;
    s32 flags;
    s32 value;
    s32 value2;
    s32 timer;
    s32 active;
    s32 choice;
    s32 drawState;
    s32 spAC;
    s32 spA8;
    s32 spFC;
    s32 spF4;
    s32 spD0;
    s32 spDC;
    s32 spD8;
    s32 spD4;
    s32 sp104[2];
    s32 scratch;
    s32 glyph;
    s32 x;
    s32 y;
    s32 z;
    u8 *object;
    u8 *record;
    u8 *small;
    u8 *medium;
    u8 *large;
    u8 *screen;
    u8 **objects;
    f32 step;
    f32 floatValue;
    objects = (u8 **)(u32)func_overlay_052_F0000000_189A670(&spAC, &spA8);
    spA8 >>= 1;
    spFC = func_overlay_052_F0000000_189A670();
    state = func_overlay_052_F0000000_189A670() & 1;
    func_overlay_052_F0000000_189A670(0, 0);

    if (D_4CC != 0) {
        D_4D0 += arg0;
        if (D_4D0 >= 0x3D) {
            if (D_4D0 >= 0xF1) {
                D_4D2 -= arg0 * 4;
                if (D_4D2 < 0) {
                    func_overlay_052_F0000000_189A670(D_4CC, D_4D2);
                    D_4CC = 0;
                } else {
                    func_overlay_052_F0000000_189A670(D_4CC, D_4D2);
                }
            } else {
                D_4D2 += arg0 * 4;
                if (D_4D2 >= 0x100) {
                    D_4D2 = 0xFF;
                }
                func_overlay_052_F0000000_189A670(D_4CC, D_4D2);
            }
        }
    }

    screen = (u8 *)(u32)func_overlay_052_F0000000_189A670(&spF4);
    if (state != 0) {
        O52_S16(D_24C, 0) = 0;
        O52_S16(D_24C, 2) = -0x78;
    } else {
        O52_S16(D_24C, 0) = -0xC;
        O52_S16(D_24C, 2) = -0x78;
    }

    if (O52_S32(D_0, 0) == 0 && arg0 > 0) {
        i = 0;
        j = arg0 & 3;
        if (j != 0) {
            i = 1;
            step = (-11.0f - D_4B0) * 0.125f;
            while (i != j) {
                i++;
                D_4B0 += step;
                step = (-11.0f - D_4B0) * 0.125f;
            }
            D_4B0 += step;
        }
        if (i != arg0) {
            i += 4;
            step = (-11.0f - D_4B0) * 0.125f;
            while (i != arg0) {
                i += 4;
                D_4B0 += step;
                D_4B0 += (-11.0f - D_4B0) * 0.125f;
                D_4B0 += (-11.0f - D_4B0) * 0.125f;
                D_4B0 += (-11.0f - D_4B0) * 0.125f;
                step = (-11.0f - D_4B0) * 0.125f;
            }
            D_4B0 += step;
            D_4B0 += (-11.0f - D_4B0) * 0.125f;
            D_4B0 += (-11.0f - D_4B0) * 0.125f;
            D_4B0 += (-11.0f - D_4B0) * 0.125f;
        }
    }

    func_overlay_052_F0000000_189A670(O52_PTR(D_8, 0), D_314, 0x14,
                                       D_58, arg0);
    func_overlay_052_F0000000_189A670(O52_PTR(D_A0, 0), D_314, 0x14,
                                       D_58, arg0);
    spD0 = (s32)D_4B0;
    spF4 = func_overlay_052_F0000000_189A670();
    O52_S8(D_0, 0x31C)++;
    O52_S8(D_0, 0x31C) = (s8)(O52_S8(D_0, 0x31C) % 10);
    sp104[0] = -1;
    sp104[1] = -1;

    for (i = 0; i < 2; i++) {
        object = objects != NULL ? objects[i] : NULL;
        if (object == NULL) {
            continue;
        }
        record = O52_PTR(object, 0x64);
        value = O52_S32(D_320, i * 4);
        if (O52_U8(record, 0x19A) != 0xFF) {
            value += arg0 * 0x10;
            if (value >= 0x100) {
                value = 0xFF;
            }
        } else {
            value -= arg0 * 8;
            if (value < 0) {
                value = 0;
            }
        }
        O52_S32(D_320, i * 4) = value;
        if (value > 0) {
            if (O52_U8(record, 0x19C) != 0) {
                sp104[i] = 0x35;
            } else if (O52_U8(record, 0x19A) != 0xFF) {
                sp104[i] = O52_S16(D_4A8, O52_U8(record, 0x19A) * 2);
            } else {
                sp104[i] = O52_S32(D_4A8, i * 4);
            }
            if (sp104[i] != O52_S32(D_4A8, i * 4)) {
                if (O52_S32(D_4A8, i * 4) != -1) {
                    func_overlay_052_F0000000_189A670(O52_S32(D_4A8, i * 4),
                                                       &D_320[i * 4], 0xFF);
                }
                O52_S32(D_4A8, i * 4) = sp104[i];
                if (sp104[i] != -1) {
                    func_overlay_052_F0000000_189A670(sp104[i], &D_320[i * 4]);
                }
            }
        }
    }

    for (i = 0; i < 2; i++) {
        if (sp104[i] != -1) {
            func_overlay_052_F0000000_189A670(sp104[i]);
            O52_S32(D_4A8, i * 4) = -1;
        }
    }

    for (i = 0; i < 2; i++) {
        object = objects != NULL ? objects[i] : NULL;
        if (object == NULL) {
            continue;
        }
        record = O52_PTR(object, 0x64);
        small = D_240 + i * 4;
        medium = D_A0 + i * 0x30;
        large = D_200 + i * 0xA0;
        choice = i * 2;
        if (func_overlay_052_F0000000_189A670(i) & 2) {
            O52_S8(small, 0) ^= 1;
        }
        func_overlay_052_F0000000_189A670(i);
        func_overlay_052_F0000000_189A670(0);
        if (O52_U8(record, 0x3BA) != 0xFF) {
            O52_S32(small, 8) = O52_U8(record, 0x3BA) << 16;
            O52_S32(small, 0x18) = O52_S16(record, 0x3BA + 0x244) << 16;
        } else {
            O52_S32(small, 8) = O52_U8(record, 0x385) << 16;
            O52_S32(small, 0x18) = O52_S16(record, 0x385 + 0x244) << 16;
        }
        if (O52_U32(record, 0x1A8) & 8) {
            func_overlay_052_F0000000_189A670(0, D_60 + i * 0x20, 0,
                                               spD0, 0xFF, 0xFF, 0xFF, 0xFF);
        } else {
            func_overlay_052_F0000000_189A670(0, small, 0, spD0,
                                               0xFF, 0xFF, 0xFF, 0xFF);
        }

        if (O52_S8(small, 0) != 0) {
            func_overlay_052_F0000540_189ABB0(D_80, medium, i, 1);
            O52_S32(medium, 8) = (O52_S16(record, 0x192) / 10) << 16;
            O52_S32(medium, 0x18) = (O52_S16(record, 0x192) % 10) << 16;
            if ((O52_S32(medium, 8) >> 16) == 1) {
                O52_S16(medium, 0xC)++;
            }
            if ((O52_S32(medium, 0x18) >> 16) == 1) {
                O52_S16(medium, 0x1C)--;
            }
            func_overlay_052_F0000000_189A670(0, medium, 0, spD0,
                                               0xFF, 0xFF, 0xFF, 0xFF);
            mode = O52_U8(record, 0x383) + 1;
            if (O52_U8(record, 0x45C) != 0) {
                mode++;
            }
            if (mode >= 4) {
                mode = 3;
            }
            if (mode <= 0) {
                mode = 1;
            }
            O52_S32(D_100 + i * 0x30, 0x18) = mode << 16;
            func_overlay_052_F0000000_189A670(0, D_100 + i * 0x30, 0, spD0,
                                               0xFF, 0xFF, 0xFF, 0xFF);
            func_overlay_052_F0000000_189A670(0);
            if (spFC != 0) {
                O52_F32(D_24C, 0x2C) = (f32)O52_S16(record, 0x254);
                O52_F32(D_24C, 0x30) = (f32)(0x47 - spD0);
            } else {
                O52_F32(D_24C, 0x2C) = -55.0f;
                O52_F32(D_24C, 0x30) = (f32)(O52_S16(record, 0x24C) - spD0 + 0x53);
            }
            func_overlay_052_F0000000_189A670(0);
            func_overlay_052_F0000000_189A670(1);
            func_overlay_052_F0000000_189A670(1);
            func_overlay_052_F0000000_189A670(0);
        } else {
            large = D_200 + i * 0xA0;
            func_overlay_052_F0000000_189A670(O52_PTR(record, 0x400), &spDC,
                                               &spD8, &spD4);
            if (O52_S32(D_0, 0) == 0 && screen != NULL &&
                O52_S8(screen, 0x86) != O52_S8(record, 0x383) &&
                func_overlay_052_F0000000_189A670() == 0 &&
                func_overlay_052_F0000000_189A670(object) != O52_S32(record, 0x400)) {
                spD4 = spD4 - spD4 % 10 + O52_S8(D_0, 0x31C);
            }
            func_overlay_052_F0000540_189ABB0(D_180, large, i, 3);
            O52_S32(large, 8) = (spDC / 10) << 16;
            O52_S32(large, 0x18) = (spDC % 10) << 16;
            O52_S32(large, 0x38) = (spD8 / 10) << 16;
            O52_S32(large, 0x48) = (spD8 % 10) << 16;
            O52_S32(large, 0x68) = (spD4 / 10) << 16;
            O52_S32(large, 0x78) = (spD4 % 10) << 16;
            for (j = 0; j < 8; j++) {
                if ((O52_S32(large, j * 0x10 + 8) >> 16) == 1) {
                    if (j == 0 || j == 3 || j == 6) {
                        O52_S16(large, j * 0x10 + 0xC)++;
                    } else {
                        O52_S16(large, j * 0x10 + 0xC)--;
                    }
                }
            }
            func_overlay_052_F0000000_189A670(0, large, 0, spD0,
                                               0xFF, 0xFF, 0xFF, 0xFF);
            func_overlay_052_F0000000_189A670(0);
            if (spFC != 0) {
                O52_F32(D_24C, 0x8C) = (f32)O52_S16(record, 0x260);
                O52_F32(D_24C, 0x90) = (f32)(0x50 - spD0);
            } else {
                O52_F32(D_24C, 0x8C) = -44.0f;
                O52_F32(D_24C, 0x90) = (f32)(O52_S16(record, 0x24C) - spD0 + 0x5C);
            }
            O52_S16(D_24C, 0x84) = (s16)((-O52_S32(record, 0x400) * 0x10000) / 300);
            func_overlay_052_F0000000_189A670(4);
            func_overlay_052_F0000000_189A670(0, D_340 + i * 0xA0, 0, spD0,
                                               0xFF, 0xFF, 0xFF, 0xFF);
        }

        value = O52_S32(D_320, i * 4);
        if (value > 0 && O52_S32(D_4A8, i * 4) != -1) {
            glyph = O52_S32(D_4A8, i * 4);
            O52_S32(D_2F4, 8) = O52_U8(record, 0x19B) << 16;
            flags = (value - (value >> 1)) & 0xFF;
            func_overlay_052_F0000000_189A670(0, D_2F4, 0, 0,
                                               0, 0, 0, flags);
            if (glyph != 0x35 && O52_U8(record, 0x19B) >= 2) {
                func_overlay_052_F0000000_189A670(0, D_2F4, 0x1C, 0x35,
                                                   0, 0, 0, flags);
                func_overlay_052_F0000000_189A670(0, D_2F4, 0x1E, 0x36,
                                                   0, 0, 0, flags);
                func_overlay_052_F0000000_189A670(0, D_2F4, 0x1D, 0x35,
                                                   0xFF, 0xFF, 0xFF, 0xFF);
            }
        }

        if (spFC != 0) {
            x = O52_S16(record, 0x264);
            y = O52_S16(record, 0x26C);
            z = O52_S16(record, 0x268);
        } else {
            x = O52_S16(record, 0x270);
            y = O52_S16(record, 0x278);
            z = O52_S16(record, 0x274);
        }
        active = 0;
        if (O52_U8(record, 0x388) != 0) {
            active = 1;
            for (j = 0; j < arg0; j++) {
                O52_S16(D_4A0, i * 2) += (x - O52_S16(D_4A0, i * 2)) >> 3;
            }
        } else if (z != O52_S16(D_4A0, i * 2)) {
            for (j = 0; j < arg0; j++) {
                O52_S16(D_4A0, i * 2) += (y - O52_S16(D_4A0, i * 2)) >> 3;
            }
            if ((y >> 6) == (O52_S16(D_4A0, i * 2) >> 6)) {
                O52_S16(D_4A0, i * 2) = z;
            } else {
                active = 1;
            }
        }
        if (active != 0) {
            O52_S16(D_480, 0xC) = O52_S16(D_4A0, i * 2) >> 4;
            O52_S16(D_480, 0xE) = spFC != 0 ? 0x3C : (i == 0 ? 0x28 : 0xA0);
            func_overlay_052_F0000000_189A670(0, D_480, 0, 0,
                                               0xFF, 0xFF, 0xFF, 0xC0);
        }

        timer = O52_S16(record, 0x456);
        if (O52_U8(record, 0x383) < (screen != NULL ? O52_U8(screen, 0x86) : 0)) {
            if (timer >= arg0) {
                timer -= arg0;
                O52_S16(record, 0x456) = timer;
                for (j = 0; j < arg0; j++) {
                    O52_S16(D_4B4, O52_U8(record, 0) * 2) +=
                        (O52_S16(D_4C4, O52_U8(record, 0) * 2) -
                         O52_S16(D_4B4, O52_U8(record, 0) * 2)) >> 3;
                    O52_S16(D_4B8, O52_U8(record, 0) * 2) +=
                        (O52_S16(D_4C8, O52_U8(record, 0) * 2) -
                         O52_S16(D_4B8, O52_U8(record, 0) * 2)) >> 3;
                }
            } else {
                if (timer != -1 && O52_S16(record, 0x454) < 0) {
                    scratch = 0;
                    func_overlay_052_F0000000_189A670(0x1F9, 0);
                    timer = -1;
                    O52_S16(record, 0x456) = -1;
                }
                for (j = 0; j < arg0; j++) {
                    O52_S16(D_4B4, O52_U8(record, 0) * 2) +=
                        (0x1900 - O52_S16(D_4B4, O52_U8(record, 0) * 2)) >> 3;
                    O52_S16(D_4B8, O52_U8(record, 0) * 2) +=
                        ((spA8 * O52_U8(record, 0) * 0x10 -
                          O52_S16(D_4B8, O52_U8(record, 0) * 2) - 0x140) >> 3);
                }
                if (O52_S16(D_4B4, O52_U8(record, 0) * 2) >= 0x1861) {
                    O52_S16(D_4B4, O52_U8(record, 0) * 2) =
                        O52_S16(D_4C4, O52_U8(record, 0) * 2);
                    O52_S16(D_4B8, O52_U8(record, 0) * 2) =
                        O52_S16(D_4C8, O52_U8(record, 0) * 2);
                }
            }
        }

        if (O52_S16(record, 0x454) <= 0) {
            O52_S32(D_160, 8) = 0xC0000;
            O52_S32(D_160, 0) = O52_S32(D_50, 0);
            O52_S32(D_160, 4) = O52_S32(D_54, 0);
            for (j = 0; j < 8; j++) {
                O52_S32(D_170 + j * 0x10, 0) = O52_S32(D_50, 0);
                O52_S32(D_170 + j * 0x10, 4) = O52_S32(D_54, 0);
            }
            value2 = -O52_S16(record, 0x454);
        } else {
            O52_S32(D_160, 8) = 0xD0000;
            O52_S32(D_160, 0) = O52_S32(D_140, 0);
            O52_S32(D_160, 4) = O52_S32(D_54, 0);
            for (j = 0; j < 8; j++) {
                O52_S32(D_170 + j * 0x10, 0) = O52_S32(D_140, 0);
                O52_S32(D_170 + j * 0x10, 4) = O52_S32(D_54, 0);
            }
            value2 = O52_S16(record, 0x454);
        }
        func_overlay_052_F0000000_189A670(value2, &spDC, &spD8, &spD4);
        O52_S32(D_160, 0x18) = (spDC / 10) << 16;
        O52_S32(D_160, 0x28) = (spDC % 10) << 16;
        O52_S32(D_160, 0x48) = (spD8 / 10) << 16;
        O52_S32(D_160, 0x58) = (spD8 % 10) << 16;
        O52_S32(D_160, 0x78) = (spD4 / 10) << 16;
        O52_S32(D_160, 0x88) = (spD4 % 10) << 16;
        for (j = 0; j < 8; j++) {
            if ((O52_S32(D_170 + j * 0x10, 8) >> 16) == 1) {
                O52_S16(D_170 + j * 0x10, 0xC) =
                    O52_S16(D_F0 + j * 0x10, 0xC) +
                    ((j == 0 || j == 3 || j == 6) ? 1 : -1);
            } else {
                O52_S16(D_170 + j * 0x10, 0xC) = O52_S16(D_F0 + j * 0x10, 0xC);
            }
        }
        func_overlay_052_F0000000_189A670(0, D_160,
                                           O52_S16(D_4B4, O52_U8(record, 0) * 2) >> 4,
                                           O52_S16(D_4B8, O52_U8(record, 0) * 2) >> 4,
                                           0xFF, 0xFF, 0xFF, 0xFE);

        if (func_overlay_052_F0000000_189A670(i) & 1) {
            O52_S8(D_240, i) ^= 1;
        }
        drawState = func_overlay_052_F0000000_189A670();
        if (drawState == 0) {
            flags = func_overlay_052_F0000000_189A670() & 0xFFFF;
            value = func_overlay_052_F0000000_189A670(1);
            if (((value | flags) & 0x9000) && O52_S32(D_0, 0x318) == 0) {
                func_overlay_052_F0000000_189A670(1);
                func_overlay_052_F0000000_189A670(1);
                func_overlay_052_F0000000_189A670(0);
                func_overlay_052_F0000000_189A670(2, 0x40800000, 0xBF800000,
                                                   0, 0, 0, 0);
                func_overlay_052_F0000000_189A670(0x12, 0, 0, 7, 1, 1);
                func_overlay_052_F0000000_189A670(0x40400000, 0);
                O52_S32(D_0, 0x318) = 1;
            }
        }
    }
    func_overlay_052_F0000000_189A670(0);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o052/overlay52TailB/func_overlay_052_F000063C_189ACAC.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_052_F000063C_189ACAC:start
 * symbol: func_overlay_052_F000063C_189ACAC
 * score: 1568/1687 words
 * frame: 0x118
 * relocations: 314
 * first-mismatch: +0x0
 * summary: Linked promotion trial text-differs 1686 words; candidate is 119 words short despite exact frame.
 * PLATEAU-HANDOFF:func_overlay_052_F000063C_189ACAC:end
 */
