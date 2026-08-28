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
u8 *D_8007944C = 0;
u8 *D_80079450 = 0;
u8 *D_80079454 = 0;
s32 D_80079458 = 0;
u16 D_8007945C[2] = { 0, 0x4000 };
extern s32 D_800CB278;
extern s32 D_800CB27C;
extern s32 D_800CB280;
extern s32 D_800CB284;
extern s32 D_800CB288;
extern void *func_8002B280(s32 size, s32 tag);
extern void mmFree(void *ptr);
extern s32 getXZCompareMask(void *grid, s32 xMin, s32 zMin, s32 xMax, s32 zMax);
extern s32 mathXZInTri(s32 x, s32 z, void *a, void *b, void *c);
extern f32 D_80079464[];
extern f32 D_800CB28C;

typedef struct ShadowQueryVolume {
    u8 pad0[0x6C];
    s16 minY6C;
    s16 maxY6E;
} ShadowQueryVolume;

typedef struct ShadowQuery {
    u8 pad0[0xC];
    f32 x0C;
    f32 y10;
    f32 z14;
    u8 pad18[0x16];
    s16 sector2E;
    u8 pad30[0x10];
    ShadowQueryVolume *volume40;
    u8 pad44[0xC];
    f32 *value50;
} ShadowQuery;

typedef struct ShadowWorld {
    u8 pad0[4];
    void *sectors4;
    u32 *grid8;
} ShadowWorld;

typedef struct ShadowSector {
    u8 *vertices0;
    u8 *triangles4;
    u8 pad8[4];
    struct ShadowBlock *blocksC;
    u32 *masks10;
    u8 pad14[0x10];
    s16 blockCount24;
} ShadowSector;

typedef struct ShadowBlock {
    u8 pad0[6];
    u8 vertexBase6;
    u8 pad7;
    s16 firstVertex8;
    u8 padA[2];
    u32 flagsC;
    u8 pad10[8];
    s16 lastVertex18;
} ShadowBlock;

typedef struct ShadowTriangle {
    u8 pad0;
    u8 vertex1;
    u8 vertex2;
    u8 vertex3;
} ShadowTriangle;

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
#pragma GLOBAL_ASM("asm/nonmatchings/main/shadows/func_80017140.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/shadows/func_80017660.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/shadows/func_80017BCC.s")
/* Workbench verdict: structure-mismatch, 179 differing words, first mismatch +0x0. */
/* Candidate: 204/206 instructions with the exact -0x90 frame; two instruction and relocation-position residuals remain. */
/* Shape status: sector/block loops, three-vertex bounds test, and fade update are preserved, but the candidate is not shape-exact. */
/* PROVENANCE: Mickey's m2c control-flow draft and resident shadow offsets supply this reconstruction; no external body is copied. */
#ifdef NON_MATCHING
void func_800180B4(ShadowQuery *query) {
    ShadowQueryVolume *volume;
    ShadowWorld *world;
    ShadowSector *sector;
    ShadowBlock *block;
    ShadowTriangle *triangle;
    ShadowTriangle *nextTriangle;
    u32 flags;
    u32 maskWord;
    s32 y;
    s32 yMin;
    s32 yMax;
    s32 sectorIndex;
    s32 mask;
    s32 blockOffset;
    s32 blockNumber;
    s32 vertex;
    s32 vertexOffset;
    s32 triangleNumber;
    s16 lowY;
    s16 highY;
    s16 currentY;
    f32 *value;
    f32 oldValue;
    f32 targetValue;
    s32 done;

    volume = query->volume40;
    y = (s32) query->y10;
    yMax = y + volume->maxY6E;
    yMin = y + volume->minY6C;
    done = 0;
    sectorIndex = query->sector2E;
    if (sectorIndex != -1) {
        world = (ShadowWorld *) D_800CB284;
        mask = getXZCompareMask(
            (u8 *) world->grid8 + (sectorIndex * 0xC),
            (s32) (query->x0C - 16.0f),
            (s32) (query->z14 - 16.0f),
            (s32) (query->x0C + 16.0f),
            (s32) (query->z14 + 16.0f));
        blockNumber = 0;
        sector = (ShadowSector *) ((u8 *) world->sectors4 + (sectorIndex << 6));
        blockOffset = 0;
        block = sector->blocksC;
        if (sector->blockCount24 > 0) {
            do {
                flags = block->flagsC;
                if ((flags & 0x08013880) == 0) {
                    vertex = block->firstVertex8;
                    vertexOffset = vertex * 4;
                    if ((vertex < block->lastVertex18) && (done == 0)) {
                        do {
                            maskWord = *(u32 *) ((u8 *) sector->masks10 + vertexOffset);
                            if (((maskWord & mask) != 0) &&
                                ((maskWord >> 16) != 0)) {
                                triangle = (ShadowTriangle *)
                                    ((u8 *) sector->triangles4 +
                                     (vertex * 0x10));
                                nextTriangle = triangle + 1;
                                lowY = *(s16 *) ((u8 *) sector->vertices0 +
                                                 (triangle->vertex1 * 0xA) + 2);
                                highY = lowY;
                                triangleNumber = 1;
                                do {
                                    triangleNumber++;
                                    currentY = *(s16 *)
                                        ((u8 *) sector->vertices0 +
                                         (nextTriangle->vertex1 * 0xA) + 2);
                                    if (currentY < lowY) {
                                        lowY = currentY;
                                    } else if (highY < currentY) {
                                        highY = currentY;
                                    }
                                    nextTriangle++;
                                } while (triangleNumber != 3);
                                if ((highY >= yMin) && (yMax >= lowY) &&
                                    (mathXZInTri((s32) query->x0C,
                                                 (s32) query->z14,
                                                 (u8 *) sector->vertices0 +
                                                     (triangle->vertex1 * 0xA),
                                                 (u8 *) sector->vertices0 +
                                                     (triangle->vertex2 * 0xA),
                                                 (u8 *) sector->vertices0 +
                                                     (triangle->vertex3 * 0xA)) != 0)) {
                                    value = query->value50;
                                    oldValue = *value;
                                    done = 1;
                                    targetValue =
                                        (1.0f - D_80079464[(flags >> 24) & 7]) -
                                        oldValue;
                                    *value = oldValue + (targetValue * D_800CB28C);
                                }
                            }
                            vertex++;
                            vertexOffset += 4;
                            block = (ShadowBlock *)
                                ((u8 *) sector->blocksC + blockOffset);
                        } while ((vertex < block->lastVertex18) &&
                                 (done == 0));
                    }
                }
                blockOffset += 0x10;
                block = (ShadowBlock *) ((u8 *) block + 0x10);
                blockNumber++;
            } while ((blockNumber < sector->blockCount24) && (done == 0));
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/shadows/func_800180B4.s")
#endif
