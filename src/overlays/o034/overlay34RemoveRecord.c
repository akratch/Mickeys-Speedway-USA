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
extern void overlay34FreeReloc(Overlay34Resource *resource, s32 shadow);

/* Size-closed diagnostic plateau: the shadow helper argument and inert cursor
 * composition produce 44/44 words, frame 0x18, and 12 residual words (the
 * configured V0 was 42/44). The remaining gap is register-carrier allocation;
 * retain NON_MATCHING and the assembly fallback until relocation identities
 * and the a0/a1 count web are exact. */
#ifdef NON_MATCHING
void overlay34RemoveRecord(Overlay34Record *record) {
    Overlay34Record **slot;
    s32 remaining;
    s32 shadow;

    slot = gOverlay34Pointers;
    remaining = gOverlay34ActiveCount;
    shadow = remaining;
    if (remaining != 0) {
        remaining--;
        do {
            if (*slot == record) {
                if (remaining != 0) {
                    remaining--;
                    do {
                        *slot = slot[1];
                        shadow = remaining;
                        slot++;
                    } while (remaining--);
                }
                if (record->resource != NULL) {
                    overlay34FreeReloc(record->resource, shadow);
                }
                record->active = 0;
                gOverlay34ActiveCount--;
                return;
            }
            slot++;
            slot++;
            slot--;
        } while (remaining--);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o034/overlay34RemoveRecord/func_overlay_034_F00002C8_1881470.s")
#endif

/* PLATEAU-HANDOFF:overlay34RemoveRecord:start
 * symbol: overlay34RemoveRecord
 * score: 12/44 words
 * frame: 0x18
 * relocations: 9
 * first-mismatch: +0x14
 * summary: size-closed shadow/cursor form; residual is register-carrier allocation and relocation identity drift
 * PLATEAU-HANDOFF:overlay34RemoveRecord:end
 */
