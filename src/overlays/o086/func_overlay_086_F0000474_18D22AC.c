#include "PR/ultratypes.h"

#define NULL ((void *)0)
#define M2C_UNK s32
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

struct _m2c_stack_func_overlay_086_F0000474_18D22AC {
    /* 0x00 */ char pad0[0x88];
    /* 0x88 */ s16 *sp88;                           /* inferred */
    /* 0x8C */ char pad8C[8];                       /* maybe part of sp88[3]? */
    /* 0x94 */ f32 sp94;                            /* inferred */
    /* 0x98 */ f32 sp98;                            /* inferred */
    /* 0x9C */ f32 sp9C;                            /* inferred */
    /* 0xA0 */ s32 spA0;                            /* inferred */
    /* 0xA4 */ char padA4[4];
};                                                  /* size = 0xA8 */

M2C_UNK overlay86BuildTransform(void *, void *);    /* extern */
M2C_UNK overlay86ScaledVectorPosition(void *, void *, f32 *, f32 *, f32 *); /* extern */
s32 overlay86SelectPosition(void *, void *);        /* extern */
M2C_UNK ext_o0_2b90(M2C_UNK, f32, f32, f32, s32, void *); /* extern */
M2C_UNK ext_o0_2c4c(s32, s32);                     /* extern */
M2C_UNK ext_o0_2c64(s32, s32);                     /* extern */
M2C_UNK ext_o0_1bed0(void *, s32, s32, s32, s32, s32, s32); /* extern */
M2C_UNK ext_o0_2d70(s32, f32, f32, f32);           /* extern */
M2C_UNK ext_o0_2d98();                             /* extern */
M2C_UNK ext_o0_28d88(M2C_UNK);                     /* extern */
s32 ext_o0_2952c(M2C_UNK, M2C_UNK);                /* extern */
M2C_UNK ext_o0_29b94(void *, void *);              /* extern */
f32 ext_o0_2a470(s16);                             /* extern */
void *ext_o0_53d0(u8);                             /* extern */
M2C_UNK ext_o0_494ac(u8, M2C_UNK, f32, M2C_UNK, s32, s32, s32); /* extern */
M2C_UNK ext_o0_5a758(void *, M2C_UNK, f32);        /* extern */
M2C_UNK ext_o0_5a914(void *, M2C_UNK, s32, f32);   /* extern */
s16 ext_o0_f690(f32, f32, f32);                    /* extern */
M2C_UNK ext_o7_dbc(M2C_UNK);                       /* extern */

