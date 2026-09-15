# The reset web is coloured by choosing its symbol: lane wv-v

**Retained: 11 masked / 188 raw differing words, 14,456 bytes, delta zero,
frame 0x138, first mismatch +0x12E4, 1,253 owned text relocations.**
Down from 18. Aligned buckets move from 3596 exact / 18 naming / 0 / 0 to
**3603 / 11 / 0 / 0**, displacement tax 0. Unforced: `w22 forced=-2` on
every cell that counts. The C remains guarded `NON_MATCHING`; the ROM is
unaffected and `gmake verify` proves the assembly fallback, not this C.

The 11 is wv-u's forced witness reached without the force: W's nine rows
(`a2` for `t0`, +0x1280..+0x1500) and the two slot-address rows (+0x2C80,
+0x3400). Case 13's seven rows and the `+0x0` cascade are closed together,
because they were one question.

## What the question was

On wv-u's cycle-15 body (`cells-wv-u/c15.c`) the loop index of seven loops
is one symbol, `portraitX`, and its seven preheader resets are dead defs
after strength reduction that uopt still colours as **one web per symbol**
(`w22`, 7/3 = 2.33, decided at idx 171). Read off its `p1cost` row, every
caller-saved colour from `a0` up is offered at cost 0 and `a0` wins as the
lowest: no web decided before 171 holds `a0`, `a1` or `a2` at any of the
seven preheaders, and no preheader contains a call (`early.py` over blocks
9, 17, 85, 93, 136, 235, 241 lists `s0`, `s3`, `s7`, `v0`, `v1` holders
only). `a0` at block 9 then denies `D_o058_5E94`'s value (`w9`, decided
304), `w251` takes `a1`, `w222` takes `a2`, and the argument colours
cascade `a1->a0 / a2->a1 / a3->a2` through the function: 120 masked.

A dead-def web cannot be pushed later. `nocs = f(occ + bbcard)` where
`occ` is the count of reference blocks (L29), so seven dead defs in seven
blocks give 7/3 and n defs give n / (((n-2)>>2)+2) >= 2.33 for every n;
the webs it would have to follow (`w222` 2.0, `w9` 0.75, the `a2` piece
0.5) are all below that. And moving a reset out of its preheader does not
fold (cycle 1): the index stays live into the loop and the loop pays
+12/+36. So the web's colour is decided by which colours are *forbidden*
at its decision, and with no forbidding interferer available the only
lever left is the web's identity: **write the resets on a symbol whose
web is already coloured harmlessly**, so they join that web (L115/L131 in
the other direction: reuse imports the existing colour).

## The two carriers

The seven loops fall into two groups that must not share a symbol:

- **Cases 3 and 13** (blocks 136, 235, 241): `D_o058_5EE0[x]` is one SR
  temp across case 3 and case 13's row loop (`w297`, `s4`), and loop 1
  must be indexed by the same variable so the `a2` piece counts it at the
  seed (wv-u). The carrier is **`textY`**: case 1's text y, `s4` by its
  own web (`w127`, 15.9, idx 24), dead throughout cases 3 and 13. Merged
  it is 464/30 = 15.47, still `s4`, still decided at 27, and `s4` is what
  wv-u's forced witness measured harmless.
- **Cases 1 and 2** (blocks 9, 17, 85, 93): the entry loop, the
  decrement loop and both draw loops share names (`D_o058_5E50[x]` in
  entry/decrement/case-2 draw, `D_o058_5EF8[x]` in entry and *both* draw
  loops -- cycle 3 measured the split: with case 1's draw loop on another
  symbol the entry cursor `w27` moves from `s3` to `s2`). `textY` is live
  in case 1's draw loop (it is the y coordinate there), so this group needs
  a second carrier: **`letter1`**, case 10's second letter, `s1` by its own
  web (`w832`, 10.0, idx 39, blocks 604-605), dead everywhere in cases 1
  and 2. Merged it is 24/3 = 8.0 at idx 52 and keeps `s1` (letter0's `s0`
  at 603/605 is decided first), and `s1` is held by nothing at 9/17/85/93.

