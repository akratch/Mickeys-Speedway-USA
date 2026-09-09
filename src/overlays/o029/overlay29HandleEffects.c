#include "PR/ultratypes.h"

typedef struct Vec3f { f32 x, y, z; } Vec3f;

typedef struct Overlay29Record {
    s16 angle0;
    s16 angle1;
    s16 angle2;
    u8 pad06[2];
    f32 scalar;
    Vec3f position;
    Vec3f vector;
    s16 random0;
    s16 random1;
    s16 random2;
    s16 value;
} Overlay29Record;

typedef struct Overlay29State {
    u8 pad00[0xB];
    u8 active;
    u8 pad0C[0x10];
    Vec3f direction;
    Overlay29Record records[4];
} Overlay29State;

typedef struct Overlay29Resource {
    u8 pad00[6];
    u16 flags;
} Overlay29Resource;

typedef struct Overlay29Object {
    s16 angle0;
    s16 angle1;
    s16 angle2;
    s16 flags06;
    f32 scalar;
    Vec3f position;
    u8 pad18[0x30];
    Overlay29Resource *resource;
    u8 pad4C[0x18];
    Overlay29State *state;
    u8 pad68[0x18];
    u32 flags80;
} Overlay29Object;

extern void overlay29ResetReloc(void);
extern s16 overlay29AngleReloc(f32 x, f32 y);
extern f32 overlay29SqrtReloc(f32 value);
extern void overlay29TransformReloc(s16 *angles, Vec3f *vector);
extern s16 overlay29RandomRangeReloc(s32 low, s32 high);
extern void overlay29ActivateReloc(Overlay29Object *object, s32 mode);
extern void overlay29EmitReloc(s32 id, f32 x, f32 y, f32 z, s32 type, s32 arg);

#define INITIALIZE_RECORD(rec_) \
    do { \
        record->position.x = object->position.x; \
        record->position.y = object->position.y; \
        record->position.z = object->position.z; \
        record->angle0 = object->angle0; \
        record->angle1 = object->angle1; \
        record->angle2 = object->angle2; \
        record->scalar = object->scalar; \
        record->random0 = overlay29RandomRangeReloc(-0x500, 0x500);       \
        record->random1 = overlay29RandomRangeReloc(-0x500, 0x500);       \
        *(volatile s16 *) &record->random2 =                              \
            overlay29RandomRangeReloc(-0x500, 0x500);                     \
        *(volatile s16 *) &record->value = 0xFF;                           \
    } while (0)

/* Exact size (1028 bytes / 257 words), exact 0x48 frame, all 22 relocation
 * offsets and types.  253 of 257 words agree; the four that do not are one
 * fact.  The CSE web for `verticalAngle + 0x2000` -- defined in the second
 * block, spilled once and reloaded by the third and fourth -- takes `v0` here
 * and `a2` in the target.  Its compiler temp slot (sp+0x2C), its schedule, its
 * spill point and every other register in the function agree; the target
 * spends `a2` six times and `v0` nineteen, this candidate `a2` once and `v0`
 * twenty-four.
 *
 * The frame is a five-local census and it is unique: `object` is the parameter,
 * not an m2c copy of it (that copy alone was fifteen words and the whole frame
 * layout), and the order is record, state, angles, baseAngle, verticalAngle.
 * A sixth declared local costs 22 words at best in any of six positions.
 *
 * The double mask in the first block is load-bearing: a single `& 0xFFFFU`,
 * a `(u16)` cast or no mask at all is 72 words, first difference +0x30.
 * Both `volatile` casts in INITIALIZE_RECORD are load-bearing at eight words
 * each; the macro's own `record = (rec_)` was inert and is gone.
 *
 * What is exhausted, all flat at four words: statement order and record
 * placement across all four blocks (3,888 points); physical line grouping
 * within each block and across block boundaries (1,203 points); the mask
 * spelling of every angle expression, per block (1,110 points); macro body
 * order (576 points); the record-pointer spelling lattice (256); local and
 * prototype types (163); inner-block scope placement and declaration
 * permutation (136); `register` hints and a sixth local (50); naming the
 * shared value as a local at any position and type (48, all regressions);
 * the signature's return type and extra parameters (12); and the compile
 * flag lattice (13 -- `-Wab,-r4300_mul` is confirmed, everything else is
 * 234 words out).
 *
 * The next lever is whatever makes uopt exclude `v0` from this one web.  It is
 * a colour fact, not a schedule fact: the register live range 0x13C..0x164
 * contains no call, so `v0` is free there, which means the target's exclusion
 * is decided on the web's pre-spill extent across the three blocks.  Note the
 * permuter is not usable here -- its scratch scores this body 135 against a
 * measured 4. */
#ifdef NON_MATCHING
void func_overlay_029_F00010C4_187E374(Overlay29Object *object, s32 mode) {
    Overlay29Record *record;
    Overlay29State *state;
    s16 angles[2];
    s16 baseAngle;
    s16 verticalAngle;

    state = object->state;
    if ((mode & 1) != 0) {
        overlay29ResetReloc();
    } else if ((mode & 2) != 0) {
        baseAngle = overlay29AngleReloc(state->direction.x, state->direction.z);
        verticalAngle = overlay29AngleReloc(
            overlay29SqrtReloc((state->direction.z * state->direction.z) +
                               (state->direction.x * state->direction.x)),
            state->direction.y);

        record = &state->records[0];
        angles[0] = baseAngle;
        angles[1] = ((verticalAngle + 0x3000) & 0xFFFFU) & 0xFFFFU;
        record->vector.x = 0.0f;
        record->vector.y = 0.0f;
        record->vector.z = -10.0f;
        overlay29TransformReloc(angles, &record->vector);
        INITIALIZE_RECORD(record);

        record = &state->records[1];
        angles[1] = verticalAngle + 0x2000;
        angles[0] = baseAngle;
        record->vector.x = 0.0f;
        record->vector.y = 0.0f;
        record->vector.z = -10.0f;
        overlay29TransformReloc(angles, &record->vector);
        INITIALIZE_RECORD(record);

        record = &state->records[2];
        angles[0] = baseAngle - 0x3000;
        angles[1] = verticalAngle + 0x2000;
        record->vector.x = 0.0f;
        record->vector.y = 0.0f;
        record->vector.z = -10.0f;
        overlay29TransformReloc(angles, &record->vector);
        INITIALIZE_RECORD(record);

        record = &state->records[3];
        angles[0] = baseAngle + 0x3000;
        angles[1] = verticalAngle + 0x2000;
        record->vector.x = 0.0f;
        record->vector.y = 0.0f;
        record->vector.z = -10.0f;
        overlay29TransformReloc(angles, &record->vector);
        INITIALIZE_RECORD(record);
        state->active = 1;
    }

    if ((mode & 4) != 0) {
        object->flags80 |= 2;
        overlay29ActivateReloc(object, 1);
    }
    if ((mode & 8) != 0) {
        overlay29EmitReloc(0x27A, object->position.x, object->position.y,
                           object->position.z, 4, 0);
    }
    object->resource->flags &= ~1;
    object->flags06 |= 0x400;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o029/overlay29HandleEffects/func_overlay_029_F00010C4_187E374.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_029_F00010C4_187E374:start
 * symbol: func_overlay_029_F00010C4_187E374
 * score: 253/257 words
 * frame: 0x48
 * relocations: 22
 * first-mismatch: +0x13C
 * summary: Exact size, frame and relocations; the four remaining words are one colour fact -- the shared angle web takes v0 here and a2 in the target.
 * PLATEAU-HANDOFF:func_overlay_029_F00010C4_187E374:end
 */
