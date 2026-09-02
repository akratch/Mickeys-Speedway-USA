#include "PR/ultratypes.h"
#include "game/particles.h"

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

typedef struct {
    u8 pad00[0xA6];
    u8 unkA6;
    u8 padA7;
    u8 *unkA8;
    s32 *unkAC;
} Objects04B04Object;

typedef struct {
    s32 unk0;
    s32 unk4;
} Objects06868Entry;

typedef struct {
    u8 pad00[0x25];
    s8 unk25;
    u8 pad26[0x1E];
    Objects06868Entry *unk44;
} Objects06868Data;

typedef struct {
    u8 pad00[0x40];
    Objects06868Data *unk40;
    u8 pad44[0x28];
    ParticleTrigger *unk6C;
} Objects06868Object;

typedef struct {
    u8 pad00[0x14];
    u16 unk14;
} Objects0A244Header;

typedef struct {
    u8 pad00[0x40];
    Objects0A244Header *unk40;
} Objects0A244Object;

typedef struct {
    u8 pad00[0xC];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 pad18[0x2C];
    s16 unk44;
    u8 pad46[0x4B];
    u8 unk91;
} Objects04454Object;

typedef struct {
    u8 pad00[0x1B];
    u8 unk1B;
} Objects0471CData;

typedef struct {
    u8 pad00[0xC];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 pad18[0x28];
    Objects0471CData *unk40;
    u8 pad44[0x4D];
    u8 unk91;
} Objects0471CObject;

typedef struct {
    u8 pad00[0x1E];
    s8 unk1E;
} Objects06B04Source;

typedef struct {
    s32 unk0;
    s32 unk4;
} Objects0831CCommand;

typedef struct {
    u8 pad00[3];
    u8 unk3;
    u8 pad04[0xA];
    u16 unkE;
    u16 unk10;
    u8 pad12[0xE];
} Objects07C68Texture;

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
} Objects07C68Record;

typedef struct {
    u8 pad00[0x18];
    void **unk18;
    u8 pad1C[0x10];
    u8 unk2C;
} Objects07C68Source;

typedef struct {
    u8 pad00[0xA];
    s16 unkA;
    u8 pad0C[0x40];
    Objects07C68Record *unk4C;
    u8 pad50[0x40];
    u8 unk90;
} Objects07C68Object;

typedef struct {
    u8 pad00[0x50];
    s16 *unk50;
} Objects07C68Indexed;

typedef struct {
    u16 unk0;
    s8 unk2;
    u8 unk3;
    u16 unk4;
    u16 unk6;
    f32 unk8;
} Objects06B04Entry;

typedef struct {
    u8 pad00[0x2F];
    u8 unk2F;
    u8 pad30[8];
    u8 *unk38;
} Objects06B04Asset;

typedef struct {
    u8 pad00[8];
    f32 unk8;
    u8 pad0C[0x34];
    Objects06B04Source *unk40;
    u8 pad44[4];
    void *unk48;
    u8 pad4C[0x1C];
    Objects06B04Asset ***unk68;
} Objects06B04Object;

typedef struct {
    u8 pad00[0xA];
    s16 unkA;
    u8 pad0C[0x68];
    u8 *unk74;
} Objects06B04Output;

typedef struct {
    f32 unk0;
    u8 pad04[0x10];
    u16 unk14;
    u8 pad16[6];
    s16 unk1C;
    u8 pad1E[4];
    s8 unk22;
    u8 pad23[0x11];
    s32 *unk34;
} Objects06C4CAsset;

typedef struct {
    u8 pad00[6];
    s16 unk6;
    f32 unk8;
    u8 pad0C[0x20];
    s16 unk2C;
    u8 pad2E[0x12];
    Objects06C4CAsset *unk40;
    u8 pad44[2];
    s16 unk46;
    u8 pad48[0x20];
    s32 *unk68;
    u8 pad6C[0x28];
    s32 unk94[1];
} Objects06C4CObject;

extern Objects06C4CAsset *func_8000486C(s32 arg0);
extern void *func_8001F520(s32 assetId, s32 flags);
extern void *func_8002B314(s32 size, s32 tag);
extern void *func_800355A0(s32 assetId, s32 flags);

typedef struct {
    u8 pad00[6];
    s16 unk6;
    u8 pad08[4];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 pad18[0x18];
    f32 unk30;
} Objects0A39CObject;

typedef struct {
    f32 pad00[2];
    f32 unk8;
    f32 pad0C[3];
    f32 unk18;
    f32 pad1C[3];
    f32 unk28;
    f32 pad2C[3];
    f32 unk38;
} Objects0A39CMatrix;

typedef struct {
    u8 unk0;
    u8 pad01[7];
    s16 unk8;
    u8 pad0A[0x0E];
    s16 unk18;
} Objects07E40Group;

typedef struct {
    u8 pad00[4];
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
} Objects07E40Record;

typedef struct {
    u8 pad00[0x10];
    u8 unk10;
    u8 pad11[5];
    s16 unk16;
    u8 *unk18;
    u8 pad1C[4];
    u8 *unk20;
    u8 *unk24;
} Objects07E40Inner;

typedef struct {
    Objects07E40Inner *unk0;
} Objects07E40Outer;

typedef struct {
    u8 pad00[0x22];
    s8 unk22;
    u8 pad23[0x7F];
    u8 unkA2;
    u8 unkA3;
    s8 unkA4;
    s8 unkA5;
} Objects07E40Data;

typedef struct {
    u8 pad00[0x40];
    Objects07E40Data *unk40;
    u8 pad44[0x24];
    Objects07E40Outer **unk68;
} Objects07E40Object;

typedef struct {
    s16 pad00;
    s16 pad02;
    s16 pad04;
    s16 unk6;
    s16 unk8;
} Objects07E40Texture;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} Objects08128Bounds;

typedef struct {
    u8 pad00[0x20];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
} Objects08128Track;

typedef struct {
    u8 pad00[0x0C];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 pad18[0x16];
    s16 unk2E;
} Objects08128Object;

extern Objects08128Track *trackGetTrack(void);
extern Objects08128Bounds *func_8000FEEC(s32);
extern s32 func_8000FAE0(f32, f32, f32);

typedef struct {
    u8 pad00[0xA6];
    u8 unkA6;
    u8 padA7;
    u8 *unkA8;
    void **unkAC;
} Objects09220Data;

typedef struct {
    u8 pad00[8];
    f32 unk8;
    u8 pad0C[0x34];
    Objects09220Data *unk40;
} Objects09220Object;

typedef struct {
    u8 pad00[0x26];
    s16 unk26;
    s16 unk28;
    s16 unk2A;
} Objects09220Source;

typedef struct {
    f32 pad00[2];
    f32 unk8;
    f32 pad0C[3];
    f32 unk18;
    f32 pad1C[3];
    f32 unk28;
} Objects09220Matrix;

typedef struct {
    u32 w0;
    u32 w1;
} Objects09220Gfx;

extern void *camGetRotationMtx(void);
extern void mathOneFloatPY(void *, f32 *);
extern void mtxf_transform_point(void *, f32, f32, f32, f32 *, f32 *, f32 *);
extern void func_80034DF0(u8, u8, u8, u8, u8, u8);
extern void func_80034E48(void);
extern void func_80023A08(void **, s32, s32, s16 *, s32, s32, s32);
extern f32 sqrtf(f32);
extern f32 D_80080F80;

typedef struct {
    u8 pad00[0x1C];
    s16 unk1C;
} Objects0A830Data;

