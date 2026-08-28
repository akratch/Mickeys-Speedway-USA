/*
 * Resident visual effects -- ROM 0x47A70-0x4BC40 (VRAM 0x80046E70).
 *
 * PROVENANCE: the translation-unit identity and the descriptive cone/wake
 * names are adapted from Jet Force Gemini's public decompilation, src/fx.c.
 * Mickey begins at JFG's fxFreeCone portion of that TU; the matching sequence
 * of texture, allocator, trigonometry and draw calls establishes the named
 * routines below. Externally referenced functions and unresolved JFG
 * placeholders retain Mickey address names. The bodies remain Mickey's
 * extracted assembly.
 */

#include "game/fx.h"

typedef struct FxConePoint {
    f32 x;
    f32 y;
    f32 z;
} FxConePoint;

typedef FxCone FxConeCoords;

typedef struct FxWakeRippleData {
    u8 pad0[0x70];
    void *texture;
    u8 mode;
    u8 active;
    s16 fade;
    s16 angle;
    s16 angleStep;
    f32 value7C;
    f32 value80;
    void *update;
} FxWakeRippleData;

typedef struct FxWakeTexture {
    u8 pad0[0x10];
    u16 length;
} FxWakeTexture;

typedef struct FxWakeUpdateOwner {
    u8 pad0[0x0C];
    f32 valueC;
    u8 pad10[4];
    f32 value14;
    u8 pad18[4];
    f32 value1C;
    u8 pad20[4];
    f32 value24;
    u8 pad28[0x2C];
    FxWakeRippleData *ripple;
} FxWakeUpdateOwner;

typedef struct FxWakeSegment {
    s32 x;
    s32 y;
    s32 z;
    u8 padC[2];
    s16 length;
} FxWakeSegment;

extern void func_80048080(s32 count, s16 arg1, s16 arg2, s16 arg3,
                          s32 arg4, s32 arg5, FxConePoint *points,
                          void *vertices, s32 alpha);
extern void viGetCurrentSize(s32 *width, s32 *height);
extern s16 Arctanf(f32 x, f32 y);
extern void wakeUpdate(s32 update, f32 x, f32 height, f32 z, s32 angle,
                       s32 delta);
extern f32 D_80083DE4;
extern void mathOneFloatPY(void *source, f32 *result, s16 angle);
extern void camSetScissor(FxGfx **dlist);
extern void func_80034920(FxGfx **dlist, void *table, FxGfx **arg2);

