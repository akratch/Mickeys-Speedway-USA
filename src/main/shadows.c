/*
 * Shadow buffers and projection -- ROM 0x16A90-0x18FF0.
 *
 * PROVENANCE -- the TU attribution and the five descriptive names below are
 * borrowed from Jet Force Gemini's public retail-derived decompilation:
 * src/shadows.c and asm/nonmatchings/shadows/*.s.  They are supported here at
 * tier B by the same call graph and at tier D by function order and masked
 * instruction shape.  This is not a whole-object tier-A match; Mickey's ROM
 * remains the authority for every body.
 *
 * The matched leaf bodies below were reconstructed from Mickey's own
 * instructions and globals; no JFG body has been adapted.  The remaining
 * pragmas preserve the original ROM bytes.  JFG's address-placeholder helper
 * names are deliberately not imported.
 */

#include "PR/ultratypes.h"

/* Shadow buffer state occupies one contiguous compiler-owned .data input
 * section. Keep the retail labels at their measured offsets for all users;
 * the terminal halfword pair is part of IDO's measured 0x50-byte section. */
u8 *D_80079410[1] = { 0 };
u8 *D_80079414[3] = { 0 };
u8 *D_80079420[1] = { 0 };
u8 *D_80079424[3] = { 0 };
u8 *D_80079430[1] = { 0 };
u8 *D_80079434[3] = { 0 };
u8 *D_80079440 = 0;
u8 *D_80079444 = 0;
u8 *D_80079448 = 0;
s32 D_8007944C = 0;
s32 D_80079450 = 0;
s32 D_80079454 = 0;
s32 D_80079458 = 0;
u16 D_8007945C[2] = { 0, 0x4000 };
extern s32 D_800CB278;
extern s32 D_800CB27C;
extern s32 D_800CB280;
extern s32 D_800CB268;
extern s32 D_800CB26C;
extern s32 D_800C9D40;
extern f32 func_8002A8BC(s16 angle);
extern f32 func_8002A8C0(s16 angle);
extern s32 D_800CAF58;
extern u8 D_800CAF60[];
extern u8 D_800C9D48[];
extern u8 D_800C9F58[];
extern f32 D_800CB260;
extern f32 D_800CB270;
extern f32 D_800CB274;
extern s32 D_800CB284;
extern s32 D_800CB288;
extern void *func_8002B280(s32 size, s32 tag);
extern void mmFree(void *ptr);
extern s8 func_80017660(void *arg0, s32 arg1, void *arg2, s32 arg3, s32 arg4);
extern void func_80018544(void *arg0, void *arg1);
extern s32 shadowBoxPolyOverlap(f32 arg0, f32 arg1, s32 arg2, s16 arg3,
                                s32 arg4, s32 arg5, s32 arg6, void *arg7);

/* PROVENANCE: adapted from JFG's public asm/nonmatchings/shadows/shadowInitBuffers.s; Mickey globals are authoritative.
 * The C body emits all 75 linked instruction words and the owning 0x50-byte
 * .data section exactly. Its sentinel pair still binds D_80079434 + 0xC where
 * the target relocation metadata names D_80079440, so relocation identity is
 * not exact. */
