#include "PR/ultratypes.h"
#include "PR/os_pi.h"

extern OSPiHandle D_800D7760;

#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_5885C/func_80057C5C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_5885C/func_80057D28.s")
/* PROVENANCE: body adapted from Jet Force Gemini's public decompilation,
 * libultra/src/flash/flashsectorerase.c:osFlashWriteBuffer. Mickey's symbol,
 * linked bytes, and relocations remain authoritative. */
s32 osFlashWriteBuffer(OSIoMesg *mb, s32 priority, void *dramAddr,
                       OSMesgQueue *mq) {
    osEPiWriteIo(&D_800D7760, D_800D7760.baseAddress | 0x10000, 0xB4000000);
    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = 0;
    mb->size = 0x80;
    return osEPiStartDma(&D_800D7760, mb, OS_WRITE);
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_5885C/func_80057E84.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/flash_5885C/func_80057F54.s")
