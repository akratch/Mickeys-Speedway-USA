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

Cycle 0 (uncounted): the harness re-pointed at this worktree reproduced
wv-u's 120 on `c15.c` with `.text` identity, and the decision ladder,
`w22`'s cost row and the early-holder census were read from the banked
`allocator-cell-c15-a.log`.

## W, on the 11 body

The identity reads exactly as wv-u itemised it, with the numbers moved by
nothing: the `&D_o058_5E9C` piece seeded at 183 splits at idx 127 (3.60 =
126/35) and accepts 185 with `left 16->12, numintf 24` (margin 0), then
rejects 191 and 202 by one; `w40` (the `n*2` temp at 13/183, 6/2 = 3.0)
takes `v1` at 136; W's remainder splits at idx 178 (2.26 = 95/42) and
rejects 185 with `left 14->11, numintf 23` (margin -1). Both need
`L = 12` at the 185 step, and they differ by precisely the `v1` taken
between them. The one ordering that satisfies both is **`w40` decided
after W's split**, i.e. `save(w40) < 2.26` or the `&D_o058_5EA0` remainder
above 3.0 at its 183 seed; neither is a dead-def or probe question (a
third `n*2` block gives 9/2 = 4.5, which puts `w40` before the *piece* and
fails W the other way), and the 5EA0 remainder's save at 183 is diluted by
its case-2/8 references, which the seed order (case 13, then 12, then 11
... 1) carves after case 12. See the handoff for what that leaves.

## Reproduction

Cells compiled from copies of the TU in private scratch with the tree's
`tools/ido/cc` and the configured flags, then with the instrumented `cc`
under `CDX_LOG=1 CDX_PROC=0 CDX_OUT=... CDX_DETAIL_WEB=all
CDX_LINEAGE_TABLES=all`; every cell passed the `.text` identity gate.
Scores from `nm_ranking.process_item` on the stock object; the adopted
body re-scored with `tools/score_symbol.py` and `tools/align_symbol.py`
on the tree. Banked under `whale-resources/cells-wv-v/` in Git's common
directory: every cell's source, `result-*.json` and `allocator-cell-*.log`,
the 16 / 15 / 11 objects (`c4d-16.o`, `c5c-15.o`, `unforced11.o`), and
the readers `readsym.py` (a symbol's `p1dec`/`p1cost`/`p1color`/blocks by
sym), `early.py` (coloured webs decided before an index that touch a
block set), `mkcarrier.py` (the two-carrier body generator) and
`runcells.sh`. Note for the next reader: a second `s32 x;` declaration in
the same scope is accepted silently by cfe and shadows the first, so a
rename that leaves the old declaration in place measures as a no-op.
