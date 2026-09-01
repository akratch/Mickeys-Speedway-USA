#include "PR/ultratypes.h"
#include "PR/os_pi.h"
#include "PR/rcp.h"

extern OSPiHandle D_800D7760;

/* PROVENANCE: body adapted from Jet Force Gemini's public decompilation,
 * libultra/src/flash/flashreinit.c. Mickey's handler symbol and output remain
 * authoritative. */
OSPiHandle *osFlashReInit(u8 latency, u8 pulse, u8 pageSize, u8 relDuration,
                          u32 start) {
    D_800D7760.baseAddress = PHYS_TO_K1(start);
    D_800D7760.type++;
    D_800D7760.latency = latency;
    D_800D7760.pulse = pulse;
    D_800D7760.pageSize = pageSize;
    D_800D7760.relDuration = relDuration;
    D_800D7760.domain = PI_DOMAIN2;

    return &D_800D7760;
}

#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_58570/func_800579C0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_58570/func_80057AB4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_58570/func_80057B44.s")
