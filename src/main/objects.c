#include "PR/ultratypes.h"

/* This TU uses -Wab,-r4300_mul; target func_8000A62C requires its three
 * R4300 multiply-hazard delay nops, and the flag is validated against the
 * existing exact object-system functions by the full-ROM verify. */

typedef struct AnimPathObject AnimPathObject;
typedef struct Gfx Gfx;
typedef struct Mtx Mtx;
typedef struct TrackVertex TrackVertex;

typedef struct TrackSkyObject {
    u8 pad00[6];
    s16 flags;
} TrackSkyObject;

typedef struct {
    u8 pad[0x58];
    s32 unk58;
} Objects06C40;

typedef struct {
    s16 unk0;
    u8 pad02[2];
    f32 unk4;
} Objects69C0Out;

typedef struct {
    u8 pad00[0x2C];
    f32 unk2C;
} Objects69C0Deep;

typedef struct {
    u8 pad00[0xE0];
    Objects69C0Deep *unkE0;
} Objects69C0Mid;

typedef struct {
    u8 pad00[0x40];
    Objects69C0Mid *unk40;
    u8 pad44[0x34];
    Objects69C0Out *unk78;
} Objects69C0In;

typedef struct {
    u8 pad00[0x1E];
    s8 unk1E[4];
    s8 unk22;
    u8 pad23[0xAD];
    f32 unkD0[4];
} Objects58C0Data;

typedef struct {
    u8 pad00[0x40];
    Objects58C0Data *unk40;
} Objects58C0Arg;

typedef struct {
    u8 pad00[0x44];
    s16 unk44;
    u8 pad46[0x22];
    s32 *unk68;
    u8 pad6C[0x1C];
    void *unk88;
} Objects08A20Arg;

typedef struct {
    u8 pad00[0x30];
    f32 unk30;
} Objects09F08Arg;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Objects0BB84Vec3;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    u8 pad0C[4];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} Objects0BB84Plane;

typedef struct {
    u8 pad00[8];
    f32 x;
    f32 y;
    f32 z;
} Objects0BB84Output;

typedef struct {
    u8 pad00[0x10];
    f32 unk10;
} Objects0BB84Depth;

typedef struct {
    u8 pad00[0xE0];
    Objects0BB84Depth *unkE0;
} Objects0BB84Node;

typedef struct {
    u8 pad00[0x40];
    Objects0BB84Node *unk40;
    u8 pad44[0x34];
    Objects0BB84Output *unk78;
} Objects0BB84Object;

typedef struct {
    u8 pad00[0x40];
    Objects58C0Data *unk40;
    u8 pad44[0x24];
    s32 *unk68;
} Objects06448Arg;

typedef struct {
    u8 pad00[8];
    s16 unk8;
    u8 pad0A[0x35];
    u8 unk3F;
} Objects08028Model;

typedef struct {
    u8 pad00[0x40];
    Objects58C0Data *unk40;
    u8 pad44[0x24];
    Objects08028Model **unk68;
} Objects08028Object;

typedef struct {
    u8 pad00[0x1E];
    s8 unk1E;
    u8 pad1F[0x35];
    f32 unk54;
    f32 unk58;
    u8 pad5C[5];
    u8 unk61;
    u8 unk62;
    u8 unk63;
    s16 unk64;
    u16 unk66;
} Objects069E8Source;

typedef struct {
    f32 unk0;
    f32 unk4;
    s32 unk8;
    u16 unkC;
    u16 unkE;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 pad14[8];
    s32 unk1C;
} Objects069E8Target;

typedef struct {
    u8 pad00[0x40];
    Objects069E8Source *unk40;
    u8 pad44[8];
    Objects069E8Target *unk4C;
} Objects069E8Object;