typedef struct {
    u8 pad00[0x40];
    Objects0A830Data *unk40;
    u8 pad44[0x20];
    void *unk64;
} Objects0A830Object;

extern void *D_800C94D8[];
extern s32 D_800C9470;
extern s32 D_800C9474;
extern s32 D_800C94A8;
extern s32 D_800C94AC;
extern s32 *D_800C9450;
extern s32 D_800C9454;
extern s32 D_800C945C;
extern Objects04B04Object **D_800C9488;
extern u16 *D_800C948C;
extern s32 *D_800C94A4;
extern s32 D_800C94B4;
extern s32 D_800C94B8;
extern s32 D_800C94BC;
extern s16 D_800C9508[];
extern s16 D_800C94B0;
extern s16 D_800C94B2;
extern s32 D_80078F84;
extern Objects04454Object *D_80078F20;
extern s8 D_80078F88;
extern s8 D_80079004;
extern u8 D_8007BDA0;
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
extern s32 func_8000A6E8(s32 arg0);
extern void func_80009F74(TrackSkyObject *object);
extern s32 func_800290A0(void);
extern void func_800367E8(Objects07C68Texture *texture, void *flags, s32 *frame,
                           s32 updateRate);
extern s32 D_80079008[];
extern s32 D_800790D0[];
extern f32 D_80080D24;
extern f32 D_80080D28;
extern void func_8000831C(void *arg0, void *arg1, s32 arg2, void *arg3, s32 arg4,
                          s32 arg5, s32 arg6, s32 arg7, f32 arg8, s32 arg9, s32 arg10);
typedef struct CameraScaledTransform CameraScaledTransform;
typedef struct FxGfx FxGfx;
extern void camPushModelMtx(Gfx **dlist, Mtx **mtx, CameraScaledTransform *transform,
                            f32 scale, f32 scaleY);
