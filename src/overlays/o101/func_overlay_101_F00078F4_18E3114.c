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
    void *textFC;
    void *text100;
    void *text104;
} Inputs;

extern Root D_0;
extern Inputs D_INPUT;
extern void *D_1C;
extern void *D_38;
extern u32 D_F4;
extern void *D_CDC;
extern void *D_2608;
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
/* Seven source-shape levers took this family from 461 masked words to 136.
 *
 * L59 -- the three non-macro assignment groups are ONE physical line each. With
 *   the stores on separate lines as1's `lineno` key emits them in source order;
 *   the ROM emits each group reversed, and folding retires the key so the raw
 *   ready-list order supplies the reversal.
 *   The same reading fixes the root header's order: the ROM emits height, width,
 *   kind, so the source writes kind, width, height.
 *   NARROWED here: the key does not exist inside a macro. Folding the image
 *   macro's post-call stores onto one continuation line, folding its pre-call
 *   stores, and folding both, each leave the object byte-identical -- a
 *   multi-line macro expansion already carries the invocation's line for every
 *   statement in it, so L59 has nothing to retire there. Do not spend a sweep.
 * L115/L101 -- THE RECOMPUTE READS THE COUNTER GLOBAL DIRECTLY. `node32 =
 *   &D_340[D_1CC]` on BOTH sides of the call leaves the address in the symbol
 *   `node32`, so uopt forms one live range per symbol (L115), that range spans
 *   the call, every caller-saved colour prices 18.0 against 6.5 for s0, and both
 *   the pre-call and the post-call definition land in the ROM's s0. Spelling the
 *   recompute through an index local -- `nodeIndex = D_1CC; node32 =
 *   &D_340[nodeIndex]` -- lets uopt forward-substitute the address into each
 *   field store instead: the post-call value becomes its own expression web that
 *   crosses no call, prices every caller-saved colour at 0.0, and takes a ring
 *   register, and s0 is not even offered to it because the pre-call symbol web
 *   already holds it. With the direct read the whole callee-saved bank falls
 *   into the ROM's layout at once: s0 node pointer, s1 &D_0, s2 the counter
 *   address, s3 the pool address, s4 0xFF, s5 the literal 2. Worth 311 to 208.
 * The counter bump must not sit behind a store. `D_1CC = D_1CC + 1` placed after
 *   any store makes uopt re-read the counter -- one extra load in each of the
 *   eight groups, +36 bytes. `nodeIndex = D_1CC` written immediately after the
 *   pointer expression is CSE'd onto the same load for free and lets the bump
 *   sit anywhere; that is the only thing the index local is for now.
 * L110 -- the text pointers are fields of the shared input block, not separate
 *   globals. One base register then two displaced loads is what the ROM emits;
 *   three independent high/low pairs is what separate externs give.
 * Statement order in the image macro's post-call block is worth 208 to 189, and
 *   it was RE-MEASURED here on the 136 shape over all 630 orders of its seven
 *   statements that respect the three read-before-write dependences. The
 *   incumbent order is the unique optimum and the second best is +8, so that
 *   lattice is now closed on two different shapes. Do not re-run it.
 *
 * THE TEXT ROWS. A UOPT LIVE RANGE IS FORMED PER IR NAME, AND AN EXPRESSION IS
 *   A NAME. L115 says a live range is formed per symbol; the lineage_member
 *   records of the instrumented uopt show the same holds for an expression.
 *   Every occurrence of one address expression in the procedure joins ONE range,
 *   whatever block it sits in and whatever value it carries. All three text rows
 *   spelled their pre-call pointer `&D_540[D_1D0]`, so those three occurrences
 *   were a single range whose hull covers all three rows and therefore two
 *   calls. That is why its cost list priced every caller-saved colour at 4.0
 *   while reusing the already-saved s0 cost 0.0, and why it took s0 where the
 *   ROM has v1. Removing the pointer local does NOT split it -- the expression
 *   is still one name -- which is the part that is easy to get wrong.
 * REFUTED, with the record that breaks it. The previous note read
 *   `available0=0x00020000` as "every caller-saved colour is forbidden to it"
 *   and concluded "the lever is that interference set, not the save ratio". The
 *   cost list under CDX_DETAIL_WEB on that web offers c4 through c13 at 4.0
 *   each: they were not forbidden, they were outbid. `available0` is the
 *   post-decision consistency mask, not the offer set, and reading it as the
 *   offer set sent a lane after an interference that was never the blocker.
 *   Only v0, v1, a0 and s1 are genuinely forbidden there, by the four webs that
 *   hold them and overlap the hull. The save ratio is exactly the lever: the
 *   pointer range's 4.5 beats each index local's 3.0, so it is coloured first
 *   and takes the s0 the ROM gives the index.
 * THE EDIT, worth 8 words. Spell the pre-call stores through the array rather
 *   than through a pointer local -- `D_540[D_1D0].x = 0x80` instead of
 *   `node24 = &D_540[D_1D0]; node24->x = 0x80`. A pointer local is a symbol, a
 *   symbol is coloured from the procedure's table or given a stack home, and the
 *   ROM's pre-call base is a ring temporary; only an expression can land there.
 *   The post-call pointer may be spelled either way -- both read 136 -- because
 *   uopt forward-substitutes it into the field stores whichever way it is
 *   written.
 * THE INDEX LOCALS, worth 4 words, and pure L115. Which declared local each row
 *   reuses for its post-call index decides the colouring: three fresh locals
 *   read 145, reusing `ownerIndex` in row 1 and `nodeIndex` in row 2 reads 141,
 *   because reusing a local that is already live elsewhere imports its
 *   interference at zero width and no instruction. Exhaustive over 343
 *   assignments from a seven-local pool and re-run after the tails were
 *   re-climbed; five assignments tie at the optimum and every one of them puts
 *   ownerIndex in row 1.
 * THE TAILS, worth 5 words. The three rows' tail orders were an optimum OF THE
 *   149 SHAPE and had to be re-climbed once the shape changed: rows 2 and 3 want
 *   plain ascending field order, row 1 wants color2 hoisted above mode. A
 *   statement-order optimum is never portable across a shape change, and the
 *   previous note lost a measurement to exactly that.
 *
 * Remaining, 136 masked (was 145): size, frame and the nine-slot frame ladder
 *   are exact; 422 byte-exact, 23 register naming, 1 immediate, 95 really
 *   different, and the register mapping is 100% coherent with no closed cycle.
 *   By region: prologue and the first root header 10 words; the chain group and
 *   the second root header 21; the eight image node groups 56; the three text
 *   rows 37.
 *
 * NEXT, and it is two named questions rather than one.
 *  (a) The text rows' 23 naming rows are one colour rotation: ours takes s0
 *   where the ROM has v1 (7 rows), a1 where it has s0 (7), a3 where it has a2
 *   (5), a2 where it has s0 (3). The ROM's layout is the index in s0 and the
 *   pointer in v1; ours is the inverse. A source shape that reaches the ROM's
 *   layout exactly is ALREADY KNOWN: read the counter into one shared index
 *   local before the call as well as after it, the same local in all three rows,
 *   every store through `D_540[thatLocal]`. That shape reads 183, and the reason
 *   is measured rather than guessed -- it carries the ROM's colouring AND the
 *   ROM's schedule, one temp-ring position behind, because the pre-call counter
 *   read then lands in a coloured symbol and draws no ring position (L129).
 *   Recovering one ring draw at the head of the text region without re-splitting
 *   those ranges is the whole remaining question here, and it is worth about 37
 *   words. The nearest measured point is that shape with row 1 alone reverted to
 *   the direct read: 160, and ROM-exact through row 1's first store group except
 *   that row 1's pre-call pointer then becomes its own range and takes v0 rather
 *   than v1 -- L101 withholds v0 only from a range that reaches a call result.
 *  (b) The eight image node groups are a SCHEDULE residual, not a colouring one:
 *   every register in them already agrees with the ROM. The ROM issues both
 *   old-link loads back to back before it computes the node address; ours puts
 *   the first link store between them. Statement order cannot reach it (630
 *   orders, above) and L59 cannot reach it (folding is inert inside a macro,
 *   above). What has NOT been tried is writing the eight groups out longhand
 *   instead of through a macro, which is the only way to give their statements
 *   distinct physical line numbers and put L59's key back in play.
 *
 * SUPERSEDED, with the measurement. The previous note closed the image groups on
 *   the colour table -- "the carried values come out in a0 and a1 where the ROM
 *   has t8 and t9, and t8/t9 are not in this procedure's colour table at all".
 *   The conclusion about carriers stands and no carrier spelling is worth
 *   re-testing. The premise about the residual does not: on the 136 shape the
 *   image groups' link loads already land in the ROM's own temporaries, and what
 *   is left there is ordering. That argument retires carriers, not the region.
 * Also still true: `register volatile Node32 *` is the worst pointer spelling,
 *   not the best; merging the three counter locals regresses; declaration order
 *   of the locals is inert over twenty permutations; a `do { } while (0)` region
 *   boundary (L97) round each text row reads 540 and round the image macro 548;
 *   and a redundant definition of node32 is eliminated before web formation, so
 *   L100's "a second definition halves the rank" is unavailable on that web. */
