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
    s32 secondChildType;
    void *secondChild;
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

typedef struct Node20 {
    s32 previousType;
    void *previous;
    s16 x;
    s16 y;
    f32 scale;
    void *handle;
} Node20;

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
    u8 pad00[0x98];
    void *data98;
    void *data9C;
    void *textA0;
    void *textA4;
    void *textA8;
    void *textAC;
} Inputs;

extern Root D_0;
extern Inputs D_INPUT;
extern void *D_1C;
extern void *D_38;
extern void *D_54;
extern void *D_CA0;
extern void *D_1514;
extern void *D_1C0[];
extern s32 D_1C4;
extern s32 D_1C8;
extern s32 D_1CC;
extern s32 D_1D0;
extern Node20 D_200[];
extern Node32 D_340[];
extern Node24 D_540[];

extern void *func_overlay_101_F0000000_18DB820();
extern s32 func_overlay_101_F000CEA8_18E86C8(void *);

/* Four source-shape levers took this family from 296/298 masked words to
 * 130/136. None costs an instruction.
 * L59 -- every per-element assignment group is ONE physical line. as1 minimises
 *   (start_time, -aftercycles, -latency, addr, lineno, list position); with a
 *   group's stores on separate lines `lineno` is the deciding key and orders them
 *   in source order, while the ROM emits that group reversed. Folding the group
 *   retires the key, the raw list order supplies the reversal, and the prologue
 *   goes byte-exact. Worth 26 words and 3 of the 25 insertion sites on its own.
 * L100/L115 -- `index` carries the panel-order counter D_1C4 and the text-row
 *   counter D_1D0. That is what puts `index` in the ROM's callee-saved home with
 *   the ROM's save placement; one shared counter gives that home to the node
 *   counter instead and rotates the ugen ring for the rest of the function.
 * L115/L101 -- the two node blocks read their counter global DIRECTLY in the
 *   pointer expression, on BOTH sides of the call, and the `slot` local is gone.
 *   This CORRECTS what this comment used to say. It recorded "dropping `slot`
 *   for a direct counter read anywhere" as regressive, and each half alone does
 *   regress -- pre-call only and post-call only are both far worse. Doing both
 *   halves together is not: it reads 197 against 211. The pre-call direct read
 *   turns the pre-call counter from a globalcolor web into a ugen ring temp,
 *   which is what the ROM has, and because both definitions of the node pointer
 *   are then the same expression the pointer keeps the ROM's callee-saved home
 *   instead of splitting. The bump must sit immediately after the recomputed
 *   pointer; behind any store it makes uopt re-read the counter.
 * The byte-length local is `u8`, not `s32`. Worth 197 to 130, the largest single
 *   lever on the function, and it is semantically exact rather than lucky: both
 *   uses already truncate to eight bits -- the node field is `u8` and the opacity
 *   expression masks with 0xFF -- so the declaration only tells the compiler what
 *   the code already guarantees, and the mask becomes redundant. A 360-cell
 *   lattice over the callee's declared return type, the local's type, the store
 *   cast and the opacity mask puts every floor cell at LOCAL `u8`, with the
 *   return type indifferent across all six spellings. The naming bucket falls
 *   169 to 36 on this edit: the pre-edit residual splits into ten windows whose
 *   sizes repeat 14 then 7 once per text row, a per-iteration ring-consumption
 *   difference, and after it what is left is one clean ring shift.
 * Exhausted, with what was varied. The counter-bump STORE: 659 constrained
 *   permutations of the whole post-call block -- the bump, the three node stores
 *   and the two root stores under their two real dependences -- across four bump
 *   spellings, all flat. The block order was re-measured at the post-u8 plateau
 *   and the adopted form is still the floor, so it is not a stale result.
 * Remaining, 130 masked: 279 byte-exact, 36 naming, 2 immediate, 79 schedule,
 *   size and frame ladder both exact. The naming rows are one PHASE: the ring
 *   runs exactly one position behind from +0x2BC onward, ours t2 against their
 *   t3, t3-t4, t6-t7, t7-t8, t8-t9 and t9 back to t2, with 83 percent of all
 *   substitution pairs consistent with that single permutation. That is L127's
 *   shape, and L127 does NOT reach it here: 155 no-op cells were measured --
 *   every read of the four counter globals and the loop index wrapped with
 *   OR-zero, AND-minus-one, XOR-zero, a byte mask and two doubled forms, then
 *   every read of a root struct field and of the call result, pointers included
 *   through an integer round trip -- and all 155 are exactly flat. So either the
 *   phase is set before any point a source no-op reaches, or these rows are
 *   globalcolor colour, which L114 as corrected for this procedure permits since
 *   it assigns three t-bank colours outright. Decide that from the `p1color`
 *   records before spending another no-op lattice. */