void func_80046E70(FxCone *cone) {
    FxConeTextureInfo *texture;
    FxConeTextureInfo *alternateTexture;

    texture = cone->texture.pointer;
    if (texture != 0) {
        func_800347A0(texture);
    }
    alternateTexture = cone->alternateTexture.pointer;
    if (alternateTexture != 0) {
        func_800347A0(alternateTexture);
    }
    mmFree(cone);
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_80046EC4.s")
void func_8004707C(FxCone *cone, s32 value2C, s32 value2D, s32 value2E,
                   s32 value30, s32 value31, s32 value32) {
    if (cone != 0) {
        cone->primRed = value2C;
        cone->primGreen = value2D;
        cone->primBlue = value2E;
        cone->envRed = value30;
        cone->envGreen = value31;
        cone->envBlue = value32;
    }
}
/* Workbench verdict: structure-mismatch, 121 differing words, first mismatch +0x44. */
/* Candidate: 150/149 instructions with the target -0x168 frame; relocation and CFG residuals remain, so it is not shape-exact. */
/* Shape status: one-word length delta; the helper loop and signed angle path are preserved for the permuter-ready pass. */
/* PROVENANCE: JFG's public src/fx.c establishes the corresponding cone routine and call roles; this body is reconstructed from Mickey's own m2c draft and typed layouts. */
#ifdef NON_MATCHING
void func_800470B0(FxCone *cone, s16 arg1, s16 arg2, s16 arg3, s16 arg4,
                   s16 arg5, f32 arg6, f32 arg7, f32 arg8) {
    FxConePoint points[17];
    FxConePoint *point;
    u8 *address;
    u8 *vertex;
    s32 angleStep;
    f32 var_f0;
    f32 var_f24;
    f32 temp_f6;
    s32 i;
    s32 j;

    if (cone->flags != 0) {
        angleStep = -0x10000 / (s32) cone->segmentCount;
        var_f0 = 0.0f;
        var_f24 = -arg8;
    } else {
        angleStep = 0x10000 / (s32) cone->segmentCount;
        var_f24 = 0.0f;
        var_f0 = -arg8;
    }
    points[0].x = 0.0f;
    points[0].y = 0.0f;
    points[0].z = var_f0;
    point = points + 1;
    i = 0;
    j = cone->segmentCount - 1;
    if (cone->segmentCount != 0) {
        do {
            point->x = (f32) (func_8002A8C0(i) * arg6);
            temp_f6 = func_8002A8BC(i) * arg7;
            point->z = var_f24;
            point++;
            i += angleStep;
            point[-1].y = temp_f6;
            j--;
        } while (j != 0);
    }
    address = (u8 *) cone;
    i = 0;
    do {
        func_80048080(cone->mode, arg1, arg2, arg3, (s32) arg4,
                      (s32) arg5, points, *(void **)(address + 8), 0xFF);
        i += 4;
        address += 4;
    } while (i < 8);
    vertex = cone->vertices;
    {
        s8 index;
        s8 next;

        index = 1;
        if ((s32) cone->segmentCount > 0) {
            do {
                next = index + 1;
                vertex[0] = 0;
                vertex[1] = index;
                vertex[2] = next;
                vertex[3] = 0;
                index = next;
                vertex += 0x10;
            } while ((s32) cone->segmentCount >= next);
        }
    }
    vertex[-0xE] = 1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_800470B0.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_80047304.s")
/* Workbench p5: mixed structure/register mismatch; 247/251 candidate/target instructions, 178 words from +0x0.
 * Lever: constant-audit and array declaration/loop spelling; loopunroll=0 won the flag sweep, while pointer and width/lifetime variants regressed.
 * Remains: candidate frame is 8 bytes larger and four instructions shorter, with a register/CFG cascade. */
#ifdef NON_MATCHING
/* Mickey-derived draft; JFG's corresponding fxMakeConeTextureCoords body is
 * also assembly-only and supplies no adaptable C source. */
void func_800475E8(FxConeCoords *cone, s16 angle) {
    FxConeTextureInfo *textureInfo;
    FxConeVertex *vertex;
    s32 width;
    s32 height;
    s32 currentAngle;
    s32 angleStep;
    s32 segmentCount;
    s32 i;
    s16 y[20];
    s16 x[20];

    currentAngle = angle;
    if (cone != 0) {
        textureInfo = cone->texture.pointer;
        if (textureInfo != 0) {
            width = textureInfo->width * 16;
            height = textureInfo->height * 16;
            vertex = cone->vertices;
            if (cone->segmentCount == 0) {
                f32 widthEdge = (f32)(width - 1);
                f32 scale = D_80083DE8;
                f32 heightEdge = (f32)(height - 1);
                i = 0;
                do {
                    f32 sine = func_8002A8C0(currentAngle);
                    f32 cosine = func_8002A8BC(currentAngle);

                    currentAngle += 0x2000;
                    y[i + 1] = (s32)(scale * sine) + width;
                    x[i + 1] = (s32)(scale * cosine) + height;
                    y[i + 9] = (s32)(widthEdge * sine) + width;
                    x[i + 9] = (s32)(heightEdge * cosine) + height;
                    i++;
                } while (i != 8);

                i = 31;
                do {
                    vertex->s0 = y[vertex->index0];
                    vertex->t0 = x[vertex->index0];
                    vertex->s1 = y[vertex->index1];
                    vertex->t1 = x[vertex->index1];
                    vertex->s2 = y[vertex->index2];
                    vertex->t2 = x[vertex->index2];
                    vertex++;
                    i--;
                } while (i != 0);
                segmentCount = 8;
                angleStep = 0x2000;
            } else {
                segmentCount = cone->segmentCount;
                angleStep = 0x10000 / segmentCount;
            }

            {
                s16 *yIt = y;
                s16 *xIt = x;
                s16 *xEnd = &x[segmentCount + 1];

                if (segmentCount >= 0) {
                    do {
                        *yIt = (s32)(func_8002A8C0(angle) *
                                     (f32)(width - 1)) + width;
                        *xIt = (s32)(func_8002A8BC(angle) *
                                     (f32)(height - 1)) + height;
                        angle += angleStep;
                        xIt++;
                        yIt++;
                    } while (xIt != xEnd);
                }
            }

            i = 0;
            if (segmentCount > 0) {
                while (i != (segmentCount & 3)) {
                    vertex->s0 = y[i];
                    vertex->t0 = x[i];
                    vertex->s1 = y[i + 1];
                    vertex->t1 = x[i + 1];
                    vertex->s2 = width;
                    vertex->t2 = height;
                    vertex++;
                    i++;
                }
                while (i != segmentCount) {
                    vertex[0].s0 = y[i + 0];
                    vertex[0].t0 = x[i + 0];
                    vertex[0].s1 = y[i + 1];
                    vertex[0].t1 = x[i + 1];
                    vertex[0].s2 = width;
                    vertex[0].t2 = height;
                    vertex[1].s0 = y[i + 1];
                    vertex[1].t0 = x[i + 1];
                    vertex[1].s1 = y[i + 2];
                    vertex[1].t1 = x[i + 2];
                    vertex[1].s2 = width;
                    vertex[1].t2 = height;
                    vertex[2].s0 = y[i + 2];
                    vertex[2].t0 = x[i + 2];
                    vertex[2].s1 = y[i + 3];
                    vertex[2].t1 = x[i + 3];
                    vertex[2].s2 = width;
                    vertex[2].t2 = height;
                    vertex[3].s0 = y[i + 3];
                    vertex[3].t0 = x[i + 3];
                    vertex[3].s1 = y[i + 4];
                    vertex[3].t1 = x[i + 4];
                    vertex[3].s2 = width;
                    vertex[3].t2 = height;
                    vertex += 4;
                    i += 4;
                }
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_800475E8.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_800479D4.s")

/*
 * PROVENANCE: the block-local display-list macro spelling below is adapted
 * from Jet Force Gemini include/f3ddkr.h. Mickey's own bytes establish every
 * invocation, argument, constant and operation order in func_80047CD8.
 */
#define FX_SHIFTL(value, shift, width) \
    ((u32)(((u32)(value) & ((1U << (width)) - 1U)) << (shift)))
#define FX_PIPE_SYNC(packet) { \
    FxGfx *_g = (FxGfx *)(packet); \
    _g->w0 = FX_SHIFTL(0xE7, 24, 8); \
    _g->w1 = 0; \
}
#define FX_SET_PRIM(packet, red, green, blue, alpha) { \
    FxGfx *_g = (FxGfx *)(packet); \
    _g->w0 = FX_SHIFTL(0xFA, 24, 8); \
    _g->w1 = FX_SHIFTL(red, 24, 8) | FX_SHIFTL(green, 16, 8) | \
             FX_SHIFTL(blue, 8, 8) | FX_SHIFTL(alpha, 0, 8); \
}
#define FX_SET_ENV(packet, red, green, blue, alpha) { \
    FxGfx *_g = (FxGfx *)(packet); \
    _g->w0 = FX_SHIFTL(0xFB, 24, 8); \
    _g->w1 = FX_SHIFTL(red, 24, 8) | FX_SHIFTL(green, 16, 8) | \
             FX_SHIFTL(blue, 8, 8) | FX_SHIFTL(alpha, 0, 8); \
}
#define FX_VERTEX_JFG(packet, address, count, first) { \
    FxGfx *_g = (FxGfx *)(packet); \
    _g->w0 = FX_SHIFTL(4, 24, 8) | \
             FX_SHIFTL(((count) << 3) | ((u32)(address) & 6) | (first), \
                       16, 8) | \
             FX_SHIFTL(((count) << 3) + ((count) << 1) + 8, 0, 16); \
    _g->w1 = (u32)(address); \
}
#define FX_POLYGON(packet, address, count, textured) { \
    FxGfx *_g = (FxGfx *)(packet); \
    _g->w0 = FX_SHIFTL((((count) - 1) << 4) | (textured), 16, 8) | \
             FX_SHIFTL(5, 24, 8) | FX_SHIFTL((count) * 16, 0, 16); \
    _g->w1 = (u32)(address); \
}

#ifdef NON_MATCHING
/* Workbench diagnostic full-TU copy: allocation-mismatch, 226/234 rows exact,
 * exact 234/-104 shape, eight register words from +0x298, and zero relocation
 * differences. The temp lane is identical; the pool first diverges at slot 28
 * as v1->a0, a0->a1, and a2->v0. There is no move/copy site for a source
 * coalescing lever, and no instrumented IDO is available for the forced-color
 * oracle. The configured TU still fails before this function on the adjacent
 * func_800475E8 block-local C99 declarations; that function is outside this
 * target's ownership. Remains: one callee-saved pool-color cascade; assembly
 * fallback stays canonical. */
/* Mickey-derived draft; JFG's corresponding fxDrawCone body is assembly-only. */
void func_80047CD8(FxGfx **dList, FxCone *cone, s32 flags, u8 alpha) {
    s32 hasTexture;

    if (cone != 0) {
        FX_PIPE_SYNC((*dList)++);
        if (flags & 0x200) {
            FX_SET_PRIM((*dList)++, cone->primRed, cone->primGreen,
                        cone->primBlue, alpha);
            FX_SET_ENV((*dList)++, cone->envRed, cone->envGreen,
                       cone->envBlue, 0);
        } else {
            FX_SET_PRIM((*dList)++, 0xFF, 0xFF, 0xFF, alpha);
            FX_SET_ENV((*dList)++, 0xFF, 0xFF, 0xFF, 0);
        }

        if (cone->texture.value != 0) {
            hasTexture = 1;
        } else {
            hasTexture = 0;
        }
        if (cone->segmentCount == 0) {
            FX_VERTEX_JFG((*dList)++,
                          cone->addresses[cone->addressIndex] + 0x80000000,
                          17, 0);
            func_800349A4(dList, cone->alternateTexture.value, flags, 0);
            FX_POLYGON((*dList)++, cone->vertices + 0x80000000, 16,
                       hasTexture);
            func_800349A4(dList, cone->texture.value, flags, 0);
            FX_POLYGON((*dList)++, cone->vertices + 0x80000200, 8,
                       hasTexture);
            func_800349A4(dList, cone->alternateTexture.value, flags, 0);
            FX_POLYGON((*dList)++, cone->vertices + 0x80000100, 16,
                       hasTexture);
        } else {
            func_800349A4(dList, cone->texture.value, flags, 0);
            FX_VERTEX_JFG((*dList)++,
                          cone->addresses[cone->addressIndex] + 0x80000000,
                          cone->mode, 0);
            FX_POLYGON((*dList)++, cone->vertices + 0x80000000,
                       cone->segmentCount, hasTexture);
        }

        FX_PIPE_SYNC((*dList)++);
        FX_SET_PRIM((*dList)++, 0xFF, 0xFF, 0xFF, 0xFF);
        FX_SET_ENV((*dList)++, 0xFF, 0xFF, 0xFF, 0);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_80047CD8.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_80048080.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/wakeAllocate.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_80048760.s")
void wakeFree(Wake *wake) {
    void *linked = wake->linked;

    if (linked != 0) {
        func_800347A0(linked);
    }
    mmFree(wake);
}
void func_80048980(WakeRipple *ripple) {
    void *linked = ripple->linked;

    if (linked != 0) {
        func_800347A0(linked);
    }
    if (ripple->wake != 0) {
        wakeFree(ripple->wake);
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/wakeUpdate.s")
/* Workbench verdict: structure-mismatch, 125 differing words, first mismatch +0x0. */
/* Candidate: 150/149 instructions with a -0x30 frame versus target -0x38; 88 structural words remain, so it is not shape-exact. */
/* Shape status: ripple fade/angle/vertex updates and both calls are present; frame and pointer-layout gap remains. */
/* PROVENANCE: JFG names the corresponding routine wakeUpdateRipple; this Mickey body uses only Mickey target offsets and calls. */
#ifdef NON_MATCHING
void func_80049000(FxWakeUpdateOwner *owner, s32 delta) {
    FxWakeRippleData *ripple;
    u8 mode;
    s16 angle;
    s16 step;
    s32 height;
    u8 *vertex;

    ripple = owner->ripple;
    if (ripple != 0) {
        if (ripple->active != 0) {
            ripple->fade = (s16) (ripple->fade + 0x20);
            if (ripple->fade >= 0x100) {
                ripple->fade = 0xFF;
            }
        } else {
            ripple->fade = (s16) (ripple->fade - 0x20);
            if (ripple->fade < 0) {
                ripple->fade = 0;
            }
        }
        step = ripple->angleStep;
        if (step != 0) {
            if (ripple->update != 0) {
                ripple->angle = (s16) (ripple->angle + (step * delta));
                while (ripple->angle >= (s32) ((FxWakeTexture *) ripple->texture)->length) {
                    ripple->angle = (s16) (ripple->angle - ((s32) ((FxWakeTexture *) ripple->texture)->length));
                }
            }
        }
        if (ripple->fade != 0) {
            mode = 1 - ripple->mode;
            ripple->mode = mode;
            height = (s32) ripple->value80;
            vertex = (u8 *) ripple + ((mode & 0xFF) * 0x28);
            *(s16 *) (vertex + 0x22) = (s16) height;
            vertex += 0x3E;
            *(s16 *) (vertex - 0x1E) = (s16) (s32) (owner->valueC + ripple->value7C);
            *(s16 *) (vertex - 0x1A) = (s16) (s32) (owner->value14 - ripple->value7C);
            *(s16 *) (vertex - 0x12) = (s16) height;
            *(s16 *) (vertex - 0x14) = (s16) (s32) (owner->valueC - ripple->value7C);
            *(s16 *) (vertex - 0x10) = (s16) (s32) (owner->value14 - ripple->value7C);
            *(s16 *) (vertex - 8) = (s16) height;
            *(s16 *) (vertex - 0xA) = (s16) (s32) (owner->valueC + ripple->value7C);
            *(s16 *) (vertex - 6) = (s16) (s32) (owner->value14 + ripple->value7C);
            *(s16 *) (vertex + 2) = (s16) height;
            *(s16 *) vertex = (s16) (s32) (owner->valueC - ripple->value7C);
            *(s16 *) (vertex + 4) = (s16) (s32) (owner->value14 + ripple->value7C);
        }
        angle = Arctanf(owner->value1C, owner->value24);
        if (ripple->update != 0) {
            wakeUpdate((s32) ripple->update, owner->valueC, ripple->value80,
                       owner->value14, angle, delta);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_80049000.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/wakeDraw.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_80049518.s")
void fxInit(void) {
    FxRecord *record;
    s32 i;

    record = D_800D5F58;
    i = 5;
    while (i--) {
        record->state = 0;
        record->flags = 0;
        record->status = 0;
        record++;
    }
    D_800D5F50 = 0;
    func_8004ACC4();
}
/* Mickey-derived body; JFG's corresponding fx.c routine is assembly-only. */
void func_8004978C(s32 index, s32 mask, s32 enable) {
    s32 unused[2];
    FxRecord *record;
    s32 count = 0;
    s32 andMask;
    s32 orMask;

    if (index == -1) {
        record = D_800D5F58;
        count = 5;
    } else if (index >= 0 && index < 5) {
        record = &D_800D5F58[index];
        count = 1;
    }
    if (count != 0) {
        andMask = ~mask;
        if (enable != 0) {
            andMask = -1;
            orMask = mask;
        } else {
            orMask = 0;
        }
        while (count--) {
            record->flags = (record->flags & andMask) | orMask;
            record++;
        }
    }
}
s32 func_80049828(s32 index, s32 mask) {
    if (index >= 0 && index < 5 && (D_800D5F5A[index].value & mask) != 0) {
        return 1;
    }
    return 0;
}
s32 func_80049864(s32 index) {
    if (index >= 0 && index < 5 && D_800D5F59[index].value != 0) {
        return 1;
    }
    return 0;
}
s32 func_8004989C(s32 index) {
    FxRecord *record;
    s32 color;

    if (index < 0 || index >= 5) {
        return 0;
    }
    record = &D_800D5F58[index];
    color = ((record->red & 0xF8) << 8) |
            ((record->green & 0xF8) << 3) |
            ((record->blue & 0xF8) >> 2);
    color |= color << 16;
    return color;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_800498FC.s")
void func_80049A8C(s32 index) {
    s32 count = 0;
    FxRecord *record;

    if (index == -1) {
        count = 5;
        record = D_800D5F58;
    } else if (index >= 0 && index < 5) {
        count = 1;
        record = &D_800D5F58[index];
    }
    while (count--) {
        record->state = 0;
        record->flags &= ~5;
        record->status = 0;
        record++;
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_80049B14.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_80049E4C.s")
void func_8004A0F0(void) {
    D_800D6038[0] = 0;
    D_800D6038[1] = 0;
    D_800D6040 = 0;
}
/* Workbench verdict: structure-mismatch, 155 differing words, first mismatch +0x0. */
/* Candidate: 156/157 instructions with a -0x60 frame versus target -0x58; 29 structural words remain, so it is not shape-exact. */
/* Shape status: nine-pixel glyph loop and VI/table relocation surface are preserved; stack/register gap remains. */
/* PROVENANCE: JFG's corresponding routine is assembly-only; this body is reconstructed from Mickey's own m2c draft and headers. */
#ifdef NON_MATCHING
void func_8004A10C(s32 *screen, u8 glyph, s32 x, s32 y, s32 arg4) {
    s32 width;
    s32 height;
    u32 *pattern;
    u16 *pixel;
    s32 glyphValue;
    s32 colorMask;
    s32 shift;
    s32 column;
    s32 rowBits;
    s32 bit;
    s32 intensity;
    s32 oldPixel;
    s32 maskedPixel;
    s32 value;

    glyphValue = glyph;
    viGetCurrentSize(&width, &height);
    colorMask = 0x7C0;
    shift = 6;
    pattern = D_8007D320;
    pixel = (u16 *) ((u8 *) screen + ((((y * width) + x) * 2)));
    if (arg4 != 0) {
        colorMask = 0xF800;
        shift = 0xB;
    }
    do {
        rowBits = *pattern;
        column = 1;
        intensity = 4;
        bit = rowBits & 7;
        rowBits >>= 3;
        if (bit != 0) {
            if (glyphValue & (1 << bit)) {
                intensity = 0x10;
            }
            oldPixel = *pixel;
            maskedPixel = oldPixel & colorMask;
            value = maskedPixel + (intensity << shift);
            if ((~colorMask & value) != 0) {
                value = colorMask;
            }
            *pixel = (oldPixel ^ maskedPixel) | value;
        }
        pixel++;
    loop_9:
        bit = rowBits & 7;
        rowBits >>= 3;
        if (bit != 0) {
            intensity = 4;
            if (glyphValue & (1 << bit)) {
                intensity = 0x10;
            }
            oldPixel = *pixel;
            maskedPixel = oldPixel & colorMask;
            value = maskedPixel + (intensity << shift);
            if ((~colorMask & value) != 0) {
                value = colorMask;
            }
            *pixel = (oldPixel ^ maskedPixel) | value;
        }
        bit = rowBits & 7;
        rowBits >>= 3;
        pixel++;
        if (bit != 0) {
            intensity = 4;
            if (glyphValue & (1 << bit)) {
                intensity = 0x10;
            }
            oldPixel = *pixel;
            maskedPixel = oldPixel & colorMask;
            value = maskedPixel + (intensity << shift);
            if ((~colorMask & value) != 0) {
                value = colorMask;
            }
            *pixel = (oldPixel ^ maskedPixel) | value;
        }
        bit = rowBits & 7;
        rowBits >>= 3;
        pixel++;
        if (bit != 0) {
            intensity = 4;
            if (glyphValue & (1 << bit)) {
                intensity = 0x10;
            }
            oldPixel = *pixel;
            maskedPixel = oldPixel & colorMask;
            value = maskedPixel + (intensity << shift);
            if ((~colorMask & value) != 0) {
                value = colorMask;
            }
            *pixel = (oldPixel ^ maskedPixel) | value;
        }
        bit = rowBits & 7;
        rowBits >>= 3;
        pixel++;
        if (bit != 0) {
            intensity = 4;
            if (glyphValue & (1 << bit)) {
                intensity = 0x10;
            }
            oldPixel = *pixel;
            maskedPixel = oldPixel & colorMask;
            value = maskedPixel + (intensity << shift);
            if ((~colorMask & value) != 0) {
                value = colorMask;
            }
            *pixel = (oldPixel ^ maskedPixel) | value;
        }
        column += 4;
        pixel++;
        if (column != 9) {
            goto loop_9;
        }
        pattern++;
        pixel += width - 9;
    } while (pattern != (u32 *) D_8007D364);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_8004A10C.s")
#endif
/* Plateau: workbench mixed constant/structure/register, stock -O2 is 74/76
 * instructions and 65 words from +0x8; buffer size and declaration order did not move the sp+0x50 text home.
 * Remaining: target's sp+0x54 cursor base and zero-index/glyph register web; prior flag and bounded-permuter passes found no exact. */
#ifdef NON_MATCHING
/* PROVENANCE: role adapted from JFG src/fx.c::func_8006DF90; both bodies are
 * assembly-only, so this reconstruction is Mickey-derived. */
void func_8004A380(s32 x, s32 y, s32 value, s32 minimumWidth, s32 arg4) {
    s32 length;
    s32 index;
    char *cursor;
    u8 glyph;
    u8 character;
    char text[32];

    length = 0;
    index = 0;
    cursor = text + index;
    sprintf(text, D_80083DE0, value);
    if (text[length] != '\0') {
        do {
            length++;
        } while (text[length] != '\0');
    }
    if (minimumWidth >= length) {
        do {
            glyph = D_8007D364[11];
            if (length < minimumWidth) {
                length++;
            } else {
                character = *cursor++;
                if (character == '-') {
                    glyph = D_8007D364[10];
                } else if (character >= '0' && character < ':') {
                    glyph = D_8007D364[character - '0'];
                }
            }
            func_8004A10C(D_800D2FA0, glyph, x, y, arg4);
            x += 10;
        } while (*cursor != '\0');
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_8004A380.s")
#endif
/* Mickey-derived body; JFG's corresponding fx.c function is assembly-only. */
void func_8004A4B0(s32 value0, s32 value2, s32 value4, s32 value6,
                   s32 value7) {
    s32 group;
    s32 *countPtr;
    FxSpdRecord *record;

    group = D_800D6040;
    countPtr = &D_800D6038[group];
    if (*countPtr < 4) {
        record = &D_800D5FF8[group][(*countPtr)++];
        record->value0 = value0;
        record->value2 = value2;
        record->value4 = value4;
        record->value6 = value6;
        record->value7 = value7;
    }
}
/* Mickey-derived body; JFG's corresponding fx.c function is assembly-only. */
void func_8004A51C(void) {
    s32 group;
    s32 count;
    FxSpdRecord *record;

    group = D_800D6040;
    count = D_800D6038[group];
    record = D_800D5FF8[group];
    D_800D6040 = group ^ 1;
    D_800D6038[D_800D6040] = 0;
    while (count--) {
        func_8004A380(record->value0, record->value2, record->value4,
                      record->value6, record->value7);
        record++;
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/fxSPDPRipple.s")
void fxQueueScreenEffect(s32 type, s32 value4, s32 value6, s32 value8,
                         s32 valueA, s32 valueC, s32 valueE, s32 value10) {
    FxScreenEffect *effect;

    if (D_8007D478 < 4) {
        effect = &D_800D6048[D_8007D478++];
        effect->type = type;
        effect->value4 = value4;
        effect->value6 = value6;
        effect->value8 = value8;
        effect->valueA = valueA;
        effect->valueC = valueC;
        effect->valueE = valueE;
        effect->value10 = value10;
    }
}
void func_8004A9CC(FxGfx **dList) {
    FxScreenEffect *effect;
    s32 index;

    effect = D_800D6048;
    index = 0;
    if (D_8007D478 > 0) {
        do {
            fxScreenEffect(dList, effect->type, effect->value4,
                           effect->value6, effect->value8, effect->valueA,
                           effect->valueC, effect->valueE, effect->value10);
            index++;
            effect++;
        } while (index < D_8007D478);
    }
    D_8007D478 = 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/fxScreenEffect.s")
#ifdef NON_MATCHING
/*
 * PROVENANCE: the descending four-slot loop skeleton is adapted from Jet
 * Force Gemini asm/nonmatchings/fx/func_8006FFF8.s. Mickey's own symbols and
 * instruction schedule establish the assignment order below.
 */
/* Plateau (near-miss p6): workbench mixed(structural:10, register:5), 14 words at 28 instructions; first +0x10.
 * Lever: structure-bucket/context audit found no new source-stable schedule beyond the closed typing/order probes.
 * Remains: callback/trap pool mapping and loop-counter schedule; assembly fallback stays canonical. */
void func_8004ACC4(void) {
    FxTextureCallback *callback;
    void **value0;
    void **value1;
    u8 *available;
    s32 i;
    FxTextureCallback trap;

    D_800D60A8 = 0;
    i = 3;
    trap = (FxTextureCallback)TrapDanglingJump;
    value0 = &D_800D60BC;
    value1 = &D_800D60CC;
    available = &D_800D60D3;
    callback = &D_8007D488;
    do {
        *value0 = 0;
        *value1 = 0;
        *available = trap == *callback;
        value0--;
        value1--;
        available--;
        callback--;
    } while (i--);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_8004ACC4.s")
#endif
s32 func_8004AD34(void) {
    FxTextureCallback callback;
    s32 index;

    index = 4;
    while (index--) {
        if ((1 << index) & D_800D60A8) {
            func_800320F0((s32)D_8007D47C + (index << 2));
            callback = (FxTextureCallback)D_8007D47C[index];
            if (callback != 0) {
                callback(index, D_800D6098[index], 0);
            }
        }
    }
    D_800D60A8 = 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_8004ADE8.s")
/* Workbench: structure-mismatch, 54/52 words, 48 positional differences from +0x04.
 * Tried constant audit, context lint, pool-vs-temp inlining, and pointer-lifetime placement.
 * The D_800D60C0 base remains a saved web, adding s7 and two boundary words. */
#ifdef NON_MATCHING
/* Mickey-derived body; JFG's fxCpuTextureFlush is assembly-only. */
void func_8004AF68(void) {
    register s32 offset;
    register s32 *value0;
    register s32 i;
    register u8 *available;
    s32 *value1;
    void *allocation;

    offset = 12;
    value0 = (s32 *)&D_800D60BC;
    i = 3;
    available = &D_800D60D3;
    do {
        allocation = (void *)*value0;
        if (allocation != 0) {
            value1 = (s32 *)(offset + (s32)D_800D60C0);
            mmFree(allocation);
            mmFree((void *)*value1);
            *value0 = 0;
            *value1 = 0;
        }
        if (*available != 0) {
            *(FxTextureCallback *)((u8 *)D_8007D47C + offset) =
                (FxTextureCallback)TrapDanglingJump;
        }
        value0--;
        available--;
        offset -= 4;
    } while (i--);
    D_800D60A8 = 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/fx/func_8004AF68.s")
#endif
