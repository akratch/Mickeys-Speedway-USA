/*
 * Model animation loading and matrix generation -- ROM 0x5B300-0x5C310
 * (VRAM 0x8005A700-0x8005B710).
 *
 * The TU name is Tier B/D: its callers and data flow establish animation-table
 * loading, reference-counted animation allocation, frame selection and model
 * matrix construction. JFG models.c supplies the nearest non-exact skeletons
 * for the loader/free pair. camConvertMatrixList alone is Tier A against JFG
 * camera.c; that isolated helper does not turn the full range into camera.c.
 *
 * PROVENANCE: JFG's permitted src/models.c, models.h and camera.c were read
 * for names, layouts and comparison. The initial split adapted no body;
 * point-of-use notes identify the later JFG adaptations. The func_8005A948
 * flag lattice showed that this TU's cache loop has target length only with
 * `-Wo,-loopunroll,0`, recorded as a per-file Makefile override.
 */

#include "PR/ultratypes.h"
#include "game/pi.h"

typedef f32 Matrix[4][4];

typedef struct ConvListEntry {
    Matrix *mtx;
    s16 count;
} ConvListEntry;

typedef struct AnimationCacheEntry {
    s32 id;
    u8 *animation;
} AnimationCacheEntry;

typedef struct LoadedAnimation {
    u8 references;
    u8 pad1[3];
    s16 id;
} LoadedAnimation;

typedef struct ModelMatrixNode {
    s16 parent;
    u8 pad2[2];
    f32 x;
    f32 y;
    f32 z;
} ModelMatrixNode;

typedef struct ModelAnimationTable {
    u8 pad0[0x4E];
    s8 animationCount;
    u8 pad4F;
    u8 **animations;
} ModelAnimationTable;

typedef struct ModelAnimationFrame {
    u8 pad0;
    u8 flags;
    s16 offset;
    u8 pad4[2];
    u8 loop;
    u8 pad7;
    u8 count;
} ModelAnimationFrame;

typedef struct ModelAnimationInfo {
    u8 pad0[0x4E];
    s8 frameCount;
    u8 pad4F;
    ModelAnimationFrame **frames;
} ModelAnimationInfo;

typedef struct ModelAnimationState {
    ModelAnimationInfo *info;
    u8 pad4[0x18];
    ModelAnimationFrame *frame;
    void *frameData;
    u8 pad24[4];
    f32 frameValue;
    f32 pad2C;
    f32 blendStart;
    f32 blendEnd;
    f32 blendValue;
    s16 frameIndex;
    s8 transition;
    u8 hasNext;
} ModelAnimationState;

typedef struct ModelAnimationInstance {
    u8 pad0[0x28];
    f32 frameValue;
    u8 pad2C[0xE];
    s8 animationIndex;
    s8 frame;
    u8 pad3C[0x2C];
    ModelAnimationState **states;
} ModelAnimationInstance;

typedef struct ModelRenderSlot {
    u8 pad0[0xC];
    Matrix *matrices;
    u8 pad10[4];
    s32 count;
} ModelRenderSlot;

typedef struct ModelRenderInstance {
    u8 pad0[4];
    s32 count;
    u8 pad8[2];
    s16 activeSlot;
    u8 padC[0x10];
    s32 animated;
    u8 pad20[8];
    f32 scale;
    f32 offset;
    u8 pad30[0xF];
    u8 mode;
    f32 *vertices[3];
} ModelRenderInstance;

typedef struct ModelRenderPointA {
    u16 vertex;
    u16 node;
} ModelRenderPointA;

typedef struct ModelRenderPointB {
    u16 vertex;
    s8 node;
    u8 pad3[9];
} ModelRenderPointB;

typedef struct ModelRenderContext {
    u8 pad0[0x1C];
    u8 *vertexData;
    u8 pad20[0xD];
    u8 count0;
    u8 count1;
    u8 count2;
    ModelRenderPointA *points0;
    ModelRenderPointB *points1;
    ModelRenderPointB *points2;
    u8 pad3C[0x13];
    s8 matrixCount;
    u8 pad50[4];
    ModelMatrixNode *nodes;
} ModelRenderContext;

typedef struct ModelRenderAsset {
    s8 cameraIndex;
    u8 pad1[0x4F];
    f32 scale;
    u8 pad54[0x3E8];
    s16 angle;
} ModelRenderAsset;

