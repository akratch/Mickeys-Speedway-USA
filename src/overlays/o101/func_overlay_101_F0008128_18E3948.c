#include "PR/ultratypes.h"

typedef struct Root {
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
} Root;

typedef struct Node32 {
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
} Node32;

typedef struct Node24 {
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
    void *text;
} Node24;

typedef struct Inputs {
    u8 pad00[0xF8];
    void *dataF8;
    u8 padFC[0xC];
    void *text108;
    void *text10C;
    void *text110;
} Inputs;

extern Root D_0;
extern Inputs D_INPUT;
extern void *D_1C;
extern void *D_38;
extern u32 D_F4;
extern void *D_CF0;
extern void *D_2934;
extern void *D_1C0[];
extern s32 D_1C4;
extern s32 D_1CC;
extern s32 D_1D0;
extern Node32 D_340[];
extern Node24 D_540[];

extern void *func_overlay_101_F0000000_18DB820();
extern s8 func_overlay_101_F000CEA8_18E86C8(void *);

/* Ownership: the shared builder BSS (+0x0..+0xFD0) is defined by the F571C
 * TU; this consumer uses that owner for the root and node pools. */
/* Four source-shape levers took this family from 461 masked words to 311, and
 * closed the missing callee-saved web that every earlier note named as the gate.
 *
 * L59 -- the three non-macro assignment groups are ONE physical line each. With
 *   the stores on separate lines as1's `lineno` key emits them in source order;
 *   the ROM emits each group reversed, and folding retires the key so the raw
 *   ready-list order supplies the reversal. The macro bodies need no fold: a
 *   multi-line macro expansion already carries the invocation's line.
 *   The same reading fixes the root header's order: the ROM emits height, width,
 *   kind, so the source writes kind, width, height.
 * L115/L101 -- the node-32 pointer is USED after the call, before the counter is
 *   re-read. `node32->handle`, `->previousType` and `->previous` all store
 *   through the pre-call pointer (the same address the re-read recomputes), so
 *   the symbol's live range spans the call, v0 costs more than a save/restore
 *   pair, and the web takes s0. That is the whole of the missing sixth web: the
 *   frame ladder goes from eight slots to the ROM's nine, the float homes move
 *   from 0x18/0x20 to the ROM's 0x10/0x18, and s0..s5 land as the ROM has them
 *   (node pointer / text index, &D_0, the counter address, the pool address,
 *   0xFF and 0x80, the literal 2). Measured with the instrumented uopt: before
 *   the edit the node-pointer web decides `bestcost=0.0 bestreg=v0`; after it,
 *   `color=14 reg=s0`. Worth 78 masked words and the whole -4 size delta.
 * L110 -- the text pointers are fields of the shared input block, not separate
 *   globals. One `lui`+`addiu` base then two displaced loads is what the ROM
 *   emits; three independent `lui`/`lw` pairs is what separate externs give.
 *   Worth 11 words and the last mnemonic-census difference.
 *
 * Refuted here, each measured: `register volatile Node32 *` was recorded as the
 *   only spelling reaching 461 and worth eight words over every other; on the
 *   corrected shape it is the WORST, 405 against plain `Node32 *` at 386, and
 *   `register` alone is inert either way. Merging the counter locals (the L100
 *   partition that paid on the F63F8 relative) regresses: {owner,node} 461,
 *   {owner,text} 513, all three 483, against 456 for three separate locals.
 * Measured inert (p1-only by the call test): statement order inside either
 *   macro, the order of the twelve stores in the root's second group (311-316
 *   over six orders including the full reversal), and the order of the three
 *   pre-`d` stores in the post-call sequence.
 * Remaining: the instruction multiset and the frame are exact; 215 of the 311
 *   are register naming and the rest is schedule order. */
