#include "PR/ultratypes.h"

typedef s32 O53Unknown;

#define O53_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * Tier D (frame identity): the target writes 0x84, 0x88, 0x8C, 0x94 and the
 * halfword pair 0x90/0x92 and then passes sp+0x84 to the eight-argument draw
 * call, so those six slots are one address-taken aggregate rather than six
 * scalars.  Split into scalars, five of the six stores are dead and uopt
 * removes them; that is what the missing-instruction runs at target +0x898
 * and the two +0x7C0/+0x828 arms of this block turned out to be.
 */
typedef struct O53DrawPacket {
    u32 unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s32 unk10;
} O53DrawPacket;

extern O53Unknown **overlay53ExternalReloc();
extern void overlay53CopyOffsetEntries();

/*
 * The overlay reaches these through runtime LOCAL relocation records, so the
 * extracted object carries no static symbol for them and the scaffold rendered
 * every one as an absolute address.  An absolute address compiles to a single
 * $zero-based access; the target materialises %hi/%lo, which is where the
 * missing words at target +0x10, +0x328, +0x3DC and their siblings live.
 * The names below assert nothing but distinctness.
 */
extern s32 D_0;
extern s16 D_84;
extern f32 D_8C;
extern f32 D_90;
extern O53Unknown *D_110;
extern s8 D_114;
extern s16 D_T16[];
extern u32 D_T32[];

extern O53Unknown D_1C;
extern O53Unknown D_DC;
extern O53Unknown D_118;
extern O53Unknown D_120;
extern O53Unknown D_140;
extern O53Unknown *D_288;
extern f32 D_290;

/*
 * Mickey-only mips_to_c scaffold. The permitted reference scan found no
 * close donor: the nearest masked skeleton is DKR results_render at 0.064.
 * The extracted assembly does not retain external relocation identities, so
 * overlay53ExternalReloc represents multiple calls and makes no naming claim.
 * Workbench p5: structure-mismatch; 713/636 candidate/target instructions, 709 differing words from +0x0, frame -0x178 vs -0xD8.
 * No new safe lever: all 119 flags are exhausted; unresolved call/data relocation identities block structural iteration.
 * Remains: concrete callee/data types and relocation ownership.
 */
/* Ownership trial (2026-08-28): no initialized .data/.rodata is emitted by
 * this TU; linked promotion remains text-differs with 634 in-range words,
 * first +0x0. Candidate text is +0x140 bytes over the 0x9F0 target span; no ownership lever remains. */
