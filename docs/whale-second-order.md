# The second-order landscape and a shared carrier, lane wv-q

**Retained: 147 masked / 318 raw differing words, 14,456 bytes, delta zero,
frame 0x138, first mismatch +0x50, 1,253 owned text relocations.** Down from
157. Aligned buckets move from 3488 exact / 118 naming / 1 immediate / 8
structural to **3498 / 108 / 1 / 8**. The single gap pair, candidate +0x12F8
against target +0x1260, is unchanged. All 720 draws survive in their original
order (the register sequence is identical draw for draw) and emissions stay at
5,068. No executable bytes are promoted; the C remains guarded NON_MATCHING.

The one source change: case 2's rank difference is carried by `columnX`
instead of `textY`. Ten naming rows in case 2 heal at zero width. Why that
carrier and not another is the subject of the third section.

## Budget

Twenty measured cycles were authorised; fourteen were spent, listed at the
end. Cycle 0, the `--hold` instrument, is not counted.

## The instrument: `web_footprint.py --hold`

Every landscape ever measured on this function was first-order: each coloured
web probed against an *unforced* baseline. `--hold` takes a list of forces
(repeatable), compiles the baseline with them applied, reads the probe plan
from that baseline's own trace (the held colours change which colours the
other webs are offered, so an unforced trace would plan against the wrong
cost tables), skips the held webs, and adds the held set to every probe cell.
Scores, footprints, winners and the packing are then relative to the held
baseline, and a winner is a force to *add* to the held set. `--trace` is
refused alongside `--hold` for the reason above. Tests are in
`tools/test_web_footprint.py` (`HoldTests`).

## The second-order landscape at 116

Holding the packed five (`w75=c16 w379=c20 w225=c14 w27=c17 w435=c15`), the
baseline measures 116, as wv-p predicted, and 920 same-kind probes over the
other 138 webs find twelve that beat it at delta zero. Three of the webs had
never been nominated by any first-order landscape, because at first order
each of them either moved nothing or changed the function's size:

    p1:w101=c17   97   (+19)   D_o058_5EF8/5EC8 cursor, cases 1 and 2, to s3
    p1:w26=c4    109   (+7)    entry-loop store cursor, off v1 (a1..a3, t0 all 109)
    p1:w37=c4    111   (+5)    entry-loop bound, rival of w26 (identical radius)

`w101` at 157 sat in s2 and every probe of it changed the size by -4 or
more; with `w75` held at s2 it drops to s1, and from there the force to s3 is
size-neutral. The packer's disjoint set is `w101=c17` plus `w26=c4`,
predicted **90**; the seven-force cell measures **90** (first mismatch moves
to +0xA4). The constant-5 web (`w933`, not `w935` as the earlier table has it)
is what `w101=c17` really moves: it falls from s3 into the freed s1, which is
the `s3->s1` substitution the 157 residual shows in eight windows.

**The third-order landscape at 90 finds nothing.** Holding all seven, 906
same-kind probes over the remaining 136 webs produce zero winners at delta
zero. So 90 is the floor for any single additional same-kind force on the
157 body; what remains there is pairs and structure: the cases-12/13 s0/s1
swap (`opponent` against `columnX`, the ratio wv-p priced), the gap pair, the
entry-loop store cursor, and case 2.

On the adopted 147 body the same seven forces measure **85** (their web
numbers are unchanged there; only `columnX` and `textY` re-shape), and the
second-order landscape at that base -- 905 same-kind probes, banked as
`hold7at147` -- again finds no single winner at delta zero.

## What the forces hide: the target's cursors are per-case webs

Reading the seven-force object row by row against the target (privately, by
register sequence) shows something the scores cannot. In case 1 the target
holds the `D_o058_5EF8` cursor in s3 and the `D_o058_5EC8` cursor in s2; in
case 2 it holds `D_o058_5EC8` in **s0** and `D_o058_5EF8` in s3. One web
cannot take two colours, so **in the target these are per-case webs**, while
ours (`w75`, `w101`, nocs 16 each) each span both cases -- L131: the same
subscript spelled the same way in two cases is one IR name. The forces
approximate the target by symmetry and leave a four-cycle in case 1
(`s0 -> s3 -> s2 -> s1 -> s0` over `w75`, `w101`, the `&D_o058_5EC8`
constant `w959` and the constant 5).

Spelling the two cases apart (cycle 7, twelve cells) costs width in every
form but one: `letter0` as case 2's index measures 160 at delta zero, and the
rows it moves are in case 1, not case 2. The split is real and the colours it
buys are not the target's.

## Case 2's rank difference is a shared variable

uopt colours a **symbol** as one web across every case: `textY` (w134) has
members in cases 1, 2, 8, 9 and 10, nocs 29, save 16.55, and is decided
early into s4. The target's case-2 rank difference sits in s1 with the
`D_o058_5D50` format-string address in s4 -- the colours of a web decided
*late*, after the case-2 cursors. So the target's variable there is not its
`textY`. Sixteen carriers were measured (cycle 12):

    columnX        147   (-10)   0x880-6 0x900-1 0xA80-3 0xB80-2; 0x180+2 0x580+1 0x280-1
    countdownX     151          same case-2 rows, +4 in case 6
    letter0 highlighted opponent portraitIndex   156
    columnStep letter1   160     columnCount 163     erase 179
    x rowHeight savedPosition centiseconds   +12 bytes
    savedX -16 bytes; folded into the sprintf argument +4 bytes

