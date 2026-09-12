#include "PR/ultratypes.h"

typedef struct Overlay89EffectState {
    u8 mode;
    u8 particleCount;
    u8 index;
    u8 count;
    u8 red;
    u8 green;
    u8 blue;
    u8 intensity;
    u8 enabled;
    u8 pad09;
    s16 timer;
    f32 squaredRange;
    s16 minX;
    s16 minY;
    s16 left;
    s16 right;
    s16 top;
    s16 bottom;
    s32 motionWord;
    void *primaryHandle;
    f32 radius;
    f32 scale;
    void *secondaryHandle;
} Overlay89EffectState;

typedef struct Overlay89ColorEntry {
    u8 pad00[6];
    u8 red;
    u8 green;
    u8 blue;
    u8 pad09;
} Overlay89ColorEntry;

typedef struct Overlay89NestedHeader {
    u8 pad00[0x12];
    s16 count;
    u8 pad14[8];
    Overlay89ColorEntry *entries;
} Overlay89NestedHeader;

typedef struct Overlay89NestedRoot {
    Overlay89NestedHeader *header;
    Overlay89ColorEntry *colors;
} Overlay89NestedRoot;

typedef struct Overlay89Status {
    u8 pad00[0x132];
    s16 initialized;
} Overlay89Status;

typedef struct Overlay89Object {
    s16 angleA;
    s16 angleB;
    u8 pad04[4];
    f32 size;
    u8 pad0C[0x2E];
    u8 active;
    u8 pad3B[0x1D];
    Overlay89Status *status;
    u8 pad5C[8];
    Overlay89EffectState *state;
    Overlay89NestedRoot **nested;
} Overlay89Object;

typedef struct Overlay89Init {
    u8 pad00[0xA];
    u8 angleA;
    u8 angleB;
    s8 minX;
    s8 minY;
    u8 halfWidth;
    u8 halfHeight;
    u8 mode;
    u8 index;
    u8 particleCount;
    u8 count;
    u8 red;
    u8 green;
    u8 blue;
    u8 intensity;
    u8 range;
    u8 flags;
    u8 descriptorMode;
    u8 pad1B;
    u16 squaredRange;
    u8 pad1E;
    u8 size;
    u8 radius;
    u8 scale;
} Overlay89Init;

typedef struct Overlay89CreateDescriptor {
    u8 mode;
    s8 kind;
    u8 flags;
    s8 sentinelByte;
    s16 zero04;
    s16 zero06;
    s16 zero08;
    s16 zero0A;
    s16 sizeA;
    s16 sizeB;
    u8 red;
    u8 green;
    u8 blue;
    u8 intensity;
    s16 sentinel;
    s8 zero16;
    s8 zero17;
} Overlay89CreateDescriptor;

extern f32 gOverlay89InitScale[];
extern u8 overlay89Evaluate(Overlay89EffectState *state);
extern void *overlay89CreatePrimaryReloc(Overlay89Object *object,
                                         Overlay89CreateDescriptor *descriptor,
                                         Overlay89EffectState *state,
                                         Overlay89Init *init);
extern void overlay89MaintainReloc(Overlay89Object *object,
                                   Overlay89EffectState *state);