typedef struct ModelRenderModel {
    u8 pad0[6];
    s16 flags;
    f32 transformScale;
    u8 padC[0x1C];
    f32 scale;
    u8 pad2C[0x18];
    s16 type;
    u8 pad46[0x1E];
    ModelRenderAsset *asset;
} ModelRenderModel;

typedef struct ModelRenderCamera {
    u8 pad0[0xC];
    f32 x;
    f32 y;
    f32 z;
    u8 pad18[0x3C];
} ModelRenderCamera;

typedef struct ModelRenderNodeData {
    u8 pad0[0x94];
    f32 x;
    f32 y;
    f32 z;
} ModelRenderNodeData;

typedef struct ModelRenderMatrixNode {
    u8 pad0[0x30];
    f32 x;
    f32 y;
    f32 z;
} ModelRenderMatrixNode;

typedef struct ModelRenderVertex {
    s16 x;
    s16 y;
    s16 z;
} ModelRenderVertex;

typedef struct ModelRenderTransform {
    s16 rotation0;
    s16 rotation1;
    s16 rotation2;
    u8 pad6[2];
    f32 scale;
    f32 x;
    f32 y;
    f32 z;
} ModelRenderTransform;

extern s32 D_800D7CF0;
extern s32 D_800D7CF4;
extern s32 D_800D7CF8;
extern s32 D_800D7CFC;
extern s32 D_800D7D00;
extern s32 D_800D7D04;
extern ConvListEntry D_800D78F0[];

s32 func_8002B280(s32 size, s32 tag);
void *func_8002B314(s32 size, u32 colourTag);
void func_80058FF0(ConvListEntry *entries, s32 count);
void func_8002A82C(void *mtx);
void mtxf_mul(void *lhs, void *rhs, void *dest);
void func_8002AA50(void *transform, void *matrix);
void func_80029AB8(void *matrix, f32 scale);
void mtxf_transform_point(Matrix matrix, f32 x, f32 y, f32 z,
                          f32 *outX, f32 *outY, f32 *outZ);
ModelRenderCamera *camGetListPtr(void);
s32 camGetMode(void);
s32 func_800290A0(void);
s32 Arctanf(f32 y, f32 x);
f32 func_8002A8BC(s16 angle);
f32 func_8002A8C0(s16 angle);
void func_8002B040(void *matrix, s32 x, s32 y, s32 z,
                   f32 *outX, f32 *outY, f32 *outZ);
void func_800591B0(Matrix *matrices, Matrix root,
                    ModelRenderInstance *instance, ModelMatrixNode *nodes,
                    void *asset);
void func_8005B644(Matrix *matrices, Matrix *root, ModelMatrixNode *node,
                   s32 count);
void mmFree(void *ptr);
u8 *func_8005A948(s16 animationId);
void func_8005AAC0(u8 *animation);

/* PROVENANCE: adapted from the modelsInit tail in JFG src/models.c. */
void func_8005A700(void) {
    s32 allocation;

    allocation = func_8002B280(0xA0, 0x80);
    D_800D7CFC = allocation;
    D_800D7D00 = allocation + 0x80;
    D_800D7CF8 = allocation + 0x90;
    D_800D7CF4 = func_8002B280(0x800, 0x80);
    D_800D7D04 = 0;
    D_800D7CF0 = 0;
}
void func_8005A764(void) {
    D_800D7CF0 = 0;
}
void func_8005A770(void) {
    func_80058FF0(D_800D78F0, D_800D7CF0);
    D_800D7CF0 = 0;
}
/*
 * PROVENANCE: Mickey-derived. JFG src/models.c::modLoadModel remains assembly
 * and supplies role/TU context only; no donor body was imported.
 *
 * Plateau: retained configured full-TU evidence is exact-sized at 106 words,
 * but uses frame 0x50 versus target 0x38. Ten raw/normalized sites remain at
 * +0x000/+0x08C/+0x0A0/+0x0BC/+0x0C0/+0x0C4/+0x0C8/+0x0DC/+0x0F4/+0x1A4.
 * The ten relocation identities agree, but the second piRomLoadSection call
 * moves from target +0xBC to candidate +0xC0. Alignment remains v1/sp+0x30
 * instead of s0/sp+0x34. More than ten source forms and all 119 flag groups
 * exhausted the frame/carrier/schedule route; one unchanged configured
 * full-TU/linked V0 remains, then park absent a new allocator mechanism.
 */