Which local is chosen matters exactly as the ladder says it should. The
`s0` carriers in the same cases (`letter0`, `portraitIndex`, `countdownX`)
find `s0` forbidden at 9 and 85 by the `D_o058_5E50` cursor, take `s1`
there, and so carry `s1` back into their own case: 15, with the cascade
gone and five new rows in case 10 or case 6. `savedPosition` (+12) and
`erase` (+92) change width; `columnStep`/`columnCount` (live in cases
12/13, dead in 1/2) lose a word in case 1 (-4).

## Cycle ledger

     1   case-13 loop-1 reset out of its preheader: before the head's third call, before the first, above the if/else join, inside the third call's last argument   4 cells: +36/+36/+12/+36, w22 spans 230-240 (live induction; a0-a3 forbidden, took s2 / a1)
     2   the seven resets renamed to textY                                              2 cells: 120 (cfe shadowed the duplicate `s32 textY;` -- byte-identical no-op) / 3358 delta -4 (real merge: s4 at idx 24, w9 heals, but textY is case 1's y and the draw loop breaks)
     3   textY for six resets, portraitX for case 1's draw loop                          1 cell: 3091 delta -4: the D_o058_5EF8 web splits, entry cursor s3 -> s2
     4   cases 1/2 on columnStep / columnCount / columnStep+probe / portraitX; cases 3/13 on textY   4 cells: 3050 (-4) / 3566 (+24) / 3064 (-4) / **16 at delta 0** (cascade healed but +0x0's five rows)
     5   cases 1/2 on savedPosition / highlighted / portraitIndex / letter0 / erase / countdownX    6 cells: +12 / 16 (no merge) / 15 / 15 / +92 / 15 (s0 carriers flip their own case)
     6   cases 1/2 on letter1                                                            1 cell: **11 at delta 0**, unforced; adopted
     7   force w40 off v1 (a0) on the 11 body, W read                                   1 cell: w251 takes v1 instead; W's growth byte-identical, still a2 (forced 69)
     8   case 12's title and row loops on the unused carrier, row reset inside the guard / at 190 / title only / with (u32)   4 cells: all one object, 2282 delta +4: W t0 (accepts 185), the 5E9C piece steals 202 (wv-t's k body)
     9   drop the unused `s32 portraitX;` declaration                                      1 cell: 284 at delta 0 -- the symbol order moves every later web number and its tie-breaks; keep it
    10   slot address: `saves + f()`, `[(u32) f()]`, byte arithmetic, `f()*32 + saves`, `saves + f()*32`, `&(*saves) + f()`, index through a dead local   7 cells: all byte-identical to 11
    11   neighbour captures (`CDX_DETAIL_WEB`) for W and the 5E9C piece on the 11 body     2 captures: itemised below

Cycle 0 (uncounted): the harness re-pointed at this worktree reproduced
wv-u's 120 on `c15.c` with `.text` identity, and the decision ladder,
`w22`'s cost row and the early-holder census were read from the banked
`allocator-cell-c15-a.log`.

## W, on the 11 body

The identity reads exactly as wv-u itemised it, and the neighbour captures
(`allocator-cell-c6a-detail-w1091-piece.log`, `...-w1130-W.log`) make it
arithmetic with named members. In the window {183, 184, 185, 190, 191, 202}
the `&D_o058_5E9C` piece (idx 127, 3.60) and W (idx 178, 2.26) each see 26
neighbours: the same 24 plus, for the piece, the `&D_o058_5EA0` remainder
and `&D_8007BEF8`'s remainder (`w922`), and for W the `&D_o058_5E9C`
remainder (`w1097`, at 191/202 only, so it counts only past 185) and
`w922`'s renumbered remainder. Coloured before the piece: `v0` (w19), `s0`
(opponent), `s1` (the title cursor, 184-185), `s4` (textY at 183; the row
loop's SR temp at 191), `s5` (columnStep), `s7` (i): 17 colours left at
the seed. Coloured between the two: `v1` (w40, idx 136) and `a0` (w251,
idx 171): W has 15. Growth: the piece accepts 184 (s1 folds, 16), 190
(16), 185 (a0-a3 fold, 12: 24 >= 24), rejects 191 and 202 by one; W accepts
184 (14), 190 (14), rejects 185 (a1-a3 fold, 11: 22 < 23), accepts 191 and
202 and colours `a2`. The `a0` between does not matter (the 185 fold takes
it either way); the `v1` is the whole margin.

What the next lane must not repeat, each measured here:

- **Freeing `v1` by moving w40 does nothing** (cycle 7): w251 (2.33, idx
  171) takes `v1` in its place, and W's growth is byte-identical.
- **A late carrier for case 12, live at 191 as well** (cycle 8): the
  target-looking configuration (W accepts 185 and takes `t0`, exactly the
  48 body's growth) but the 5E9C piece, with 18 colours at its seed, folds
  `a0`-`a3` to 13, rejects 191 by one (`2*12 < 24+1`, the s4 fold at 191
  never happens) and then takes 202 with 13: +4, wv-t's `k` result. A
  carrier at 191 is not "new" there (it is counted at the seed), so it
  does not change the 191 test.
- The arithmetic in one line: with any carrier web at 183 the piece needs
  17 colours at its seed and W needs 16, and the only colour that can
  separate them is `v1` -- **w40 (3.0) must be decided after W's split
  (2.26), not before the piece (3.60)**. Before the piece (a third `n*2`
  block, 4.5) costs the piece its 185 fold and it swallows 202. After W
  means `save(w40) < 2.26`, and `nocs = f(reference blocks + live-in
  blocks)` gives a two-block temp 6/2 whatever is added to it; or the
  `&D_o058_5EA0` remainder above 3.0 at its 183 seed, where it is 95/42
  because the seed walk carves case 13 (seeds 229, 234, 243, 245, 275:
  90 of its 185) before case 12. Neither has a zero-width handle in the
  source this lane can see.

## Two more closures

- **The unused `s32 portraitX;` declaration is load-bearing** (cycle 9):
  deleting it measures 284 at delta zero, first mismatch +0x0. An unused
  `s32` costs no frame, but its symbol number orders every later symbol's
  web number, and web number is the tie-break among equal saves (L100).
  Leave dead declarations where they are on this function.
- **The slot-address operand order is not spelled** (cycle 10): seven more
  forms at both sites -- pointer arithmetic, a `(u32)` index, byte
  arithmetic with `sizeof`, the explicitly reversed `f() * 32 + saves`, and
  the index through a dead local -- compile to the same object as
  `&saves[f()]`. uopt canonicalises the sum before ugen orders the
  operands; whatever puts `saves` first in the target is not the
  expression, and wv-r's fourteen plus these seven cover the expression.

## Reproduction

Cells compiled from copies of the TU in private scratch with the tree's
`tools/ido/cc` and the configured flags, then with the instrumented `cc`
under `CDX_LOG=1 CDX_PROC=0 CDX_OUT=... CDX_DETAIL_WEB=all
CDX_LINEAGE_TABLES=all`; every cell passed the `.text` identity gate.
Scores from `nm_ranking.process_item` on the stock object; the adopted
body re-scored with `tools/score_symbol.py` and `tools/align_symbol.py`
on the tree. Banked under `whale-resources/cells-wv-v/` in Git's common
directory: every cell's source, `result-*.json` and `allocator-cell-*.log`,
the 16 / 15 / 11 objects (`c4d-16.o`, `c5c-15.o`, `unforced11.o`), the
cycle-8 W-t0 object (`c8a-2282-Wt0.o`), the w40 force trace, the two
neighbour captures, the adopted source (`tree-adopted-11.c`), and
the readers `readsym.py` (a symbol's `p1dec`/`p1cost`/`p1color`/blocks by
sym), `early.py` (coloured webs decided before an index that touch a
block set), `mkcarrier.py` (the two-carrier body generator) and
`runcells.sh`. Note for the next reader: a second `s32 x;` declaration in
the same scope is accepted silently by cfe and shadows the first, so a
rename that leaves the old declaration in place measures as a no-op.
