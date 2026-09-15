# The split is a growth test, and every remaining fragment is its verdict off by one: lane wv-t

**Retained: 18 masked / 195 raw differing words, 14,456 bytes, delta zero,
frame 0x138, first mismatch +0x12E4, 1,253 owned text relocations.**
Unchanged from wv-s. No source edit measured at delta zero below 18; the C
remains guarded `NON_MATCHING`.

What this lane adds is the instrument wv-s named, and what it reads. The
instrumented uopt now emits every decided web's block sets and the whole of
the allocator's *split* decision -- the seed, each candidate block with the
numbers the test was made from, the verdict, and every liveblock that moves.
With it, all three of the whale's remaining fragments read as **one growth
verdict each, off by one**, the two case-12 fragments are **coupled through a
single dead-carrier web**, and the coupling is an arithmetic identity that no
body with a dead carrier at block 183 can satisfy. That is the finding; the
rest of this file is its receipts.

## Budget

Twenty measured cycles were authorised; **twelve were spent**, listed at the
end. Cycle 0 (the compiler extension, the reader, and the readings) was taken.

## The instrument

The globalcolor profile in `n64-decomp-workbench`
(`src/decomp_workbench/instrument_uopt.py`, commit `d0830b8` there) gained
six records; the rebuilt `uopt` behind `~/Desktop/dev/ido-instrumented/`
passed the `.text` identity gate on every capture below (14,464 bytes,
byte-identical to the stock object). Two other changes were needed to get
there and are recorded here because they are outside this repository:

- `ido-static-recomp/libc_impl.c`: `wrapper_ecvt`/`wrapper_fcvt` were
  `assert(0)` stubs, so uopt's own `-Wo,-zdbug:2` listing aborted the moment
  it printed a float. Implemented; the listing now completes and its flow
  graph is what tied uopt's block numbers to the source (`uoptlist18.txt`
  is banked).
- `ido-static-recomp/build/5.3/uopt.c` regenerated from the pristine source
  (its SHA-256 matched the workbench pin) with `--profile globalcolor
  --profile alias`; the old generated file reproduced byte-for-byte before
  the change, so the diff is exactly the six records.

The records, all opt-in under `CDX_LOG`:

    webblocks phase proc role web sym lr bbs=<span> aux=<pass-through>
    seed      proc lr bb
    seedcand  proc lr pass bb f16 f18 f19 f20 maskdiff
    grow      proc lr bb new left_before left_after numintf strict
    growv     proc lr bb accepted
    livbb     proc op lr bb refs          op = del-seed | del-grow | mark-entry | mark-exit

A live range keeps two block bitvectors: `+0x14` is every block it is live
in, `+0xc` the blocks it merely passes through, so the difference is its
reference blocks; for a split piece those include the blocks where the piece
starts and stops. `lr` is the live-range object. **A web number is shared by
a parent range and the piece carved from it** -- the parent is decided
(`split`) under it, then the piece is decided under the same number -- so the
last `webblocks` row for a number is the decided piece, and the growth rows
join through its `lr`. `tools/lineage_census.py` reads all of it:
`--web N` prints a web's reference and pass-through blocks and its growth
with the margin of every step (`--log` reads a saved trace); `blocks_of()`
and `growth_of()` are the library entry points, and the census JSON now
carries `refs`/`livein` per web. 31 tests.

## The split, read off the recompiled uopt

`f_split` seeds a piece at the first reference liveblock in the parent's
list (list order is the CFG walk from the switch, so within a case it is
ascending), then `f_addadjacents` grows it breadth-first over the parent's
blocks. For each successor block of a queued block it counts the
interferences the block would add (`new`), folds the block's held colours
and, for a call block, the call's argument registers into the piece's
forbidden mask, recounts the colours left, and accepts the block only while

    new < left_before   and   2 * left_after >= numintf + new

