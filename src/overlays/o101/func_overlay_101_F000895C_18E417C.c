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
    u8 padFC[0x18];
    void *text114;
    void *text118;
    void *text11C;
} Inputs;

extern Root D_0;
extern Inputs D_INPUT;
extern void *D_1C;
extern void *D_38;
extern u32 D_F4;
extern void *D_D04;
extern void *D_2C60;
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
/* Six source-shape levers took this family from 461 masked words to 149. The
 * fifth closed the gate every earlier note on this family named and none
 * reached: the ROM holds the node pointer's web across the call AND stores
 * through the pointer it recomputes after the call.
 *
 * L59 -- the three non-macro assignment groups are ONE physical line each. With
 *   the stores on separate lines as1's `lineno` key emits them in source order;
 *   the ROM emits each group reversed, and folding retires the key so the raw
 *   ready-list order supplies the reversal. The macro bodies need no fold: a
 *   multi-line macro expansion already carries the invocation's line.
 *   The same reading fixes the root header's order: the ROM emits height, width,
 *   kind, so the source writes kind, width, height.
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
 *   already holds it. Read from the instrumented uopt (p1cost/p1dec, .text
 *   confirmed byte-identical to the tree's object with traces off). With the
 *   direct read the whole callee-saved bank falls into the ROM's layout at once:
 *   s0 node pointer, s1 &D_0, s2 the counter address, s3 the pool address, s4
 *   0xFF, s5 the literal 2. Worth 311 to 208, and it closes the whole post-call
 *   block: re-read, recompute, then handle, chain and previous-link stores
 *   through the recomputed pointer, which is the ROM's shape.
 * The counter bump must not sit behind a store. `D_1CC = D_1CC + 1` placed after
 *   any store makes uopt re-read the counter -- one extra load in each of the
 *   eight groups, +36 bytes. `nodeIndex = D_1CC` written immediately after the
 *   pointer expression is CSE'd onto the same load for free and lets the bump
 *   sit anywhere; that is the only thing the index local is for now.
 * L110 -- the text pointers are fields of the shared input block, not separate
 *   globals. One `lui`+`addiu` base then two displaced loads is what the ROM
 *   emits; three independent `lui`/`lw` pairs is what separate externs give.
 * Statement order in the post-call block is worth 208 to 189 over the 180 valid
 *   orders of the six statements, exhaustively measured; the macro takes
 *   previous-link, chain, handle, bump and the root group takes handle first.
 *
 * Refuted here, with the measurement. The previous note closed this family on
 *   "the ROM needs both the web across the call and the handle store after the
 *   recompute, and no source form found so far gives both", over six post-call
 *   orderings. All six held the index local fixed, which is the decision
 *   variable; the direct counter read gives both. Also refuted earlier and still
 *   true: `register volatile Node32 *` is the worst pointer spelling, not the
 *   best, and merging the three counter locals regresses.
 * Remaining, 145 masked (was 149): size, frame and the nine-slot frame ladder
 *   are exact. By region: prologue and the first root header 10 words; the
 *   chain group and the second root header 21; the eight image node groups 56;
 *   the three text rows 46, of which 26 are naming and 20 schedule.
 *
 * REFUTED, with the measurement. The previous note said the direct counter read
 *   "only reaches row 1", on the ground that applying it to all three read 175
 *   against 165. It reaches all three. That measurement was taken without
 *   re-running the statement-order search the old spelling had been tuned
 *   against, and without the second half of the edit. Rows 2 and 3 reading
 *   D_1D0 directly, row 2 carrying its own index local, and the three row tails
 *   re-climbed reads 145: naming falls 51 rows to 29 and byte-exact rises 401 to
 *   410. The same form, applied unchanged, reads 145 on all four members of the
 *   family.
 * WHY IT WORKS, and this is the part worth carrying elsewhere. A REDUNDANT LOAD
 *   INTO AN EXPRESSION TEMPORARY IS A FREE RING DRAW. `textIndex = D_1D0;`
 *   placed after `D_1D0 = textIndex + 1;` compiles to a load into the symbol's
 *   colour; as1's peephole deletes it because the value was just stored from a
 *   register, and because the destination was a coloured symbol ugen's temp ring
 *   never advanced. `node24 = &D_540[D_1D0]` compiles to the same load into a
 *   ring temp: as1 deletes exactly the same instruction, but the ring HAS
 *   advanced. One position of ring phase, at zero bytes. That single draw per
 *   row is the whole of what the previous note called "a ugen ring phase that
 *   diverges at the second multiply result of row 1": with the direct read our
 *   mflo, and every register after it in rows 2 and 3, lands on the ROM's.
 *   This is the inverse of the copy rule -- a copy of a value already in a
 *   register draws nothing and leaves no trace; a redundant load draws and
 *   leaves no trace.
 * WHAT IT COSTS, named from the records rather than the score. The pre-call
 *   reference is what held the node24 web at totalsave 45 over nocs 2; the
 *   direct read drops it to 39, so its save falls 22.5 to 19.5, under the node32
 *   web's 21.0 (totalsave 126 over nocs 6), and p1's repeated max-save order
 *   flips: node32 takes s0 first and node24 is left paying for it. Giving row 2
 *   its own index local splits the node24 web into two ranges of save 13.0 that
 *   both take v1 again. Read off p1dec with the instrumented uopt, .text
 *   confirmed byte-identical to the tree's object with traces off.
 *
 * THE NODE GROUPS ARE CLOSED BY THE COLOUR TABLE, which is a stronger statement
 *   than the spelling argument above and supersedes it. A carrier pair does
 *   reproduce the ROM's block: the seven differing words per group fall to two,
 *   and the block issues both old-link loads before every store, which is the
 *   shape no statement order reaches. But the carried values come out in a0 and
 *   a1 where the ROM has t8 and t9, and this procedure's globalcolor table,
 *   decoded from its own p1cost rows, is c1 v0, c2 v1, c3 to c6 a0 to a3, c7 to
 *   c12 t0 to t5, c13 unnamed, c14 to c22 s0 to s8. t8 and t9 are not in it at
 *   all. A source-level local is a symbol; a symbol is either coloured from that
 *   table or given a stack home; it is never a ring temp. So no carrier spelling
 *   can reach the ROM's registers, and the 409 to 413 every carrier form
 *   measures is the arithmetic of that rather than a tuning failure. Re-measured
 *   here on the 145 shape: 411 and 413.
 * Measured negatives, so nobody re-runs them. Declaration order of the locals is
 *   inert: twenty permutations, moving each declaration to the front and to the
 *   back and swapping each with the index local, all read 146 on the shape they
 *   were measured against. A `do { } while (0)` region boundary (L97) round each
 *   text row reads 540 and round the image macro 548 -- the wrong lever here by
 *   a wide margin. A redundant definition of node32 never reaches nocs:
 *   `node32 = node32;` and a duplicated `node32 = &D_340[D_1CC];` are both
 *   eliminated before web formation and leave the object byte-identical, so
 *   L100's "a second definition halves the rank" is not available on that web.
 *   The pre-call pointer spelled inline as `D_540[D_1D0].x = 0x80` or through a
 *   separate pointer local is 146 or worse across all 27 combinations.
 * Next, and it is one named blocker rather than 26 problems. The text rows'
 *   remaining naming rows are a single colour rotation: our pre-call node24
 *   pointer takes s0 where the ROM has v1, the index takes a1 where the ROM has
 *   s0, and the constant 24 and the D_INPUT base each shift one colour down
 *   behind them. The pre-call pointer's web records `available0=0x00020000` --
 *   exactly one colour, c14 s0 -- with numintf 13, so every caller-saved colour
 *   is forbidden to it and no save or spelling lever can reach v1; the index web
 *   records `available0=0x0ffc0000`, c4 to c13, and cannot reach s0. The lever
 *   is therefore that interference set, not the save ratio: find why a range
 *   whose only uses are the two pre-call stores interferes with thirteen webs,
 *   and whether the ROM's pre-call and post-call pointers are one range rather
 *   than two. */
