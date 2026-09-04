#include "PR/ultratypes.h"

extern s32 gO96EntryCountReloc;
extern s32 gO96EntriesReloc[16];

/* Plateau (near-miss batch 13 plus retained-base reshape): -O2 -g3 -mips2
 * is size-exact; 25/34 words differ, first +0x0. A direct-global form gives
 * the target's six relocations and reduces opcode differences from 15 to 11,
 * but regresses positional differences to 29/34. Register qualifiers are
 * byte-neutral; pointer post-decrement and volatile forms grow the function. */
#ifdef NON_MATCHING
void overlay96Unregister(s32 value) {
    s32 count;
    s32 index;
    s32 *entry;
    s32 *countPointer;

    countPointer = &gO96EntryCountReloc;
    count = *countPointer;
    index = count;
    if (index != 0) {
        index--;
        entry = &gO96EntriesReloc[index];
        do {
            if (value == *entry) {
                *countPointer = count - 1;
                if (index < count - 1) {
                    do {
                        *entry = entry[1];
                        entry++;
                    } while (entry < &gO96EntriesReloc[count - 1]);
                }
                return;
            }
            entry--;
        } while (index--);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o096/overlay96Unregister/func_overlay_096_F0000070_18D76A8.s")
#endif

/* PLATEAU-HANDOFF:overlay96Unregister:start
 * symbol: overlay96Unregister
 * score: 9/34 words
 * frame: frameless
 * relocations: 8
 * first-mismatch: +0x0
 * summary: Retained-base reshape exhausted: direct globals prove the six-reloc shape but regress to 5/34 positional words; remaining blocker is the coupled count/index/input register web.
 * PLATEAU-HANDOFF:overlay96Unregister:end
 */
