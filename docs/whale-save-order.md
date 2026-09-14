# Save order and the case-6 walker, lane wv-p

**Retained: 157 masked / 328 raw differing words, 14,456 bytes, delta zero,
frame 0x138, first mismatch +0x50, 1,253 owned text relocations.** Down from
160. Aligned buckets move from 3485 exact / 119 naming / 1 immediate / 8
structural to **3488 / 118 / 1 / 8** (the aligner's structural count includes
the two unpaired gap words; `residual_map.py` reports 6 paired structural
rows, down from 8). The single gap pair, candidate +0x12F8 against target
+0x1260, is unchanged. All 720 draws survive in their original order
(emissions 5,070 to 5,068). No executable bytes are promoted; the C remains
guarded NON_MATCHING.

The 160 instrument set is now banked at `whale-resources/` under Git's common
directory with the `160` suffix (footprints, landscape, lattice, lineage,
draws, residual, allocator), and the 157 set beside it. Both landscapes carry
a source fingerprint and `web_footprint --report` stamps them clean against
the tree they were measured on.

## Budget

Twenty measured cycles were authorised; fourteen were spent. Cycle 0 (the
bank) is not counted. Every cycle is listed at the end with its result.

## Two dispatch levers that do not exist on this function

**`p2` forces are inert here.** The banked allocator trace carries 35,336
`phase=p1` records and zero `phase=p2`. A `p2:w75=c16` cell returns "expected
one decision and one final colour record for web 75" and its own trace has no
p2 row. This is the brief's call test: a procedure that issues calls emits p1
records only. The dispatch ranked this lever first; the brief is the authority
and the dispatch was stale on this point.

**The joint-force grammar never reaches the compiler.** `force_lattice.py`
accepts `p1:w225+w379=c20`, and the instrumented `uopt` refuses it:

    CDX_FORCE entry "p1:w225+w379=c20" is not a phase-qualified force
    control; write p1:w9=c30 ... or p1:w9=s to force the split path.

Every joint cell in the campaign's records would have failed the same way
(exit 55). What the lattice tool calls a joint force is only reachable as two
single `--force` flags in one cell, which the tool already does. The same
message names a grammar nothing in `tools/` uses: **`p1:wN=s` forces the
split path**. It works (`p1:w22=s` spills the case-13 inner index to
`sp+304` at +4 bytes), but `force_lattice.force_acceptance` cannot parse a
split receipt (`invalid literal for int()`), so it reports the cell as not
accepted while the object is real. That is the instrument for the 254 split
decisions the plan says no force addresses; it needs a parser fix before a
lane can use it.

## The decision order is descending `save`, and that names every swap

The 437 `p1dec` records are in decision order. Sorted by `save`, they have 46
inversions, and every one is a split web or a split fragment being
recoloured; among coloured webs the order is strictly descending save. So
"which of two webs takes s0" is arithmetic on the two `totalsave/nocs`
ratios, and a pair swap in the residual is a pair of ratios in the wrong
order.

Identification was done by forcing each candidate web to a same-kind colour
and word-diffing the object against the unforced one (cycles 2 and 3, 76
compiles). The webs behind the remaining naming rows:

    web    reg  save     nocs  value
    w22    s0   141.6     8    opponent (cases 1/2 copy, 8, 9, 10, 12, 13)
    w889   s0   152.5     4    coalesced with opponent's case-12/13 inner index
    w372   s1    79.5    12    columnX (cases 9, 12, 13)
    w384   s0    15.5     2    generated D_o058_5C98 cursor, case 12 first loop
    w379   s5    23.5    10    columnStep (stride in case 12, count in case 13)
    w75    s0     9.5    16    entry-array cursor, cases 1 and 2
    w259   s1     6.8     6    D_o058_5EF8 cursor, case 1
    w935   s3     1.3    16    the constant 5 (every literal 5 in the function)
    w63    s0    16.5     4    portraitX as a scalar: case-6 walker and case-13
                               first-loop coordinate
    w435   s1    15.5     2    cursor, case-13 first loop
    w577   s1     1.3     3    countdownX constants, case 6