#ifdef NON_MATCHING
void func_overlay_101_F000895C_18E417C(void) {
    Node32 *node32;
    Node24 *node24;
    void *handle;
    s32 length;
    s32 nodeIndex;
    s32 ownerIndex;
    s32 textIndex;
    s32 tIdx2;

    D_0.kind = 4; D_0.width2E = 0x140; D_0.height30 = 0xF0; D_0.asset34 = &D_D04; D_0.color32 = 0xFF; D_0.color33 = 0xFF; D_0.value26 = 0; D_0.value28 = 0; D_0.value2A = 0; D_0.value2C = 0; D_0.chainType = 0; D_0.chain = 0; ownerIndex = D_1C4; D_1C0[ownerIndex] = &D_1C; D_1C4 = ownerIndex + 1;

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

/* All three rows read the counter global directly in the pre-call pointer
 * expression, and row 2 carries its own index local. Both are load-bearing and
 * both are explained in the note above: the direct read is a free ring draw,
 * and the separate index local pays back the colour race that the direct read
 * would otherwise lose. The tail orders are the measured optimum, not tidiness;
 * every one of them was scored. */
#define ADD_TEXT_ROW_FIRST(field, rowY)                                     \
    node24 = &D_540[D_1D0];                                                  \
    node24->x = 0x80;                                                        \
    node24->y = (rowY);                                                      \
    length = func_overlay_101_F000CEA8_18E86C8(field);                       \
    textIndex = D_1D0;                                                       \
    node24 = &D_540[textIndex];                                              \
    node24->length = (u8)length;                                             \
    node24->opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1);    \
    node24->color1 = 0;                                                      \
    node24->color3 = 0;                                                      \
    node24->kind = 4;                                                        \
    node24->color2 = 0;                                                      \
    node24->color0 = 0;                                                      \
    node24->mode = 2;                                                        \
    node24->text = (field);                                                  \
    node24->previousType = D_0.childType;                                    \
    node24->previous = D_0.child;                                            \
    D_0.childType = 3;                                                       \
    D_0.child = node24;                                                      \
    D_1D0 = textIndex + 1

