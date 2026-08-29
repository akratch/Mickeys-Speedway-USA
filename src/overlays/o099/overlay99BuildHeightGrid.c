#include "PR/ultratypes.h"

typedef struct Overlay99GridPoint {
    s16 reserved00;
    s16 reserved02;
    s16 height;
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
} Overlay99GridPoint;

typedef struct Overlay99Segment {
    u8 bytes[0x30];
} Overlay99Segment;

extern s32 gOverlay99CurrentGrid;
extern Overlay99GridPoint *gOverlay99Grids[];
extern s32 gOverlay99SegmentCount;
extern Overlay99Segment gOverlay99Segments[];
extern s32 gOverlay99GridWidth;
extern s32 gOverlay99GridHeight;
extern s32 gOverlay99WidthMinusOne;
extern s32 gOverlay99HeightMinusOne;
extern s32 gOverlay99Arg4;
extern s32 gOverlay99Arg5;
extern void overlay99ApplySegment(Overlay99Segment *segment, f32 scale);

/*
 * Retained-evidence diagnostic (reviewed 2026-08-29): target is 114 words with
 * a 0x28 frame. The retained genuine isolated object predates the grid-load
 * reorder below and is 104/114 words after runtime-relocation normalization,
 * but its recipe omitted the TU's explicit -Wo,-loopunroll,0. Its ten
 * register-only sites are at +0x108,+0x10C,+0x110,
 * +0x114,+0x118,+0x120,+0x124,+0x154,+0x15C,+0x170. Its 29 static
 * relocations match all 29 shipped runtime sites and types. The reorder was
 * historically reported to remove the first seven sites, leaving the
 * three-use negative-magnitude web, but no untouched current-source object
 * survives. Both bodies also used an invented empty read of `unused` to alter
 * its ABI lifetime; that guard is removed, so both scores are diagnostic only.
 * Ordinary object/link/module/ROM equality proves GLOBAL_ASM. Compile clean
 * V0, retain the 119-flag lattice, take one allocator trace, then try only its
 * indicated natural magnitude-carrier form. */
#ifdef NON_MATCHING
void overlay99BuildHeightGrid(f32 scale, void *unused, s32 widthMinusOne,
                              s32 heightMinusOne, s32 arg4, s32 arg5) {
    Overlay99GridPoint *point;
    s32 *widthPtr;
    s32 *heightPtr;
    s32 i;
    s32 value;

    point = gOverlay99Grids[gOverlay99CurrentGrid];
    if (point == 0) {
        return;
    }

    widthPtr = &gOverlay99GridWidth;
    heightPtr = &gOverlay99GridHeight;
    *widthPtr = widthMinusOne + 1;
    *heightPtr = heightMinusOne + 1;
    gOverlay99WidthMinusOne = widthMinusOne;
    gOverlay99HeightMinusOne = heightMinusOne;
    gOverlay99Arg4 = arg4;
    gOverlay99Arg5 = arg5;

    i = *heightPtr * *widthPtr;
    while (i--) {
        point->height = 5;
        point++;
    }

    i = gOverlay99SegmentCount;
    while (i--) {
        overlay99ApplySegment(&gOverlay99Segments[i], scale);
    }

    widthPtr = &gOverlay99GridWidth;
    heightPtr = &gOverlay99GridHeight;
    i = *heightPtr * *widthPtr;
    point = gOverlay99Grids[gOverlay99CurrentGrid];
    while (i--) {
        value = point->height - 5;
        if (value < 0) {
            if (value < -40) {
                value = -40;
            }
            value = -value;
            point->red = 0;
            point->green = 0;
            point->blue = 0;
            point->alpha = value;
        } else if (value > 0) {
            if (value > 40) {
                value = 40;
            }
            point->red = 0xFF;
            point->green = 0xFF;
            point->blue = 0xFF;
            point->alpha = value;
        } else {
            point->alpha = 0;
        }
        point++;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o099/overlay99BuildHeightGrid/func_overlay_099_F0000638_18D9BE8.s")
#endif
