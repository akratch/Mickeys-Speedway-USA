#include "overlays/overlay_056.h"

/*
 * Overlay 56, ADR 0006 consolidation: one translation unit in ROM order.
 * Exact DKR v77/v80 and JFG scans are negative for the matched C functions;
 * the unresolved middle function remains GLOBAL_ASM.
 */

void overlay56OffsetCoordinates(u32 *x, u32 *y) {
    u32 width;
    u32 height;

    overlay56GetDimensionsReloc(&width, &height);
    *x += width >> 1;
    *y = (height >> 1) - *y;
}

void overlay56CenterCoordinates(s32 *x, s32 *y) {
    u32 width;
    u32 height;

    overlay56GetDimensionsReloc(&width, &height);
    *x -= width >> 1;
    *y = (height >> 1) - *y;
}

void overlay56SplitTime(s32 value, s32 *minutes, s32 *seconds,
                        s32 *centiseconds) {
    s32 wholeMinutes;
    wholeMinutes = value / 18000;
    *minutes = wholeMinutes;
    *seconds = (value / 300) - (wholeMinutes * 60);
    *centiseconds = (value / 3) % 100;
}

void overlay56SetMode(s32 mode) {
    gOverlay56Mode = mode;
}

void overlay56LoadResource(void) {
    Overlay56Context *context;

    context = overlay56GetContextReloc();
    if (context->resourceId != -1) {
        gOverlay56Resource = overlay56LoadResourceReloc(context->resourceId);
    } else {
        gOverlay56Resource = 0;
    }
    gOverlay56ResourceState = 0;
}

void overlay56ReleaseResource(void) {
    if (gOverlay56Resource != 0) {
        overlay56ReleaseResourceReloc(gOverlay56Resource);
        gOverlay56Resource = 0;
    }
}

/* Workbench: structure-mismatch; 578 words differ, first mismatch +0x0. */
/* Candidate is not shape-exact: 584/581 instructions; frame -504/-504 bytes. */
/* Remaining gap is structural CFG/type lowering plus relocation identity. */
#ifdef NON_MATCHING
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((u8 *)(expr) + (offset)))

#define O56_GLOBAL32 (*(volatile s32 *)0)
#define O56_GLOBAL16 (*(volatile s16 *)0)
#define O56_GLOBAL8 (*(volatile u8 *)0)
#define O56_RAW58 (*(volatile void **)0x58)

extern u8 *func_overlay_056_F0000000_18A2D78();
extern f32 overlay56FloatCall(s16);
extern s16 D_78[];
extern s16 D_84[];
extern u8 D_80000004[];
extern u8 D_80000030[];

typedef struct {
    s32 value0;
    s32 value4;
    s32 value8;
    s16 valueC;
    s16 valueE;
    s32 value10;
    u8 pad[112];
} Overlay56CallState;

