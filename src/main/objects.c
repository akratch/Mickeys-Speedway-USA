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
    f32 unk0;
    u8 pad04[0x1A];
    s8 unk1E;
    u8 pad1F[3];
    s8 unk22;
    u8 pad23[0xB1];
    f32 unkD4;
} Objects09F74Data;

typedef struct {
    u8 pad00[8];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 pad18[0x22];
    s8 unk3A;
    u8 pad3B[5];
    Objects09F74Data *unk40;
    s16 unk44;
    u8 pad46[0x1E];
    void *unk64;
    u8 pad68[0x2B];
    s8 unk93;
} Objects09F74Object;

typedef struct {
    u8 pad00[0x4E];
    u8 unk4E;
} Objects09F74Camera;

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
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s32 unk14;
    u8 pad18[4];
    u16 unk1C;
    u16 unk1E;
    f32 unk20;
    f32 unk24;
    f32 unk28;
} Objects0B3CCConfig;

typedef struct {
    f32 unk0;
    u8 pad04[0x1A];
    u16 unk1E;
    u8 pad20[0x30];
    f32 unk50;
    u8 pad54[0x8C];
    Objects0B3CCConfig *unkE0;
} Objects0B3CCData;

typedef struct {
    s16 flags;
    s16 unk2;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    void *unk24;
} Objects0B3CCState;

typedef struct {
    u8 pad00[2];
    s16 unk2;
    s16 unk4;
    u8 pad06[6];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 pad18[4];
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    u8 pad28[0x18];
    Objects0B3CCData *unk40;
    u8 pad44[0x34];
    Objects0B3CCState *unk78;
    u8 pad7C[4];
    s32 unk80;
} Objects0B3CCObject;

extern void func_80002FE0(s32 id, f32 x, f32 y, f32 z, s32 priority,
                          void **handle);
extern void func_8000309C(void *handle, u8 volume);
extern void trackMakePolylist(s32 count, Objects0BB84Vec3 *start,
                              Objects0BB84Vec3 *end, f32 *radius, s32 arg4,
                              s32 arg5);
extern s32 func_80010900(Objects0BB84Vec3 *start, Objects0BB84Vec3 *end,
                         f32 radius, s32 actor, void *callback);
extern u32 func_8001357C(f32 arg0, f32 arg1, f32 *arg2, s32 arg3,
                         void *arg4);
extern void partUpdateTriggers(void *object, s32 updateRate);
extern void func_8000BB84(s32 arg0, Objects0BB84Vec3 *arg1,
                          Objects0BB84Vec3 *arg2, f32 arg3,
                          Objects0BB84Plane *arg4, Objects0BB84Object *arg5);
extern f32 D_8008152C;
extern f32 D_80081530;
extern f32 D_80081534;

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

extern void *func_8000486C(s32 arg0);
extern void *func_80006C4C(s32 arg0);
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
    s32 start;
    s32 end;
} Objects0486CTableEntry;

typedef struct {
    f32 unk0;
    u8 pad04[0x18];
    s16 unk1C;
    u8 pad1E[0x16];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    u8 pad48[4];
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x52];
    u8 unkA6;
    u8 padA7;
    s32 unkA8;
    s32 unkAC;
    s32 unkB0;
    u8 padB4[0x2C];
    s32 unkE0;
} Objects0486CAsset;

typedef struct {
    u16 unk0;
    s8 unk2;
    u8 unk3;
    s16 unk4;
    u8 pad06[2];
    s32 unk8;
} Objects06534Record;

typedef struct {
    u8 pad00[0x23];
    s8 unk23;
    s8 unk24;
    u8 pad25[0x13];
    s32 *unk38;
    s32 unk3C;
    Objects06534Record *unk40;
} Objects06534Data;

typedef struct {
    u8 pad00[8];
    f32 unk8;
    u8 pad0C[0x34];
    Objects06534Data *unk40;
    u8 pad44[0x18];
    void *unk5C;
    void *unk60;
    u8 pad64[0x28];
    s8 unk8C;
} Objects06534Object;