#define ADD_TEXT_ROW(field, rowY)                                           \
    node24 = &D_540[D_1D0];                                                  \
    node24->x = 0x80;                                                        \
    node24->y = (rowY);                                                      \
    length = func_overlay_101_F000CEA8_18E86C8(field);                       \
    tIdx2 = D_1D0;                                                           \
    node24 = &D_540[tIdx2];                                                  \
    node24->length = (u8)length;                                             \
    node24->opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1);    \
    node24->mode = 2;                                                        \
    node24->color0 = 0;                                                      \
    node24->color1 = 0;                                                      \
    node24->color2 = 0;                                                      \
    node24->color3 = 0;                                                      \
    node24->kind = 4;                                                        \
    node24->text = (field);                                                  \
    node24->previous = D_0.child;                                            \
    node24->previousType = D_0.childType;                                    \
    D_0.child = node24;                                                      \
    D_0.childType = 3;                                                       \
    D_1D0 = tIdx2 + 1

#define ADD_TEXT_ROW_B(field, rowY)                                         \
    node24 = &D_540[D_1D0];                                                  \
    node24->x = 0x80;                                                        \
    node24->y = (rowY);                                                      \
    length = func_overlay_101_F000CEA8_18E86C8(field);                       \
    textIndex = D_1D0;                                                       \
    node24 = &D_540[textIndex];                                              \
    node24->length = (u8)length;                                             \
    node24->opacity = (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1);    \
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
    D_0.child = node24;                                                      \
    D_1D0 = textIndex + 1


    ADD_TEXT_ROW_FIRST(D_INPUT.text114, 0x92);
    ADD_TEXT_ROW(D_INPUT.text118, 0x9C);
    ADD_TEXT_ROW_B(D_INPUT.text11C, 0xA6);

#undef ADD_TEXT_ROW
#undef ADD_TEXT_ROW_B
#undef ADD_TEXT_ROW_FIRST

    func_overlay_101_F0000000_18DB820(&D_2C60);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/func_overlay_101_F000895C_18E417C/func_overlay_101_F000895C_18E417C.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_101_F000895C_18E417C:start
 * symbol: func_overlay_101_F000895C_18E417C
 * score: 145/525 words
 * frame: 0x40
 * relocations: 59
 * first-mismatch: +0xA8
 * summary: 145 masked from 149; frame and ladder exact. The direct counter read in rows 2 and 3 is a free ring draw and lands the whole ring phase.
 * PLATEAU-HANDOFF:func_overlay_101_F000895C_18E417C:end
 */