/* Workbench register residual: 29 positional words in the exact 662-word/-0xA8 frame, first +0xA8.
 * The declaration list is evidence-fixed, not stylistic: cfe homes every declared auto in declaration order from
 * (frame top - 4) downwards, so the eighth declaration owns sp+0x88 -- the slot the target spills the command pointer to.
 * Measured here, not remembered: eleven autos give 0xA8, and both twelve and thirteen give 0xB0.  That is the
 * recorded round8(4 * declared locals + 4 * uopt temporaries) block landing exactly on 56 with three temporaries and
 * no slack, so the retail body declares exactly eleven and every extra carrier has to come out of an existing one.
 *
 * 59 -> 51 -> 29.  The 51 were read as "one interfering caller-saved web must be removed"; the removal is real and
 * this pass found it, but it is a web SHORTENED rather than deleted, and it costs nothing.  Read from the traced
 * globalcolor rather than guessed:
 *
 *   - The two `M2C_FIELD(M2C_FIELD(temp_v0, void **, 0x48), u16 *, 6)` sites shared ONE cfe temporary, so uopt saw
 *     one web with six references (totalsave 60, nocs 2, save 30) spanning both switch arms.  A web that long is
 *     live across a call, and a web live across a call has **v0 struck from its candidate list entirely** -- the
 *     traced `p1cost` rows for it start at colour 2, so no `CDX_FORCE` can put it on v0 either.  That is why the
 *     target's v0 and the candidate's v1 looked like a colouring choice and were not one.
 *   - Carrying the case-0 site in the already-declared `temp_a0` splits that shared temporary in two.  The case-2/3
 *     site becomes its own web (totalsave 30, nocs 1), stops crossing a call, regains v0, and takes it; the state
 *     byte then falls into v1 and the whole `M2C_FIELD(temp_s0, u8 *, 0)` family -- ten rows -- closes with it.
 *     51 -> 30 at delta 0, no frame move, no new declaration ([L44] carrier identity).
 *   - Writing the 0xC command word through the pointer before the advance (`*var_a1 = 0xC;`) is the target's own
 *     shape (`sh t1, 0(s5)`) and closes one more: 29.
 *
 * The general result, and it is the same one the overlay 57 marked-entry loop produced the same day: on a
 * pressure-free procedure a caller-saved residual is a question about which SYMBOL a value's references belong to.
 * Merging two block-scoped copies into one declaration raises a web's save and lengthens it; giving a shared
 * compiler temporary a per-site carrier lowers its save AND shortens it, and shortening is what returns v0 to a web
 * that was live across a call.  Neither costs an instruction.
 *
 * The 29 that remain are two families, both measured to their wall:
 *
 *   7  `temp_a0`'s own web (the case-0 +0x48 read and the +0x3E0 pointer) sits on a0 and the target reads v0.  The
 *      web still spans the head block's three +0x40 reads and the case-1/2/3 `ext_o0_f690` results, so it is still
 *      live across calls and v0 is still struck from its candidate list.  Shortening it further needs a carrier
 *      that is dead in the head block: `spA0` is (it is overwritten by the 1024-scale a few statements later) and
 *      routing either +0x40 group through it costs 579 words and 4 bytes, because the `ext_o0_2d98` argument then
 *      has to be re-materialised.  A twelfth declaration is 0xB0.  Measured flat or worse: the full 27-cell lattice
 *      of {inline, temp_a0, var_a1} x {both +0x48 sites} x {+0x3E0}, whose floor is this form at 30.
 *   22 the command pointer `var_a1` sits on a2 and the target reads a1, with the schedule rows that follow.  a1 is
 *      forbidden for it by an interfering web that holds a1 and is decided first (save 6.67 against 2.67), and that
 *      web genuinely needs a1: forcing it to a2 or a3 costs 211 words and 4 bytes and to t0 costs 344.  Raising the pointer's save
 *      past it needs twelve more references or a span short enough for nocs 1, and it spans the whole head block.
 *      Store-order variants on top of this form: 29 (first store through the pointer or through
 *      `M2C_FIELD(temp_s0, s16 *, 0x30)`), 31, 32, 33 and 34 for five reorderings of the six command words.
 *      Advance spellings are inert: `var_a1 += 6`, `&var_a1[6]`, a `(u32)` cast and writing the first command word
 *      after the advance through `M2C_FIELD(temp_s0, s16 *, 0x30)` all measure 29; re-deriving the advanced pointer
 *      from `temp_s0 + 0x3C` is 623 at delta -4, because the pointer then stops being an induction of the
 *      already-live s5 carrier.
 *
 * Also measured flat at the 51-word state and not worth repeating: do { } while (0) and if (1) { } region
 * boundaries at five placements; four byte-test spellings; three pointer-init and two advance spellings; a dead
 * tested expression, which adds a web and moves the ladder the wrong way.
 *
 * 2026-09-10, lane w8-bigclose: the 22-word term is now read off the allocator,
 * and the closure above is CONFIRMED rather than superseded -- which is worth
 * saying, because the corrected form of [L100] raises the possibility that p1
 * does not own a small procedure at all.  Here it owns all of it.  An
 * instrumented uopt (CDX log; its object is byte-identical to the tree's, which
 * is the identity gate) records 1218 p1 records over 66 decisions and ZERO p2,
 * so [L106]'s ascending-web-number axis does not exist on this function and
 * `save = totalsave/nocs` is the only order there is.  Ask that question first
 * ([L108]); it costs one compile and it retires a whole axis.
 *
 * The traced numbers are exactly the ones recorded above: the web holding a1 is
 * save 6.666667 (nocs 3, totalsave 20) and takes that colour at cost 0 with it
 * as its only zero-cost candidate; the command pointer is save 2.666667 (nocs 3,
 * totalsave 8) at cost 1.  What the trace adds is that by the time the pointer
 * is decided the colour is not merely taken but FORBIDDEN -- its forbidden mask
 * carries the first four caller-saved colours -- so a direct force of the
 * pointer onto a1 is DECLINED and the object comes back byte-identical.  A
 * forced-colour experiment on this web therefore proves nothing about the
 * target, and the only route is the decision order.
 *
 * [L109]'s zero-cost probe does not supply it, and that is a new negative worth
 * carrying: five reference forms on the pointer -- a discarded `(void)p;`, a
 * bare `p;`, a self-assignment, `p += 0;`, `p = &p[0];` and an idempotent cast
 * round-trip -- at 1, 4, 6, 13, 14 and 20 repetitions ALL leave totalsave at
 * exactly 8.000000 and the score at 29, delta 0.  uopt takes its reference
 * count after copy propagation, so a probe that merely names a pointer at loop
 * depth 0 never reaches it; only the tested form `if (p == 0) { }` reaches the
 * count, and six of those are 235.  The pointer's live range holds no loop, so
 * there is no depth at which [L109]'s x10 weighting is available either.
 *
 * Two more axes are now measured flat on THIS base rather than on the 51-word
 * one, which is the re-measurement [L47] asks for: eleven positions for the
 * pointer's defining statement ([L106]/[L105]) score 29 at the four latest and
 * 30, 32, 55, 55, 55, 55, 60 and 641 as it moves earlier; and ten [L107] region
 * boundaries -- `if (1) { }` and `do { } while (0)` after the definition,
 * before the advance, before the first command word, around the definition,
 * around the six writes, inside the arm, two at once, and the in-place advance
 * with and without one -- are all exactly 29 at delta 0.  The reassociation
 * that turns the advance into an offset from the base carrier is not opened by
 * a region here.
 *
 * So the next lever is unchanged and now has a number on it: the command
 * pointer needs totalsave above 20 at nocs 3, which is thirteen more counted
 * references, or a span short enough for nocs 1 at its present 8.  Neither is
 * reachable from the forms above.
 *
 * 2026-09-12, lane p7-ovl3: THAT REQUIREMENT IS THE WRONG VARIABLE, and the
 * records say so.  Baseline reproduces exactly -- 2648 bytes, delta 0, 29
 * masked, aligned 640 byte-exact / 14 naming / 2 immediate / 8 structural.
 * The instrumented uopt (object byte-identical to the tree's, which is the
 * identity gate) gives the pointer web as web 40, save 2.666667, nocs 3,
 * totalsave 8, and the a1 holder as web 274, save 6.666667.  Three facts, none
 * of which fits "a1 is held by a web that outranks it":
 *
 *   - web 40's `p1cost` list STARTS AT COLOUR 5.  v0, v1, a0 and a1 are not
 *     priced for it at all -- they are absent from the list, not outbid in it.
 *     No save ratio can buy a colour that is never offered, so the recorded
 *     requirement (totalsave above 20, or nocs 1) cannot deliver a1 however it
 *     is met.
 *   - its interference list holds NO neighbour assigned colour 4.  The
 *     forbidden bit for a1 is set with nothing holding a1.
 *   - forcing web 274 off a1 to c5, c6 and c7 leaves web 40 on a2 in all three
 *     runs (211, 211 and 344 words), and a direct `CDX_FORCE=p1:w40=c4` is
 *     declined byte-identically.  Freeing the incumbent does not open the
 *     colour.
 *
 * What does open it is the ARITY of the `ext_o0_2d98` call sites inside the
 * pointer's live range.  Compiling `ext_o0_2d98(temp_a0)` in place of
 * `ext_o0_2d98(temp_a0, var_a1)` at both sites clears exactly bit 27 of web
 * 40's forbidden mask -- 0x7803e000 becomes 0x7003e000 -- and globalcolor then
 * reports `bestcolor=4 bestreg=a1` for the same web at the same save 2.666667.
 * The rule underneath is that a web live across a call is denied the argument
 * registers that call actually loads: this procedure's calls in that range load
 * a0 and a1, which is why a2 and a3 stay priced for it, and it is the same
 * shape as the o046 residual measured the same day where the spanned calls load
 * a0..a3 and the cost list starts at colour 7.
 *
 * That form is not a match and the reason is now isolated rather than diffuse:
 * it is exactly four bytes short, aligned 593 byte-exact / 10 naming / 6
 * immediate / 54 structural, because the target emits `move a1,s5` followed by
 * `addiu a1,a1,12` where the one-argument candidate folds both into
 * `addiu a1,s5,12`.  With the arity changed, every one of the naming rows in
 * that region reads a1 on both sides.  Seven advance spellings on top of the
 * one-argument form (`+= 6`, `&var_a1[6]`, a `(u32)` cast, a store after the
 * advance, two region boundaries and a re-assignment from the base) are all
 * identical at delta -4, so the missing `move` is not an advance-spelling
 * question.  Note also that `ext_o0_2d98` is declared here with NO prototype
 * and the third call site in this same function passes ONE argument, so the
 * two-argument reconstruction at these two sites is an m2c guess, not
 * evidence; the ROM cannot distinguish a two-argument call whose a1 already
 * holds the pointer from a one-argument call.
 *
 * The decision variable is therefore: what emits the target's separate
 * `move a1,s5` at +0x17C once a1 is available to the pointer web.  Nine
 * call-site spellings of the two-argument form (an inline base expression at
 * both sites, `&var_a1[0]`, a `(u32)` cast round trip, `(void *)`, `(u8 *)`, a
 * second local carrying the same value, a third argument and swapped
 * arguments) are all 29 or worse, because uopt folds every one of them back
 * onto the same web and the mask does not move.
 *
 * 2026-09-12, lane p8-arity: 29 -> 9, and the arity lever is the first half of
 * it.  Two edits, both measured on the instrumented allocator before they were
 * scored:
 *
 *   - Dropping the second argument from BOTH `ext_o0_2d98` calls clears bit 27
 *     of web 40's forbidden mask (0x7803e000 -> 0x7003e000) and globalcolor
 *     then reports bestcolor=4 bestreg=a1 at the same save 2.666667, exactly as
 *     the previous pass predicted.  The refinement this pass adds is WHICH call
 *     does it: dropping the argument at the case-2 site alone leaves the mask
 *     at 0x7803e000 and the pointer on a2 (28 masked, delta 0, and it does buy
 *     the target's `nop` delay slot); dropping it at the 0x24 == 0 else site
 *     alone clears the bit on its own.  One spanned call decides the colour,
 *     not the set of them.  With a1 held by the pointer web the ROM cannot
 *     distinguish a one-argument call from a two-argument one whose a1 already
 *     holds the pointer, so the one-argument form is the evidence-preferred
 *     reconstruction, not merely a cheaper one.
 *   - The four-byte deficit the previous pass named is a SPLIT ADVANCE.  uopt
 *     folds a single in-place advance into its rematerialisation
 *     (`addiu a1,s5,12`); two successive in-place advances leave the copy
 *     standing and combine only the constants, which is the target's
 *     `move a1,s5` at +0x17C followed by `addiu a1,a1,12` at +0x18C.  Every
 *     two-step spelling measured is byte-identical -- `+= 3; += 3`,
 *     `++; += 5`, `+= 1; += 5`, `+= 2; += 4`, `+= 5; += 1`, `&var_a1[1]` then
 *     `&var_a1[5]`, and the byte forms -- so the adopted `*var_a1++ = 0xC;`
 *     with `var_a1 += 5;` is chosen for being the natural display-list idiom.
 *     Delta returns to 0 and the score is 15.
 *   - The five payload stores are then a physical-line question ([L59]).  All
 *     120 orders were compiled: the two that reach 9 both put the -2 spA0 store
 *     LAST, and -0xA, -8, -6, -4, -2 is the memory order.  The base order is 15
 *     and the worst is 33.
 *
 * Flat on top of that 9, all at delta 0 unless noted: the 41-form lattice of
 * advance spellings, store spellings, seven [L97] region placements plus two
 * region wrappings, definition positions 217 to 222, extra reaching definitions
 * in one arm and in both, and an integer-carrier round trip -- every one of
 * them returned a byte-identical object on the one-argument base.  The 16-cell
 * carrier lattice for the temp_a0 web (sites {case-0 +0x48, case-2/3 +0x3E0} x
 * carriers {temp_a0, var_a1, spA0, inlined}) has its floor at the retained
 * form; routing either site through var_a1 breaks the a1 colouring outright
 * (first mismatch moves to +0x70) and spA0 costs 8 bytes.  `CDX_FORCE=p1:w40=s`
 * is ACCEPTED with CDX_PROC set and gives delta +4 at 444 aligned-exact, so the
 * target does not split web 40 either.
 *
 * The 9 that remain are two families and nothing else: 7 words of temp_a0's own
 * web, which sits on a0 where the target reads v0 (the case-0 +0x48 read at
 * +0x330 and the +0x3E0 pointer at +0x630), and 2 words at +0xA8 where as1
 * schedules the `move a1,s5` one slot after `sh t8,44(s0)` and the target puts
 * it one slot before.
 *
 * Both are now bounded rather than merely named, and neither is a single
 * colouring decision:
 *
 *   - A full greedy force ceiling over this procedure -- all 64 p1 decisions
 *     against colours 1..22 and 24..30 and the split path, 1,470 objects of
 *     which 938 were accepted and 532 declined byte-identically -- finds
 *     NOTHING better than 9.  The best accepted cell is 12.  So the residual
 *     is not one globalcolor decision and a colour lattice is not the lever.
 *   - Web 28 is the merged `temp_a0` symbol.  Forcing it to t0 moves the
 *     +0x48 read, the +0x3E0 read AND the `ext_o0_2d98` argument together,
 *     which is what makes it one web; its `p1cost` list offers v1 at infinite
 *     cost, a0 at zero (the argument affinity), t0..t5, c13 and s3..s8, and
 *     omits v0, a1, a2 and a3 entirely.  v0 is the L101 call-result denial,
 *     not interference, so no force and no arity change reaches it; the web
 *     has to become shorter, and the only split available is a twelfth
 *     declaration, which is 0xB0.
 *   - The +0xA8 slot is not a source-order tie: all 120 permutations of the
 *     five head-block statements including the pointer's definition score
 *     exactly 9, and so do twelve physical-line foldings of adjacent pairs
 *     ([L59]/[L132]).  The pair simply moves; it never closes.
 * 38 relocation identities are diagnostic. */
