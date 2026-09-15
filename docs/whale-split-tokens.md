# The gap closes on a split, not a colour: the case-12 reset and an interference token, lane wv-s

**Retained: 18 masked / 195 raw differing words, 14,456 bytes, delta zero,
frame 0x138, first mismatch +0x12E4, 1,253 owned text relocations.** Down
from 48. Aligned buckets move from 3597 exact / 11 naming / 1 immediate / 8
structural to **3596 / 18 / 0 / 0**; the gap pair (candidate +0x12F8 against
target +0x1260) is closed and the 30-word displacement tax with it. No
executable bytes are promoted; the C remains guarded NON_MATCHING.

Three edits in case 12, measured alone and composed:

    visible index reset through the first title-colour call's last argument,
    `(0xFF - (i = 0))`, wv-i's value-producing spelling             (with 2, 3)
    title loop subscripts by a DEAD carrier, `textY`, whose reset folds
    into the generated cursor's constant init and is deleted        48 -> -4 bytes
    a discarded read of a local that is dead in case 12, placed in the
    transition's zero-store block                                   -4 -> 18

The dispatch's first cycle was right about where the gap lived and wrong
about why it had been expensive. Every one of wv-i's "four bytes" was the
same kind of thing: an **address-constant lineage re-split** — a fragment of
`&D_o058_5EA0` or `&D_o058_5E9C` adjoining or shedding one block — and none
of it was a colour. The 18 that remain are three such fragments and nothing
else.

## Budget

Twenty measured cycles were authorised; fourteen were spent, listed at the
end. Cycle 0 (the two lineage readings) was taken; both are recorded below.

## The gap: `i` early, a dead subscript, and what that costs

On the 48 body the reset sat next to the loop because the loop subscripted by
`i`: uopt folds a known-zero index into the strength-reduced cursor's constant
init only from the loop's own block, so `i = 0` two calls earlier leaves the
cursor initialised from `s7` (`sll`/`addu`, +8; cycle 1's control). The target
resets `s7` in the first call's delay slot and initialises its `s1` cursor
from the symbol, so its subscript variable is not the count. A dead carrier
does exactly that: `D_o058_5C98[K]` with `K = 0` in the preheader and `K += 1`
in the tail is strength-reduced away entirely, the reset folds, and nothing of
`K` survives but its membership in the allocator's records.

That membership is the whole cost, and it is carrier-specific:

- Fourteen dead scalars (`portraitX`, `x`, `columnX`, ...) leave the
  `&D_o058_5EA0` fragments untouched and **shrink** an `&D_o058_5E9C`
  fragment: the restore's `sw` goes through `at` and the `else` branch's
  reload rematerialises, +4.
- `textY` and `columnCount` leave `&D_o058_5E9C` alone and **grow** the
  `&D_o058_5EA0` fragment W (head, row preheader, loop exit, restore stores)
  by the zero-store block, so `D_o058_5EA0 = 0` shares W's register instead
  of the target's `lui at` pair: -4.
- A live carrier (reset in blocks 180-182, cycle 3) keeps W's extent and
  costs the reset word plus a cursor derived from it (+8/+12).
- The three declared-pointer families and `cursor` shared with case 13
  (cycles 2, 4, 5) reproduce the target's loop head exactly and pay
  elsewhere; see below.

The probe closes the growth. `if (columnCount != 0);` — a discarded read of
a local that is never defined on case 12's path — placed before
`D_o058_5EA0 = 0` adds one live range to that block and W no longer adjoins
it (cycle 7: 40 cells, `columnCount` and `erase` both 18, every live local
inert or worse). The reset then sits in the delay slot, the cursor is
constant-initialised, the row loop is byte-identical to the target's, and
the two structural words are gone.

**The token is not a general tool.** It is zero-width only in that block:
the same read in the head, the row preheader or the loop emits a real load
of the variable's home (+4/+8, and +24 through `erase`'s own case-9 web);
a never-defined local declared for the purpose moves nine immediate rows by
its declaration alone (cycle 11, 22 cells). Why the zero-store block deletes
the read and the others do not is not explained here.