/* 205 instructions, frame 0x58 and five relocation sites are exact.  58 masked words fell to 21
 * on 2026-09-12 (lane p7-ovl3) in three steps, each measured against the whole 205-word target:
 *
 *   - Assigning `colors` before `source` in the nested-colour loop is 58 -> 45.  The two pointers
 *     are a closed v0/v1 cycle in `register_census` (one global mapping, one window, 100% coherent),
 *     which [L127] says is ONE ring-phase fact and not thirteen colour problems: initialising them
 *     in the other order swaps the pair and closes all thirteen at zero width.  Swapping their
 *     DECLARATIONS instead is byte-identical, so it is the definition order that decides.
 *   - Dropping both re-reads of `object->state` -- the one after the create call and the one inside
 *     `if (root != NULL)` -- is 45 -> 27.  Neither edit pays alone: the first alone is -4 bytes and
 *     69 words, the second alone +4 and 77.  Together they are delta 0 and 27, which is [L100]'s
 *     warning that a direction must not be discarded because one half of it regresses.
 *   - Declaration order is then worth 27 -> 21 (`root` first, `state` after `descriptor`), fixing
 *     the a2 spill home: sp+0x54 -> sp+0x38, aligned immediate-only 7 -> 1.  11,520 orders were
 *     measured and 21 is the floor; all of them keep the 0x58 frame.
 *
 * The 21 that remain are 18 register naming, 1 immediate-only and 2 structural:
 *
 *   17 of the 18 are one float ring phase.  The instrumented uopt records exactly two class-2 webs
 *   in this procedure, webs 42 and 16, and BOTH take colour 24 with an EMPTY forbidden mask
 *   (`available0=0x000000fc`, numintf=0, cost 0).  An empty mask is what makes this reading the
 *   TABLE rather than one web's constraint: this procedure offers c24..c29 only.  c24 is f0, so the
 *   `size` local is coloured f0 and the ugen float ring starts one position later than the target's
 *   -- the target spends f6/f8/f10/f16 where we spend f0/f6/f8/f10.  The target has no such web.
 *   Removing it by inlining the expression does reach the colour (naming 18 -> 13) but hoists the
 *   `gOverlay89InitScale[1]` load ahead of the conversion and costs 8 structural words (diff 2 -> 10,
 *   masked 26).  Nineteen forms of the inlined family were measured -- both operand orders, an
 *   extra parenthesisation, `*(gOverlay89InitScale + 1)`, a dead trailing assignment, two
 *   statement positions, four region boundaries and a block-scoped scale local -- and the family
 *   bottoms at 26 while the local family holds 21, with nothing between the two minima.  Naming
 *   an integer intermediate instead (`count = (u32)init->size`) deletes real instructions
 *   (delta -16 and -36) and is not the same edit.
 *   Forcing web 42 off c24 is worth 2 words at most (c28 gives 19), so colour is not the lever --
 *   the web has to stop existing without moving the load.
 *   The 2 structural are the maintain call: the target has `move a0,s0` then `move a1,a2`, reusing
 *   the state pointer it reloads from sp+0x38, where we emit `move a0,s0` then `lw a1,100(s0)`.
 *   Passing `state` there instead of `object->state` is NOT that edit -- it deletes four more
 *   instructions (delta -16, 195 words).
 *
 * 2026-09-12, lane p8-close: the colour half is bounded harder and the blocker is renamed.
 *
 *   The earlier note priced colour from a one-web sweep. The JOINT grid -- both class-2 webs
 *   against all six offered colours, 36 cells, plus both split paths -- floors at 17, at web 42
 *   on c24 with web 16 on c28. So colour is worth at most 4 of the 21, not 2, and it still
 *   cannot close the function: both webs take the same colour and the ugen float ring only
 *   shifts when NO web holds it.
 *
 *   Read from the object, the first float block makes the fact exact. The target draws five
 *   scratch float registers there and we draw four, because our conversion result lands in the
 *   coloured web instead of the ring; every later float register in the procedure is one ring
 *   position behind as a result. Deleting the `size` local is the whole 17 words.
 *
 *   The inline form already has the right ring -- five scratch draws, and the multiply's
 *   destination is the target's -- and loses on ONE thing: it evaluates the scale load before
 *   the conversion, so the load takes the first ring slot and everything else moves up. The two
 *   source operand orders compile BYTE-IDENTICALLY, so IDO canonicalises the commutative
 *   multiply and the source cannot choose the order by writing it. Measured flat at 26 on top of
 *   the nineteen forms already recorded: `const` and sized array declarations of the scale
 *   symbol, `*(g + 1)`, `(g + 1)[0]`, an extra parenthesisation, `| 0` and `+ 0` folded into the
 *   conversion, and `if (1) { }`, `do { } while (0)` and a bare brace before or around the
 *   statement. Also refuted, none at 21 or better: the assignment-in-expression and comma forms
 *   (55 each), `object->size` as its own intermediate (187), a ternary (200) and a second use of
 *   `size` (197); reusing `range` as the intermediate and an integer-typed intermediate are
 *   byte-identical to the incumbent.
 *
 *   Decision variable for the next lane: a source form in which the multiply's conversion operand
 *   is evaluated before its memory operand while both remain in one statement. Nothing measured
 *   moves IDO's canonicalisation, so this is a weight question inside ugen, not a spelling. */
