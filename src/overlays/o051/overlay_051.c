#include "overlays/overlay_051.h"

/*
 * Overlay 51, ADR 0006 consolidation: one translation unit in ROM order.
 * The pinned DKR v77/v80 and JFG scans found no exact donor for the matched
 * C functions; the unresolved middle function remains GLOBAL_ASM.
 */

void overlay51Initialize(void) {
    overlay51CreateReloc(gOverlay51Resource0);
    overlay51CreateReloc(gOverlay51Resource18);
    overlay51CreateReloc(4);
    overlay51CreateReloc(11);
    overlay51PrepareReloc(gOverlay51ResourceBC);
    overlay51PrepareReloc(gOverlay51Resource1C);
    gOverlay51InitialValue = -80.0f;
    overlay51CreateReloc();
    gOverlay51Mode = -1;
    gOverlay51Handle = overlay51CreateReloc();
}

void overlay51PatchIndices(OverlayPatchIndexEntry *entry) {
    while (entry->first != 0) {
        entry->first = (s32) gOverlay51Objects[entry->first];
        if (entry->second != 0) {
            entry->second = (s32) gOverlay51Objects[entry->second];
        }
        entry++;
    }
}

/* Workbench: structure-mismatch; 476 words differ, first mismatch +0x0. */
/* Candidate is not shape-exact: 481/482 instructions and frame -184/-208 bytes. */
/* Remaining gap is raw-address/relocation modeling; this is not a matched function. */
#ifdef NON_MATCHING
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((u8 *)(expr) + (offset)))

#define O51_VALUE32 (*(s32 *)D_0)
#define O51_VALUEF32 (*(f32 *)D_0)

extern u8 *func_overlay_051_F0000000_1899900();
extern u8 D_0[];
extern s32 D_1C;
extern s32 D_BC;
extern s8 D_DC;
extern s8 D_E0;
extern s32 D_E4;

typedef struct {
    s32 value0;
    s32 value4;
    s32 value8;
    s16 valueC;
    s16 valueE;
    s32 value10;
} O51CallState;