`columnX` is `w372`, 954/12 = 79.5 at 157, the third web decided; adding
case 2's def and use makes it 14 blocks and it still takes s1 in every case
where the target has it. The reading is that the original source reuses one
scratch variable for case 2's rank difference and for cases 9/12/13's column
X -- carrier identity across cases, L44 at function scope. The three rows it
costs in case 1 are the cursor four-cycle above shifting by one colour.

## The entry-loop store cursor, and a mechanism with no free spelling

The target keeps the entry loop's `D_o058_5E50` store cursor in **s0** and
case 2's decrement cursor in s0 as well; both loops are call-free, and ours
take a0/v1 -- the lowest offered colour, every caller-save colour costing
zero in the cost table. Ten index carriers for the entry loop are byte-inert
(cycle 6: seven reproduce 157 exactly), so the choice is not interference.

With `i` as the entry loop's index, the entry store and case 2's decrement
loop become **one web** (`D_o058_5E50[i]` in both: bbs 9-10 and 85-88, save
27.3), still byte-identical at 157; forcing that merged web to s0 with the
other five measures **86** at delta zero and heals the entry and decrement
windows completely (cycle 10). So the target's source shares the index name
between those two loops. What it does not explain is why the target's web
prefers s0 at equal cost. Sharing the name with case 2's *draw* loop as well
(`D_o058_5E50[i]` for the rank reads, cycle 11) does make the web span calls
and it then takes a callee-save colour on its own -- the entry and decrement
windows heal without any force -- but the draw loop's shape changes by one
word (+4) and the cell is not adoptable. The mechanism is measured; the
spelling that gets it at width is not found.

## Closed this lane

- The case-13 first-loop pair (dispatch objective 2): the cursor's 31/2 is
  robust to its definition's placement (three placements, 158 each, one
  row at +0x1700); `portraitX`'s definition inside the guard costs 16 bytes;
  merging the second loop's index and counter costs 8-12 bytes either way.
  The ratio stays at 15.5 against 11.67 from every placement of either def.
- A local copy of the switch discriminant across the entry calls: twenty
  cells, every one changes size, including the placement after all three
  calls -- the second switch's reload is aliasing-forced and the target has
  it too.
- Per-case cursor spellings for cases 1 and 2 (cycle 7): width in eleven of
  twelve cells; `letter0` 160.

## Cycle ledger

    0   --hold instrument, tests, identity gate (byte-identical .text)   --
    1   second-order landscape at 116, 920 probes                   12 winners, packing 90
    2   case-13 def placement x7                                    158 x3, -16 x3, -8
    3   case-13 second-loop index/counter merge x3                  +12, +8, 177
    4   packing at 116: +w101=c17 +w26=c4                           90 (= predicted), 97
    5   discriminant copy, 5 carriers x 4 placements                every cell changes size
    6   entry-loop index carrier x13                                7 byte-identical, rest width
    7   per-case subscript spelling, cases 1/2 x12                  160 (letter0), rest width
    8   third-order landscape at 90, 906 probes                     0 winners
    9   w26=c14 cross-kind at the seven                             not applied (s0 forbidden)
    10  i-merged store/decrement cursor to s0 with the five         86 (delta 0)
    11  share the merged name with the draw loop x6                 +4 bytes, entry/decrement heal
    12  case-2 rank-difference carrier x16                          columnX 147 -- adopted
    13  seven forces on the 147 body                                85
    14  second-order landscape at 85 (hold7at147), 905 probes       0 winners

What cycle 15 would have been: the merged `D_o058_5E50[i]` web spans a call
only when it shares the draw loop's name, and that costs a word in the draw
loop. The next spelling to measure is the draw loop reading the rank through
the same name but a different *expression* (the row pointer plus offset, or
`i` from a second index), so the store/decrement web is call-spanning and the
draw loop's cursor keeps its own shape. Read the entry window and +0x780
first; the target's rows are exact there under the force.

## A note on the per-pass log

`docs/matching-triage-handoffs/func_overlay_058_F000138C_18B0574.md` stood at
262,105 bytes before this lane -- 39 bytes under the clean-room `oversize`
limit -- so no pass could be appended to it. It is now the fourth entry in
`CONTENT_EXEMPTIONS` (`tools/cleanroom_detectors.py`) for that rule, with the
same reason class as `docs/resident.md`: a prose ledger that grows with every
pass and carries no extracted data. The word-table rule still applies to it.

## Reproduction

Cells were compiled from variant sources in a private scratch directory with
the configured full-TU command (the input path swapped, everything else from
`force_lattice.compile_command`), scored directly with a private
`nm_ranking` work directory, and the tree source was never written while a
landscape ran. The instrumented object is byte-identical to the stock one on
the unforced baseline. The 157 landscapes (`hold5`, `hold7`) and the 147 one
(`hold7at147`) are banked, with the 147 body's unforced allocator log and
draw census, at `whale-resources/` under Git's common directory
with the allocator log of each held baseline, plus the `i`-merged variant's
trace (`allocator157i-merged.log`) and its forced cell. Gates ran through
`tools/gates.sh`; ROM verification proves the assembly fallback, not this C.