**Cases 12 and 13, s0/s1.** The target has `columnX` in s0 and the inner
index in s1; ours is the reverse because `opponent` (141.6) is decided at
position 2 and `columnX` (79.5) at position 3. Forcing `w22=c15` lets
`columnX` take s0 for free and repairs 19 rows in +0x1300..+0x1900, but costs
45 in cases 8-10 where the target also has `opponent` in s0: 186 either way,
and 151 when added to the packed quartet (125 alone). A fresh inner-index
name for cases 12/13 (v401) becomes web 403 with **save 164 over 5** and is
decided first anyway; `opponent` without those cases drops to 78.25. Six
existing dead carriers do not change the order (`portraitIndex` and
`letter0` reproduce 160 exactly). `rowBase` costs 20 bytes, `textY` 4, `x`
280. So the target's source has `columnX` above ~165 or its inner index
below ~80, and no renaming reaches either from this body. Without case 9,
`columnX` measures 130.2 over 5 (v805), still short.

**Case 13, s5/s6.** `columnStep` is one web across the case-12 stride and
the case-13 count; the target keeps the stride in s5 in both cases and the
count in s6, so they are two webs there. Exchanging the two names inside
case 13 measures 251 and a fresh count name 259.

**Case 13 first loop, s0/s1 — and why 157 is a composite.** `portraitX`'s
scalar web (w63) is 66/4 = 16.5 at 160, one step above the cursor's 15.5, so
it is decided first and takes s0 as the target does. Its members are the
case-6 walker and the case-13 coordinate; its case-1/2/13-row index roles
are absorbed by strength reduction and belong to expression webs.

## The improvement: case 6's walker is compiler-generated

The target's case-6 node X is a temporary (`addiu v1,v1,40`), not a saved
register, and `countdownX` sits in s0. Writing

    nodes[i].x = countdownX + D_o058_5EA8 + i * 0x28;

with no `portraitX` walker lets strength reduction generate it (L160): case 6
heals completely (+0x2300 four naming and two structural rows, +0x2400 three
naming rows, nine rows in all), `countdownX` takes s0, and w577's three
constants agree. Draw count and order are unchanged; two producer events
disappear at the removed lines.

The cost is exactly the arithmetic above: w63 loses its case-6 member and
falls to 35/3 = 11.67, the cursor is decided first, and case 13's first
loop swaps s0/s1 (+6 naming rows at +0x1700). Net −3 positional, −3 aligned
rows, and 157 is the composite.

Everything tried to keep both (cycles 9-12) fails:

- keeping `portraitX` as the case-6 base (`portraitX + i * 0x28`): 12.33,
  case 13 still swapped, and the base costs two structural rows — 159;
- probes on `portraitX` in case 13's loop: one probe 252, two 246, three
  +8 bytes; the raised ratio reorders it against webs it shares with
  cases 1/2;
- probes in case 6 are loop-invariant, hoist to depth 0 and change nothing
  (159);
- any other carrier for case 13's first-loop coordinate costs **exactly four
  bytes** (`columnX`, `highlighted`, `countdownX`, `letter0`: 2322 at −4), so
  the target's coordinate variable is the same one as its row-loop entry
  index, as ours is; `textY` 220, `erase` 259 at width;
- moving any of the three increments ahead of the `func_8002F618` call opens a
  new gap pair in case 13 (163-170) or costs 4 bytes.

The next constraint is precise: **raise `portraitX`'s scalar web above 15.5
without a declared walker anywhere**, or lower the case-13 cursor's 31/2
below 11.67. Neither a probe nor a carrier does it from this body.

## Other closed cells

- Generated inner-loop coordinates (`columnX + opponent * columnStep` and the
  first-loop analogues): IDO emits the multiply for an index times a
  loop-invariant stride; +20 to +88 bytes in every combination.
- Case 13's cursor back to `D_800D31C8_o058Reloc[0x51 + i]`: +48 (`!=`) and
  +52 (`<`), as the in-source note predicted.
- Case 9's tail coordinate from `i` at constant stride: +28.
- A declared local added at the end of the list costs eight immediate rows in
  +0x3400..+0x3700 even when it is register-allocated (v401, v805); declared
  after `opponent` it shifts the ladder to 416.

