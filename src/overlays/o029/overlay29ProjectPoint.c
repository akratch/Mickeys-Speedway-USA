#include "PR/ultratypes.h"

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct Overlay29PathState {
    u8 pad00[0x14];
    u32 flags;
    u8 pad18[4];
    Vec3f direction;
} Overlay29PathState;

typedef struct Overlay29Transform {
    Vec3f direction;
    u8 pad0C[4];
    Vec3f position;
    f32 height;
    u32 flags;
} Overlay29Transform;

typedef struct Overlay29Owner {
    u8 pad00[0x64];
    Overlay29PathState *state;
} Overlay29Owner;

extern u8 gOverlay29MinimumYReloc[];
extern f32 overlay29SqrtReloc(f32 value);

/* Reusing lengthSquared for the square root and final delta, as in the exact
 * overlay 26 sibling, removes two declarations; placing it before the cross
 * component declarations then recovers the target 0x68 frame and every stack
 * home. The configured candidate is 118/121 words with all integer, FP-pool,
 * and FP-temp lanes exact. Plain operand swaps and explicit dereference forms
 * are byte-flat; compound/two-step forms regress, while the negated-difference
 * form reaches the three target mul.s orders but rotates the axis-load FP web.
 * All three relocation identities are statically exact. */
#ifdef NON_MATCHING
void func_overlay_029_F0000EE0_187E190(
    s32 unused, Vec3f *output, Vec3f *axis, f32 height,
    Overlay29Transform *transform, Overlay29Owner *owner) {
    Overlay29PathState *state;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 projectedX;
    f32 projectedY;
    f32 projectedZ;
    f32 lengthSquared;
    f32 crossX;
    f32 crossY;
    f32 crossZ;

    dirY = transform->direction.y;
    dirX = transform->direction.x;
    dirZ = transform->direction.z;
    state = owner->state;
    if ((*(f32 *)(gOverlay29MinimumYReloc + 0x14) <= dirY) ||
        ((transform->flags & 0x10000000) != 0)) {
        crossX = (dirY * axis->z) - (axis->y * dirZ);
        crossY = (dirZ * axis->x) - (axis->z * dirX);
        crossZ = (dirX * axis->y) - (axis->x * dirY);
        projectedX = (crossY * dirZ) - (crossZ * dirY);
        projectedY = (crossZ * dirX) - (crossX * dirZ);
        projectedZ = (crossX * dirY) - (crossY * dirX);
        lengthSquared = (projectedX * projectedX) +
                        (projectedY * projectedY) +
                        (projectedZ * projectedZ);
        if (lengthSquared > 0.0f) {
            lengthSquared = overlay29SqrtReloc(lengthSquared);
            projectedY /= lengthSquared;
            projectedX /= lengthSquared;
            projectedZ /= lengthSquared;
            lengthSquared = height - transform->height;
            output->x = transform->position.x + (lengthSquared * projectedX);
            output->y = transform->position.y + (lengthSquared * projectedY);
            output->z = transform->position.z + (lengthSquared * projectedZ);
        } else {
            output->x = transform->position.x;
            output->y = transform->position.y;
            output->z = transform->position.z;
        }
        state->flags |= 2;
    } else {
        output->x = transform->position.x;
        output->y = transform->position.y;
        output->z = transform->position.z;
        state->direction.x = dirX;
        state->direction.y = dirY;
        state->direction.z = dirZ;
        state->flags |= 4;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o029/overlay29ProjectPoint/func_overlay_029_F0000EE0_187E190.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_029_F0000EE0_187E190:start
 * symbol: func_overlay_029_F0000EE0_187E190
 * score: 118/121 words
 * frame: 0x68
 * relocations: 3
 * first-mismatch: +0x60
 * summary: Dead-carrier reuse and declaration order cut 29 to 3 words with exact frame and lanes. Three cross-product mul.s operand orders remain after ten forms.
 * PLATEAU-HANDOFF:func_overlay_029_F0000EE0_187E190:end
 */