extern void *D_800C94D8[];
extern s32 D_800C9470;
extern s32 D_800C9474;
extern s32 D_800C94A8;
extern s32 D_800C94AC;
extern s32 *D_800C94A4;
extern s32 D_800C94B4;
extern s32 D_800C94B8;
extern s32 D_800C94BC;
extern s16 D_800C9508[];
extern s16 D_800C94B0;
extern s16 D_800C94B2;
extern s32 D_80078F84;
extern s8 D_80078F88;
extern s8 D_80079004;
extern u8 D_8007BF0C;
extern f32 D_80080F84;
extern void **D_800C94F4;
extern s32 D_800C94F8;
extern void **D_800C9494;
extern s32 D_800C9498;
extern s32 D_800C949C;
extern s32 D_800C94FC;
extern s32 D_800C9490;
extern u8 *D_800C9460;
extern s32 D_800C9468;
extern s32 *D_800C9458;
extern s16 *D_800C94E0;
extern s32 D_800C94C0[];
extern s32 D_800C94C8[];
extern void **D_800C94EC;
extern s32 D_800C94F0;
extern void **D_800C9500;
extern s32 D_800C9504;
extern s32 piRomLoadSection(u32 assetIndex, u32 address, s32 assetOffset, s32 size);
extern f32 sqrtf(f32 value);
extern void func_80006FA0(void);
extern void func_80007118();
extern void TrapDanglingJump();
extern void mmFree(void *data);
extern void modFreeModel(void *resource);
extern void func_800347A0(void *texture);
extern void func_800359D4(void *sprite);
extern void func_80009F74(TrackSkyObject *object);
extern s32 D_80079008[];
extern s32 D_800790D0[];
extern void func_8000831C(void *arg0, void *arg1, s32 arg2, void *arg3, s32 arg4,
                          s32 arg5, s32 arg6, s32 arg7, f32 arg8, s32 arg9, s32 arg10);
extern s32 func_800291FC(void);
extern s32 func_80034448(s16 resourceId, void *output);

