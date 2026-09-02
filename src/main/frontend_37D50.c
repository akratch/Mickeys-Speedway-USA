#include "PR/ultratypes.h"
#include "game/memory.h"

extern s32 D_8007BEA8;
extern s32 D_8007BE90;
extern s32 D_8007BE94;
extern s32 D_8007BE98;
extern s32 D_8007BEB8;
extern s32 D_8007BE80;
extern s32 D_8007BEAC;
extern s32 D_8007BE9C;
extern s32 D_8007BEA0;
extern s32 D_8007BEA4;
extern s32 D_8007BEE0;
extern s32 D_800D2FA8;
typedef struct FrontendBufferPointers {
    void *unk0;
    void *unk4;
    void *unk8;
} FrontendBufferPointers;
extern FrontendBufferPointers D_8007BE88;
typedef struct FrontendVertex {
    s16 x;
    s16 y;
    s16 unk4;
    s8 r;
    s8 g;
    s8 b;
    s8 a;
} FrontendVertex;
extern s32 D_8007BE84;
extern s32 D_8007BEB0;
extern s32 D_8007BEB4;
extern f32 D_800826A0;
extern void func_800378A4(f32 arg0, s32 arg1);
extern f32 func_8002A8C0(s32 angle);
extern f32 sqrtf(f32 value);
extern void func_80037AEC(f32 arg0, s32 arg1);

typedef struct FrontendGfxWords {
    u32 w0;
    u32 w1;
} FrontendGfxWords;
typedef struct Gfx {
    FrontendGfxWords words;
} Gfx;
typedef struct Mtx Mtx;
typedef struct MainVertex MainVertex;