A call block is accepted but never expanded. `strict` is the uopt byte at
`0x1001eb10` and reads 1 on this profile (with it clear, any block that
leaves a colour is accepted). After growth the piece is decided like any
web; a piece whose references are all rematerialisable can also simply be
dropped -- it never appears again in any record, its blocks are emitted
through `at`/lui-folded loads, and it does *not* linger in anyone's
interference list (checked: the dropped `&D_o058_5E9C` piece below is named
by no later `intf` row).

## Case 12, with block numbers

The `-zdbug:2` flow graph and the coloured symbol webs fix the map: 180-182
are the three calls (a call ends a block), 183 is `opponent = ...;
columnStep = ...; if (n > 0)`, 184 the title-loop preheader, 185 the loop
body's call, 186-188 the tail and its probe blocks, 190 the save block
(`savedPosition/savedOffset`, `i = 0`, `rowY = rowBase`, the guard), 191
the row-loop preheader, 192-201 the row loop, 202 the restore block
(`D_o058_5E9C = saved...; D_o058_5EA0 -= ...; if (< 0)`), 203 the zero
store.

**W** (`&D_o058_5EA0`, 9 rows): refs {183, 190, 202}, pass-through {184,
191}; that is the head `lui/addiu/lw`, a *dead* `lui/addiu` at +0x1354 that
the target also has, and the two restore stores. Its growth on the 18 body:

    seed 183
    184  new=1  left 15->14  numintf 20  ACCEPT
    190  new=2  left 14->14  numintf 21  ACCEPT
    185  new=0  left 14->11  numintf 23  reject     22 < 23, by one
    191  new=2  left 14->14  numintf 23  ACCEPT
    202  new=1  left 14->14  numintf 25  ACCEPT
    192, 203                             reject

On the 48 body (the same piece, `t0`): 184, 190, then **185 accepted**
(`left 15->12, numintf 22`: 24 >= 22), 191 rejected, 202 accepted. So **W is
`a2` because it does not absorb the title loop's call block**: with 185 in
the piece the call's `a2`/`a3` are forbidden and the piece takes `t0`; without
it `a2` is the lowest free colour. wv-s's "a2 is offered once numintf reaches
25" was the shadow of this test. The two numbers that differ between 48 and
18 at the 185 step are both the dead carrier `textY`: its web (s4, decided
early) is one more interferer at 183 and one fewer colour left.

**The `&D_o058_5E9C` 202-piece** (`v1`; the restore store, the reload in
204/205, the `else` branch): it exists only if the *earlier* `&D_o058_5E9C`
piece seeded at 183 -- refs 183 (the value load for `opponent`), 190, and
the same shape as W -- does not swallow 202 first. That piece is dropped
either way (its references rematerialise), but if it grows into 202 it takes
the restore store's liveblock with it, the later piece seeds at 216 instead,
and the store goes through `at`: +4. On the 18 body:

    seed 183 ; 184 ; 190 ; 185  new=0 left 16->12 numintf 24  ACCEPT
    191  new=1  left 12->12  numintf 24  reject   24 < 25
    202  new=1  left 12->12  numintf 24  reject   24 < 25      <- 202 survives

On 48: 185 (`left 17->13`), **191 accepted** (`left 13->12, numintf 23`:
24 >= 24), then 202 rejected (24 < 25). Two different routes to the same
good outcome, and on 18 the route runs through `textY`'s `s4` again: it is
the colour that makes `left` 12 instead of 13.