#ifdef NON_MATCHING
void func_overlay_101_F00078F4_18E3114(void) {
    Node32 *node32;
    Node24 *node24;
    void *handle;
    s32 length;
    s32 nodeIndex;
    s32 ownerIndex;
    s32 textIndex;
    s32 tIdx2;

    D_0.kind = 4; D_0.width2E = 0x140; D_0.height30 = 0xF0; D_0.asset34 = &D_CDC; D_0.color32 = 0xFF; D_0.color33 = 0xFF; D_0.value26 = 0; D_0.value28 = 0; D_0.value2A = 0; D_0.value2C = 0; D_0.chainType = 0; D_0.chain = 0; ownerIndex = D_1C4; D_1C0[ownerIndex] = &D_1C; D_1C4 = ownerIndex + 1;

    node32 = &D_340[D_1CC]; node32->x = 0x4E; node32->y = 0x14E; node32->value10 = 0; node32->color12 = 0xFF; node32->color13 = 0; node32->value18 = 0; node32->scale = 1.0f; node32->value14 = 0.0f; handle = func_overlay_101_F0000000_18DB820(0x92, 0); node32 = &D_340[D_1CC]; nodeIndex = D_1CC; node32->handle = handle; node32->previousType = D_0.chainType; node32->previous = D_0.chain; D_0.chain = node32; D_0.chainType = 2; D_1CC = nodeIndex + 1;

    D_0.value4A = 0x100; D_0.height48 = 0x14; D_0.color4F = 0xFF; D_0.value4C = 0xB4; D_0.color4E = 0xFF; D_0.mode40 = 0; D_0.y46 = 0x20; D_0.x42 = 0x20; D_0.width44 = 0x18; D_0.data50 = D_INPUT.dataF8; D_0.childType = 0; D_0.child = 0; ownerIndex = D_1C4; D_1C0[ownerIndex] = &D_38; D_1C4 = ownerIndex + 1;

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
    handle = func_overlay_101_F0000000_18DB820((imageId), 0);                \
    node32 = &D_340[D_1CC];                                                  \
    nodeIndex = D_1CC;                                                       \
    node32->previousType = D_0.childType;                                    \
    node32->previous = D_0.child;                                            \
    D_0.childType = 2;                                                       \
    D_0.child = node32;                                                      \
    node32->handle = handle;                                                 \
    D_1CC = nodeIndex + 1

    if (((D_F4 << 5) >> 28) & 1) {
        ADD_IMAGE_NODE(0x98, 0x2E, 0x97);
    } else {
        ADD_IMAGE_NODE(0x98, 0x2E, 0x98);
    }
    if (((D_F4 << 5) >> 28) & 2) {
        ADD_IMAGE_NODE(0xDA, 0x2E, 0x99);
    } else {
        ADD_IMAGE_NODE(0xDA, 0x2E, 0x9A);
    }
    if (((D_F4 << 5) >> 28) & 4) {
        ADD_IMAGE_NODE(0x98, 0x70, 0x9D);
    } else {
        ADD_IMAGE_NODE(0x98, 0x70, 0x9E);
    }
    if (((D_F4 << 5) >> 28) & 8) {
        ADD_IMAGE_NODE(0xDA, 0x70, 0x9B);
    } else {
        ADD_IMAGE_NODE(0xDA, 0x70, 0x9C);
    }

#undef ADD_IMAGE_NODE

#define ADD_TEXT_ROW_FIRST(field, rowY) \
    D_540[D_1D0].x = 0x80; \
    D_540[D_1D0].y = (rowY); \
    length = func_overlay_101_F000CEA8_18E86C8(field); \
    ownerIndex = D_1D0; \
    node24 = &D_540[ownerIndex]; \
    node24->length = (u8)length; \
    node24->opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1); \
    node24->color2 = 0; \
    node24->mode = 2; \
    node24->color0 = 0; \
    node24->color1 = 0; \
    node24->color3 = 0; \
    node24->kind = 4; \
    node24->text = (field); \
    node24->previousType = D_0.childType; \
    node24->previous = D_0.child; \
    D_0.childType = 3; \
    D_0.child = node24; \
    D_1D0 = ownerIndex + 1

