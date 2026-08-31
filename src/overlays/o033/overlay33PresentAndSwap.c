#include "PR/ultratypes.h"

extern volatile s32 gOverlay33BufferIndex;
extern u8 D_8[][0xC00];
extern void *gOverlay33DisplayLists[];
extern void *gOverlay33BufferRefs[];
extern void *gOverlay33ActiveBuffer;
extern s32 gOverlay33Ready;

extern void overlay33BuildDisplayList(void);
extern void overlay33SubmitReloc(void *commands, void *displayList, s32 mode,
                                 void *buffer);

/* Plateau: the exact-size, opcode-identical 39-word body retains 21 masked
 * differences, first +0x10, in two independent allocator lanes.  The runtime
 * surface has 18 exact offset/type sites; D_8 and the +0x19C builder are now
 * named from authenticated Overlay 33 identities.  Block-local state,
 * signedness, offset lifetime, register, and split-XOR forms were neutral. */
#ifdef NON_MATCHING
void overlay33PresentAndSwap(void) {
    s32 index;

    overlay33BuildDisplayList();
    index = gOverlay33BufferIndex;
    overlay33SubmitReloc(&D_8[index][0],
                         gOverlay33DisplayLists[index], 4,
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
 * first-mismatch: +0x10
 * summary: Exact opcode/size and 18 relocation sites; builder identity now resolves. Next lever is authenticated temp-FIFO phase evidence, not more source permutation.
 * PLATEAU-HANDOFF:overlay33PresentAndSwap:end
 */