**The identity.** Write L1, N1 for W's colours-left and interference at its
185 step and L2, N2 for the 5E9C piece's at its 202 step. W needs
2·L1 >= N1; the 202-piece needs 2·L2 < N2 + 1. The two pieces span the same
blocks; L2 is read at line ~21041 of the trace and L1 at ~28402, and between
them `w40` (the `n*2` temp at 183) takes `v1`, so L1 = L2 - 1. Without a
carrier (48): N1 = 22, N2 = 23, L2 = 13, L1 = 12 -- both hold. With any dead
carrier at 183, N1 = 23 and N2 = 24; then W needs L1 >= 12 and the 202-piece
needs L2 <= 12, i.e. L1 <= 11. **No body with a dead-carrier web at 183
satisfies both.** `textY` (s4) lands L1 = 11, L2 = 12: 5E9C right, W wrong,
the 18 body. A fresh carrier `k` lands L1 = 12, L2 = 13: W right (`t0`),
5E9C wrong (+4), measured. The target therefore has no carrier web at 183,
which is the 48 configuration -- and the 48 configuration cannot put the
reset in call 1's delay slot by any mechanism in the shard's laws (the call
barrier on the fold; a duplicated def is dead-code-eliminated, re-measured
here as byte-identical to 48).

The one escape the identity leaves is timing: a web living in W's blocks
and coloured **`v1` before line 21041** would take L2 to 12 while leaving L1
at 12 (W loses `v1` to `w40` regardless). Every colour other than `v1` is
either already forbidden to both pieces or forbidden to both by the call in
185. A fresh carrier's web has save 1.0 and is decided late; raising it
needs loop-weighted occurrences, and a second dead def is eliminated before
range formation (cycle 8), while a live second role spans calls and cascades
(cycle 12). That is the exact shape of the next cycle, below.

