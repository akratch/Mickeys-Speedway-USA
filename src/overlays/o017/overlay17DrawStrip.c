#include "PR/ultratypes.h"
typedef struct G { u32 w0, w1; } G;
typedef struct V { s16 x,y,z; u8 r,g,b,a; } V;
typedef struct Pair { V first, second; } Pair;
typedef struct Strip { s16 count; u8 buffer,pad03; void *material; u8 pad08[0x24]; Pair *buffers[2]; void *triangles; } Strip;
extern void overlay17PrepareStripReloc(G **, void *, s32, s32);
#define S(v,s,w) (((u32)(v)&((1U<<(w))-1U))<<(s))
#define PRIM(p) { G *m=(G *)(p); m->w0=0xFA000000; m->w1=0xFFFFFFFF; }
#define VTX(p,a,n) { G *m=(G *)(p); m->w0=S(4,24,8)|S(((n)<<3)|((u32)(a)&6),16,8)|S(((n)<<3)+((n)<<1)+8,0,16); m->w1=(u32)(a); }
#define STRIP(p,a,n,t) { G *m=(G *)(p); m->w0=S(5,24,8)|S((((n)-1)<<4)|(t),16,8)|S((n)<<4,0,16); m->w1=(u32)(a); }
#define SYNC(p) { G *m=(G *)(p); m->w1=0; m->w0=0xE7000000; }

/*
 * Plateau: spelling the final packet through the existing pair local gives
 * an exact-size 119-word candidate; the linked promotion trial agrees with the
 * object and carries no collateral.
 *
 * 2026-09-10, lane c6-close: the frame blocker is closed. The 0x38/0x40
 * difference was never packet-local lifetimes -- it is the declaration ORDER,
 * and moving `textured` ahead of `vertices` and `flush` recovers 0x38 with no
 * other home moving and 119 instructions unchanged: 16 -> 14 masked words.
 * All 40,320 declaration orders and all 6,720 legal orders of the eight
 * prologue statements were measured; 14 is the floor of both. Adding dummy
 * locals is inert here, because an unused local is eliminated before the frame
 * is sized.
 *
 * 2026-09-11, lane w3-low: 14 -> 3, and the structural residual is gone.
 * The three-row block that sat three rows early was never a scheduling fault.
 * It was a web-count fault: written as a test followed by a separate
 * decrement, the loop-entry guard and the do-while's own post-decrement built
 * two temp symbols where the target builds one. Spelling the guard as a
 * post-decrement makes cfe reuse a single temp for both, which merges the two
 * webs, and the whole prologue colouring -- the packet cursor, `previous`, and
 * the count/counter pair -- lands on the target's. Nothing else changed:
 * frame 0x38, 119 instructions, zero size delta, zero rows structurally
 * different, first mismatch now +0x110.
 *
 * What is left is exactly one web: the vertex packet's segment-relocated
 * address, which this candidate colours one register later than the target.
 * The decision variable is named and receipted. That address is a uopt
 * cross-statement common subexpression, so its web is numbered after every
 * front-end symbol, including the two packet-cursor block locals. Four webs
 * tie at save 30.0 with one live-range chunk each -- the two packet cursors,
 * the vertex-count shift temp, and the address -- and globalcolor breaks that
 * tie on ascending web number, so the address is decided last of the four and
 * takes the fifth colour instead of the fourth. Forcing the strip cursor's web
 * to the fifth colour is ACCEPTED and reproduces the target exactly, all 119
 * words; forcing the address web to the fourth is DECLINED with the first four
 * colours already forbidden, which is genuine interference rather than a ratio
 * the source can move.
 *
 * Measured and flat at 3 (all zero size delta unless noted): rewriting either
 * packet macro to bind the address to a named local, before or after the
 * cursor; fusing the two packet stores into one comma expression in either
 * order; storing the second word first; splitting either cursor into two
 * locals (copy propagation merges them straight back); a byte-pointer or
 * word-pointer cursor; reordering the strip write against the start reset;
 * and commuting the address operands. Regressions, so they are not the route:
 * carrying either cursor in `previous` or `start`, binding the strip macro's
 * address to a local (+8 bytes), and writing the strip packet through a
 * negative index off the command pointer. Three discarded-expression probes on
 * the address -- or-with-zero, and-with-minus-one, xor-with-zero -- left every
 * save in the records untouched, so L109 does not reach a subexpression that
 * uopt has already folded.
 *
 * Second pass, same lane: the other two routes are refuted from the records
 * too. Serving both packet macros from one shared cursor local collapses the
 * tie from four webs to two and DOES give the address the register it wants --
 * but the counter web then takes the fifth colour where the target has it in
 * the sixth, and the merge deletes a copy, so the candidate is 8 bytes short.
 * The four-way tie is load-bearing: five webs must be decided before the
 * counter so it lands in the sixth colour, and the address must be third of
 * them. Declaration placement does not reach the numbering at all -- declaring
 * the strip cursor first, or hoisting either cursor into the enclosing block,
 * gives a byte-identical record set, because web numbers follow the order of
 * the defining assignments, and reordering those means emitting the packets in
 * the wrong order. Writing the strip packet through the command pointer with
 * the increment last does make its cursor a uopt temp and does move the
 * address up, but the command-value web stops being one shared four-site range
 * and loses the first colour.
 *
 * 2026-09-11, lane p6-tight priced the second-definition half of that
 * arithmetic and it is empty at zero size delta. A second definition alone
 * halves 30.0 to 15.0, below the counter web, so the reachable points need a
 * surviving copy that also adds references: five references over two
 * definitions is 25.0, seven over three is 23.3. Twelve copy forms were
 * measured. A self-assignment of the strip cursor placed after both packet
 * stores is dead-code-eliminated and byte-identical to the base; placed
 * between or before them it survives, moves the colouring, and costs eight
 * words. A cast copy, an add-of-zero copy, an unsigned round trip, a split
 * declaration, two self-assignments in a row and a separate named pointer for
 * the second store are all eight words too. So on this web a copy carrier is
 * either deleted and record-free, or surviving and expensive -- that is L126's
 * bound measured at this site.
 *
 * The arithmetic of the remaining cell, for whoever picks it up: the strip
 * cursor's save must land strictly between the counter web's 20.6 and the
 * group's 30.0, or the address web must be numbered below the strip cursor's.
 * Every reference in that block sits at loop depth one and therefore weighs
 * ten, and every one of the four webs has a single chunk, so a save there can
 * only be a multiple of ten -- 20.0 loses the fifth colour to the counter web
 * and 30.0 keeps the tie. A fractional save needs the chunk count to move,
 * which no source form measured here does.
 */