extern void camPopModelMtx(Gfx **dlist);
extern void func_800349A4(FxGfx **dlist, s32 texture, s32 flags, s32 arg3);
extern s32 func_800291FC(void);
extern s32 func_80034448(s16 resourceId, void *output);
extern void **func_8000572C(s32 *start, s32 *end);
extern f32 func_8000BD0C(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);
extern void partInitTrigger(ParticleTrigger *trigger, s32 type, s32 value);
extern void partInitTriggerSPPos(ParticleTrigger *trigger, s32 type, s32 value, s32 index);
extern void partInitTriggerPos(ParticleTrigger *trigger, s32 type, s32 value, s16 x, s16 y, s16 z);
extern u8 *func_80028F54(void);

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
/* Workbench verdict: schedule/register-permutation; 6 differing words (73/79). */
/* First mismatch: +0x50; size, frame, CFG, and relocation surface are exact. */
/* Structural gap: none; late scheduling and register allocation are reserved for the permuter. */
#ifdef NON_MATCHING
void *func_80004454(f32 arg0, f32 arg1, f32 arg2, u8 arg3) {
    volatile s32 frame_reserve[2];
    s32 sp7C;
    s32 sp78;
    f32 temp_f0;
    f32 var_f20;
    s32 temp_v0;
    s32 var_s1;
    s32 var_v0;
    u8 *var_s2;
    Objects04454Object *temp_s0;
    void *var_s4;

    temp_v0 = (s32)func_8000572C(&sp7C, &sp78);
    var_f20 = D_80080D24;
    var_s4 = NULL;
    if (sp7C < sp78) {
        var_s1 = sp7C * 4;
        var_s2 = (u8 *)temp_v0 + var_s1;
        do {
            temp_s0 = *(Objects04454Object **)var_s2;
            if (temp_s0->unk91 != 0) {
                var_v0 = sp78 * 4;
            } else {
                if ((temp_s0 != D_80078F20) && (arg3 == temp_s0->unk44)) {
                    temp_f0 = func_8000BD0C(temp_s0->unkC, temp_s0->unk10,
                                            temp_s0->unk14, arg0, arg1, arg2);
                    if (temp_f0 < var_f20) {
                        var_f20 = temp_f0;
                        var_s4 = temp_s0;
                    }
                }
                var_v0 = sp78 * 4;
            }
            var_s1 += 4;
            var_s2 += 4;
        } while (var_s1 < var_v0);
    }
    return var_s4;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004454.s")
#endif
/* Workbench verdict: structure-mismatch; 96 differing words (99/104). */
/* First mismatch: +0x0; target frame is 0x50, candidate frame is 0x58. */
/* Structural gap: stack homes and four-at-a-time tail control flow remain unresolved. */
#ifdef NON_MATCHING
s32 func_80004590(s32 arg0) {
    s32 sp4C;
    s32 sp48;
    s32 sp40;
    s32 temp_s0;
    s32 temp_t7;
    s32 temp_v0;
    s32 var_a2;
    s32 var_a3;
    s32 var_t1;
    s32 var_a1;
    Objects04454Object *object;
    s32 var_a1_2;

    temp_s0 = arg0 & 0xFF;
    sp40 = 0;
    temp_v0 = (s32)func_8000572C(&sp4C, &sp48);
    var_a2 = sp40;
    var_a3 = sp4C;
    if (sp4C < sp48) {
        temp_t7 = (sp48 - sp4C) & 3;
        if (temp_t7 != 0) {
            var_a1 = temp_v0 + (sp4C * 4);
            do {
                object = *(Objects04454Object **)var_a1;
                var_a3 += 1;
                if ((object->unk91 == 0) && (object != D_80078F20) &&
                    (temp_s0 == object->unk44)) {
                    var_a2 += 1;
                }
                var_a1 += 4;
            } while ((temp_t7 + sp4C) != var_a3);
            if (var_a3 != sp48) {
                goto block_9;
            }
        } else {
block_9:
            var_t1 = var_a3 * 4;
            var_a1_2 = temp_v0 + var_t1;
            do {
                object = *(Objects04454Object **)(var_a1_2 + 0);
                var_t1 += 0x10;
                if ((object->unk91 == 0) && (object != D_80078F20) &&
                    (temp_s0 == object->unk44)) {
                    var_a2 += 1;
                }
                object = *(Objects04454Object **)(var_a1_2 + 4);
                if ((object->unk91 == 0) && (object != D_80078F20) &&
                    (temp_s0 == object->unk44)) {
                    var_a2 += 1;
                }
                object = *(Objects04454Object **)(var_a1_2 + 8);
                if ((object->unk91 == 0) && (object != D_80078F20) &&
                    (temp_s0 == object->unk44)) {
                    var_a2 += 1;
                }
                object = *(Objects04454Object **)(var_a1_2 + 12);
                if ((object->unk91 == 0) && (object != D_80078F20) &&
                    (temp_s0 == object->unk44)) {
                    var_a2 += 1;
                }
                var_a1_2 += 4;
            } while (var_t1 != (sp48 * 4));
        }
    }
    return var_a2;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004590.s")
#endif
/* Workbench verdict: schedule/register-permutation; 6 differing words (74/80). */
/* First mismatch: +0x50; size, frame, CFG, and FP register shape are exact. */
/* Structural gap: none; residual register/scheduling differences are permuter-ready. */
#ifdef NON_MATCHING
void *func_8000471C(f32 arg0, f32 arg1, f32 arg2, u8 arg3) {
    volatile s32 frame_reserve[2];
    s32 sp7C;
    s32 sp78;
    f32 temp_f0;
    f32 var_f20;
    s32 temp_v0;
    s32 var_s1;
    s32 var_v0;
    u8 *var_s2;
    Objects0471CObject *temp_s0;
    void *var_s4;

    temp_v0 = (s32)func_8000572C(&sp7C, &sp78);
    var_f20 = D_80080D28;
    var_s4 = NULL;
    if (sp7C < sp78) {
        var_s1 = sp7C * 4;
        var_s2 = (u8 *)temp_v0 + var_s1;
        do {
            temp_s0 = *(Objects0471CObject **)var_s2;
            if (temp_s0->unk91 != 0) {
                var_v0 = sp78 * 4;
            } else {
                if ((temp_s0 != (Objects0471CObject *)D_80078F20) && (arg3 == temp_s0->unk40->unk1B)) {
                    temp_f0 = func_8000BD0C(temp_s0->unkC, temp_s0->unk10, temp_s0->unk14, arg0, arg1, arg2);
                    if (temp_f0 < var_f20) {
                        var_f20 = temp_f0;
                        var_s4 = temp_s0;
                    }
                }
                var_v0 = sp78 * 4;
            }
            var_s1 += 4;
            var_s2 += 4;
        } while (var_s1 < var_v0);
    }
    return var_s4;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000471C.s")
#endif
void func_8000485C(s8 arg0) {
    D_80078F88 = arg0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000486C.s")
/* Workbench verdict: register-permutation; 14 differing words (59/73). */
/* First mismatch: +0x4; size, frame, CFG, and relocation surface are exact. */
/* Structural gap: none; global-color pool allocation is reserved for the permuter. */
#ifdef NON_MATCHING
void func_80004B04(s32 arg0) {
    s32 temp_v0;
    s32 var_s1;
    s32 var_s2;
    s32 var_s3;
    u16 *temp_v1;
    u16 temp_a1;
    u16 temp_v0_2;
    Objects04B04Object *temp_s0;

    temp_v0 = arg0 * 2;
    temp_v1 = (u16 *)((u8 *)D_800C948C + temp_v0);
    temp_a1 = *temp_v1;
    if (temp_a1 != 0) {
        *temp_v1 = temp_a1 - 1;
        if (*(u16 *)((u8 *)D_800C948C + temp_v0) == 0) {
            var_s2 = 0;
            temp_s0 = D_800C9488[arg0];
            var_s3 = 0;
            var_s1 = 0;
            if (temp_s0->unkA6 > 0) {
                do {
                    temp_v0_2 = *(u16 *)(temp_s0->unkA8 + var_s3);
                    if ((temp_v0_2 & 0xC000) == 0xC000) {
                        func_800347A0(*(void **)((u8 *)temp_s0->unkAC + var_s1));
                    } else if (temp_v0_2 & 0x8000) {
                        func_800359D4(*(void **)((u8 *)temp_s0->unkAC + var_s1));
                    } else {
                        modFreeModel(*(void **)((u8 *)temp_s0->unkAC + var_s1));
                    }
                    var_s2 += 1;
                    var_s3 += 2;
                    var_s1 += 4;
                } while (var_s2 < (s32)temp_s0->unkA6);
            }
            mmFree(temp_s0);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004B04.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004C28.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004FE0.s")
/* Workbench verdict: structure-mismatch; 81 raw differing words (85/87 instructions). */
/* First mismatch: +0x18; frame is exact, but the target retains two setup instructions. */
/* Structural gap: zero-index setup and register carriers remain unresolved. */
#ifdef NON_MATCHING
void func_80005548(s32 arg0) {
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a0_2;
    s8 temp_t1;
    s8 temp_t2;
    s8 temp_t2_2;
    s8 temp_t3;
    s8 temp_t3_2;
    s8 temp_t4;
    s8 temp_t6;
    s8 temp_t6_2;
    s8 temp_t8;
    s8 temp_t9;
    void *var_v1;
    void *var_v1_2;
    void *var_v1_3;
    void *var_v1_4;

    temp_v0 = (s32)func_80028F54();
    if (D_8007BF0C != 0) {
        var_a0 = 0;
        if (arg0 > 0) {
            temp_a3 = arg0 & 3;
            if (temp_a3 != 0) {
                var_v1 = (s8 *)temp_v0 + (0 << 5);
                do {
                    temp_t8 = ((s8 *)var_v1)[5];
                    var_a0 += 1;
                    var_v1 = (u8 *)var_v1 + 0x28;
                    ((s8 *)var_v1)[-0x22] = temp_t8;
                } while (temp_a3 != var_a0);
                if (var_a0 == arg0) {
                    return;
                }
            }
            var_v1_2 = (s8 *)temp_v0 + (var_a0 * 0x28);
            do {
                temp_t2 = ((s8 *)var_v1_2)[0x2D];
                temp_t3 = ((s8 *)var_v1_2)[0x55];
                temp_t4 = ((s8 *)var_v1_2)[0x7D];
                temp_t1 = ((s8 *)var_v1_2)[5];
                var_v1_2 = (u8 *)var_v1_2 + 0xA0;
                ((s8 *)var_v1_2)[-0x72] = temp_t2;
                ((s8 *)var_v1_2)[-0x4A] = temp_t3;
                ((s8 *)var_v1_2)[-0x22] = temp_t4;
                ((s8 *)var_v1_2)[-0x9A] = temp_t1;
            } while (var_v1_2 != (void *)((s8 *)temp_v0 + (arg0 * 0x28)));
        }
    } else {
        var_a0_2 = 0;
        if (arg0 > 0) {
            temp_a3_2 = arg0 & 3;
            if (temp_a3_2 != 0) {
                var_v1_3 = (s8 *)temp_v0 + (0 << 5);
                do {
                    temp_t6 = ((s8 *)var_v1_3)[5];
                    var_a0_2 += 1;
                    var_v1_3 = (u8 *)var_v1_3 + 0x28;
                    ((s8 *)var_v1_3)[-0x22] = (arg0 - temp_t6) - 1;
                } while (temp_a3_2 != var_a0_2);
                if (var_a0_2 == arg0) {
                    return;
                }
            }
            var_v1_4 = (s8 *)temp_v0 + (var_a0_2 * 0x28);
            do {
                temp_t3_2 = (arg0 - ((s8 *)var_v1_4)[5]) - 1;
                temp_t2_2 = (arg0 - ((s8 *)var_v1_4)[0x7D]) - 1;
                temp_t9 = (arg0 - ((s8 *)var_v1_4)[0x55]) - 1;
                temp_t6_2 = (arg0 - ((s8 *)var_v1_4)[0x2D]) - 1;
                var_v1_4 = (u8 *)var_v1_4 + 0xA0;
                ((s8 *)var_v1_4)[-0x72] = temp_t6_2;
                ((s8 *)var_v1_4)[-0x4A] = temp_t9;
                ((s8 *)var_v1_4)[-0x22] = temp_t2_2;
                ((s8 *)var_v1_4)[-0x9A] = temp_t3_2;
            } while (var_v1_4 != (void *)((s8 *)temp_v0 + (arg0 * 0x28)));
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80005548.s")
#endif
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
/* PROVENANCE: control-flow body adapted from Jet Force Gemini's public
 * src/objects.c func_80007494; Mickey's offsets, globals, and calls are authoritative. */
/* Workbench verdict: allocation-mismatch; 1 differing word (85/86). */
/* First mismatch: +0x7C; size, frame, CFG, and call/relocation shape are exact. */
/* Structural gap: none; s5/v1 comparison color is reserved for the permuter. */
#ifdef NON_MATCHING
s32 func_80006868(Objects06868Object *arg0, void *arg1) {
    s32 temp_v0_2;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_s1;
    s32 var_s3;
    s8 var_v1;
    Objects06868Entry *temp_a2;
    Objects06868Data *temp_v0;
    Objects06868Entry *var_s0;

    temp_v0 = arg0->unk40;
    arg0->unk6C = arg1;
    var_v1 = temp_v0->unk25;
    var_s3 = 0;
    temp_a2 = temp_v0->unk44;
    if (var_v1 <= 0) {
        goto done;
    }
    var_s0 = temp_a2;
    var_s1 = 0;
    do {
        temp_v0_2 = var_s0->unk0;
        temp_v1 = temp_v0_2 & 0xFFFF0000;
        if (temp_v1 == 0xFFFF0000) {
            partInitTrigger((u8 *)arg0->unk6C + var_s1, (temp_v0_2 >> 8) & 0xFF,
                            temp_v0_2 & 0xFF);
        } else if (temp_v1 == 0xFFFE0000) {
            partInitTriggerSPPos((u8 *)arg0->unk6C + var_s1, (temp_v0_2 >> 8) & 0xFF,
                                 temp_v0_2 & 0xFF, var_s0->unk4 & 0xFF);
        } else {
            temp_v1_2 = var_s0->unk4;
            partInitTriggerPos((u8 *)arg0->unk6C + var_s1, (temp_v0_2 >> 24) & 0xFF,
                               (temp_v0_2 >> 16) & 0xFF, temp_v0_2 & 0xFFFF,
                               (temp_v1_2 >> 16) & 0xFFFF, temp_v1_2 & 0xFFFF);
        }
        var_s3 += 1;
        var_s1 += 0x24;
        var_v1 = arg0->unk40->unk25;
        var_s0 += 1;
    } while (var_s3 < var_v1);
done:
    return ((var_v1 * 0x24) + 3) & ~3;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006868.s")
#endif
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
/* Workbench verdict: structure-mismatch; 63 differing words (78 candidate / 79 target). */
/* First mismatch: +0x20; both outputs are frameless and the candidate is one instruction shorter. */
/* Structural gap: asset/count carrier and late loop register shape remain unresolved. */
#ifdef NON_MATCHING
s32 func_80006B04(Objects06B04Object *arg0, Objects06B04Output *arg1, volatile s32 arg2) {
    s16 temp_t0;
    s32 var_a3;
    s32 var_t2;
    s32 var_v1;
    u8 temp_a3;
    u8 temp_t4;
    Objects06B04Asset *temp_a2;
    u8 *temp_t9;
    u8 *var_a1;
    Objects06B04Output *output;

    arg0->unk48 = arg1;
    var_v1 = 0x7C;
    if (arg0->unk40->unk1E == 0) {
        temp_t9 = (u8 *)arg1 + 0x7C;
        temp_a2 = **arg0->unk68;
        output = arg1;
        temp_a3 = temp_a2->unk2F;
        if ((s32)temp_a3 > 0) {
            output->unkA = temp_a3;
            temp_t0 = output->unkA;
            output->unk74 = temp_t9;
            var_t2 = 0;
            var_v1 = (temp_t0 * 0x34) + 0x7C;
            if (temp_t0 > 0) {
                var_a1 = temp_t9;
                var_a3 = 0;
                do {
                    *(u16 *)var_a1 = 0;
                    *(u16 *)(var_a1 + 2) = 0;
                    *(u16 *)(var_a1 + 4) = 0;
                    var_t2 += 1;
                    var_a1 += 0x34;
                    *(u16 *)(var_a1 - 0x2E) = *(u16 *)(temp_a2->unk38 + var_a3);
                    *(s8 *)(var_a1 - 0x2C) = *(s8 *)(temp_a2->unk38 + var_a3 + 2);
                    *(u16 *)(var_a1 - 0x2A) = 0;
                    *(u8 *)(var_a1 - 0x2B) = *(u8 *)(temp_a2->unk38 + var_a3 + 3);
                    *(f32 *)(var_a1 - 0x10) = *(f32 *)(temp_a2->unk38 + var_a3 + 8) * arg0->unk8;
                    *(f32 *)(var_a1 - 0xC) = *(f32 *)(temp_a2->unk38 + var_a3 + 8) * arg0->unk8;
                    *(u8 *)(var_a1 - 0x6) = (u8)*(u16 *)(temp_a2->unk38 + var_a3 + 4);
                    temp_t4 = *(u8 *)(var_a1 - 0x4) | 0x80;
                    *(u8 *)(var_a1 - 0x4) = temp_t4;
                    *(u16 *)(var_a1 - 0x8) = 0;
                    *(u8 *)(var_a1 - 0x4) = temp_t4 & 0xBF;
                    *(u8 *)(var_a1 - 0x5) = (u8)*(u16 *)(temp_a2->unk38 + var_a3 + 6);
                    var_a3 += 0xC;
                } while (var_t2 < output->unkA);
            }
        }
    }
    return var_v1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006B04.s")
#endif
s32 func_80006C40(Objects06C40 *arg0, s32 arg1) {
    arg0->unk58 = arg1;
    return 0x13C;
}
/* Workbench verdict: structure-mismatch; 132 differing words (target 149, candidate 146). */
/* First mismatch: +0x0; target frame 0x50 versus candidate frame 0x40. */
/* Structural gap: allocator-result stack home and three target instructions remain. */
#ifdef NON_MATCHING
void *func_80006C4C(s32 arg0) {
    Objects06C4CAsset *asset;
    Objects06C4CObject *object;
    Objects06C4CObject *newObject;
    u8 *end;
    s32 count;
    s32 remaining;
    s32 index;
    s32 offset;
    s32 loadType;
    s32 failed;
    s32 size;
    s32 words;
    s32 *source;
    s32 *destination;
    void *result;

    if (arg0 >= D_800C945C) {
        arg0 = 0;
    }
    asset = func_8000486C(arg0);
    if (asset == NULL) {
        return NULL;
    }

    object = (Objects06C4CObject *)D_800C9450;
    end = (u8 *)object + 0x94 + (asset->unk22 * 4);
    count = D_800C9454 >> 2;
    if (count != 0) {
        remaining = count - 1;
        source = (s32 *)D_800C9450;
        do {
            *source = 0;
            source += 1;
            remaining -= 1;
        } while (remaining != 0);
    }

    object->unk40 = asset;
    object->unk6 = 2;
    object->unk2C = (s16)arg0;
    object->unk46 = (s16)arg0;
    object->unk8 = asset->unk0;
    object->unk6 = (s16)(object->unk6 | (asset->unk14 & 0x20C));
    loadType = func_8000A6E8(asset->unk1C) & 3;
    failed = 0;
    index = 0;
    offset = 0;
    if (object->unk40->unk22 > 0) {
        do {
            object->unk68 = object->unk94;
            if (func_800058C0((Objects58C0Arg *)object, index) == 0) {
                *(s32 *)((u8 *)object->unk68 + offset) =
                    (s32)func_8001F520(
                        *(s32 *)((u8 *)object->unk40->unk34 + offset), loadType);
            } else {
                *(s32 *)((u8 *)object->unk68 + offset) =
                    (s32)func_800355A0(
                        *(s32 *)((u8 *)object->unk40->unk34 + offset), 0xA);
            }
            if (*(s32 *)((u8 *)object->unk68 + offset) == 0) {
                failed = 1;
            }
            index += 1;
            offset += 4;
        } while (index < object->unk40->unk22);
    }

    result = NULL;
    if (failed == 0) {
        size = (s32)(end - (u8 *)object);
        if (size & 0xF) {
            size = (size + 0xF) & ~0xF;
        }
        result = func_8002B314(size, 0x8B);
        if (result != NULL) {
            newObject = (Objects06C4CObject *)result;
            words = size >> 2;
            source = (s32 *)D_800C9450;
            destination = (s32 *)result;
            if (words != 0) {
                remaining = words - 1;
                do {
                    *destination = *source;
                    destination += 1;
                    source += 1;
                    remaining -= 1;
                } while (remaining != 0);
            }
            newObject->unk68 = newObject->unk94;
        } else {
            failed = 1;
        }
    }
    if (failed != 0) {
        func_80006448(object);
        func_80004B04(arg0);
        result = NULL;
    }
    return result;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006C4C.s")
#endif
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
/* Workbench verdict: structure-mismatch; 63 differing words (94/94). */
/* First mismatch: +0x0C; frame and instruction count are exact. */
/* Structural gap: active-list carriers and loop register allocation remain unresolved. */
#ifdef NON_MATCHING
void func_80006FA0(void) {
    s32 *temp_v0;
    s32 temp_a0;
    s32 temp_s2;
    s32 temp_t4;
    s32 temp_t7;
    s32 var_a0;
    s32 var_s4;
    s32 var_s5;
    s32 var_s6;
    s32 var_v0;
    s32 var_v1;

    var_s5 = 0;
    if (D_800C94F0 > 0) {
        var_s6 = 0;
        do {
            temp_a0 = D_800C9498;
            var_s4 = -1;
            temp_s2 = *(s32 *)((u8 *)D_800C94EC + var_s6);
            var_v0 = 0;
            if (temp_a0 > 0) {
                do {
                    temp_t4 = temp_a0 - 1;
                    if (temp_s2 == ((s32 *)D_800C9494)[var_v0]) {
                        var_s4 = var_v0;
                        var_a0 = var_v0 * 4;
                        if (var_v0 < D_800C94B2) {
                            D_800C94B2 -= 1;
                        }
                        D_800C9498 = temp_t4;
                        var_v1 = var_v0;
                        if (var_v0 < temp_t4) {
                            do {
                                var_v1 += 1;
                                temp_v0 = (s32 *)((u8 *)D_800C9494 + var_a0);
                                temp_t7 = temp_v0[1];
                                var_a0 += 4;
                                temp_v0[0] = temp_t7;
                            } while (var_v1 < D_800C9498);
                        }
                        func_80007118(temp_s2, 0);
                        var_v0 = D_800C9498;
                    }
                    var_v0 += 1;
                } while (var_v0 < temp_a0);
            }
            if (var_s4 == -1) {
                func_80006EE4(temp_s2);
            }
            var_s5 += 1;
            var_s6 += 4;
        } while (var_s5 < D_800C94F0);
    }
    D_800C94F0 = 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006FA0.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80007118.s")
void func_80007844(void) {
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000784C.s")
/* Workbench verdict: allocation-mismatch; 42 differing words (76/118). */
/* First mismatch: +0x7C; size, frame, and opcode schedule are exact. */
/* Structural gap: none; register allocation and one stack-home constant are permuter-ready. */
#ifdef NON_MATCHING
void func_80007C68(Objects07C68Object *arg0, Objects07C68Source *arg1,
                   Objects07C68Object *arg2, s32 arg3) {
    s32 sp58;
    s16 temp_lo;
    s16 temp_v0_2;
    s32 temp_t3;
    s32 temp_v0;
    s32 var_s3;
    Objects07C68Record *var_s0;
    s16 *var_s2;
    Objects07C68Texture *texture;

    if (func_800290A0() != 0) {
        arg3 = 0;
    }
    var_s0 = arg2->unk4C;
    if (var_s0 != NULL) {
        var_s3 = 0;
        var_s2 = ((Objects07C68Indexed *)((u8 *)arg2 + (arg2->unkA * 4)))->unk50;
        if ((s32)arg1->unk2C > 0) {
            do {
                temp_v0 = var_s0->unk4;
                texture = *(Objects07C68Texture **)((u8 *)arg1->unk18 +
                                                     ((temp_v0 & 0xFF) * 8));
                if (temp_v0 & 0x100000) {
                    sp58 = (s32)var_s0->unk0;
                    if (var_s0->unk4 & 0x200000) {
                        D_8007BDA0 = arg0->unk90;
                    }
                    func_800367E8(texture, &var_s0->unk4, &sp58, arg3);
                    var_s0->unk0 = (s16)sp58;
                    if (var_s0->unk2 >= 0) {
                        temp_t3 = sp58 + 0x100;
                        sp58 = temp_t3;
                        if (temp_t3 >= (s32)texture->unk10) {
                            if (texture->unk3 & 2) {
                                sp58 = 0;
                            } else {
                                sp58 -= 0x100;
                            }
                        }
                        var_s0->unk2 = (s16)sp58;
                    }
                }
                temp_v0_2 = var_s0->unk2;
                if (temp_v0_2 >= 0) {
                    var_s2 += 1;
                    var_s2[-1] = (s16)((temp_v0_2 >> 8) * texture->unkE);
                }
                var_s3 += 1;
                temp_lo = ((s16)var_s0->unk0 >> 8) * texture->unkE;
                var_s2 += 1;
                var_s0 += 1;
                var_s2[-1] = temp_lo;
            } while (var_s3 < (s32)arg1->unk2C);
        }
        if (arg0->unk90 == 1) {
            arg0->unk90 = 2;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80007C68.s")
#endif
/* Workbench verdict: structure-mismatch; 131 differing words (target 122, candidate 131). */
/* First mismatch: +0x0; candidate frame 0x10 versus target frame 0x18. */
/* Structural gap: texture/group/record carrier spelling does not preserve target register/frame shape. */
#ifdef NON_MATCHING
void func_80007E40(Objects07E40Object *arg0, s32 arg1) {
    Objects07E40Object *object;
    Objects07E40Data *temp_v0;
    s16 temp_lo;
    s16 temp_lo_2;
    s16 temp_s3;
    s16 temp_s3_2;
    s16 temp_s3_3;
    s16 temp_s3_4;
    s16 var_s2;
    s32 var_a3_2;
    s32 var_t2;
    s32 var_t4;
    s8 var_t3;
    u8 temp_t0;
    u8 var_a3;
    u8 var_t1;
    Objects07E40Texture *temp_t4;
    Objects07E40Record *temp_t5;
    Objects07E40Inner *temp_v0_2;
    u8 *var_s1;

    object = arg0;
    temp_v0 = arg0->unk40;
    var_a3 = temp_v0->unkA3;
    temp_lo = temp_v0->unkA4 * arg1;
    temp_t0 = temp_v0->unkA2;
    temp_lo_2 = temp_v0->unkA5 * arg1;
    if ((var_a3 == 0xFF) || ((s32) var_a3 < temp_v0->unk22)) {
        var_t3 = var_a3 + 1;
        if (var_a3 == 0xFF) {
            var_a3 = 0;
            var_t3 = temp_v0->unk22;
        }
        var_t1 = var_a3;
        if ((s32) var_a3 < var_t3) {
            var_t2 = var_a3 * 4;
            do {
                var_t1 += 1;
                temp_v0_2 = (*(object->unk68 + (var_t2 >> 2)))->unk0;
                if ((s32) temp_t0 < (s32) temp_v0_2->unk10) {
                    temp_t4 = *(Objects07E40Texture **)(temp_v0_2->unk18 + (temp_t0 * 8));
                    var_a3_2 = 0;
                    var_t4 = 0;
                    if (temp_v0_2->unk16 > 0) {
                        var_s1 = temp_v0_2->unk24;
                        do {
                            var_a3_2 += 1;
                            if (temp_t0 == *(u8 *)var_s1) {
                                var_s2 = *(s16 *)(var_s1 + 8);
                                if (var_s2 < *(s16 *)(var_s1 + 0x18)) {
                                    do {
                                        temp_t5 = (Objects07E40Record *)
                                            (temp_v0_2->unk20 + (var_s2 * 0x10));
                                        if (temp_lo != 0) {
                                            temp_s3 = temp_t5->unk4;
                                            temp_t5->unk4 = (temp_s3 + temp_lo) &
                                                ((s16) (temp_t4->unk6 << 5) - 1);
                                            temp_s3_2 = temp_t5->unk4;
                                            temp_t5->unk8 = temp_s3_2 + (temp_t5->unk8 - temp_s3);
                                            temp_t5->unkC = temp_s3_2 + (temp_t5->unkC - temp_s3);
                                        }
                                        if (temp_lo_2 != 0) {
                                            temp_s3_3 = temp_t5->unk6;
                                            temp_t5->unk6 = (temp_s3_3 + temp_lo_2) &
                                                ((s16) (temp_t4->unk8 << 5) - 1);
                                            temp_s3_4 = temp_t5->unk6;
                                            temp_t5->unkA = temp_s3_4 + (temp_t5->unkA - temp_s3_3);
                                            temp_t5->unkE = temp_s3_4 + (temp_t5->unkE - temp_s3_3);
                                        }
                                        var_s2 += 1;
                                        var_s1 = temp_v0_2->unk24 + var_t4;
                                    } while (var_s2 < *(s16 *)(var_s1 + 0x18));
                                }
                            }
                            var_t4 += 0x10;
                            var_s1 += 0x10;
                        } while (var_a3_2 < temp_v0_2->unk16);
                    }
                }
                var_t2 += 4;
            } while (var_t1 != var_t3);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80007E40.s")
#endif
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
/* Workbench candidate: source-level movement and segment-bound checks. */
#ifdef NON_MATCHING
s32 func_80008128(Objects08128Object *arg0, f32 arg1, f32 arg2, f32 arg3) {
    Objects08128Track *track;
    Objects08128Bounds *bounds;
    s32 x;
    s32 y;
    s32 z;
    s32 result;

    track = trackGetTrack();
    x = (s32) (arg0->unkC + arg1);
    result = 0;
    y = (s32) (arg0->unk10 + arg2);
    z = (s32) (arg0->unk14 + arg3);
    if ((D_80079004 == 0) && (track != NULL) &&
        ((x < (track->unk20 - 1000)) ||
         (y < (track->unk24 - 1000)) ||
         (z < (track->unk28 - 1000)) ||
         ((track->unk22 + 1000) < x) ||
         ((track->unk26 + 1000) < y) ||
         ((track->unk2A + 1000) < z))) {
        result = 1;
    }
    D_80079004 = 0;
    if (result == 0) {
        arg0->unkC += arg1;
        arg0->unk10 += arg2;
        arg0->unk14 += arg3;
        if (track != NULL) {
            bounds = func_8000FEEC(arg0->unk2E);
            if ((bounds != NULL) &&
                ((x < bounds->unk0) || (y < bounds->unk2) ||
                 (z < bounds->unk4) || (bounds->unk6 < x) ||
                 (bounds->unk8 < y) || (bounds->unkA < z))) {
                bounds = NULL;
            }
            if (bounds == NULL) {
                arg0->unk2E = func_8000FAE0(arg0->unkC, arg0->unk10,
                                            arg0->unk14);
            }
        }
    }
    return result;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80008128.s")
#endif
/* Workbench verdict: structure-mismatch; 116 differing words (target 125, candidate 130). */
/* First mismatch: +0x0; candidate frame 0x30 versus target frame 0x40. */
/* Structural gap: callee-induced stack layout and late bound-check carriers differ. */
#ifdef NON_MATCHING
void func_8000831C(void *arg0, void *arg1, s32 arg2, void *arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, f32 arg8, s32 arg9, s32 arg10) {
    s32 sp24;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_t3;
    Objects0831CCommand *temp_v0;

    sp24 = 0;
    camPushModelMtx((Gfx **)&D_800C94B4, (Mtx **)&D_800C94B8,
                    (CameraScaledTransform *)arg0, arg8, 0.0f);
    if ((arg6 & 0x240) == 0) {
        temp_v0 = (Objects0831CCommand *)D_800C94B4;
        D_800C94B4 = (s32)(temp_v0 + 1);
        temp_v0->unk0 = 0xFA000000;
        temp_t3 = arg9 & 0xFF;
        temp_v0->unk4 = (temp_t3 << 24) | (temp_t3 << 16) |
                        (temp_t3 << 8) | (arg10 & 0xFF);
        temp_v0 = (Objects0831CCommand *)D_800C94B4;
        D_800C94B4 = (s32)(temp_v0 + 1);
        temp_v0->unk4 = -0x100;
        temp_v0->unk0 = 0xFB000000;
    }
    if (arg5 != 0) {
        sp24 = 1;
    }
    if (arg10 < 0xFF) {
        arg6 |= 4;
    }
    func_800349A4((FxGfx **)&D_800C94B4, arg5, arg6, arg7);
    temp_v0 = (Objects0831CCommand *)D_800C94B4;
    temp_a2 = (s32)arg1 + 0x80000000;
    D_800C94B4 = (s32)(temp_v0 + 1);
    temp_a1 = arg2 * 8;
    temp_v0->unk0 = ((((temp_a1 | (temp_a2 & 6)) & 0xFF) << 16) |
                     0x04000000 | (((arg2 * 0xA) + 8) & 0xFFFF));
    temp_v0->unk4 = temp_a2;
    temp_v0 = (Objects0831CCommand *)D_800C94B4;
    D_800C94B4 = (s32)(temp_v0 + 1);
    temp_v0->unk0 = (((((arg4 - 1) * 0x10) | sp24) & 0xFF) << 16) |
                     0x05000000 | ((arg4 * 0x10) & 0xFFFF);
    temp_v0->unk4 = (s32)arg3 + 0x80000000;
    camPopModelMtx((Gfx **)&D_800C94B4);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000831C.s")
#endif
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
/* Workbench verdict: structure-mismatch; 55 differing words (target/candidate 125/125). */
/* First mismatch: +0x1C; frame 0x70 and instruction count are exact. */
/* Shape status: control flow is complete; residuals are mostly register/constant allocation. */
#ifdef NON_MATCHING
void func_80009220(void **arg0, s32 arg1, s32 arg2, Objects09220Object *arg3,
                   s32 arg4, Objects09220Source *arg5, s32 arg6) {
    f32 direction[2];
    f32 projection;
    f32 scale;
    f32 distance;
    f32 transformedX;
    f32 transformedY;
    f32 transformedZ;
    s16 rotation[3];
    s32 alpha;
    volatile f32 negativeOne;
    volatile f32 frameZero;
    volatile f32 frameScale;
    Objects09220Matrix *matrix;
    Objects09220Gfx *command;

    if (arg3->unk40->unkA6 > 0) {
        negativeOne = -1.0f;
        mathOneFloatPY(arg3, direction);
        matrix = camGetRotationMtx();
        projection = (negativeOne * matrix->unk28) +
                     ((matrix->unk8 * direction[0]) +
                      (matrix->unk18 * direction[1]));
        if (projection > 0.0f) {
            distance = sqrtf(projection);
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            frameZero = 0.0f;
            scale = arg3->unk8 * distance * (f32) arg6 * D_80080F80;
            frameScale = scale;
            mtxf_transform_point((void *)arg4, (f32) arg5->unk26,
                                 (f32) arg5->unk28, (f32) arg5->unk2A,
                                 &transformedX, &transformedY, &transformedZ);
            alpha = (s32) (distance * 320.0f);
            if (alpha >= 0x100) {
                alpha = 0xFF;
            }
            func_80034DF0(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0);
            func_80023A08(arg0, arg1, arg2, rotation,
                          *arg3->unk40->unkAC, 4, alpha);
            func_80034E48();
            command = *(Objects09220Gfx **)arg0;
            *arg0 = (void *) (command + 1);
            command->w1 = -0x100;
            command->w0 = 0xFB000000;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009220.s")
#endif
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
/* PROVENANCE: partition loop adapted from Diddy Kong Racing's public
 * src/objects.c get_first_active_object; Mickey's list and header offsets are authoritative. */
s32 func_8000A244(s32 *arg0) {
    s32 i;
    s32 j;
    s32 minIndex;
    s32 maxIndex;
    s32 breakLoop;
    Objects0A244Object *tempObject;

    *arg0 = D_800C9498;
    if (D_800C94B2 != 0) {
        return D_800C94B2;
    }
    i = D_800C949C;
    j = D_800C9498 - 1;
    minIndex = i;
    maxIndex = j;
    while (i <= j) {
        breakLoop = 0;
        while ((i <= maxIndex) && (breakLoop == 0)) {
            if (((Objects0A244Object **)D_800C9494)[i]->unk40->unk14 & 1) {
                i += 1;
            } else {
                breakLoop = -1;
            }
        }
        breakLoop = 0;
        while ((j >= minIndex) && (breakLoop == 0)) {
            if (!(((Objects0A244Object **)D_800C9494)[j]->unk40->unk14 & 1)) {
                j -= 1;
            } else {
                breakLoop = -1;
            }
        }
        if (i < j) {
            tempObject = ((Objects0A244Object **)D_800C9494)[i];
            ((Objects0A244Object **)D_800C9494)[i] = ((Objects0A244Object **)D_800C9494)[j];
            ((Objects0A244Object **)D_800C9494)[j] = tempObject;
            i += 1;
            j -= 1;
        }
    }
    D_800C94B2 = i;
    return i;
}
/* Workbench verdict: structure-mismatch; 158 differing words (164/164 instructions). */
/* First mismatch: +0x0; candidate frame 0x60 versus target frame 0x58. */
/* Shape status: update unrolling and early-exit sort are complete; residuals are structural/register. */
#ifdef NON_MATCHING
void func_8000A39C(s32 arg0, s32 arg1) {
    Objects0A39CObject **objects;
    Objects0A39CObject *object;
    Objects0A39CObject *current;
    Objects0A39CObject *next;
    Objects0A39CMatrix *matrix;
    f32 matrixX;
    f32 matrixY;
    f32 matrixZ;
    f32 matrixW;
    f32 currentDepth;
    f32 nextDepth;
    s32 difference;
    s32 index;
    s32 updateCount;
    s32 remainder;
    s32 passCount;
    s32 swapped;

    difference = arg1 - arg0;
    if (difference > 0) {
        objects = (Objects0A39CObject **)D_800C9494;
        matrix = (Objects0A39CMatrix *)camGetRotationMtx();
        matrixX = matrix->unk8;
        matrixY = matrix->unk18;
        matrixZ = matrix->unk28;
        matrixW = matrix->unk38;

        index = arg0;
        updateCount = difference + 1;
        remainder = updateCount & 3;
        while (remainder != 0) {
            object = objects[index];
            if (object != NULL) {
                object->unk30 = -((object->unkC * matrixX) +
                                  (object->unk10 * matrixY) +
                                  (object->unk14 * matrixZ) + matrixW);
            }
            index += 1;
            remainder -= 1;
            updateCount -= 1;
        }
        while (updateCount != 0) {
            object = objects[index];
            if (object != NULL) {
                object->unk30 = -((object->unkC * matrixX) +
                                  (object->unk10 * matrixY) +
                                  (object->unk14 * matrixZ) + matrixW);
            }
            index += 1;
            updateCount -= 1;
            object = objects[index];
            if (object != NULL) {
                object->unk30 = -((object->unkC * matrixX) +
                                  (object->unk10 * matrixY) +
                                  (object->unk14 * matrixZ) + matrixW);
            }
            index += 1;
            updateCount -= 1;
            object = objects[index];
            if (object != NULL) {
                object->unk30 = -((object->unkC * matrixX) +
                                  (object->unk10 * matrixY) +
                                  (object->unk14 * matrixZ) + matrixW);
            }
            index += 1;
            updateCount -= 1;
            object = objects[index];
            if (object != NULL) {
                object->unk30 = -((object->unkC * matrixX) +
                                  (object->unk10 * matrixY) +
                                  (object->unk14 * matrixZ) + matrixW);
            }
            index += 1;
            updateCount -= 1;
        }

        passCount = difference;
        do {
            current = objects[arg0];
            currentDepth = current->unk30;
            if (current->unk6 & 0x800) {
                currentDepth += 32768.0f;
            }
            swapped = 0;
            for (index = 1; index <= passCount; index += 1) {
                next = objects[arg0 + index];
                nextDepth = next->unk30;
                if (next->unk6 & 0x800) {
                    nextDepth += 32768.0f;
                }
                if (nextDepth < currentDepth) {
                    objects[arg0 + index - 1] = next;
                    swapped = 1;
                } else {
                    objects[arg0 + index - 1] = current;
                    current = next;
                    currentDepth = nextDepth;
                }
            }
            objects[arg0 + passCount] = current;
            passCount -= 1;
        } while ((passCount != 0) && (swapped != 0));
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A39C.s")
#endif
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
/* Workbench verdict: structure-mismatch; 2 differing words (82/82). */
/* First mismatch: +0x10; switch text is instruction-exact, but linked table placement is not. */
/* Structural gap: canonical C switch table lands in anonymous .rodata rather than target late rodata. */
#ifdef NON_MATCHING
s32 func_8000A6E8(s32 arg0) {
    switch (arg0) {
        case 1:
            return 0xF01;
        case 4:
            return 0x301;
        case 3:
            return 0xB01;
        case 31:
            return 0x1101;
        case 24:
            return 0x1301;
        case 25:
            return 0x1200;
        case 26:
            return 0xB01;
        case 22:
        case 29:
        case 73:
        case 79:
            return 0x301;
        case 23:
            return 0x200;
        case 11:
        case 12:
        case 49:
            return 0x200;
        case 33:
            return 0x301;
        case 41:
            return 0x1101;
        case 53:
            return 0x200;
        case 54:
            return 0x101;
        case 55:
            return 0x101;
        case 56:
            return 0x101;
        case 57:
            return 0xB01;
        case 58:
            return 0xA00;
        case 60:
            return 0x200;
        case 63:
            return 0x101;
        case 64:
            return 0xB01;
        case 65:
            return 0x301;
        case 67:
            return 0xB01;
        case 71:
            return 0x101;
        case 72:
            return 0x301;
        case 74:
            return 0x301;
        case 75:
            return 0x101;
        case 77:
            return 0x101;
        case 78:
            return 0x301;
        case 82:
            return 0xB01;
        case 84:
            return 0x301;
        case 85:
            return 0x200;
        case 86:
            return 0x101;
        case 87:
            return 0x301;
        case 88:
            return 0x301;
        default:
            return 0;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A6E8.s")
#endif
/* Workbench verdict: allocation-mismatch; 7 differing words (130/130). */
/* First mismatch: +0x10; frame and opcode schedule are exact. */
/* Shape status: recovered switch control flow; jump-table relocation targets remain. */
#ifdef NON_MATCHING
s32 func_8000A830(Objects0A830Object *arg0, void *arg1) {
    s32 size;
    s32 type;

    type = arg0->unk40->unk1C - 1;
    switch (type) {
        case 0: size = 0x460; break;
        case 5: size = 0x18; break;
        case 19: size = 0xC; break;
        case 20: size = 0x10; break;
        case 13: size = 0x30; break;
        case 72: size = 0x54; break;
        case 28: size = 0x54; break;
        case 27: size = 0x1C; break;
        case 35: size = 0xD0; break;
        case 40: size = 0x30; break;
        case 11: size = 0xE; break;
        case 41: size = 8; break;
        case 49: size = 0xCC; break;
        case 42: size = 2; break;
        case 43: size = 4; break;
        case 32: size = 0xC; break;
        case 45: size = 6; break;
        case 46: size = 0xC; break;
        case 47: size = 0xC; break;
        case 48: size = 0xC; break;
        case 52: size = 0xC; break;
        case 53: size = 0x50; break;
        case 54: size = 0x44; break;
        case 55: size = 0; break;
        case 56: size = 0xE4; break;
        case 57: size = 0xD8; break;
        case 58: size = 6; break;
        case 59: size = 0x20; break;
        case 62: size = 0x10; break;
        case 63: size = 0x40; break;
        case 64: size = 0x24; break;
        case 65: size = 0x198; break;
        case 66: size = 0x40; break;
        case 67: size = 0x24; break;
        case 68: size = 0x14; break;
        case 69: size = 0x14; break;
        case 70: size = 0x50; break;
        case 71: size = 0x38; break;
        case 73: size = 4; break;
        case 79: size = 0x18; break;
        case 74: size = 4; break;
        case 75: size = 0x1C; break;
        case 76: size = 4; break;
        case 77: size = 0x44; break;
        case 80: size = 0xC; break;
        case 81: size = 0x18; break;
        case 82: size = 8; break;
        case 83: size = 0x38; break;
        case 84: size = 0x9C; break;
        case 85: size = 0x28; break;
        case 86: size = 8; break;
        case 87: size = 8; break;
        case 88: size = 0x14; break;
        case 89: size = 8; break;
        case 90: size = 0x238; break;
        case 91: size = 0xC0; break;
        default: size = 0; break;
    }
    if (size != 0) {
        arg0->unk64 = arg1;
    }
    return (size & -4) + 4;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A830.s")
#endif
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

/* PLATEAU-HANDOFF:func_80004B04:start
 * symbol: func_80004B04
 * score: 59/73 words
 * frame: 0x30
 * relocations: 8
 * first-mismatch: +0x4
 * summary: Shape exact; global-color pool allocation remains for the permuter.
 * PLATEAU-HANDOFF:func_80004B04:end
 */

/* PLATEAU-HANDOFF:func_80004454:start
 * symbol: func_80004454
 * score: 73/79 words
 * frame: 0x88
 * relocations: 6
 * first-mismatch: +0x50
 * summary: Shape exact; two late scheduling and four register-allocation words remain for the permuter.
 * PLATEAU-HANDOFF:func_80004454:end
 */

/* PLATEAU-HANDOFF:func_80006B04:start
 * symbol: func_80006B04
 * score: 63 differing words
 * frame: frameless
 * relocations: 0
 * first-mismatch: +0x20
 * summary: Asset/count carrier and late loop register shape remain structural; next lever is a typed source record/output layout.
 * PLATEAU-HANDOFF:func_80006B04:end
 */

/* PLATEAU-HANDOFF:func_8000471C:start
 * symbol: func_8000471C
 * score: 6 differing words
 * frame: 0x88
 * relocations: 6
 * first-mismatch: +0x50
 * summary: Shape-exact with six schedule/register residuals; ready for the bounded permuter.
 * PLATEAU-HANDOFF:func_8000471C:end
 */

/* PLATEAU-HANDOFF:func_8000A6E8:start
 * symbol: func_8000A6E8
 * score: 2 differing words
 * frame: frameless
 * relocations: 3
 * first-mismatch: +0x10
 * summary: Switch text is 82/82 instructions with exact case constants; canonical C table placement remains unresolved after the linked promotion changed the ROM checksum.
 * PLATEAU-HANDOFF:func_8000A6E8:end
 */

/* PLATEAU-HANDOFF:func_80006868:start
 * symbol: func_80006868
 * score: 1 differing words
 * frame: 0x38
 * relocations: 3
 * first-mismatch: +0x7C
 * summary: JFG-07494-derived switch/trigger initialization is 86/86 instructions with exact CFG and call surface; only bne s5/v1 versus v1/s5 allocation color remains.
 * PLATEAU-HANDOFF:func_80006868:end
 */

/* PLATEAU-HANDOFF:func_80005548:start
 * symbol: func_80005548
 * score: 81 differing words
 * frame: 0x18
 * relocations: 2
 * first-mismatch: +0x18
 * summary: Best rolled-loop candidate is 85 instructions versus 87 target; two zero-index setup instructions and register carriers remain structural.
 * PLATEAU-HANDOFF:func_80005548:end
 */

/* PLATEAU-HANDOFF:func_80006FA0:start
 * symbol: func_80006FA0
 * score: 63 differing words
 * frame: 0x40
 * relocations: 16
 * first-mismatch: +0x0C
 * summary: Active-list carriers and loop register allocation remain structural after restoring the two-argument destroy call.
 * PLATEAU-HANDOFF:func_80006FA0:end
 */

/* PLATEAU-HANDOFF:func_80004590:start
 * symbol: func_80004590
 * score: 96 differing words
 * frame: 0x58
 * relocations: 5
 * first-mismatch: +0x0
 * summary: Stack homes and four-at-a-time tail control flow remain unresolved after the m2c translation.
 * PLATEAU-HANDOFF:func_80004590:end
 */

/* PLATEAU-HANDOFF:func_8000831C:start
 * symbol: func_8000831C
 * score: 81 differing words
 * frame: 0x28
 * relocations: 13
 * first-mismatch: +0x0
 * summary: Late argument carriers and call scheduling remain structural after the display-list command translation.
 * PLATEAU-HANDOFF:func_8000831C:end
 */

/* PLATEAU-HANDOFF:func_80007C68:start
 * symbol: func_80007C68
 * score: 42 differing words
 * frame: 0x60
 * relocations: 8
 * first-mismatch: +0x7C
 * summary: Opcode shape and frame are exact; remaining register allocation and one stack-home constant are permuter-ready.
 * PLATEAU-HANDOFF:func_80007C68:end
 */