void func_80004340(void) {
    D_800C9470 = 0;
    D_800C9474 = 0;
    D_800C9498 = 0;
    D_800C949C = 0;
    D_800C94A8 = 0;
    D_800C94AC = 0;
    D_800C94F0 = 0;
    D_800C94F8 = 0;
    D_800C9504 = 0;
    D_800C94B0 = 0;
    D_800C94B2 = 0;
}
void func_8000439C(void) {
    s32 count;
    s32 i;
    s32 offset;

    D_800C94FC = 0;
    func_80006FA0();
    count = D_800C9498;
    i = 0;
    offset = 0;
    if (count > 0) {
        do {
            func_80007118(*(void **)((u8 *)D_800C9494 + offset), 1);
            i += 1;
            offset += 4;
        } while (i != count);
    }
    D_800C94F0 = 0;
    D_800C9498 = 0;
    D_800C949C = 0;
    func_80004340();
    mmFree(D_800C94D8[0]);
    mmFree(D_800C94D8[1]);
    D_80078F84 = 0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004454.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004590.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000471C.s")
void func_8000485C(s8 arg0) {
    D_80078F88 = arg0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000486C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004B04.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004C28.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004FE0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80005548.s")
/* Workbench verdict: allocation-mismatch (temp-FIFO phase); 8 differing words. */
/* First mismatch: +0x24. */
/* Shape-exact candidate; register allocation is reserved for the permuter. */
#ifdef NON_MATCHING
void *func_800056A4(s32 tableIndex) {
    s32 **table = (s32 **)0x800C9464;

    if ((tableIndex < 0) || (tableIndex >= D_800C9468)) {
        return D_800C9460;
    }
    return D_800C9460 + ((*table)[tableIndex] * 4);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_800056A4.s")
#endif
void *func_800056F0(s32 index) {
    if ((index < 0) || (index >= D_800C9498)) {
        return 0;
    }
    return D_800C9494[index];
}
void **func_8000572C(s32 *start, s32 *end) {
    *start = D_800C949C;
    *end = D_800C9498;
    return D_800C9494;
}
void **func_80005750(s32 *count) {
    *count = D_800C94F8;
    return D_800C94F4;
}
void func_80005768(AnimPathObject *object) {
    D_800C9500[D_800C9504] = object;
    D_800C9504 += 1;
}
void func_80005798(void *object) {
    s32 i;

    if (D_800C9504 > 0) {
        for (i = 0; i < D_800C9504; i++) {
            if (D_800C9500[i] == object) {
                D_800C9500[i] = D_800C9500[D_800C9504 - 1];
            }
        }
        D_800C9504 -= 1;
    }
}
void **func_80005808(s32 *count) {
    *count = D_800C9504;
    return D_800C9500;
}
s32 func_80005820(s32 arg0) {
    if (D_800C94F8 == 0) {
        return 0;
    }
    if ((arg0 < 0) || (arg0 >= D_800C94F8)) {
        return 0;
    }
    return D_800C94F4[arg0];
}
/* Workbench verdict: allocation-mismatch; 8 differing words. */
/* First mismatch: +0x1C. */
/* Shape-exact candidate; stack-home/register allocation is reserved for the permuter. */
#ifdef NON_MATCHING
s16 func_80005868(s32 arg0) {
    u8 buffer[0xC0];
    s16 index = D_800C94E0[arg0];

    piRomLoadSection(0x2D, (u32)buffer, D_800C9458[index], 0xC0);
    return *(s16 *)(buffer + 0x1C);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80005868.s")
#endif
s8 func_800058C0(Objects58C0Arg *arg0, s32 arg1) {
    if ((arg1 >= 4) || (arg0->unk40->unkD0[arg1] == 0.0f)) {
        return arg0->unk40->unk1E[0];
    }
    return arg0->unk40->unk1E[arg1];
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000590C.s")
/* Workbench verdict: structure-mismatch; 27 differing words (59/59). */
/* First mismatch: +0x1C; size, frame, and relocations are exact. */
/* Structural gap: CFE s-register carriers and resource-load carrier differ. */
#ifdef NON_MATCHING
void func_80006448(void *arg0) {
    s32 offset = 0;
    s32 index = 0;
    void *resource;
    s8 type;

    if (((Objects06448Arg *)arg0)->unk40->unk22 > 0) {
        do {
            type = func_800058C0((Objects58C0Arg *)arg0, index);
            if (type == 0) {
                resource = *(void **)((u8 *)((Objects06448Arg *)arg0)->unk68 + offset);
                if (resource != 0) {
                    modFreeModel(resource);
                }
            } else if (type == 2) {
                resource = *(void **)((u8 *)((Objects06448Arg *)arg0)->unk68 + offset);
                if (resource != 0) {
                    func_800347A0(resource);
                }
            } else {
                resource = *(void **)((u8 *)((Objects06448Arg *)arg0)->unk68 + offset);
                if (resource != 0) {
                    func_800359D4(resource);
                }
            }
            index += 1;
            offset += 4;
        } while (index < ((Objects06448Arg *)arg0)->unk40->unk22);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006448.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006534.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006868.s")
s32 func_800069C0(Objects69C0In *arg0, Objects69C0Out *arg1) {
    arg0->unk78 = arg1;
    arg1->unk4 = arg0->unk40->unkE0->unk2C;
    arg0->unk78->unk0 = 2;
    return 0x2C;
}
/* Workbench verdict: allocation-mismatch; 19 differing words (52/71). */
/* First mismatch: +0x8C; size, frame, CFG, and constants are exact. */
/* Structural gap: none; pool/temp register allocation is reserved for the permuter. */
#ifdef NON_MATCHING
s32 func_800069E8(Objects069E8Object *arg0, Objects069E8Target *arg1) {
    Objects069E8Source *source;
    Objects069E8Source *source2;
    s32 sp1C;
    s32 temp_v0;
    s32 temp_v0_2;

    source2 = arg0->unk40;
    source = source2;
    arg0->unk4C = arg1;
    arg1->unk0 = source2->unk54;
    arg1->unk4 = source2->unk58;
    arg1->unk10 = source2->unk61;
    arg1->unk11 = source2->unk62;
    arg1->unk12 = source2->unk63;
    arg1->unk13 = 0;
    arg1->unkC = 0;
    arg1->unkE = source2->unk66;
    sp1C = 0x20;
    arg1->unk1C = 0;
    if (func_800291FC() == 0) {
        if (arg1->unk10 & 8) {
            temp_v0 = ((s32)((u8 *)arg1 + 0x20) & ~3) + 4;
            arg1->unk1C = temp_v0;
            sp1C = (temp_v0 - (s32)arg1) + 0xBC;
        }
    } else {
        arg1->unk10 = 5;
        arg1->unk11 = 1;
    }
    if (arg0->unk40->unk1E == 0) {
        arg1->unk10 |= 0x20;
    }
    temp_v0_2 = func_80034448(source->unk64, arg1);
    arg1->unk8 = temp_v0_2;
    if (temp_v0_2 == 0) {
        return 0;
    }
    D_800C9490 = arg1->unk8;
    return (sp1C & ~3) + 4;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_800069E8.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006B04.s")
s32 func_80006C40(Objects06C40 *arg0, s32 arg1) {
    arg0->unk58 = arg1;
    return 0x13C;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006C4C.s")
void func_80006EA0(void *ptr) {
    if (((u8 *) ptr)[0x91] == 0) {
        ((u8 *) ptr)[0x91] = 1;
        D_800C94EC[D_800C94F0] = ptr;
        D_800C94F0 += 1;
    }
}
/* Workbench verdict: structure-mismatch; 40 differing words, candidate 46/47. */
/* First mismatch: +0x08; the target has one direct global-pointer load extra. */
/* Structural candidate: global-pointer load/register shape remains unresolved. */
#ifdef NON_MATCHING
s32 func_80006EE4(s32 object) {
    s32 index;
    s32 *entry;

    index = 0;
    if (D_800C94A8 > 0) {
        entry = D_800C94A4;
        do {
            if (object == *entry) {
                D_800C94A8 -= 1;
                if (index < D_800C94A8) {
                    s32 shiftIndex = index;
                    s32 offset = shiftIndex * 4;

                    do {
                        shiftIndex += 1;
                        *(s32 *)((u8 *)D_800C94A4 + offset) =
                            *(s32 *)((u8 *)D_800C94A4 + offset + 4);
                        offset += 4;
                    } while (shiftIndex < D_800C94A8);
                }
                func_80007118(object, 0, index, &D_800C94A8);
                return index;
            }
            index += 1;
            entry += 1;
        } while (index < D_800C94A8);
    }
    return -1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006EE4.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006FA0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80007118.s")
void func_80007844(void) {
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000784C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80007C68.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80007E40.s")
/* Workbench verdict: structure-mismatch; 28 differing words (60/60). */
/* First mismatch: +0x24; size and frame are exact, with a near-identical CFG. */
/* Structural gap: outer object/offset carriers and inner model-index allocation differ. */
#ifdef NON_MATCHING
void func_80008028(s32 arg0) {
    s32 objectIndex;
    Objects08028Object *object;
    s32 objectOffset;
    s32 modelIndex;
    s32 updateModels;
    Objects58C0Data *data;
    Objects08028Model *model;
    u8 modelReferences;

    objectIndex = D_800C949C;
    if (objectIndex < D_800C9498) {
        objectOffset = objectIndex * 4;
        do {
            objectIndex += 1;
            object = *(Objects08028Object **)((u8 *)D_800C9494 + objectOffset);
            data = object->unk40;
            if (data->unk1E[0] == 0) {
                updateModels = 0;
                if (data->unkD0[1] != 0.0f) {
                    updateModels = 1;
                }
                if (data->unk22 > 0) {
                    modelIndex = 0;
                    do {
                        if ((updateModels == 0) || (data->unk1E[modelIndex] == 0)) {
                            model = object->unk68[modelIndex];
                            modelReferences = model->unk3F;
                            model->unk8 = arg0;
                            if (modelReferences != 0) {
                                model->unk3F = modelReferences - 1;
                            }
                        }
                        modelIndex += 1;
                    } while (modelIndex < object->unk40->unk22);
                }
            }
            objectOffset += 4;
        } while (objectIndex < D_800C9498);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80008028.s")
#endif
void func_80008118(void) {
    D_80079004 = 1;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80008128.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000831C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_800084C4.s")
void func_80008A20(Objects08A20Arg *arg0) {
    func_8000831C(arg0, D_80079008, 0x14, D_800790D0, 0x18, *arg0->unk68, 2, 0, 1.0f, 0xFF, 0xFF);
}
void func_80008A8C(Objects08A20Arg *arg0) {
    switch (arg0->unk44) {
        case 0x3D:
            if (arg0->unk88 != NULL) {
                TrapDanglingJump(arg0);
            }
            TrapDanglingJump(&D_800C94B4, arg0);
            break;
        case 0x24:
            TrapDanglingJump(&D_800C94B4, &D_800C94B8, arg0);
            break;
        case 0x3B:
            func_80008A20(arg0);
            break;
        case 0x41:
            TrapDanglingJump(arg0, &D_800C94B4, &D_800C94B8, &D_800C94BC);
            break;
        case 0x55:
            TrapDanglingJump(&D_800C94B4, &D_800C94B8, arg0);
            break;
        case 0x5B:
            TrapDanglingJump(&D_800C94B4, &D_800C94B8, arg0);
            break;
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80008B94.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009220.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009414.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009AA8.s")
void func_80009E78(Gfx **displayList, Mtx **matrix, TrackVertex **vertices,
                   TrackSkyObject *object) {
    if ((object->flags & 0xC00) == 0) {
        D_800C94B4 = (s32) *displayList;
        D_800C94B8 = (s32) *matrix;
        D_800C94BC = (s32) *vertices;
        func_80009F74(object);
        *displayList = (Gfx *) D_800C94B4;
        *matrix = (Mtx *) D_800C94B8;
        *vertices = (TrackVertex *) D_800C94BC;
    }
}
f32 func_80009F08(Objects09F08Arg *arg0) {
    f32 temp_f0;
    f32 var_f2;

    var_f2 = 1.0f;
    if (D_8007BF0C == 0) {
        temp_f0 = arg0->unk30;
        if (temp_f0 > 250.0f) {
            var_f2 += (temp_f0 - 250.0f) * D_80080F84;
            if (var_f2 > 2.0f) {
                var_f2 = 2.0f;
            }
        }
    }
    return var_f2;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009F74.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A244.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A39C.s")
/* PROVENANCE: body adapted from Jet Force Gemini's public src/objects.c
 * setObjectViewNormal; Mickey's target globals and byte output are authoritative. */
void func_8000A62C(f32 x, f32 y, f32 z) {
    f32 vecLength = sqrtf((x * x) + (y * y) + (z * z));
    f32 normalizedLength;

    if (vecLength != 0.0f) {
        normalizedLength = -8192.0f / vecLength;
        x *= normalizedLength;
        y *= normalizedLength;
        z *= normalizedLength;
    }
    D_800C9508[0] = x;
    D_800C9508[1] = y;
    D_800C9508[2] = z;
}
void func_8000A6DC(s32 arg0) {
    D_800C94B0 = arg0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A6E8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A830.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000AA38.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000AEEC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000B3CC.s")
/* Workbench verdict: structure-mismatch; 61 differing words (65/65). */
/* First mismatch: +0x0; target frame is 0x28, candidate frame is 0x30. */
/* Structural gap: FP register/stack allocation and argument homes differ. */
#ifdef NON_MATCHING
void func_8000BB84(s32 arg0, Objects0BB84Vec3 *arg1, Objects0BB84Vec3 *arg2,
                   f32 arg3, Objects0BB84Plane *arg4, Objects0BB84Object *arg5) {
    f32 normalX;
    f32 normalY;
    f32 normalZ;
    f32 inputX;
    f32 inputY;
    f32 inputZ;
    f32 dot;
    f32 factor;
    f32 reflectedX;
    f32 reflectedY;
    f32 reflectedZ;
    f32 scale;
    Objects0BB84Output *output;

    output = arg5->unk78;
    normalX = arg4->x;
    normalY = arg4->y;
    normalZ = arg4->z;
    inputX = arg2->x;
    inputY = arg2->y;
    inputZ = arg2->z;
    dot = (normalX * inputX) + (normalY * inputY) + (normalZ * inputZ);
    factor = 2.0f * -dot;
    reflectedX = inputX + (factor * normalX);
    reflectedY = inputY + (factor * normalY);
    reflectedZ = inputZ + (factor * normalZ);
    scale = (arg3 - arg4->unk1C) * arg5->unk40->unkE0->unk10;
    arg1->x = arg4->unk10 + (scale * reflectedX);
    arg1->y = arg4->unk14 + (scale * reflectedY);
    arg1->z = arg4->unk18 + (scale * reflectedZ);
    output->x = normalX;
    output->y = normalY;
    output->z = normalZ;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000BB84.s")
#endif
void GetRomlistInfo(s32 *romlist, s32 *size, s32 index) {
    *romlist = D_800C94C0[index];
    *size = D_800C94C8[index];
}
/* Workbench verdict: schedule-mismatch; 2 differing words. */
/* First mismatch: +0x3C. */
/* Shape-exact candidate; FP scheduling is reserved for the permuter. */
#ifdef NON_MATCHING
/* PROVENANCE: JFG, src/objects.c (GetRange); adapted to Mickey's symbol. */
f32 func_8000BCB0(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    f32 temp_f0;
    f32 temp_f16;
    f32 temp_f2;

    temp_f0 = arg0 - arg3;
    temp_f2 = arg1 - arg4;
    temp_f16 = arg2 - arg5;
    return sqrtf((temp_f0 * temp_f0) + (temp_f2 * temp_f2) + (temp_f16 * temp_f16));
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000BCB0.s")
#endif
/* Workbench verdict: schedule-mismatch; 2 differing words. */
/* First mismatch: +0x2C. */
/* Shape-exact candidate; instruction ordering is reserved for the permuter. */
#ifdef NON_MATCHING
/* PROVENANCE: JFG, src/objects.c (GetRangeSquared); adapted to Mickey's symbol. */
f32 func_8000BD0C(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5) {
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f2;

    temp_f2 = arg0 - arg3;
    temp_f16 = arg1 - arg4;
    temp_f18 = arg2 - arg5;
    return (temp_f2 * temp_f2) + (temp_f16 * temp_f16) + (temp_f18 * temp_f18);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000BD0C.s")
#endif

/* PLATEAU-HANDOFF:func_8000BD0C:start
 * symbol: func_8000BD0C
 * score: 2 differing words
 * frame: frameless
 * relocations: 0
 * first-mismatch: +0x2C
 * summary: late FP scheduling tie; JFG GetRangeSquared shape is exact and the bounded permuter is next
 * PLATEAU-HANDOFF:func_8000BD0C:end
 */

/* PLATEAU-HANDOFF:func_800056A4:start
 * symbol: func_800056A4
 * score: 8 differing words
 * frame: frameless
 * relocations: 6
 * first-mismatch: +0x24
 * summary: fixed-address D_800C9464 pointer-variable form has exact shape and relocations; one temp-FIFO phase remains
 * PLATEAU-HANDOFF:func_800056A4:end
 */

/* PLATEAU-HANDOFF:func_80005868:start
 * symbol: func_80005868
 * score: 8 differing words
 * frame: -0xE0
 * relocations: 5
 * first-mismatch: +0x1C
 * summary: target DMA buffer stack home is 8 bytes earlier; six register and two stack-offset words remain
 * PLATEAU-HANDOFF:func_80005868:end
 */

/* PLATEAU-HANDOFF:func_8000BCB0:start
 * symbol: func_8000BCB0
 * score: 2 differing words
 * frame: -0x18
 * relocations: 1
 * first-mismatch: +0x3C
 * summary: late FP scheduling tie in the JFG GetRange adaptation; bounded permuter is next
 * PLATEAU-HANDOFF:func_8000BCB0:end
 */

/* PLATEAU-HANDOFF:func_80006EE4:start
 * symbol: func_80006EE4
 * score: 46/47 words
 * frame: 0x28
 * relocations: 7
 * first-mismatch: +0x8
 * summary: One target direct global-pointer load and allocator shape remain unresolved after bounded source attempts.
 * PLATEAU-HANDOFF:func_80006EE4:end
 */

/* PLATEAU-HANDOFF:func_80006448:start
 * symbol: func_80006448
 * score: 32/59 words
 * frame: 0x28
 * relocations: 4
 * first-mismatch: +0x1C
 * summary: Shape and relocation surface are exact; CFE carrier allocation differs for the object, offset, and resource values.
 * PLATEAU-HANDOFF:func_80006448:end
 */

/* PLATEAU-HANDOFF:func_80008028:start
 * symbol: func_80008028
 * score: 32/60 words
 * frame: 0x8
 * relocations: 8
 * first-mismatch: +0x24
 * summary: Near-identical control-flow shape and frame; CFE allocates outer object/offset and inner model-index carriers differently.
 * PLATEAU-HANDOFF:func_80008028:end
 */

/* PLATEAU-HANDOFF:func_8000BB84:start
 * symbol: func_8000BB84
 * score: 4/65 words
 * frame: 0x30
 * relocations: 0
 * first-mismatch: +0x0
 * summary: Arithmetic/control-flow size is exact, but the target's 0x28 frame and FP register/stack allocation remain unresolved.
 * PLATEAU-HANDOFF:func_8000BB84:end
 */

/* PLATEAU-HANDOFF:func_800069E8:start
 * symbol: func_800069E8
 * score: 52/71 words
 * frame: 0x28
 * relocations: 4
 * first-mismatch: +0x8C
 * summary: Shape exact; uopt global-color and ugen temp-carrier allocation remain for the permuter.
 * PLATEAU-HANDOFF:func_800069E8:end
 */
