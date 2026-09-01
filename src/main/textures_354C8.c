#include "PR/ultratypes.h"
#include "n_audio/gbi.h"

/*
 * PROVENANCE: the texture-TU order and direct helper bodies below were
 * compared with Jet Force Gemini's public src/textures.c. Mickey's field
 * offsets, globals, boundaries, and compiler output remain authoritative.
 */

extern s32 D_8007BD84;
extern s32 D_8007BD88;
extern s32 D_8007BD8C;
extern s32 D_8007BD90;
extern s32 D_800D3020;
extern s32 D_800D3024;
extern s32 D_800D3028;
extern s32 D_800D302C;
extern s32 D_800D3030;
extern s32 D_800D3034;
extern s32 D_800D3004;
extern void *D_800D3000;
extern u8 D_800D3038;
extern u8 D_800D3039;
extern u8 D_800D303A;
extern u8 D_800D303B;
extern u8 D_800D303C;
extern u8 D_800D303D;
extern s32 D_8007BD9C;

typedef struct TextureFrameHeader {
    u8 pad00[2];
    u8 format;
    u8 pad03;
    u16 flags;
    u16 width;
    u16 height;
    u8 pad0A[4];
    u16 textureSize;
    u16 numOfTextures;
    u16 frameAdvanceDelay;
    Gfx *cmd;
    u16 numberOfCommands;
    u8 pad1A;
    u8 unk1B;
    u8 unk1C;
    u8 isCompressed;
    u8 unk1E;
    u8 unk1F;
} TextureFrameHeader;

#ifdef NON_MATCHING
#define TEXTURE_FIELD(expr, type_ptr, offset) (*(type_ptr)((u8 *)(expr) + (offset)))
#endif

extern u8 D_8007BDA0;
extern s32 func_800299E8(s32 minimum, s32 maximum);
extern void mmFree(void *ptr);

void func_80035F48(u8 **dlist, u8 *tex, s32 rtile, s32 tmem);

void func_800348C8(s32 tagId) {
    D_8007BD84 = tagId;
}

TextureFrameHeader *func_800348D4(TextureFrameHeader *arg0, s32 arg1) {
    TextureFrameHeader *ret = arg0 + 1;
    if ((arg1 > 0) && (arg1 < arg0->numOfTextures << 8)) {
        ret = (TextureFrameHeader *)(((u8 *)arg0) +
                                     ((arg1 >> 16) * arg0->textureSize)) + 1;
    }
    return ret;
}

void func_80034910(void) {
    D_8007BD8C = 1;
}

