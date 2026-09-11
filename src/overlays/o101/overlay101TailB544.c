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
    u8 pad54[0x130];
    void *text184;
    void *text188;
    void *text18C;
    void *text190;
    void *text194;
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
    u8 pad000[0x184];
    void *data184;
    void *text188;
    void *text18C;
    void *text190;
    void *text194;
} Inputs;

extern Root D_0;
extern Inputs D_INPUT;
extern void *D_1C;
extern void *D_38;
extern void *D_DA4;
extern void *D_4358;
extern void *D_1C0[];
extern s32 D_1C4;
extern s32 D_1CC;
extern s32 D_1D0;
extern Node32 D_340[];
extern Node24 D_540[];

extern void *func_overlay_101_F0000000_18DB820();
extern s32 func_overlay_101_F000CEA8_18E86C8(void *);

/* Ownership: the shared builder BSS (+0x0..+0xFD0) is defined by the F571C TU;
 * this consumer uses that owner for the root and node pools. */
/* 189 -> 158 masked on 2026-09-11 by importing the quadruplet's levers plus a
 * statement-order climb. Size, frame and the nine-slot frame ladder are exact.
 *
 * L115/L101 -- READ THE COUNTER GLOBAL DIRECTLY IN THE POINTER EXPRESSION. This
 *   TU never had the lever the four sibling builders carry. Spelling the node
 *   group's PRE-call read `node32 = &D_340[D_1CC]` instead of `index = D_1CC;
 *   node32 = &D_340[index]` is worth 189 to 184 on its own: with the index
 *   local, uopt forward-substitutes and the address becomes its own web that
 *   takes a colour; reading the global inside the expression leaves it in the
 *   temp ring where the ROM has it. The POST-call read must keep the index
 *   local -- making it direct as well reads 185, and making the post-call read
 *   direct alone reads 201.
 * The same lever on the text rows is per row, not global. Rows 3 and 4 pay
 *   (184 to 174 together); row 1 costs 119 and row 2 costs 2, so applying it to
 *   all four reads 294. Each row was measured separately and then in all
 *   sixteen combinations.
 * Statement order over seven blocks -- the two root headers, the chain group
 *   and the four text rows -- is worth 174 to 158 by pairwise-swap hill
 *   climbing with four random restarts. The climb also joins the two-line
 *   `opacity` statement onto one physical line in the unfolded rows, which is
 *   an L59 fold and part of the win.
 * Measured regressive: splitting the one shared `index` local into separate
 *   node and text counters reads 285 with a -16 size delta, which agrees with
 *   the siblings' finding that merging or splitting the counter locals is not
 *   free.
 * Remaining, 158 masked: 177 byte-exact, 80 register naming, 1 immediate only,
 *   71 really different. Unlike the four siblings the instruction multiset was
 *   never exact here -- it is 8 words out at 189 and 10 at 158 -- so a size or
 *   multiset claim about this function has to be re-derived, not inherited from
 *   them. The dependence-graph closure argued in the siblings' note (as1 orders
 *   every memory reference after a preceding store with a different base
 *   register, so the ROM's both-loads-first node block is infeasible without a
 *   carried old-link value, and every carrier spelling is coloured) applies
 *   here too and was not re-measured on this TU. */
