typedef signed short s16;
typedef signed int s32;
typedef float f32;

typedef struct O38Particle {
    f32 velocity;
    f32 x;
    f32 y;
    f32 z;
    f32 direction[3];
} O38Particle;

typedef struct O38Pool {
    s32 count;
    s32 alpha;
    O38Particle particles[20];
} O38Pool;

typedef struct O38Object {
    s16 type;
    s16 pad02;
    f32 pad04;
    f32 pad08;
    f32 x;
    f32 y;
    f32 z;
    char pad18[0x4C];
    O38Pool *pool;
} O38Object;

typedef struct O38Descriptor {
    char pad00[0xA];
    s16 type;
} O38Descriptor;

typedef struct O38DirectionInput {
    char pad00[0x4];
    s16 angles[2];
} O38DirectionInput;

extern s32 mathRnd(s32 minimum, s32 maximum);
extern void mathOneFloatPY(s16 *rotation, f32 *vector);

/* Emit one burst of particles for an object, seeding each with a random
 * horizontal offset, speed and pitch.
 * PROVENANCE: Mickey-derived; no external C is adapted here.
 *
 * Matched 2026-09-10 by three edits that only work together:
 *
 * 1. `direction` is initialised inside a `if (1) { }` region. Written as a
 *    plain statement, uopt reassociates `particle + 16` through `particle`'s
 *    own definition into `pool + 24`, and the direction cursor stops
 *    depending on the particle cursor -- which is what reorders the whole
 *    six-instruction preheader. Only control flow opens a uopt region, and a
 *    region boundary anywhere between the two definitions blocks the fold; a
 *    bare block does not, and neither does any spelling of the address.
 *
 * 2. The scratch rotation aggregate is eight bytes, not sixteen. The region
 *    costs eight bytes of compiler temporary below the declared block, so the
 *    aggregate has to give the same eight back for the frame to stay 0x60 and
 *    for its address to stay at sp+76.
 *
 * 3. `offset = 0;` and the region sit on one physical line. as1 breaks a tie
 *    between two ready nodes on their source line numbers, and on separate
 *    lines it emits the counter's clear ahead of the direction cursor; equal
 *    line numbers drop the tie to ready-list position, which is the shipped
 *    order. Reversing the two statements instead of folding them regresses,
 *    because the definition order is also what assigns s2 and s3.
 */
void func_overlay_038_F0000000_1885D10(O38Object *object,
                                       O38Descriptor *descriptor)
{
    O38Pool *pool = object->pool;
    O38Particle *particle;
    s32 offset;
    f32 *direction;
    O38DirectionInput randomDirection;

    object->type = descriptor->type;
    particle = pool->particles;
    offset = 0; if (1) { direction = particle->direction; }
    pool->count = 60;
    pool->alpha = 255;
    for (; offset != 0x230; offset += sizeof(O38Particle)) {
        particle->x = object->x + (f32)mathRnd(-20, 20);
        particle->y = object->y;
        particle->z = object->z + (f32)mathRnd(-20, 20);
        particle->velocity = (f32)mathRnd(1, 10) / 10.0f;
        randomDirection.angles[0] = mathRnd(-0x7FFF, 0x7FFF);
        randomDirection.angles[1] = mathRnd(0x2000, 0x4000);
        particle->direction[2] = (f32)mathRnd(20, 50) / -10.0f;
        mathOneFloatPY(randomDirection.angles, direction);
        particle++;
        direction += sizeof(O38Particle) / sizeof(f32);
    }
}
