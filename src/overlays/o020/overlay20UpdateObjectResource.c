#include "PR/ultratypes.h"

typedef struct Overlay20LookupResult {
    s16 x0;
    s16 pad02;
    s16 y0;
    s16 x1;
    s16 pad08;
    s16 y1;
} Overlay20LookupResult;

typedef struct Overlay20Context {
    void **entries;
    u8 pad04[0x14];
    s16 count;
} Overlay20Context;

typedef struct Overlay20Config {
    u8 pad00[0xA];
    s8 useLookup;
    s8 entryIndex;
    u8 columns;
    u8 rows;
    u8 value0E;
    u8 value0F;
    s16 width;
    s16 height;
    u8 start;
    u8 current;
    u8 end;
    u8 scaleDivisor;
} Overlay20Config;

typedef struct Overlay20Object {
    u8 pad00[0xC];
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[0x16];
    s16 lookupIndex;
    u8 pad30[4];
    f32 radius;
    u8 pad38[0x30];
    void **fallbackEntry;
    u8 pad6C[0x18];
    void *resource;
} Overlay20Object;

extern Overlay20Context *overlay20GetContextReloc(void);
extern Overlay20LookupResult *overlay20LookupReloc(s16 index);
extern void *overlay20ConfigureResourceReloc();
extern f32 overlay20SqrtReloc(f32 value);

/* Reproof lead: historical same-body configured C was measured at 90/98 raw/
 * normalized words, frame 0x70, first +0xB0. A stale local configured object
 * reproduces those scalars and four proxy call records but has no source hash or
 * build ledger and predates current line layout. Canonical Git retains only the
 * eight-difference isolated scalar; no candidate object/report survives there.
 * Current score, frame, size, and emitted relocations are unknown. The owned
 * +0x204..+0x38C / ROM 0x18767DC..0x1876964 range has no target padding; IDO's
 * trailing eight-byte section alignment is outside the function. Four
 * count/entries carrier sites remain at
 * +0xB0/+0xB4/+0xC0/+0xC8; +0x108/+0x10C/+0x110/+0x114 are a downstream
 * value0F/start argument-temporary cluster, but causal independence is
 * unproved. Runtime tables authenticate target calls at +0x14/+0x30/+0x134/
 * +0x168 as trackGetTrack, func_8000FEEC, local overlay20ConfigureResource, and
 * sqrtf; the stale candidate's proxy records prove offsets/types, not identities.
 * Historical source/flag/trace/permuter and linked-C
 * trial claims are unretained; linked equality proves fallback only.
 * Exact pinned overlay-wide DKR v77/v80/JFG scans found no candidate; a retained
 * function-specific structural scan is also negative (best 0.075, first JFG
 * 0.052). Compile V0; on regression use one historical line-layout control and
 * invalidate the baseline if it also fails. Otherwise retain 119 configurations
 * including V0, one trace, one natural form per cluster, and combine only
 * independent strict gains. Hard cap 122 deterministic builds plus one trace,
 * 123 only for the control. Allow one 20-minute/2,000-candidate annotated batch
 * only after a legal natural-source gain; no generic batch from V0. */
#ifdef NON_MATCHING
void overlay20UpdateObjectResource(Overlay20Object *object,
                                   Overlay20Config *config) {
    s32 baseX;
    s32 baseY;
    s32 objectY;
    s32 width;
    s32 height;
    void *owner;
    Overlay20LookupResult *lookup;
    Overlay20Context *context;

    context = overlay20GetContextReloc();
    if ((config->useLookup != 0) &&
        ((lookup = overlay20LookupReloc(object->lookupIndex)), lookup != 0)) {
        baseX = lookup->x0;
        objectY = (s32)object->y;
        baseY = lookup->y0;
        width = lookup->x1 - lookup->x0;
        height = lookup->y1 - lookup->y0;
    } else {
        baseX = (s32)object->x;
        objectY = (s32)object->y;
        baseY = (s32)object->z;
        width = config->width;
        height = config->height;
    }

    {
        s16 count;

        if ((config->entryIndex >= 0) &&
            (config->entryIndex < (count = context->count))) {
            owner = context->entries[config->entryIndex * 2];
        } else {
            owner = *object->fallbackEntry;
        }
    }

    object->resource = overlay20ConfigureResourceReloc(
        object->resource, baseX, objectY, baseY, width, height,
        config->columns, config->rows, owner, config->value0E,
        config->value0F, config->start, config->current, config->end,
        config->scaleDivisor);
    object->radius = overlay20SqrtReloc((f32)((width * width) +
                                               (height * height)));
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o020/overlay20UpdateObjectResource/func_overlay_020_F0000204_18767DC.s")
#endif