/*
 * 2026-09-12, lane p10-tight read this procedure's whole colour ladder out of
 * the instrumented compiler, after gating its object text byte for byte against
 * the configured build's. Twenty phase-one decisions, no phase-two records, and
 * the selection order is strictly descending save. The four tied webs all carry
 * save 30.0 with divisor 1 and total 30, and the lineage capture puts all four
 * in ONE basic block. Reading the divisor as a count of basic blocks rather
 * than of definitions therefore does not open this: it is already at its floor
 * and can only move up, which divides the save below the counter web's 20.6.
 * With loop-depth-one references weighing ten and depth-zero ones weighing one,
 * a divisor of 1 admits totals of 30, 21 and 12 for three references, and only
 * 21 lands inside the open interval -- so the cell is exactly "one of the strip
 * cursor's three references at loop depth zero, surviving", and the three are
 * its own definition and the two packet stores, none of which can leave the
 * loop. The untested prediction the third pass left is also refuted: any web
 * that forbids the contested colour to the strip cursor must be live in the one
 * block it occupies, and the address web occupies that same block, so
 * block-set interference reaches both.
 */
#ifdef NON_MATCHING
void overlay17DrawStrip(G **commands, Strip *strip) {
    Pair *pair;
    Pair *previous;
    Pair *start;
    s32 textured;
    s32 vertices;
    s32 flush;
    u32 remaining;
    u32 segment;
    if (strip == 0) return;
    segment=0x80000000U;
    textured = strip->material ? 1 : 0;
    textured |= (overlay17PrepareStripReloc(commands, strip->material, 0x1F, 0), 0);
    PRIM((*commands)++);
    vertices=strip->count;
    remaining=vertices;
    previous=0;
    pair=strip->buffers[strip->buffer];
    start=0; vertices=0; flush=0;
    /* Post-decrement, not a separate test-then-decrement: cfe reuses ONE temp
     * symbol for this entry test and for the do-while's own `remaining--`, so
     * the two share a single web instead of two. That one merge moves the
     * prologue's whole colour assignment onto the target's and empties the
     * structural residual: 14 -> 3 masked words, zero size delta. */
    if (remaining--) {
        do {
            vertices += 2;
            if (remaining == 0) {
                flush=1;
                goto check_flush;
            }
            if (pair->first.a == 0) {
                flush=1;
                goto check_flush;
            }
            if (start == 0) {
                if (previous != 0) { start=previous; vertices=4; }
                else { start=pair; vertices=2; }
            }
            if (vertices >= 17) flush=1;
check_flush:
            if (flush) {
                flush=0;
                if (start != 0) {
                    VTX((*commands)++, (void *)((u32)start+segment), vertices);
                    STRIP((*commands)++, (void *)((u32)strip->triangles+segment), vertices-2,
                          textured);
                    start=0;
                }
                vertices=0;
            }
            previous=pair++;
        } while (remaining--);
    }
    pair = (Pair *)(*commands)++;
    ((G *)pair)->w1 = 0;
    ((G *)pair)->w0 = 0xE7000000;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o017/overlay17DrawStrip/func_overlay_017_F00008B4_187426C.s")
#endif

/* PLATEAU-HANDOFF:overlay17DrawStrip:start
 * symbol: overlay17DrawStrip
 * score: 3/119 words
 * frame: 0x38
 * relocations: 1
 * first-mismatch: +0x110
 * summary: 14 -> 3 (2026-09-11, lane w3-low), and the recorded structural reading is refuted. The three-row block was never a scheduling fault and the web count was not right: spelled as a test plus a separate decrement, the loop-entry guard and the do-while's own decrement build two cfe temp symbols where the target builds one. Spelling the guard as a post-decrement merges them and the whole prologue colouring lands on the target's, taking all 12 naming rows and both structural rows at once. What is left is one web, the vertex packet's segment-relocated address, coloured one register late; forcing the strip cursor one colour higher is accepted and reproduces the ROM at 119 of 119. The tie-break arithmetic is now fitted rather than guessed: save is totalsave over a divisor, totalsave sums ten to the loop depth over every reference including the defining one, and on this web the divisor is the number of DEFINITIONS -- which is movable at zero size delta while totalsave is not. A second definition halves the save for free; the save the target needs lies strictly between two of the other webs and the first reachable points all cost at least two instructions. So the interval is non-empty and empty at zero size delta.
 * PLATEAU-HANDOFF:overlay17DrawStrip:end
 */
