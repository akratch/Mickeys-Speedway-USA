#include "PR/ultratypes.h"

typedef s32 O54Word;

#define O54_S8(base, offset) (*(s8 *)((u8 *)(base) + (offset)))
#define O54_U8(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define O54_S16(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define O54_U16(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define O54_S32(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define O54_U32(base, offset) (*(u32 *)((u8 *)(base) + (offset)))
#define O54_F32(base, offset) (*(f32 *)((u8 *)(base) + (offset)))
#define O54_PTR(base, offset) (*(u8 **)((u8 *)(base) + (offset)))

extern s32 func_overlay_054_F0000000_189ECA0();
extern void func_overlay_054_F0000504_189F1A4();

extern u8 D_0[];
extern u8 D_4[];
extern u8 D_8[];
extern u8 D_10[];
extern u8 D_28[];
extern u8 D_2C[];
extern u8 D_30[];
extern u8 D_38[];
extern u8 D_50[];
extern u8 D_54[];
extern u8 D_58[];
extern u8 D_78[];
extern u8 D_84[];
extern u8 D_88[];
extern u8 D_8C[];
extern u8 D_90[];
extern u8 D_A0[];
extern u8 D_AC[];
extern u8 D_C0[];
extern u8 D_CC[];
extern u8 D_FC[];
extern u8 D_12C[];
extern u8 D_140[];
extern u8 D_14C[];
extern u8 D_1C0[];
extern u8 D_1E8[];
extern u8 D_218[];
extern u8 D_248[];
extern u8 D_268[];
extern u8 D_278[];
extern u8 D_280[];
extern u8 D_298[];
extern u8 D_2A8[];
extern u8 D_2B4[];
extern u8 D_340[];
extern u8 D_5C0[];
extern u8 D_640[];
extern u8 D_648[];
extern u8 D_654[];
extern u8 D_660[];
extern void *D_668;
extern s16 D_66C;
extern s16 D_66E;
extern f32 D_658;
extern s8 D_2B0;