#ifdef NON_MATCHING
void func_overlay_053_F0000240_189DBE8(s32 arg0) {
    O53Unknown spD0;
    s32 spCC;
    s32 spC8;
    O53Unknown **spC4;
    O53Unknown spB4;
    u32 spB0;
    s32 spAC;
    s32 spA8;
    s32 spA4;
    O53DrawPacket sp84;
    O53Unknown sp80;
    u32 sp7C;
    s32 *sp78;
    s32 sp70;
    O53Unknown **sp6C;
    O53Unknown **sp60;
    u32 *sp5C;
    u32 *sp50;
    O53Unknown **temp_s7;
    O53Unknown **temp_v0_2;
    O53Unknown **temp_v1;
    O53Unknown **var_v0_3;
    O53Unknown *temp_a0;
    O53Unknown *temp_v0_3;
    O53Unknown *temp_v0_5;
    O53Unknown *var_fp;
    O53Unknown *var_v0_2;
    f32 var_f16;
    f32 var_f16_2;
    s16 *temp_v0_7;
    s32 *temp_s1;
    s32 *temp_s1_2;
    s32 *var_s0;
    s32 temp_s3;
    s32 temp_s3_2;
    s32 temp_v0;
    s32 temp_v0_6;
    s32 temp_v1_2;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_v0;
    s32 var_v0_4;
    s32 var_v1;
    s8 temp_t3;
    u32 *temp_a1;
    u32 *temp_a1_2;
    u8 temp_v0_4;
    void *temp_s2;
    void *temp_s2_2;

    sp78 = overlay53ExternalReloc();
    overlay53ExternalReloc(&sp80, &sp7C);
    sp7C >>= 1;
    overlay53ExternalReloc(0, 0);
    temp_s7 = overlay53ExternalReloc(&spB4);
    if (D_0 == 0) {
        for (var_a1 = 0; var_a1 < arg0; var_a1++) {
            D_290 += (-11.0f - D_290) * 0.125f;
        }
    }
    spA4 = (s32)D_290;
    temp_v0_2 = overlay53ExternalReloc();
    temp_t3 = D_114 + 1;
    D_114 = temp_t3;
    spC4 = temp_v0_2;
    D_114 = temp_t3 % 10;
    spC8 = -1;
    spCC = -1;
    sp70 = 0;
    sp6C = temp_s7;
loop_10:
    temp_v0_3 = *sp6C;
    if (temp_v0_3 != 0) {
        temp_s2 = O53_FIELD(temp_v0_3, void **, 0x64);
        temp_s1 = sp70 + &D_118;
        if (O53_FIELD(temp_s2, u8 *, 0x19A) != 0xFF) {
            var_v0 = *temp_s1 + (arg0 * 0x10);
            *temp_s1 = var_v0;
            if (var_v0 >= 0x100) {
                *temp_s1 = 0xFF;
                var_v0 = 0xFF;
            }
        } else {
            var_v0 = *temp_s1 - (arg0 * 8);
            *temp_s1 = var_v0;
            if (var_v0 < 0) {
                *temp_s1 = 0;
                var_v0 = 0;
            }
        }
        if (var_v0 > 0) {
            temp_a1 = sp70 + &D_288;
            temp_v1 = sp70 + &spC8;
            if (O53_FIELD(temp_s2, s32 *, 0x19C) != 0) {
                *temp_v1 = (O53Unknown *)0x35;
            } else {
                temp_v0_4 = O53_FIELD(temp_s2, u8 *, 0x19A);
                if (temp_v0_4 != 0xFF) {
                    *temp_v1 = (O53Unknown *)*(s16 *)(temp_v0_4 * 2);
                } else {
                    *temp_v1 = *temp_a1;
                }
            }
            var_v0_2 = *temp_v1;
            if ((u32)var_v0_2 != *temp_a1) {
                if (*temp_a1 != -1U) {
                    sp60 = temp_v1;
                    sp5C = temp_a1;
                    overlay53ExternalReloc(*temp_a1, temp_a1);
                    var_v0_2 = *temp_v1;
                }
                *temp_a1 = (u32)var_v0_2;
                if (var_v0_2 != (O53Unknown *)-1) {
                    overlay53ExternalReloc(var_v0_2, temp_a1);
                }
            }
        }
        sp6C += 4;
        sp70 += 4;
        if ((temp_s1 + 4) == &D_120) {
            var_v0_3 = &D_288;
            do {
                temp_a0 = *var_v0_3;
                if ((temp_a0 != (O53Unknown *)-1) &&
                    (temp_a0 != (O53Unknown *)spC8) &&
                    (temp_a0 != (O53Unknown *)spCC)) {
                    sp6C = var_v0_3;
                    overlay53ExternalReloc(temp_a0);
                    *var_v0_3 = (O53Unknown *)-1;
                }
                var_v0_3 += 4;
            } while ((u32)var_v0_3 < (u32)&D_290);
            var_a2 = 0;
            var_s0 = &spC8;
            do {
                temp_v1_2 = *var_s0;
                if (temp_v1_2 != -1) {
                    *(var_a2 + &D_288) = temp_v1_2;
                    if (*(s32 *)(temp_v1_2 * 4) == 0) {
                        sp70 = var_a2;
                        overlay53ExternalReloc(temp_v1_2);
                    }
                }
                var_s0 += 4;
                var_a2 += 4;
            } while (var_s0 != &spD0);
            sp6C = temp_s7;
            var_fp = 0;
            sp70 = 0;
loop_40:
            temp_v0_5 = *sp6C;
            temp_s1_2 = sp70 + &D_118;
            if (temp_v0_5 != 0) {
                temp_s2_2 = O53_FIELD(temp_v0_5, void **, 0x64);
                overlay53ExternalReloc(var_fp);
                overlay53ExternalReloc(0);
                if (*sp78 == 6) {
                    temp_v0_6 = (((s32)var_fp * 4) + (s32)var_fp) << 5;
                    sp50 = temp_v0_6 + &D_140;
                    sp5C = (u32 *)temp_v0_6;
                    temp_s3 = (s32)var_fp * 2;
                    overlay53ExternalReloc(
                        O53_FIELD(temp_s2_2, O53Unknown **, 0x400),
                        &spB0, &spAC, &spA8);
                    if ((D_0 == 0) &&
                        (O53_FIELD(spC4, s8 *, 0x86) !=
                         O53_FIELD(temp_s2_2, s8 *, 0x383)) &&
                        (overlay53ExternalReloc() == 0) &&
                        (O53_FIELD(temp_s2_2, O53Unknown **, 0x400) !=
                         (O53Unknown *)0x83D60)) {
                        spA8 = (spA8 - (spA8 % 10)) + D_114;
                    }
                    overlay53CopyOffsetEntries(&D_1C, temp_v0_6, var_fp, 0);
                    O53_FIELD(temp_v0_6, s32 *, 8) = (spB0 / 10) << 0x10;
                    O53_FIELD(temp_v0_6, s32 *, 0x18) = (spB0 % 10) << 0x10;
                    O53_FIELD(temp_v0_6, s32 *, 0x38) = (spAC / 10) << 0x10;
                    O53_FIELD(temp_v0_6, s32 *, 0x48) = (spAC % 10) << 0x10;
                    O53_FIELD(temp_v0_6, s32 *, 0x68) = (spA8 / 10) << 0x10;
                    O53_FIELD(temp_v0_6, s32 *, 0x78) = (spA8 % 10) << 0x10;
                    var_v1 = 0;
                    var_v0_4 = (s32)sp5C;
                    do {
                        if ((O53_FIELD(var_v0_4, s32 *, 8) >> 0x10) == 1) {
                            if ((var_v1 == 0) || (var_v1 == 0x30) ||
                                (var_v1 == 0x60)) {
                                O53_FIELD(var_v0_4, s16 *, 0xC)++;
                            } else {
                                O53_FIELD(var_v0_4, s16 *, 0xC)--;
                            }
                        }
                        var_v1 += 0x10;
                        var_v0_4 += 0x10;
                    } while (var_v1 != 0x80);
                    overlay53ExternalReloc(0, (u32 *)temp_v0_6, 0,
                                           (s32 *)spA4, 0xFF, 0xFF, 0xFF,
                                           0xFF);
                    overlay53ExternalReloc(0);
                    if (overlay53ExternalReloc() == (O53Unknown **)1) {
                        D_90 = (f32)(0x50 - spA4);
                        D_8C =
                            (f32)O53_FIELD(temp_s3, s16 *, 0xEC);
                    } else {
                        D_8C = -44.0f;
                        D_90 =
                            (f32)(O53_FIELD(temp_s3, s16 *, 0xDC) - spA4 +
                                  0x5C);
                    }
                    D_84 =
                        (s32)((s32)O53_FIELD(temp_s2_2, O53Unknown **, 0x400) *
                              -0x10000) /
                        300;
                    overlay53ExternalReloc(4);
                    overlay53ExternalReloc(0, sp50, 0, (s32 *)spA4, 0xFF,
                                           0xFF, 0xFF, 0xFF);
                }
                if (*temp_s1_2 > 0) {
                    temp_a1_2 = sp70 + &D_288;
                    temp_s3_2 = (s32)var_fp * 2;
                    if (*temp_a1_2 != -1U) {
                        sp5C = temp_a1_2;
                        if (overlay53ExternalReloc() != 0) {
                            if (*temp_a1_2 == 0x35) {
                                sp84.unk0E = 0xB4;
                                sp84.unk0C = O53_FIELD(temp_s3_2, s16 *, 0xE4);
                            } else {
                                sp84.unk0E = 0xBA;
                                sp84.unk0C = O53_FIELD(temp_s3_2, s16 *, 0xE8);
                            }
                        } else {
                            temp_v0_7 = temp_s3_2 + &D_DC;
                            if (*temp_a1_2 == 0x35) {
                                sp84.unk0C = 0x19;
                                sp84.unk0E = 0x3E - *temp_v0_7;
                            } else {
                                sp84.unk0C = 0x1F;
                                sp84.unk0E = 0x44 - *temp_v0_7;
                            }
                        }
                        sp84.unk08 = 0;
                        sp84.unk04 = 0;
                        sp84.unk10 = 0;
                        sp84.unk00 = *(u32 *)(*temp_a1_2 * 4);
                        overlay53ExternalReloc(0, &sp84, 0, 0, 0xFF, 0xFF,
                                               0xFF, *temp_s1_2);
                    }
                }
                if ((overlay53ExternalReloc() == 0) &&
                    ((s32)*overlay53ExternalReloc() == 5) &&
                    (D_0 == 0) && (D_110 == 0)) {
                    overlay53ExternalReloc(1);
                    overlay53ExternalReloc(1);
                    overlay53ExternalReloc();
                    overlay53ExternalReloc(2, 4.0f, -1.0f, 0, 0, 0, 0);
                    overlay53ExternalReloc(0x12, 0, 0, 7, 1, 1);
                    overlay53ExternalReloc(3.0f, 0);
                    D_110 = (O53Unknown *)1;
                }
                var_fp++;
                sp6C += 4;
                sp70 += 4;
                if (var_fp == (O53Unknown *)2) {
                    overlay53ExternalReloc(0, 0);
                    overlay53ExternalReloc(0);
                } else {
                    goto loop_40;
                }
            }
        } else {
            goto loop_10;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o053/func_overlay_053_F0000240_189DBE8/func_overlay_053_F0000240_189DBE8.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_053_F0000240_189DBE8:start
 * symbol: func_overlay_053_F0000240_189DBE8
 * score: 636 differing words
 * frame: 0x178
 * relocations: 91
 * first-mismatch: +0x0
 * summary: 608/636 instructions, -112 bytes (was +308). Residual: loop store rotation, frame 0x178 vs 0xD8, unrecovered indexed tables.
 * PLATEAU-HANDOFF:func_overlay_053_F0000240_189DBE8:end
 */
