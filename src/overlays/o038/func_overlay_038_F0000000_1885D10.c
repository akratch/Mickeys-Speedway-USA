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
    char pad00[0xC];
    s16 angles[2];
} O38DirectionInput;

extern s32 mathRnd(s32 minimum, s32 maximum);
extern void mathOneFloatPY(s16 *rotation, f32 *vector);

/* Workbench (2026-08-28): 340 B/85 words, exact 0x60 frame, with
 * seven schedule words at +0x48..+0x60. The module relocation table resolves
 * six calls to resident mathRnd and the seventh to resident mathOneFloatPY;
 * they are not masked match credit.
 * Particle/direction declaration initializers regress to 11 words and swap
 * s2/s3; inherited pointer-chain and initializer sweeps were exhausted.
 * Re-prove unchanged V0 with those seven identities; canonical assembly stays
 * absent a new scheduling mechanism. */
#ifdef NON_MATCHING
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
    offset = 0;
    direction = particle->direction;
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
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o038/func_overlay_038_F0000000_1885D10/func_overlay_038_F0000000_1885D10.s")
#endif
