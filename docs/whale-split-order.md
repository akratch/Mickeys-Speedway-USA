# The identity itemised, and case 13 closed under one colour: lane wv-u

**Retained: 18 masked / 195 raw differing words, 14,456 bytes, delta zero,
frame 0x138, first mismatch +0x12E4, 1,253 owned text relocations.**
Unchanged from wv-s. No unforced cell measured below 18 at delta zero, so the
tree source is untouched and the C remains guarded `NON_MATCHING`.

What this lane adds is two readings that turn the whole 18 into arithmetic
with named terms, and one **forced witness at 11 masked / delta zero**
(aligned 3603 exact / 11 naming / 0 / 0: W's nine rows and the two slot rows,
nothing else) that shows case 13's seven rows closing completely once one
web -- portraitX's function-wide web of folded index resets -- is kept off
`a0`/`a1`/`a2`. The witness is forced (`CDX_FORCE=p1:w22=c6`, also `c7` and
`c18`); the lane did not find the source shape that colours that web
harmlessly on its own, and the last section says exactly what the shape has
to satisfy.

## Budget

Twenty measured cycles were authorised; **seventeen were spent**, listed at
the end. Readings taken without a compile (the neighbour-list itemisation,
the flow-graph blocks, the frame census) are not counted.

## Instruments added (banked under `cells-wv-u/`, never tracked)

- `readW.py LOG`: finds W (the coloured `&D_o058_5EA0` case-12 piece) by
  shape, rebases block numbers on where 183 landed, and prints every piece
  seeded there with its growth, plus the coloured webs of the window in
  decision order. Needed because any probe cell shifts the block map.
- `intfdiff.py LOG WEB WIN`: the interferer list at *each* decision of a
  `CDX_DETAIL_WEB` capture, restricted to a block window, with the set
  difference between consecutive decisions. This itemised the identity.
- `peels.py LOG SYM`: one lineage's peel history (seed, save, nocs,
  totalsave per decision).
- `colourdiff.py A B LO HI`: coloured webs matched across two traces by
  (type, reference blocks), showing colour, save and index changes.
- `case13.py LOG`: the case-13 `a2` piece's growth and the window's webs.
- `cellcc.py` now gives `nm_ranking` a private work directory per process:
  parallel cells sharing `build/nm_ranking` raced on `target.o`.

## Case 12: the identity, itemised

wv-t read the two case-12 fragments as one growth identity. From the banked
neighbour captures (`allocator18-detail-w1139.log`, which carries *both* of
W's decisions, and `allocator18-detail-w1100.log`) the identity is exact and
tighter than the dispatch assumed:

- In {183, 184, 185, 190} the 183-seeded `&D_o058_5E9C` piece counts 24
  interferers at its decision (idx 128) and W counts 23 at its own (idx
  268). Every member is shared except W's *own* lineage remainder
  (`w1096`), which never counts for W; the lineage remainders are renumbered
  between the two decisions (931 -> 1118, 926 -> 1187, 964 -> 1191, 38 ->
  1178, 0 -> 1179) but none leaves the window. **There is no transient
  interferer**, so `N2 = N1 + 1` structurally.
- The held colours differ by `v1` (w40, idx 137), `a0` (w251, 172) and `a1`
  (w222, 188), all decided between the two; the call at 185 folds `a0`-`a3`
  for both pieces, so `L1 = L2 - 1` exactly, and the 185 fold for W removes
  three colours (`a2`, `a3`, and whichever of `a0`/`a1` was not yet held)
  against four for the piece -- the "X" in earlier readings is just that.
- Consequence: **no body with a dead carrier web at 183 can pass both
  tests**, and with `L1 = 11` W fails 202 even at `N1 = 22`. The target is
  the 48 configuration on the allocator side.
- The one free variable inside the identity is the **order of w40 (3.0)
  against the remainder's 183 step (3.60 on every body measured, 126/35)**.
  Two head probes (`if (D_8007BEF8 * 2);` after `columnStep`) take w40 to
  4.0 and the piece then rejects 202 with `left 12` -- the piece side of the
  identity satisfied for the first time (cycle 2, cell p2). But every `if`
  probe inserts two basic blocks: block 183's interleaved schedule in the
  target (the `&D_o058_5EA0` `lui` between `lh t3` and `addu s5`) forbids a
  boundary there, and the blocks lower W's save (`nocs` 3, 0.67, decided at
  302) or rematerialise W's head read outright. Lowering the remainder
  instead, by moving `D_o058_5E9C` reads onto the adjacent `&D_o058_5E98`
  lineage (`(&D_o058_5E98)[1]`, identical bytes after link), reaches 3.20
  from case 2's loop (and costs 39 rows of case-2 colour), splits the
  `D_o058_5E9C` *value* web when applied to the head read (w251's `a0`
  becomes a ring temp, +4 on every body), and poisons alias analysis when
  applied to the row-loop negate (+969 aligned rows, though the order did
  flip and the piece then failed 185 by the same one colour).