#ifdef NON_MATCHING
void overlay89InitializeEffect(Overlay89Object *object,
                               Overlay89Init *init) {
    Overlay89NestedRoot *root;
    Overlay89CreateDescriptor descriptor;
    Overlay89EffectState *state;
    Overlay89ColorEntry *source;
    Overlay89ColorEntry *colors;
    s32 count;
    f32 size;
    f32 range;

    object->angleA = init->angleA << 8;
    state = object->state;
    object->angleB = init->angleB << 8;

    size = (f32)(u32)init->size;
    object->size = size * gOverlay89InitScale[1];

    state->mode = init->mode;
    state->particleCount = init->particleCount;
    state->index = init->index;
    state->count = init->count;
    state->red = init->red;
    state->green = init->green;
    state->blue = init->blue;
    state->intensity = init->intensity;
    state->enabled = 1;
    state->timer = 0;

    range = (f32)(u32)init->squaredRange;
    state->squaredRange = range * range;
    state->minX = init->minX << 6;
    state->minY = init->minY << 6;
    state->left = object->angleA - (init->halfWidth << 7);
    state->right = object->angleA + (init->halfWidth << 7);
    state->top = object->angleB - (init->halfHeight << 7);
    state->bottom = object->angleB + (init->halfHeight << 7);
    state->motionWord = 0;

    state->enabled = overlay89Evaluate(state);
    state->primaryHandle = NULL;
    state->secondaryHandle = NULL;

    descriptor.mode = init->descriptorMode;
    descriptor.kind = 3;
    descriptor.flags = (~init->flags) & 0x61;
    descriptor.sentinelByte = -1;
    descriptor.zero04 = 0;
    descriptor.zero06 = 0;
    descriptor.zero08 = 0;
    descriptor.zero0A = 0;
    descriptor.sizeA = init->range * 8;
    descriptor.sizeB = init->range * 8;
    descriptor.red = init->red;
    descriptor.green = init->green;
    descriptor.blue = init->blue;
    descriptor.intensity = state->enabled ? init->intensity : 0;
    descriptor.sentinel = -1;
    descriptor.zero16 = 0;
    descriptor.zero17 = 0;
    if (init->descriptorMode == 3) {
        descriptor.flags |= 0x40;
    }

    state->primaryHandle =
        overlay89CreatePrimaryReloc(object, &descriptor, state, init);
    state->scale = (f32)(init->scale * 8);
    state->radius = (f32)(init->radius * 8);

    if (init->descriptorMode == 3 && !(init->flags & 0x40)) {
        overlay89MaintainReloc(object, object->state);
    }

    object->status->initialized = 1;
    object->active = 0;

    root = *object->nested;
    if (root != NULL) {
        colors = root->colors;
        source = root->header->entries;
        count = root->header->count;
        while (count--) {
            if (source->red == 0 && source->green == 0 && source->blue == 0) {
                colors->red = state->red;
                colors->green = state->green;
                colors->blue = state->blue;
            }
            source++;
            colors++;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o089/overlay89InitializeEffect/func_overlay_089_F0000270_18D44A0.s")
#endif

/* PLATEAU-HANDOFF:overlay89InitializeEffect:start
 * symbol: overlay89InitializeEffect
 * score: 21/205 words
 * frame: 0x58
 * relocations: 5
 * first-mismatch: +0x40
 * summary: source question is conversion-before-scale evaluation in one statement
 * PLATEAU-HANDOFF:overlay89InitializeEffect:end
 */
