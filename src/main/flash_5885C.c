#include "PR/ultratypes.h"
#include "PR/os_pi.h"

extern OSPiHandle D_800D7760;
extern s32 D_800D77D8;

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
/* PROVENANCE: body adapted from Jet Force Gemini's public decompilation,
 * libultra/src/flash/flashreadarray.c:osFlashReadArray. Mickey's symbol,
 * linked bytes, and relocations remain authoritative. */
s32 osFlashReadArray(OSIoMesg *mb, s32 priority, u32 page_num, void *dramAddr,
                     u32 n_pages, OSMesgQueue *mq) {
    u32 ret;
    u32 tmp;

    osEPiWriteIo(&D_800D7760, D_800D7760.baseAddress | 0x10000, 0xF0000000);
    osEPiReadIo(&D_800D7760, D_800D7760.baseAddress, &tmp);
    mb->hdr.pri = priority;
    mb->hdr.retQueue = mq;
    mb->dramAddr = dramAddr;
    mb->devAddr = (page_num ^ 0) * D_800D77D8;
    mb->size = n_pages << 7;
    ret = osEPiStartDma(&D_800D7760, mb, OS_READ);
    return ret;
}
