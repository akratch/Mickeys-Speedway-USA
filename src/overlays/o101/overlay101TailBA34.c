#include "PR/ultratypes.h"

typedef struct O101TailBA34Root {
    u8 pad00[0x1C];
    s32 chainType;
    void *chain;
    u8 kind;
    u8 pad25;
    s16 value26;
    s16 value28;
    s16 value2A;
    s16 value2C;
    s16 width2E;
    s16 height30;
    u8 color32;
    u8 color33;
    void *asset34;
    s32 childType;
    void *child;
    u8 mode40;
    u8 pad41;
    s16 x42;
    s16 width44;
    s16 y46;
    s16 height48;
    s16 value4A;
    s16 value4C;
    u8 color4E;
    u8 color4F;
    void *data50;
    s32 tertiaryType;
    void *tertiary;
    u8 mode5C;
    u8 pad5D;
    s16 x5E;
    s16 width60;
    s16 y62;
    s16 height64;
    s16 value66;
    s16 value68;
    u8 color6A;
    u8 color6B;
    void *data6C;
} O101TailBA34Root;

typedef struct O101TailBA34Node32 {
    s32 previousType;
    void *previous;
    s16 x;
    s16 y;
    f32 scale;
    s16 value10;
    u8 color12;
    u8 color13;
    f32 value14;
    s32 value18;
    void *handle;
} O101TailBA34Node32;

typedef struct O101TailBA34Node24 {
    s32 previousType;
    void *previous;
    s16 x;
    s16 y;
    u8 length;
    s8 opacity;
    u8 mode;
    u8 color0;
    u8 color1;
    u8 color2;
    u8 color3;
    u8 kind;
    u8 *text;
} O101TailBA34Node24;

typedef struct O101TailBA34Inputs {
    u8 pad000[0x84];
    void *data084;
    u8 pad088[0x110];
    void *data198;
    u8 *text19C;
    u8 *text1A0;
    u8 *text1A4;
    u8 *text1A8;
    u8 *text1AC;
} O101TailBA34Inputs;

extern O101TailBA34Root gO101TailBA34Root;
extern s32 gO101TailBA34OrderCount;
extern s32 gO101TailBA34OrderCountCall0;
extern s32 gO101TailBA34OrderCountCall1;
extern s32 gO101TailBA34OrderCountCall2;
extern void *gO101TailBA34OrderSlots[];
extern s32 gO101TailBA34Node32Count;
extern O101TailBA34Node32 gO101TailBA34Nodes32[];
extern s32 gO101TailBA34Node24Count;
extern O101TailBA34Node24 gO101TailBA34Nodes24[];
extern O101TailBA34Inputs gO101TailBA34Inputs;
extern u8 gO101TailBA34AssetDB8;
extern u8 gO101TailBA34FinalObject4630;

/* The three creator calls and finalizer are packet-local opaque identities. */
extern void *o101TailBA34RootCreatorReloc(s32 key, void *source,
                                          s32 *orderCount);
extern void *o101TailBA34ZeroCreatorReloc(s32 key, void *source,
                                          s32 *orderCount);
extern void *o101TailBA34NonzeroCreatorReloc(s32 key, void *source,
                                             s32 *orderCount);
extern void o101TailBA34FinalizerReloc(void *object);
extern s32 overlay101ByteLength(u8 *text);

/* Exact typed owner for overlay 101 +0xBA34..+0xC144. */
/* 393 -> 0, lane p10-o101 2026-09-12, on three edits and in that order. The
 * residual read as a prologue problem -- first mismatch at +0 with a frame
 * 0x10 too large -- and it was two carriers and one constant.
 * L145, but PARTIALLY, and the boundary is the point. Delete the locals that
 *   held an element address or an element index and spell those stores through
 *   the array subscript of the counter global itself, so that the subscript
 *   written on both sides of a call is ONE IR name (L131) and the pre-call read
 *   lands in a ring temporary a local can never be (L130). That works for the
 *   sprite nodes and the text rows, whose counter has ONE name. It does NOT
 *   work for the three root headers: this reconstruction models the shipped
 *   relocation identity by giving the order counter two names, Call0 and Call1,
 *   so the pre-call and post-call subscripts are different expressions, L131
 *   cannot fuse them, and going direct there costs instructions -- whole-body
 *   direct measures 444 at delta +8 against 393. Mixed, it is 333 at delta 0.
 *   THE TEST IS WHETHER BOTH SIDES OF THE CALL CAN BE SPELLED THE SAME.
 * L144 -- `register volatile f32 scale` was the second carrier and the larger
 *   one. volatile pins a home and re-loads it at every use: the frame census
 *   showed our +0x40 slot taking five loads where the target takes one, plus a
 *   whole extra slot. The shipped code has no carrier at all -- the multiplier
 *   is the `(f32)(s32)1` literal written in place, exactly as in the rest of
 *   this overlay's builders. 304 -> 263 at delta 0, and the first divergence
 *   moves from +0 to +0x144, which is the frame going exact.
 * L146 -- then re-climb, because every order measured before those two edits is
 *   void. The climb moves one statement: kind ahead of width2E in the first
 *   root header. 4 -> 0. Two further rounds are flat. */