/* Ownership trial (2026-08-28): fixed the TU's +0x80..+0xA0 .rodata range;
 * linked promotion is text-differs with 660 in-range words, first at +0x0.
 * Module growth is cleared; the remaining gap is codegen/register allocation. */
#ifdef NON_MATCHING
void func_overlay_086_F0000474_18D22AC(void *arg0, s32 arg1) {
    f32 temp_f20;
    s32 spA0;
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 temp_f20_2;
    s32 temp_a0;
    s16 *var_a1;
    void *temp_s0;
    void *temp_s4;
    void *temp_v0;

    temp_f20 = (f32) arg1;
    temp_s0 = M2C_FIELD(arg0, void **, 0x64);
    M2C_FIELD(temp_s0, s16 *, 0x28) = 0x2000;
    temp_v0 = ext_o0_53d0(M2C_FIELD(temp_s0, u8 *, 1));
    if (temp_v0 != NULL) {
        temp_s4 = M2C_FIELD(temp_v0, void **, 0x64);
        spA0 = 0;
        temp_a0 = M2C_FIELD(temp_s0, u8 *, 0);
        if (temp_a0 != 0) {
            ext_o0_5a758(arg0, 0x3C88CE70, temp_f20);
            M2C_FIELD(temp_s0, s16 *, 0x28) = 0x22;
            M2C_FIELD(temp_s0, s16 *, 0x2C) = 0x24;
            M2C_FIELD(temp_s0, s16 *, 0x2A) = M2C_FIELD(temp_s0, s16 *, 0x1C);
            M2C_FIELD(temp_s0, s16 *, 0x2E) = M2C_FIELD(temp_s0, s16 *, 0x1C);
            var_a1 = (s16 *)((u8 *)temp_s0 + 0x30);
            if (M2C_FIELD(temp_s0, s16 *, 0x24) != 0) {
                M2C_FIELD(temp_s0, s16 *, 0x26) = (s16) (M2C_FIELD(temp_s0, s16 *, 0x26) + (arg1 << 9));
                if (M2C_FIELD(temp_s0, u8 *, 0) != 2) {
                    M2C_FIELD(temp_s0, s16 *, 0x24) = (s16) (M2C_FIELD(temp_s0, s16 *, 0x24) - (arg1 * 0x11));
                    if (M2C_FIELD(temp_s0, s16 *, 0x24) < 0) {
                        M2C_FIELD(temp_s0, s16 *, 0x24) = 0;
                    }
                    temp_a0 = M2C_FIELD(temp_s0, s32 *, 0x40);
                    if (temp_a0 == 0) {
                        ext_o0_2b90(0x1BE, M2C_FIELD(arg0, f32 *, 0xC), M2C_FIELD(arg0, f32 *, 0x10), M2C_FIELD(arg0, f32 *, 0x14), 1, (u8 *)temp_s0 + 0x40);
                    } else if (temp_a0 != 0) {
                        ext_o0_2d70(temp_a0, M2C_FIELD(arg0, f32 *, 0xC), M2C_FIELD(arg0, f32 *, 0x10), M2C_FIELD(arg0, f32 *, 0x14));
                    }
                } else {
                    temp_a0 = M2C_FIELD(temp_s0, s32 *, 0x40);
                    if (temp_a0 != 0) {
                        ext_o0_2d98(temp_a0);
                    }
                }
                spA0 = (s32) (ext_o0_2a470(M2C_FIELD(temp_s0, s16 *, 0x26)) * 1024.0f);
                *var_a1++ = 0xC;
                var_a1 += 5;
                M2C_FIELD(var_a1, s16 *, -0xA) = (s16) M2C_FIELD(temp_s0, s16 *, 0x24);
                M2C_FIELD(var_a1, s16 *, -8) = 3;
                M2C_FIELD(var_a1, s16 *, -6) = (s16) (M2C_FIELD(temp_s0, s16 *, 0x24) * 2);
                M2C_FIELD(var_a1, s16 *, -4) = 0xB;
                M2C_FIELD(var_a1, s16 *, -2) = (s16) spA0;
            } else {
                temp_a0 = M2C_FIELD(temp_s0, s32 *, 0x40);
                if (temp_a0 != 0) {
                    ext_o0_2d98(temp_a0);
                }
            }
            *var_a1 = 0x2000;
        }
        if (arg1 != 0) {
            temp_f20_2 = 0.05f;
            do {
                switch (M2C_FIELD(temp_s0, u8 *, 0)) {
                case 0:
                    if ((M2C_FIELD(temp_s4, u8 *, 0x170) != 0) && !(M2C_FIELD(temp_s4, u16 *, 0x1A8) & 1)) {
                        overlay86ScaledVectorPosition(temp_v0, temp_s4, &sp9C, &sp98, &sp94);
                        M2C_FIELD(arg0, s16 *, 0) = (s16) (M2C_FIELD(temp_v0, s16 *, 0) + 0x8000);
                        M2C_FIELD(arg0, s16 *, 2) = -0x1000;
                        M2C_FIELD(arg0, s16 *, 4) = 0;
                        M2C_FIELD(arg0, f32 *, 0xC) = sp9C;
                        M2C_FIELD(arg0, f32 *, 0x10) = (f32) (sp98 + 320.0f);
                        M2C_FIELD(arg0, f32 *, 0x14) = sp94;
                        M2C_FIELD(arg0, s16 *, 0x2E) = ext_o0_f690(M2C_FIELD(arg0, f32 *, 0xC), M2C_FIELD(arg0, f32 *, 0x10), M2C_FIELD(arg0, f32 *, 0x14));
                        M2C_FIELD(temp_s0, s16 *, 2) = 0x3C;
                        M2C_FIELD(temp_s0, u8 *, 0) = 1U;
                        M2C_FIELD(temp_s0, f32 *, 0x10) = 0.0f;
                        M2C_FIELD(temp_s0, s16 *, 0x24) = 0;
                        M2C_FIELD(temp_s0, f32 *, 0x14) = 320.0f;
                        ext_o0_494ac(M2C_FIELD(temp_s0, u8 *, 1), 0x3F4CCCCD, -1.0f, 0, 0, 0, 0);
                        ext_o0_28d88(0x5A);
                        M2C_FIELD(arg0, s16 *, 6) = (s16) (M2C_FIELD(arg0, s16 *, 6) & 0xFBFF);
                        ext_o0_5a914(arg0, 1, -1, 0.0f);
                        temp_a0 = (s32) M2C_FIELD(temp_v0, void **, 0x48);
                        M2C_FIELD((void *) temp_a0, u16 *, 6) &= 0xFFFE;
                    } else {
                        arg1 = 0;
                    }
                    break;
                case 1:
                    overlay86ScaledVectorPosition(temp_v0, temp_s4, &sp9C, &sp98, &sp94);
                    if ((M2C_FIELD(temp_s0, s16 *, 2) != 0) && (arg1 != 0)) {
loop_26:
                        M2C_FIELD(arg0, f32 *, 0xC) += (sp9C - M2C_FIELD(arg0, f32 *, 0xC)) * 0.1f;
                        M2C_FIELD(arg0, f32 *, 0x14) += (sp94 - M2C_FIELD(arg0, f32 *, 0x14)) * 0.1f;
                        M2C_FIELD(temp_s0, f32 *, 0x10) = M2C_FIELD(temp_s0, f32 *, 0x14) * temp_f20_2;
                        if (M2C_FIELD(temp_s0, f32 *, 0x10) > 10.0f) {
                            M2C_FIELD(temp_s0, f32 *, 0x10) = 10.0f;
                        }
                        arg1 -= 1;
                        M2C_FIELD(temp_s0, s16 *, 2) = (s16) (M2C_FIELD(temp_s0, s16 *, 2) - 1);
                        M2C_FIELD(temp_s0, f32 *, 0x14) = (f32) (M2C_FIELD(temp_s0, f32 *, 0x14) - M2C_FIELD(temp_s0, f32 *, 0x10));
                        if ((M2C_FIELD(temp_s0, s16 *, 2) != 0) && (arg1 != 0)) {
                            goto loop_26;
                        }
                    }
                    M2C_FIELD(arg0, f32 *, 0x10) = (f32) (M2C_FIELD(temp_s0, f32 *, 0x14) + sp98);
                    if (M2C_FIELD(temp_s0, s16 *, 2) == 0) {
                        M2C_FIELD(arg0, s16 *, 0) = (s16) (overlay86SelectPosition(temp_v0, temp_s0) + 0x8000);
                        M2C_FIELD(arg0, s16 *, 2) = -0x1000;
                        M2C_FIELD(arg0, f32 *, 0xC) = (f32) M2C_FIELD(temp_s0, f32 *, 4);
                        M2C_FIELD(arg0, f32 *, 0x10) = (f32) (M2C_FIELD(temp_s0, f32 *, 8) + 400.0f);
                        M2C_FIELD(arg0, f32 *, 0x14) = (f32) M2C_FIELD(temp_s0, f32 *, 0xC);
                        overlay86BuildTransform(arg0, temp_v0);
                        ext_o0_1bed0(temp_v0, M2C_FIELD(temp_v0, s32 *, 0xC), M2C_FIELD(temp_v0, s32 *, 0x10), M2C_FIELD(temp_v0, s32 *, 0x14), (s32) M2C_FIELD(temp_v0, s16 *, 0), (s32) M2C_FIELD(temp_v0, s16 *, 2), (s32) M2C_FIELD(temp_v0, s16 *, 4));
                        M2C_FIELD(temp_s4, s8 *, 0x191) = 1;
                        M2C_FIELD(temp_s4, u8 *, 0x170) = 0U;
                        M2C_FIELD(temp_s4, u16 *, 0x1A8) = (u16) (M2C_FIELD(temp_s4, u16 *, 0x1A8) & 0xFFF7);
                        M2C_FIELD(temp_s0, u8 *, 0) = 2U;
                        M2C_FIELD(temp_s0, s16 *, 0x24) = 0x400;
                        M2C_FIELD(temp_s0, s16 *, 2) = 0x5A;
                        ext_o0_494ac(M2C_FIELD(temp_s0, u8 *, 1), 0x3EB33333, 0.0f, 0, 0, 0, 0x80);
                        ext_o7_dbc(9);
                    }
                    temp_a0 = ext_o0_f690(M2C_FIELD(arg0, f32 *, 0xC), M2C_FIELD(arg0, f32 *, 0x10), M2C_FIELD(arg0, f32 *, 0x14));
                    if (temp_a0 != -1) {
                        M2C_FIELD(arg0, s16 *, 0x2E) = temp_a0;
                    }
                    break;
                case 2:
                case 3:
                    if ((M2C_FIELD(temp_s0, s16 *, 2) != 0) && (arg1 != 0)) {
loop_37:
                        M2C_FIELD(temp_s0, f32 *, 0x10) = (f32) ((M2C_FIELD(temp_s0, f32 *, 8) - M2C_FIELD(arg0, f32 *, 0x10)) * temp_f20_2);
                        if (M2C_FIELD(temp_s0, f32 *, 0x10) < -6.0f) {
                            M2C_FIELD(temp_s0, f32 *, 0x10) = -6.0f;
                        }
                        arg1 -= 1;
                        M2C_FIELD(arg0, f32 *, 0x10) = (f32) (M2C_FIELD(temp_s0, f32 *, 0x10) + M2C_FIELD(arg0, f32 *, 0x10));
                        M2C_FIELD(temp_s0, s16 *, 2) = (s16) (M2C_FIELD(temp_s0, s16 *, 2) - 1);
                        if ((M2C_FIELD(temp_s0, s16 *, 2) != 0) && (arg1 != 0)) {
                            goto loop_37;
                        }
                    }
                    overlay86BuildTransform(arg0, temp_v0);
                    M2C_FIELD(temp_v0, s16 *, 4) = (s16) spA0;
                    if (M2C_FIELD(temp_s0, s16 *, 2) == 0) {
                        if (M2C_FIELD(temp_s0, u8 *, 0) == 2) {
                            M2C_FIELD(temp_s0, s16 *, 2) = 0x1E;
                            M2C_FIELD(temp_s0, u8 *, 0) = 3U;
                        } else {
                            ext_o0_2b90(0x1BF, M2C_FIELD(arg0, f32 *, 0xC), M2C_FIELD(arg0, f32 *, 0x10), M2C_FIELD(arg0, f32 *, 0x14), 4, NULL);
                            M2C_FIELD(temp_s4, s8 *, 0x191) = 0;
                            M2C_FIELD(temp_s4, s8 *, 0x16C) = 1;
                            ext_o0_5a914(temp_v0, 0xC, -1, 0.0f);
                            M2C_FIELD(temp_s0, s16 *, 0x1E) = 0;
                            M2C_FIELD(temp_s0, s16 *, 0x20) = 0;
                            M2C_FIELD(temp_s0, s16 *, 0x22) = 0;
                            M2C_FIELD(temp_s0, s16 *, 2) = 0xB4;
                            M2C_FIELD(temp_s0, u8 *, 0) = 4U;
                            M2C_FIELD(temp_s0, f32 *, 0x10) = 0;
                            temp_a0 = (s32)M2C_FIELD(temp_s4, void **, 0x3E0);
                            if (temp_a0 != 0) {
                                M2C_FIELD((void *)temp_a0, u16 *, 0x10) &= 0xFFF7;
                                M2C_FIELD(temp_s4, void **, 0x3E0) = NULL;
                            }
                            M2C_FIELD(M2C_FIELD(temp_v0, void **, 0x48), u16 *, 6) |= 1;
                        }
                    }
                    temp_a0 = ext_o0_f690(M2C_FIELD(arg0, f32 *, 0xC), M2C_FIELD(arg0, f32 *, 0x10), M2C_FIELD(arg0, f32 *, 0x14));
                    if (temp_a0 != -1) {
                        M2C_FIELD(arg0, s16 *, 0x2E) = temp_a0;
                    }
                    break;
                case 4:
                    if ((M2C_FIELD(temp_s0, s16 *, 2) != 0) && (arg1 != 0)) {
loop_52:
                        M2C_FIELD(temp_s0, f32 *, 0x10) = (f32) (M2C_FIELD(temp_s0, f32 *, 0x10) + temp_f20_2);
                        M2C_FIELD(arg0, s16 *, 0) = (s16) (M2C_FIELD(temp_s0, s16 *, 0x1E) + M2C_FIELD(arg0, s16 *, 0));
                        M2C_FIELD(arg0, s16 *, 2) = (s16) (M2C_FIELD(temp_s0, s16 *, 0x20) + M2C_FIELD(arg0, s16 *, 2));
                        M2C_FIELD(arg0, s16 *, 4) = (s16) (M2C_FIELD(temp_s0, s16 *, 0x22) + M2C_FIELD(arg0, s16 *, 4));
                        if (M2C_FIELD(temp_s0, s16 *, 0x1E) >= -0x3F) {
                            M2C_FIELD(temp_s0, s16 *, 0x1E) -= 2;
                        }
                        if (M2C_FIELD(temp_s0, s16 *, 0x20) < 0x80) {
                            M2C_FIELD(temp_s0, s16 *, 0x20) += 8;
                        }
                        if (M2C_FIELD(temp_s0, s16 *, 0x22) >= -0x7F) {
                            M2C_FIELD(temp_s0, s16 *, 0x22) -= 8;
                        }
                        arg1 -= 1;
                        M2C_FIELD(temp_s0, s16 *, 2) = (s16) (M2C_FIELD(temp_s0, s16 *, 2) - 1);
                        if ((M2C_FIELD(temp_s0, s16 *, 2) != 0) && (arg1 != 0)) {
                            goto loop_52;
                        }
                    }
                    if (M2C_FIELD(temp_s0, s16 *, 2) == 0) {
                        M2C_FIELD(arg0, s16 *, 0x2E) = -1;
                        M2C_FIELD(arg0, s16 *, 6) = (s16) (M2C_FIELD(arg0, s16 *, 6) | 0x400);
                        M2C_FIELD(temp_s0, u8 *, 0) = 0U;
                    } else {
                        if (M2C_FIELD(temp_s0, f32 *, 0x10) > 5.0f) {
                            M2C_FIELD(temp_s0, f32 *, 0x10) = 5.0f;
                        }
                        if (M2C_FIELD(arg0, s16 *, 2) >= 0x4001) {
                            M2C_FIELD(arg0, s16 *, 2) = 0x4000;
                        }
                        if (M2C_FIELD(arg0, s16 *, 4) < -0x2000) {
                            M2C_FIELD(arg0, s16 *, 4) = -0x2000;
                        }
                        M2C_FIELD(arg0, f32 *, 0x24) = (f32) (-M2C_FIELD(temp_s0, f32 *, 0x10) * temp_f20);
                        ext_o0_29b94(arg0, (u8 *)arg0 + 0x1C);
                        M2C_FIELD(arg0, f32 *, 0xC) += M2C_FIELD(arg0, f32 *, 0x1C) * temp_f20;
                        M2C_FIELD(arg0, f32 *, 0x10) += M2C_FIELD(arg0, f32 *, 0x20) * temp_f20;
                        M2C_FIELD(arg0, f32 *, 0x14) += M2C_FIELD(arg0, f32 *, 0x24) * temp_f20;
                        temp_a0 = ext_o0_f690(M2C_FIELD(arg0, f32 *, 0xC), M2C_FIELD(arg0, f32 *, 0x10), M2C_FIELD(arg0, f32 *, 0x14));
                        if (temp_a0 != -1) {
                            M2C_FIELD(arg0, s16 *, 0x2E) = temp_a0;
                        }
                    }
                    break;
                }
            } while (arg1 != 0);
        }
        if (M2C_FIELD(temp_s0, u8 *, 0) != 0) {
            arg1 = M2C_FIELD(temp_s0, s32 *, 0x18);
            if (arg1 == 0) {
                ext_o0_2b90(0x16, M2C_FIELD(arg0, f32 *, 0xC), M2C_FIELD(arg0, f32 *, 0x10), M2C_FIELD(arg0, f32 *, 0x14), 1, (u8 *)temp_s0 + 0x18);
                arg1 = M2C_FIELD(temp_s0, s32 *, 0x18);
                if (arg1 != 0) {
                    ext_o0_2c4c(arg1, 0x7F);
                }
            }
            arg1 = M2C_FIELD(temp_s0, s32 *, 0x18);
            if (arg1 != 0) {
                temp_f20_2 = M2C_FIELD(temp_s0, f32 *, 0x10) * 8.0f;
                if (temp_f20_2 < 0.0f) {
                    temp_f20_2 = -temp_f20_2;
                }
                temp_f20_2 += 100.0f;
                if (temp_f20_2 > 150.0f) {
                    temp_f20_2 = 150.0f;
                }
                temp_f20_2 += (f32) ext_o0_2952c(-5, 5);
                ext_o0_2d70(M2C_FIELD(temp_s0, s32 *, 0x18), M2C_FIELD(arg0, f32 *, 0xC), M2C_FIELD(arg0, f32 *, 0x10), M2C_FIELD(arg0, f32 *, 0x14));
                ext_o0_2c64(M2C_FIELD(temp_s0, s32 *, 0x18), (u32) temp_f20_2 & 0xFF);
                if (M2C_FIELD(temp_s0, u8 *, 0) == 4) {
                    ext_o0_2c4c(M2C_FIELD(temp_s0, s32 *, 0x18), ((s32) (M2C_FIELD(temp_s0, s16 *, 2) * 0x7F) / 180) & 0xFF);
                }
            }
            spA0 = (s32) (M2C_FIELD(temp_s0, f32 *, 0x10) * 256.0f);
            if (spA0 < 0) {
                spA0 = -spA0;
            }
            spA0 += 0x800;
            if (spA0 >= 0x1001) {
                spA0 = 0x1000;
            }
            M2C_FIELD(temp_s0, s16 *, 0x1C) = (s16) (M2C_FIELD(temp_s0, s16 *, 0x1C) + (spA0 * (s32) temp_f20));
            return;
        }
        arg1 = M2C_FIELD(temp_s0, s32 *, 0x18);
        if (arg1 != 0) {
            ext_o0_2d98(arg1);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o086/func_overlay_086_F0000474_18D22AC/func_overlay_086_F0000474_18D22AC.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_086_F0000474_18D22AC:start
 * symbol: func_overlay_086_F0000474_18D22AC
 * score: 9 differing words
 * frame: 0xA8
 * relocations: 38
 * first-mismatch: +0xA8
 * summary: 29 fell to 9 on three edits whose cause was read off the instrumented allocator first. Dropping the second argument from the ext_o0_2d98 calls clears bit 27 of the command pointer web's forbidden mask and globalcolor gives it a1 at an unchanged save; the else-arm call alone is enough, the case-2 call alone is not. Splitting the pointer advance into two in-place steps stops uopt folding the rematerialisation into the add, which is the target's move a1,s5 plus addiu a1,a1,12 and returns delta to 0. Ordering the five payload stores by memory address, with the -2 spA0 store last, is worth six more. The 9 left are 7 words of temp_a0's own web on a0 against the target's v0 and one two-word as1 schedule slot at +0xA8.
 * PLATEAU-HANDOFF:func_overlay_086_F0000474_18D22AC:end
 */