#ifdef NON_MATCHING
s32 func_8005A7A0(ModelAnimationTable *model, s32 modelId) {
    s32 alignment;
    s32 firstAnimation;
    s32 lastAnimation;
    s32 loadSize;
    s32 loaded;
    s32 inputOffset;
    u16 *bounds;

    piRomLoadSection(0x28, (void *)D_800D7D00, (modelId & ~3) * 2, 0x10);
    bounds = (u16 *)D_800D7D00 + (modelId & 3);
    firstAnimation = bounds[0] >> 1;
    lastAnimation = bounds[1] >> 1;
    model->animationCount = lastAnimation - firstAnimation;
    if (firstAnimation == lastAnimation) {
        return TRUE;
    }

    alignment = firstAnimation & 3;
    loadSize = ((model->animationCount & ~3) + 4) << 1;
    if (alignment != 0) {
        loadSize += 8;
    }
    piRomLoadSection(0x29, (void *)D_800D7CFC, (firstAnimation & ~3) * 2, loadSize);
    model->animations = (u8 **)func_8002B314(model->animationCount * 4, 0x80);
    if (model->animations == NULL) {
        return FALSE;
    }

    loaded = 0;
    inputOffset = alignment * 2;
    alignment = 0;
    do {
        *(u8 **)((u8 *)model->animations + alignment) =
            func_8005A948(*(s16 *)(D_800D7CFC + inputOffset));
        if (*(u8 **)((u8 *)model->animations + alignment) == NULL) {
            alignment = 0;
            if (loaded > 0) {
                inputOffset = 0;
                do {
                    func_8005AAC0(*(u8 **)((u8 *)model->animations + inputOffset));
                    alignment++;
                    inputOffset += 4;
                } while (alignment != loaded);
            }
            mmFree(model->animations);
            model->animations = NULL;
            return FALSE;
        }
        loaded++;
        inputOffset += 2;
        alignment += 4;
    } while (loaded < model->animationCount);
    return TRUE;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/models_5B300/func_8005A7A0.s")
#endif
/* Plateau (near-miss p10): the surviving configured full-TU object is the
 * earlier 11-word state at +0x40/+0x44/+0x80/+0x130/+0x148/+0x14C/+0x150/
 * +0x154/+0x158/+0x15C/+0x160, exact-sized at 94 words/frame 0x38 with 13
 * exact relocation tuples. The later raw interleaved-array source removed
 * the final eight sites at commit time, leaving historical p10 evidence at
 * +0x40/+0x44/+0x80, but no configured or linked p10 artifact survives.
 * Reprove unchanged p10 once; if those three t7/target-t8 FIFO sites reproduce,
 * run one instrumented ugen causal trace and at most its indicated
 * address-producer spelling, then park if flat. */
#ifdef NON_MATCHING
u8 *func_8005A948(s16 animationId) {
    s32 i;
    s32 emptyIndex;
    s32 offset;
    s32 size;
    LoadedAnimation *animation;

    emptyIndex = -1;
    i = 0;
    if (D_800D7D04 > 0) {
        do {
            AnimationCacheEntry *entry = &((AnimationCacheEntry *)D_800D7CF4)[i];

            if ((animationId == entry->id) != 0U) {
                u8 *existing = entry->animation;

                existing[0]++;
                return existing;
            }
            if (entry->id == -1) {
                emptyIndex = i;
            }
            i++;
        } while (i < D_800D7D04);
    }

    if (emptyIndex == -1) {
        emptyIndex = D_800D7D04;
        if (D_800D7D04 >= 0x100) {
            return NULL;
        }
        D_800D7D04++;
    }

    i = (animationId & 1) * 4;
    piRomLoadSection(0x2A, (u8 *)D_800D7CF8, (animationId & ~1) * 4, 0x10);
    offset = *(s32 *)(D_800D7CF8 + i);
    size = *(s32 *)(D_800D7CF8 + i + 4) - offset;
    animation = (LoadedAnimation *)func_8002B314(size, 0x80);
    if (animation == NULL) {
        return NULL;
    }

    piRomLoadSection(0x2B, animation, offset, size);
    animation->references = 1;
    animation->id = animationId;
    ((s32 *)D_800D7CF4)[emptyIndex * 2] = animationId;
    ((u8 **)D_800D7CF4)[(emptyIndex * 2) + 1] = (u8 *)animation;
    return (u8 *)animation;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/models_5B300/func_8005A948.s")
#endif

/* PROVENANCE: Mickey-only reconstruction informed by JFG's corresponding
 * modFreeAnim identity and structure; the public JFG peer remained assembly,
 * and no external C body is copied. */
void func_8005AAC0(u8 *animation) {
    s32 i;
    s32 index;

    if (animation != NULL) {
        animation[0]--;
        if (animation[0] > 0) {
            return;
        }
        index = -1;
        if (D_800D7D04 > 0) {
            i = 0;
            do {
                if (animation == ((u8 **)D_800D7CF4)[(i << 1) + 1]) {
                    index = i;
                }
                i++;
            } while (i < D_800D7D04);
        }
        if (index != -1) {
            mmFree(animation);
            ((s32 *)D_800D7CF4)[index * 2] = -1;
            ((s32 *)D_800D7CF4)[index * 2 + 1] = -1;
        }
    }
}
/* PROVENANCE: adapted from JFG src/camera.c (camConvertMatrixList). */
void camConvertMatrixList(Matrix *mtx, s32 count) {
    s32 index = D_800D7CF0;
    ConvListEntry *entry = &D_800D78F0[index];

    entry->mtx = mtx;
    D_800D7CF0 = index + 1;
    entry->count = count;
}

/* Keep the original TU order: func_8005ABA8 precedes func_8005AD64. */
/* Workbench: structure-mismatch, 97 differing words, first mismatch +0x38. */
/* Candidate shape: 110 instructions/no frame vs target 111/no frame; not permuter-ready. */
/* Remaining structural gap: preserve the validated frame pointer in a2 before the split. */
/* PROVENANCE: Mickey-only reconstruction from func_8005ABA8.s and the
 * existing models TU layouts; no external function body is copied. */
#ifdef NON_MATCHING
s32 func_8005ABA8(ModelAnimationInstance *instance, f32 arg1, f32 arg2) {
    s32 var_v1;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f2;
    f32 temp_f2_2;
    void *temp_a1;
    ModelAnimationState *temp_v0;

    temp_v0 = instance->states[(s32)instance->animationIndex];
    var_v1 = 0;
    temp_a1 = temp_v0->frame;
    if (temp_a1 == NULL) {
        return 0;
    }
    if (temp_v0->transition != 0) {
        if (temp_v0->hasNext != 0) {
            temp_f2 = temp_v0->blendEnd;
            temp_f0 = temp_v0->blendValue + arg2;
            temp_v0->blendValue = 0.0f;
            temp_v0->blendEnd = temp_f2 - temp_f0;
            temp_v0->blendStart = temp_f0 / temp_f2;
        } else {
            temp_v0->blendValue = temp_v0->blendValue + arg2;
        }
        temp_f2_2 = temp_v0->blendEnd;
        if ((temp_f2_2 <= 0.0f) || (temp_f2_2 <= temp_v0->blendValue)) {
            temp_v0->transition = 0;
            temp_v0->blendStart = 0.0f;
            temp_v0->blendValue = 0.0f;
            instance->frameValue = (f32)temp_v0->frameIndex /
                                   temp_v0->frameValue;
        }
    } else {
        instance->frameValue += arg1 * arg2;
        temp_f0_2 = instance->frameValue;
        if (temp_f0_2 >= 1.0f) {
            if (((ModelAnimationFrame *)temp_a1)->loop != 0) {
                if (temp_f0_2 >= 1.0f) {
                    do {
                        instance->frameValue -= 1.0f;
                    } while (instance->frameValue >= 1.0f);
                    var_v1 = 1;
                } else {
                    goto animation_done;
                }
            } else {
                instance->frameValue = 1.0f;
animation_done:
                var_v1 = 1;
            }
        } else if (temp_f0_2 < 0.0f) {
            var_v1 = 1;
            if (((ModelAnimationFrame *)temp_a1)->loop != 0) {
                if (temp_f0_2 < 0.0f) {
                    do {
                        instance->frameValue += 1.0f;
                    } while (instance->frameValue < 0.0f);
                }
            } else {
                instance->frameValue = 0.0f;
            }
        }
    }
    return var_v1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/models_5B300/func_8005ABA8.s")
#endif
/* Workbench: structure-mismatch, 74 differing words, first mismatch +0x0. */
/* Candidate shape: 111 instructions/no frame vs target 108/no frame; not permuter-ready. */
/* Remaining structural gap: argument reloads and unsigned-count conversion add 3 instructions. */
/* PROVENANCE: Mickey-only reconstruction from func_8005AD64.s and the
 * existing models TU layouts; no external function body is copied. */
#ifdef NON_MATCHING
void func_8005AD64(ModelAnimationInstance *instance, s32 frame, s32 arg2,
                   f32 value) {
    f32 temp_f0;
    f32 var_f6;
    s32 temp_f6;
    s32 var_a1;
    s32 temp_a1;
    s32 var_a3;
    s32 temp_t9;
    s32 var_v1;
    ModelAnimationFrame *temp_a0;
    ModelAnimationState *temp_v0;
    ModelAnimationInfo *temp_v1;

    temp_v0 = instance->states[(s32)instance->animationIndex];
    var_a3 = frame;
    temp_v1 = temp_v0->info;
    if (temp_v1->frameCount != 0) {
        if (value > 1.0f) {
            value = 1.0f;
        } else if (value < 0.0f) {
            value = 0.0f;
        }
        instance->frameValue = value;
        temp_a1 = temp_v1->frameCount;
        if (var_a3 >= temp_a1) {
            var_a3 = temp_a1 - 1;
        } else if (var_a3 < 0) {
            var_a3 = 0;
        }
        instance->frame = var_a3;
        var_a1 = 0;
        if ((temp_v0->frame != NULL) && (temp_v0->hasNext != 0)) {
            var_a1 = 1;
        }
        temp_a0 = temp_v1->frames[var_a3];
        temp_v0->frame = temp_a0;
        temp_v0->frameData = (u8 *)temp_a0 + temp_a0->offset + 0x14;
        temp_t9 = temp_a0->count;
        var_f6 = (f32)temp_t9;
        if (temp_t9 < 0) {
            var_f6 += 4294967296.0f;
        }
        temp_v0->frameValue = var_f6;
        if (temp_a0->loop == 0) {
            temp_v0->frameValue = temp_v0->frameValue - 1.0f;
        }
        if (arg2 != -1) {
            var_v1 = arg2;
        } else {
            var_v1 = temp_a0->flags;
        }
        if ((var_a1 != 0) && (var_v1 != 0)) {
            temp_f0 = temp_v0->frameValue * value;
            temp_v0->transition = 1;
            temp_v0->blendStart = 0.0f;
            temp_f6 = (s32)temp_f0;
            temp_v0->blendEnd = (f32)var_v1;
            if ((temp_f0 - (f32)temp_f6) >= 0.5f) {
                temp_v0->frameIndex = temp_f6 + 1;
                return;
            }
            temp_v0->frameIndex = temp_f6;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/models_5B300/func_8005AD64.s")
#endif

/*
 * Plateau: the animation-frame update's closest reconstruction emits 110
 * instructions against 111 and follows the broad target CFG, but diverges at
 * +0x38 before cascading through the FP allocator. The 119-combination flag
 * lattice found no exact result; its closest alternate still differs in 59
 * words and would also perturb this TU's already-exact functions.
 */
/*
 * PROVENANCE: field roles were cross-checked against Jet Force Gemini's
 * permitted public decomp at pinned commit c82affff, specifically
 * include/structs.h (ObjectModel/ModelInstance), src/models.h, and
 * src/camera.h. JFG's peer body remains assembly; Mickey's offsets, node
 * selection, control flow, and call sequence are reconstructed from Mickey.
 */
/* Workbench: structure-mismatch, 377 differing words, first mismatch +0x0. */
/* Structural gap: target 460 instructions/frame -0xF8 versus candidate 463/-0x110; camera-angle stack layout remains unresolved. */
/* Next: constant-audit the earliest immediate, then repair structure before register allocation (workbench mixed-residual routing). */
/* Not shape-exact or permuter-ready; model matrix and attachment-point control flow are represented. */
#ifdef NON_MATCHING
void func_8005AF14(ModelRenderInstance *instance, ModelRenderContext *context,
                   ModelRenderModel *model) {
    s32 matrixList;
    ModelRenderSlot *slot;
    Matrix *activeMatrices;
    ModelRenderAsset *asset;
    ModelRenderCamera *camera;
    ModelRenderNodeData *nodeData;
    ModelRenderMatrixNode *matrixNode;
    ModelRenderPointA *pointA;
    ModelRenderPointB *pointB;
    ModelRenderVertex *vertex;
    void *assetPart;
    u8 *matrixBase;
    f32 *output;
    f32 scale;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    f32 sine;
    f32 cosine;
    s16 yaw;
    s16 pitch;
    s16 angle;
    s16 rawAngle;
    s16 clampedAngle;
    s16 scaledAngle;
    s32 index;
    s32 pointOffset;
    s32 temp;

    {
        Matrix matrix;

        scale = 1.0f;
        if (model->type == 1) {
            asset = model->asset;
            func_8002AA50((u8 *) asset + 0x43C, matrix);
            if (asset->scale != 1.0f) {
                scale = asset->scale;
                func_80029AB8(matrix, asset->scale);
            }
        } else {
            func_8002AA50(model, matrix);
        }

        instance->activeSlot ^= 1;
    slot = (ModelRenderSlot *) ((u8 *) instance + (instance->activeSlot * 4));
    matrixList = (s32) slot->matrices;
    instance->count = slot->count;
    if (instance->animated == 0) {
        func_8005B644((Matrix *) matrixList, matrix, context->nodes, context->matrixCount);
    } else {
        instance->offset = model->scale * instance->scale;
        switch (model->type) {
        case 1:
            assetPart = (u8 *) model->asset + 0x1B8;
            break;
        case 0x36:
            assetPart = (u8 *) model->asset + 0x3E;
            break;
        case 0x37:
            assetPart = (u8 *) model->asset + 0x28;
            break;
        case 0x54:
            assetPart = (u8 *) model->asset + 0x1C;
            break;
        case 0x56:
            assetPart = (u8 *) model->asset + 0x10;
            break;
        default:
            assetPart = NULL;
            break;
        }
        func_800591B0((Matrix *) matrixList, matrix, instance, context->nodes, assetPart);
        instance->mode = 2;
        }
    }

    if ((model->flags & 0x1000) != 0) {
        ModelRenderTransform transform;

        camera = camGetListPtr();
        if ((model->type == 1) && (asset->cameraIndex >= 0)) {
            temp = asset->cameraIndex;
            if (camGetMode() >= temp) {
                camera += temp;
            }
        }
        slot = (ModelRenderSlot *) ((u8 *) instance + (instance->activeSlot * 4));
        activeMatrices = slot->matrices;
        matrixBase = (u8 *) activeMatrices;
        matrixNode = (ModelRenderMatrixNode *) (matrixBase + 0x240);
        deltaX = camera->x - matrixNode->x;
        deltaY = camera->y - matrixNode->y;
        deltaZ = camera->z - matrixNode->z;
        yaw = Arctanf(deltaX, deltaZ);
        if (deltaY < 0.0f) {
            deltaY *= deltaY;
        } else {
            deltaY = -(deltaY * deltaY);
        }
        pitch = Arctanf(deltaY, (deltaX * deltaX) + (deltaZ * deltaZ));
        func_8002B040(matrixBase + 0x200, 0, 0, 0x3F800000,
                      &deltaX, &deltaY, &deltaZ);
        angle = -yaw;
        sine = func_8002A8C0(angle);
        cosine = func_8002A8BC(angle);
        rawAngle = Arctanf(-((deltaX * cosine) + (deltaZ * sine)),
                           (deltaZ * cosine) - (deltaX * sine));
        clampedAngle = rawAngle;
        if (rawAngle >= 0x4001) {
            clampedAngle = 0x4000 - (rawAngle - 0x4000);
        } else if (rawAngle < -0x4000) {
            clampedAngle = -0x4000 - (rawAngle + 0x4000);
        }
        scaledAngle = (s16) ((s32) (((f32) clampedAngle / 16384.0f) * 8192.0f));
        func_8002B040(matrixBase + 0x200, 0, 0x3F800000, 0,
                      &deltaX, &deltaY, &deltaZ);
        sine = func_8002A8C0(angle);
        transform.rotation0 = yaw;
        transform.rotation1 = pitch;
        transform.rotation2 = scaledAngle +
                               Arctanf(-((deltaX * func_8002A8BC(angle)) +
                                          (deltaZ * sine)), deltaY);
        transform.scale = model->transformScale;
        nodeData = (ModelRenderNodeData *) context->nodes;
        mtxf_transform_point((Matrix *) (matrixBase + 0x200), nodeData->x,
                             nodeData->y, nodeData->z, &transform.x,
                             &transform.y, &transform.z);
        func_8002AA50(&transform, matrixNode);
        func_80029AB8(matrixNode, scale);
    }

    if ((model->type == 1) && (func_800290A0() == 0)) {
        slot = (ModelRenderSlot *) ((u8 *) instance + (instance->activeSlot * 4));
        func_8002B040((u8 *) slot->matrices, 0, 0, 0x3F800000,
                      &deltaX, &deltaY, &deltaZ);
        asset->angle = Arctanf(deltaX, deltaZ);
    }

    output = instance->vertices[0];
    index = 0;
    pointOffset = 0;
    if ((s32) context->count0 > 0) {
        do {
            pointA = (ModelRenderPointA *) ((u8 *) context->points0 + pointOffset);
            vertex = (ModelRenderVertex *) (context->vertexData + (pointA->vertex * 0xA));
            slot = (ModelRenderSlot *) ((u8 *) instance + (instance->activeSlot * 4));
            activeMatrices = slot->matrices;
            mtxf_transform_point((Matrix *) ((u8 *) activeMatrices + (pointA->node << 6)),
                                 (f32) vertex->x, (f32) vertex->y, (f32) vertex->z,
                                 output, output + 1, output + 2);
            index++;
            pointOffset += 4;
            output += 3;
        } while (index < (s32) context->count0);
    }

    output = instance->vertices[1];
    index = 0;
    pointOffset = 0;
    if ((s32) context->count1 > 0) {
        do {
            pointB = (ModelRenderPointB *) ((u8 *) context->points1 + pointOffset);
            vertex = (ModelRenderVertex *) (context->vertexData + (pointB->vertex * 0xA));
            slot = (ModelRenderSlot *) ((u8 *) instance + (instance->activeSlot * 4));
            activeMatrices = slot->matrices;
            mtxf_transform_point((Matrix *) ((u8 *) activeMatrices + ((s32) pointB->node << 6)),
                                 (f32) vertex->x, (f32) vertex->y, (f32) vertex->z,
                                 output, output + 1, output + 2);
            index++;
            pointOffset += 0xC;
            output += 3;
        } while (index < (s32) context->count1);
    }

    output = instance->vertices[2];
    index = 0;
    pointOffset = 0;
    if ((s32) context->count2 > 0) {
        do {
            pointB = (ModelRenderPointB *) ((u8 *) context->points2 + pointOffset);
            vertex = (ModelRenderVertex *) (context->vertexData + (pointB->vertex * 0xA));
            slot = (ModelRenderSlot *) ((u8 *) instance + (instance->activeSlot * 4));
            activeMatrices = slot->matrices;
            mtxf_transform_point((Matrix *) ((u8 *) activeMatrices + ((s32) pointB->node << 6)),
                                 (f32) vertex->x, (f32) vertex->y, (f32) vertex->z,
                                 output, output + 1, output + 2);
            index++;
            pointOffset += 0xC;
            output += 3;
        } while (index < (s32) context->count2);
    }
    camConvertMatrixList((Matrix *) matrixList, context->matrixCount);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/models_5B300/func_8005AF14.s")
#endif

/* Mickey-derived parented matrix-list builder; JFG retains its peer as asm. */
void func_8005B644(Matrix *matrices, Matrix *root, ModelMatrixNode *node, s32 count) {
    Matrix temp;
    Matrix *output;
    Matrix *parent;
    s32 i;

    output = matrices;
    i = 0;
    if (count > 0) {
        do {
            func_8002A82C((u8 *)temp - 8);
            *(f32 *)((u8 *)temp + 0x28) = node->x;
            *(f32 *)((u8 *)temp + 0x2C) = node->y;
            *(f32 *)((u8 *)temp + 0x30) = node->z;
            if ((node->parent == -1) != FALSE) {
                parent = root;
            } else {
                parent = &matrices[node->parent];
            }
            mtxf_mul((u8 *)temp - 8, parent, output);
            i++;
            output++;
            node++;
        } while (i != count);
    }
}
