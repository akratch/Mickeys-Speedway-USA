#include "PR/ultratypes.h"

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

extern s16 D_800C94B0;
extern s8 D_80078F88;
extern s8 D_80079004;
extern void **D_800C94F4;
extern s32 D_800C94F8;
extern void **D_800C9494;
extern s32 D_800C9498;
extern s32 D_800C949C;
extern s32 D_800C94C0[];
extern s32 D_800C94C8[];
extern void **D_800C9500;
extern s32 D_800C9504;

#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80004340.s")
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
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_800056A4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_800056F0.s")
void **func_8000572C(s32 *start, s32 *end) {
    *start = D_800C949C;
    *end = D_800C9498;
    return D_800C9494;
}
void **func_80005750(s32 *count) {
    *count = D_800C94F8;
    return D_800C94F4;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80005768.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80005798.s")
void **func_80005808(s32 *count) {
    *count = D_800C9504;
    return D_800C9500;
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80005820.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80005868.s")
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_800058C0.s")
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
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_80006EA0.s")
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
#pragma GLOBAL_ASM("asm/nonmatchings/main/objects/func_8000BD0C.s")