void func_overlay_056_F00001A0_18A2F18(void **arg0, s32 *arg1, s32 arg2) {
    u32 sp1D4;
    f32 sp1B4;
    u32 sp168;
    u32 sp128;
    void *spE8;
    u32 spE4;
    s32 spE0;
    Overlay56CallState spC0State;
    u8 *spBC;
    u8 *spB4;
    s32 spB0;
    u32 spAC;
    s32 spA8;
    s32 sp8C;
    s16 *sp7C;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 var_f10;
    f32 var_f16;
    f32 var_f16_2;
    f32 var_f6;
    f32 var_f6_2;
    s16 *temp_t5;
    s16 *var_s6;
    s32 temp_t5_2;
    u8 **temp_v0_4;
    u8 **temp_v0_5;
    s32 temp_v1_3;
    s32 var_a0;
    s32 var_s1;
    s32 var_s2;
    s32 var_s2_2;
    s32 var_s7;
    s32 var_v1;
    u32 temp_t4;
    u32 temp_t7;
    u32 temp_v0_3;
    u32 temp_v0_6;
    u32 var_a2;
    u8 *temp_s1;
    u8 *temp_s3;
    u8 *temp_v0;
    u8 temp_a0;
    u8 temp_v1;
    u8 temp_v1_4;
    u8 **var_s5;
    void *temp_fp;
    u8 *temp_s0;
    u8 *temp_s1_2;
    u8 *temp_t3;
    u8 *temp_t5_3;
    u8 *temp_t6;
    u8 *temp_v0_2;
    u8 *temp_v1_2;
    u8 *temp_v1_5;
    u8 *var_s0;
    temp_v0 = func_overlay_056_F0000000_18A2D78();
    spBC = temp_v0;
    if (O56_GLOBAL32 == 0) {
        temp_a0 = O56_GLOBAL8;
        if (temp_a0 == 3) {
            var_v1 = 2;
        } else {
            var_v1 = 1;
        }
        O56_GLOBAL16 = (s16) (O56_GLOBAL16 + (arg2 << var_v1));
        if ((temp_a0 != 3) && (O56_GLOBAL16 >= 0xA1)) {
            O56_GLOBAL16 = 0xA0;
        } else if (O56_GLOBAL16 >= 0x100) {
            O56_GLOBAL16 = 0xFF;
        }
    }
    if ((O56_GLOBAL32 != 0) &&
        ((O56_GLOBAL32 == 0) ||
         ((temp_v1 = *temp_v0, (temp_v1 != 0)) &&
          (temp_v1 != 2) && (temp_v1 != 1) && (temp_v1 != 3) &&
          (temp_v1 != 4)))) {
        spB4 = *arg0;
        spB0 = *arg1;
        func_overlay_056_F0000000_18A2D78(&spAC, &spA8);
        temp_t6 = spB4;
        spB4 = temp_t6 + 8;
        M2C_FIELD(temp_t6, u32 *, 4) = 0;
        M2C_FIELD(temp_t6, u32 *, 0) = 0xE7000000;
        temp_v1_2 = spB4;
        spB4 = temp_v1_2 + 8;
        M2C_FIELD(temp_v1_2, u32 *, 0) = 0xED000000;
        var_f6 = (f32) spA8;
        if (spA8 < 0) {
            var_f6 += 4294967296.0f;
        }
        M2C_FIELD(temp_v1_2, u32 *, 4) =
            (u32) ((((s32) ((f32) spAC * 4.0f) & 0xFFF) << 0xC) |
                   ((s32) (var_f6 * 4.0f) & 0xFFF));
        func_overlay_056_F0000000_18A2D78((u32 *) &spB4, &spB0);
        temp_s1 = func_overlay_056_F0000000_18A2D78(&sp1D4);
        temp_s3 = func_overlay_056_F0000000_18A2D78();
        spC0State.value4 = 0;
        spC0State.value0 = O56_GLOBAL32;
        if ((O56_GLOBAL8 == 2) &&
            (var_a0 = 4,
             func_overlay_056_F0000000_18A2D78() != NULL)) {
        } else {
            var_a0 = O56_GLOBAL8 - 1;
        }
        temp_v0_2 = *(volatile u8 **)0;
        temp_v1_3 = var_a0;
        temp_t5 = &D_84[temp_v1_3];
        sp7C = temp_t5;
        spC0State.valueE = (*temp_t5 -
                            ((s32) M2C_FIELD(temp_v0_2, u16 *, 8) >> 1)) + 0xB4;
        spC0State.value10 = 0;
        if (O56_GLOBAL8 != 0) {
            var_s6 = &D_78[temp_v1_3];
            spC0State.valueC = (-*var_s6 -
                    ((s32) M2C_FIELD(temp_v0_2, u16 *, 6) >> 1)) + 0x3C;
            func_overlay_056_F0000000_18A2D78(
                (u32 *) &spB4, &spC0State.value0,
                (void **) 0x43A00000, NULL,
                0x3F800000U, 0x3F800000U, O56_GLOBAL16 | ~0xFF, 0x1002U);
        } else {
            var_s6 = &D_78[temp_v1_3];
            spC0State.valueC = (*var_s6 -
                    ((s32) M2C_FIELD(temp_v0_2, u16 *, 6) >> 1)) + 0x104;
            func_overlay_056_F0000000_18A2D78(
                (u32 *) &spB4, &spC0State.value0, NULL, NULL,
                0xFFU, 0xFFU, 0xFFU,
                (u32) O56_GLOBAL16);
        }
        func_overlay_056_F0000000_18A2D78(&spE4, &spE0);
        temp_fp = O56_RAW58;
        temp_f22 = overlay56FloatCall(
            M2C_FIELD(temp_s3, s16 *, 0x122));
        temp_f24 = overlay56FloatCall(
            M2C_FIELD(temp_s3, s16 *, 0x122));
        temp_t5_2 = (s16) O56_GLOBAL16 * 2;
        var_s7 = temp_t5_2;
        if (temp_t5_2 >= 0x100) {
            var_s7 = 0xFF;
        }
        spC0State.value4 = 0;
        spC0State.value10 = 0;
        var_s2 = sp1D4 - 1;
        if (sp1D4 != 0) {
            var_s5 = (u8 **)(temp_s1 + (var_s2 * 4));
            do {
                temp_s0 = *var_s5;
                temp_f2 = M2C_FIELD(temp_s3, f32 *, 0x128);
                temp_v1_4 = O56_GLOBAL8;
                temp_f0 = M2C_FIELD(temp_s0, f32 *, 0xC) * temp_f2;
                temp_f12 = M2C_FIELD(temp_s0, f32 *, 0x14) * temp_f2;
                temp_s1_2 = M2C_FIELD(temp_s0, u8 **, 0x64);
                temp_f14 = (temp_f0 * temp_f22) - (temp_f12 * temp_f24);
                var_f16 = temp_f14;
                temp_f20 = (temp_f12 * temp_f22) + (temp_f0 * temp_f24);
                if (temp_v1_4 != 0) {
                    var_f16 = -temp_f14;
                }
                if (var_s2 < (s32) O56_GLOBAL8) {
                    sp1B4 = var_f16;
                    func_overlay_056_F0000000_18A2D78(
                        (u32 *)(s32) temp_f12, (s32 *)(s32) temp_f14, &spB4,
                        NULL, 5U,
                        0U);
                    func_overlay_056_F0000000_18A2D78(&sp168);
                    if (O56_GLOBAL8 != 0) {
                        var_f16_2 = sp1B4 +
                            (f32) ((*var_s6 -
                                    M2C_FIELD(temp_s3, s16 *, 0x124)) +
                                   0x208);
                    } else {
                        var_f16_2 = sp1B4 +
                            (f32) (M2C_FIELD(temp_s3, s16 *, 0x124) +
                                   *var_s6);
                    }
                    temp_t7 = spE4 >> 1;
                    temp_t4 = (u32) spE0 >> 1;
                    var_f6_2 = (f32) temp_t7;
                    if ((s32) temp_t7 < 0) {
                        var_f6_2 += 4294967296.0f;
                    }
                    var_f10 = (f32) temp_t4;
                    if ((s32) temp_t4 < 0) {
                        var_f10 += 4294967296.0f;
                    }
                    func_overlay_056_F0000000_18A2D78(
                        (u32 *)(s32) (var_f16_2 - var_f6_2),
                        (s32 *)(s32) (var_f10 -
                                 (temp_f20 +
                                  (f32) (M2C_FIELD(temp_s3, s16 *, 0x126) +
                                         *sp7C))),
                        NULL, &sp168);
                    if (O56_GLOBAL8 != 0) {
                        func_overlay_056_F0000000_18A2D78(
                            (u32 *) (s16) ((M2C_FIELD(temp_s0, s16 *, 0) -
                                            M2C_FIELD(temp_s3, s16 *, 0x122)) *
                                           -1),
                            (s32 *) &sp168);
                    } else {
                        func_overlay_056_F0000000_18A2D78(
                            (u32 *) (s16) (M2C_FIELD(temp_s0, s16 *, 0) -
                                           M2C_FIELD(temp_s3, s16 *, 0x122)),
                            (s32 *) &sp168);
                    }
                    func_overlay_056_F0000000_18A2D78(&sp128);
                    func_overlay_056_F0000000_18A2D78(
                        &sp168, (s32 *) &sp128, &spE8);
                    func_overlay_056_F0000000_18A2D78(
                        (u32 *) &spE8, (s32 *) spB0);
                    temp_t5_3 = spB4;
                    spB4 = temp_t5_3 + 8;
                    M2C_FIELD(temp_t5_3, u32 *, 0) = 0x01000040;
                    M2C_FIELD(temp_t5_3, u32 *, 4) = spB0 + 0x80000000;
                    temp_v0_2 = spB4;
                    spB0 += 0x40;
                    if (((s32) (M2C_FIELD(temp_s1_2, u8 *, 0x190) *
                               var_s7) >> 8) > 0) {
                        spB4 = temp_v0_2 + 8;
                        M2C_FIELD(temp_v0_2, u32 *, 0) = 0xFA000000;
                        M2C_FIELD(temp_v0_2, u32 *, 4) =
                            (M2C_FIELD((u8 *)(s32)(M2C_FIELD(temp_s1_2,
                                                             s8 *, 1) * 4),
                                        u32 *, 0x50) |
                             ((s32) (var_s7 *
                                     M2C_FIELD(temp_s1_2, u8 *, 0x190)) >>
                              8));
                        temp_v1_5 = spB4;
                        temp_t3 = temp_v1_5 + 8;
                        spB4 = temp_t3;
                        M2C_FIELD(temp_v1_5, u32 *, 0) =
                            (((((s32) D_80000004 & 6) | 0x20) & 0xFF) <<
                             0x10) | 0x04000000 | 0x30;
                        M2C_FIELD(temp_v1_5, u32 *, 4) = (u32) D_80000004;
                        spB4 = temp_t3 + 8;
                        M2C_FIELD(temp_t3, u32 *, 4) = (u32) D_80000030;
                        M2C_FIELD(temp_t3, u32 *, 0) = 0x05110020;
                    }
                } else {
                    spC0State.value0 = (s32) O56_RAW58;
                    spC0State.valueC = (s16) (s32) (var_f16 -
                                       (f32) ((s32) M2C_FIELD(temp_fp,
                                                               u16 *, 6) >> 1));
                    spC0State.valueE = (s16) (s32) (temp_f20 -
                                       (f32) ((s32) M2C_FIELD(temp_fp,
                                                               u16 *, 8) >> 1));
                    temp_v0_3 = M2C_FIELD(
                        (u8 *)(s32)(M2C_FIELD(temp_s1_2, s8 *, 1) * 4),
                        u32 *, 0x50);
                    if (temp_v1_4 != 0) {
                        var_a2 = (*var_s6 -
                                  M2C_FIELD(temp_s3, s16 *, 0x124)) + 0x208;
                    } else {
                        var_a2 = *var_s6 +
                                 M2C_FIELD(temp_s3, s16 *, 0x124);
                    }
                    func_overlay_056_F0000000_18A2D78(
                        (u32 *)(s32) temp_f12, (s32 *)(s32) temp_f14, &spB4,
                        (u32 *) &spC0State.value0, var_a2,
                        *sp7C + M2C_FIELD(temp_s3, s16 *, 0x126),
                        temp_v0_3 >> 0x18, (temp_v0_3 >> 0x10) & 0xFF,
                        (temp_v0_3 >> 8) & 0xFF,
                        (s32) (M2C_FIELD(temp_s1_2, u8 *, 0x190) * var_s7) >>
                            8);
                }
                var_s5 -= 4;
                var_s2 -= 1;
            } while (var_s2 != 0);
        }
        var_s2_2 = 1;
        if (*spBC == 1) {
            var_s1 = sp8C;
            do {
                temp_v0_4 = (u8 **)0;
                if ((var_s2_2 != 0) &&
                    (temp_v0_4 = *(u8 ***)0, temp_v0_4 != NULL)) {
                    var_s0 = *temp_v0_4;
                    var_s1 = 0xFF;
                } else {
                    var_s0 = NULL;
                    if (var_s2_2 == 0) {
                        temp_v0_5 = *(u8 ***)0;
                        if (temp_v0_5 != NULL) {
                            var_s0 = *temp_v0_5;
                            var_s1 = 0x55;
                        }
                    }
                }
                if (var_s0 != NULL) {
                    temp_f2_2 = M2C_FIELD(temp_s3, f32 *, 0x128);
                    temp_f0_2 = M2C_FIELD(var_s0, f32 *, 0xC) * temp_f2_2;
                    temp_f12_2 = M2C_FIELD(var_s0, f32 *, 0x14) * temp_f2_2;
                    spC0State.value0 = (s32) O56_RAW58;
                    temp_v0_6 = var_s1 & 0xFF;
                    spC0State.valueC = (s16) (s32) (((temp_f0_2 * temp_f22) -
                                         (temp_f12_2 * temp_f24)) -
                                        (f32) ((s32) M2C_FIELD(temp_fp,
                                                               u16 *, 6) >> 1));
                    spC0State.valueE = (s16) (s32) (((temp_f12_2 * temp_f22) +
                                         (temp_f0_2 * temp_f24)) -
                                        (f32) ((s32) M2C_FIELD(temp_fp,
                                                               u16 *, 8) >> 1));
                    func_overlay_056_F0000000_18A2D78(
                        (u32 *)(s32) temp_f12_2, (s32 *) &spB4,
                        &spC0State.value0,
                        *var_s6 + M2C_FIELD(temp_s3, s16 *, 0x124),
                        *sp7C + M2C_FIELD(temp_s3, s16 *, 0x126), temp_v0_6,
                        temp_v0_6, temp_v0_6,
                        (s32) (M2C_FIELD(M2C_FIELD(var_s0, u8 **, 0x64),
                                        u8 *, 0x0F) * var_s7) >> 7);
                }
                var_s2_2 -= 1;
            } while (var_s2_2 != 0);
            sp8C = var_s1;
        }
        *arg0 = spB4;
        *arg1 = spB0;
    }
}

#undef O56_RAW58
#undef O56_GLOBAL8
#undef O56_GLOBAL16
#undef O56_GLOBAL32
#undef M2C_FIELD
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o056/overlay_056/func_overlay_056_F00001A0_18A2F18.s")
#endif

void overlay56UnpackColor(s32 index, u32 *red, s32 *green, s32 *blue) {
    u32 *color = &gOverlay56Colors[index];
    *red = *color >> 24;
    *green = (*color >> 16) & 0xFF;
    *blue = (*color >> 8) & 0xFF;
}