#ifdef NON_MATCHING
void func_overlay_101_F000B544_18E6D64(void) {
    s32 index;
    s32 length;
    void *handle;
    Node32 *node32;
    Node24 *node24;

    D_0.kind = 4; D_0.value26 = 0; D_0.value28 = 0; D_0.value2A = 0; D_0.color32 = 0xFF; D_0.color33 = 0xFF; D_0.width2E = 0x140; D_0.value2C = 0; D_0.chainType = 0; D_0.height30 = 0xF0; D_0.asset34 = &D_DA4; D_0.chain = 0; index = D_1C4; D_1C0[index] = &D_1C; D_1C4 = index + 1;

    node32 = &D_340[D_1CC]; node32->x = 0x108; node32->scale = 1.0f; node32->value10 = 0; node32->color12 = 0xFF; node32->value18 = 0; node32->color13 = 0; node32->y = 0x154; node32->value14 = 0.0f; handle = func_overlay_101_F0000000_18DB820(0x91, 0); index = D_1CC; node32 = &D_340[index]; node32->previousType = D_0.chainType; node32->previous = D_0.chain; node32->handle = handle; D_0.chainType = 2; D_0.chain = node32; D_1CC = index + 1;

    index = D_1C4; D_0.x42 = 0x20; D_0.width44 = 0x18; D_0.y46 = 0x1C; D_0.height48 = 0x50; D_0.value4A = 0xD8; D_0.value4C = 0x42; D_0.mode40 = 0; D_0.color4E = 0xFF; D_0.color4F = 0xFF; D_0.childType = 0; D_0.child = 0; D_0.data50 = D_INPUT.data184; D_1C0[index] = &D_38; D_1C4 = index + 1;

    index = D_1D0; node24 = &D_540[index]; node24->y = 0x10; node24->x = 0x6C; length = func_overlay_101_F000CEA8_18E86C8(D_INPUT.text188); index = D_1D0; node24 = &D_540[index]; node24->length = (u8)length; node24->opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(u32)0); node24->text = D_INPUT.text188; node24->mode = 2; node24->color0 = 0xFF; node24->color2 = 0; node24->color3 = 0xFF; node24->color1 = 0x80; node24->kind = 4; node24->previousType = D_0.childType; node24->previous = D_0.child; D_1D0 = index + 1; D_0.child = node24; D_0.childType = 3;

    index = D_1D0;
    node24 = &D_540[index];
    node24->y = 0x1E;
    node24->x = 0x6C;
    length = func_overlay_101_F000CEA8_18E86C8(D_INPUT.text18C);
    index = D_1D0;
    node24 = &D_540[index];
    node24->length = (u8)length;
    node24->opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(u32)0);
    node24->color2 = 0;
    node24->color0 = 0xFF;
    node24->color1 = 0xFF;
    node24->previous = D_0.child;
    node24->color3 = 0xFF;
    node24->previousType = D_0.childType;
    node24->text = D_INPUT.text18C;
    node24->kind = 4;
    node24->mode = 2;
    D_0.childType = 3;
    D_0.child = node24;
    D_1D0 = index + 1;

    node24 = &D_540[D_1D0];
    node24->y = 0x28;
    node24->x = 0x6C;
    length = func_overlay_101_F000CEA8_18E86C8(D_INPUT.text190);
    index = D_1D0;
    node24 = &D_540[index];
    node24->length = (u8)length;
    node24->opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(u32)0);
    node24->color2 = 0;
    node24->color0 = 0xFF;
    node24->color1 = 0xFF;
    node24->previousType = D_0.childType;
    node24->color3 = 0xFF;
    node24->previous = D_0.child;
    node24->text = D_INPUT.text190;
    node24->kind = 4;
    node24->mode = 2;
    D_0.childType = 3;
    D_0.child = node24;
    D_1D0 = index + 1;

    node24 = &D_540[D_1D0]; node24->x = 0x6C; node24->y = 0x32; length = func_overlay_101_F000CEA8_18E86C8(D_INPUT.text194); index = D_1D0; node24 = &D_540[index]; node24->length = (u8)length; node24->opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(u32)0); node24->mode = 2; node24->color0 = 0xFF; node24->color1 = 0xFF; node24->color2 = 0; node24->color3 = 0xFF; node24->previousType = D_0.childType; node24->text = D_INPUT.text194; node24->kind = 4; node24->previous = D_0.child; D_0.childType = 3; D_0.child = node24; D_1D0 = index + 1;

    func_overlay_101_F0000000_18DB820(&D_4358);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/overlay101TailB544/func_overlay_101_F000B544_18E6D64.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_101_F000B544_18E6D64:start
 * symbol: func_overlay_101_F000B544_18E6D64
 * score: 158/316 words
 * frame: 0x38
 * relocations: 38
 * first-mismatch: +0x9C
 * summary: 158 masked from 189 at exact size and frame; the pre-call direct counter read, per-row text folds and a seven-block statement-order climb. Multiset was never exact here.
 * PLATEAU-HANDOFF:func_overlay_101_F000B544_18E6D64:end
 */
