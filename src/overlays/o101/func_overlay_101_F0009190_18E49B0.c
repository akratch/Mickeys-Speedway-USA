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
    u8 padFC[0x24];
    void *text120;
    void *text124;
    void *text128;
} Inputs;

extern Root D_0;
extern Inputs D_INPUT;
extern void *D_1C;
extern void *D_38;
extern u32 D_F4;
extern void *D_D18;
extern void *D_2F8C;
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
 * Remaining, 149 masked (was 189): size, frame and the nine-slot frame ladder
 *   are exact. By region: the eight image node groups hold 40 schedule words
 *   with 8 insertion and 8 deletion words; the chain group and the two root
 *   headers hold 14 more schedule words and 4 naming rows; the three text rows
 *   hold 47 naming rows and 4 schedule words.
 *
 * REFUTED, with the measurement. The previous note said "the next lever is the
 *   node groups' as1 order; the text rows follow how many scratch registers
 *   those groups consume". The text rows do NOT follow the node groups. Their
 *   81 naming rows were their own defect, and the fix is the L115/L101 lever
 *   this file already applies to the node macro but which was never applied to
 *   the text macro: read the counter global directly in the pointer expression.
 *   `node24 = &D_540[D_1D0]` in place of `textIndex = D_1D0; node24 =
 *   &D_540[textIndex]` on the FIRST text row's pre-call read is worth 189 to
 *   165 on its own, with the node groups untouched.
 * Only the first row, and that is not a coincidence. Rows 2 and 3 fold their
 *   pre-call index into the previous row's increment, so the spelling only
 *   reaches row 1; applying it to all three instead reads 175, and the records
 *   say exactly why. The pre-call reference is what keeps the `node24` symbol
 *   web's totalsave at 48 over nocs 2, i.e. save 24, above the node pointer
 *   web's save of 21. p1 is repeated max-save selection, so node24 is decided
 *   first, while s0 is still unpaid at 6.5, and the first caller-saved colour
 *   at 6.0 wins -- which is the ROM's v1. Drop those references and node24's
 *   save falls to 19.5, the node pointer web takes s0 first, s0 becomes free at
 *   0.0 for everything after it, and node24 takes s0 where the ROM has v1.
 *   `CDX_FORCE=p1:w99=c2` on that shape reads 158 against 175, confirming the
 *   colour is the whole difference.
 * Statement order then pays 165 to 149 over six blocks -- two root headers, the
 *   chain group, the image macro and the two text macros -- by pairwise-swap
 *   hill climbing with eight random restarts, all of which reconverge on 149.
 *   1500 random text-macro orders measured separately are all at or above 189,
 *   so this is a narrow optimum and not a plateau.
 *
 * THE NODE GROUPS ARE DAG-CLOSED, and this is the useful negative. as1 orders
 *   every memory reference after any preceding store whose base register
 *   differs, and disambiguates only same-base/different-displacement pairs;
 *   read that off the `-Wa,-R` node table, where the previous-link store lists
 *   the chain-pointer load as an after-node. Because of that edge the target's
 *   node block -- which issues both old-link loads before every store -- is
 *   INFEASIBLE in this candidate's dependence graph. No as1 tie-break, no
 *   physical-line fold and no statement order reaches it; the graph has to
 *   change, and only a carried old-link value changes it.
 * Every carrier spelling was measured and every one lands on 421 (from 149's
 *   sibling shape), with the carriers in a globalcolor colour where the ROM has
 *   ring registers: a new s32/void* pair, a new u32/Node32* pair, eight
 *   distinct per-invocation symbols, `register`-qualified carriers, and four
 *   reuses of existing locals. The records say it is not a spelling problem. A
 *   carrier web spans no call, so after globalcolor splits it the residue reads
 *   `nocs=1 totalsave=1.000000 bestcost=0.000000` and the pass colours whenever
 *   totalsave exceeds bestcost -- which for a single-block range is always,
 *   while any caller-saved colour is free. Any colour at all removes two ring
 *   consumers from each of the eight node blocks, so the ring re-phases
 *   throughout: naming goes 0 to 109 there, against a structural gain of 40 to
 *   38. The carrier direction is closed on arithmetic, not on spellings.
 * Two instrument facts worth inheriting. `CDX_FORCE=p1:wN=s` does NOT apply to
 *   a web whose first decision is already `split`; the second, colouring
 *   decision on the same web number records `forced=-1` and colours anyway, so
 *   the split path cannot be forced on a split residue (colour forces do apply
 *   and record `forced=<colour>`). And L109 probes are inert on this procedure:
 *   `(s32)node24 | 0`, `& -1`, `^ 0`, at one, two and four copies, all leave
 *   `totalsave=39.000000` unchanged and the object byte-identical.
 * L114 for this procedure, unchanged and re-confirmed: its own p1cost rows
 *   decode as c1 v0, c2 v1, c3 a0, c4 a1, c5 a2, c6 a3, c7 t0, c8 t1, c9 t2,
 *   c10 t3, c11 t4, c12 t5, c13 unnamed, c14-c22 s0-s8. Do not classify a
 *   naming row by register bank; decode the table from your own records.
 * Next: the 40 image-group schedule words are closed as argued above. The 47
 *   text naming rows are a ugen ring phase that diverges at the second multiply
 *   result of row 1 while every preceding register agrees, so the lever is
 *   ugen's pre-schedule order rather than a colour; read `cc -S` for that
 *   procedure and find what the ROM allocates a ring register to there. */