typedef struct {
    void *unk0;
    s8 unk4;
    u8 unk5;
    u8 pad06[2];
    f32 unk8;
    s32 unkC;
    f32 unk10;
} Objects06534Sprite;

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
extern void *camGetProjOrgMtx(void);
extern void mathOneFloatPY(void *, f32 *);
extern void mtxf_transform_point(void *, f32, f32, f32, f32 *, f32 *, f32 *);
extern s32 func_800246B0(f32, f32, f32, f32 *, f32 *, u8);
extern void func_80034DF0(u8, u8, u8, u8, u8, u8);
extern void func_80034E48(void);
extern void func_80023598(void **, void *, void *, void *, void *, s32);
extern void func_80023A08(void **, s32, s32, s16 *, s32, s32, s32);
extern f32 sqrtf(f32);
extern f32 D_80080F80;
extern f32 D_80080F7C;

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
extern u16 D_8007BF1C;
extern void *D_800C94A0;
extern Objects04B04Object **D_800C9488;
extern u16 *D_800C948C;
extern s32 *D_800C94A4;
extern s32 D_800C94B4;
extern s32 D_800C94B8;
extern s32 D_800C94BC;
extern s32 D_8007A210;
extern void *D_8007A214;
extern void *D_8007A218;
extern s32 D_8007A21C;
extern s16 D_800C9508[];
extern s16 D_800C94B0;
extern s16 D_800C94B2;
extern s32 D_80078F84;
extern Objects04454Object *D_80078F20;
extern s8 D_80078F88;
extern s8 D_80079004;
extern u8 D_8007BDA0;
extern u8 D_8007BEF8;
extern u8 D_8007BF0C;
extern s8 D_80079250;
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
extern s32 D_800C94D0[];
extern s32 D_800C94D4[];
extern s32 D_800C9478;
extern f32 D_800C946C;
extern s32 D_80078F80;
extern s32 D_80078F78;
extern s32 D_800C94E8;
extern u8 D_800D3128[];
extern void **D_800C94EC;
extern s32 D_800C94F0;
extern void **D_800C9500;
extern s32 D_800C9504;
extern s32 D_8007A1F4;
extern s32 D_8007A1F8;
extern s32 D_8007A1FC;
extern u8 D_8007BEFC;
extern u8 D_8007BF04;
extern u8 D_8007BF10;
extern void *D_80078F7C;
extern s16 D_80078F8C[];
extern s16 D_80078FA0[];
extern s16 D_80078FB4[];
extern s16 D_80078FC8[];
extern s16 D_80078FDC[];
extern s16 D_80078FF0[];
extern s32 D_800C947C;
extern s32 D_800C9480;
extern s32 D_800C9484;
extern s32 piRomLoadSection(u32 assetIndex, u32 address, s32 assetOffset, s32 size);
extern s32 *piRomLoad(s32 assetIndex);
extern s32 runlinkDownloadCode(s32 overlayIndex);
extern void *func_8000590C();
extern f32 sqrtf(f32 value);
extern void func_80006FA0(void);
extern void func_80007118();
extern s32 TrapDanglingJump();
extern void mmFree(void *data);
extern void modFreeModel(void *resource);
extern void func_800347A0(void *texture);
extern void func_800359D4(void *sprite);
extern s32 func_8000A6E8(s32 arg0);
extern void *func_8002B280(s32 size, s32 tag);
extern void *func_8002B4C0(void *slots, s32 size);
extern s32 mathRnd(s32 minimum, s32 maximum);
extern void func_80009F74(void *object);
extern u8 *levelGetLevel(void);
extern s32 levelGetNumber(void);
extern s32 controlGetPlayerSetup(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3);
extern void func_80058250(void);
extern s32 camGetNo(void);
extern Objects09F74Camera *camGetPtr(void);
extern f32 camGetProjZ(f32 x, f32 y, f32 z);
extern u8 *levelGetColourCycling(void);
extern void func_80008B94(void *object);
typedef struct Objects09AA8Object Objects09AA8Object;
extern void func_80009AA8(Objects09AA8Object *object);
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
extern void camRestoreModelMtx(Gfx **dlist);
extern void func_80034920(Gfx **dlist);
extern void func_800349A4(FxGfx **dlist, s32 texture, s32 flags, s32 arg3);
extern s32 func_800291FC(void);
extern s32 func_80034448(s32 resourceId, void *output);
extern void func_8005AF14(void *arg0, void *arg1, void *arg2);
extern void func_80019AB8(void *arg0, void *arg1, s32 arg2, s32 arg3);
extern void func_80007C68(Objects07C68Object *arg0, Objects07C68Source *arg1,
                          Objects07C68Object *arg2, s32 arg3);
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
/* Workbench verdict: structure-mismatch; 146 differing words (target 166, candidate 168). */
/* First mismatch: +0x0; target frame 0x48 versus candidate frame 0x58. */
/* Structural gap: cache-index stack home and two extra call-path instructions remain. */
#ifdef NON_MATCHING
void *func_8000486C(s32 arg0) {
    Objects0486CTableEntry *tableEntry;
    Objects0486CAsset *asset;
    u16 refValue;
    u16 descriptor;
    s32 assetStart;
    s32 assetSize;
    s32 relativeE0;
    s32 index;
    s32 descriptorOffset;
    s32 resourceOffset;

    refValue = D_800C948C[arg0];
    if (refValue != 0) {
        D_800C948C[arg0] = refValue + 1;
        return ((Objects0486CAsset **)D_800C9488)[arg0];
    }

    tableEntry = &((Objects0486CTableEntry *)D_800C9458)[arg0];
    assetStart = tableEntry->start;
    assetSize = tableEntry->end - assetStart;
    asset = (Objects0486CAsset *)func_8002B4C0(D_800C94A0, assetSize);
    if ((asset == NULL) && (D_80078F88 != 0)) {
        asset = (Objects0486CAsset *)func_8002B280(assetSize, 0x8B);
    }
    if (asset == NULL) {
        return NULL;
    }

    piRomLoadSection(0x2D, (u32)asset, assetStart, assetSize);
    relativeE0 = asset->unkE0;
    asset->unk4C = (s32)((u8 *)asset + asset->unk4C);
    asset->unk50 = (s32)((u8 *)asset + asset->unk50);
    asset->unk44 = (s32)((u8 *)asset + asset->unk44);
    asset->unkB0 = (s32)((u8 *)asset + asset->unkB0);
    asset->unk38 = (s32)((u8 *)asset + asset->unk38);
    asset->unk3C = (s32)((u8 *)asset + asset->unk3C);
    asset->unk40 = (s32)((u8 *)asset + asset->unk40);
    asset->unk34 = (s32)((u8 *)asset + asset->unk34);
    asset->unkA8 = (s32)((u8 *)asset + asset->unkA8);
    asset->unkAC = (s32)((u8 *)asset + asset->unkAC);
    if (relativeE0 != 0) {
        asset->unkE0 = (s32)((u8 *)asset + relativeE0);
    }

    index = 0;
    descriptorOffset = 0;
    resourceOffset = 0;
    if (asset->unkA6 > 0) {
        do {
        descriptor = *(u16 *)((u8 *)asset->unkA8 + descriptorOffset);
        if ((descriptor & 0xC000) == 0xC000) {
            *(void **)((u8 *)asset->unkAC + resourceOffset) =
                (void *)func_80034448(descriptor & 0x3FFF, 0);
        } else if (descriptor & 0x8000) {
            *(void **)((u8 *)asset->unkAC + resourceOffset) =
                func_800355A0(descriptor & 0x3FFF, 1);
        } else {
            *(void **)((u8 *)asset->unkAC + resourceOffset) =
                func_8001F520(descriptor, 0);
        }
        index += 1;
        descriptorOffset += 2;
        resourceOffset += 4;
        } while (index < asset->unkA6);
    }

    if ((asset->unk1C == 1) && (D_8007BF1C & 8) &&
        (*func_80028F54() != 1)) {
        asset->unk0 *= 0.5f;
    }
    ((Objects0486CAsset **)D_800C9488)[arg0] = asset;
    D_800C948C[arg0] = 1;
    return asset;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000486C.s")
#endif
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
/* Workbench verdict: structure-mismatch; 234 differing words (target 238, candidate 242). */
/* First mismatch: +0x0; target frame 0x48 versus candidate frame 0x68. */
/* Structural gap: ROM table carrier setup adds four instructions and 0x20 bytes of stack. */
#ifdef NON_MATCHING
void func_80004C28(s32 arg0, s32 arg1) {
    void *heap;
    s32 *romTable;
    s32 *table;
    s16 **currentSlot;
    s32 *sizeSlot;
    s32 *idSlot;
    void **heapSlot;
    s16 *current;
    s32 start;
    s32 size;
    s32 tableCount;
    s32 offset;
    s32 indexOffset;
    s32 shouldCall;
    s16 type;
    u8 recordSize;

    heap = func_8002B280(0x3000, 0x8B);
    indexOffset = arg1 * 4;
    heapSlot = (void **)((u8 *)D_800C94D8 + indexOffset);
    currentSlot = (s16 **)((u8 *)D_800C94C0 + indexOffset);
    sizeSlot = (s32 *)((u8 *)D_800C94C8 + indexOffset);
    idSlot = (s32 *)((u8 *)D_800C94D0 + indexOffset);
    *heapSlot = heap;
    *currentSlot = (s16 *)((u8 *)heap + 0x10);
    *sizeSlot = 0;
    *idSlot = arg0;

    romTable = piRomLoad(0x1C);
    table = romTable;
    tableCount = 0;
    if (*table != -1) {
        do {
            tableCount += 1;
            table += 1;
        } while (table[1] != -1);
    }
    tableCount -= 1;
    if (arg0 >= tableCount) {
        arg0 = 0;
    }
    table = &romTable[arg0];
    start = table[0];
    size = table[1] - start;
    mmFree(romTable);
    if (size == 0) {
        return;
    }

    piRomLoadSection(0x1D, (u32)heap, start, size);
    *currentSlot = (s16 *)((u8 *)*heapSlot + 0x10);
    *sizeSlot = *(s32 *)heap;
    D_800C94E8 = arg1;
    if (idSlot == &D_800C94D4[0]) {
        D_80078F78 = 1;
        runlinkDownloadCode(8);
        runlinkDownloadCode(1);
        TrapDanglingJump();
        D_80078F78 = 0;
    }

    offset = 0;
    if (*sizeSlot > 0) {
        do {
            current = *currentSlot;
            type = *current;
            shouldCall = 1;
            if (D_8007BF0C != 0) {
                switch (type) {
                case 0x1B:
                    *current = 0xEA;
                    break;
                case 0x86:
                    *current = 0xEB;
                    break;
                case 0x02:
                case 0x16:
                case 0x17:
                case 0x76:
                case 0x78:
                case 0x7D:
                case 0x104:
                case 0x105:
                case 0x107:
                case 0x12C:
                    if ((s32)D_8007BEF8 >= 3) {
                        shouldCall = 0;
                    }
                    break;
                case 0x106:
                case 0x131:
                case 0x132:
                case 0x147:
                    shouldCall = 0;
                    break;
                }
            }
            current = *currentSlot;
            type = *current;
            if ((type == 0x3F) && ((D_8007BF1C & 0x20) != 0)) {
                shouldCall = 0;
            }
            if ((type == 1) || (type == 0x155) || (type == 0x156)) {
                if (D_800D3128[0x12] >= 0x15) {
                    *current = 0x156;
                } else if ((*(u16 *)(D_800D3128 + 0x14) & 0x40) != 0) {
                    *current = 0x155;
                } else {
                    *current = 1;
                }
                current = *currentSlot;
            }
            if (shouldCall != 0) {
                func_8000590C(current, 1, offset, 0x155);
                current = *currentSlot;
            }
            recordSize = *((u8 *)current + 2);
            *currentSlot = (s16 *)((u8 *)current + recordSize);
            offset += recordSize;
        } while (offset < *sizeSlot);
    }
    *currentSlot = (s16 *)((u8 *)*heapSlot + 0x10);
    if (D_800C9478 == 0) {
        func_80006FA0();
    }
    D_800C9478 = 1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004C28.s")
#endif
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    u8 pad0A[2];
    s16 unkC;
    s16 unkE;
    u8 unk10;
    u8 unk11;
} Objects04FE0Packet;

typedef struct {
    u8 pad00[4];
    u8 unk4;
    u8 pad05;
    s8 unk6;
    u8 pad07[0x21];
} Objects04FE0ModeRecord;

typedef struct {
    u8 pad00[4];
    s16 unk4;
    s16 unk6;
    s16 unk8;
} Objects04FE0Source;

typedef struct {
    s16 unk0;
    u8 pad02[0x3A];
    void *unk3C;
    u8 pad40[4];
    s16 unk44;
    u8 pad46[0x3E];
    s32 unk84;
    s32 unk88;
} Objects04FE0Object;

/* Workbench verdict: structure-mismatch; 337 differing words (347/346 instructions). */
/* First mismatch: +0x64; target frame is 0x100, candidate frame is 0x128. */
/* Structural gap: packet carriers and allocator call ABI leave a larger candidate frame. */
#ifdef NON_MATCHING
void func_80004FE0(s32 arg0) {
    u8 *modeState;
    u8 *level;
    s32 playerCount;
    s32 i;
    s32 offset;
    s32 slot;
    s32 player;
    s16 playerSetup0;
    s16 playerSetup1;
    s16 playerSetup2;
    s16 playerSetup3;
    Objects04FE0Object *category[6];
    Objects04FE0Packet packets[6];
    Objects04FE0Packet extraPacket;
    Objects04FE0Packet specialPacket;
    Objects04FE0ModeRecord *records;
    Objects04FE0Object **objects;
    Objects04FE0Object *object;
    Objects04FE0Object *sourceObject;
    Objects04FE0Source *source;
    s32 flags;
    u8 type;

    modeState = func_80028F54();
    level = levelGetLevel();
    playerCount = func_800291FC();
    D_800C94F8 = 0;
    if ((level[0x83] != 1) && (level[0x83] != 2)) {
        for (i = 0; i < 6; i++) {
            category[i] = NULL;
        }
        objects = (Objects04FE0Object **)D_800C9494;
        for (i = 0; i < D_800C9498; i++) {
            object = objects[i];
            if ((object->unk44 == 5) && (arg0 == object->unk88)) {
                slot = object->unk84;
                if ((slot >= 0) && (slot < 6)) {
                    if (category[slot] == NULL) {
                        category[slot] = object;
                    }
                } else {
                    category[0] = object;
                    category[2] = object;
                    category[3] = object;
                    category[4] = object;
                    category[5] = object;
                }
            }
        }
        records = (Objects04FE0ModeRecord *)modeState;
        for (player = 0; player < playerCount; player++) {
            type = records[player].unk4;
            if (type >= 0xA) {
                type = 0;
            }
            if (D_8007BF10 != 0) {
                packets[player].unk0 = D_80078FF0[type];
            } else if (D_8007BF0C == 0) {
                if (level[0x83] == 3) {
                    packets[player].unk0 = D_80078FB4[type];
                } else if (D_8007BF04 != 0) {
                    packets[player].unk0 = D_80078FA0[type];
                } else {
                    packets[player].unk0 = D_80078F8C[type];
                }
            } else if (level[0x83] == 3) {
                packets[player].unk0 = D_80078FDC[type];
            } else {
                packets[player].unk0 = D_80078FC8[type];
            }
            packets[player].unk2 = 0x12;
            packets[player].unkC = 0;
            packets[player].unk10 = player;
            packets[player].unk11 = type;
            if ((*modeState == 5) || (*modeState == 6)) {
                slot = player;
            } else if ((*modeState == 1) || (*modeState == 2)) {
                slot = 0;
            } else if (D_8007BF0C != 0) {
                slot = (playerCount - records[player].unk6) - 1;
            } else {
                slot = records[player].unk6;
            }
            sourceObject = category[slot];
            if (sourceObject != NULL) {
                source = (Objects04FE0Source *)sourceObject->unk3C;
                packets[player].unk4 = source->unk4;
                packets[player].unk6 = source->unk6;
                packets[player].unk8 = source->unk8;
                packets[player].unkE = sourceObject->unk0;
                category[slot] = NULL;
            } else {
                packets[player].unk4 = 0;
                packets[player].unk6 = 0;
                packets[player].unk8 = 0;
                packets[player].unkE = 0;
            }
        }
        controlGetPlayerSetup(&playerSetup0, &playerSetup1, &playerSetup2,
                              &playerSetup3);
        for (i = 0; i < 8; i++) {
            D_800C94F4[i] = NULL;
        }
        for (player = 0, offset = 0; player < playerCount;
             player++, offset += 4) {
            object = (Objects04FE0Object *)func_8000590C(&packets[player], 1,
                                                         offset);
            D_800C94F4[player] = object;
            if (object != NULL) {
                object->unk3C = NULL;
            }
        }
        D_800C94F8 = playerCount;
        for (i = 0; i < playerCount - D_8007BEFC; i++) {
            extraPacket.unk0 = 0x71;
            extraPacket.unk4 = 0;
            extraPacket.unk6 = 0;
            extraPacket.unk8 = 0;
            extraPacket.unk10 = i;
            object = (Objects04FE0Object *)func_8000590C(&extraPacket, 1);
            if (object != NULL) {
                object->unk3C = NULL;
            }
        }
        if (D_8007BF0C == 0) {
            specialPacket.unk0 = 0x70;
        } else {
            specialPacket.unk0 = 0x77;
        }
        specialPacket.unk2 = 0xC;
        specialPacket.unk4 = playerSetup0;
        specialPacket.unk6 = playerSetup1;
        specialPacket.unk8 = playerSetup2;
        specialPacket.unkC = playerSetup3;
        object = (Objects04FE0Object *)func_8000590C(&specialPacket, 1);
        if (object != NULL) {
            object->unk3C = NULL;
            if (D_8007BF0C != 0) {
                D_80078F7C = object;
            }
        }
        if ((*modeState == 1) && (D_800C94F4[0] != NULL)) {
            flags = *(s32 *)D_800D3128;
            if ((flags & 0x80000) != 0) {
                TrapDanglingJump(D_8007A1F4);
            }
            if ((flags & 0x100000) != 0) {
                TrapDanglingJump(levelGetNumber());
                TrapDanglingJump(D_8007A1F8);
            }
            D_8007A1FC = TrapDanglingJump(levelGetNumber());
            TrapDanglingJump(); TrapDanglingJump(D_800C94F4[0]);
        }
        D_800C947C = 1;
        D_800C9480 = 0x80;
        D_800C9484 = 0;
        D_80078F80 = 0;
    }
    runlinkDownloadCode(8);
    runlinkDownloadCode(1);
    TrapDanglingJump((s8)level[0x83]);
    func_80058250();
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004FE0.s")
#endif
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
typedef struct {
    s16 unk0;
    u8 pad02[2];
    s16 unk4;
    s16 unk6;
    s16 unk8;
} Objects0590CPacket;

typedef struct {
    f32 unk0;
    u8 pad04[0x10];
    u16 unk14;
    u8 pad16[2];
    s16 unk18;
    s8 unk1A;
    u8 pad1B[1];
    s16 unk1C;
    u8 pad1E[4];
    s8 unk22;
    s8 unk23;
    s8 unk24;
    s8 unk25;
    u8 pad26[2];
    s8 unk28;
    u8 unk29;
    u8 pad2A[6];
    u8 unk30;
    u8 pad31[3];
    s32 *unk34;
    u8 pad38[0x29];
    u8 unk61;
    u8 pad62[0x10];
    s8 unk72;
    u8 pad73[0x34];
    u8 unkA7;
    u8 padA8[0xC];
    s16 unkB4;
    s16 unkB6;
    s16 unkB8;
    u8 unkBA;
    u8 unkBB;
    f32 unkBC;
    f32 unkC0;
    f32 unkC4;
    f32 unkC8;
    s32 unkCC;
    u8 padD0[0x10];
    s32 unkE0;
} Objects0590CAsset;

typedef struct {
    u8 pad00[0x30];
    s32 unk30;
} Objects0590CParticleList;

typedef struct Objects0AA38Object Objects0AA38Object;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 pad18[0x14];
    s16 unk2C;
    s16 unk2E;
    u8 pad30[4];
    f32 unk34;
    u8 pad38[1];
    u8 unk39;
    u8 pad3A[2];
    void *unk3C;
    Objects0590CAsset *unk40;
    u8 pad44[2];
    s16 unk46;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 *unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    s32 unk7C;
    u8 pad80[0xD];
    u8 unk8D;
    u8 pad8E[3];
    u8 unk91;
    u8 unk92;
    u8 unk93;
    s32 unk94[1];
} Objects0590CObject;

extern u8 *align4(u8 *address);
extern void lightSetupLightSources(void *object);
extern void lightSetupFlareSources(void *object);
extern s32 func_8001A008(void *object, void *state);
extern void modelSetModelFlags(s32 flags);
extern s32 func_80048760(void *object, s32 state);
extern void func_80053550(void *source, s32 kind, s32 mode, s16 rotationX,
                           s16 rotationY, s16 rotationZ, f32 radius, f32 height,
                           f32 arg8, f32 arg9, s32 collisionType, u16 flags);
extern void func_8000AA38(Objects0AA38Object *object, void *arg1);
extern void func_80006448(void *object);
extern s32 func_80006534(Objects06534Object *object);
extern s32 func_80006868(Objects06868Object *object, void *data);
extern s32 func_800069C0(Objects69C0In *object, Objects69C0Out *data);
extern s32 func_800069E8(Objects069E8Object *object, Objects069E8Target *data);
extern s32 func_80006B04(Objects06B04Object *object, Objects06B04Output *data,
                          volatile s32 arg2);
extern s32 func_80006C40(Objects06C40 *object, s32 data);
extern s32 func_8000A830(Objects0A830Object *object, void *data);

/* Workbench verdict: structure-mismatch; 691 differing words (target 719, candidate 692). */
/* First mismatch: +0x0; target frame 0x90, candidate frame 0xA8. */
/* Structural gap: typed staging/relocation spelling and CFE register carriers remain. */
#ifdef NON_MATCHING
void *func_8000590C(void *arg0, s32 arg1) {
    Objects0590CObject *object;
    Objects0590CObject *newObject;
    u8 *cursor;
    u8 *aligned;
    u8 *base;
    u8 *relocated;
    s32 index;
    s32 offset;
    s32 count;
    s32 loadFlags;
    s32 loadType;
    s16 selected;
    s32 size;
    s32 words;
    s32 copied;
    s32 tail;
    s32 copyOffset;
    s32 copyEnd;
    s32 resultSize;
    void *result;
    s8 failed;

    D_8007A210 = 1;
    D_8007A214 = NULL;
    D_8007A218 = NULL;
    D_8007A21C = 1;

    if (arg1 & 2) {
        selected = ((Objects0590CPacket *)arg0)->unk0;
    } else {
        selected = D_800C94E0[((Objects0590CPacket *)arg0)->unk0];
    }
    if ((selected < 0) || (selected >= D_800C945C)) {
        D_8007A21C = 3;
        return NULL;
    }

    count = D_800C9454 >> 2;
    offset = 0;
    index = 0;
    if (count > 0) {
        do {
            index += 1;
            *(s32 *)((u8 *)D_800C9450 + offset) = 0;
            offset += 4;
        } while (index < (D_800C9454 >> 2));
    }
    object = (Objects0590CObject *)D_800C9450;
    D_8007A214 = object;
    object->unk6 = 2;
    object->unk40 = (Objects0590CAsset *)func_8000486C(selected);
    if (object->unk40 == NULL) {
        D_8007A21C = 2;
        return NULL;
    }
    D_8007A218 = (u8 *)object->unk40 + 4;
    object->unk6 = (s16)(object->unk6 | (object->unk40->unk14 & 0x124C));
    object->unkC = (f32)((Objects0590CPacket *)arg0)->unk4;
    object->unk10 = (f32)((Objects0590CPacket *)arg0)->unk6;
    object->unk14 = (f32)((Objects0590CPacket *)arg0)->unk8;
    object->unk2E = func_8000FAE0(object->unkC, object->unk10, object->unk14);
    object->unk2C = selected;
    object->unk46 = ((Objects0590CPacket *)arg0)->unk0;
    object->unk3C = arg0;
    object->unk8 = object->unk40->unk0;
    object->unk39 = 0xFF;
    object->unk34 = (f32)object->unk40->unk18 * object->unk8;
    object->unk91 = 0;
    object->unk93 = 0;
    object->unk8D = object->unk40->unkA7;
    loadFlags = func_8000A6E8(object->unk40->unk1C);
    loadType = loadFlags & 3;
    object->unk40->unk1A += 1;
    object->unk68 = (s32 *)((u8 *)object + 0x94);
    failed = 0;

    for (index = 0; index < object->unk40->unk22; index++) {
        s8 type = func_800058C0((Objects58C0Arg *)object, index);
        offset = index * 4;
        if (type == 2) {
            object->unk68[index] = (s32)func_80034448(object->unk40->unk34[index], NULL);
            if (object->unk68[index] == 0) {
                failed = 1;
            }
        } else if (type == 0) {
            if (object->unk40->unk30 != 0) {
                modelSetModelFlags(0);
            }
            object->unk68[index] = (s32)func_8001F520(object->unk40->unk34[index], loadType);
            if (object->unk68[index] == 0) {
                failed = 1;
            }
            modelSetModelFlags(8);
        } else if (type == 1) {
            object->unk68[index] = (s32)func_800355A0(object->unk40->unk34[index], 0xA);
            if (object->unk68[index] == 0) {
                failed = 1;
            }
        } else {
            object->unk68 = NULL;
        }
    }
    if (failed != 0) {
        func_80006448(object);
        func_80004B04(selected);
        D_8007A21C = 2;
        return NULL;
    }

    cursor = (u8 *)object->unk68 + (object->unk40->unk22 * 4);
    cursor += func_8000A830((Objects0A830Object *)object, cursor);
    D_800C9490 = 0;
    if (loadFlags & 0x100) {
        cursor += func_8001A008(object, cursor);
        object->unk50 = (s32)((u8 *)object->unk50);
        *(u8 *)((u8 *)object->unk50 + 0xC) = *(u8 *)((u8 *)object->unk50 + 0x25);
        *(u8 *)((u8 *)object->unk50 + 0xD) = *(u8 *)((u8 *)object->unk50 + 0x27);
    }
    resultSize = loadFlags & 0x200;
    if ((resultSize != 0) && (object->unk40->unk61 != 0)) {
        resultSize = func_800069E8((Objects069E8Object *)object,
                                   (Objects069E8Target *)cursor);
        cursor += resultSize;
        if (resultSize == 0) {
            func_80006448(object);
            func_80004B04(selected);
            D_8007A21C = 2;
            return NULL;
        }
    }
    if ((loadFlags & 0x400) && (object->unk40->unk72 != -1)) {
        resultSize = func_80048760(object, (s32)cursor);
        cursor += resultSize;
        if (resultSize == 0) {
            if (D_800C9490 != 0) {
                func_800347A0(D_800C9490);
            }
            func_80006448(object);
            func_80004B04(selected);
            D_8007A21C = 2;
            return NULL;
        }
    }
    if (loadFlags & 0x800) {
        cursor += func_80006B04((Objects06B04Object *)object,
                                (Objects06B04Output *)cursor,
                                ((Objects0590CPacket *)arg0)->unk0);
    }
    if (loadFlags & 0x1000) {
        cursor += func_80006C40((Objects06C40 *)object, (s32)cursor);
    }
    if ((object->unk40->unk23 > 0) && (object->unk40->unk23 < 0xA)) {
        object->unk5C = (s32)cursor;
        aligned = align4(cursor + 0x34);
        ((Objects0590CParticleList *)cursor)->unk30 = (s32)aligned;
        cursor = aligned + (object->unk40->unk23 * 0x40);
    }
    if ((object->unk40->unk24 > 0) && (object->unk40->unk24 < 0xA)) {
        aligned = align4(cursor);
        object->unk60 = (s32)aligned;
        cursor = aligned + (object->unk40->unk24 * 0x14);
    }
    if (object->unk40->unk25 > 0) {
        cursor += func_80006868((Objects06868Object *)object, cursor);
    }
    if (object->unk40->unk28 > 0) {
        object->unk70 = (s32)cursor;
        cursor += object->unk40->unk28 * 4;
    }
    if (object->unk40->unk29 > 0) {
        object->unk74 = (s32)cursor;
        cursor += object->unk40->unk29 * 4;
    }
    if (object->unk40->unkE0 != 0) {
        cursor += func_800069C0((Objects69C0In *)object,
                                (Objects69C0Out *)cursor);
    }
    if (object->unk8D != 0) {
        aligned = align4(cursor);
        object->unk7C = (s32)aligned;
        cursor = aligned + (object->unk40->unkA7 * 0x24);
    }

    size = (s32)(cursor - (u8 *)object);
    if (size & 0xF) {
        size = (size & ~0xF) + 0x10;
    }
    result = func_8002B4C0(D_800C94A0, size);
    if ((result == NULL) && (D_80078F88 != 0)) {
        result = func_8002B280(size, 0x8B);
    }
    if (result == NULL) {
        if (D_800C9490 != 0) {
            func_800347A0(D_800C9490);
        }
        func_80006448(object);
        func_80004B04(selected);
        D_8007A21C = 2;
        return NULL;
    }

    newObject = (Objects0590CObject *)result;
    words = size >> 2;
    copied = 0;
    if (words > 0) {
        tail = words & 3;
        if (tail != 0) {
            copyOffset = 0;
            do {
                copied += 1;
                copyOffset += 4;
                *(s32 *)((u8 *)newObject + copyOffset - 4) =
                    *(s32 *)((u8 *)D_800C9450 + copyOffset - 4);
            } while (copied != tail);
        }
        if (copied != words) {
            copyOffset = copied * 4;
            copyEnd = words * 4;
            do {
                *(s32 *)((u8 *)newObject + copyOffset) =
                    *(s32 *)((u8 *)D_800C9450 + copyOffset);
                *(s32 *)((u8 *)newObject + copyOffset + 4) =
                    *(s32 *)((u8 *)D_800C9450 + copyOffset + 4);
                *(s32 *)((u8 *)newObject + copyOffset + 8) =
                    *(s32 *)((u8 *)D_800C9450 + copyOffset + 8);
                *(s32 *)((u8 *)newObject + copyOffset + 0xC) =
                    *(s32 *)((u8 *)D_800C9450 + copyOffset + 0xC);
                copyOffset += 0x10;
            } while (copyOffset != copyEnd);
        }
    }
    base = (u8 *)D_800C9450;
    if (newObject->unk54 != 0) {
        newObject->unk54 = (s32)(((u8 *)newObject + newObject->unk54) - base);
    }
    if (newObject->unk4C != 0) {
        relocated = ((u8 *)newObject + newObject->unk4C) - base;
        newObject->unk4C = (s32)relocated;
        if (*(s32 *)(relocated + 0x1C) != 0) {
            *(s32 *)(relocated + 0x1C) =
                (s32)(((u8 *)newObject + *(s32 *)(relocated + 0x1C)) - base);
        }
    }
    if (newObject->unk50 != 0) {
        newObject->unk50 = (s32)(((u8 *)newObject + newObject->unk50) - base);
    }
    if (newObject->unk64 != 0) {
        newObject->unk64 = (s32)(((u8 *)newObject + newObject->unk64) - base);
    }
    if (newObject->unk48 != 0) {
        relocated = ((u8 *)newObject + newObject->unk48) - base;
        newObject->unk48 = (s32)relocated;
        if (*(s32 *)(relocated + 0x74) != 0) {
            *(s32 *)(relocated + 0x74) =
                (s32)(((u8 *)newObject + *(s32 *)(relocated + 0x74)) - base);
        }
    }
    if (newObject->unk58 != 0) {
        newObject->unk58 = (s32)(((u8 *)newObject + newObject->unk58) - base);
    }
    if (newObject->unk5C != 0) {
        newObject->unk5C = (s32)(((u8 *)newObject + newObject->unk5C) - base);
    }
    if (newObject->unk60 != 0) {
        newObject->unk60 = (s32)(((u8 *)newObject + newObject->unk60) - base);
    }
    if (newObject->unk7C != 0) {
        newObject->unk7C = (s32)(((u8 *)newObject + newObject->unk7C) - base);
    }
    if (newObject->unk6C != 0) {
        newObject->unk6C = (s32)(((u8 *)newObject + newObject->unk6C) - base);
    }
    if (newObject->unk40->unk28 > 0) {
        newObject->unk70 = (s32)(((u8 *)newObject + newObject->unk70) - base);
    }
    if (newObject->unk40->unk29 > 0) {
        newObject->unk74 = (s32)(((u8 *)newObject + newObject->unk74) - base);
    }
    if (newObject->unk40->unkE0 != 0) {
        newObject->unk78 = (s32)(((u8 *)newObject + newObject->unk78) - base);
    }
    newObject->unk68 = (s32 *)((u8 *)newObject + 0x94);
    if (arg1 & 1) {
        D_800C9494[D_800C9498] = newObject;
        D_800C9498 += 1;
    } else {
        D_800C94A4[D_800C94A8] = (s32)newObject;
        D_800C94A8 += 1;
    }
    if (newObject->unk40->unk28 > 0) {
        lightSetupLightSources(newObject);
    }
    if (newObject->unk40->unk29 > 0) {
        lightSetupFlareSources(newObject);
    }
    func_8000AA38((Objects0AA38Object *)newObject, arg0);
    if (newObject->unk58 != 0) {
        TrapDanglingJump(newObject);
    }
    if (newObject->unk48 != 0) {
        func_80053550(newObject, newObject->unk40->unkBA, newObject->unk40->unkBB,
                      newObject->unk40->unkB4, newObject->unk40->unkB6,
                      newObject->unk40->unkB8, newObject->unk40->unkBC,
                      newObject->unk40->unkC0, newObject->unk40->unkC4,
                      newObject->unk40->unkC8, newObject->unk40->unkCC, 1);
    }
    if (func_80006534((Objects06534Object *)newObject) != 0) {
        if (D_800C9490 != 0) {
            func_800347A0(D_800C9490);
        }
        func_80006448(newObject);
        func_80004B04(selected);
        mmFree(newObject);
        if (arg1 & 1) {
            D_800C9498 -= 1;
        } else {
            D_800C94A8 -= 1;
        }
        D_8007A21C = 2;
        return NULL;
    }
    if ((loadFlags & 0x200) && (newObject->unk40->unk61 != 0)) {
        relocated = (u8 *)(s32)newObject->unk4C;
        if ((*(s32 *)(relocated + 0x1C) != 0) &&
            ((*(u8 *)(relocated + 0x10) & 8) != 0)) {
            TrapDanglingJump(newObject);
        }
    }
    D_8007A21C = 4;
    return newObject;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000590C.s")
#endif
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
/* Workbench verdict: structure-mismatch; 198 differing words (target 205, candidate 207). */
/* First mismatch: +0x0; target frame 0x38 versus candidate 0x48. */
/* Structural gap: resource-list carrier and cleanup control flow are complete; stack shape differs. */
#ifdef NON_MATCHING
s32 func_80006534(Objects06534Object *object) {
    Objects06534Record *record;
    Objects06534Sprite *sprite;
    void *list;
    void *loaded;
    s32 failed;
    s32 count;
    s32 index;

    list = NULL;
    sprite = NULL;
    failed = 0;
    if ((object->unk40->unk23 > 0) && (object->unk40->unk23 < 0xA)) {
        list = object->unk5C;
        count = object->unk40->unk23;
        *(s32 *)list = count;
        for (index = 0; index < count; index++) {
            loaded = func_80006C4C(object->unk40->unk38[index]);
            if (loaded != NULL) {
                *(f32 *)((u8 *)loaded + 8) *= object->unk8;
            }
            *(void **)((u8 *)list + (index * 4) + 4) = loaded;
            if (loaded == NULL) {
                failed = 1;
            }
        }
        *(s32 *)((u8 *)list + 0x2C) = object->unk40->unk3C;
    }

    object->unk8C = object->unk40->unk24;
    count = (u8)object->unk40->unk24;
    if ((count > 0) && (object->unk40->unk24 < 0xA)) {
        sprite = (Objects06534Sprite *)object->unk60;
        record = object->unk40->unk40;
        for (index = 0; index < count; index++, sprite++, record++) {
            sprite->unk0 = func_800355A0(record->unk0, 1);
            sprite->unk4 = record->unk2;
            sprite->unkC = record->unk8;
            sprite->unk8 = ((f32)record->unk4 / 500.0f) * object->unk8;
            sprite->unk5 = record->unk3;
            if (record->unk8 & 0x80000000) {
                sprite->unk10 = (f32)mathRnd(0, *(u8 *)sprite->unk0);
            } else if (record->unk8 & 0x40000000) {
                sprite->unk10 = ((Objects06534Sprite *)object->unk60)[(record->unk8 >> 22) & 0x3F].unk10 +
                                (f32)((record->unk8 >> 16) & 0x3F);
            } else if (record->unk8 & 0x20000000) {
                sprite->unk10 = ((Objects06534Sprite *)object->unk60)[(record->unk8 >> 22) & 0x3F].unk10 -
                                (f32)((record->unk8 >> 16) & 0x3F);
            }
            if ((f32)*(u8 *)sprite->unk0 < sprite->unk10) {
                sprite->unk10 -= (f32)*(u8 *)sprite->unk0;
            } else if (sprite->unk10 < 0.0f) {
                sprite->unk10 += (f32)*(u8 *)sprite->unk0;
            }
            if (sprite->unk0 == NULL) {
                failed = 1;
            }
        }
    }

    if (failed != 0) {
        if (list != NULL) {
            count = *(s32 *)list;
            for (index = 0; index < count; index++) {
                loaded = *(void **)((u8 *)list + (index * 4) + 4);
                if (loaded != NULL) {
                    func_80006448(loaded);
                    func_80004B04(*(s16 *)((u8 *)loaded + 0x2C));
                    mmFree(loaded);
                }
            }
            *(s32 *)((u8 *)list + 0x2C) = 0;
        }
        if (sprite != NULL) {
            sprite = (Objects06534Sprite *)object->unk60;
            for (index = 0; index < (u8)object->unk8C; index++) {
                loaded = sprite->unk0;
                if (loaded != NULL) {
                    func_800359D4(loaded);
                    sprite->unk0 = NULL;
                }
            }
        }
    }
    return failed;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006534.s")
#endif
/* PROVENANCE: control-flow body adapted from Jet Force Gemini's public
 * src/objects.c func_80007494; Mickey's offsets, globals, and calls are authoritative. */
/* Workbench verdict: allocation-mismatch; 1 differing word (85/86). */
/* First mismatch: +0x7C; size, frame, CFG, and call/relocation shape are exact. */
/* Structural gap: none; s5/v1 comparison color is reserved for the permuter. */
s32 func_80006868(Objects06868Object *arg0, void *arg1)
{
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
  if (var_v1 <= 0)
  {
    goto done;
  }
  var_s0 = temp_a2;
  var_s1 = 0;
  do
  {
    temp_v0_2 = var_s0->unk0;
    temp_v1 = temp_v0_2 & 0xFFFF0000;
    if (temp_v1 == 0xFFFF0000)
    {
      partInitTrigger(((u8 *) arg0->unk6C) + var_s1, (temp_v0_2 >> 8) & 0xFF, temp_v0_2 & 0xFF);
    }
    else
      if ((temp_v0_2 & 0xFFFF0000) == 0xFFFE0000)
    {
      partInitTriggerSPPos(((u8 *) arg0->unk6C) + var_s1, (temp_v0_2 >> 8) & 0xFF, temp_v0_2 & 0xFF, var_s0->unk4 & 0xFF);
    }
    else
    {
      temp_v1_2 = var_s0->unk4;
      partInitTriggerPos(((u8 *) arg0->unk6C) + var_s1, (temp_v0_2 >> 24) & 0xFF, (temp_v0_2 >> 16) & 0xFF, temp_v0_2 & 0xFFFF, (temp_v1_2 >> 16) & 0xFFFF, temp_v1_2 & 0xFFFF);
    }
    var_s3 += 1;
    var_s1 += 0x24;
    var_v1 = arg0->unk40->unk25;
    var_s0 += 1;
  }
  while (var_s3 < var_v1);
  done:
  return ((var_v1 * 0x24) + 3) & (~3);

}
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
extern void func_800031E8(void *object);
extern void func_80005798(void *object);
extern void func_8000D728(void *object);
extern void camlightDelete(void *object);
extern void partObjFreeTriggers(void *object);
extern void partNullifyCircularParticleParents(void *object);
extern void lightKillGlowingLight(void);
extern void func_80048980(void *object);
extern void func_8001C088(void *object);
extern void killLight(void *light);
extern void amSndStop(void *sound);
extern void func_80046E70(void *object);

/* Workbench candidate: object/resource teardown with the complete type switch. */
#ifdef NON_MATCHING
void func_80007118(void *arg0, s32 arg1) {
    u8 *object;
    u8 *data;
    u8 *payload;
    u8 *freePayload;
    u8 *resource;
    u8 *entry;
    s32 *list;
    s32 count;
    s32 i;
    s32 type;
    void *value;

    object = (u8 *)arg0;
    data = *(u8 **)(object + 0x40);
    type = *(s16 *)(object + 0x44);
    payload = *(u8 **)(object + 0x64);
    freePayload = payload;
    if (type == 1) {
        value = *(void **)(payload + 0xD0);
        if (value != NULL && *(u8 *)((u8 *)value + 0x91) == 0) {
            freePayload = (u8 *)(s32)func_80006EE4((s32)value);
        }
        value = *(void **)(freePayload + 0xD4);
        if (value != NULL && *(u8 *)((u8 *)value + 0x91) == 0) {
            func_80006EE4((s32)value);
        }
    }
    D_8007A210 = 2;
    D_8007A21C = 1;
    D_8007A214 = object;
    D_8007A218 = (s32)(data + 4);

    list = *(s32 **)(object + 0x5C);
    if (list != NULL) {
        count = list[0];
        for (i = 0; i < count; i++) {
            entry = *(u8 **)((u8 *)list + 4 + (i * 4));
            func_80006448(entry);
            func_80004B04(*(s16 *)(entry + 0x2C));
            mmFree(entry);
        }
    }
    value = *(void **)(object + 0x60);
    if (value != NULL) {
        count = *(u8 *)(object + 0x8C);
        for (i = 0; i < count; i++) {
            func_800359D4(*(void **)((u8 *)value + (i * 0x14)));
        }
    }
    value = *(void **)(object + 0x70);
    if (value != NULL) {
        count = *(s8 *)(data + 0x28);
        for (i = 0; i < count; i++) {
            killLight(*(void **)((u8 *)value + (i * 4)));
        }
    }
    value = *(void **)(object + 0x74);
    if (value != NULL) {
        count = *(u8 *)(data + 0x29);
        for (i = 0; i < count; i++) {
            camlightDelete(*(void **)((u8 *)value + (i * 4)));
        }
    }
    partObjFreeTriggers(object);

    switch (type) {
        case 1:
            TrapDanglingJump(*(void **)(payload + 0x64));
            break;
        case 2:
            func_800359D4(*(void **)(payload + 0x20));
            break;
        case 3:
            func_800031E8(*(void **)(payload + 0x3C));
            break;
        case 4:
        case 5:
            TrapDanglingJump();
            break;
        case 6:
            func_800031E8(*(void **)(payload + 0x3C));
            break;
        case 7:
            value = *(void **)(payload + 0x20);
            if (value != NULL) {
                u8 *owner = *(u8 **)(*(void **)((u8 *)value + 0x64));
                if (*(void **)(owner + 0xD4) == object) {
                    *(void **)(owner + 0xD4) = NULL;
                }
            }
            break;
        case 8:
            value = *(void **)(payload + 0x0);
            if (value != NULL) {
                u8 *owner = *(u8 **)(*(void **)((u8 *)value + 0x64));
                if (*(void **)(owner + 0xD0) == object) {
                    *(void **)(owner + 0xD0) = NULL;
                }
            }
            func_800031E8(*(void **)(payload + 0x14));
            break;
        case 9:
            value = *(void **)(payload + 0x10);
            if (value != NULL) {
                u8 *owner = *(u8 **)(*(void **)((u8 *)value + 0x64));
                if (*(void **)(owner + 0xD8) == object) {
                    *(void **)(owner + 0xD8) = NULL;
                }
            }
            break;
        case 10:
            amSndStop(*(void **)(payload + 0x0));
            break;
        case 11:
            func_800031E8(*(void **)(payload + 0x3C));
            break;
        case 12:
            func_800031E8(*(void **)(payload + 0x18));
            func_800031E8(*(void **)(payload + 0x1C));
            func_800031E8(*(void **)(payload + 0x40));
            break;
        case 13:
            func_800031E8(*(void **)(payload + 0x38));
            break;
        case 14:
            D_80078F7C = NULL;
            break;
        case 15:
            value = (void *)D_8007A1F4;
            if (value != NULL && *(void **)value == object) {
                *(void **)value = NULL;
            }
            break;
        case 16:
        case 17:
            TrapDanglingJump(object);
            break;
        case 18:
            func_8001C088(object);
            break;
        case 19:
            killLight(*(void **)(payload + 0x64));
            break;
        case 20:
            value = *(void **)(payload + 0x20);
            if (value != NULL) {
                killLight(value);
            }
            func_8000D728(*(void **)(payload + 0x2C));
            break;
        case 21:
            camlightDelete(*(void **)(object + 0x84));
            break;
        case 22:
            lightKillGlowingLight();
            break;
        case 23:
            mmFree(*(void **)(payload + 4));
            break;
        case 24:
            func_80005798(object);
            value = *(void **)(object + 0x84);
            if (value != NULL) {
                func_800031E8(value);
            }
            break;
        case 25:
            func_80005798(object);
            break;
        case 26:
            value = *(void **)(object + 0x84);
            if (value != NULL) {
                func_800031E8(value);
            }
            break;
        case 27:
            func_800031E8(*(void **)(payload + 0xA4));
            func_800031E8(*(void **)(payload + 0xA8));
            func_800031E8(*(void **)(payload + 0xAC));
            func_800031E8(*(void **)(payload + 0xB0));
            func_800031E8(*(void **)(payload + 0xB4));
            func_800031E8(*(void **)(payload + 0xB8));
            func_800031E8(*(void **)(payload + 0xBC));
            func_800031E8(*(void **)(payload + 0xC0));
            func_800031E8(*(void **)(payload + 0xC4));
            break;
        default:
            break;
    }

    resource = *(u8 **)(object + 0x4C);
    if (resource != NULL) {
        if (*(void **)(resource + 0x1C) != NULL && ((*(u8 *)(data + 0x61) & 8) != 0)) {
            TrapDanglingJump();
        }
        func_800347A0(*(void **)(resource + 8));
    }
    value = *(void **)(object + 0x54);
    if (value != NULL) {
        func_80048980(value);
    }
    value = *(void **)(object + 0x78);
    if (value != NULL) {
        func_800031E8(*(void **)((u8 *)value + 0x24));
    }
    if (*(u8 *)(object + 0x92) != 0) {
        partNullifyCircularParticleParents(object);
    }
    func_80006448(object);
    func_80004B04(*(s16 *)(object + 0x2C));
    mmFree(object);
    D_8007A21C = 4;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80007118.s")
#endif
void func_80007844(void) {
}
typedef struct {
    u8 pad00[0x1E];
    s8 unk1E;
    u8 pad1F[0x82];
    u8 unkA1[1];
    u8 unkA2;
} Objects0784CData;

typedef struct {
    u8 pad00[0x38];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0xB2];
    s16 unkF6;
    s16 unkF8;
    s16 unkFA;
} Objects0784COutput;

typedef struct {
    u8 pad00[8];
    void *unk8;
    u16 unkC;
    u16 unkE;
} Objects0784CAnimation;

typedef struct {
    void *unk0;
    u8 pad04[1];
    u8 unk5;
    u8 pad06[6];
    s32 unkC;
    u8 pad10[4];
} Objects0784CEffect;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u8 pad06[6];
    f32 unkC;
    f32 unk10;
    f32 unk14;
    u8 pad18[0x28];
    Objects0784CData *unk40;
    s16 unk44;
    u8 pad46[6];
    Objects0784CAnimation *unk4C;
    u8 pad50[4];
    void *unk54;
    u8 pad58[8];
    Objects0784CEffect *unk60;
    Objects0784COutput *unk64;
    u8 pad68[0x24];
    u8 unk8C;
} Objects0784CObject;

extern s32 runlinkIsModuleLoaded(s32 module);
extern void func_8000AEEC(void *object, s32 updateRate);
extern void func_8000B3CC(void *object, s32 updateRate);
extern void spranimOnceControl(void *object, s32 updateRate);
extern void spranimControl(void *object, s32 updateRate);
extern void texscrollControl(void *object, s32 updateRate);
extern void effectboxControl(void *object, s32 updateRate);
extern void func_800148E0(void *object);
extern void func_8001B798(void *object, s32 updateRate);
extern void func_8001BB04(void *object, s32 updateRate);
extern void func_8001BB10(void *object, s32 updateRate);
extern void rangetriggerControl(void *object, s32 updateRate);
extern void func_80007E40(Objects07E40Object *object, s32 updateRate);
extern void func_80049000(void *object, s32 updateRate);
extern void func_80036544(void *entry, s32 *mode, s32 animationId, void *state,
                          s32 updateRate);
extern void func_8001CB84(void *object, s32 updateRate);
extern void func_8001D2A0(void *object, s32 updateRate);
extern void func_80053868(s32 updateRate);
extern void lightUpdateLights(s32 updateRate);
extern void lightUpdateObjects(void);
extern void amPlayAudioMap(void **objects, s32 count, s32 updateRate);

/* Workbench verdict: structure-mismatch; 273 differing words (277/263). */
/* First mismatch: +0x0; target frame 0x100 versus candidate frame 0x108. */
/* Structural gap: object-list carrier, stack-home placement, and late-loop register shape remain unresolved. */
#ifdef NON_MATCHING
void func_8000784C(s32 arg0) {
    void *pending[0x20];
    Objects0784CObject *object;
    Objects0784CObject ** volatile *objectList;
    s32 *objectCount;
    Objects0784CData *data;
    Objects0784CAnimation *animation;
    Objects0784CEffect *effect;
    Objects0784CEffect *effectEnd;
    s32 i;
    s32 objectOffset;
    s32 objectEnd;
    s32 pendingCount;
    s32 timer;

    objectList = (Objects0784CObject ** volatile *)&D_800C94F4;
    objectCount = &D_800C94F8;
    i = 0;
    if (*objectCount > 0) {
        objectOffset = 0;
        do {
            object = *(Objects0784CObject **)((u8 *)*objectList + objectOffset);
            object->unk64->unk38 = object->unkC;
            object->unk64->unk3C =
                (*(Objects0784CObject **)((u8 *)*objectList + objectOffset))->unk10;
            object->unk64->unk40 =
                (*(Objects0784CObject **)((u8 *)*objectList + objectOffset))->unk14;
            object->unk64->unkF6 =
                (*(Objects0784CObject **)((u8 *)*objectList + objectOffset))->unk0;
            object->unk64->unkF8 =
                (*(Objects0784CObject **)((u8 *)*objectList + objectOffset))->unk2;
            object->unk64->unkFA =
                (*(Objects0784CObject **)((u8 *)*objectList + objectOffset))->unk4;
            i += 1;
            objectOffset += 4;
        } while (i < *objectCount);
    }

    objectList = (Objects0784CObject ** volatile *)&D_800C94F4;

    if (runlinkIsModuleLoaded(0x14) != 0) {
        TrapDanglingJump(arg0);
    }

    pendingCount = 0;
    if (D_800C949C < D_800C9498) {
        objectOffset = D_800C949C * 4;
        objectEnd = D_800C9498 * 4;
        do {
            object = *(Objects0784CObject **)((u8 *)D_800C9494 + objectOffset);
            if ((object->unk44 == 7) || (object->unk44 == 0x1C) ||
                (object->unk44 == 0x50) || (object->unk44 == 0x42)) {
                if (pendingCount < 0x20) {
                    pending[pendingCount] = object;
                    pendingCount += 1;
                }
            } else if (object->unk44 != 0xF) {
                func_8000AEEC(object, arg0);
                data = object->unk40;
                if ((data->unk1E == 0) && (data->unkA2 != 0xFF)) {
                    func_80007E40((Objects07E40Object *)object, arg0);
                }
                if ((object->unk44 != 1) && (object->unk54 != NULL)) {
                    func_80049000(object, arg0);
                }
                animation = (Objects0784CAnimation *)object->unk4C;
                if (animation != NULL) {
                    if (animation->unkE != 0) {
                        if (animation->unk8 != NULL) {
                            timer = animation->unkC + (animation->unkE * arg0);
                            animation->unkC = timer;
                            if ((timer & 0xFFFF) >=
                                ((u16 *)animation->unk8)[8]) {
                                do {
                                    timer = (timer & 0xFFFF) -
                                        ((u16 *)animation->unk8)[8];
                                    animation->unkC = timer;
                                } while ((timer & 0xFFFF) >=
                                         ((u16 *)animation->unk8)[8]);
                            }
                        }
                    }
                }
                effect = (Objects0784CEffect *)object->unk60;
                if (effect != NULL) {
                    i = 0;
                    effectEnd = effect + object->unk8C;
                    while (effect < effectEnd) {
                        func_80036544(effect->unk0, &effect->unkC,
                                      effect->unk5, &effect->pad10,
                                      arg0);
                        i += 1;
                        effect += 1;
                    }
                }
            }
            objectOffset += 4;
        } while (objectOffset < objectEnd);
    }

    timer = D_80078F80 - arg0;
    if (D_80078F80 != 0) {
        D_80078F80 = timer;
        if (timer < 0) {
            D_80078F80 = 0;
        }
    }

    i = 0;
    objectOffset = 0;
    if (D_800C94F8 > 0) {
        do {
            func_8001CB84((void *)(*objectList)[objectOffset >> 2], arg0);
            i += 1;
            objectOffset += 4;
        } while (i < *objectCount);
    }
    func_80053868(arg0);
    i = 0;
    objectOffset = 0;
    if (D_800C94F8 > 0) {
        do {
            func_8001D2A0((void *)(*objectList)[objectOffset >> 2], arg0);
            i += 1;
            objectOffset += 4;
        } while (i < *objectCount);
    }
    objectOffset = 0;
    while (objectOffset < pendingCount) {
        func_8000AEEC(pending[objectOffset], arg0);
        objectOffset += 1;
    }
    lightUpdateLights(arg0);
    lightUpdateObjects();
    amPlayAudioMap(D_800C94F4, D_800C94F8, arg0);
    if (*func_80028F54() == 1) {
        TrapDanglingJump(arg0);
    }
    if (D_800C9478 == 0) {
        func_80006FA0();
    }
    D_800C9478 = 1;
    D_800C946C = (f32)arg0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000784C.s")
#endif
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
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Objects084C4Point;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} Objects084C4Vertex;

typedef struct {
    u32 w0;
    u32 w1;
} Objects084C4Gfx;

/* Workbench verdict: structure-mismatch; 314 differing words (348/343 instructions). */
/* First mismatch: +0x10; target frame is 0xB0, candidate frame is 0x98. */
/* Structural gap: matrix/vertex carriers and late display-list command scheduling differ. */
#ifdef NON_MATCHING
void func_800084C4(Objects084C4Gfx **arg0, Objects084C4Vertex **arg1,
                   s32 arg2, s32 arg3, Objects084C4Point *arg4,
                   Objects084C4Point *arg5, f32 arg6, s32 arg7, s32 arg8,
                   s32 arg9) {
    f32 pointBX;
    f32 pointBY;
    f32 pointBZ;
    f32 pointAX;
    f32 pointAY;
    f32 pointAZ;
    f32 clippedBX;
    f32 clippedBY;
    f32 clippedBZ;
    f32 clippedAX;
    f32 clippedAY;
    f32 clippedAZ;
    f32 projectedBX;
    f32 projectedBY;
    f32 projectedAX;
    f32 projectedAY;
    f32 deltaX;
    f32 deltaY;
    f32 deltaLengthSquared;
    f32 scale;
    void *rotationMatrix;
    Objects084C4Gfx *displayList;
    Objects084C4Vertex *vertices;
    Objects084C4Vertex *output;
    u32 segmentedVertices;
    Objects084C4Gfx *command;

    if ((arg4->x == arg5->x) && (arg4->y == arg5->y) &&
        (arg4->z == arg5->z)) {
        return;
    }
    rotationMatrix = camGetRotationMtx();
    mtxf_transform_point(rotationMatrix, arg5->x, arg5->y, arg5->z,
                         &pointBX, &pointBY, &pointBZ);
    mtxf_transform_point(rotationMatrix, arg4->x, arg4->y, arg4->z,
                         &pointAX, &pointAY, &pointAZ);
    clippedBX = pointBX;
    clippedBY = pointBY;
    clippedBZ = pointBZ;
    clippedAX = pointAX;
    clippedAY = pointAY;
    clippedAZ = pointAZ;
    if ((pointBZ > -10.0f) && (pointAZ > -10.0f)) {
        return;
    }
    if (pointBZ > -10.0f) {
        clippedBZ = -10.0f;
        scale = (-10.0f - pointBZ) / (pointAZ - pointBZ);
        clippedBX = pointBX + ((pointAX - pointBX) * scale);
        clippedBY = pointBY + ((pointAY - pointBY) * scale);
    } else if (pointAZ > -10.0f) {
        clippedAZ = -10.0f;
        scale = (-10.0f - pointAZ) / (pointBZ - pointAZ);
        clippedAX = pointAX + ((pointBX - pointAX) * scale);
        clippedAY = pointAY + ((pointBY - pointAY) * scale);
    }
    displayList = *arg0;
    vertices = *arg1;
    output = vertices;
    if (func_800246B0(clippedBX, clippedBY, clippedBZ,
                      &projectedBX, &projectedBY, 0) != 0) {
        if (func_800246B0(clippedAX, clippedAY, clippedAZ,
                          &projectedAX, &projectedAY, 0) != 0) {
            deltaX = projectedAX - projectedBX;
            deltaY = projectedAY - projectedBY;
            deltaLengthSquared = (deltaX * deltaX) + (deltaY * deltaY);
            if (deltaLengthSquared > 0.0f) {
                scale = arg6 / sqrtf(deltaLengthSquared);
                deltaX *= scale;
                deltaY *= scale;
            }
            command = displayList++;
            command->w0 = 0x01010040;
            command->w1 = (u32)camGetProjOrgMtx() + 0x80000000;
            func_800349A4((FxGfx **)&displayList, arg2, arg9 | 6, 0);
            command = displayList++;
            command->w0 = 0xFA000000;
            command->w1 = arg7;
            command = displayList++;
            command->w0 = 0xFB000000;
            command->w1 = arg8;
            segmentedVertices = (u32)vertices + 0x80000000;
            command = displayList++;
            command->w0 = 0x04000030 |
                          (((segmentedVertices & 6) | 0x20) << 16);
            command->w1 = segmentedVertices;
            command = displayList++;
            command->w0 = 0x05110020;
            command->w1 = (u32)arg3 + 0x80000000;
            command = displayList++;
            command->w0 = 0xE7000000;
            command->w1 = 0;
            command = displayList++;
            command->w0 = 0xFA000000;
            command->w1 = (u32)-1;
            command = displayList++;
            command->w0 = 0xFB000000;
            command->w1 = (u32)-1;
            command = displayList++;
            command->w0 = 0xBC00000A;
            command->w1 = 0;
            output[0].x = (s16)(pointBX + deltaY);
            output[0].y = (s16)(pointBY + deltaX);
            output[0].z = (s16)pointBZ;
            output[0].r = 0xFF;
            output[0].g = 0xFF;
            output[0].b = 0xFF;
            output[0].a = 0xFF;
            output[1].x = (s16)(pointBX - deltaY);
            output[1].y = (s16)(pointBY - deltaX);
            output[1].z = (s16)pointBZ;
            output[1].r = 0xFF;
            output[1].g = 0xFF;
            output[1].b = 0xFF;
            output[1].a = 0xFF;
            output[2].x = (s16)(pointAX + deltaY);
            output[2].y = (s16)(pointAY + deltaX);
            output[2].z = (s16)pointAZ;
            output[2].r = 0xFF;
            output[2].g = 0xFF;
            output[2].b = 0xFF;
            output[2].a = 0xFF;
            output[3].x = (s16)(pointAX - deltaY);
            output[3].y = (s16)(pointAY - deltaX);
            output[3].z = (s16)pointAZ;
            output[3].r = 0xFF;
            output[3].g = 0xFF;
            output[3].b = 0xFF;
            output[3].a = 0xFF;
            output += 4;
        }
    }
    *arg0 = displayList;
    *arg1 = output;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_800084C4.s")
#endif
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

typedef struct {
    u8 pad00[0x1E];
    u8 unk1E;
    u8 pad1F[0x11];
    u8 unk30;
} Objects08B94Data;

typedef struct {
    u8 pad00[6];
    s16 unk6;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 unkD;
} Objects08B94Resource;

typedef struct {
    f32 unk0;
    u8 pad04;
    u8 unk5;
    u8 unk6;
    u8 unk7;
} Objects08B94Multiplier;

typedef struct {
    u8 pad00[0xD];
    u8 unkD;
} Objects08B94Palette;

typedef struct {
    u8 pad00[8];
    u8 r;
    u8 g;
    u8 b;
    u8 pad0B[5];
} Objects08B94Colour;

typedef struct {
    u8 pad00[0x64];
    u8 *unk64;
} Objects08B94Camera;

typedef struct {
    u8 pad00[4];
    u8 unk4;
    u8 unk5;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u8 pad0A[0xA];
    Objects08B94Camera *unk14;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
} Objects08B94Info;

typedef struct {
    u8 pad00[6];
    s16 unk6;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s32 unk14;
    u8 pad18[0x21];
    u8 unk39;
    s8 unk3A;
    u8 pad3B;
    Objects08B94Palette *unk3C;
    Objects08B94Data *unk40;
    s16 unk44;
    u8 pad46[0xA];
    Objects08B94Multiplier *unk50;
    u8 pad54[0x10];
    Objects08B94Info *unk64;
    Objects08B94Resource **unk68;
} Objects08B94Object;

/* The target uses the low byte of the computed alpha stack home here. */
#ifdef NON_MATCHING
void func_80008B94(void *arg0) {
    Objects08B94Object *object;
    Objects08B94Resource *resource;
    Objects08B94Multiplier *multiplier;
    Objects08B94Colour *colourA;
    Objects08B94Colour *colourB;
    Objects0831CCommand *command;
    Objects08B94Camera *camera;
    s32 flags;
    s32 alpha;
    s32 colourState;
    s32 useColourState;
    s32 useMultiplier;
    s32 specialColour;
    s32 savedAlpha;
    s32 computedAlpha;
    s32 extraGreen;
    s32 extraBlue;
    f32 savedScale;
    volatile s32 frame_reserve[6];

    object = (Objects08B94Object *)arg0;
    if (object->unk44 == 0x45) {
        TrapDanglingJump(&D_800C94B4, &D_800C94B8, &D_800C94BC, object);
        return;
    }

    flags = object->unk6 & 6;
    if (object->unk40->unk30 == 0) {
        flags |= 8;
    }
    if (D_8007C854 != 0) {
        colourState = D_8007C85C;
        useColourState = colourState != 0xFF;
    } else {
        colourState = 0xFF;
        useColourState = 0;
    }
    useMultiplier = 0;
    if (object->unk8 < 0.0f) {
        flags |= 0x8000;
        object->unk8 = -object->unk8;
    }

    multiplier = object->unk50;
    if (multiplier != NULL) {
        useColourState = 1;
        useMultiplier = 1;
        colourState = (s32)((f32)colourState * multiplier->unk0);
    }
    alpha = object->unk39;
    if (object->unk44 == 0x50) {
        savedAlpha = alpha;
        camera = object->unk64->unk14;
        if (camGetNo() == (s32)(s8)camera->unk64[0]) {
            alpha >>= 1;
        }
    }
    if (alpha < 0xFF) {
        useColourState = 1;
        flags |= 4;
    }

    resource = object->unk68[(u8)object->unk3A];
    specialColour = 0;
    if (object->unk44 == 0x44) {
        func_80034DF0(object->unk64->unk4, object->unk64->unk5,
                      object->unk64->unk6, object->unk64->unk7,
                      object->unk64->unk8, object->unk64->unk9);
        specialColour = 1;
    } else if (object->unk44 == 0x3C) {
        func_80034DF0(0xFF, 0xFF, 0xFF, object->unk64->unk18,
                      object->unk64->unk19, object->unk64->unk1A);
        specialColour = 1;
    } else if (object->unk44 == 0x20 && object->unk3C != NULL &&
               (object->unk3C->unkD & 0x80) != 0) {
        Objects08B94Colour *colours;
        u8 paletteIndex;

        colours = (Objects08B94Colour *)levelGetColourCycling();
        paletteIndex = object->unk3C->unkD;
        colourA = colours + (paletteIndex & 7);
        colourB = colours + ((paletteIndex >> 3) & 7);
        func_80034DF0(colourA->r, colourA->g, colourA->b,
                      colourB->r, colourB->g, colourB->b);
        specialColour = 1;
    } else if (multiplier != NULL && resource != NULL &&
               (resource->unk6 & 0x200) != 0) {
        computedAlpha = (((s32)resource->unkB * multiplier->unk5) * colourState) >> 16;
        extraGreen = (((s32)resource->unkC * multiplier->unk6) * colourState) >> 16;
        extraBlue = (((s32)resource->unkD * multiplier->unk7) * colourState) >> 16;
        func_80034DF0((u8)(((s32)resource->unk8 * colourState) >> 8),
                      (u8)(((s32)resource->unk9 * colourState) >> 8),
                      (u8)(((s32)resource->unkA * colourState) >> 8),
                      (u8)computedAlpha, (u8)extraGreen, (u8)extraBlue);
        specialColour = 1;
    }

    if (!specialColour && object->unk44 == 0x50) {
        s32 red;
        s32 green;
        s32 blue;

        camera = object->unk64->unk14;
        TrapDanglingJump((s32)(s8)camera->unk64[1], &computedAlpha,
                         &extraGreen, &extraBlue);
        red = 0xFF;
        green = 0xFF;
        blue = 0xFF;
        func_80034DF0(red, green, blue, (u8)computedAlpha,
                      (u8)extraGreen, (u8)extraBlue);
        specialColour = 1;
        alpha = savedAlpha;
    }

    if (!specialColour && (useColourState || alpha < 0xFF)) {
        command = (Objects0831CCommand *)D_800C94B4;
        D_800C94B4 = (s32)(command + 1);
        command->unk0 = 0xFA000000;
        if (useColourState) {
            u32 packedColour;

            packedColour = ((colourState & 0xFF) << 24) |
                           ((colourState & 0xFF) << 16) |
                           ((colourState & 0xFF) << 8) |
                           (alpha & 0xFF);
            command->unk4 = packedColour;
        } else {
            command->unk4 = (u32)-1;
        }
    }
    if (useMultiplier) {
        command = (Objects0831CCommand *)D_800C94B4;
        D_800C94B4 = (s32)(command + 1);
        command->unk0 = 0xFB000000;
        command->unk4 = ((u32)multiplier->unk5 << 24) |
                        ((u32)multiplier->unk6 << 16) |
                        ((u32)multiplier->unk7 << 8);
    }

    savedScale = object->unk8;
    if (object->unk44 == 0x50) {
        object->unk8 = 1.5f -
                       (camGetProjZ(object->unkC, object->unk10, object->unk14) *
                        D_80080F7C);
    }
    if (object->unk40->unk1E == 0) {
        func_80023598((void **)&D_800C94B4, &D_800C94B8, &D_800C94BC,
                      object, resource, alpha);
    } else {
        func_80023A08((void **)&D_800C94B4, (s32)&D_800C94B8,
                      (s32)&D_800C94BC,
                      (s16 *)object, resource, flags, alpha);
    }
    if (specialColour) {
        func_80034E48();
    }
    if (useColourState) {
        command = (Objects0831CCommand *)D_800C94B4;
        D_800C94B4 = (s32)(command + 1);
        command->unk0 = 0xFA000000;
        command->unk4 = (u32)-1;
    }
    if (useMultiplier) {
        command = (Objects0831CCommand *)D_800C94B4;
        D_800C94B4 = (s32)(command + 1);
        command->unk0 = 0xFB000000;
        command->unk4 = (u32)-0x100;
    }
    object->unk8 = savedScale;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80008B94.s")
#endif
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
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Objects09414Vector;

typedef struct {
    u8 pad00[0xA];
    s16 unkA;
    u8 pad0C[0x34];
    Objects09414Vector *unk40;
    u8 pad44[0xC];
    f32 unk50;
    u8 pad54[0x104];
    s16 unk158;
} Objects09414Resource;

typedef struct {
    f32 unk0;
    u8 pad04[0x1A];
    s8 unk1E[4];
} Objects09414Data;

typedef struct {
    void *unk0;
    s8 unk4;
    u8 pad05[3];
    f32 unk8;
    u8 pad0C[8];
} Objects09414Entry;

typedef struct {
    u8 unk0;
    u8 pad01;
    s8 unk2;
    u8 unk3;
    u8 unk4;
    u8 pad05[3];
    void *unk8;
} Objects09414StaticEntry;

typedef struct {
    s16 angle;
    s16 frame;
    u16 pad04;
    u16 divisor;
    f32 transformScale;
    f32 matrixScale;
    f32 x;
    f32 y;
    f32 z;
    s32 frameCount;
    void *spriteData;
} Objects09414Sprite;

typedef struct {
    u32 w0;
    u32 w1;
} Objects09414Gfx;

typedef struct {
    u8 pad00[0x39];
    u8 unk39;
    u8 pad3A[6];
    Objects09414Data *unk40;
    u8 pad44[0xC];
    f32 *unk50;
    u8 pad54[0xC];
    Objects09414Entry *unk60;
    Objects09414Resource *unk64;
    Objects09414Resource **unk68;
    u8 pad6C[0x20];
    u8 unk8C;
    u8 pad8D[6];
    u8 unk93;
} Objects09414Object;

extern void func_80022E80(void *transform);
extern void func_80022FD4(void **displayList, s32 matrices, s32 vertices,
                          void *transform, f32 *opacity,
                          Objects09414Sprite *sprite, s32 flags, s32 alpha);
extern void func_80047CD8(void **displayList, void *cone, s32 flags, u8 alpha);
extern f32 func_80009F08(Objects09F08Arg *arg0);

/* Workbench verdict: structure-mismatch; 411 differing words (421/378). */
/* First mismatch: +0x0; target frame 0x198, candidate frame 0x1A0. */
/* Structural gap: renderer control flow is complete; local sort/display layout is 43 instructions short. */
#ifdef NON_MATCHING
void func_80009414(void **arg0, s32 arg1, s32 arg2, void *arg3) {
    Objects09414Object *object;
    Objects09414Data *data;
    Objects09414Resource *resource;
    Objects09414Resource *root;
    Objects09414StaticEntry *staticEntry;
    Objects09414Entry *entry;
    Objects09414Vector *vector;
    Objects09414Sprite sprite;
    Objects09414Gfx *command;
    Objects09414Entry *entries[8];
    void *cones[8];
    u8 *textures[8];
    f32 depths[8];
    s16 sortIndex[8];
    s16 kindOrEntry[8];
    u16 alphas[8];
    s32 *callbacks;
    u8 *textureBase;
    f32 inverseScale;
    f32 temp;
    s32 count;
    s32 i;
    s32 j;
    s32 type;
    s32 base;
    s32 mode;
    s16 swap;

    object = (Objects09414Object *)arg3;
    resource = object->unk64;
    root = *object->unk68;
    count = 0;

    command = (Objects09414Gfx *)*arg0;
    *arg0 = (void *)(command + 1);
    command->w0 = 0xE7000000;
    command->w1 = 0;
    command = (Objects09414Gfx *)*arg0;
    *arg0 = (void *)(command + 1);
    command->w0 = 0xFB000000;
    command->w1 = (u32)-0x100;

    callbacks = (s32 *)((u8 *)resource + 0x134);
    for (i = 0; i < 4; i++) {
        if (callbacks[i] != 0) {
            TrapDanglingJump(arg0, callbacks[i]);
        }
    }

    data = object->unk40;
    if (data->unk1E[object->unk93] == 0) {
        for (i = 0; i < 4; i++) {
            staticEntry = (Objects09414StaticEntry *)
                ((u8 *)resource + 0x34C + (i * 0xC));
            if ((staticEntry->unk4 != 0) && (staticEntry->unk8 != NULL)) {
                vector = &root->unk40[staticEntry->unk2];
                depths[count] = camGetProjZ(vector->x, vector->y, vector->z);
                cones[count] = staticEntry->unk8;
                base = *(s32 *)((u8 *)root + (root->unkA * 4) + 0xC);
                textureBase = (u8 *)base;
                textures[count] = textureBase + (staticEntry->unk3 << 6);
                alphas[count] = staticEntry->unk4;
                kindOrEntry[count] = staticEntry->unk0 | 0x80;
                sortIndex[count] = count;
                count += 1;
            }
        }

        if ((resource->unk158 == 0) && (object->unk60 != NULL)) {
            entry = object->unk60;
            for (i = 0; (i < object->unk8C) && (i < 4); i++, entry++) {
                vector = &root->unk40[entry->unk4];
                depths[count] = camGetProjZ(vector->x, vector->y, vector->z);
                entries[count] = entry;
                kindOrEntry[count] = i;
                sortIndex[count] = count;
                count += 1;
            }
        }

        for (i = count - 1; i > 0; i--) {
            for (j = 0; j < i; j++) {
                if (depths[sortIndex[j + 1]] < depths[sortIndex[j]]) {
                    swap = sortIndex[j];
                    sortIndex[j] = sortIndex[j + 1];
                    sortIndex[j + 1] = swap;
                }
            }
        }

        func_80022E80((void *)((u8 *)resource + 0x43C));
        temp = func_80009F08((Objects09F08Arg *)object);
        inverseScale = temp / data->unk0;
        for (i = 0; i < count; i++) {
            j = sortIndex[i];
            type = kindOrEntry[j];
            if ((type & 0x80) != 0) {
                type &= 0x7F;
                if (type == 0) {
                    mode = 0x206;
                } else if (type == 1) {
                    mode = 6;
                } else if (type == 2) {
                    mode = 0x16;
                    func_80009220(arg0, arg1, arg2,
                                  (Objects09220Object *)object,
                                  (s32)textures[j],
                                  (Objects09220Source *)cones[j],
                                  alphas[j]);
                } else {
                    mode = 0x3333;
                }
                command = (Objects09414Gfx *)*arg0;
                *arg0 = (void *)(command + 1);
                command->w0 = 0x01810040;
                command->w1 = (u32)textures[j] + 0x80000000;
                func_80047CD8(arg0, cones[j], mode,
                              (u8)alphas[j]);
                command = (Objects09414Gfx *)*arg0;
                *arg0 = (void *)(command + 1);
                command->w0 = 0xBC00000A;
                command->w1 = 0;
            } else {
                entry = entries[j];
                sprite.angle = *(s16 *)((u8 *)resource + 0x10C +
                                         (type * 2));
                sprite.frame = *(s16 *)((u8 *)resource + 0x114 +
                                         (type * 2));
                sprite.divisor = 3;
                sprite.transformScale = entry->unk8 * inverseScale;
                sprite.matrixScale = resource->unk50;
                vector = &root->unk40[entry->unk4];
                sprite.x = vector->x;
                sprite.y = *(f32 *)((u8 *)resource + 0x11C +
                                    (type * 4)) + vector->y;
                sprite.z = vector->z;
                sprite.frameCount = 0x3333;
                sprite.spriteData = entry->unk0;
                func_80022FD4(arg0, arg1, arg2,
                              (void *)((u8 *)resource + 0x43C), object->unk50,
                              &sprite, 0xE, object->unk39);
            }
        }
    }

    callbacks = (s32 *)((u8 *)resource + 0xD0);
    if (callbacks[0] != 0) {
        TrapDanglingJump(arg0, arg1, callbacks[0]);
    }
    if (callbacks[1] != 0) {
        TrapDanglingJump(arg0, arg1, arg2, callbacks[1]);
    }
    if (callbacks[2] != 0) {
        TrapDanglingJump(arg0, arg1, callbacks[2]);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009414.s")
#endif
typedef struct {
    u8 pad00[0xD4];
    f32 unkD4;
} Objects09AA8Data;

typedef struct {
    u8 pad00[0x11];
    u8 unk11;
    u8 pad12[0x3C];
    s8 unk4E;
    u8 pad4F[0x21];
    s32 unk70;
    u8 *unk78;
} Objects09AA8Material;

typedef struct {
    void *unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    u8 pad10[0x40];
    s32 unk50;
} Objects09AA8Entry;

typedef struct {
    void *unk0;
    u8 pad04[0x64];
    s32 unk68;
    s32 unk6C;
} Objects09AA8Root;

struct Objects09AA8Object {
    u8 pad00[0x39];
    u8 unk39;
    s8 unk3A;
    u8 pad3B[5];
    Objects09AA8Data *unk40;
    u8 pad44[0xC];
    s32 unk50;
    u8 pad54[0x14];
    Objects09AA8Entry **unk68;
    u8 pad6C[0x27];
    s8 unk93;
};

typedef struct {
    u32 w0;
    u32 w1;
} Objects09AA8Command;

/* Workbench verdict: structure-mismatch; 268 differing words (target 244, candidate 269). */
/* First mismatch: +0x0; target frame 0x60 versus candidate frame 0x68. */
/* Structural gap: display-list pointer carrier adds 25 instructions and 0x08 bytes of stack. */
#ifdef NON_MATCHING
void func_80009AA8(Objects09AA8Object *object) {
    Objects09AA8Entry *sp54;
    s32 sp48;
    s32 sp38;
    s32 sp34;
    s32 temp_t5;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a1;
    s32 var_t2;
    s32 var_t3;
    s32 var_v0;
    Objects09AA8Entry **temp_v1;
    Objects09AA8Entry *temp_a1;
    Objects09AA8Root *temp_s0;
    Objects09AA8Material *temp_s1;
    Objects09AA8Entry *var_s2;
    Objects09AA8Command *command;
    register s32 *displayList;

    var_v0 = 0;
    var_t2 = 0;
    if (object->unk40->unkD4 != 0.0f) {
        var_v0 = 1;
    }
    temp_v1 = object->unk68;
    temp_a1 = temp_v1[(s32)object->unk3A];
    sp54 = temp_a1;
    if (var_v0 != 0) {
        var_s2 = temp_v1[0];
        sp38 = object->unk93;
    } else {
        var_s2 = temp_a1;
        sp38 = 0;
    }
    temp_s1 = var_s2->unk0;
    temp_s0 = (Objects09AA8Root *)sp54->unk0;
    if (var_s2->unk8 != 0) {
        if (temp_s1->unk4E != 0) {
            sp34 = 0;
            func_8005AF14(var_s2, temp_s1, object);
            var_t2 = sp34;
        } else if (temp_s1->unk11 != 0) {
            var_s2->unkA = (s16)(var_s2->unkA ^ 1);
        }
        sp34 = var_t2;
        func_80019AB8(object, var_s2, object->unk50,
                      ((Objects09AA8Entry *)((u8 *)var_s2 +
                                             (var_s2->unkA * 4)))->unkC);
        if (temp_s1->unk11 != 0) {
            sp34 = var_t2;
            func_80007C68((Objects07C68Object *)object,
                          (Objects07C68Source *)temp_s1,
                          (Objects07C68Object *)var_s2, var_s2->unk8);
        }
        var_s2->unk8 = 0;
    }
    if (temp_s1->unk11 != 0) {
        var_t2 = 1;
    }
    if ((sp38 != 0) && (sp54->unk8 != 0)) {
        sp34 = var_t2;
        func_80019AB8(object, sp54, object->unk50,
                      ((Objects09AA8Entry *)((u8 *)var_s2 +
                                             (var_s2->unkA * 4)))->unkC);
        sp54->unk8 = 0;
    }
    if (object->unk39 == 0xFF) {
        var_t3 = temp_s0->unk68;
    } else {
        var_t3 = temp_s0->unk6C;
    }

    displayList = (s32 *)&D_800C94B4;
    command = (Objects09AA8Command *)*displayList;
    *displayList = (s32)(command + 1);
    command->w0 = 0xFA000000;
    command->w1 = (u32)((s32)object->unk39 | ~0xFF);
    if (temp_s1->unk4E == 0) {
        sp34 = var_t2;
        sp48 = var_t3;
        camPushModelMtx((Gfx **)displayList, (Mtx **)&D_800C94B8,
                        (CameraScaledTransform *)object, 1.0f, 0.0f);
    }
    command = (Objects09AA8Command *)*displayList;
    *displayList = (s32)(command + 1);
    command->w0 = (u32)((((u32)(((Objects09AA8Entry *)((u8 *)var_s2 +
                                                        (var_s2->unkA * 4)))->unkC +
                                0x80000000) & 0xFFFFFF)) | 0xBF000000);
    command->w1 = (u32)(sp54->unk4 + 0x80000000);
    if (var_t2 != 0) {
        if (sp38 != 0) {
            command = (Objects09AA8Command *)*displayList;
            *displayList = (s32)(command + 1);
            command->w0 = 0x02000050;
            command->w1 = (u32)((u8 *)D_80078F20 + 8);
        } else {
            command = (Objects09AA8Command *)*displayList;
            *displayList = (s32)(command + 1);
            command->w0 = 0x02000050;
            command->w1 = (u32)(((Objects09AA8Entry *)((u8 *)var_s2 +
                                                       (var_s2->unkA * 4)))->unk50 +
                                0x80000000);
        }
    }
    temp_v0 = temp_s1->unk70;
    if (temp_v0 != 0) {
        var_a0 = 0;
        if ((temp_v0 + 1) > 0) {
            var_a1 = 0;
            do {
                if (var_t2 != 0) {
                    command = (Objects09AA8Command *)*displayList;
                    *displayList = (s32)(command + 1);
                    command->w0 = 0x02000050;
                    command->w1 = (u32)((u8 *)D_80078F20 + 8);
                }
                command = (Objects09AA8Command *)*displayList;
                var_a0 += 1;
                *displayList = (s32)(command + 1);
                command->w0 = 0x06000000;
                temp_t5 = *(s32 *)((u8 *)temp_s1->unk78 + var_a1);
                var_a1 += 4;
                command->w1 = (u32)(temp_t5 + 0x80000000);
            } while (temp_s1->unk70 >= var_a0);
        }
    } else {
        command = (Objects09AA8Command *)*displayList;
        *displayList = (s32)(command + 1);
        command->w0 = 0x06000000;
        command->w1 = (u32)(var_t3 + 0x80000000);
    }
    command = (Objects09AA8Command *)*displayList;
    *displayList = (s32)(command + 1);
    command->w0 = 0xBF000000;
    command->w1 = 0;
    camRestoreModelMtx((Gfx **)displayList);
    func_80034920((Gfx **)displayList);
    command = (Objects09AA8Command *)*displayList;
    *displayList = (s32)(command + 1);
    command->w0 = 0xFA000000;
    command->w1 = (u32)-1;
    command = (Objects09AA8Command *)*displayList;
    *displayList = (s32)(command + 1);
    command->w0 = 0xFB000000;
    command->w1 = (u32)-0x100;
    D_80079250 = 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009AA8.s")
#endif
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
            var_f2 += (temp_f0 - 250.0f) * 0.00134f;
            if (var_f2 > 2.0f) {
                var_f2 = 2.0f;
            }
        }
    }
    return var_f2;
}
/* Workbench verdict: structure-mismatch; 149 differing words (target 180, candidate 178). */
/* First mismatch: +0x0; target frame 0x28 versus candidate 0x30. */
/* Structural gap: dispatch control flow is complete, but two instructions and stack homes differ. */
#ifdef NON_MATCHING
void func_80009F74(void *arg0) {
    Objects09F74Object *object;
    Objects09F74Data *data;
    Objects09F74Camera *camera;
    f32 projection;
    f32 threshold;
    s32 variant;
    s8 count;
    s8 selector;

    object = (Objects09F74Object *)arg0;
    data = object->unk40;
    if (data->unkD4 == 0.0f) {
        object->unk93 = object->unk3A;
    } else {
        projection = -camGetProjZ(object->unkC, object->unk10, object->unk14);
        variant = 0;
        count = data->unk22;
        if ((count > 0) && (data->unkD4 != 0.0f) &&
            (data->unkD4 < projection)) {
            do {
                variant += 1;
                if ((variant >= 3) || (variant >= count)) {
                    break;
                }
                threshold = *(f32 *)((u8 *)data + (variant * 4) + 0xD4);
                if ((threshold == 0.0f) || (threshold >= projection)) {
                    break;
                }
            } while (1);
        }
        if ((object->unk44 == 1) && (D_8007BF0C != 0) &&
            (variant == 0) && ((s32)D_8007BEF8 >= 3)) {
            if (camGetNo() != *(s8 *)object->unk64) {
                variant = 1;
            }
        }
        object->unk93 = (s8)variant;
    }

    if ((object->unk44 == 1) || (object->unk44 == 0x3F)) {
        camera = camGetPtr();
        if (camera->unk4E >= 2) {
            object->unk93 = 0;
        } else if (object->unk44 == 0x3F) {
            object->unk8 = func_80009F08((Objects09F08Arg *)object) *
                           data->unk0;
        } else {
            *(f32 *)((u8 *)object->unk64 + 0x444) =
                func_80009F08((Objects09F08Arg *)object) * object->unk8;
        }
    }

    data = object->unk40;
    if (data->unkD4 == 0.0f) {
        selector = data->unk1E;
        switch (selector) {
        case 0:
            func_80009AA8(object);
            return;
        case 1:
            func_80008B94(object);
            return;
        case 2:
            func_80008A8C((Objects08A20Arg *)object);
            return;
        }
    } else {
        variant = (u8)object->unk93;
        selector = *(s8 *)((u8 *)data + variant + 0x1E);
        switch (selector) {
        case 0:
            object->unk3A = (s8)variant;
            func_80009AA8(object);
            object->unk3A = 0;
            return;
        case 1:
            object->unk3A = (s8)variant;
            func_80008B94(object);
            object->unk3A = 0;
            return;
        case 2:
            object->unk3A = (s8)variant;
            func_80008A8C((Objects08A20Arg *)object);
            return;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009F74.s")
#endif
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
s32 func_8000A6E8(s32 arg0) {
    s32 flags;

    switch (arg0) {
        case 1:
            flags = 0xF01;
            break;
        case 4:
            flags = 0x301;
            break;
        case 3:
            flags = 0xB01;
            break;
        case 31:
            flags = 0x1101;
            break;
        case 24:
            flags = 0x1301;
            break;
        case 25:
            flags = 0x1200;
            break;
        case 26:
            flags = 0xB01;
            break;
        case 22:
        case 29:
        case 73:
        case 79:
            flags = 0x301;
            break;
        case 23:
            flags = 0x200;
            break;
        case 11:
        case 12:
        case 49:
            flags = 0x200;
            break;
        case 33:
            flags = 0x301;
            break;
        case 41:
            flags = 0x1101;
            break;
        case 53:
            flags = 0x200;
            break;
        case 54:
            flags = 0x101;
            break;
        case 55:
            flags = 0x101;
            break;
        case 56:
            flags = 0x101;
            break;
        case 57:
            flags = 0xB01;
            break;
        case 58:
            flags = 0xA00;
            break;
        case 60:
            flags = 0x200;
            break;
        case 63:
            flags = 0x101;
            break;
        case 64:
            flags = 0xB01;
            break;
        case 65:
            flags = 0x301;
            break;
        case 67:
            flags = 0xB01;
            break;
        case 71:
            flags = 0x101;
            break;
        case 72:
            flags = 0x301;
            break;
        case 74:
            flags = 0x301;
            break;
        case 75:
            flags = 0x101;
            break;
        case 77:
            flags = 0x101;
            break;
        case 78:
            flags = 0x301;
            break;
        case 82:
            flags = 0xB01;
            break;
        case 84:
            flags = 0x301;
            break;
        case 85:
            flags = 0x200;
            break;
        case 86:
            flags = 0x101;
            break;
        case 87:
            flags = 0x301;
            break;
        case 88:
            flags = 0x301;
            break;
        default:
            flags = 0;
            break;
    }
    return flags;
}
#else
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
struct Objects0AA38Object {
    u8 pad00[0x40];
    Objects0A830Data *unk40;
    s16 unk44;
};

extern void func_8001C4C0(void *object, void *arg1, s32 mode);
extern void spranimInit(void *object, void *arg1);
extern void sprasjiInit(void *object, void *arg1);
extern void func_8001A154(void *object);
extern void func_8001BAE4(void *object, void *arg1);
extern void func_8001BAF8(void *object, void *arg1);

/* Workbench verdict: relocation-layout-mismatch; 0 differing words (301/301). */
/* First mismatch: relocation metadata at +0x20; frame and opcode schedule are exact. */
/* Structural gap: the generated switch table is anonymous .rodata rather than jtbl_80081258. */
#ifdef NON_MATCHING
void func_8000AA38(Objects0AA38Object *arg0, void *arg1) {
    arg0->unk44 = arg0->unk40->unk1C;
    switch (arg0->unk44 - 1) {
        case 0:
            func_8001C4C0(arg0, arg1, 1);
            break;
        case 1:
        case 2:
        case 3:
            TrapDanglingJump();
            break;
        case 4:
            TrapDanglingJump();
            break;
        case 5:
            TrapDanglingJump();
            break;
        case 6:
            TrapDanglingJump();
            break;
        case 7:
            TrapDanglingJump();
            break;
        case 8:
            TrapDanglingJump();
            break;
        case 9:
            break;
        case 10:
            TrapDanglingJump();
            break;
        case 11:
            TrapDanglingJump();
            break;
        case 12:
            break;
        case 13:
            TrapDanglingJump();
            break;
        case 14:
            TrapDanglingJump();
            break;
        case 15:
            break;
        case 16:
            break;
        case 17:
            break;
        case 18:
            break;
        case 19:
            TrapDanglingJump();
            break;
        case 20:
            TrapDanglingJump();
            break;
        case 21:
            break;
        case 22:
            break;
        case 23:
        case 24:
            func_8001BAE4(arg0, arg1);
            break;
        case 25:
            func_8001BAF8(arg0, arg1);
            break;
        case 26:
            break;
        case 27:
            TrapDanglingJump();
            break;
        case 28:
            TrapDanglingJump();
            break;
        case 29:
            TrapDanglingJump();
            break;
        case 30:
            TrapDanglingJump();
            break;
        case 31:
        case 33:
            spranimInit(arg0, arg1);
            break;
        case 32:
            TrapDanglingJump();
            break;
        case 34:
            func_8001A154(arg0);
            break;
        case 35:
            TrapDanglingJump();
            break;
        case 36:
            break;
        case 37:
            break;
        case 38:
            break;
        case 39:
            break;
        case 40:
            TrapDanglingJump();
            break;
        case 41:
            TrapDanglingJump();
            break;
        case 42:
            TrapDanglingJump();
            break;
        case 43:
            TrapDanglingJump();
            break;
        case 44:
            TrapDanglingJump();
            break;
        case 45:
            TrapDanglingJump();
            break;
        case 46:
            TrapDanglingJump();
            break;
        case 47:
            TrapDanglingJump();
            break;
        case 48:
            TrapDanglingJump();
            break;
        case 49:
            TrapDanglingJump();
            break;
        case 50:
            TrapDanglingJump();
            break;
        case 51:
            break;
        case 52:
            TrapDanglingJump();
            break;
        case 53:
            TrapDanglingJump();
            break;
        case 54:
            TrapDanglingJump();
            break;
        case 55:
            break;
        case 56:
            TrapDanglingJump();
            break;
        case 57:
            TrapDanglingJump();
            break;
        case 58:
            sprasjiInit(arg0, arg1);
            break;
        case 59:
            TrapDanglingJump();
            break;
        case 60:
            TrapDanglingJump();
            break;
        case 61:
            TrapDanglingJump();
            break;
        case 62:
            TrapDanglingJump();
            break;
        case 63:
            TrapDanglingJump();
            break;
        case 64:
            TrapDanglingJump();
            break;
        case 65:
            break;
        case 66:
            TrapDanglingJump();
            break;
        case 67:
            TrapDanglingJump();
            break;
        case 68:
            TrapDanglingJump();
            break;
        case 69:
            TrapDanglingJump();
            break;
        case 70:
            TrapDanglingJump();
            break;
        case 71:
            TrapDanglingJump();
            break;
        case 72:
            TrapDanglingJump();
            break;
        case 73:
            TrapDanglingJump();
            break;
        case 74:
            TrapDanglingJump();
            break;
        case 75:
            TrapDanglingJump();
            break;
        case 76:
            TrapDanglingJump();
            break;
        case 77:
            TrapDanglingJump();
            break;
        case 78:
            break;
        case 79:
            TrapDanglingJump();
            break;
        case 80:
            TrapDanglingJump();
            break;
        case 81:
            TrapDanglingJump();
            break;
        case 82:
            TrapDanglingJump();
            break;
        case 83:
            TrapDanglingJump();
            break;
        case 84:
            TrapDanglingJump();
            break;
        case 85:
            TrapDanglingJump();
            break;
        case 86:
            TrapDanglingJump();
            break;
        case 87:
            TrapDanglingJump();
            break;
        case 88:
            TrapDanglingJump();
            break;
        case 89:
            TrapDanglingJump();
            break;
        case 90:
            TrapDanglingJump();
            break;
        case 91:
            TrapDanglingJump();
            break;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000AA38.s")
#endif
typedef struct {
    u8 pad00[0x40];
    u8 *unk40;
    s16 unk44;
    u8 pad46[0x32];
    void *unk78;
} Objects0AEECObject;

/* Workbench verdict: allocation-mismatch; 65 differing words (312/312). */
/* First mismatch: +0x44; opcode schedule and frame are exact. */
/* Shape status: recovered control-type dispatch; only register allocation and jump-table naming remain. */
#ifdef NON_MATCHING
void func_8000AEEC(void *arg0, s32 arg1) {
    Objects0AEECObject *object;
    s32 type;

    D_8007A210 = 3;
    D_8007A21C = 1;
    D_8007A214 = arg0;
    object = (Objects0AEECObject *)arg0;
    D_8007A218 = object->unk40 + 4;
    type = object->unk44 - 3;
    switch (type) {
        case 0:
            TrapDanglingJump(object, arg1);
            break;
        case 31:
            spranimOnceControl(object, arg1);
            break;
        case 29:
            spranimControl(object, arg1);
            break;
        case 17:
            texscrollControl(object, arg1);
            break;
        case 3:
            effectboxControl(object, arg1);
            break;
        case 38:
            TrapDanglingJump(object, arg1);
            break;
        case 33:
            TrapDanglingJump(object);
            break;
        case 39:
            TrapDanglingJump(object, arg1);
            break;
        case 5:
            TrapDanglingJump(object, arg1);
            break;
        case 4:
            func_800148E0(object);
            break;
        case 26:
            TrapDanglingJump(object, arg1);
            break;
        case 70:
            TrapDanglingJump(object, arg1);
            break;
        case 25:
            func_8001B798(object, arg1);
            break;
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
            func_8001BB04(object, arg1);
            break;
        case 76:
            func_8001BB10(object, arg1);
            break;
        case 30:
            TrapDanglingJump(object, arg1);
            break;
        case 43:
            TrapDanglingJump(object, arg1);
            break;
        case 44:
            TrapDanglingJump(object, arg1);
            break;
        case 45:
            TrapDanglingJump(object, arg1);
            break;
        case 46:
            TrapDanglingJump(object, arg1);
            break;
        case 47:
            TrapDanglingJump(object, arg1);
            break;
        case 50:
            TrapDanglingJump(object, arg1);
            break;
        case 51:
            TrapDanglingJump(object, arg1);
            break;
        case 52:
            TrapDanglingJump(object, arg1);
            break;
        case 54:
            TrapDanglingJump(object, arg1);
            break;
        case 55:
            TrapDanglingJump(object, arg1);
            break;
        case 57:
            TrapDanglingJump(object, arg1);
            break;
        case 58:
            TrapDanglingJump(object);
            break;
        case 60:
            TrapDanglingJump(object, arg1);
            break;
        case 61:
            TrapDanglingJump(object, arg1);
            break;
        case 62:
            TrapDanglingJump(object, arg1);
            break;
        case 63:
            TrapDanglingJump(object, arg1);
            break;
        case 64:
            TrapDanglingJump(object, arg1);
            break;
        case 65:
            TrapDanglingJump(object, arg1);
            break;
        case 66:
            TrapDanglingJump(object, arg1);
            break;
        case 68:
            TrapDanglingJump(object, arg1);
            break;
        case 69:
            TrapDanglingJump(object, arg1);
            break;
        case 71:
            TrapDanglingJump(object, arg1);
            break;
        case 72:
            TrapDanglingJump(object, arg1);
            break;
        case 73:
            TrapDanglingJump(object, arg1);
            break;
        case 74:
            TrapDanglingJump(object, arg1);
            break;
        case 75:
            TrapDanglingJump(object, arg1);
            break;
        case 77:
            TrapDanglingJump(object, arg1);
            break;
        case 78:
            TrapDanglingJump(object, arg1);
            break;
        case 79:
            TrapDanglingJump(object, arg1);
            break;
        case 80:
            TrapDanglingJump(object, arg1);
            break;
        case 81:
            TrapDanglingJump(object, arg1);
            break;
        case 82:
            TrapDanglingJump(object, arg1);
            break;
        case 83:
            TrapDanglingJump(object, arg1);
            break;
        case 84:
            TrapDanglingJump(object, arg1);
            break;
        case 85:
            TrapDanglingJump(object, arg1);
            break;
        case 86:
            TrapDanglingJump(object, arg1);
            break;
        case 15:
            rangetriggerControl(object, arg1);
            break;
        case 87:
            TrapDanglingJump(object, arg1);
            break;
        case 88:
            TrapDanglingJump(object, arg1);
            break;
        case 1:
        case 2:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 16:
        case 18:
        case 27:
        case 28:
        case 32:
        case 34:
        case 35:
        case 36:
        case 37:
        case 40:
        case 41:
        case 42:
        case 48:
        case 49:
        case 53:
        case 56:
        case 59:
        case 67:
            break;
        default:
            break;
    }
    if (object->unk78 != NULL) {
        func_8000B3CC(object, arg1);
    }
    D_8007A21C = 4;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000AEEC.s")
#endif
/* Workbench verdict: structure-mismatch; 489 differing words (494/427). */
/* First mismatch: +0x0; target frame 0x98, candidate frame 0x88. */
/* Structural gap: candidate is 67 instructions short; collision-state control flow is complete. */
#ifdef NON_MATCHING
void func_8000B3CC(void *arg0, s32 arg1) {
    Objects0B3CCObject *object;
    Objects0B3CCData *data;
    Objects0B3CCConfig *config;
    Objects0B3CCState *state;
    Objects0BB84Vec3 start;
    Objects0BB84Vec3 end;
    f32 radius;
    f32 step;
    f32 acceleration;
    f32 damping;
    f32 speed;
    f32 dot;
    f32 factor;
    f32 volume;
    f32 savedY;
    s16 savedAngle2;
    s16 savedAngle4;
    u32 collision;
    s32 bounced;

    object = (Objects0B3CCObject *)arg0;
    data = object->unk40;
    state = object->unk78;
    state->unk2 = 0;
    if ((state->flags & 2) != 0) {
        return;
    }

    step = (f32)arg1;
    config = data->unkE0;
    if ((state->flags & 1) != 0) {
        acceleration = -config->unk8;
        damping = config->unkC;
    } else {
        acceleration = -config->unk0;
        damping = config->unk4;
    }

    start.x = object->unkC + config->unk20;
    start.y = object->unk10 + config->unk24;
    start.z = object->unk14 + config->unk28;
    end.x = start.x + (object->unk1C * step) + state->unk1C;
    end.z = start.z + (object->unk24 * step) + state->unk20;
    end.y = start.y + (object->unk20 * step) +
            (0.5f * acceleration * step * step);
    radius = state->unk4;

    trackMakePolylist(1, &start, &end, &radius, 0x10000, 0);
    collision = (u32)func_80010900(&start, &end, radius, (s32)object,
                                    (void *)func_8000BB84);
    if ((collision >> 30) != 0) {
        object->unk1C = 0.0f;
        object->unk20 = 0.0f;
        object->unk24 = 0.0f;
        state->flags |= 2;
        return;
    }

    func_80008128((Objects08128Object *)object, end.x - start.x,
                  end.y - start.y, end.z - start.z);
    object->unk20 += acceleration * step;
    speed = sqrtf((object->unk1C * object->unk1C) +
                  (object->unk20 * object->unk20) +
                  (object->unk24 * object->unk24));
    state->unk18 = speed;

    if (((func_8001357C(object->unkC, object->unk14, &state->unk14,
                        0x10000, NULL) & 0x10000) != 0) &&
        ((end.y - radius) < state->unk14)) {
        damping = config->unkC;
        if ((state->flags & 1) == 0) {
            state->flags |= 1;
            if ((config->unk1C != 0) && (state->unk24 == NULL)) {
                func_80002FE0(config->unk1C, object->unkC, object->unk10,
                              object->unk14, 4, &state->unk24);
                if (state->unk24 != NULL) {
                    volume = state->unk18 * D_8008152C;
                    if (volume > 1.0f) {
                        volume = 1.0f;
                    }
                    func_8000309C(state->unk24,
                                  (u8)((s32)(127.0f * volume) & 0xFF));
                }
            }
            if (config->unk14 != 0) {
                savedY = object->unk10;
                savedAngle2 = object->unk2;
                savedAngle4 = object->unk4;
                object->unk2 = 0;
                object->unk4 = 0;
                object->unk10 = state->unk14;
                object->unk80 = config->unk14;
                partUpdateTriggers(object, arg1);
                object->unk80 = 0;
                object->unk10 = savedY;
                object->unk2 = savedAngle2;
                object->unk4 = savedAngle4;
            }
        }
    } else {
        state->flags &= ~1;
    }

    object->unk1C *= damping;
    object->unk20 *= damping;
    object->unk24 *= damping;
    state->unk18 = sqrtf((object->unk1C * object->unk1C) +
                         (object->unk20 * object->unk20) +
                         (object->unk24 * object->unk24));

    bounced = 0;
    if ((collision << 2) != 0) {
        state->unk2 = 1;
        speed = state->unk18;
        if ((config->unk10 == 0.0f) || (speed == 0.0f)) {
            object->unk1C = 0.0f;
            object->unk20 = 0.0f;
            object->unk24 = 0.0f;
            state->flags |= 2;
        } else {
            object->unk1C /= speed;
            object->unk20 /= speed;
            object->unk24 /= speed;
            state->unk18 *= config->unk10;
            dot = (state->unk8 * object->unk1C) +
                  (state->unkC * object->unk20) +
                  (state->unk10 * object->unk24);
            factor = 2.0f * -dot;
            object->unk1C = ((factor * state->unk8) + object->unk1C) *
                            state->unk18;
            object->unk20 = ((factor * state->unkC) + object->unk20) *
                            state->unk18;
            object->unk24 = ((factor * state->unk10) + object->unk24) *
                            state->unk18;
            speed = sqrtf((object->unk1C * object->unk1C) +
                          (object->unk20 * object->unk20) +
                          (object->unk24 * object->unk24));
            if (speed < 1.0f) {
                if (state->unkC < D_80081530) {
                    object->unk1C = state->unk8;
                    object->unk20 = state->unkC;
                    object->unk24 = state->unk10;
                } else {
                    object->unk1C = 0.0f;
                    object->unk20 = 0.0f;
                    object->unk24 = 0.0f;
                    state->unk18 = 0.0f;
                    state->flags |= 2;
                }
            } else {
                bounced = 1;
            }
        }
        if ((bounced != 0) && (config->unk1E != 0) &&
            ((state->flags & 1) == 0) && (state->unk24 == NULL)) {
            func_80002FE0(config->unk1E, object->unkC, object->unk10,
                          object->unk14, 4, &state->unk24);
            if (state->unk24 != NULL) {
                volume = state->unk18 * D_80081534;
                if (volume > 1.0f) {
                    volume = 1.0f;
                }
                func_8000309C(state->unk24,
                              (u8)((s32)(127.0f * volume) & 0xFF));
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000B3CC.s")
#endif
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
f32 func_8000BCB0(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5)
{
  f32 temp_f0;
  f32 temp_f16;
  f32 temp_f2;
  temp_f0 = arg0 - arg3;
  temp_f2 = arg1 - arg4;
  temp_f16 = arg2 - arg5;
  return sqrtf(((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) + (temp_f16 * temp_f16));
}
/* Workbench verdict: schedule-mismatch; 2 differing words. */
/* First mismatch: +0x2C. */
/* Shape-exact candidate; instruction ordering is reserved for the permuter. */
f32 func_8000BD0C(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5)
{
  f32 temp_f16;
  f32 temp_f18;
  f32 temp_f2;
  temp_f2 = arg0 - arg3;
  temp_f16 = arg1 - arg4;
  temp_f18 = arg2 - arg5;
  return ((temp_f2 * temp_f2) + (temp_f16 * temp_f16)) + (temp_f18 * temp_f18);
}


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