**The explicit-pointer exit** (the shard's route for cases 7/11) was
measured too: a fresh `char **title` initialised inside the guard gives W
`t0`, the delay-slot reset and the constant cursor init, and costs exactly
one interferer at 184 -- the pointer's init is a reference of the
`&D_o058_5C98` address-constant lineage, whose remainder then counts at 184,
where the strength-reduced cursor's init did not. N2 becomes 24 again and
202 is swallowed. Four init spellings (`&arr[0]`, `arr + i`, `&arr[i]`, a
cast) are canonicalised to the same reference (cycle 7). The loop head also
schedules `lw a3,0(s1)` early in that form (one word pair).

## Case 13, with block numbers

234 is the head (`portraitX = ...`, the guard), 235-240 the first loop
(238 its call), 241 the save block with `portraitX = 0; i = 0`, 242-252 the
row loop, 253 the transition, 254/265/266 the `D_o058_5EB0` branches.

The `a2` piece (`&D_o058_5E9C`, refs 234, 236, 241, 253, 265) is seeded at
234 and on the 18 body accepts 235, 241, 236, 242, 253, 237, then **254 with
`new=2, left 15, numintf 28`: 30 >= 30**, then 265, 266. With `opponent` as
the coordinate the same walk reaches 254 at **numintf 29: 30 < 31**, the
piece stops at 253, is dropped, and a later piece seeds at 265 (`v1`): the
head `lui a2/lw a0` becomes a folded `lui a0/lw a0` and the transition loses
its shared register -- the one word and three extra words wv-s described.
The extra interferer is exact: on 18 `portraitX` is one web (`w22`, s1)
covering both the coordinate role (234-239) and its reset at 241; with
`opponent` as the coordinate that web is still at 241 (the reset) while
`opponent`'s web (s0) now covers 234-237, so two webs count where one did.
A private case-13 cursor does not change it (cycle 10). The target's row
loop has no second induction (wv-s), so its `portraitX` web is not at 241 --
which is the row-loop-on-`i` shape wv-s priced through the shared row-cursor
lineage. The alternative, keeping `portraitX` as the coordinate and letting
its save beat the cursor temp's 15.5, is closed by the ratio's own arithmetic:
a depth-1 probe adds 10 to `totalsave` and 1 to `nocs`, so the ratio
converges on 10 (measured 9.25 -> 9.40 -> 11.4 -> 11.17, cycle 11), which is
the ceiling wv-s recorded as 11.67.

## Cases 9/10

Not touched. The operand order of `addu s2,s1,t5` is not a split question;
the records carry nothing below the web.

## Closed this lane

- Any dead carrier at block 183, including a fresh scalar in either
  declaration position: the identity above (cycles 1, 2).
- A second `i = 0` at the 48 position or in the preheader: dead-code
  elimination of the first, byte-identical to 48 / +7 (cycle 3).
- A plain `i = 0;` before the first call: 2384 / +8 (cycle 4), as the shard
  says.
- A fresh pointer cursor, either declaration position, four init spellings:
  W `t0`, one interferer at 184, 202 swallowed (cycles 5, 7).
- `saved[2]` in place of the two saved scalars: uopt keeps the aggregate
  differently, -12 bytes (cycle 6).
- A second dead def of the carrier in the row loop: eliminated before range
  formation (cycle 8); a live second role: spans calls, cascades (cycle 12).
- `opponent` as coordinate, with or without a private cursor: 254 rejected by
  one (cycles 9, 10). Probes on `portraitX`: the ratio converges on 10
  (cycle 11).

## Cycle ledger

     1   columnX / portraitX carriers on 18, read with the instrument   2 cells: 2282 (+4), the 202 theft
     2   fresh carrier k, declared last / first                          2 cells: 2282 (+4) W=t0, 202 stolen; declared first also moves homes
     3   second i = 0 at 183 / inside the guard                          2 cells: 48 (identical to the 48 body) / 55
     4   plain i = 0 before call 1                                       1 cell:  2384 (+8)
     5   fresh pointer cursor, two declaration positions                 2 cells: 2288 (+4), W=t0, +1 at 184
     6   saved[2] array, both element orders                             2 cells: 2406 / 2410 (-12)
     7   pointer-init spelling lattice                                   4 cells: all 2288, same reference
     8   carrier with a second dead def in the row loop                  1 cell:  2282, def eliminated early
     9   opponent as the case-13 coordinate, read                        1 cell:  2201 (-4), 254 rejected by one
    10   ... with a private case-13 cursor                               1 cell:  2201, same
    11   portraitX loop probes x1/x2/x3                                  3 cells: 170 / 3064 / 3066, ratio -> 10
    12   carrier as the inner-loop subscript too                         1 cell:  3051 (+4), cascade

What cycle 13 is: **a web that lives in {183, 184, 185, 190} and is coloured
`v1` before the `&D_o058_5E9C` remainder splits at save 3.60.** Its
signature is a save above 3.6 with no call in its span. The candidates are
the `n * 2` temp `w40` (save 3.0 from two weight-1 occurrences at blocks 13
and 183; a third occurrence in a loop would carry it) or a carrier whose
loop-weighted occurrence survives dead-code elimination without making it
live across a call. Read the growth with `lineage_census.py --web` before
believing any score: the number to watch is `left_after` at the 202 step of
the piece seeded at 183, which must read 12.

## Reproduction

Cells were compiled from copies of the TU in a private scratch directory with
the tree's own `tools/ido/cc` and the configured flags (direct `cc`; the
stock object is byte-identical to the asm-processor build on this
candidate), then with the instrumented `cc` under `CDX_LOG=1 CDX_PROC=0
CDX_DETAIL_WEB=all CDX_LINEAGE_TABLES=all`; every pair passed the `.text`
identity gate and was scored with `nm_ranking.process_item` on the stock
object. The tree source was never written. Traces are banked under
`whale-resources/` in Git's common directory: `allocator18-growth.log` and
`allocator48-growth.log` (the two reference bodies with every record),
`allocator18-cell-{klast,ptr,opp,double}.log` with their sources
`cell18-*.c`, the three `allocator18-detail-w{1139,1100,1077}.log`
neighbour captures, `uoptlist18.txt`, and `cells-wv-t/` with every other
cell's result and source plus the scratch harness (`cellcc.py`,
`growth.py`, `growtrace.py`, `symwebs.py`, `side.py`). Gates ran through
`tools/gates.sh --staged`; ROM verification proves the assembly fallback,
not this C.