## Landscape at 157

The complete single-force landscape at 157 (1,933 probes over 143 coloured
webs, 539 at target width, no deferrals) stamps clean against the committed
body. Thirteen forces beat 157 at delta zero; the packer's disjoint set is
the same quartet as at 160 and predicts 123:

    p1:w75=c16    142  (+15)    entry-array cursor to s2
    p1:w379=c20   148  (+9)     count/stride to s6 (rival of w225=c20)
    p1:w225=c14   150  (+7)     decrement cursor to s0
    p1:w435=c15   151  (+6)     case-13 cursor to s1 -- the pair above
    p1:w27=c17    154  (+3)     copy input to s3

The quartet measures **123**, equal to its prediction. `w435=c15` reads as
colliding with `w379` at the 0x80 window and is additive in fact: the
quartet plus `w435=c15` measures **116**, and `w435=c15` with `w379=c20`
alone 141 (148 − 7). `w959=c17` is antagonistic (+39 on the quartet). So the
colour-only diagnostic on this body is 116 with five forces; every one of
the five now has a named source variable and a measured reason it is not
taken naturally.

Split forces through the new receipt (cycle 16 and 18): `w22=s` +4 bytes,
`w372=s` +36, `w379=s` +20, `w75=s` +8, `w259=s` +12, `w935=s` (the
constant 5) 157 at delta zero with 16 words moved; `w435=s`, `w384=s` and
`w889=s` are recorded below. Splitting a swap partner buys nothing at width.

## Cycle ledger

    0   bank 160 set (copied wv-o's fingerprint-clean artefacts, wv-n's lattice)   —
    1   p2:w75=c16, p2:w379=c20                          not applied; no p2 records
    2   identification, 30 webs                          table above
    3   identification, 48 webs                          table above
    4   fresh inner index ×2, 6 carriers, count/stride    168, 416, 160-3143, 251, 259
    5   w372=c14+w22=c15 (186), quartet+swap (151), joint syntax (exit 55)
    6   w889 identification                              coalesced with opponent
    7   generated inner/first-loop coordinates ×7        +20..+88 bytes
    8   L160: case-13 indexed, case-9 tail, case-6, row  +48, +52, +28, **157**, 168
    9   v804 lattice: c13 columnX, keep base, rowBase, portraitX inner   2322, 159, 3083, 197
    10  probe lattice on portraitX ×7                    246-252, 159
    11  case-13 carrier sweep ×5                         2322 (−4) ×3, 220, 259
    12  increment order in case 13 ×7                    163-170, +4
    13  landscape at 157 (1,933 probes)                  13 winners, packing 123
    14  packing lattice ×6                               123 = predicted; +w435=c15 116
    15  split forces ×9 through the new receipt          +4..+36 bytes; w935=s 157
    16  non-invariant probes on portraitX in case 6 ×4   +8, +8, 161, +16
    17  split forces on w435, w384, w889 (fragment receipt)   +8, +28, no such web

Eighteen cycles by the coordinator's running count, seventeen by this
ledger plus the uncounted bank; both are under the twenty authorised.

What the next cycle would have been: a `--hold` option for
`web_footprint.py` that keeps the packed five forces applied while every
other coloured web is probed — the second-order landscape. At 116 the five
nominated colours all have a named variable and a measured save behind them,
and the question that remains is which colours the *other* 138 webs would
take once those five are right; that is a 1,900-compile sweep, one cycle,
and it has never been run on any body of this function.

## Reproduction

Cells were compiled with the instrumented `cc` swapped into the configured
full-TU command (`force_lattice.compile_command`), objects scored directly,
and the tree source restored after every cell; the instrumented object is
byte-identical to the stock one on the unforced baseline. The private scratch
holds every variant source, object, allocator log and residual. Gates ran
through `tools/gates.sh`; ROM verification proves the assembly fallback, not
this C. `config/nonmatching-ranking.us.json` and `docs/nm-ranking.md` were
refreshed locally so the 157 landscape stamps against the finalized text and
were then reverted; the coordinator's regenerate reproduces the same digest.