- Bare expression statements (`D_8007BEF8 * 2;`, `(x) | 0;`) are dropped by
  cfe: byte-identical, no record moves. **Weight-0 occurrences do not exist
  in source.**

Case 12 therefore stands where wv-t left it, with the lever named and every
zero-width handle for it measured closed.

## Case 13: the whole cluster is one colour

The seven rows are loop 1's `s0`/`s1` swap: target `addu s0,t4,a1` /
`sh s0,132(sp)` (coordinate `s0`) and `lui s1; addiu s1` / `lw t6,324(s1)`
(cursor `s1`); ours the mirror. Read from the ladder:

- The cursor (`w436`, 31/2 = 15.50, idx 26) is decided before portraitX's
  web (`w22`, 37/4 = 9.25, idx 49) and takes `s0`. **`w22` can never be
  `s0`**: `w26`, the shared `D_o058_5E50[portraitX]` cursor (idx 22, `s0`),
  lives at blocks 9 and 85, which are two of portraitX's reset blocks. Three
  after-loop probes on the cursor bring it to 8.5 and `w22` still takes `s1`
  (cycle 10). A real coordinate def counts an occurrence, so a fresh
  coordinate is 31/3 = 10.3 and cannot beat the cursor either; **the only
  web that can is `opponent` (99.4, idx 2)** -- wv-r's role reading, now
  derived from the ladder.
- With `opponent` as the coordinate the `a2` piece fails 254 by one (wv-t):
  portraitX's web is *new* at 241. The zero-width fix is to make it counted
  at the seed: **loop 1 indexed by portraitX** (`D_800D31C8_o058Reloc[0x51 +
  portraitX]`, reset folded at 235, the `cursor` declaration kept for its
  frame cell). The piece then accepts 254 at 28 (cycle 15).
