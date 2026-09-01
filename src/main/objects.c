#include "PR/ultratypes.h"

typedef struct AnimPathObject AnimPathObject;

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
    u8 pad22[0xAE];
    f32 unkD0[4];
} Objects58C0Data;

typedef struct {
    u8 pad00[0x40];
    Objects58C0Data *unk40;
} Objects58C0Arg;

extern s32 D_800C9470;
extern s32 D_800C9474;
extern s32 D_800C94A8;
extern s32 D_800C94AC;
extern s16 D_800C94B0;
extern s16 D_800C94B2;
extern s8 D_80078F88;
extern s8 D_80079004;
extern void **D_800C94F4;
extern s32 D_800C94F8;
extern void **D_800C9494;
extern s32 D_800C9498;
extern s32 D_800C949C;
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
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000439C.s")
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
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80005798.s")
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
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006448.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006534.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006868.s")
s32 func_800069C0(Objects69C0In *arg0, Objects69C0Out *arg1) {
    arg0->unk78 = arg1;
    arg1->unk4 = arg0->unk40->unkE0->unk2C;
    arg0->unk78->unk0 = 2;
    return 0x2C;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_800069E8.s")
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
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006EE4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006FA0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80007118.s")
void func_80007844(void) {
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000784C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80007C68.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80007E40.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80008028.s")
void func_80008118(void) {
    D_80079004 = 1;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80008128.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000831C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_800084C4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80008A20.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80008A8C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80008B94.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009220.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009414.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009AA8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009E78.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009F08.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80009F74.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A244.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A39C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A62C.s")
void func_8000A6DC(s32 arg0) {
    D_800C94B0 = arg0;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A6E8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000A830.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000AA38.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000AEEC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000B3CC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000BB84.s")
void GetRomlistInfo(s32 *romlist, s32 *size, s32 index) {
    *romlist = D_800C94C0[index];
    *size = D_800C94C8[index];
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000BCB0.s")
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