## What 18 is made of, and why it is three fragments

    +0x1200  5 naming   W's head (lui/addiu/lw) and row-preheader pieces: a2 for t0
    +0x1400  4 naming   W's loop-exit piece and the two restore stores:   a2 for t0
    +0x1600  7 naming   case 13 first loop: coordinate s1 for s0, cursor s0 for s1
    +0x2C00  1 naming   case 9  slot address, operands of a commutative addu swapped
    +0x3400  1 naming   case 10 slot address, the same

**W's colour is a pressure effect, read off the records.** In every trace
W's `regsleft` equals the length of its `p1cost` offer list, and `a2`/`a3`
appear in that list exactly when W's `numintf` is 25 or more (base 22,
`opponent`-coordinate body 23, shared-cursor body 22: `t0`; the `textY`
bodies 25-27, the live-`columnCount` body 25: `a2`). The choice is then the
lowest offered colour at equal cost, `c5` before `c7`. No web coloured `a2`
interferes with W in base, so this is not L142's spanned-call denial, and
the dispatch-era rule that "twice the available-register count" bounds a
colouring does not hold either: 56 of base's 140 coloured webs exceed it. The
three extra interferers on the 18 body are the dead carrier's own web (s4)
and two pieces of a neighbouring lineage whose split moved with it; the
records give counts, not members, so they were not named.

Forcing W to `t0` on the 18 body is accepted and costs +64 bytes: `t0` is a
ring temporary there (no web holds it), so the force pulls it out of the
free list. **Acceptance reads as `forced=<colour>` in the `p1color` row, not
`-1`** — the brief's statement is stale; a force that was never applied
reads `forced=-2`.

## Cluster B is one lever with one word attached

The target's case-13 first loop keeps its coordinate in `s0` and its cursor
in `s1`; ours are reversed. Reading the target: its `s0` there is the
register `opponent` holds through cases 8-13 as column X, its row loop has
**no second induction** (one `move s7,zero`, a constant-initialised `s4`
cursor), and case 13's row subscript shares its IR name with cases 2 and 3
(lineage 239:0 must keep 22.4/9 and `s4`; dropping case 13 from it gives
11.8/6 and `s0`, the case-3 four-cycle of cycles 4-5). So the coordinate is
`opponent` and `portraitX` stays the row induction: measured, the cursor
takes `s1` on its own (`opponent` forbids `s0` in loop 1) and the seven
naming rows go — for exactly one word. The `&D_o058_5E9C` fragment that
spans loop 1's head to the transition in base (`a2`) sheds the head piece:
`lui a0/lw a0` at +0x171C, then three extra words at +0x199C/+0x1A40/+0x1A44
where the target reuses `a2`. Every spelling of the head or row-preheader
assignment (17 cells, cycle 14), the row induction's reset in the guard
(cycle 12), and interference tokens in nine case-13 blocks (cycle 10) leave
that piece where it is. wv-r's fourteen dead coordinate carriers measured
the same -4; this lane names the piece.

The shared `cursor` (cycles 4-5) is the same story from the other side: it
gives case 12 the target's loop head byte for byte and moves case 13's
cursor to `s1`, but `portraitX` is then live against `opponent` in the row
loop, takes `s2`, and `rowBase` (1283:0, seven row-preheader blocks, `s2`)
is split — a ring-wide shift.

## Cluster C: the carrier is not the lever either

`slot = &saves[levelGetBlurEffect(...)]`: the target writes `addu s2,s1,t5`,
ours `addu s2,t5,s1`. Deleting `saves` and indexing the call result
directly, at either site or both, costs 10-18 rows and moves the call into
the subscript's evaluation order (cycle 13, 7 cells); the index-first
spelling is inert, as wv-r's fourteen were. The operand order is an IR
identity question below the level these records reach.