#ifdef NON_MATCHING
void func_overlay_101_F0005E08_18E1628(void) {
    s32 index;
    u8 length;
    void *handle;
    Node20 *node20;
    Node24 *node24;
    Node32 *node32;

    D_0.kind = 4; D_0.width2E = 0x140; D_0.height30 = 0xF0; D_0.asset34 = &D_CA0; D_0.color32 = 0xFF; D_0.color33 = 0xFF; D_0.value26 = 0; D_0.value28 = 0; D_0.value2A = 0; D_0.value2C = 0; D_0.chainType = 0; D_0.chain = 0; index = D_1C4; D_1C0[index] = &D_1C; D_1C4 = index + 1;

    node32 = &D_340[D_1CC]; node32->x = 0xF2; node32->y = 0x14E; node32->value10 = 0; node32->color12 = 0xFF; node32->color13 = 0; node32->value18 = 0; node32->scale = 1.0f; node32->value14 = 0.0f; handle = func_overlay_101_F0000000_18DB820(0x93, 0); node32 = &D_340[D_1CC]; D_1CC = D_1CC + 1; node32->previous = D_0.chain; node32->handle = handle; node32->previousType = D_0.chainType; D_0.chainType = 2; D_0.chain = node32;

    index = D_1C4; D_0.x42 = 0x20; D_0.width44 = 0x18; D_0.y46 = 0x5A; D_0.height48 = 0x20; D_0.value4A = 0x4C; D_0.value4C = 0x54; D_0.mode40 = 0; D_0.color4E = 0xFF; D_0.color4F = 0xFF; D_0.childType = 0; D_0.child = 0; D_0.data50 = D_INPUT.data98; D_1C0[index] = &D_38; D_1C4 = index + 1;

    node20 = &D_200[D_1C8]; node20->x = 6; node20->y = 0xE; node20->scale = 1.0f; handle = func_overlay_101_F0000000_18DB820(7); node20 = &D_200[D_1C8]; D_1C8 = D_1C8 + 1; node20->previous = D_0.child; node20->handle = handle; node20->previousType = D_0.childType; D_0.childType = 1; D_0.child = node20;

    index = D_1C4; D_0.x5E = 0x20; D_0.width60 = 0x40; D_0.y62 = 0x20; D_0.height64 = 0x78; D_0.value66 = 0xC0; D_0.value68 = 0x46; D_0.mode5C = 0; D_0.color6A = 0xFF; D_0.color6B = 0xFF; D_0.secondChildType = 0; D_0.secondChild = 0; D_0.data6C = D_INPUT.data9C; D_1C0[index] = &D_54; D_1C4 = index + 1;

#define ADD_TEXT_ROW(field, rowY)                                            \
    index = D_1D0;                                                          \
    node24 = &D_540[index];                                                  \
    node24->x = 0x60;                                                        \
    node24->y = (rowY);                                                      \
    length = func_overlay_101_F000CEA8_18E86C8(D_INPUT.field);               \
    index = D_1D0;                                                          \
    node24 = &D_540[index];                                                  \
    node24->length = (u8)length;                                             \
    node24->opacity =                                                       \
        (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1);                 \
    node24->mode = 2;                                                        \
    node24->color0 = 0;                                                      \
    node24->color1 = 0;                                                      \
    node24->color2 = 0;                                                      \
    node24->color3 = 0;                                                      \
    node24->kind = 4;                                                        \
    node24->text = D_INPUT.field;                                            \
    node24->previousType = D_0.secondChildType;                              \
    node24->previous = D_0.secondChild;                                      \
    D_0.secondChildType = 3;                                                 \
    D_0.secondChild = node24;                                                \
    D_1D0 = index + 1

    ADD_TEXT_ROW(textA0, 0x10);
    ADD_TEXT_ROW(textA4, 0x1E);
    ADD_TEXT_ROW(textA8, 0x28);
    ADD_TEXT_ROW(textAC, 0x36);

#undef ADD_TEXT_ROW

    func_overlay_101_F0000000_18DB820(&D_1514);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/func_overlay_101_F0005E08_18E1628/func_overlay_101_F0005E08_18E1628.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_101_F0005E08_18E1628:start
 * symbol: func_overlay_101_F0005E08_18E1628
 * score: 130/380 words
 * frame: 0x38
 * relocations: 49
 * first-mismatch: +0x9C
 * summary: 130 masked words; the byte-length local is u8, which collapsed the naming bucket by more than four to one and left a clean one-position ring shift from +0x2BC.
 * PLATEAU-HANDOFF:func_overlay_101_F0005E08_18E1628:end
 */