#ifdef NON_MATCHING
void func_overlay_101_F0008128_18E3948(void) {
    Node32 *node32;
    Node24 *node24;
    void *handle;
    s32 length;
    s32 nodeIndex;
    s32 ownerIndex;
    s32 textIndex;

    D_0.kind = 4; D_0.width2E = 0x140; D_0.height30 = 0xF0; D_0.asset34 = &D_CF0; D_0.color32 = 0xFF; D_0.color33 = 0xFF; D_0.value26 = 0; D_0.value28 = 0; D_0.value2A = 0; D_0.value2C = 0; D_0.chainType = 0; D_0.chain = 0; ownerIndex = D_1C4; D_1C0[ownerIndex] = &D_1C; D_1C4 = ownerIndex + 1;

    node32 = &D_340[D_1CC]; node32->x = 0xF2; node32->y = 0x14E; node32->value10 = 0; node32->color12 = 0xFF; node32->color13 = 0; node32->value18 = 0; node32->scale = 1.0f; node32->value14 = 0.0f; handle = func_overlay_101_F0000000_18DB820(0x93, 0); node32->handle = handle; node32->previousType = D_0.chainType; node32->previous = D_0.chain; nodeIndex = D_1CC; node32 = &D_340[nodeIndex]; D_0.chainType = 2; D_0.chain = node32; D_1CC = nodeIndex + 1;

    D_0.x42 = 0x20; D_0.width44 = 0x18; D_0.y46 = 0x20; D_0.height48 = 0x14; D_0.value4A = 0x100; D_0.value4C = 0xB4; D_0.mode40 = 0; D_0.color4E = 0xFF; D_0.color4F = 0xFF; D_0.childType = 0; D_0.child = 0; D_0.data50 = D_INPUT.dataF8; ownerIndex = D_1C4; D_1C0[ownerIndex] = &D_38; D_1C4 = ownerIndex + 1;

#define ADD_IMAGE_NODE(nodeX, nodeY, imageId)                                \
    node32 = &D_340[D_1CC];                                                  \
    node32->x = (nodeX);                                                     \
    node32->y = (nodeY);                                                     \
    node32->scale = 1.0f;                                                    \
    node32->value10 = 0;                                                     \
    node32->color12 = 0xFF;                                                  \
    node32->color13 = 0;                                                     \
    node32->value14 = 0.0f;                                                  \
    node32->value18 = 0;                                                     \
    handle = func_overlay_101_F0000000_18DB820((imageId), 0);                 \
    node32->handle = handle;                                                 \
    node32->previousType = D_0.childType;                                    \
    node32->previous = D_0.child;                                            \
    nodeIndex = D_1CC;                                                       \
    node32 = &D_340[nodeIndex];                                              \
    D_0.childType = 2;                                                       \
    D_0.child = node32;                                                      \
    D_1CC = nodeIndex + 1

    if (((D_F4 << 5) >> 28) & 1) {
        ADD_IMAGE_NODE(0x26, 0x2E, 0x97);
    } else {
        ADD_IMAGE_NODE(0x26, 0x2E, 0x98);
    }
    if (((D_F4 << 5) >> 28) & 2) {
        ADD_IMAGE_NODE(0x68, 0x2E, 0x99);
    } else {
        ADD_IMAGE_NODE(0x68, 0x2E, 0x9A);
    }
    if (((D_F4 << 5) >> 28) & 4) {
        ADD_IMAGE_NODE(0x26, 0x70, 0x9D);
    } else {
        ADD_IMAGE_NODE(0x26, 0x70, 0x9E);
    }
    if (((D_F4 << 5) >> 28) & 8) {
        ADD_IMAGE_NODE(0x68, 0x70, 0x9B);
    } else {
        ADD_IMAGE_NODE(0x68, 0x70, 0x9C);
    }

#undef ADD_IMAGE_NODE

#define ADD_TEXT_ROW(field, rowY)                                            \
    textIndex = D_1D0;                                                       \
    node24 = &D_540[textIndex];                                              \
    node24->x = 0x80;                                                        \
    node24->y = (rowY);                                                      \
    length = func_overlay_101_F000CEA8_18E86C8(field);                       \
    textIndex = D_1D0;                                                       \
    node24 = &D_540[textIndex];                                              \
    node24->length = (u8)length;                                             \
    node24->opacity =                                                        \
        (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1);                  \
    node24->mode = 2;                                                        \
    node24->color0 = 0;                                                      \
    node24->color1 = 0;                                                      \
    node24->color2 = 0;                                                      \
    node24->color3 = 0;                                                      \
    node24->kind = 4;                                                        \
    node24->text = (field);                                                  \
    node24->previousType = D_0.childType;                                    \
    node24->previous = D_0.child;                                            \
    D_0.childType = 3;                                                       \
    D_0.child = node24;                                                       \
    D_1D0 = textIndex + 1

    ADD_TEXT_ROW(D_INPUT.text108, 0x92);
    ADD_TEXT_ROW(D_INPUT.text10C, 0x9C);
    ADD_TEXT_ROW(D_INPUT.text110, 0xA6);

#undef ADD_TEXT_ROW

    func_overlay_101_F0000000_18DB820(&D_2934);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/func_overlay_101_F0008128_18E3948/func_overlay_101_F0008128_18E3948.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_101_F0008128_18E3948:start
 * symbol: func_overlay_101_F0008128_18E3948
 * score: 311/525 words
 * frame: 0x40
 * relocations: 59
 * first-mismatch: +0xA8
 * summary: 311 masked words from 461; size, frame and instruction multiset all exact. Residual is 215 register naming and 111 schedule order.
 * PLATEAU-HANDOFF:func_overlay_101_F0008128_18E3948:end
 */
