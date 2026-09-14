# Roles, not colours: the shared cursor web and two role swaps, lane wv-r

**Retained: 48 masked / 221 raw differing words, 14,456 bytes, delta zero,
frame 0x138, first mismatch +0x1260, 1,253 owned text relocations.** Down
from 147. Aligned buckets move from 3498 exact / 108 naming / 1 immediate / 8
structural to **3597 / 11 / 1 / 8** (the aligner's structural count still
includes the two unpaired gap words). The single gap pair, candidate +0x12F8
against target +0x1260, is unchanged and is now the first mismatch. All 720
draws survive in their original order (the register sequence is identical
draw for draw); emissions 5,068 to 5,076. No executable bytes are promoted;
the C remains guarded NON_MATCHING.

Five source changes, each measured alone and then composed:

    entry store / decrement / draw-loop 5E50 web through one name   147 -> 85
    case 2 preheader: drop the probe between savedX and savedOffset    85 -> 83
    case 8: `0x5B + D_o058_5EAC + opponent * 0x1B`                     83 -> 82
    cases 12/13: `opponent` and `columnX` exchange roles               82 -> 63
    case 9 grid: `columnX = 0; do {` on one physical line              63 -> 60
    case 13: stride in `columnStep`, count in `columnCount`, two probes 60 -> 48

wv-q's principle -- uopt colours a symbol as one web function-wide, so *which
cases a variable spans* fixes the decision order -- did all the work. The
generalisation this lane adds: **a variable keeps its colour, so give each
role to the variable whose colour the target shows for that role.** Three of
the four residual blocks were roles assigned to the wrong carrier, not
colours assigned to the wrong web.

## Budget

Twenty measured cycles were authorised; eighteen were spent, listed at the
end. Sub-batches inside one hypothesis are counted as one cycle.

## The 5E50 web: the target's three loops share one name, and the reset must survive

wv-q's cycle 10 forced the merged entry/decrement `D_o058_5E50[i]` web to s0
and healed both windows; cycle 11 found the web takes a callee-save colour on
its own only when case 2's draw loop shares the name, at +4. Reading the
target's case 2 privately settles what the +4 was hiding: the target holds
the 5E50 cursor in **s0 in all three loops** (entry store, decrement, draw),
with `D_o058_5EC8` in s2 and `D_o058_5EF8` in s3 in the draw loop, and its
draw-loop cursors are **constant-initialised** (`lui`/`addiu` from the
symbol), while the visible counter s7 is not re-initialised in the
preheader. So the draw loop's subscript variable is known-zero at the
preheader, the visible counter is a second variable, and the subscript
variable is the same name in all three loops.

