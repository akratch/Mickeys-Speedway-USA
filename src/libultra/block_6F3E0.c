#include "PR/ultratypes.h"

/* verdict: structure-mismatch; target range is a 0x40-byte zero hole */
/* first mismatch: +0x0; an empty C function still emits a return sequence */
/* blocker: project docs classify 0x6F3E0-0x6F420 as unclaimed padding */
#ifdef NON_MATCHING
void func_8006E7E0(void) {
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/libultra/block_6F3E0/func_8006E7E0.s")
#endif

/* PLATEAU-HANDOFF:func_8006E7E0:start
 * symbol: func_8006E7E0
 * score: 2 differing words
 * frame: frameless
 * relocations: 0
 * first-mismatch: +0x0
 * summary: Target labels one zero word and leaves 0x3c bytes as padding; empty C emits jr/nop, confirming this is a hole rather than a compiler-generated function.
 * PLATEAU-HANDOFF:func_8006E7E0:end
 */