/* promotion_trial: text-differs, 1587 words differ in-range; first mismatch +0x0. */
/* Preflight shape: target/candidate frame 0x138/0x138; NON_MATCHING candidate is 1564 words. */
/* Structural gap remains: 30 candidate words short; this is not permuter-ready. */
#ifdef NON_MATCHING
void func_overlay_054_F00005AC_189F24C(s32 arg0) {
    s32 i;
    s32 count;
    s32 state;
    s32 mode;
    s32 x;
    s32 y;
    s32 z;
    s32 oldValue;
    s32 newValue;
    s32 frame;
    s32 option;
    s32 active;
    s32 flags;
    s32 sp108;
    s32 sp124;
    s32 sp120;
    s32 sp12C;
    s32 sp100;
    s32 spFC;
    s32 spF8;
    s32 spF4;
    s32 spF0;
    s32 spB8;
    s32 spB4;
    u32 spB0;
    u8 *object;
    u8 *record;
    u8 **objects;
    u8 *cursor;
    u8 *next;
    u8 *small;
    u8 *large;
    f32 step;
    f32 value;
    volatile u8 stackPad[0x20];

    stackPad[0] = (u8)arg0;
    func_overlay_054_F0000000_189ECA0(0, 0);
    if (D_668 != NULL) {
        D_66C += arg0;
        if (D_66C >= 0x3D) {
            if (D_66C >= 0xF1) {
                D_66E -= arg0 * 4;
                if (D_66E < 0) {
                    func_overlay_054_F0000000_189ECA0(D_668, D_66E);
                    D_668 = NULL;
                } else {
                    func_overlay_054_F0000000_189ECA0(D_668, D_66E);
                }
            } else {
                D_66E += arg0 * 4;
                if (D_66E >= 0x100) {
                    D_66E = 0xFF;
                }
                func_overlay_054_F0000000_189ECA0(D_668, D_66E);
            }
        }
    }

    objects = (u8 **)(u32)func_overlay_054_F0000000_189ECA0(&sp108);
    if (O54_U8(D_0, 0) == 0) {
        state = 0;
        if (arg0 > 0) {
            i = arg0 & 3;
            if (i != 0) {
                state = 1;
                step = (-11.0f - D_658) * 0.125f;
                while (state != i) {
                    state++;
                    D_658 += step;
                    step = (-11.0f - D_658) * 0.125f;
                }
                D_658 += step;
            }
            if (state != arg0) {
                state += 4;
                step = (-11.0f - D_658) * 0.125f;
                while (state != arg0) {
                    state += 4;
                    D_658 += step;
                    D_658 += (-11.0f - D_658) * 0.125f;
                    D_658 += (-11.0f - D_658) * 0.125f;
                    D_658 += (-11.0f - D_658) * 0.125f;
                    step = (-11.0f - D_658) * 0.125f;
                }
                D_658 += step;
                D_658 += (-11.0f - D_658) * 0.125f;
                D_658 += (-11.0f - D_658) * 0.125f;
                D_658 += (-11.0f - D_658) * 0.125f;
            }
        }
    }

    func_overlay_054_F0000000_189ECA0(O54_PTR(D_8, 0), D_2A8, 0x14,
                                       D_58, arg0);
    func_overlay_054_F0000000_189ECA0(O54_PTR(D_A0, 0), D_2A8, 0x14,
                                       D_38, arg0);
    spF0 = (s32)D_658;
    func_overlay_054_F0000000_189ECA0(&spB4, &spB0);
    D_2B0 += 1;
    D_2B0 = (s8)((s8)D_2B0 % 10);
    count = O54_U8(D_0, 0);
    sp12C = 0;
    cursor = (u8 *)objects;
    while (sp12C < count) {
        object = O54_PTR(cursor, 0);
        cursor += 4;
        if (object != NULL) {
            record = O54_PTR(object, 0x64);
            small = D_C0 + sp12C * 0x20;
            next = D_1C0 + sp12C * 0x30;
            large = D_340 + sp12C * 0xA0;
            O54_S32(D_298 + sp12C * 4, 0) =
                (O54_S32(D_298 + sp12C * 4, 0) + 1) & 1;

            func_overlay_054_F0000000_189ECA0(sp12C, O54_S32(D_298 + sp12C * 4, 0));
            mode = O54_S8(record, 0x383) + 1;
            if (O54_U8(record, 0x45C) != 0) {
                mode++;
            }
            if (mode >= 4) {
                mode = 3;
            }
            if (mode <= 0) {
                mode = 1;
            }
            O54_S32(D_280 + sp12C * 0x30, 0x18) = mode << 16;
            O54_S32(next, 8) = (O54_S16(record, 0x192) / 10) << 16;
            O54_S32(next, 0x18) = (O54_S16(record, 0x192) % 10) << 16;
            O54_S32(next, 0x28) = (s32)(D_658 * 65536.0f);

            func_overlay_054_F0000504_189F1A4(sp12C, 1, &sp124, &sp120);
            if ((O54_S32(next, 8) >> 16) == 1) {
                O54_S16(next, 0xC) = O54_S16(D_78, 0) + sp124 + 1;
            } else {
                O54_S16(next, 0xC) = O54_S16(D_78, 0) + sp124;
            }
            if ((O54_S32(next, 0x18) >> 16) == 1) {
                O54_S16(next, 0x1C) = O54_S16(D_88, 0) + sp124 - 1;
            } else {
                O54_S16(next, 0x1C) = O54_S16(D_88, 0) + sp124;
            }
            O54_S32(small, 8) = O54_U8(record, 0x3BA) == 0xFF
                                   ? O54_U8(record, 0x385) << 16
                                   : O54_U8(record, 0x3BA) << 16;
            func_overlay_054_F0000000_189ECA0(O54_PTR(record, 0x400), &spFC,
                                               &spF8, &spF4);
            O54_S32(large, 8) = (spFC / 10) << 16;
            O54_S32(large, 0x18) = (spFC % 10) << 16;
            O54_S32(large, 0x38) = (spF8 / 10) << 16;
            O54_S32(large, 0x48) = (spF8 % 10) << 16;
            O54_S32(large, 0x68) = (spF4 / 10) << 16;
            O54_S32(large, 0x78) = (spF4 % 10) << 16;

            func_overlay_054_F0000504_189F1A4(sp12C, 3, &sp124, &sp120);
            cursor = D_CC;
            next = large;
            for (i = 0; i < 4; i++) {
                if ((O54_S32(next, 8) >> 16) == 1) {
                    O54_S16(next, 0xC) = O54_S16(cursor, 0xC) + sp124 +
                        ((cursor == D_CC || cursor == D_FC || cursor == D_12C) ? 1 : -1);
                } else {
                    O54_S16(next, 0xC) = O54_S16(cursor, 0xC) + sp124;
                }
                cursor += 0x10;
                next += 0x10;
            }

            if (O54_U32(record, 0x1A8) & 8) {
                func_overlay_054_F0000000_189ECA0(0, D_140 + sp12C * 0x20,
                                                   0, spF0, 0xFF, 0xFF, 0xFF, 0xFF);
            } else {
                func_overlay_054_F0000000_189ECA0(0, small, 0, spF0,
                                                   0xFF, 0xFF, 0xFF, 0xFF);
            }
            flags = func_overlay_054_F0000000_189ECA0(sp12C);
            if (flags & 2) {
                O54_S32(D_298 + sp12C * 4, 0)++;
                if (O54_S32(D_298 + sp12C * 4, 0) >= 2) {
                    O54_S32(D_298 + sp12C * 4, 0) = 0;
                }
            }
            option = O54_S32(D_298 + sp12C * 4, 0);
            switch (option) {
            case 0:
                func_overlay_054_F0000000_189ECA0(0, D_280 + sp12C * 0x30,
                                                   0, spF0, 0xFF, 0xFF, 0xFF, 0xFF);
                func_overlay_054_F0000000_189ECA0(0, D_1C0 + sp12C * 0x30,
                                                   0, spF0, 0xFF, 0xFF, 0xFF, 0xFF);
                func_overlay_054_F0000504_189F1A4(sp12C, 1, &sp124, &sp120);
                O54_F32(D_2A8, 0x24) = (f32)(sp124 - 0x98);
                O54_F32(D_2A8, 0x28) = (f32)((-sp120 - spF0) + 0x6C);
                func_overlay_054_F0000000_189ECA0(0);
                func_overlay_054_F0000000_189ECA0(1);
                func_overlay_054_F0000000_189ECA0(1);
                break;
            case 1:
                func_overlay_054_F0000000_189ECA0(0, large, 0, spF0,
                                                   0xFF, 0xFF, 0xFF, 0xFF);
                func_overlay_054_F0000000_189ECA0(0);
                func_overlay_054_F0000504_189F1A4(sp12C, 3, &sp124, &sp120);
                O54_F32(D_2A8, 0x8C) = (f32)(sp124 - 0xAD);
                O54_F32(D_2A8, 0x90) = (f32)((-sp120 - spF0) + 0x74);
                O54_F32(D_2A8, 0x84) =
                    (f32)((O54_S32(record, 0x400) * -0x10000) / 300);
                func_overlay_054_F0000000_189ECA0(4);
                func_overlay_054_F0000000_189ECA0(0, D_5C0 + sp12C * 0x20,
                                                   0, spF0, 0xFF, 0xFF, 0xFF, 0xFF);
                break;
            }

            if (O54_U8(record, 0x19A) != 0xFF) {
                newValue = O54_S32(D_2B4 + sp12C * 4, 0) + arg0 * 0x10;
                if (newValue >= 0xA5) {
                    newValue = 0xA4;
                }
            } else {
                newValue = O54_S32(D_2B4 + sp12C * 4, 0) - arg0 * 8;
                if (newValue < 0) {
                    newValue = 0;
                }
            }
            O54_S32(D_2B4 + sp12C * 4, 0) = newValue;
            if (newValue > 0) {
                O54_S8(D_654, sp12C) = O54_U8(record, 0x19C) != 0
                    ? 0x35 : (O54_U8(record, 0x19A) == 0xFF
                        ? O54_S8(D_654, sp12C) : O54_S8(D_0, O54_U8(record, 0x19A) * 2));
                if (O54_S8(D_654, sp12C) != -1) {
                    x = (sp12C & 1) ? 0x114 : 0x19;
                    y = (sp12C & 2) ? 0xC5 : 0x59;
                    if (func_overlay_054_F0000000_189ECA0() == 1) {
                        y = (sp12C & 2) ? 0xD1 : 0x59;
                    }
                    if (O54_S8(D_654, sp12C) == 0x35) {
                        x -= 7;
                        y -= 6;
                    }
                    frame = O54_S32(D_0, O54_S8(D_654, sp12C) * 4);
                    sp100 = O54_S32(D_2B4 + sp12C * 4, 0);
                    func_overlay_054_F0000000_189ECA0(0, &frame, x, y,
                                                       0, 0, ~0xFF, 1);
                    if (O54_S8(D_654, sp12C) != 0x35 && O54_U8(record, 0x19B) >= 2) {
                        O54_S32(D_278, 8) = O54_U8(record, 0x19B) << 16;
                        func_overlay_054_F0000000_189ECA0(0, D_278, x + 0x12,
                                                           y + 0x12, 0, 0, 0, sp100);
                        func_overlay_054_F0000000_189ECA0(0, D_278, x + 0x14,
                                                           y + 0x14, 0, 0, 0, sp100);
                        func_overlay_054_F0000000_189ECA0(0, D_278, x + 0x13,
                                                           y + 0x13, 0xFF, 0xFF, 0xFF, 0xFF);
                    }
                }
            } else {
                O54_S8(D_654, sp12C) = -1;
            }

            mode = sp12C & 1;
            x = O54_S16(D_1E8 + mode * 2, 0);
            y = O54_S16(D_218 + mode * 2, 0);
            z = O54_S16(D_248 + mode * 2, 0);
            active = O54_U8(record, 0x388) != 0;
            if (active) {
                for (i = 0; i < arg0; i++) {
                    O54_S16(D_660 + mode * 2, 0) +=
                        (s16)((x - O54_S16(D_660 + mode * 2, 0)) >> 3);
                }
                flags = 1;
            } else if (y != O54_S16(D_660 + mode * 2, 0)) {
                for (i = 0; i < arg0; i++) {
                    O54_S16(D_660 + mode * 2, 0) +=
                        (s16)((y - O54_S16(D_660 + mode * 2, 0)) >> 3);
                }
                flags = (y >> 6) == (O54_S16(D_660 + mode * 2, 0) >> 6);
                if (flags) {
                    O54_S16(D_660 + mode * 2, 0) = z;
                }
            } else {
                flags = 0;
            }
            if (flags) {
                O54_S16(D_AC, 0) = O54_S16(D_660 + mode * 2, 0) >> 4;
                if (func_overlay_054_F0000000_189ECA0(x, y, flags, z) == 1) {
                    O54_S16(D_AC, 2) = sp12C < 2 ? 0x28 : 0xA0;
                } else {
                    O54_S16(D_AC, 2) = sp12C < 2 ? 0x2D : 0x99;
                }
                func_overlay_054_F0000000_189ECA0(0, D_A0, 0, 0,
                                                   0xFF, 0xFF, 0xFF, 0xC0);
            }

            if (mode != 0) {
                O54_S32(D_10, 8) = (spFC / 10) << 16;
                O54_S32(D_10, 0x18) = (spFC % 10) << 16;
                O54_S32(D_10, 0x28) = (spF8 / 10) << 16;
                O54_S32(D_10, 0x48) = (spF8 % 10) << 16;
                O54_S32(D_10, 0x78) = (spF4 / 10) << 16;
                O54_S32(D_10, 0x88) = (spF4 % 10) << 16;
            }
            object = (u8 *)func_overlay_054_F0000000_189ECA0();
            if (O54_U8(record, 0x383) < O54_U8(object, 0x86)) {
                frame = O54_S16(record, 0x456);
                x = O54_U8(record, 0) * 2;
                y = (mode != 0) ? 0xA00 : 0x100;
                if (func_overlay_054_F0000000_189ECA0() == 1) {
                    z = (spB0 >> 1) * ((O54_S8(record, 0) >> 1) * 0x10);
                } else {
                    z = (sp12C < 2) ? 0xC0 : 0x780;
                }
                if (frame >= arg0) {
                    O54_S16(record, 0x456) = (s16)(frame - arg0);
                    for (i = 0; i < arg0; i++) {
                        oldValue = O54_S16(D_640 + x, 0);
                        O54_S16(D_640 + x, 0) = oldValue +
                            ((y - oldValue + 0x330) >> 3);
                        oldValue = O54_S16(D_648 + x, 0);
                        O54_S16(D_648 + x, 0) = oldValue +
                            ((z - oldValue + 0x230) >> 3);
                    }
                } else {
                    if (frame != -1 && O54_S16(record, 0x454) < 0) {
                        func_overlay_054_F0000000_189ECA0(0x1F9, 0);
                        O54_S16(record, 0x456) = -1;
                    }
                    for (i = 0; i < arg0; i++) {
                        oldValue = O54_S16(D_640 + x, 0);
                        O54_S16(D_640 + x, 0) = oldValue +
                            (((spB4 * 8 + y) - oldValue + 0x330) >> 3);
                        oldValue = O54_S16(D_648 + x, 0);
                        O54_S16(D_648 + x, 0) = oldValue +
                            ((z - oldValue - 0x190) >> 3);
                    }
                    if ((u32)(y + spB4 * 8 + 0x290) <
                        (u32)O54_S16(D_640 + x, 0)) {
                        O54_S16(D_640 + x, 0) = (s16)(y - 0x500);
                        O54_S16(D_648 + x, 0) = (s16)(z - 0x140);
                    }
                }
            }
            if (O54_S16(record, 0x454) <= 0) {
                O54_S32(D_8, 0) = 0xC0000;
                oldValue = O54_S32(D_50, 0);
                state = O54_S32(D_54, 0);
                for (cursor = D_10; cursor < D_90; cursor += 0x40) {
                    O54_S32(cursor, 0x10) = oldValue;
                    O54_S32(cursor, 0x14) = state;
                    O54_S32(cursor, 0x20) = oldValue;
                    O54_S32(cursor, 0x24) = state;
                    O54_S32(cursor, 0x30) = oldValue;
                    O54_S32(cursor, 0x34) = state;
                    O54_S32(cursor, 0x0) = oldValue;
                    O54_S32(cursor, 0x4) = state;
                }
            } else {
                O54_S32(D_8, 0) = 0xD0000;
                oldValue = O54_S32(D_140, 0);
                state = O54_S32(D_54, 0);
                for (cursor = D_10; cursor < D_90; cursor += 0x40) {
                    O54_S32(cursor, 0x10) = oldValue;
                    O54_S32(cursor, 0x14) = state;
                    O54_S32(cursor, 0x20) = oldValue;
                    O54_S32(cursor, 0x24) = state;
                    O54_S32(cursor, 0x30) = oldValue;
                    O54_S32(cursor, 0x34) = state;
                    O54_S32(cursor, 0x0) = oldValue;
                    O54_S32(cursor, 0x4) = state;
                }
            }
            func_overlay_054_F0000000_189ECA0(state, &spFC, &spF8, &spF4);
            cursor = D_1E8;
            next = D_10;
            while (cursor < D_268) {
                O54_S16(next, 0xC) = O54_S16(cursor, 0xC) +
                    ((O54_S32(next, 8) >> 16) == 1 ? 1 : 0);
                cursor += 0x30;
                next += 0x10;
            }
            func_overlay_054_F0000000_189ECA0(0, 0,
                O54_S16(D_640 + mode * 2, 0) >> 4,
                O54_S16(D_648 + mode * 2, 0) >> 4,
                0xFF, 0xFF, 0xFF, 0xFF);
            if (func_overlay_054_F0000000_189ECA0(sp12C) & 1) {
                O54_S32(D_2A8, 0) ^= 1;
            }
            if (func_overlay_054_F0000000_189ECA0() == 0) {
                flags = 0;
                for (i = 0; i < count; i++) {
                    flags |= func_overlay_054_F0000000_189ECA0(i) & 0x9000;
                }
                mode = O54_U8(D_2A8, 4);
                if ((mode == 3 || mode == 4) && O54_S32(D_2A8, 0) == 0 &&
                    flags != 0 && O54_S32(D_2A8, 4) == 0) {
                    func_overlay_054_F0000000_189ECA0(1);
                    func_overlay_054_F0000000_189ECA0(1);
                    func_overlay_054_F0000000_189ECA0();
                    func_overlay_054_F0000000_189ECA0(2, 0x40800000,
                                                       0xBF800000, 0, 0, 0, 0);
                    func_overlay_054_F0000000_189ECA0(0x12, 0, 0, 7, 1, 1);
                    func_overlay_054_F0000000_189ECA0(0x40400000, 0);
                    O54_S32(D_2A8, 4) = 1;
                }
            }
        }
        sp12C++;
    }
    func_overlay_054_F0000000_189ECA0(0);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o054/overlay54TailA/func_overlay_054_F00005AC_189F24C.s")
#endif