#define ADD_TEXT_ROW(field, rowY) \
    D_540[D_1D0].x = 0x80; \
    D_540[D_1D0].y = (rowY); \
    length = func_overlay_101_F000CEA8_18E86C8(field); \
    nodeIndex = D_1D0; \
    node24 = &D_540[nodeIndex]; \
    node24->length = (u8)length; \
    node24->opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1); \
    node24->mode = 2; \
    node24->color0 = 0; \
    node24->color1 = 0; \
    node24->color2 = 0; \
    node24->color3 = 0; \
    node24->kind = 4; \
    node24->text = (field); \
    node24->previousType = D_0.childType; \
    node24->previous = D_0.child; \
    D_0.childType = 3; \
    D_0.child = node24; \
    D_1D0 = nodeIndex + 1

#define ADD_TEXT_ROW_B(field, rowY) \
    D_540[D_1D0].x = 0x80; \
    D_540[D_1D0].y = (rowY); \
    length = func_overlay_101_F000CEA8_18E86C8(field); \
    tIdx2 = D_1D0; \
    node24 = &D_540[tIdx2]; \
    node24->length = (u8)length; \
    node24->opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1); \
    node24->mode = 2; \
    node24->color0 = 0; \
    node24->color1 = 0; \
    node24->color2 = 0; \
    node24->color3 = 0; \
    node24->kind = 4; \
    node24->text = (field); \
    node24->previousType = D_0.childType; \
    node24->previous = D_0.child; \
    D_0.childType = 3; \
    D_0.child = node24; \
    D_1D0 = tIdx2 + 1











    ADD_TEXT_ROW_FIRST(D_INPUT.textFC, 0x92);
    ADD_TEXT_ROW(D_INPUT.text100, 0x9C);
    ADD_TEXT_ROW_B(D_INPUT.text104, 0xA6);

#undef ADD_TEXT_ROW
#undef ADD_TEXT_ROW_B
#undef ADD_TEXT_ROW_FIRST

    func_overlay_101_F0000000_18DB820(&D_2608);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/func_overlay_101_F00078F4_18E3114/func_overlay_101_F00078F4_18E3114.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_101_F00078F4_18E3114:start
 * symbol: func_overlay_101_F00078F4_18E3114
 * score: 136/525 words
 * frame: 0x40
 * relocations: 59
 * first-mismatch: +0xA8
 * summary: 136 masked from 145; frame and ladder exact. The pre-call node pointer must be an expression, not a local -- one uopt live range is formed per IR name, and an expression is a name.
 * PLATEAU-HANDOFF:func_overlay_101_F00078F4_18E3114:end
 */
