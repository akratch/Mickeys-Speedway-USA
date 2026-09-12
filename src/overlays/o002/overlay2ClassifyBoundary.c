#include "PR/ultratypes.h"

extern s32 gOverlay2BoundaryAxis;
extern f32 gOverlay2BoundaryValue;

s32 overlay2ClassifyBoundary(f32 x1, f32 y1, f32 x2, f32 y2, s32 *side1,
                             s32 *side2) {
    if (gOverlay2BoundaryAxis == 0) {
        *side1 = y1 < gOverlay2BoundaryValue;
        *side2 = y2 < gOverlay2BoundaryValue;
        if (y1 == gOverlay2BoundaryValue) {
            *side1 = *side2;
        } else if (y2 == gOverlay2BoundaryValue) {
            *side2 = *side1;
        }
    } else {
        *side1 = x1 < gOverlay2BoundaryValue;
        *side2 = x2 < gOverlay2BoundaryValue;
        if (x1 == gOverlay2BoundaryValue) {
            *side1 = *side2;
        } else if (x2 == gOverlay2BoundaryValue) {
            *side2 = *side1;
        }
    }
    return 1;
}
