#include "PR/ultratypes.h"

typedef struct Overlay14Asset {
    s16 count;
    s16 pad2;
    void *pointer;
    s32 pad8;
    s16 marker;
    s16 padE;
} Overlay14Asset;

typedef struct Overlay14State {
    u8 pad00[0xF4];
    s32 *ranges;
} Overlay14State;

extern Overlay14State gOverlay14State;
extern s32 D_A4;
extern s32 D_B4;
extern void *func_overlay_014_F0000000_186F8D8();

#ifdef NON_MATCHING
/* Workbench 2026-08-29: allocation-mismatch with an exact 54-instruction,
 * 0xD8-byte body and 0x28 frame. The first allocator divergence is pool slot
 * 11 at +0x74; ten register-only words remain. All six target relocations are
 * exact, while the C object also carries gOverlay14State HI16/LO16 records at
 * +0x0/+0x4 that the target does not. A bounded one-thread permuter pass ran
 * for 361 seconds and stayed at the ten-word floor with no zero candidate.
 * Assembly fallback stays canonical. */
Overlay14Asset *func_overlay_014_F00009F4_18702CC(s32 index, s32 context) {
    s32 pad;
    s32 start;
    s32 size;
    Overlay14Asset *asset;
    s32 i;
    Overlay14Asset *entry;
    void *pointer;

    pad = index;
    start = gOverlay14State.ranges[index];
    size = gOverlay14State.ranges[index + 1] - start;
    asset = (Overlay14Asset *)func_overlay_014_F0000000_186F8D8(size, 0x85, start);
    if (asset != 0) {
        func_overlay_014_F0000000_186F8D8(context, asset, start, size);
        for (i = 0, entry = asset; i < asset->count; i++, entry++) {
            pointer = entry->pointer;
            if (pointer == 0) {
                if (entry->marker == 0x4000) {
                    entry->pointer = &D_A4;
                } else {
                    entry->pointer = &D_B4;
                }
            } else {
                entry->pointer = (u8 *)asset + (s32)pointer;
            }
        }
    }
    return asset;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o014/func_overlay_014_F00009F4_18702CC/func_overlay_014_F00009F4_18702CC.s")
#endif
