#ifndef PERMUTER
#include "PR/ultratypes.h"
#else
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef int s32;
typedef float f32;
#endif

typedef struct Overlay97ModelBounds {
    s16 bounds[6];
} Overlay97ModelBounds;

typedef struct Overlay97ModelValue {
    s16 value;
} Overlay97ModelValue;

typedef struct Overlay97ModelHeader {
    u8 bytes[0x3C];
    Overlay97ModelBounds bounds;
} Overlay97ModelHeader;

typedef struct Overlay97ModelInstance {
    Overlay97ModelHeader *model;
} Overlay97ModelInstance;

typedef struct Overlay97Header {
    f32 scale;
    u8 pad04[0x1E];
    s8 modelCount;
    u8 pad23[0x31];
    f32 shadowScale;
    f32 shadowOffset;
} Overlay97Header;

typedef struct Overlay97Shadow {
    f32 scale;
    f32 offset;
} Overlay97Shadow;

typedef struct Overlay97Interaction {
    u8 pad00[0x132];
    s16 enabled;
} Overlay97Interaction;

typedef struct Overlay97ScaleObject {
    s16 angle;
    u8 pad02[6];
    f32 scale;
    u8 pad0C[0x28];
    f32 radius;
    u8 pad38[2];
    s8 modelIndex;
    u8 pad3B[5];
    Overlay97Header *header;
    u8 pad44[8];
    Overlay97Shadow *shadow;
    u8 pad50[8];
    Overlay97Interaction *interaction;
    u8 pad5C[0xC];
    Overlay97ModelInstance **models;
} Overlay97ScaleObject;

typedef struct Overlay97ScaleEntry {
    u8 pad00[0xA];
    u8 modelIndex;
    u8 radius;
    u8 angle;
} Overlay97ScaleEntry;

/*
 * PROVENANCE: Diddy Kong Racing, src/object_functions.c
 * (obj_init_scenery), supplies the radius clamp, scale, model-index, and
 * angle semantics. Neither DKR v77/v80 nor JFG contains an exact donor for
 * Mickey's model-bound scan tail. The local header-sized pointer step is
 * intentional: it preserves the original traversal base through the scan.
 *
 * Promotion reproof (2026-08-29): retained configured NON_MATCHING C is
 * 143/144 against an independently assembled target, one word away at +0xD0.
 * The artifact predates current HEAD, but the source body and physical overlay
 * boundaries are unchanged; current-HEAD reproducibility is still unproven.
 * No linked-C trial artifact survives; ordinary linked equality proves the
 * fallback only. Retail derives the
 * values cursor as a3+2, while C emits the equivalent a1+0x3E address. Both
 * forms are frameless, 0x240 bytes / 144 instructions, and have no relocations.
 * The earlier object-exact note compared the fallback assembly to itself and
 * was invalid. The checked-in three-word ranking also omitted
 * -Wab,-r4300_mul; that flag fixes its +0x24/+0x28 scheduling differences and
 * leaves only +0xD0. Historical volatile, typed-base, declaration, flag, and
 * permutation outcomes have no surviving attributable variant artifacts and
 * do not prove exhaustion. Re-run configured V0, then assign bounds first and
 * independently derive values from that live carrier as (u8 *)bounds + 2 and
 * &bounds->bounds[1]. Only if V0 drifts and a probe strictly improves it may a
 * fourth build tighten scan-tail scope. The assembly fallback remains
 * canonical.
 */
#ifdef NON_MATCHING
void overlay97InitScale(Overlay97ScaleObject *object, void *entryArg) {
    f32 radius;
    f32 maximum;
    f32 absolute;
    f32 value;
    register volatile Overlay97ModelBounds *bounds;
    register Overlay97ModelValue *values;
    s32 i;
    s32 modelIndex;
    Overlay97ModelInstance *instance;
    Overlay97Interaction *interaction;
    Overlay97ScaleEntry *entry;
    Overlay97ModelHeader *model;

    entry = entryArg;

    radius = entry->radius & 0xFF;
    if (radius < 10.0f) {
        radius = 10.0f;
    }
    radius /= 64.0f;
    object->scale = object->header->scale * radius;
    if (object->shadow != 0) {
        object->shadow->scale = object->header->shadowScale * radius;
        object->shadow->offset = object->header->shadowOffset * radius;
    }
    absolute = radius;
    value = absolute;

    object->modelIndex = entry->modelIndex;
    object->angle = entry->angle << 8;
    interaction = object->interaction;
    interaction->enabled = 1;
    modelIndex = object->modelIndex;
    if (modelIndex >= object->header->modelCount) {
        object->modelIndex = 0;
        modelIndex = object->modelIndex;
    }

    instance = object->models[modelIndex];
    model = instance->model;
    values = (Overlay97ModelValue *)((u8 *)instance->model + 0x3E);
    bounds = (volatile Overlay97ModelBounds *)((u8 *)model + 0x3C);
    {
        absolute = 0.0f;
        maximum = bounds->bounds[0];
        value = maximum;
        if (maximum < 0.0f) {
            value = -maximum;
        }
        if (absolute < value) {
            absolute = value;
        }
    }
    {
        maximum = values->value;
        value = maximum;
        if (maximum < 0.0f) {
            value = -maximum;
        }
        if (absolute < value) {
            absolute = value;
        }
        values++;
    }
    {
        maximum = values->value;
        value = maximum;
        if (maximum < 0.0f) {
            value = -maximum;
        }
        if (absolute < value) {
            absolute = value;
        }
        values++;
    }
    {
        maximum = values->value;
        value = maximum;
        if (maximum < 0.0f) {
            value = -maximum;
        }
        if (absolute < value) {
            absolute = value;
        }
        values++;
    }
    {
        maximum = values->value;
        value = maximum;
        if (maximum < 0.0f) {
            value = -maximum;
        }
        if (absolute < value) {
            absolute = value;
        }
        values++;
    }
    {
        maximum = values->value;
        value = maximum;
        if (maximum < 0.0f) {
            value = -maximum;
        }
        if (absolute < value) {
            absolute = value;
        }
        values++;
    }
    object->radius = object->scale * absolute;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o097/overlay97InitScale/func_overlay_097_F0000508_18D83A0.s")
#endif
