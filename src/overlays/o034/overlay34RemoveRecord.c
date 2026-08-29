#include "PR/ultratypes.h"

typedef struct Overlay34Resource Overlay34Resource;

typedef struct Overlay34Record {
    u8 pad00[0x20];
    Overlay34Resource *resource;
    u8 pad24[0x1A];
    u8 active;
    u8 pad3F[0x29];
} Overlay34Record;

extern Overlay34Record **gOverlay34Pointers;
extern s32 gOverlay34ActiveCount;
extern void overlay34FreeReloc(Overlay34Resource *resource);

/* The prior allocation-aided body measured 32/44 raw words, first +0x14, but
 * its shadow argument, three-step pointer increment, integer resource field,
 * and initializer-named helper were not source-faithful. Current clean V0 is
 * uncompiled. The owned +0x2C8..+0x378 range has no padding. Runtime metadata
 * authenticates seven roles: three active-count pairs, one pointer pair, and
 * the +0x6C SYMBOL call through this zero-field proxy to resident ORT 140
 * (func_800347A0). The fallback object retains only five static records, and
 * linked equality proves assembly only. Retain 119 flags, trace once, and try
 * at most one trace-supported natural lifetime form; cap 120 builds plus trace.
 */
#ifdef NON_MATCHING
void overlay34RemoveRecord(Overlay34Record *record) {
    s32 remaining;
    Overlay34Record **slot;

    slot = gOverlay34Pointers;
    remaining = gOverlay34ActiveCount;
    if (remaining != 0) {
        remaining--;
        do {
            if (*slot == record) {
                if (remaining != 0) {
                    remaining--;
                    do {
                        *slot = slot[1];
                        slot++;
                    } while (remaining--);
                }
                if (record->resource != NULL) {
                    overlay34FreeReloc(record->resource);
                }
                record->active = 0;
                gOverlay34ActiveCount--;
                return;
            }
            slot++;
        } while (remaining--);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o034/overlay34RemoveRecord/func_overlay_034_F00002C8_1881470.s")
#endif