#ifdef NON_MATCHING
void func_overlay_101_F0009190_18E49B0(void) {
    Node32 *node32;
    Node24 *node24;
    void *handle;
    s32 length;
    s32 nodeIndex;
    s32 ownerIndex;
    s32 textIndex;

    D_0.kind = 4; D_0.width2E = 0x140; D_0.height30 = 0xF0; D_0.asset34 = &D_D18; D_0.color32 = 0xFF; D_0.color33 = 0xFF; D_0.value26 = 0; D_0.value28 = 0; D_0.value2A = 0; D_0.value2C = 0; D_0.chainType = 0; D_0.chain = 0; ownerIndex = D_1C4; D_1C0[ownerIndex] = &D_1C; D_1C4 = ownerIndex + 1;

    node32 = &D_340[D_1CC]; node32->x = 0xF2; node32->y = 0x14E; node32->value10 = 0; node32->color12 = 0xFF; node32->color13 = 0; node32->value18 = 0; node32->scale = 1.0f; node32->value14 = 0.0f; handle = func_overlay_101_F0000000_18DB820(0x93, 0); node32 = &D_340[D_1CC]; nodeIndex = D_1CC; node32->handle = handle; node32->previousType = D_0.chainType; node32->previous = D_0.chain; D_0.chain = node32; D_0.chainType = 2; D_1CC = nodeIndex + 1;

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

/* Row 1 only: the pre-call read is the counter global itself, so the address
 * is an expression web in the temp ring rather than a coloured index local.
 * Rows 2 and 3 fold their pre-call index into the previous row's increment,
 * so the spelling never reaches them, and forcing it there costs the node24
 * web the save that wins it v1. See the note above. */
#define ADD_TEXT_ROW_FIRST(field, rowY)                                       \
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

#define ADD_TEXT_ROW(field, rowY)                                            \
    textIndex = D_1D0;                                                       \
    node24 = &D_540[textIndex];                                              \
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
    D_1D0 = textIndex + 1;                                                   \
    node24->previousType = D_0.childType;                                    \
    node24->previous = D_0.child;                                            \
    D_0.child = node24;                                                      \
    D_0.childType = 3

    ADD_TEXT_ROW_FIRST(D_INPUT.text120, 0x92);
    ADD_TEXT_ROW(D_INPUT.text124, 0x9C);
    ADD_TEXT_ROW(D_INPUT.text128, 0xA6);

#undef ADD_TEXT_ROW
#undef ADD_TEXT_ROW_FIRST

    func_overlay_101_F0000000_18DB820(&D_2F8C);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/func_overlay_101_F0009190_18E49B0/func_overlay_101_F0009190_18E49B0.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_101_F0009190_18E49B0:start
 * symbol: func_overlay_101_F0009190_18E49B0
 * score: 149/525 words
 * frame: 0x40
 * relocations: 59
 * first-mismatch: +0xB0
 * summary: 149 masked from 189; size, frame and the nine-slot frame ladder exact. The text rows' rotation was their own pre-call index local, not the node groups' ring consumption; the node groups' 40 schedule words are DAG-closed.
 * PLATEAU-HANDOFF:func_overlay_101_F0009190_18E49B0:end
 */
