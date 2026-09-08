#include "PR/ultratypes.h"

extern volatile s32 gOverlay33BufferIndex;
extern u8 D_8[][0xC00];
extern void *gOverlay33DisplayLists[];
extern void *gOverlay33BufferRefs[];
extern void *gOverlay33ActiveBuffer;
extern s32 gOverlay33Ready;

extern void overlay33BuildDisplayList(void);
extern s32 rcpFast3d(u64 *dataStart, u64 *dataEnd, s32 taskType,
                     void *framebuffer);

/* Plateau: the exact-size, opcode-identical 39-word body retains 21 masked
 * differences, first +0x8.  A fidelity-clean trace authenticates seven
 * candidate temp births, but target-side temp events remain unavailable.
 * Naming the s32 rcpFast3d call proves all 18 relocation identities without
 * changing the residual allocation shape. */
#ifdef NON_MATCHING
void overlay33PresentAndSwap(void) {
    s32 index;

    overlay33BuildDisplayList();
    index = gOverlay33BufferIndex;
    rcpFast3d((u64 *)&D_8[index][0],
              (u64 *)gOverlay33DisplayLists[index], 4,
              gOverlay33BufferRefs[index]);
    index = gOverlay33BufferIndex ^ 1;
    gOverlay33BufferIndex = index;
    gOverlay33ActiveBuffer = gOverlay33BufferRefs[index];
    gOverlay33Ready = 1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o033/overlay33PresentAndSwap/func_overlay_033_F000066C_1880E54.s")
#endif

/* PLATEAU-HANDOFF:overlay33PresentAndSwap:start
 * symbol: overlay33PresentAndSwap
 * score: 18/39 words
 * frame: 0x18
 * relocations: 18
 * first-mismatch: +0x8
 * summary: Candidate trace proves seven temp births. rcpFast3d fixes all 18 relocation identities; 21 masked differences remain without target temp events.
 * PLATEAU-HANDOFF:overlay33PresentAndSwap:end
 */