void func_overlay_101_F000BA34_18E7254(s32 variant) {
    void *handle;
    s32 length;
    s32 orderIndex;

    orderIndex = gO101TailBA34OrderCountCall0; gO101TailBA34Root.width2E = 0x140; gO101TailBA34Root.height30 = 0xF0; gO101TailBA34Root.kind = 4; gO101TailBA34Root.asset34 = &gO101TailBA34AssetDB8; gO101TailBA34Root.color32 = 0xFF; gO101TailBA34Root.color33 = 0xFF; gO101TailBA34Root.value26 = 0; gO101TailBA34Root.value28 = 0; gO101TailBA34Root.value2A = 0; gO101TailBA34Root.value2C = 0; gO101TailBA34Root.chainType = 0; gO101TailBA34Root.chain = NULL; gO101TailBA34OrderSlots[orderIndex] = &gO101TailBA34Root.chainType; gO101TailBA34OrderCountCall0 = orderIndex + 1;

    gO101TailBA34Nodes32[gO101TailBA34Node32Count].x = 0x4E; gO101TailBA34Nodes32[gO101TailBA34Node32Count].y = 0x14E; gO101TailBA34Nodes32[gO101TailBA34Node32Count].value10 = 0; gO101TailBA34Nodes32[gO101TailBA34Node32Count].color12 = 0xFF; gO101TailBA34Nodes32[gO101TailBA34Node32Count].color13 = 0; gO101TailBA34Nodes32[gO101TailBA34Node32Count].value18 = 0; gO101TailBA34Nodes32[gO101TailBA34Node32Count].scale = 1.0f; gO101TailBA34Nodes32[gO101TailBA34Node32Count].value14 = 0.0f; handle = o101TailBA34RootCreatorReloc(0x92, NULL, &gO101TailBA34OrderCountCall0); gO101TailBA34Nodes32[gO101TailBA34Node32Count].previousType = gO101TailBA34Root.chainType; gO101TailBA34Nodes32[gO101TailBA34Node32Count].previous = gO101TailBA34Root.chain; gO101TailBA34Nodes32[gO101TailBA34Node32Count].handle = handle; gO101TailBA34Root.chainType = 2; gO101TailBA34Root.chain = &gO101TailBA34Nodes32[gO101TailBA34Node32Count]; gO101TailBA34Node32Count = gO101TailBA34Node32Count + 1;

    orderIndex = gO101TailBA34OrderCountCall1; gO101TailBA34Root.x42 = 0x20; gO101TailBA34Root.width44 = 0x18; gO101TailBA34Root.y46 = 0x8C; gO101TailBA34Root.height48 = 0x18; gO101TailBA34Root.value4A = 0x5B; gO101TailBA34Root.value4C = 0x63; gO101TailBA34Root.mode40 = 0; gO101TailBA34Root.color4E = 0xFF; gO101TailBA34Root.color4F = 0xFF; gO101TailBA34Root.childType = 0; gO101TailBA34Root.child = NULL; gO101TailBA34Root.data50 = gO101TailBA34Inputs.data198; gO101TailBA34OrderSlots[orderIndex] = &gO101TailBA34Root.childType; gO101TailBA34OrderCountCall1 = orderIndex + 1;

    if (variant == 0) {
        gO101TailBA34Nodes32[gO101TailBA34Node32Count].x = 6; gO101TailBA34Nodes32[gO101TailBA34Node32Count].y = 12; gO101TailBA34Nodes32[gO101TailBA34Node32Count].scale = 1.0f; gO101TailBA34Nodes32[gO101TailBA34Node32Count].value10 = 0; gO101TailBA34Nodes32[gO101TailBA34Node32Count].color12 = 0xFF; gO101TailBA34Nodes32[gO101TailBA34Node32Count].color13 = 0; gO101TailBA34Nodes32[gO101TailBA34Node32Count].value14 = 0.0f; gO101TailBA34Nodes32[gO101TailBA34Node32Count].value18 = 0; handle = o101TailBA34ZeroCreatorReloc(0xA0, NULL, &gO101TailBA34OrderCountCall1); gO101TailBA34Nodes32[gO101TailBA34Node32Count].previousType = gO101TailBA34Root.childType; gO101TailBA34Nodes32[gO101TailBA34Node32Count].previous = gO101TailBA34Root.child; gO101TailBA34Nodes32[gO101TailBA34Node32Count].handle = handle; gO101TailBA34Root.childType = 2; gO101TailBA34Root.child = &gO101TailBA34Nodes32[gO101TailBA34Node32Count]; gO101TailBA34Node32Count = gO101TailBA34Node32Count + 1;
    } else {
        gO101TailBA34Nodes32[gO101TailBA34Node32Count].x = 6; gO101TailBA34Nodes32[gO101TailBA34Node32Count].y = 12; gO101TailBA34Nodes32[gO101TailBA34Node32Count].scale = 1.0f; gO101TailBA34Nodes32[gO101TailBA34Node32Count].value10 = 0; gO101TailBA34Nodes32[gO101TailBA34Node32Count].color12 = 0xFF; gO101TailBA34Nodes32[gO101TailBA34Node32Count].color13 = 0; gO101TailBA34Nodes32[gO101TailBA34Node32Count].value14 = 0.0f; gO101TailBA34Nodes32[gO101TailBA34Node32Count].value18 = 0; handle = o101TailBA34NonzeroCreatorReloc(0x9F, NULL, &gO101TailBA34OrderCountCall1); gO101TailBA34Nodes32[gO101TailBA34Node32Count].previousType = gO101TailBA34Root.childType; gO101TailBA34Nodes32[gO101TailBA34Node32Count].previous = gO101TailBA34Root.child; gO101TailBA34Nodes32[gO101TailBA34Node32Count].handle = handle; gO101TailBA34Root.childType = 2; gO101TailBA34Root.child = &gO101TailBA34Nodes32[gO101TailBA34Node32Count]; gO101TailBA34Node32Count = gO101TailBA34Node32Count + 1;
    }

    orderIndex = gO101TailBA34OrderCountCall1; gO101TailBA34Root.x5E = 0x20; gO101TailBA34Root.width60 = 0x40; gO101TailBA34Root.y62 = 0x5A; gO101TailBA34Root.height64 = 0x7E; gO101TailBA34Root.value66 = 0xCC; gO101TailBA34Root.value68 = 0x4A; gO101TailBA34Root.mode5C = 0; gO101TailBA34Root.color6A = 0xFF; gO101TailBA34Root.color6B = 0xFF; gO101TailBA34Root.tertiaryType = 0; gO101TailBA34Root.tertiary = NULL; gO101TailBA34Root.data6C = gO101TailBA34Inputs.data084; gO101TailBA34OrderSlots[orderIndex] = &gO101TailBA34Root.tertiaryType; gO101TailBA34OrderCountCall1 = orderIndex + 1;

    gO101TailBA34Nodes24[gO101TailBA34Node24Count].x = 0x66; gO101TailBA34Nodes24[gO101TailBA34Node24Count].y = 0x10; length = overlay101ByteLength(gO101TailBA34Inputs.text19C); gO101TailBA34Nodes24[gO101TailBA34Node24Count].length = (u8)length; gO101TailBA34Nodes24[gO101TailBA34Node24Count].opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1); gO101TailBA34Nodes24[gO101TailBA34Node24Count].mode = 2; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color0 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color1 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color2 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color3 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].kind = 4; gO101TailBA34Nodes24[gO101TailBA34Node24Count].text = gO101TailBA34Inputs.text19C; gO101TailBA34Nodes24[gO101TailBA34Node24Count].previousType = gO101TailBA34Root.tertiaryType; gO101TailBA34Nodes24[gO101TailBA34Node24Count].previous = gO101TailBA34Root.tertiary; gO101TailBA34Root.tertiaryType = 3; gO101TailBA34Root.tertiary = &gO101TailBA34Nodes24[gO101TailBA34Node24Count]; gO101TailBA34Node24Count = gO101TailBA34Node24Count + 1;

    gO101TailBA34Nodes24[gO101TailBA34Node24Count].x = 0x66; gO101TailBA34Nodes24[gO101TailBA34Node24Count].y = 0x1E; length = overlay101ByteLength(gO101TailBA34Inputs.text1A0); gO101TailBA34Nodes24[gO101TailBA34Node24Count].length = (u8)length; gO101TailBA34Nodes24[gO101TailBA34Node24Count].opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1); gO101TailBA34Nodes24[gO101TailBA34Node24Count].mode = 2; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color0 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color1 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color2 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color3 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].kind = 4; gO101TailBA34Nodes24[gO101TailBA34Node24Count].text = gO101TailBA34Inputs.text1A0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].previousType = gO101TailBA34Root.tertiaryType; gO101TailBA34Nodes24[gO101TailBA34Node24Count].previous = gO101TailBA34Root.tertiary; gO101TailBA34Root.tertiaryType = 3; gO101TailBA34Root.tertiary = &gO101TailBA34Nodes24[gO101TailBA34Node24Count]; gO101TailBA34Node24Count = gO101TailBA34Node24Count + 1;

    gO101TailBA34Nodes24[gO101TailBA34Node24Count].x = 0x66; gO101TailBA34Nodes24[gO101TailBA34Node24Count].y = 0x28; length = overlay101ByteLength(gO101TailBA34Inputs.text1A4); gO101TailBA34Nodes24[gO101TailBA34Node24Count].length = (u8)length; gO101TailBA34Nodes24[gO101TailBA34Node24Count].opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1); gO101TailBA34Nodes24[gO101TailBA34Node24Count].mode = 2; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color0 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color1 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color2 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color3 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].kind = 4; gO101TailBA34Nodes24[gO101TailBA34Node24Count].text = gO101TailBA34Inputs.text1A4; gO101TailBA34Nodes24[gO101TailBA34Node24Count].previousType = gO101TailBA34Root.tertiaryType; gO101TailBA34Nodes24[gO101TailBA34Node24Count].previous = gO101TailBA34Root.tertiary; gO101TailBA34Root.tertiaryType = 3; gO101TailBA34Root.tertiary = &gO101TailBA34Nodes24[gO101TailBA34Node24Count]; gO101TailBA34Node24Count = gO101TailBA34Node24Count + 1;

    gO101TailBA34Nodes24[gO101TailBA34Node24Count].x = 0x66; gO101TailBA34Nodes24[gO101TailBA34Node24Count].y = 0x32; length = overlay101ByteLength(gO101TailBA34Inputs.text1A8); gO101TailBA34Nodes24[gO101TailBA34Node24Count].length = (u8)length; gO101TailBA34Nodes24[gO101TailBA34Node24Count].opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1); gO101TailBA34Nodes24[gO101TailBA34Node24Count].mode = 2; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color0 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color1 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color2 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color3 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].kind = 4; gO101TailBA34Nodes24[gO101TailBA34Node24Count].text = gO101TailBA34Inputs.text1A8; gO101TailBA34Nodes24[gO101TailBA34Node24Count].previousType = gO101TailBA34Root.tertiaryType; gO101TailBA34Nodes24[gO101TailBA34Node24Count].previous = gO101TailBA34Root.tertiary; gO101TailBA34Root.tertiaryType = 3; gO101TailBA34Root.tertiary = &gO101TailBA34Nodes24[gO101TailBA34Node24Count]; gO101TailBA34Node24Count = gO101TailBA34Node24Count + 1;

    gO101TailBA34Nodes24[gO101TailBA34Node24Count].x = 0x66; gO101TailBA34Nodes24[gO101TailBA34Node24Count].y = 0x3C; length = overlay101ByteLength(gO101TailBA34Inputs.text1AC); gO101TailBA34Nodes24[gO101TailBA34Node24Count].length = (u8)length; gO101TailBA34Nodes24[gO101TailBA34Node24Count].opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1); gO101TailBA34Nodes24[gO101TailBA34Node24Count].mode = 2; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color0 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color1 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color2 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].color3 = 0; gO101TailBA34Nodes24[gO101TailBA34Node24Count].kind = 4; gO101TailBA34Nodes24[gO101TailBA34Node24Count].text = gO101TailBA34Inputs.text1AC; gO101TailBA34Nodes24[gO101TailBA34Node24Count].previousType = gO101TailBA34Root.tertiaryType; gO101TailBA34Nodes24[gO101TailBA34Node24Count].previous = gO101TailBA34Root.tertiary; gO101TailBA34Root.tertiaryType = 3; gO101TailBA34Root.tertiary = &gO101TailBA34Nodes24[gO101TailBA34Node24Count]; gO101TailBA34Node24Count = gO101TailBA34Node24Count + 1;

    o101TailBA34FinalizerReloc(&gO101TailBA34FinalObject4630);
}

