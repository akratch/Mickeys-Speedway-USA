#include "PR/ultratypes.h"
#include "overlays/overlay058.h"

typedef struct Overlay58LargePointVertex {
    s16 x;
    s16 y;
    s16 z;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} Overlay58LargePointVertex;

typedef struct Overlay58LargePointGfx {
    u32 w0;
    u32 w1;
} Overlay58LargePointGfx;

typedef struct Overlay58LargePointRenderState {
    u8 pad0[0x1E8];
    void *resource;
} Overlay58LargePointRenderState;

extern Overlay58LargePointGfx *gOverlay58LargePointDisplayListReloc;
extern Overlay58LargePointVertex *gOverlay58LargePointVertexCursorReloc;
extern Overlay58LargePointRenderState gOverlay58LargePointRenderStateReloc;
extern u8 D_80000098[];
/*
 * The callee is reached through the overlay loader's relocation table, so its
 * `jal` encodes 0 and splat names it after overlay offset 0 for every such
 * call in this overlay; the real callee is not identified.  What the target
 * bytes do decide is its FIRST ARGUMENT: see the note below.
 */
extern void func_overlay_058_F0000000_18AF1E8(
    Overlay58LargePointVertex **cursor, void *resource, s32 mode,
    s32 arg3);

/*
 * Plateau (2026-09-10): 26 of 104 relocation-masked words differ, down from 70,
 * at exact 104-instruction geometry and an exact 0x18 frame.  Two mechanisms
 * closed 44 words and both are properties of the allocator, not of spelling:
 *
 *  1. The call's first argument is
 *     `&gOverlay58LargePointVertexCursorReloc`, the same symbol the body
 *     re-reads twenty times afterwards -- NOT the display list.  p1 splits
 *     that one address web around the call and the post-call piece keeps the
 *     a0 argument affinity at cost 0 (ido-5.3 L57/L66), which is the only
 *     construct that puts the long-lived cursor address in a0 instead of v0.  Worth 16 words by itself; all twenty `0(a0)` sites go
 *     register-exact.  1352 spellings measured against the old argument
 *     spelling never reached below 68.
 *  2. Four discarded-expression probes (ido-5.3 L37) at the positions below.
 *     They cost zero instructions and re-order p1's colouring by adding web
 *     occurrences; worth a further 26 words.  Their positions were found by a
 *     frame-constrained hill climb, 7800 variants, and are a local optimum
 *     under single insert/delete/move at 4968 further variants.
 *
 * Falsified here: declaration order is wholly inert (400 permutations, none
 * below the then-best); statement-block order, the pointer/array and cast
 * forms, `physicalBase` inlining, the second-gfx word order, and every shape
 * that drops the early `vertices` assignment all regress.
 *
 * Residual: `gfx` wants v0 and takes a2; the physical-address chain wants ring
 * temps and takes colours; `xPlus` wants a1 and takes t0; the two `lui`s at
 * +0x88 are transposed; the two `lui`s at +0x14 are transposed.
 */
#ifdef NON_MATCHING
void overlay58DrawLargePointQuad(s32 x, s32 y, s32 z) {
    Overlay58LargePointGfx *gfx;
    Overlay58LargePointVertex *vertices;
    u32 physicalVertices;
    s32 physicalBase;
    s32 xPlus;
    s32 xMinus;
    s32 zMinus;
    s32 zPlus;
    if (x != 0);

    func_overlay_058_F0000000_18AF1E8(
        &gOverlay58LargePointVertexCursorReloc,
        gOverlay58LargePointRenderStateReloc.resource, 5, 0);
    gfx = gOverlay58LargePointDisplayListReloc++;
    vertices = gOverlay58LargePointVertexCursorReloc;
    physicalBase = 0x80000000U;
    physicalVertices = (u32)vertices + physicalBase;
    gfx->w0 = 0x04000000U |
              ((((u8)((physicalVertices & 6U) | 0x20U)) & 0xFFU) << 16) |
              0x30U;
    gfx->w1 = (u32)gOverlay58LargePointVertexCursorReloc + physicalBase;
    if (vertices != 0);

    gfx = gOverlay58LargePointDisplayListReloc++;
    /* One physical line, deliberately: as1's scheduling tie-break reads source
     * line numbers (ido-5.3 L59), and folding these two stores onto one line
     * retires the tie that transposes the two `lui`s at +0x88.  26 -> 24
     * differing words, measured. */
    gfx->w0 = 0x05110020U; gfx->w1 = (u32)D_80000098;

    vertices = gOverlay58LargePointVertexCursorReloc;
    vertices[1].r = 0xFF;
    vertices[1].g = 0xFF;
    vertices[1].b = 0xFF;
    vertices[1].a = 0xFF;
    vertices[2].r = 0xFF;
    vertices[2].g = 0xFF;
    vertices[2].b = 0xFF;
    vertices[2].a = 0xFF;
    vertices[3].r = 0xFF;
    vertices[3].g = 0xFF;
    vertices[3].b = 0xFF;
    vertices[3].a = 0xFF;
    vertices += 3;
    vertices[-3].r = 0xFF;
    vertices[-3].g = 0xFF;
    vertices[-3].b = 0xFF;
    vertices[-3].a = 0xFF;

    xPlus = x + 18;
    zPlus = z + 18;
    if (gfx != 0);
    if (vertices != 0);
    xMinus = x - 18;
    zMinus = z - 18;

    gOverlay58LargePointVertexCursorReloc->x = (s16)xMinus;
    gOverlay58LargePointVertexCursorReloc->y = (s16)y;
    gOverlay58LargePointVertexCursorReloc->z = (s16)zMinus;
    gOverlay58LargePointVertexCursorReloc++;

    gOverlay58LargePointVertexCursorReloc->x = (s16)xPlus;
    gOverlay58LargePointVertexCursorReloc->y = (s16)y;
    gOverlay58LargePointVertexCursorReloc->z = (s16)zMinus;
    gOverlay58LargePointVertexCursorReloc++;

    gOverlay58LargePointVertexCursorReloc->x = (s16)xMinus;
    gOverlay58LargePointVertexCursorReloc->y = (s16)y;
    gOverlay58LargePointVertexCursorReloc->z = (s16)zPlus;
    gOverlay58LargePointVertexCursorReloc++;

    gOverlay58LargePointVertexCursorReloc->x = (s16)xPlus;
    gOverlay58LargePointVertexCursorReloc->y = (s16)y;
    gOverlay58LargePointVertexCursorReloc->z = (s16)zPlus;
    gOverlay58LargePointVertexCursorReloc++;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o058/overlay58DrawLargePointQuad/func_overlay_058_F00050C8_18B42B0.s")
#endif

/* PLATEAU-HANDOFF:overlay58DrawLargePointQuad:start
 * symbol: overlay58DrawLargePointQuad
 * score: 24/104 words
 * frame: 0x18
 * relocations: 11
 * first-mismatch: +0x14
 * summary: Exact Group B residual transfer confirms the same p1 phase-and-colour blocker; no source-authentic web evidence yet changes it.
 * PLATEAU-HANDOFF:overlay58DrawLargePointQuad:end
 */
