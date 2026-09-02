#include "PR/ultratypes.h"

#define O60_U8(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define O60_S8(base, offset) (*(s8 *)((u8 *)(base) + (offset)))
#define O60_U16(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define O60_S16(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define O60_U32(base, offset) (*(u32 *)((u8 *)(base) + (offset)))
#define O60_S32(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define O60_F32(base, offset) (*(f32 *)((u8 *)(base) + (offset)))
#define O60_PTR(base, offset) (*(u8 **)((u8 *)(base) + (offset)))

extern s32 func_overlay_060_F0000000_18B9DD8();
extern s32 func_overlay_060_F0002F54_18BCD2C(s32, s32, s32, s32, s32, s32,
                                             s32);
extern void func_overlay_060_F00032CC_18BD0A4(s32, s32, s32, s32);
extern void func_overlay_060_F00033D8_18BD1B0(s32, s32, s32, s32);
extern void func_overlay_060_F0003488_18BD260(void);

/* The overlay's data image is addressed by offsets in the original TU. */
extern u8 D_0[];

/* promotion_trial: text-differs, 2781 words differ in-range; first mismatch +0x0. */
/* Preflight shape: target/candidate frame 0x198/0x198; candidate is 871 of 2789 words. */
/* Structural gap remains: linked ROM text differs throughout; candidate is 1918 words short. */
#ifdef NON_MATCHING
void func_overlay_060_F0000334_18BA10C(s32 arg0) {
    volatile u8 stackPad[0xA0];
    u8 **objects;
    u8 *object;
    u8 *entry;
    u8 *cursor;
    s32 i;
    s32 j;
    s32 count;
    s32 state;
    s32 selected;
    s32 visible;
    s32 input;
    s32 value;
    s32 value2;
    s32 progress;
    s32 mode;
    s32 choice;
    s32 limit;
    s32 axis;
    s32 spBC;
    s32 sp154;
    s32 sp158;
    s32 sp15C;
    s32 sp18C;
    s32 sp190;
    s32 sp194;
    s32 bits[16];
    f32 scale;

    stackPad[0] = (u8)arg0;
    sp190 = 0;
    sp18C = 0;

    if (O60_PTR(D_0, 0xA8) == NULL) {
        objects = (u8 **)(u32)func_overlay_060_F0000000_18B9DD8(&sp190,
                                                                  &sp18C);
        if (objects != NULL && sp190 < sp18C) {
            do {
                object = objects[sp190];
                sp190++;
                if (object != NULL && O60_S16(object, 0x44) == 0x54) {
                    O60_PTR(D_0, 0xA8) = object;
                    sp190 = sp18C;
                }
            } while (sp190 < sp18C);
        }
    }

    if (O60_S32(D_0, 0x160) != 0) {
        func_overlay_060_F0000000_18B9DD8(O60_U16(D_0, 0x2) >> 10);
        value = O60_S32(D_0, 0x0);
        func_overlay_060_F0000000_18B9DD8((value << 9) >> 30);
        func_overlay_060_F0000000_18B9DD8(O60_U16(D_0, 0x4));
        func_overlay_060_F0000000_18B9DD8(O60_U16(D_0, 0x6));
        O60_S32(D_0, 0x160) = 0;
    }

    if (O60_PTR(D_0, 0xA8) != NULL &&
        func_overlay_060_F0000000_18B9DD8(O60_PTR(D_0, 0x58)) != 0) {
        func_overlay_060_F0000000_18B9DD8(2);
        progress = O60_S32(D_0, 0x40) + arg0 * 2;
        if (progress >= 0x100) {
            progress = 0xFF;
        }
        O60_S32(D_0, 0x40) = progress;
        func_overlay_060_F0000000_18B9DD8(0, 0xFF, 0, 0xFF, progress);
        sp194 = func_overlay_060_F0000000_18B9DD8(O60_PTR(D_0, 0xA8));

        if (O60_S32(D_0, 0x4C) == -1 &&
            func_overlay_060_F0000000_18B9DD8(O60_PTR(D_0, 0xA8)) != 0) {
            O60_S32(D_0, 0x4C) = sp194;
            O60_S32(D_0, 0x14C) = 0;
            O60_S32(D_0, 0x50) = 0;
            O60_S32(D_0, 0x54) = 0;
            O60_S32(D_0, 0x150) = 0;
            O60_S32(D_0, 0x154) = 1;
            O60_S32(D_0, 0x158) = 0;
        }

        if (func_overlay_060_F0000000_18B9DD8(O60_PTR(D_0, 0xA8)) != 0 &&
            sp194 == O60_S32(D_0, 0x4C)) {
            value = O60_S32(D_0, 0x44) + arg0 * 8;
            O60_S32(D_0, 0x44) = value < 0x100 ? value : 0xFF;
        } else {
            value = O60_S32(D_0, 0x44) - arg0 * 8;
            O60_S32(D_0, 0x44) = value < 0 ? 0 : value;
            if (value < 0) {
                O60_S32(D_0, 0x4C) = -1;
            }
        }

        if (func_overlay_060_F0000000_18B9DD8(O60_PTR(D_0, 0xA8)) != 0) {
            value = O60_S32(D_0, 0x48) - arg0 * 4;
            O60_S32(D_0, 0x48) = value < 0x69 ? 0x69 : value;
        } else {
            value = O60_S32(D_0, 0x48) + arg0 * 4;
            O60_S32(D_0, 0x48) = value >= 0x9C ? 0x9B : value;
        }

        if (O60_S32(D_0, 0x44) != 0) {
            state = O60_S32(D_0, 0x4C);
            if ((u32)state < 10) {
                switch (state) {
                case 0:
                    if (O60_S32(D_0, 0x134) == 0) {
                        value = func_overlay_060_F0000000_18B9DD8(9);
                        O60_S32(D_0, 0x134) = O60_U16(D_0, 0x138 + value * 2);
                    }
                    value = func_overlay_060_F0000000_18B9DD8();
                    value = func_overlay_060_F0002F54_18BCD2C(
                        0x5F, 0xB9, 0x64, 0x28, value,
                        O60_S32(D_0, 0x44), arg0);
                    func_overlay_060_F0000000_18B9DD8(value);
                    break;

                case 1:
                case 8:
                    input = O60_S16(D_0, 0x0);
                    if (func_overlay_060_F0000000_18B9DD8(
                            O60_PTR(D_0, 0xA8)) != 0) {
                        if (input < 0) {
                            if (O60_S32(D_0, 0x50) == 0) {
                                O60_S32(D_0, 0x50) = 1;
                                func_overlay_060_F0000000_18B9DD8(0xF, 0);
                            } else {
                                func_overlay_060_F0000000_18B9DD8(0xE, 0);
                            }
                        } else if (input > 0) {
                            if (O60_S32(D_0, 0x50) == 1) {
                                O60_S32(D_0, 0x50) = 0;
                                func_overlay_060_F0000000_18B9DD8(0xF, 0);
                            } else {
                                func_overlay_060_F0000000_18B9DD8(0xE, 0);
                            }
                        }
                    }
                    if (O60_S32(D_0, 0x50) == 0) {
                        func_overlay_060_F0000000_18B9DD8(
                            0x64, 0xFF, 0x64, 0xFF,
                            O60_S32(D_0, 0x44));
                        if (input != 0) {
                            O60_U8(D_0, 0x2) =
                                (u8)((O60_U8(D_0, 0x2) & 0xFD) |
                                     (((O60_S32(D_0, 0x0) >> 9) & 1) ^ 1) * 2);
                            func_overlay_060_F0000000_18B9DD8(0xF, 0);
                        }
                    } else {
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0xBE, 0, 0xFF, O60_S32(D_0, 0x44));
                    }
                    spBC = func_overlay_060_F0000000_18B9DD8(
                        &spBC, &O60_S32(D_0, 0x40), 0x1D4,
                        O60_PTR(D_0, 0x38));
                    func_overlay_060_F0000000_18B9DD8(
                        0, 0x91, state == 1 ? 0x96 : 0xAA, &spBC, 0xC);
                    break;

                case 2:
                    selected = func_overlay_060_F0000000_18B9DD8();
                    if (O60_PTR(D_0, 0xA8) != NULL && input != 0) {
                        selected ^= 1;
                        func_overlay_060_F0000000_18B9DD8(selected);
                        func_overlay_060_F0000000_18B9DD8(0xF, 0);
                    }
                    func_overlay_060_F0000000_18B9DD8(
                        0x64, 0xFF, 0x64, 0xFF, O60_S32(D_0, 0x44));
                    func_overlay_060_F0000000_18B9DD8(
                        0, 0x91, 0x9B,
                        O60_PTR(D_0, 0x1E8 + selected * 4), 0xC);
                    break;

                case 3:
                    count = 0;
                    for (i = 0; i < 16; i++) {
                        if (O60_U16(D_0, 0x14) & (1 << i)) {
                            bits[count++] = i;
                        }
                    }
                    input = O60_S16(D_0, 0x0);
                    if (input != 0 && count != 0) {
                        if (input < 0 && O60_S32(D_0, 0x50) > 0) {
                            O60_S32(D_0, 0x50)--;
                            if (O60_S32(D_0, 0x50) < O60_S32(D_0, 0x54)) {
                                O60_S32(D_0, 0x54) = O60_S32(D_0, 0x50);
                            }
                            func_overlay_060_F0000000_18B9DD8(0xF, 0);
                        } else if (input > 0 &&
                                   O60_S32(D_0, 0x50) < count - 1) {
                            O60_S32(D_0, 0x50)++;
                            if (O60_S32(D_0, 0x54) <
                                O60_S32(D_0, 0x50) - 5) {
                                O60_S32(D_0, 0x54) = O60_S32(D_0, 0x50) - 5;
                            }
                            func_overlay_060_F0000000_18B9DD8(0xF, 0);
                        }
                        if (input != 0) {
                            O60_U16(D_0, 0x14) ^=
                                1 << bits[O60_S32(D_0, 0x50)];
                            func_overlay_060_F0000000_18B9DD8(0xF, 0);
                        }
                    }
                    limit = O60_S32(D_0, 0x54) + 6;
                    for (i = O60_S32(D_0, 0x54); i < limit && i < count;
                         i++) {
                        if (i == O60_S32(D_0, 0x50)) {
                            func_overlay_060_F0000000_18B9DD8(
                                0x64, 0xFF, 0x64, 0xFF,
                                O60_S32(D_0, 0x44));
                        } else {
                            func_overlay_060_F0000000_18B9DD8(
                                0, 0xBE, 0, 0xFF, O60_S32(D_0, 0x44));
                        }
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0x4D, 0x8C + (i - O60_S32(D_0, 0x54)) * 0xC,
                            O60_PTR(D_0, 0x1F0 + bits[i] * 4), 8);
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0xC1, 0x8C + (i - O60_S32(D_0, 0x54)) * 0xC,
                            O60_PTR(D_0, 0x38 +
                                   ((O60_U16(D_0, 0x14) >> bits[i]) & 1) * 4),
                            8);
                    }
                    break;

                case 4:
                    value = func_overlay_060_F0000000_18B9DD8();
                    value = func_overlay_060_F0002F54_18BCD2C(
                        0x5F, 0xB9, 0x64, 0x28, value,
                        O60_S32(D_0, 0x44), arg0);
                    func_overlay_060_F0000000_18B9DD8(value);
                    if (O60_U16(D_0, 0x0) & 0x80) {
                        if (O60_S32(D_0, 0x150) > 0) {
                            O60_S32(D_0, 0x150) -= arg0;
                            if (O60_S32(D_0, 0x150) <= 0) {
                                O60_S32(D_0, 0x150) = 0;
                                func_overlay_060_F0000000_18B9DD8(
                                    O60_U8(D_0, 0x15F));
                            }
                        }
                        input = O60_U16(D_0, 0x0);
                        if (input & 4 && O60_S32(D_0, 0x50) > 0) {
                            O60_S32(D_0, 0x50)--;
                            func_overlay_060_F0000000_18B9DD8(0xF, 0);
                        } else if ((input & 8) &&
                                   O60_S32(D_0, 0x50) < 0x2A) {
                            O60_S32(D_0, 0x50)++;
                            func_overlay_060_F0000000_18B9DD8(0xF, 0);
                        }
                        func_overlay_060_F0000000_18B9DD8(
                            &spBC, O60_PTR(D_0, 0x2C8),
                            O60_S32(D_0, 0x50) + 1);
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0x91, 0x7F, &spBC, 0xC);
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0x91, 0x75, D_0 + 0xC0, 0xC);
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0x91, 0x89, D_0 + 0xC4, 0xC);
                    }
                    break;

                case 5:
                    selected = func_overlay_060_F0000000_18B9DD8();
                    input = O60_S16(D_0, 0x0);
                    if (input < 0) {
                        selected = (selected + 3) & 3;
                    } else if (input > 0) {
                        selected = (selected + 1) & 3;
                    }
                    func_overlay_060_F0000000_18B9DD8(selected);
                    func_overlay_060_F0000000_18B9DD8(
                        0, 0xFF, 0, 0xFF, O60_S32(D_0, 0x44));
                    func_overlay_060_F0000000_18B9DD8(
                        0, 0x91, selected == 3 ? 0xB9 : 0x9B,
                        O60_PTR(D_0, 0x298 + (selected & 3) * 4), 0xC);
                    break;

                case 6:
                    selected = func_overlay_060_F0000000_18B9DD8();
                    if (O60_S32(D_0, 0x158) == 0 && input != 0) {
                        O60_S32(D_0, 0x158) = 1;
                        func_overlay_060_F0000000_18B9DD8(0xF, 0);
                    }
                    if (O60_S32(D_0, 0x158) != 0) {
                        if (selected == 0) {
                            selected = 1;
                        } else {
                            selected = 0;
                        }
                        func_overlay_060_F0000000_18B9DD8(selected);
                        func_overlay_060_F0000000_18B9DD8(0x3B449BA6,
                                                           (s32)arg0);
                        O60_S32(D_0, 0x158) = 0;
                    }
                    func_overlay_060_F0000000_18B9DD8(
                        0x64, 0xFF, 0x64, 0xFF, O60_S32(D_0, 0x44));
                    func_overlay_060_F0000000_18B9DD8(
                        0, 0x91, 0x91, O60_PTR(D_0, 0x1DC), 0xC);
                    func_overlay_060_F0000000_18B9DD8(selected);
                    break;

                case 7:
                    if (O60_S32(D_0, 0x14C) == 0) {
                        if (O60_U16(D_0, 0x0) & 0x9000) {
                            O60_S32(D_0, 0x14C) = 1;
                            O60_S32(D_0, 0x150) = 0;
                            func_overlay_060_F0000000_18B9DD8(0xC, 0);
                        } else if (input < 0 && O60_S32(D_0, 0x50) > 0) {
                            O60_S32(D_0, 0x50)--;
                            func_overlay_060_F0000000_18B9DD8(0xF, 0);
                        } else if (input > 0 && O60_S32(D_0, 0x50) < 3) {
                            O60_S32(D_0, 0x50)++;
                            func_overlay_060_F0000000_18B9DD8(0xF, 0);
                        }
                        O60_S32(D_0, 0x154) = 0;
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0x91, 0x8F, O60_PTR(D_0, 0x280), 0xC);
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0x91, 0x9B, O60_PTR(D_0, 0x288), 0xC);
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0x91, 0xA7, O60_PTR(D_0, 0x27C), 0xC);
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0x91, 0xB3, O60_PTR(D_0, 0x28C), 0xC);
                    } else {
                        mode = O60_S32(D_0, 0x50);
                        for (i = 0; i < 4; i++) {
                            if (i == mode) {
                                func_overlay_060_F0000000_18B9DD8(
                                    0x80, 0xFF, 0x80, 0xFF,
                                    O60_S32(D_0, 0x44));
                            } else {
                                func_overlay_060_F0000000_18B9DD8(
                                    0, 0xD0, 0, 0xFF,
                                    O60_S32(D_0, 0x44));
                            }
                            func_overlay_060_F0000000_18B9DD8(
                                0, 0x91, 0x8F + i * 0xC,
                                O60_PTR(D_0, 0x280 + i * 4), 0xC);
                        }
                    }
                    break;

                case 9:
                    input = O60_S16(D_0, 0x0);
                    if (input < 0 && O60_S32(D_0, 0x50) < 0xB) {
                        O60_S32(D_0, 0x50)++;
                        func_overlay_060_F0000000_18B9DD8(0xF, 0);
                    } else if (input > 0 && O60_S32(D_0, 0x50) > 0) {
                        O60_S32(D_0, 0x50)--;
                        func_overlay_060_F0000000_18B9DD8(0xF, 0);
                    }
                    for (i = O60_S32(D_0, 0x54);
                         i < O60_S32(D_0, 0x54) + 6; i++) {
                        func_overlay_060_F0000000_18B9DD8(
                            0, i == O60_S32(D_0, 0x50) ? 0x91 : 0xBE,
                            0x8C + (i - O60_S32(D_0, 0x54)) * 0xC,
                            O60_PTR(D_0, 0x1F0 + i * 4), 8);
                        func_overlay_060_F0000000_18B9DD8(
                            0, 0xC1,
                            0x8C + (i - O60_S32(D_0, 0x54)) * 0xC,
                            O60_PTR(D_0, 0x38 +
                                   ((O60_U16(D_0, 0x14) >> (i + 2)) & 1) * 4),
                            8);
                    }
                    break;
                }
            }
        }

        if ((O60_U16(D_0, 0x0) & 0x4000) &&
            O60_S32(D_0, 0x4C) == -1 && O60_S32(D_0, 0x164) == 0) {
            O60_S32(D_0, 0x164) = 1;
            if (O60_U16(D_0, 0x0) & 0x40) {
                O60_U16(D_0, 0x0) &= 0xFFBF;
                func_overlay_060_F0000000_18B9DD8();
            } else {
                func_overlay_060_F0000000_18B9DD8(0xC, 0, 0, 0xC, 1, 0);
            }
            func_overlay_060_F0000000_18B9DD8(0xD, 0);
        }

        func_overlay_060_F0000000_18B9DD8(
            0, 0xFF, 0, 0xFF, O60_S32(D_0, 0x40));
        func_overlay_060_F0000000_18B9DD8(
            0, 0x91, O60_S32(D_0, 0x48),
            O60_PTR(D_0, 0x48 + O60_S32(D_0, 0x48) * 4), 0xC);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o060/overlay60Prefix/func_overlay_060_F0000334_18BA10C.s")
#endif
