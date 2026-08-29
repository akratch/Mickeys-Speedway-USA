#include "overlays/overlay_099.h"

typedef struct Overlay99Segment {
    f32 x0;
    f32 z0;
    f32 x1;
    f32 z1;
    u8 pad10[0x1A];
    s8 headingOffset;
    u8 pad2B;
    Overlay99RenderState *object;
} Overlay99Segment;

extern s32 gOverlay99SegmentCount;
extern Overlay99Segment gOverlay99Segments[];
extern void *gOverlay99Texture;
extern int overlay99Begin(void **outA, s32 *outB);
extern void overlay99Setup(Gfx **displayList, void *a, s32 b,
                           void *zero0, s32 zero1, void *a2, s32 b2);
extern void overlay99End(Gfx **displayList);
extern s16 overlay99Angle(f32 x, f32 z);
extern f32 overlay99Sqrt(f32 squared);
extern void overlay99DrawObject(Gfx **displayList, Mtx **matrices,
                                void *vertices, Overlay99RenderState *object);

/* Bounded workbench closeout (2026-08-28): the configured candidate is
 * exact-sized at 142 words with the exact 0xa8 frame and all 15 runtime
 * relocation roles. Stock-gated CFE/Ucode and ugen/as1 captures traced the
 * addressed setup result; three natural layout/scope variants either moved
 * its homes or grew the frame. A five-minute, one-thread permuter found the
 * non-void begin ABI, which the real full-TU build confirmed by selecting the
 * retail display-list register. The retained candidate is 140/142 words after
 * masking relocation addends; its only residual is setup.b at sp+0xa0 rather
 * than retail's sp+0x98, first visible at +0x7c. */
#ifdef NON_MATCHING
void overlay99RenderSegments(Gfx **displayList, Mtx **matrices, void *vertices,
                             f32 scale) {
    struct {
        void *a;
        s32 b;
        u32 reserved;
    } setup;
    f32 dx;
    f32 dz;
    f32 length;
    Overlay99Segment *segment;
    Overlay99RenderState *object;
    Gfx *command;
    s32 initialized;
    s32 i;

    i = 0;
    segment = gOverlay99Segments;
    initialized = 0;
    if (gOverlay99SegmentCount > 0) {
        do {
            object = segment->object;
            if (object != 0) {
                if (initialized == 0) {
                    overlay99Begin(&setup.a, &setup.b);
                    initialized = 1;
                    command = *displayList;
                    *displayList = command + 1;
                    command->words.w0 = 0xBC000806;
                    command->words.w1 = (u32)gOverlay99Texture + 0x80000000;
                    overlay99Setup(displayList, setup.a, setup.b, 0, 0,
                                   setup.a, setup.b);
                    overlay99End(displayList);
                }

                dx = segment->x1 - segment->x0;
                dz = segment->z1 - segment->z0;
                object->rotation0 = 0;
                object->rotation1 = 0;
                object->heading = overlay99Angle(-dx, dz);
                dx *= scale;
                dz *= scale;
                object->x = segment->x0 + dx;
                object->z = segment->z0 + dz;
                object->y = 5.0f;
                length = overlay99Sqrt((dx * dx) + (dz * dz));
                if (segment->headingOffset != 0) {
                    object->heading += (s32)((f32)segment->headingOffset *
                                             65536.0f * scale);
                }
                object->flags &= ~0x400;
                overlay99DrawObject(displayList, matrices, vertices, object);
                object->flags |= 0x400;
                overlay99RenderSortedEntries(displayList, matrices, vertices,
                                             object, length);
            }
            i++;
            segment++;
        } while (i < gOverlay99SegmentCount);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o099/overlay99RenderSegments/func_overlay_099_F0000BA4_18DA154.s")
#endif