void shadowInitBuffers(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 i;
    s32 stride0;
    s32 stride1;
    s32 stride2;

    D_800CB284 = arg0;
    D_800CB288 = arg1;
    D_800CB278 = arg2;
    D_800CB27C = arg3;
    stride0 = arg2 * 10;
    D_800CB280 = arg4;
    D_80079410[0] = func_8002B280(stride0 * 4, 0x8D);
    stride1 = arg3 * 16;
    D_80079420[0] = func_8002B280(stride1 * 4, 0x8D);
    stride2 = arg4 * 8;
    D_80079430[0] = func_8002B280(stride2 * 4, 0x8D);

    for (i = 0; i < 3; i++) {
        D_80079414[i] = D_80079414[i - 1] + stride0;
        D_80079424[i] = D_80079424[i - 1] + stride1;
        D_80079434[i] = D_80079434[i - 1] + stride2;
    }
    D_80079458 = 0;
}
/* PROVENANCE -- adapted from JFG's public asm/nonmatchings/shadows/shadowFreeBuffers.s. */
void shadowFreeBuffers(void) {
    if (D_80079410[0] != NULL) {
        mmFree(D_80079410[0]);
        D_80079410[0] = NULL;
    }
    if (D_80079420[0] != NULL) {
        mmFree(D_80079420[0]);
        D_80079420[0] = NULL;
    }
    if (D_80079430[0] != NULL) {
        mmFree(D_80079430[0]);
        D_80079430[0] = NULL;
    }
}
void shadowChangeBuffer(void) {
    D_80079458 ^= 1;
}
void shadowGetBuffers(s32 arg0, void **arg1, void **arg2, void **arg3) {
    s32 index = D_80079458;

    if (arg0 & 2) {
        index += 2;
    }
    *arg1 = D_80079410[index];
    *arg2 = D_80079420[index];
    *arg3 = D_80079430[index];
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/shadows/shadowGenerate.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/shadows/func_80016890.s")
/*
 * PROVENANCE: organized from the public JFG shadow polygon pipeline and
 * Mickey's own m2c control flow; all field offsets and buffer limits remain
 * Mickey-only evidence.
 */
#ifdef NON_MATCHING
/* Workbench verdict: structure-mismatch, 337 differing words; first mismatch is at +0x0. */
/* Target is 328 instructions/frame -320; candidate is 344 instructions/frame -280. */
/* Remaining gap is structural: pointer/stack scheduling and frame shape; not permuter-ready. */
void func_80017140(void *arg0, s32 arg1, void *arg2, s32 arg3) {
    u8 polygon[0x30];
    u8 *var_a3;
    u8 *temp_a3;
    u8 *temp_t1;
    u8 *temp_t2;
    u8 *temp_v0;
    u8 *var_v0;
    u8 *var_v1_2;
    u8 *var_v1_3;
    u8 *var_v1_4;
    u8 *trackBase;
    u8 *vertexBase;
    f32 pointHeight;
    s16 temp_a1;
    s16 temp_v0_3;
    s16 temp_v1;
    s16 var_a0_3;
    s16 var_a1;
    s16 var_a1_2;
    s16 var_a2;
    s16 var_ra;
    s16 var_t0;
    s32 temp_s1;
    s32 temp_t9;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a2_2;
    s32 var_lo;
    s32 var_t0_2;
    s32 var_t1;
    s32 var_v1;
    s8 temp_v0_4;
    u32 temp_a0;
    u32 temp_v0_2;
    u32 *maskBase;

    var_t0 = *(s16 *) ((u8 *) arg2 + 0x24);
    var_t1 = 0;
    if (var_t0 > 0) {
        s32 sp7C = 0;
        do {
            temp_v0 = *(u8 **) ((u8 *) arg2 + 0xC) + sp7C;
            temp_a0 = *(u32 *) (temp_v0 + 0xC);
            trackBase = *(u8 **) ((u8 *) arg2 + 0x0);
            maskBase = *(u32 **) ((u8 *) arg2 + 0x10);
            vertexBase = *(u8 **) ((u8 *) arg2 + 0x1C);
            if (!(temp_a0 & 0x08013880)) {
                var_ra = *(s16 *) (temp_v0 + 0x8);
                temp_a1 = *(s16 *) (temp_v0 + 0x18);
                temp_s1 = (s32) (trackBase + (*(s16 *) (temp_v0 + 0x6) * 0xA));
                if (var_ra < temp_a1) {
                    var_v1 = var_ra * 8;
                    var_a0 = var_ra * 4;
                    do {
                        temp_t9 = *(u32 *) (*(u8 **) ((u8 *) arg2 + 0x18) + var_v1) * 4;
                        temp_v0_2 = (*(u32 *) (maskBase + var_a0)) & arg3;
                        if ((temp_v0_2 & 0xFFFF) &&
                            ((temp_v0_2 >> 0x10) != 0) &&
                            (*(f32 *) (vertexBase + (temp_t9 * 4) + 0x4) > 0.5f)) {
                            var_a0_2 = 1;
                            temp_a3 = *(u8 **) ((u8 *) arg2 + 0x4) + (var_ra * 0x10);
                            var_v1_2 = temp_a3 + 1;
                            var_a1 = *(s8 *) (temp_s1 + (*(u8 *) (temp_a3 + 1) * 0xA) + 0x2);
                            var_a2 = var_a1;
                            do {
                                var_a0_2 += 1;
                                temp_v0_3 = *(s8 *)
                                    (temp_s1 + (*(u8 *) (var_v1_2 + 1) * 0xA) + 0x2);
                                if (temp_v0_3 < var_a1) {
                                    var_a1 = temp_v0_3;
                                } else if (var_a2 < temp_v0_3) {
                                    var_a2 = temp_v0_3;
                                }
                                var_v1_2 += 1;
                            } while (var_a0_2 < 3);
                            if (*(s16 *) ((u8 *) arg0 + 0x18) >= var_a1) {
                                var_v1_3 = temp_a3;
                                if (var_a2 >= *(s16 *) ((u8 *) arg0 + 0x16)) {
                                    var_v0 = polygon + 0x10;
                                    var_lo = *(u8 *) (var_v1_3 + 1) * 0xA;
                                    while (var_v0 != polygon + 0x30) {
                                        var_v0 += 0x10;
                                        var_v1_3 += 1;
                                        *(f32 *) (var_v0 - 0x20) =
                                            (f32) *(s16 *) (temp_s1 + var_lo);
                                        *(s32 *) (var_v0 - 0x12) = -1;
                                        *(f32 *) (var_v0 - 0x18) =
                                            (f32) *(s16 *)
                                                (temp_s1 + (*(u8 *) (var_v1_3 + 0x0) * 0xA) + 0x4);
                                        var_lo = *(u8 *) (var_v1_3 + 1) * 0xA;
                                    }
                                    *(f32 *) (var_v0 - 0x10) =
                                        (f32) *(s16 *) (temp_s1 + var_lo);
                                    *(s32 *) (var_v0 - 0x2) = -1;
                                    *(f32 *) (var_v0 - 0x8) =
                                        (f32) *(s16 *)
                                            (temp_s1 + (*(u8 *) (var_v1_3 + 1) * 0xA) + 0x4);
                                    if (shadowBoxPolyOverlap(
                                            *(f32 *) ((u8 *) arg0 + 0x40),
                                            *(f32 *) ((u8 *) arg0 + 0x44),
                                            var_a0_2, var_a1,
                                            *(s32 *) ((u8 *) arg0 + 0x48),
                                            *(s32 *) ((u8 *) arg0 + 0x4C), 3,
                                            polygon) != 0) {
                                        *(u8 **) ((u8 *) arg0 + 0x4) =
                                            vertexBase + (temp_t9 * 4);
                                        if (*(f32 *) ((u8 *) arg0 + 0x24) > 0.0f) {
                                            func_80018544(arg0, polygon);
                                        }
                                        temp_v0_4 = func_80017660(arg0, 3, polygon, 4, arg1);
                                        if (temp_v0_4 >= 3) {
                                            temp_t2 = D_800CAF60 + (D_800CAF58 * 0xC);
                                            *(u8 *) (temp_t2 + 1) = 0;
                                            var_t0_2 = 0;
                                            if (temp_v0_4 > 0) {
                                                var_a3 = polygon;
                                                do {
                                                    temp_v1 = *(s16 *) (var_a3 + 0xE);
                                                    var_a1_2 = -1;
                                                    var_a0_3 = 0;
                                                    if (temp_v1 < 0) {
                                                        var_a2_2 = D_800C9D40;
                                                        temp_t1 = temp_t2 + var_t0_2;
                                                        if (var_a2_2 > 0) {
                                                            var_v1_4 = D_800C9D48;
loop_27:
                                                            if ((*(f32 *) (var_v1_4 + 0x0) ==
                                                                 *(f32 *) (var_a3 + 0x0)) &&
                                                                (*(f32 *) (var_v1_4 + 0x8) ==
                                                                 *(f32 *) (var_a3 + 0x8))) {
                                                                var_a1_2 = var_a0_3;
                                                            }
                                                            var_a0_3 += 1;
                                                            var_v1_4 += 0x10;
                                                            if ((var_a0_3 < var_a2_2) &&
                                                                (var_a1_2 == -1)) {
                                                                goto loop_27;
                                                            }
                                                        }
                                                        if (var_a1_2 == -1) {
                                                            if (var_a2_2 >= 0x20) {
                                                                D_800C9D40 = 0x1F;
                                                                var_a2_2 = 0x1F;
                                                            }
                                                            var_v1_4 = D_800C9D48 + (var_a2_2 * 0x10);
                                                            *(f32 *) (var_v1_4 + 0x0) = *(f32 *) (var_a3 + 0x0);
                                                            *(f32 *) (var_v1_4 + 0x8) = *(f32 *) (var_a3 + 0x8);
                                                            D_800C9D40 = var_a2_2 + 1;
                                                            *(s8 *) (temp_t1 + 0x2) = var_a2_2;
                                                            *(s32 *) (var_v1_4 + 0xC) =
                                                                *(s32 *) ((u8 *) arg0 + 0x4);
                                                        } else {
                                                            *(s8 *) (temp_t1 + 0x2) = var_a1_2;
                                                        }
                                                    } else {
                                                        *(s8 *) (temp_t2 + var_t0_2 + 0x2) = temp_v1;
                                                        *(u8 *) (temp_t2 + 1) |= (1 << var_t0_2);
                                                    }
                                                    var_t0_2 += 1;
                                                    var_a3 += 0x10;
                                                } while (var_t0_2 != temp_v0_4);
                                            }
                                            *(u8 *) (temp_t2 + 0x0) = temp_v0_4;
                                            *(s16 *) (temp_t2 + 0xA) = *(s32 *) ((u8 *) temp_v0 + 0x94);
                                            D_800CAF58 += 1;
                                            D_800CB26C = 0;
                                            D_800CB268 = *(s32 *) ((u8 *) temp_v0 + 0x94);
                                        }
                                    }
                                }
                            }
                        }
                        var_ra += 1;
                        var_v1 += 8;
                        var_a0 += 4;
                    } while (var_ra < temp_a1);
                    var_t0 = *(s16 *) ((u8 *) arg2 + 0x24);
                }
            }
            var_t1 += 1;
            sp7C += 0x10;
        } while (var_t1 < var_t0);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/shadows/func_80017140.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/shadows/func_80017660.s")
/*
 * PROVENANCE: adapted from the public Diddy Kong Racing/JFG shadow-buffer
 * and projected-triangle organization; Mickey's target bytes, globals, and
 * resident buffer layouts determine the field bindings below.
 */
#ifdef NON_MATCHING
/* Workbench verdict: structure-mismatch, 312 differing words; first mismatch is at +0x0. */
/* Target is 314 instructions/frame -264; candidate is 326 instructions/frame -280. */
/* Remaining gap is structural: prologue/constant setup and pointer/register scheduling; not permuter-ready. */
s32 func_80017BCC(void *arg0, void *arg1, void *arg2) {
    u32 projected[3];
    u8 *var_a0;
    u8 *var_a2;
    u8 *var_a3;
    u8 *var_s6;
    u8 *var_t4;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *temp_v0;
    u8 *source;
    volatile f32 spA8;
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 var_f0;
    f32 var_f0_2;
    f32 var_f12;
    f32 var_f14;
    f32 var_f16;
    f32 var_f18;
    f32 var_f22;
    f32 var_f24;
    f32 var_f26;
    f32 var_f28;
    s32 var_fp;
    s32 var_s4;
    s32 var_s7;
    s32 var_t2;
    s32 var_t3;
    s32 var_v1;
    s32 var_v1_2;
    s8 var_a0_2;
    s8 var_a1_2;
    s8 var_t5;
    u8 var_a1;
    u8 vertexCount;

    if ((*(u8 *) ((u8 *) arg2 + 0x10) & 0x10) != 0) {
        var_f16 = 1.0f;
        var_f14 = 0.0f;
    } else {
        if (arg1 != NULL) {
            spA8 = func_8002A8C0(*(s16 *) ((u8 *) arg1 + 0x0));
            var_f0 = func_8002A8BC(*(s16 *) ((u8 *) arg1 + 0x0));
            var_f14 = spA8;
        } else {
            spA8 = func_8002A8C0(*(s16 *) ((u8 *) arg0 + 0x14));
            var_f0 = func_8002A8BC(*(s16 *) ((u8 *) arg0 + 0x14));
            var_f14 = spA8;
        }
        var_f16 = var_f0;
    }
    temp_v0 = *(u8 **) ((u8 *) arg0 + 0x0);
    var_f26 = *(f32 *) ((u8 *) arg0 + 0x34);
    temp_f12 = *(f32 *) ((u8 *) arg0 + 0x24);
    var_f28 = *(f32 *) ((u8 *) arg0 + 0x38);
    var_f18 = 255.0f;
    var_t5 = 0x19;
    var_t4 = D_800CAF60;
    var_s7 = 0;
    var_f22 = (f32) (*(u16 *) (temp_v0 + 0x6) * 0x10) / var_f26;
    var_f24 = (f32) (*(u16 *) (temp_v0 + 0x8) << 5) /
              (var_f28 + *(f32 *) ((u8 *) arg0 + 0x3C));
    if (temp_f12 > 0.0f) {
        temp_f2 = *(f32 *) ((u8 *) arg0 + 0xC) -
                  *(f32 *) ((u8 *) arg0 + 0x20);
        if (temp_f12 < temp_f2) {
            var_f18 = 255.0f *
                      (1.0f - ((temp_f2 - temp_f12) /
                               *(f32 *) ((u8 *) arg0 + 0x28)));
            if (var_f18 < 0.0f) {
                var_f18 = 0.0f;
            }
        }
        if (temp_f2 > 0.0f) {
            temp_f0 = (temp_f2 / 200.0f) + 1.0f;
            var_f22 *= temp_f0;
            var_f26 /= temp_f0;
            var_f24 *= temp_f0;
            var_f28 /= temp_f0;
        }
    }
    var_s4 = (s32) (var_f18 * D_800CB260);
    if (arg1 != NULL) {
        var_s4 = (s32) (*(s16 *) ((u8 *) arg1 + 0x4) * var_s4) >> 8;
    }
    var_f18 = D_800CB270;
    var_f0 = D_800CB274;
    var_t2 = D_8007944C;
    var_t3 = D_80079450;
    var_a3 = D_80079444 + (var_t3 * 0x10);
    var_fp = D_80079454;
    var_a2 = D_80079440 + (var_t2 * 0xA);
    var_s6 = D_80079448 + (var_fp * 8);
    if (D_800CAF58 > 0) {
loop_16:
        var_a0 = var_t4;
        if ((*(u8 *) (var_t4 + 0x0) + var_t5) >= 0x18) {
            *(s16 *) (var_s6 + 0x6) = var_t2;
            *(s16 *) (var_s6 + 0x4) = var_t3;
            var_s6 += 8;
            var_fp += 1;
            var_t5 = 0;
            *(u32 *) (var_s6 - 0x8) = *(u32 *) ((u8 *) arg0 + 0x0);
        }
        if (var_fp >= D_800CB280) {
            return 0;
        }
        var_a1 = *(u8 *) (var_t4 + 0x1);
        var_v1 = 0;
        vertexCount = *(u8 *) (var_t4 + 0x0);
        if ((s32) vertexCount > 0) {
loop_21:
            if (var_a1 & 1) {
                var_v0 = D_800C9F58 + (*(u8 *) (var_a0 + 0x2) << 5);
                var_f0_2 = *(f32 *) (var_v0 + 0x0);
                var_f12 = *(f32 *) (var_v0 + 0x4);
            } else {
                var_v0 = D_800C9D48 + (*(u8 *) (var_a0 + 0x2) * 0x10);
                var_f0_2 = *(f32 *) (var_v0 + 0x0);
                var_f12 = *(f32 *) (var_v0 + 0x4);
            }
            temp_f2_2 = *(f32 *) (var_v0 + 0x8);
            var_a1 = (u8) ((s32) var_a1 >> 1);
            var_t2 += 1;
            var_a2 += 0xA;
            *(s16 *) (var_a2 - 0xA) = (s32) var_f0_2;
            *(s8 *) (var_a2 - 0x4) = 0xFF;
            *(s8 *) (var_a2 - 0x3) = 0xFF;
            *(s8 *) (var_a2 - 0x2) = 0xFF;
            *(s8 *) (var_a2 - 0x1) = (s8) var_s4;
            *(s16 *) (var_a2 - 0x6) = (s32) temp_f2_2;
            *(s16 *) (var_a2 - 0x8) =
                (s32) (*(f32 *) ((u8 *) arg0 + 0x1C) + var_f12);
            if (var_t2 >= D_800CB278) {
                return 0;
            }
            temp_f12_2 = var_f0_2 - var_f18;
            var_a0 += 1;
            temp_f2_3 = temp_f2_2 - var_f0;
            projected[var_v1] =
                ((s32) (((temp_f2_3 * var_f16) +
                         (temp_f12_2 * var_f14) + var_f28) * var_f24) &
                 0xFFFF) |
                ((s32) (var_f22 * (((temp_f12_2 * var_f16) -
                                    (temp_f2_3 * var_f14)) + var_f26)) <<
                 0x10);
            var_v1 += 1;
            if (var_v1 < (s32) *(u8 *) (var_t4 + 0x0)) {
                goto loop_21;
            }
        }
        var_v1_2 = 1;
        if ((*(u8 *) (var_t4 + 0x0) - 1) >= 2) {
            var_a0_2 = var_t5 + 1;
            var_a1_2 = var_a0_2 + 1;
            var_v0_2 = (u8 *) &projected[1];
loop_29:
            *(u8 *) (var_a3 + 0x0) = 0;
            *(u8 *) (var_a3 + 0x1) = var_a0_2;
            *(u8 *) (var_a3 + 0x2) = var_a1_2;
            *(u8 *) (var_a3 + 0x3) = var_t5;
            *(u32 *) (var_a3 + 0x4) = *(u32 *) (var_v0_2 + 0x0);
            var_t3 += 1;
            var_v1_2 += 1;
            *(u32 *) (var_a3 + 0x8) = *(u32 *) (var_v0_2 + 0x4);
            *(u32 *) (var_a3 + 0xC) = projected[0];
            var_a3 += 0x10;
            if (var_t3 >= D_800CB27C) {
                return 0;
            }
            var_v0_2 += 4;
            var_a0_2 += 1;
            var_a1_2 += 1;
            if (var_v1_2 < (*(u8 *) (var_t4 + 0x0) - 1)) {
                goto loop_29;
            }
        }
        var_s7 += 1;
        var_t5 += *(u8 *) (var_t4 + 0x0);
        var_t4 += 0xC;
        if (var_s7 < D_800CAF58) {
            goto loop_16;
        }
    }
    D_8007944C = var_t2;
    D_80079450 = var_t3;
    D_80079454 = var_fp;
    return 1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/shadows/func_80017BCC.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/shadows/func_800180B4.s")