## Closed this lane

- wv-i's attempt 2 on the 48 body (cycle 1): the dead-carrier form lands the
  reset at +0x1268 with a constant-initialised cursor for every carrier; the
  cost is one lineage split per carrier, never the reset.
- Declared pointer cursors for case 12, fresh or dead-role, either loop test
  (cycle 2): `<` +4, `!=` delta zero at 183+ with the init above the guard;
  the shared `cursor` inside the guard is exact locally and pays through
  `rowBase`.
- Dead-reset placement in blocks 180-183 (cycle 3): position inside 183 is
  inert; outside it the reset survives.
- Additivity of the two fragment losses (cycle 6, 19 crosses): no pair
  cancels.
- Every dead carrier with the probe (cycle 8): `textY` alone reaches 18.
- W's interference by guarded reset or by tokens elsewhere (cycle 9): inert.
- Tokens as a general lever (cycles 10, 11): not zero-width outside the one
  block; a declared token costs nine rows unread.
- `opponent` as case-13 coordinate: fixes the pair, costs the head piece
  (cycles 4, 12, 14; 27 cells).
- `saves` carrier deletion (cycle 13).

## Cycle ledger

    1   attempt 2 on 48, 16 carriers x 2 tests + cursor + controls   36 cells: -4/+4/+24; textY-tK 159 at 0
    2   declared pointer cursors x 2 tests                            14 cells: +4 / 183
    3   dead-reset block placement, textY and columnCount             16 cells: -4 in 183, +8/+12 outside
    4   shared cursor x opponent coordinate x row induction            8 cells: oppRi 83, curne-oppRi 207
    5   shared guarded cursor x row loop on i                           6 cells: rowi 68
    6   reset forms x coordinate forms                                 19 cells: additive
    7   zero-store-block probes on the textY body                      40 cells: 18 -- adopted
    8   dead carriers x probe                                          32 cells: textY only
    9   W interference shaving on 18                                   13 cells: inert / worse
    10  case-13 tokens on the opponent body                             9 cells: no
    11  never-defined local as token, three bodies                     22 cells: closed
    12  opponent coordinate on 18, guarded reset, row on i              5 cells: 36 / -4
    13  saves carrier deletion, cases 9/10                               7 cells: 28-36
    14  head and row-preheader spellings, opponent coordinate           17 cells: all -4

What cycle 15 is: **a per-web block-set dump from the instrumented uopt**.
Both remaining fragment questions — W's three extra interferers, and which
carve detaches `&D_o058_5E9C`'s head piece when the coordinate is
`opponent` — are readings that `webdetail`'s `bb=-1` cannot give and that no
source lattice answered in 27 cells. With the members named, each is one
zero-width edit; without them, the next cell is a guess. The source-side
cycle, if the instrument is not extended: on the 18 body, split-force W
(`p1:wN=s`) and read whether the remainder's `numintf` falls below 24 — if it
does, the head piece is the interferer to detach, and the edit is to keep
`&D_o058_5EA0`'s head read out of the address web.

## Reproduction

Cells were compiled from variant sources in a private scratch directory with
the configured command's compiler and flags (direct `cc`, byte-identical
`.text` to the asm-processor object on this candidate; ~0.65 s per cell,
eight in parallel), scored with `nm_ranking.process_item` in a private work
directory and mapped with `residual_map.measure --object`; the tree source
was never written while a batch ran. Every instrumented trace passed the
`.text` identity gate against its stock object. The 18 body's allocator
log, lineage tables, residual map and alignment are banked at
`whale-resources/` under Git's common directory with the `18` suffix
(`allocator18.log`, `lineage18.json`, `residual18.txt`, `align18.txt`).
Gates ran through `tools/gates.sh --staged`; ROM verification proves the
assembly fallback, not this C.