void func_overlay_051_F00000D0_18999D0(void *arg0, s32 arg1) {
    s32 *spC8;
    s32 *spC4;
    volatile s32 spC0;
    s32 sp70;
    O51CallState sp50;
    void *sp48;
    u8 *sp44;
    s32 sp40;
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp28;
    f32 var_f16;
    f32 var_f16_2;
    s16 var_v0;
    f32 temp_f10;
    f32 temp_f10_2;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f14_3;
    f32 temp_f14_4;
    s32 temp_v0;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s8 temp_a0;
    s8 temp_v0_3;
    u8 *temp_v0_2;
    u8 temp_v1;

    sp44 = func_overlay_051_F0000000_1899900();
    if (arg0 != NULL) {
        sp48 = M2C_FIELD(arg0, void **, 0x64);
        func_overlay_051_F0000000_1899900(gOverlay51Resource0, &gOverlay51InitialValue);
        if (O51_VALUE32 == 0) {
            var_v1 = 0;
            if (arg1 > 0) {
                temp_v0 = arg1 & 3;
                if (temp_v0 != 0) {
                    var_v1 += 1;
                    var_f16 = (0.0f - O51_VALUEF32) * 0.125f;
                    if (temp_v0 != var_v1) {
                        O51_VALUEF32 += var_f16;
rem_loop:
                        var_v1 += 1;
                        var_f16 = (0.0f - O51_VALUEF32) * 0.125f;
                        if (temp_v0 != var_v1) {
                            O51_VALUEF32 += var_f16;
                            goto rem_loop;
                        }
                    }
                    O51_VALUEF32 += var_f16;
                    if (var_v1 != arg1) {
                        goto block_7;
                    }
                } else {
block_7:
                    var_v1_2 = var_v1 + 4;
                    var_f16_2 = (0.0f - O51_VALUEF32) * 0.125f;
                    if (var_v1_2 != arg1) {
                        O51_VALUEF32 += var_f16_2;
full_loop:
                        var_v1_2 += 4;
                        temp_f14 = O51_VALUEF32;
                        O51_VALUEF32 = temp_f14 + ((0.0f - temp_f14) * 0.125f);
                        temp_f10 = O51_VALUEF32;
                        O51_VALUEF32 = temp_f10 + ((0.0f - temp_f10) * 0.125f);
                        temp_f14_2 = O51_VALUEF32;
                        O51_VALUEF32 = temp_f14_2 + ((0.0f - temp_f14_2) * 0.125f);
                        var_f16_2 = (0.0f - O51_VALUEF32) * 0.125f;
                        if (var_v1_2 != arg1) {
                            O51_VALUEF32 += var_f16_2;
                            goto full_loop;
                        }
                    }
                    O51_VALUEF32 += var_f16_2;
                    temp_f14_3 = O51_VALUEF32;
                    O51_VALUEF32 = temp_f14_3 + ((0.0f - temp_f14_3) * 0.125f);
                    temp_f10_2 = O51_VALUEF32;
                    O51_VALUEF32 = temp_f10_2 + ((0.0f - temp_f10_2) * 0.125f);
                    temp_f14_4 = O51_VALUEF32;
                    O51_VALUEF32 = temp_f14_4 + ((0.0f - temp_f14_4) * 0.125f);
                }
            }
        }
        sp34 = (s32) *(f32 *)gOverlay51Resource0;
        if (*sp44 == 6) {
            func_overlay_051_F0000000_1899900(M2C_FIELD(sp48, s32 *, 0x400), &sp40, &sp3C, &sp38);
            temp_v0_2 = func_overlay_051_F0000000_1899900();
            if ((O51_VALUE32 == 0) && (M2C_FIELD(temp_v0_2, s8 *, 0x86) != M2C_FIELD(sp48, s8 *, 0x383)) && (func_overlay_051_F0000000_1899900() == NULL) && (M2C_FIELD(sp48, s32 *, 0x400) != 0x83D60)) {
                temp_v0_3 = D_DC;
                D_DC = temp_v0_3 + 1;
                sp38 = (sp38 - (sp38 % 10)) + temp_v0_3;
                D_DC = (s8) ((s8) D_DC % 10);
            }
            M2C_FIELD(&D_1C, s32 *, 8) = (s32) ((sp40 / 10) << 0x10);
            M2C_FIELD(&D_1C, s32 *, 0x18) = (s32) ((sp40 % 10) << 0x10);
            M2C_FIELD(&D_1C, s32 *, 0x38) = (s32) ((sp3C / 10) << 0x10);
            M2C_FIELD(&D_1C, s32 *, 0x48) = (s32) ((sp3C % 10) << 0x10);
            M2C_FIELD(&D_1C, s32 *, 0x68) = (s32) ((sp38 / 10) << 0x10);
            M2C_FIELD(&D_1C, s32 *, 0x78) = (s32) ((sp38 % 10) << 0x10);
            func_overlay_051_F0000000_1899900(gOverlay51Resource0, &D_1C, NULL, (s32 *) sp34, 0xFF, (s32 **)0xFF, (s32 **)0xFF, 0xFF);
            func_overlay_051_F0000000_1899900(gOverlay51Resource0);
            *(f32 *)0x8C = 42.0f;
            *(f32 *)0x90 = (f32) (0x54 - sp34);
            *(s16 *)0x84 = (s16) ((s32) (M2C_FIELD(sp48, s32 *, 0x400) * -0x10000) / 300);
            func_overlay_051_F0000000_1899900(4);
            func_overlay_051_F0000000_1899900(gOverlay51Resource0, &D_BC, NULL, (s32 *) sp34, 0xFF, (s32 **)0xFF, (s32 **)0xFF, 0xFF);
            func_overlay_051_F0000000_1899900(gOverlay51Resource0);
        }
        if (M2C_FIELD(sp48, u8 *, 0x19A) != (u8) 0xFF) {
            var_v1_3 = D_E4 + (arg1 * 0x10);
            D_E4 = var_v1_3;
            if (var_v1_3 >= 0x100) {
                D_E4 = 0xFF;
                var_v1_3 = 0xFF;
            }
        } else {
            var_v1_3 = D_E4 - (arg1 * 8);
            D_E4 = var_v1_3;
            if (var_v1_3 < 0) {
                D_E4 = 0;
                var_v1_3 = 0;
            }
        }
        if (var_v1_3 > 0) {
            if (M2C_FIELD(sp48, s32 *, 0x19C) != 0) {
                var_v0 = 0x35;
                sp28 = (s32) *(s8 *)0xE0;
            } else {
                temp_v1 = M2C_FIELD(sp48, u8 *, 0x19A);
                if (temp_v1 != (u8) 0xFF) {
                    var_v0 = ((s16 *)gOverlay51Resource0)[temp_v1];
                    sp28 = (s32) *(s8 *)0xE0;
                } else {
                    var_v0 = (s16) *(s8 *)0xE0;
                    sp28 = (s32) var_v0;
                }
            }
            if (var_v0 != sp28) {
                if (sp28 != -1) {
                    spC0 = (s32) func_overlay_051_F0000000_1899900(sp28);
                    var_v0 = (s16) spC0;
                }
                D_E0 = (s8) var_v0;
                sp28 = (s32) D_E0;
                if (D_E0 != -1) {
                    func_overlay_051_F0000000_1899900((s32) D_E0);
                    sp28 = (s32) *(s8 *)0xE0;
                }
            }
            if (sp28 != -1) {
                if (sp28 == 0x35) {
                    sp50.valueC = 0x8A;
                    sp50.valueE = 0xF;
                } else {
                    sp50.valueC = 0x90;
                    sp50.valueE = 0x15;
                }
                sp50.value8 = 0;
                sp50.value4 = 0;
                sp50.value10 = 0;
                sp50.value0 = ((s32 *)gOverlay51Resource0)[sp28];
                func_overlay_051_F0000000_1899900(gOverlay51Resource0, &sp50.value0, NULL, NULL, 0xFF, (s32 **)0xFF, (s32 **)0xFF, *(s32 *)0xE4);
            }
        } else {
            temp_a0 = *(s8 *)0xE0;
            if (temp_a0 != -1) {
                func_overlay_051_F0000000_1899900((s32) temp_a0);
                *(s8 *)0xE0 = -1;
            }
        }
        if (func_overlay_051_F0000000_1899900(0, (s32 *)-0x18, (s32 *)0xBE, (s32 *)0x30, 0xBE, &spC8, &spC4, 1) != NULL) {
            func_overlay_051_F0000000_1899900(0, &sp70);
            if (sp70 != 0) {
                func_overlay_051_F0000000_1899900(gOverlay51Resource0, &sp70, spC8, spC4, 0xFF, (s32 **)0xFF, (s32 **)0xFF, 0xFF);
            }
        }
        if ((func_overlay_051_F0000000_1899900() == NULL) && (*sp44 == 5) && (O51_VALUE32 == 0) && (*(volatile s32 *)0xF0 == 0)) {
            func_overlay_051_F0000000_1899900(1);
            func_overlay_051_F0000000_1899900();
            func_overlay_051_F0000000_1899900(2, (s32 *)0x40800000, (s32 *)0xBF800000, NULL, 0, NULL, NULL);
            func_overlay_051_F0000000_1899900(0x12, NULL, NULL, (s32 *)7, 1, (s32 **)1);
            func_overlay_051_F0000000_1899900(3.0f, NULL);
            *(volatile s32 *)0xF0 = 1;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o051/overlay_051/func_overlay_051_F00000D0_18999D0.s")
#endif

void overlay51ReleaseState(void) {
    s32 index;

    overlay51ReleaseReloc(gOverlay51InlineResource);
    overlay51FinalizeReloc();
    index = gOverlay51Index;
    if (index != -1) {
        overlay51ReleaseIndexReloc(index);
        gOverlay51Index = -1;
    }
}
