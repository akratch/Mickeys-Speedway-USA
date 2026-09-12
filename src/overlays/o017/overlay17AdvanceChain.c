#include "PR/ultratypes.h"

typedef struct Overlay17StripPoint {
    s16 x0, y0, z0;
    u8 r0, g0, b0, a0;
    s16 x1, y1, z1;
    u8 r1, g1, b1, a1;
} Overlay17StripPoint;

typedef struct Overlay17Chain {
    s16 count;
    u8 selectedBuffer;
    u8 pad03[0x21];
    u8 red, green, blue, alpha;
    u8 pad28[4];
    Overlay17StripPoint *buffers[2];
} Overlay17Chain;

extern void func_overlay_017_F0000000_18739B8(Overlay17Chain *chain,
                                               f32 *x0, f32 *y0, f32 *z0,
                                               f32 *x1, f32 *y1, f32 *z1);

/* NON_MATCHING: exact size and frame; the remaining deficit is in the
 * pre-call copy setup. See the symbol-owned handoff for measured closures. */
#ifdef NON_MATCHING
void overlay17AdvanceChain(Overlay17Chain *chain, s32 useAlpha) {
    s32 count;
    s32 savedAlpha;
    f32 x0, y0, z0, x1, y1, z1;
    u16 *sourceCursor;
    u16 *destinationCursor;
    Overlay17StripPoint *writeCursor;
    u8 oldBuffer;
    u8 newBuffer;

    if (chain == 0) {
        return;
    }

    count = chain->count;
    oldBuffer = chain->selectedBuffer;
    sourceCursor = (u16 *)((u8 *)chain->buffers[oldBuffer] +
                           (((count - 1) << 1) * 10));
    newBuffer = oldBuffer ^ 1;
    chain->selectedBuffer = newBuffer;
    writeCursor = chain->buffers[newBuffer];
    destinationCursor = (u16 *)((u8 *)writeCursor +
                                ((count << 1) * 10));
    count--;
    count = (count << 2) + count;
    count <<= 1;
    if (count--) {
        do {
            u16 value = sourceCursor[-1];
            destinationCursor--;
            sourceCursor--;
            *destinationCursor = value;
        } while (count--);
        writeCursor = chain->buffers[chain->selectedBuffer];
    }
    if (useAlpha != 0) {
        savedAlpha = chain->alpha;
    } else {
        savedAlpha = 0;
    }
    func_overlay_017_F0000000_18739B8(chain, &x0, &y0, &z0,
                                      &x1, &y1, &z1);

    writeCursor->x0 = (s16)(s32)x0;
    writeCursor->y0 = (s16)(s32)y0;
    writeCursor->z0 = (s16)(s32)z0;
    writeCursor->r0 = chain->red;
    writeCursor->g0 = chain->green;
    writeCursor->b0 = chain->blue;
    writeCursor->a0 = (u8)savedAlpha;
    writeCursor->x1 = (s16)(s32)x1;
    writeCursor->y1 = (s16)(s32)y1;
    writeCursor->z1 = (s16)(s32)z1;
    writeCursor->r1 = chain->red;
    writeCursor->g1 = chain->green;
    writeCursor->b1 = chain->blue;
    writeCursor->a1 = (u8)savedAlpha;
    writeCursor++;

    count = chain->count - 1;
    while (count--) {
        if (writeCursor->a0 != 0) {
            savedAlpha = (chain->alpha * count) / (chain->count - 1);
            writeCursor->a0 = (u8)savedAlpha;
            writeCursor->a1 = (u8)savedAlpha;
        }
        writeCursor++;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o017/overlay17AdvanceChain/func_overlay_017_F0000668_1874020.s")
#endif

/* PLATEAU-HANDOFF:overlay17AdvanceChain:start
 * symbol: overlay17AdvanceChain
 * score: 49 differing words
 * frame: 0x70
 * relocations: 1
 * first-mismatch: +0x18
 * summary: 49-word pre-call setup residual; exact size/home set; count reassociation and buffer-carrier alternatives stalled.
 * PLATEAU-HANDOFF:overlay17AdvanceChain:end
 */