The 546-cell lattice over {entry index} x {decrement index} x {draw-loop
5E50 subscript} (cycle 1) shows why every earlier attempt cost width: any
draw-loop spelling through `i` makes uopt initialise the cursors from `i*4`
(`sll`/`addu`), because `i` reaches the preheader from two `i = 0`
definitions and uopt folds a constant only from a single reaching
definition. Writing `i = 0` in the preheader fixes the init and deletes the
two resets the target keeps (`move s7,zero` at +0x760 and +0x7D4): -4. And
any decrement index other than `i` lets uopt delete the post-loop `i = 0` as
redundant: -4 again (cycle 1's D-columns).

The spelling that satisfies all three constraints (cycle 3, twelve cells at
delta zero out of 96): the decrement loop counts **two** inductions --
`portraitX` as the subscript and loop test, `i` beside it so that the
post-loop `i = 0` is a real reset -- and the entry loop indexes by
`portraitX` as well. `portraitX` is already the draw loop's array induction,
so `D_o058_5E50[portraitX]` is one IR name in all three loops (L131), spans
the draw loop's calls, and takes s0 on its own: **85**, first mismatch moves
from +0x50 to +0x858, and the entry, decrement and draw windows are exact.
The four spellings (init order, increment order) compile to the same text.

## Two role swaps

**Cases 12/13.** The target keeps column X in s0 and the inner index in s1;
ours had `opponent` (inner index, save 141.5 over 8 blocks, s0) and
`columnX` (81.2 over 12, s1). Every carrier sweep for the inner index (cycle
7, 16 carriers x three scopes) is byte-inert or costs width, as wv-p found
at 160, because a dead carrier's save is decided by the inner loop alone.
Reading the matching windows instead: `opponent` is s0 in cases 8, 9 and 10
and `columnX` is s1 in case 9 -- in the target too, since those windows are
exact. So the target's column X in cases 12/13 is the s0 variable and its
inner index the s1 variable: **exchange the two roles** and neither web
changes colour. Cycle 10: **63** at delta zero, 19 naming rows healed in
+0x1200..+0x1800; the swap in case 12 alone measures 68, in case 13 alone 77.

**Case 13's count and stride (s5/s6).** The target keeps the stride in s5 in
both cases and the count in s6; ours had `columnStep` as case 12's stride and
case 13's count (s5) and `columnCount` as case 13's stride (s6). The plain
exchange measures 151-174 at delta zero with an s6/s7 shift through every
case: the records show why (cycle 15). After the exchange the count web is
124 over 6 = 20.67, below `i`'s 21.9, so `i` is decided first and takes s6.
The target's order is stride, count, `i`. One depth-1 probe on the count in
case 13's first loop lifts it to 134/6 = 22.33 -- but the exchange also
lowers the stride to 222/10 = 22.2, so the count now precedes the stride and
takes s5 (63). A second depth-1 probe, on the stride in **case 12's** first
loop, restores the order: stride 23.2, count 22.33, `i` 21.9 -- **48** at
delta zero (cycle 16). The same stride probe placed in case 13's first loop
instead measures 151: the probe's block joins the stride web there.

Both probes are `if (x != 0);` discarded reads (L109). The `x |= 0;` form
costs 24 bytes on a pointer role and 3-7 rows elsewhere.

## Two physical-line ties

Case 2's preheader loaded `D_o058_5EA0`'s address before `D_o058_5E98`'s;
the target the reverse. The probe `if (i != 0);` that an earlier climb had
placed between `savedX = ...` and `savedOffset = ...` was the tie-breaker;
removing it (or placing it before or after both) heals both rows: 83
(cycle 4). Case 9's grid preheader emits `columnX = 0` before the hoisted
format-string addresses; the target after. Putting `columnX = 0; do {` on
one physical line (L59) heals three rows: 60 (cycle 13). Neither statement
swap nor blank lines nor a one-line pair does it.

## Closed this lane

- Draw loop indexed by `i` in any form (cycles 1-2): width in every cell;
  the cursor init mechanism above is the reason.
- Case-12 first loop through `cursor` (cycles 9 and 12): on the 82 body the
  best cell closes the gap pair and costs 120 rows of colour (201); on the 63
  body every form is +24 bytes. The gap pair stays.
- Case 13 first-loop coordinate (target s0, cursor s1; ours reversed): every
  dead carrier is exactly -4 (cycle 11, as wv-p measured), depth-1 probes on
  `portraitX` cost 1,600+ rows (cycle 14), and a depth-0 block for the
  `cursor` web elsewhere is inert or worse (cycle 17). The pair is decided
  by `portraitX`'s scalar web (35/3) against the cursor's 31/2 and nothing
  measured moves either without width.
- The slot address `saves + index * 32` operand order (cases 9 and 10, two
  rows): fourteen spellings, all inert or worse (cycle 5).
- Case 8's `textY` expression order: only the `0x5B + D_o058_5EAC + ...`
  form moves a row.

## Cycle ledger

    1   index-carrier lattice, 546 cells (E x D x R)              6 at 147, rest width
    2   draw loop by `i`, 40 cells                                 -40..+12 bytes
    3   two-induction decrement loop, 96 cells                     85 (delta 0)
    4   case-2 preheader order x8                                  83
    5   operand order: case 8 x6, slot x14                         84; slot inert
    6   case-10 init order x7 (wrong window; see 13)               inert
    7   inner-index carriers x42, grid-9 coordinate carriers x16   inert / width
    8   case-13 count carriers x29                                 174 best
    9   case-12 cursor loop x26                                    201 best
    10  opponent/columnX role swap x16                             63 -- adopted
    11  case-13 coordinate carriers x14                            -4 each
    12  case-12 cursor loop on the 63 body x26                     +24 bytes
    13  case-9 grid preheader order x9                             60 -- adopted
    14  probes on portraitX in case 13 x13                         1687+
    15  count/stride exchange with count probes x16                63
    16  exchange with count and stride probes x12                  48 -- adopted
    17  cursor-web depth-0 probes x16                              inert / worse
    18  first-order landscape at 48                                banked

What cycle 19 would have been: the case-13 first-loop pair by the same
principle -- read which of our variables the target keeps in s0 across a
*matching* window that neighbours case 13, and give the coordinate role to
it; `opponent` is s0 but every carrier costs the one word, so the question
is which word (the residual of `coord-countdownX` names +0x17CC and +0x17E4
as target-only and +0x199C, +0x1A40 as candidate-only: the coordinate's
increment leaves the loop tail and the row loop gains two words). The
landscape at 48 is banked for the colour side of that question.

## Reproduction

Cells were compiled from variant sources in a private scratch directory with
the configured command's compiler and flags (direct `cc`, which reproduces
the asm-processor object's text for this candidate; ~0.65 s per cell, eight
in parallel), scored directly against the target with a private
`nm_ranking` work directory, and the tree source was never written while a
landscape ran. The instrumented object is byte-identical to the stock one on
the 85, 82, 63 and 48 bodies. The 48 body's allocator log, draw census,
residual map, alignment and first-order landscape are banked at
`whale-resources/` under Git's common directory with the `48` suffix
(`allocator48.log`, `draws48.json`, `residual48.txt`, `align48.txt`,
`landscape48/`, `landscape48.txt`), beside `draws85.json`, `draws63.json`,
`allocator63.log`, `residual63.txt` and `align63.txt`. Gates ran through
`tools/gates.sh`; ROM verification proves the assembly fallback, not this C.