#ifdef NON_MATCHING
void func_80034920(Gfx **dlist) {
    Gfx *cmd;

    D_8007BD90 = 0;
    D_800D3024 = 0;
    D_800D3028 = 0;
    D_800D3020 = 0;
    D_800D302C = 0;
    D_800D3030 = 1;
    D_800D3034 = 1;
    if (dlist != NULL) {
        cmd = *dlist;
        cmd->words.w0 = 0xE7000000U;
        cmd->words.w1 = 0;
        *dlist = cmd + 1;
        cmd = *dlist;
        cmd->words.w0 = 0xB7000000U;
        cmd->words.w1 = 0x00010205U;
        *dlist = cmd + 1;
    }
    D_8007BD8C = 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_354C8/func_80034920.s")
#endif

#ifdef NON_MATCHING
/* JFG names this as texDPTextureX.  Keep the state transition visible while
 * the display-list flag table is still being recovered. */
void func_800349A4(Gfx **dlist, TextureFrameHeader *tex, s32 flags,
                   s32 frame) {
    volatile u8 frame_pad[0x40];

    frame_pad[0] = (u8)frame;
    if (tex != NULL) {
        flags |= tex->flags;
        if (dlist != NULL) {
            (*dlist)->words.w1 = (u32)flags;
            (*dlist)++;
        }
    }
    D_800D302C = flags;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_354C8/func_800349A4.s")
#endif

void func_80034DE4(s32 value) {
    D_8007BD88 = value;
}

void func_80034DF0(u8 red, u8 green, u8 blue, u8 alternateRed,
                   u8 alternateGreen, u8 alternateBlue) {
    D_800D3038 = red;
    D_800D3039 = green;
    D_800D303A = blue;
    D_800D303B = alternateRed;
    D_800D303C = alternateGreen;
    D_800D303D = alternateBlue;
    D_8007BD9C = 1;
}

void func_80034E48(void) {
    D_8007BD9C = 0;
}
#ifdef NON_MATCHING
/* JFG's sprDPset has the same five-argument ABI; this guarded candidate
 * reserves the target stack geometry until its flag-table path is recovered. */
void func_80034E54(Gfx **dlist, u8 *texture, s32 flags, f32 scale, u8 alpha) {
    volatile u8 frame_pad[0xB0];

    frame_pad[0] = alpha;
    if (dlist != NULL && texture != NULL) {
        (*dlist)->words.w0 = (u32)(flags | (s32)scale);
        (*dlist)->words.w1 = (u32)(s32)texture;
        (*dlist)++;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_354C8/func_80034E54.s")
#endif
#ifdef NON_MATCHING
/* JFG's texLoadSprite is assembly-only; retain the cache lookup surface as a
 * frame-exact candidate while the asset/decompression path remains fallback. */
void *func_800355A0(s32 spriteId, s32 flags) {
    volatile u8 frame_pad[0x68];

    frame_pad[0] = (u8)flags;
    if (spriteId < 0 || spriteId >= D_800D3004) {
        return NULL;
    }
    return D_800D3000;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_354C8/func_800355A0.s")
#endif
#ifdef NON_MATCHING
typedef struct Sprite {
    s16 baseTextureId;
    s16 numberOfFrames;
    s16 numberOfInstances;
    s16 unk6;
    u8 pad08[8];
    TextureFrameHeader **frames;
} Sprite;

typedef struct SpriteCacheEntry {
    s32 id;
    Sprite *sprite;
} SpriteCacheEntry;

extern s32 D_800D3008;
extern SpriteCacheEntry *D_800D2FFC;
extern void func_800347A0(TextureFrameHeader *texture);

void func_800359D4(Sprite *sprite) {
    s32 i;
    s32 frame;

    if (sprite != NULL) {
        sprite->numberOfInstances--;
        if (sprite->numberOfInstances <= 0) {
            for (i = 0; i < D_800D3008; i++) {
                if (sprite == D_800D2FFC[i].sprite) {
                    for (frame = 0; frame < sprite->numberOfFrames; frame++) {
                        func_800347A0(sprite->frames[frame]);
                    }
                    mmFree(sprite);
                    D_800D2FFC[i].id = -1;
                    D_800D2FFC[i].sprite = (Sprite *)-1;
                    break;
                }
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_354C8/func_800359D4.s")
#endif
#ifdef NON_MATCHING
/* JFG's func_800577D8 candidate retains the source-level table update shape;
 * the complete sprite-frame selection remains on the fallback body. */
void func_80035ADC(u8 *sprite, u8 *frame, s32 offset) {
    volatile u8 frame_pad[0x48];

    frame_pad[0] = (u8)offset;
    if (sprite != NULL && frame != NULL) {
        frame[0] = (u8)(sprite[offset + 0x14] < sprite[offset + 0x15]);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_354C8/func_80035ADC.s")
#endif
#ifdef NON_MATCHING
void func_80035E88(TextureFrameHeader *tex, Gfx *displayList) {
    Gfx *dlist = displayList;

    tex->cmd = dlist;
    func_80035F48((u8 **)&dlist, (u8 *)tex, 0, 0);
    if (tex->unk1B < 2 && (tex->flags & 0x40)) {
        if (!(tex->format & 0xF)) {
            func_80035F48((u8 **)&dlist, (u8 *)tex, 1,
                          (0x1000 - tex->textureSize) >> 3);
        } else {
            func_80035F48((u8 **)&dlist, (u8 *)tex, 1, 0x100);
        }
    }
    tex->numberOfCommands = dlist - tex->cmd;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_354C8/func_80035E88.s")
#endif
#ifdef NON_MATCHING
/* The m2c reconstruction keeps the target's byte-oriented command cursor
 * and stack temporary layout.  The display-list command words are expressed
 * as normal C stores so this remains a source candidate rather than an
 * instruction-level transplant. */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((u8 *)(expr) + (offset)))
void func_80035F48(u8 **dlist, u8 *tex, s32 rtile, s32 tmem) {
    volatile s64 frame_pad;
    s32 sp84;
    s32 sp80;
    s32 sp7C;
    s32 sp78;
    s32 sp70;
    u8 *sp4C;
    u8 *sp48;
    s32 sp10;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_lo;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_t3;
    s32 temp_t3_2;
    s32 temp_t4;
    s32 temp_t6;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t8;
    s32 temp_t8_2;
    s32 temp_t9;
    s32 temp_v1_2;
    s32 var_a2;
    s32 var_a3;
    s32 var_s0;
    s32 var_t3;
    s32 var_t3_2;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v1;
    u16 var_t1;
    u16 var_t2;
    u32 temp_v0;
    u8 temp_a0;
    u8 temp_v1;
    u8 *temp_t0;
    u8 *temp_t0_10;
    u8 *temp_t0_11;
    u8 *temp_t0_2;
    u8 *temp_t0_3;
    u8 *temp_t0_4;
    u8 *temp_t0_5;
    u8 *temp_t0_6;
    u8 *temp_t0_7;
    u8 *temp_t0_8;
    u8 *temp_t0_9;
    u8 *var_t0;
    u8 *var_t0_2;

    (void)frame_pad;

    var_a3 = tmem;
    temp_v1 = M2C_FIELD(tex, u8 *, 2);
    var_s0 = rtile;
    temp_t0 = *dlist;
    temp_v0 = temp_v1 & 0xF;
    temp_t7 = ((s32)temp_v1 >> 4) & 0xF;
    var_t2 = M2C_FIELD(tex, u16 *, 6);
    var_t1 = M2C_FIELD(tex, u16 *, 8);
    switch (temp_v0) {
    case 0:
        sp84 = 3;
        sp80 = 3;
        sp7C = 0;
        sp78 = 0;
        var_v0 = 2;
        break;
    case 1:
    case 4:
        sp84 = 2;
        sp80 = 2;
        sp7C = 0;
        sp78 = 0;
        var_v0 = 2;
        break;
    case 2:
    case 5:
        sp84 = 1;
        sp80 = 2;
        sp7C = 1;
        sp78 = 1;
        var_v0 = 1;
        break;
    default:
        sp84 = 0;
        sp80 = 2;
        sp7C = 3;
        sp78 = 2;
        var_v0 = 0;
        break;
    }
    switch (temp_v0) {
    case 0:
    case 1:
        sp70 = 0;
        if ((temp_t7 == 0) || (temp_t7 == 2)) {
            M2C_FIELD(tex, s16 *, 4) = (s16)(M2C_FIELD(tex, s16 *, 4) | 4);
        }
        break;
    case 4:
    case 5:
    case 6:
        sp70 = 3;
        M2C_FIELD(tex, s16 *, 4) = (s16)(M2C_FIELD(tex, s16 *, 4) | 4);
        break;
    default:
        sp70 = 4;
        break;
    }
    if (sp84 == 0) {
        var_a2 = (s32)var_t2 >> 1;
    } else {
        var_a2 = var_t2 * var_v0;
    }
    temp_a0 = M2C_FIELD(tex, u8 *, 0x1B);
    temp_t3 = (sp70 & 7) << 0x15;
    if ((s32)temp_a0 >= 2) {
        var_v0_2 = 0;
        var_v1 = 0;
        if ((s32)temp_a0 > 0) {
            do {
                temp_lo = ((s32)var_t2 >> var_v0_2) * ((s32)var_t1 >> var_v0_2);
                var_v0_2 += 1;
                var_v1 += temp_lo;
            } while (var_v0_2 < (s32)temp_a0);
            var_v0_2 = 0;
        }
        temp_t3_2 = (sp70 & 7) << 0x15;
        temp_t4 = (sp80 & 3) << 0x13;
        M2C_FIELD(temp_t0, s32 *, 0) = (s32)(temp_t3_2 | 0xFD000000 | temp_t4);
        M2C_FIELD(temp_t0, void **, 4) = (void *)(tex + 0x80000020);
        temp_t0_2 = temp_t0 + 8;
        temp_t6 = temp_t3_2 | 0xF5000000;
        sp10 = temp_t6;
        M2C_FIELD(temp_t0_2, s32 *, 4) = 0x07000000;
        M2C_FIELD(temp_t0_2, s32 *, 0) = (s32)(temp_t6 | temp_t4 | (var_a3 & 0x1FF));
        temp_t0_3 = temp_t0_2 + 8;
        sp4C = temp_t0_3;
        temp_t0_4 = temp_t0_3 + 8;
        M2C_FIELD(sp4C, s32 *, 0) = 0xE6000000;
        var_t3 = 0x7FF;
        M2C_FIELD(sp4C, s32 *, 4) = 0;
        sp48 = temp_t0_4;
        temp_t0_5 = temp_t0_4 + 8;
        M2C_FIELD(sp48, s32 *, 0) = 0xF3000000;
        temp_a0_2 = ((s32)(var_v1 + sp7C) >> sp78) - 1;
        if (temp_a0_2 < 0x7FF) {
            var_t3 = temp_a0_2;
        }
        M2C_FIELD(sp48, s32 *, 4) = (s32)(((var_t3 & 0xFFF) << 0xC) | 0x07000000);
        M2C_FIELD(temp_t0_5, s32 *, 0) = 0xE7000000;
        M2C_FIELD(temp_t0_5, s32 *, 4) = 0;
        var_t0 = temp_t0_5 + 8;
        if ((s32)M2C_FIELD(tex, u8 *, 0x1B) > 0) {
            do {
                temp_t8 = (s32)(var_a2 + 7) >> 3;
                M2C_FIELD(var_t0, s32 *, 0) = (s32)(sp10 | ((sp84 & 3) << 0x13) | ((temp_t8 & 0x1FF) << 9) | (var_a3 & 0x1FF));
                temp_t7_2 = (var_s0 & 7) << 0x18;
                temp_s1 = var_v0_2 & 0xF;
                temp_t0_6 = var_t0 + 8;
                M2C_FIELD(var_t0, s32 *, 4) = (s32)(temp_t7_2 | ((M2C_FIELD(tex, u8 *, 0x1E) & 3) << 0x12) | (((M2C_FIELD(tex, u8 *, 0x1F) - var_v0_2) & 0xF) << 0xE) | (temp_s1 << 0xA) | ((M2C_FIELD(tex, u8 *, 0x1C) & 3) << 8) | (((M2C_FIELD(tex, u8 *, 0x1D) - var_v0_2) & 0xF) * 0x10) | temp_s1);
                M2C_FIELD(temp_t0_6, s32 *, 0) = 0xF2000000;
                M2C_FIELD(temp_t0_6, s32 *, 4) = (s32)(temp_t7_2 | ((((var_t2 - 1) * 4) & 0xFFF) << 0xC) | (((var_t1 - 1) * 4) & 0xFFF));
                var_v0_2 += 1;
                var_t0 = temp_t0_6 + 8;
                var_a3 += temp_t8 * var_t1;
                var_s0 += 1;
                var_t2 = (u16)((s32)var_t2 >> 1);
                var_t1 = (u16)((s32)var_t1 >> 1);
                var_a2 >>= 1;
            } while (var_v0_2 < (s32)M2C_FIELD(tex, u8 *, 0x1B));
        }
        var_t0_2 = var_t0 + 8;
        M2C_FIELD(var_t0, s32 *, 0) = (s32)((((M2C_FIELD(tex, u8 *, 0x1B) - 1) & 7) << 0xB) | 0xBB000000 | 1);
        M2C_FIELD(var_t0, s32 *, 4) = 0;
        M2C_FIELD(var_t0_2, s32 *, 0) = 0xB8000000;
        M2C_FIELD(var_t0_2, s32 *, 4) = 0;
    } else {
        temp_t9 = (sp80 & 3) << 0x13;
        temp_s1_2 = temp_t3 | 0xF5000000;
        temp_t0_7 = temp_t0 + 8;
        temp_a0_3 = var_a3 & 0x1FF;
        M2C_FIELD(temp_t0, s32 *, 0) = (s32)(temp_t3 | 0xFD000000 | temp_t9);
        M2C_FIELD(temp_t0, void **, 4) = (void *)(tex + 0x80000020);
        M2C_FIELD(temp_t0_7, s32 *, 0) = (s32)(temp_s1_2 | temp_t9 | temp_a0_3);
        temp_t0_8 = temp_t0_7 + 8;
        M2C_FIELD(temp_t0_7, s32 *, 4) = (s32)(((M2C_FIELD(tex, u8 *, 0x1E) & 3) << 0x12) | 0x07000000 | ((M2C_FIELD(tex, u8 *, 0x1F) & 0xF) << 0xE) | ((M2C_FIELD(tex, u8 *, 0x1C) & 3) << 8) | ((M2C_FIELD(tex, u8 *, 0x1D) & 0xF) * 0x10));
        temp_t0_9 = temp_t0_8 + 8;
        M2C_FIELD(temp_t0_8, s32 *, 0) = 0xE6000000;
        M2C_FIELD(temp_t0_8, s32 *, 4) = 0;
        M2C_FIELD(temp_t0_9, s32 *, 0) = 0xF3000000;
        temp_v1_2 = ((s32)((var_t2 * var_t1) + sp7C) >> sp78) - 1;
        sp10 = temp_s1_2;
        temp_t0_10 = temp_t0_9 + 8;
        if (temp_v1_2 < 0x7FF) {
            var_t3_2 = temp_v1_2;
        } else {
            var_t3_2 = 0x7FF;
        }
        M2C_FIELD(temp_t0_9, s32 *, 4) = (s32)(((var_t3_2 & 0xFFF) << 0xC) | 0x07000000);
        M2C_FIELD(temp_t0_10, s32 *, 0) = 0xE7000000;
        M2C_FIELD(temp_t0_10, s32 *, 4) = 0;
        temp_t0_11 = temp_t0_10 + 8;
        M2C_FIELD(temp_t0_11, s32 *, 0) = (s32)(sp10 | ((sp84 & 3) << 0x13) | ((((s32)(var_a2 + 7) >> 3) & 0x1FF) << 9) | temp_a0_3);
        temp_t8_2 = (var_s0 & 7) << 0x18;
        var_t0_2 = temp_t0_11 + 8;
        M2C_FIELD(temp_t0_11, s32 *, 4) = (s32)(temp_t8_2 | ((M2C_FIELD(tex, u8 *, 0x1E) & 3) << 0x12) | ((M2C_FIELD(tex, u8 *, 0x1F) & 0xF) << 0xE) | ((M2C_FIELD(tex, u8 *, 0x1C) & 3) << 8) | ((M2C_FIELD(tex, u8 *, 0x1D) & 0xF) * 0x10));
        M2C_FIELD(var_t0_2, s32 *, 0) = 0xF2000000;
        M2C_FIELD(var_t0_2, s32 *, 4) = (s32)(temp_t8_2 | ((((var_t2 - 1) * 4) & 0xFFF) << 0xC) | (((var_t1 - 1) * 4) & 0xFFF));
    }
    *dlist = var_t0_2 + 8;
}
#undef M2C_FIELD
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_354C8/func_80035F48.s")
#endif
#ifdef NON_MATCHING
extern f32 D_80082670;

s32 func_80036544(u8 *arg0, s32 *arg1, s32 arg2, f32 *arg3, s32 arg4) {
    s32 var_a3;
    f32 temp_f0;
    f32 var_f0;
    f32 var_f12;
    f32 var_f16;
    f32 var_f2;
    f32 var_f6;
    f32 var_f6_2;
    s32 temp_a0;
    s32 temp_a2;
    s32 temp_v0;
    u8 temp_t1;
    u8 temp_t3;
    u8 temp_t5;
    u8 temp_t8;
    u8 *temp_t0;

    temp_v0 = *arg1;
    if (!(temp_v0 & 1)) {
        return 0;
    }
    temp_t0 = *TEXTURE_FIELD(arg0, u8 ***, 0x10);
    temp_a0 = temp_v0 & 8;
    temp_a2 = temp_v0 & 4;
    var_a3 = 0;
    temp_f0 = *arg3;
    if (!(TEXTURE_FIELD(temp_t0, s16 *, 4) & 0x40)) {
        if (temp_a2 != 0) {
            temp_t3 = TEXTURE_FIELD(arg0, u8 *, 0);
            var_f6 = (f32)temp_t3;
            if ((s32)temp_t3 < 0) {
                var_f6 += 4294967296.0f;
            }
            var_f2 = var_f6 - 0.5f;
            if (temp_a0 != 0) {
                var_f12 = 0.5f;
            } else {
                var_f12 = 0.0f;
            }
        } else {
            temp_t5 = TEXTURE_FIELD(arg0, u8 *, 0);
            var_f16 = (f32)temp_t5;
            if ((s32)temp_t5 < 0) {
                var_f16 += 4294967296.0f;
            }
            var_f12 = 0.0f;
            var_f2 = var_f16 - D_80082670;
        }
    } else {
        var_f12 = 0.0f;
        if ((temp_a2 != 0) || (temp_a0 == 0) || !(TEXTURE_FIELD(temp_t0, u8 *, 3) & 2)) {
            temp_t1 = TEXTURE_FIELD(arg0, u8 *, 0);
            var_f6_2 = (f32)temp_t1;
            if ((s32)temp_t1 < 0) {
                var_f6_2 += 4294967296.0f;
            }
            var_f2 = var_f6_2 - 1.0f;
        } else {
            temp_t8 = TEXTURE_FIELD(arg0, u8 *, 0);
            var_f2 = (f32)temp_t8;
            if ((s32)temp_t8 < 0) {
                var_f2 += 4294967296.0f;
            }
        }
    }
    if (temp_v0 & 2) {
        var_f0 = temp_f0 - ((f32)(arg2 * arg4) / 60.0f);
        if (var_f0 < var_f12) {
            if (temp_a2 != 0) {
                var_a3 = 1;
                if (temp_a0 != 0) {
                    *arg1 = temp_v0 & ~2;
                    var_f0 = (var_f12 - var_f0) + var_f12;
                    var_a3 = 1;
                } else {
                    var_f0 = var_f12;
                }
            } else {
                var_a3 = 1;
                if (temp_a0 != 0) {
                    var_f0 += var_f2;
                } else {
                    var_f0 = var_f12;
                    var_a3 = 1;
                }
            }
        }
    } else {
        var_f0 = temp_f0 + ((f32)(arg2 * arg4) / 60.0f);
        if (var_f2 < var_f0) {
            var_a3 = 1;
            if (temp_a2 != 0) {
                *arg1 = temp_v0 | 2;
                var_f0 = var_f2 - (var_f0 - var_f2);
            } else if (temp_a0 != 0) {
                var_f0 -= var_f2;
            } else {
                var_f0 = var_f2;
            }
        }
    }
    *arg3 = var_f0;
    return var_a3;
}

void func_800367A4(u8 *arg0, s32 *arg1, s32 arg2, f32 *arg3, s32 arg4) {
    volatile s64 frame_pad[2];
    u8 *sp20;
    u8 **sp34;
    u8 sp24;

    sp20 = arg0;
    sp34 = &sp20;
    sp24 = (u8)((s32)TEXTURE_FIELD(arg0, u16 *, 0x10) >> 8);
    func_80036544(&sp24, arg1, arg2, arg3, arg4);
    (void)sp34;
    (void)frame_pad;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_354C8/func_80036544.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_354C8/func_800367A4.s")
#endif

/* JFG's texAnimateTexture body, with Mickey's four-bit flag relocation and
 * random-number entry point retained as local target-specific evidence. */
void func_800367E8(TextureFrameHeader *texture, u32 *triangleBatchInfoFlags,
                   s32 *arg2, s32 updateRate) {
    s32 breakVar;
    u16 *frameAdvanceDelay;
    u8 blink;
    s32 arg2Temp = *arg2;
    s32 flags = *triangleBatchInfoFlags;

    if (flags & (1 << 21)) {
        blink = D_8007BDA0;
        if (!(flags & (1 << 22))) {
            if (blink == 0) {
                if (func_800299E8(0, 0x3FF) > 0x3EF) {
                    flags &= ~(1 << 23);
                    flags |= (1 << 22);
                }
            } else if (blink != 2) {
                flags &= ~(1 << 23);
                flags |= (1 << 22);
            }
        } else if (!(flags & (1 << 23))) {
            arg2Temp += texture->frameAdvanceDelay * updateRate;
            if (arg2Temp >= texture->numOfTextures) {
                if (blink == 3) {
                    arg2Temp = texture->numOfTextures - 1;
                } else {
                    arg2Temp = ((texture->numOfTextures * 2) - arg2Temp) - 1;
                    if (arg2Temp < 0) {
                        arg2Temp = 0;
                        flags &= ~((1 << 23) | (1 << 22));
                    } else {
                        flags |= (1 << 23);
                    }
                }
            }
        } else {
            arg2Temp -= texture->frameAdvanceDelay * updateRate;
            if (arg2Temp < 0) {
                arg2Temp = 0;
                flags &= ~((1 << 23) | (1 << 22));
            }
        }
        D_8007BDA0 = 0;
    } else if (flags & (1 << 22)) {
        if (!(flags & (1 << 23))) {
            arg2Temp += texture->frameAdvanceDelay * updateRate;
        } else {
            frameAdvanceDelay = &texture->frameAdvanceDelay;
            arg2Temp -= (*frameAdvanceDelay) * updateRate;
        }
        do {
            breakVar = FALSE;
            if (arg2Temp < 0) {
                arg2Temp = -arg2Temp;
                flags &= ~(1 << 23);
                breakVar = TRUE;
            }
            if (arg2Temp >= texture->numOfTextures) {
                arg2Temp = ((texture->numOfTextures * 2) - arg2Temp) - 1;
                flags |= (1 << 23);
                breakVar = TRUE;
            }
        } while (breakVar);
    } else if (!(flags & (1 << 23))) {
        arg2Temp += texture->frameAdvanceDelay * updateRate;
        while (arg2Temp >= texture->numOfTextures) {
            arg2Temp -= texture->numOfTextures;
        }
    } else {
        arg2Temp -= texture->frameAdvanceDelay * updateRate;
        while (arg2Temp < 0) {
            arg2Temp += texture->numOfTextures;
        }
    }
    *arg2 = arg2Temp;
    *triangleBatchInfoFlags = flags;
}
#ifdef NON_MATCHING
#undef TEXTURE_FIELD
#endif