void func_80037150(void) {
    D_8007BEA8 = 0;
    if (D_8007BE88.unk0 != NULL) {
        mmFree(D_8007BE88.unk0);
        D_8007BE88.unk0 = NULL;
    }
    if (D_8007BE88.unk4 != NULL) {
        mmFree(D_8007BE88.unk4);
        D_8007BE88.unk4 = NULL;
    }
    D_8007BE80 = 0;
}
extern s32 viGetVideoMode(void);
extern void *func_8002B280(s32, s32);
/* Workbench verdict: structure-mismatch, 148 differing words; target 150/candidate 159 words. */
/* First mismatch: +0x28; both frames are 0x38, but record-store order diverges. */
/* Structural gap: grid induction and register/record-store schedule remain. */
#ifdef NON_MATCHING
void func_800371BC(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
    s16 temp_v0_2;
    s32 temp_t1;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_a3;
    s32 var_ra;
    s32 var_s2;
    s32 var_t0;
    void **var_s6;
    void *temp_v0;
    u8 *var_v1;

    if (D_8007BE80 == 0) {
        if (viGetVideoMode() & 1) {
            var_s2 = 0x238;
        } else {
            var_s2 = 0x1AA;
        }
        var_s6 = (void **) &D_8007BE88;
        do {
            temp_v0 = func_8002B280(0xB4A, 0x87);
            *var_s6 = temp_v0;
            var_v1 = (u8 *) temp_v0;
            if (temp_v0 != NULL) {
                temp_t1 = var_s2 >> 1;
                var_ra = 0;
                do {
                    var_a3 = var_s2 * 3;
                    *(s16 *) (var_v1 + 0) = (s16) (-temp_t1 - 1);
                    temp_v0_2 = (s16) (0x79 - (var_ra / 16));
                    *(s16 *) (var_v1 + 2) = temp_v0_2;
                    *(s16 *) (var_v1 + 4) = 0;
                    var_v1[6] = 0xFF;
                    var_v1[7] = 0xFF;
                    var_v1[8] = 0xFF;
                    var_v1[9] = 0xFF;
                    var_v1 += 0xA;
                    var_a0 = 1;
                    var_a1 = var_s2;
                    var_a2 = var_s2 * 2;
                    var_t0 = var_s2 * 4;
loop_grid:
                    *(s16 *) (var_v1 + 0) =
                        (s16) (((var_a1 / 16) - temp_t1) - 1);
                    var_a0 += 4;
                    var_a1 += var_s2 * 4;
                    *(s16 *) (var_v1 + 0xC) = temp_v0_2;
                    *(s16 *) (var_v1 + 0xE) = 0;
                    var_v1[0x10] = 0xFF;
                    var_v1[0x11] = 0xFF;
                    var_v1[0x12] = 0xFF;
                    var_v1[0x13] = 0xFF;
                    *(s16 *) (var_v1 + 0x16) = temp_v0_2;
                    *(s16 *) (var_v1 + 0x18) = 0;
                    var_v1[0x1A] = 0xFF;
                    var_v1[0x1B] = 0xFF;
                    var_v1[0x1C] = 0xFF;
                    var_v1[0x1D] = 0xFF;
                    *(s16 *) (var_v1 + 0x20) = temp_v0_2;
                    *(s16 *) (var_v1 + 0x22) = 0;
                    var_v1[0x24] = 0xFF;
                    var_v1[0x25] = 0xFF;
                    var_v1[0x26] = 0xFF;
                    var_v1[0x27] = 0xFF;
                    var_v1 += 0x28;
                    *(s16 *) (var_v1 - 0x26) = temp_v0_2;
                    *(s16 *) (var_v1 - 0x24) = 0;
                    var_v1[-0x22] = 0xFF;
                    var_v1[-0x21] = 0xFF;
                    var_v1[-0x20] = 0xFF;
                    var_v1[-0x1F] = 0xFF;
                    var_v1[-0x1E] =
                        (s16) (((var_a2 / 16) - temp_t1) - 1);
                    var_v1[-0x14] =
                        (s16) (((var_a3 / 16) - temp_t1) - 1);
                    var_v1[-0xA] =
                        (s16) (((var_t0 / 16) - temp_t1) - 1);
                    var_t0 += var_s2 * 4;
                    var_a3 += var_s2 * 4;
                    var_a2 += var_s2 * 4;
                    if (var_a0 != 0x11) {
                        goto loop_grid;
                    }
                    var_ra += 0xF0;
                } while (var_ra != 0xFF0);
            }
            var_s6 += 1;
        } while (var_s6 != (void **) &D_8007BE90);
        D_8007BE80 = 1;
        if ((((FrontendBufferPointers *) &D_8007BE88)->unk0 == NULL) ||
            (((FrontendBufferPointers *) &D_8007BE88)->unk4 == NULL)) {
            func_80037150();
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_800371BC.s")
#endif
extern void TrapDanglingJump();
/* Workbench verdict: structure-mismatch, 59 differing words; target 145/candidate 146 words. */
/* First mismatch: +0x10; target frame 0x30 versus candidate 0x40. */
/* Structural gap: argument homes and floating/integer carrier allocation. */
#ifdef NON_MATCHING
void func_80037414(s32 arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6) {
    s32 sp2C;
    s32 sp28;
    s32 sp20;
    s32 sp1C;
    s32 var_a1;
    s32 var_a2;
    s32 var_t0;

    var_t0 = (s32) (arg1 * 60.0f);
    var_a2 = (s32) (arg2 * 60.0f);
    var_a1 = var_t0;
    sp28 = var_a2;
    if (D_8007BE80 == 0) {
        sp2C = var_a1;
        sp1C = var_a2;
        sp20 = var_t0;
        func_800371BC(arg1, arg2, var_a1, var_a2);
    }
    if ((D_8007BEA8 != 0) &&
        ((D_8007BE90 == 4) || (D_8007BE90 == 5))) {
        sp2C = var_a1;
        sp1C = var_a2;
        sp20 = var_t0;
        TrapDanglingJump(arg0, var_a1, var_a2);
    }
    if ((arg6 == 0) || (D_8007BEA8 == 0)) {
        if ((arg0 & 1) && (var_a2 != 0)) {
            D_8007BEA8 = 2;
        } else {
            D_8007BEA8 = 1;
        }
        D_8007BEAC = 0;
        D_8007BEB0 = 0;
        D_8007BEB4 = 0x8000;
    } else if (D_8007BEA8 == 2) {
        D_8007BEAC = (s32) (sp28 * D_8007BEAC) / D_8007BE98;
        D_8007BEB0 = 0;
    } else if (!(arg0 & 1)) {
        D_8007BEAC = (s32) (var_a1 * D_8007BEB0) / 1024;
    } else {
        D_8007BEAC = (s32) ((0x400 - D_8007BEB0) * var_a1) / 1024;
    }
    D_8007BE90 = arg0;
    D_8007BE94 = var_t0;
    D_8007BE98 = var_a2;
    D_8007BE9C = (u8) arg3;
    D_8007BEA0 = (u8) arg4;
    D_8007BEA4 = (u8) arg5;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037414.s")
#endif
void func_80037658(void) {
    D_8007BEA8 = 0;
}
s32 func_80037664(void) {
    if ((D_8007BEA8 == 0) && (D_8007BEB8 == 0)) {
        return 0;
    }
    if ((D_8007BEA8 != 2) || ((D_8007BE90 & 1) != 0) ||
        (D_8007BEB8 != 0)) {
        return 1;
    }
    return 2;
}
/* Workbench verdict: structure-mismatch, 95 differing words; target 118/candidate 119 words. */
/* First mismatch: +0x0; target frame 0x18 versus candidate 0x20. */
/* Structural gap: state-machine carrier/frame allocation and branch schedule. */
#ifdef NON_MATCHING
void func_800376CC(s32 arg0) {
    s32 temp_t0;
    s32 temp_t7;
    s32 temp_t9;
    register s32 old_state;
    register s32 var_a1;
    s32 var_a2;
    s32 var_v0;

    var_a2 = arg0;
    temp_t0 = D_8007BE90 & 1;
    old_state = D_8007BEA8;
    do {
        var_a1 = 1;
        if (D_8007BEA8 == 2) {
            if (D_8007BE98 >= 0) {
                temp_t7 = D_8007BEAC + var_a2;
                D_8007BEAC = temp_t7;
                if (temp_t7 >= D_8007BE98) {
                    var_a2 = temp_t7 - D_8007BE98;
                    if (temp_t0 != 0) {
                        D_8007BEA8 = 1;
                    } else {
                        D_8007BEA8 = 0;
                    }
                    D_8007BEAC = 0;
                    goto block_17;
                }
            }
        } else if (D_8007BEA8 == 1) {
            temp_t9 = D_8007BEAC + var_a2;
            D_8007BEAC = temp_t9;
            if (temp_t0 == 0) {
                D_8007BEB0 = (s32) (temp_t9 << 0xA) / D_8007BE94;
                var_v0 = temp_t9;
            } else {
                var_v0 = D_8007BEAC;
                D_8007BEB0 = (s32) ((D_8007BE94 - var_v0) << 0xA) /
                              D_8007BE94;
            }
            if (var_v0 >= D_8007BE94) {
                var_a2 = var_v0 - D_8007BE94;
                if (temp_t0 != 0) {
                    D_8007BEA8 = 0;
                } else {
                    D_8007BEA8 = 2;
                }
                D_8007BEAC = 0;
block_17:
                var_a1 = 0;
            }
        }
    } while (var_a1 == 0);
    if (D_8007BEB8 != 0) {
        D_8007BEB8 -= 1;
    }
    if ((old_state != 2) && (D_8007BEA8 == 2)) {
        D_8007BEB8 = 1;
    }
    if ((old_state != 0) && (D_8007BEA8 == 0) &&
        ((D_8007BEB8 = 1, (D_8007BE90 == 4)) ||
         (D_8007BE90 == 5))) {
        TrapDanglingJump(&D_8007BEAC, var_a1, var_a2, &D_8007BEA8);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_800376CC.s")
#endif
/* Workbench verdict: structure-mismatch, 103 differing words. */
/* First mismatch: +0x14; target 117 instructions/candidate 112, both 0x68 frames. */
/* Structural gap: radial-loop FP lifetimes and clamp/control-flow shape remain. */
#ifdef NON_MATCHING
void func_800378A4(f32 arg0, s32 arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 var_f20;
    s32 temp_s3;
    s32 var_a0;
    s32 var_s1;
    s32 var_s6;
    s32 var_v0;
    FrontendVertex *var_s0;

    var_a0 = (s32) arg0;
    arg0 *= D_800826A0;
    var_s0 = ((FrontendVertex **) &D_8007BE88)[D_8007BE84];
    if (var_s0 != NULL) {
        temp_s3 = 0xFF - var_a0;
        var_s6 = 0;
        do {
            var_s1 = 0;
            temp_f0 = (f32) (var_s6 - 8) * 15.0f;

            do {
                temp_f0_2 = (f32) (var_s1 - 8) * 20.0f;
                temp_f0_3 = sqrtf((temp_f0_2 * temp_f0_2) +
                                  (temp_f0 * temp_f0));
                var_f20 = (200.0f - temp_f0_3) * arg0;
                if (var_f20 < 0.0f) {
                    var_f20 = 0.0f;
                }
                var_a0 = (s32) (func_8002A8C0(
                    (s32) (temp_f0_3 * 1000.0f) + D_8007BEB4) * var_f20);
                var_v0 = (s32) ((temp_s3 + var_a0) * arg1) >> 8;
                if (var_v0 < 0) {
                    var_v0 = 0;
                }
                if (var_v0 >= 0x100) {
                    var_v0 = 0xFF;
                }
                var_s0->unk4 = (s16) (var_a0 + 5);
                var_s0->r = (s8) var_v0;
                var_s0->g = (s8) var_v0;
                var_s0->b = (s8) var_v0;
                var_s1 += 1;
                var_s0 += 1;
            } while (var_s1 != 0x11);
            var_s6 += 1;
        } while (var_s6 != 0x11);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_800378A4.s")
#endif
void func_80037A78(void) {
    D_8007BEB4 = 0x8000 - (D_8007BEB0 << 8);
    if (D_8007BEB0 < 0x200) {
        func_800378A4((f32) ((s32) D_8007BEB0 >> 3), 0x100);
        return;
    }
    func_800378A4(64.0f, (s32) (0x400 - D_8007BEB0) >> 1);
}
/* Workbench verdict: allocation-mismatch, 7 differing words. */
/* First mismatch: +0x54; target and candidate are 66 words with 0x40 frames. */
/* Structural gap: only register allocation remains. */
#ifdef NON_MATCHING
void func_80037AEC(f32 arg0, s32 arg1) {
    FrontendVertex *vertex;
    s32 row;
    s32 phase;

    vertex = ((FrontendVertex **) &D_8007BE88)[D_8007BE84];
    if (vertex != NULL) {
        s32 contrast = 0xFF - ((s32) arg0 * 2);

        phase = D_8007BEB4;
        row = 0;
        do {
            s32 column = 0;
            s32 angle = phase;

            do {
                f32 value = func_8002A8C0(angle) * arg0;
                s32 integerValue;
                s32 intensity;

                column += 1;
                angle += 0x2000;
                vertex += 1;
                integerValue = (s32) value;
                vertex[-1].unk4 = (s16) (integerValue + 5);
                intensity = (s32) (((integerValue * 2) + contrast) * arg1) >> 8;
                vertex[-1].r = (s8) intensity;
                vertex[-1].g = (s8) intensity;
                vertex[-1].b = (s8) intensity;
            } while (column != 0x11);
            row += 1;
            phase += 0x800;
        } while (row != 0x11);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037AEC.s")
#endif
/* Workbench verdict: structure-mismatch, with a four-byte size deficit. */
/* First mismatch: +0x40; target multiplies before the call and has a nop delay slot. */
/* Structural gap: the interpolation multiply/call scheduling shape is unresolved. */
#ifdef NON_MATCHING
void func_80037BF4(void) {
    D_8007BEB4 = (D_8007BEB0 << 8) + 0x8000;
    if (D_8007BEB0 < 0x200) {
        func_80037AEC((f32) D_8007BEB0 * 0.0625f, 0x100);
    } else {
        func_80037AEC(32.0f, (s32) (0x400 - D_8007BEB0) >> 1);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037BF4.s")
#endif
extern u8 D_7BE40[];
extern s32 D_800D2FAC;
extern void camStandardPersp(Gfx **, Mtx **);
extern void func_80034920(Gfx **);
extern u8 D_8007BEC0[];
extern void viGetCurrentSize(s32 *, s32 *);
extern void func_80037A78(void);
extern void func_80037BF4(void);

#define FRONTEND_EMIT(pkt, opcode, data) \
    do { \
        Gfx *_cmd = *(pkt); \
        _cmd->words.w0 = (u32) (opcode); \
        _cmd->words.w1 = (u32) (data); \
        *(pkt) = _cmd + 1; \
    } while (0)

/* Workbench verdict: structure-mismatch, 324 differing words; target 327/candidate 308 words. */
/* First mismatch: +0x0; target frame 0xE0 versus candidate 0xF0. */
/* Structural gap: Gfx emission schedule and jump-table/render-loop shape. */
#ifdef NON_MATCHING
void func_80037C74(Gfx **arg0, Mtx **arg1, MainVertex **arg2) {
    s32 spD8;
    s32 spB8;
    s32 sp40;
    s32 temp_a1;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t5;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_a0;
    s32 var_a2;
    s32 var_ra;
    s32 var_s0;
    s32 var_s1;
    s32 var_t2;
    s32 var_t3;
    s32 var_t4;
    s32 var_v1;

    if (D_8007BE80 != 0) {
        camStandardPersp(arg0, arg1);
        FRONTEND_EMIT(arg0, 0xE7000000, 0);
        FRONTEND_EMIT(arg0, 0xED000000, 0x5003C0);
        FRONTEND_EMIT(arg0, 0xEF30000F, 0);
        FRONTEND_EMIT(arg0, 0xF7000000, 0x10001);
        FRONTEND_EMIT(arg0, 0xF64FC3BC, 0);
        FRONTEND_EMIT(arg0, 0xE7000000, 0);
        FRONTEND_EMIT(arg0, 0xB6000000, 0x10001);
        var_a0 = 0;
        if ((D_8007BE90 == 2) || (D_8007BE90 == 3)) {
            FRONTEND_EMIT(arg0, 0xFC357E04, 0x1F10F3FF);
            FRONTEND_EMIT(arg0, 0xEF182C0F, 0x0F0A4000);
        } else {
            FRONTEND_EMIT(arg0, 0xFC121824, 0xFF33FFFF);
            FRONTEND_EMIT(arg0, 0xEF082C0F, 0x0F0A4000);
        }
        FRONTEND_EMIT(arg0, 0xFD10013F, D_800D2FAC);
        sp40 = 0;
        spD8 = 0;
        spB8 = (D_8007BEB0 << 5) / 1024;
        do {
            var_v1 = 0;
            var_s0 = spB8;
            var_ra = sp40 * 0xA;
            var_s1 = 0;
            temp_s2 = var_a0 - 1;
            temp_s3 = var_a0 + 0xF;
loop_7:
            temp_v0 = var_v1 - 1;
            temp_t5 = var_v1 + 0x28;
            if (temp_v0 > 0) {
                var_t3 = temp_v0;
            } else {
                var_t3 = 0;
            }
            if (temp_s2 > 0) {
                var_t4 = temp_s2;
            } else {
                var_t4 = 0;
            }
            if (temp_t5 < 0x13F) {
                var_t2 = temp_t5;
            } else {
                var_t2 = 0x13F;
            }
            temp_t0 = var_t2 - var_t3;
            if (temp_s3 < 0xEF) {
                var_a2 = temp_s3;
            } else {
                var_a2 = 0xEF;
            }
            temp_t1 = ((((temp_t0 * 2) + 9) >> 3) & 0x1FF) << 9;
            FRONTEND_EMIT(arg0, temp_t1 | 0xF5100000, 0x07080200);
            FRONTEND_EMIT(arg0, 0xE6000000, 0);
            FRONTEND_EMIT(arg0,
                          (((var_t3 * 4) & 0xFFF) << 12) |
                              0xF4000000 | ((var_t4 * 4) & 0xFFF),
                          (((var_t2 * 4) & 0xFFF) << 12) |
                              0x07000000 | ((var_a2 * 4) & 0xFFF));
            FRONTEND_EMIT(arg0, 0xE7000000, 0);
            FRONTEND_EMIT(arg0, 0xF2000000,
                          ((((temp_t0 - 1) * 4) & 0xFFF) << 12) |
                              (((var_a2 - var_t4 - 1) * 4) & 0xFFF));
            if ((D_8007BE90 == 2) || (D_8007BE90 == 3)) {
                FRONTEND_EMIT(arg0, 0xFA000000, var_s0);
                var_s0 ^= -0x100;
            }
            var_s1 += 2;
            temp_v1 = ((Gfx **) &D_8007BE88)[D_8007BE84];
            temp_v1 += var_ra + 0x80000000;
            FRONTEND_EMIT(arg0,
                          (((((temp_v1 & 6) | 0x18) & 0xFF) << 16) |
                              0x04000026),
                          temp_v1);
            temp_v1_2 = ((Gfx **) &D_8007BE88)[D_8007BE84];
            temp_v1_2 += var_ra + 0xAA + 0x80000000;
            FRONTEND_EMIT(arg0,
                          (((((temp_v1_2 & 6) | 0x18) & 0xFF) << 16) |
                              0x04000626),
                          temp_v1_2);
            FRONTEND_EMIT(arg0, 0x05310040, D_7BE40);
            var_ra += 0x14;
            var_v1 = temp_t5;
            if (var_s1 != 0x10) {
                goto loop_7;
            }
            var_a0 = temp_s3;
            temp_a1 = spD8 + 1;
            if (spD8 & 1) {
                spB8 ^= -0x100;
            }
            spD8 = temp_a1;
            sp40 += 0x11;
        } while (temp_a1 != 0x10);
        func_80034920(arg0);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80037C74.s")
#endif

/* Workbench verdict: structure-mismatch, 365 differing words; target 368/candidate 342 words. */
/* First mismatch: +0x0; both frames are 0xE8, with 86 relocation-site differences. */
/* Structural gap: Gfx emission schedule, relocation web, and render-loop shape remain unresolved. */
#ifdef NON_MATCHING
void func_80038190(Gfx **arg0, Mtx **arg1, MainVertex **arg2) {
    s32 spE0;
    s32 spDC;
    f32 spD8;
    u8 *viewport;
    s32 var_a2;
    s32 var_a3;
    s32 var_t2;
    s32 var_t3;
    s32 var_t4;
    s32 var_v0;
    u32 temp_v1;
    u32 temp_v1_2;

    if ((D_8007BE80 != 0) && (D_8007BEA8 != 0)) {
        FRONTEND_EMIT(arg0, 0xE7000000, 0);
        FRONTEND_EMIT(arg0, 0xED000000, 0x5003C0);
        if (D_8007BEA8 == 2) {
            FRONTEND_EMIT(arg0, 0xEF30000F, 0);
            FRONTEND_EMIT(arg0, 0xF7000000, 0x10001);
            FRONTEND_EMIT(arg0, 0xF64FC3BC, 0);
        } else {
            spD8 = (f32) D_8007BEAC / (f32) D_8007BE94;
            switch (D_8007BE90) {
            case 0:
            case 1:
                func_80037A78();
                break;
            case 2:
            case 3:
                func_80037BF4();
                break;
            case 4:
            case 5:
                TrapDanglingJump(spD8, D_8007BEB0, 0x10, 0x10, 0x14, 0xF);
                break;
            }
            D_8007BEE0 = (D_8007BEE0 + 1) & 1;
            viGetCurrentSize(&spE0, &spDC);
            viewport = D_8007BEC0 + (D_8007BEE0 * 0x10);
            *(s16 *) (viewport + 8) = (s16) (spE0 * 2);
            *(s16 *) (viewport + 0xA) = (s16) (spDC * 2);
            *(s16 *) (viewport + 0) = (s16) (spE0 * 2);
            *(s16 *) (viewport + 2) = (s16) (spE0 * 2);
            FRONTEND_EMIT(arg0, 0xE7000000, 0);
            FRONTEND_EMIT(arg0, 0xBC000406,
                          (u32) D_800D2FAC + 0x80000000u);
            FRONTEND_EMIT(arg0, 0xBC001006,
                          (u32) D_800D2FAC + 0x7FFFFB00u);
            FRONTEND_EMIT(arg0, 0xBC000806, 0x80000000u);
            FRONTEND_EMIT(arg0, 0xFF10013F, 0x01000000);
            FRONTEND_EMIT(arg0, 0xB6000000, 0x10001);
            FRONTEND_EMIT(arg0, 0xEF20000F, 0);
            var_a3 = 0;
            var_a2 = 0;
            do {
                var_t2 = var_a2 + 4;
                FRONTEND_EMIT(arg0, 0xFD100000,
                              (u32) D_800D2FA8 + var_a3);
                var_a3 += 0xA00;
                FRONTEND_EMIT(arg0, 0xF5100000, 0x07080200);
                FRONTEND_EMIT(arg0, 0xE6000000, 0);
                FRONTEND_EMIT(arg0, 0xF3000000, 0x074FF01A);
                FRONTEND_EMIT(arg0, 0xE7000000, 0);
                FRONTEND_EMIT(arg0, 0xF510A000, 0x80200);
                FRONTEND_EMIT(arg0, 0xF2000000, 0x4FC00C);
                FRONTEND_EMIT(arg0, 0xE4500000 | ((var_t2 * 4) & 0xFFF),
                              (var_a2 * 4) & 0xFFF);
                FRONTEND_EMIT(arg0, 0xB3000000, 0);
                FRONTEND_EMIT(arg0, 0xB2000000, 0x10000400);
                var_a2 = var_t2;
            } while (var_t2 != 0xF0);
            FRONTEND_EMIT(arg0, 0xE7000000, 0);
            FRONTEND_EMIT(arg0, 0xBC000406,
                          (u32) D_800D2FA8 + 0x80000000u);
            FRONTEND_EMIT(arg0, 0xBC001006,
                          (u32) D_800D2FA8 + 0x7FFFFB00u);
            FRONTEND_EMIT(arg0, 0xFF10013F, 0x01000000);
            FRONTEND_EMIT(arg0, 0x03800010, (u32) viewport + 0x80000000u);
            if ((D_8007BE90 == 4) || (D_8007BE90 == 5)) {
                TrapDanglingJump((f32) (u32) arg0, (s32) arg1, (s32) arg2,
                                 spD8, 0x10, 0x10, 4, 0x28, 0xF);
            } else {
                func_80037C74(arg0, arg1, arg2);
            }
            D_8007BE84 ^= 1;
        }
        FRONTEND_EMIT(arg0, 0xE7000000, 0);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/frontend_37D50/func_80038190.s")
#endif
#undef FRONTEND_EMIT

/* PLATEAU-HANDOFF:func_800371BC:start
 * symbol: func_800371BC
 * score: 148 differing words
 * frame: 0x38
 * relocations: 13
 * first-mismatch: +0x28
 * summary: Grid induction and record-store schedule remain structurally different.
 * PLATEAU-HANDOFF:func_800371BC:end
 */

/* PLATEAU-HANDOFF:func_80037414:start
 * symbol: func_80037414
 * score: 59 differing words
 * frame: 0x40
 * relocations: 42
 * first-mismatch: +0x10
 * summary: VI transition semantics compile, but argument homes and carrier allocation remain structurally different.
 * PLATEAU-HANDOFF:func_80037414:end
 */

/* PLATEAU-HANDOFF:func_800376CC:start
 * symbol: func_800376CC
 * score: 95 differing words
 * frame: 0x20
 * relocations: 15
 * first-mismatch: +0x0
 * summary: State-machine branch and carrier allocation remain structurally different.
 * PLATEAU-HANDOFF:func_800376CC:end
 */

/* PLATEAU-HANDOFF:func_800378A4:start
 * symbol: func_800378A4
 * score: 103 differing words
 * frame: 0x68
 * relocations: 10
 * first-mismatch: +0x14
 * summary: Radial-gradient induction, floating-point lifetimes, and clamp control flow remain unresolved.
 * PLATEAU-HANDOFF:func_800378A4:end
 */

/* PLATEAU-HANDOFF:func_80037AEC:start
 * symbol: func_80037AEC
 * score: 7 differing words
 * frame: 0x40
 * relocations: 7
 * first-mismatch: +0x54
 * summary: Shape-exact 66-word candidate; only register allocation differs and it is permuter-ready.
 * PLATEAU-HANDOFF:func_80037AEC:end
 */

/* PLATEAU-HANDOFF:func_80037BF4:start
 * symbol: func_80037BF4
 * score: 17 differing words
 * frame: 0x18
 * relocations: 6
 * first-mismatch: +0x40
 * summary: Target has one extra instruction; interpolation multiply and call scheduling remain structurally different.
 * PLATEAU-HANDOFF:func_80037BF4:end
 */

/* PLATEAU-HANDOFF:func_80037C74:start
 * symbol: func_80037C74
 * score: 324 differing words
 * frame: 0xF0
 * relocations: 18
 * first-mismatch: +0x0
 * summary: Display-list emission schedule, relocation web, and render-loop shape remain unresolved.
 * PLATEAU-HANDOFF:func_80037C74:end
 */
