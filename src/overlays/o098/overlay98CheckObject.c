#include "PR/ultratypes.h"

/* Exact O98 +0x0848 owner; DKR v77/v80 and JFG donor scans are negative. */
typedef struct Vec3f { f32 x, y, z; } Vec3f;

typedef struct O98NodeData {
    u8 pad00[0x44];
    s16 height;
} O98NodeData;

typedef struct O98Node {
    O98NodeData *data;
} O98Node;

typedef struct O98Object {
    u8 pad00[8];
    f32 scale;
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[0x22];
    s8 nodeIndex;
    u8 pad3B[5];
    s8 *stateTable;
    u8 pad44[0x24];
    O98Node **nodes;
    u8 pad6C[0x27];
    u8 stateIndex;
} O98Object;

extern s32 overlay98CheckInitialReloc(f32, f32, f32 *, s32, s32);
extern s32 overlay98CheckCandidateReloc(void *, Vec3f *, Vec3f *, s32, s32);
extern s32 overlay98UniqueCountReloc;
extern s16 overlay98UniqueYReloc[];

/* PROVENANCE: reproduced from the retired public mirror, commit
 * f56d08c746f891f76c4b7bab8e3a2a4332894634, which no longer resolves. All
 * retained measurements were independently derived from Mickey's own source,
 * object, relocation tables, and retail bytes. */
/* Workbench allocation-mismatch: 31 raw words; 111 instructions, frame 0xA8,
 * and six relocation sites have exact shape after moving scratch[10] before
 * the vector locals. The remaining register/operand allocation is a coherent
 * cycle; the assembly fallback stays canonical. */
#ifdef NON_MATCHING
s32 overlay98CheckObject(O98Object *object, u8 *context, f32 *result) {
    s32 scratch[10];
    Vec3f output;
    Vec3f input;
    f32 adjustment;
    s32 index;
    s32 accepted;
    (void)&scratch;

    accepted = 0;
    if (object->stateTable[object->stateIndex + 0x1E] == 0) {
        O98Node *node = object->nodes[object->nodeIndex];
        adjustment = (f32)node->data->height * object->scale;
    } else {
        adjustment = 0.0f;
    }

    if ((overlay98CheckInitialReloc(object->x, object->z, result, 0x8000, 0) & 0x8000) &&
        ((*result - 5.0f) < object->y)) {
        accepted = 1;
    } else {
        input.x = object->x;
        input.z = object->z;
        index = 0;
        if (overlay98UniqueCountReloc > 0) {
            do {
                f32 height = (f32)overlay98UniqueYReloc[index];

                input.y = (height - (object->y - height)) - adjustment;
                if (overlay98CheckCandidateReloc(context + 0xC, &input, &output, 0, 0x8000) != 0) {
                    *result = output.z;
                    accepted = 1;
                    index = overlay98UniqueCountReloc;
                }
                index++;
            } while (index < overlay98UniqueCountReloc);
        }
    }
    return accepted;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o098/overlay98CheckObject/func_overlay_098_F0000848_18D9208.s")
#endif

/* PLATEAU-HANDOFF:overlay98CheckObject:start
 * symbol: overlay98CheckObject
 * score: 80/111 words (31 differing)
 * frame: 0xA8
 * relocations: 6
 * first-mismatch: +0x38
 * summary: frame and instruction/relocation shape exact; moving scratch[10] first removes the five displaced stack-home differences, leaving 31 register/operand differences. A capped permutation found no exact real-TU candidate.
 * PLATEAU-HANDOFF:overlay98CheckObject:end
 */