- On that body `w22` is seven folded resets {9, 17, 85, 93, 136, 235, 241},
  7/3 = 2.33, decided at ~150, and takes `a0` (120 masked). Forced to `a3`,
  `t0` or `s4` it measures **11 at delta zero**; `s3` is refused (the entry
  loop's `D_o058_5EF8` cursor), `s8` costs the prologue, `a1` 95.
- The cost of `a0`/`a1`/`a2` is itemised: `a0` collides with `w9`
  (`D_o058_5E94`'s value, `a0` over blocks 3-13, decided at 304, save 0.75)
  at block 9 -- the six `a1->a0` rows at +0x0 on every body where portraitX
  loses its loop-1 role (cycle 7, cycle 13's force from 36 to 31); `a1`
  collides with `w222` (`D_o058_5EA0` value, idx 188) at 241 and with the
  case-2/3 value webs at 85/93/136 (the opponent body's cascade, +27 rows);
  `a2` with the `a2` piece itself (idx 328).
- The ordering is what closes every natural route: `w22` would have to be
  decided after the `a2` piece (save 0.5) to be offered `a3`, or be
  callee-only (a call span) *and* decided after `rowBase` (`s2`, 0.54, at
  241) with `s1` held by the cursor -- and a web of dead defs has save >= 1.0
  (`nocs` runs about half the def count: 5 -> 2, 6 -> 3, 7 -> 3). Probes
  after a reset are not folded here (cycle 16: +5 to the total, +5 to nocs,
  and +36..+64 bytes), so there is no weight-0 occurrence to lower it.
- The row loop must keep the name `D_o058_5EE0[portraitX]`: it is one SR
  temp with case 3's (`w297`, 202/9 = 22.44, `s4`, idx 17), and it takes `s4`
  only because case 13's inner loop holds `s0`-`s3`; split from case 13 it is
  11.83, `s0` at 31, and case 3 pays a four-cycle (20 rows; cycle 7's
  row-on-`i` cells). So case 3's index and case 13's row index are the same
  variable, reset at 136 and 241, and both resets are members of `w22`.

Two more readings bound what the target can be:

- The entry, decrement and draw loops of cases 1/2 must share one index
  (cycle 14): indexed by `i`, the entry+decrement `D_o058_5E50[i]` web is two
  disjoint ranges with no call inside and takes `v1` (25 rows); the draw
  loop cannot use `i` (two reaching defs, wv-r). And whichever variable
  carries the entry reset sits at block 9 inside `w9`'s `a0` range, so that
  variable's web must also be harmless -- the same constraint as `w22`'s.
- The frame census is identical (35 slots, the same ladder), so the target
  has no pointer home we lack: pointer walks (cycle 11: +24..+56, frame
  moves) are not what it does.

So the target's case-13 side is fully determined -- coordinate `opponent`,
loop 1 indexed by the same variable that indexes case 3 and the row loop,
reset folded at 235 -- **except for how that variable's web of seven folded
resets is coloured `a3`, `t0` or `s4` rather than `a0`.** That is the
question the next cycle owns.

## Other closures this lane

- Carriers already live at 183 as the title-loop index (cycle 1):
  memory-class locals keep the reset as a store (`sw zero,232(sp)`), +4.
- Depth-0 probes in loop 1's preheader (cycle 8): every web live across the
  inserted blocks gains `nocs` (stride 23.2 -> 21.1, count 22.33 -> 19.1,
  `i` 21.31 -> 21.0), which breaks the stride > count > `i` order wv-r set;
  and a probe reading a constant is folded and adds only `nocs`.
- `columnX` or `textY` as the case-1/2 index (cycle 12): +60 / +20, the
  frame moves; `columnX` is assigned inside the draw loop.

## Cycle ledger

     1   carriers already live at 183 (savedPosition, savedOffset, rowY)     3 cells: 2417/2417/2282, all +4
     2   fresh carrier + n*2 probes after columnStep, six forms              6 cells: 2282; probes 2391..3535, blocks inserted; p2's piece rejects 202, W rematerialised
     3   the same probes above the head                                     3 cells: 2282 (+4); w40 2.67, nocs 3
     4   alias D_o058_5E9C through &D_o058_5E98, case 2 / case 3 loops      4 cells: 2321 (+4, 3.20, 39 case-2 rows) / 2706 (-16) / 2745 / 2330
     5   alias the head read at 183                                         3 cells: 2435 (+4) on k, ptr, 18: the a0 value web splits
     6   alias the row-loop negate at 192 (+194)                            4 cells: ~3000, delta -4/-8/+12: alias analysis poisoned; order flipped on the control
     7   case-13 row loop on i, with/without opponent coordinate            4 cells: 36 / 36 / 38 / 53 at delta 0; case 13 exact; w297 split (20), w22 a0 (6)
     8   depth-0 cursor probes in the preheader                             4 cells: 122 / 122 / 170 / 170: nocs of stride/count/i
     9   bare expression statements                                         5 cells: byte-identical, dropped by cfe
    10   after-loop cursor probes                                           3 cells: 2169/2154/2169 (-8); cursor 10.7/11.0/8.5; w22 still s1; count nocs +1
    11   cases 1/2 loops as pointer walks                                   4 cells: +56/+56/+36/+24, frame moves
    12   columnX / textY as the case-1/2 index                              3 cells: +60 / +20 / +60, frame moves
    13   force w22 = s1 / s2 on the 36 body                                 2 cells: 31 (delta 0, +0x0 heals) / +24
    14   entry+decrement on i, draw loops on a fresh z, probe variants      3 cells: 125 / +16 / +12; entry cursor v1; portraitX 11.0 (nocs 3)
    15   opponent coordinate + loop 1 indexed by portraitX                  1 cell: 120 unforced (w22 a0); forced a3 / t0: 11 at delta 0
    16   twelve probes after the resets, two layouts, two bodies            4 cells: +36..+64, not folded (w22 12/8)
    17   force w22 = s4 / s3 / s8 / a1 on the cycle-15 body                 4 cells: 11 / refused / +12 / 95

## What cycle 18 is

**Colour the seven-reset web harmlessly without a force.** The body is
cycle 15's (`cells-wv-u/c15.c`: opponent coordinate, loop 1 indexed by
portraitX, row loop on portraitX, `cursor` declared and unused); read
`w22`'s `p1cost` table and decision index on every cell, and the `a2` piece's
254 step. Three routes, none yet measured:

1. **A byte-free call span for the index variable**, making its cost table
   callee-only, *plus* `s1` and `s2` held at its blocks before it (the
   cursor at 235 is; `rowBase` at 241 is decided at 325, so the span must
   also push `w22` after 0.54, or `rowBase` must come first). `s4` is
   measured harmless.
2. **A source form whose index resets are not defs at colouring time** --
   the resets are ugen-emitted `move` instructions that as1 later deletes
   on both sides, so the target's source produces them too unless it
   initialises the index some other way. The frame rules out pointers.
3. **A different variable for the seven resets** whose existing web already
   holds `a3`/`t0`/`s4` (none of the current locals does: the case-9/10
   locals are `s0`/`s1`/`s2`/`s7`).

Read the colour before the score: `w22 reg=` in the `p1color` row, and
`forced=` must be `-2` (unforced) for the cell to count.

## Reproduction

As wv-t: cells compiled from copies of the TU in private scratch with the
tree's `tools/ido/cc` and configured flags, then with the instrumented `cc`
under `CDX_LOG=1 CDX_PROC=0 CDX_OUT=... CDX_DETAIL_WEB=all
CDX_LINEAGE_TABLES=all` (plus `CDX_FORCE` for the forced cells); every
unforced pair passed the `.text` identity gate. Scores from
`nm_ranking.process_item` on the stock object; forced scores on the
instrumented object. Banked at `whale-resources/cells-wv-u/` in Git's
common directory: every cell's source and `result-*.json`, the traces
`allocator-cell-*.log`, the harness, the neighbour captures
(`allocator-cell-ptr-dW.log`, `allocator-cell-opp-d1074.log`), the
case-13-exact 36 object (`case13exact36.o`), the cycle-15 unforced object
and the two forced 11 objects (`forced11-w22a3.o`, `forced11-w22s4.o`). The
tree source was never written; gates ran through `tools/gates.sh --staged`.
