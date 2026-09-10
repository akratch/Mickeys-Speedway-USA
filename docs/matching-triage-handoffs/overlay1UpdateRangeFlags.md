<!-- plateau-handoff:overlay1UpdateRangeFlags:start -->
### `overlay1UpdateRangeFlags` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 118/120 words
- frame: 0x70
- relocations: 4
- first mismatch: +0x34
- summary: residual is one ugen ring-queue slot fixed by the angle block; no source form reorders that queue without breaking the sign-extension rows

#### tu2-o1tail: the residual is one FP pool web, same law as overlay1AppendPathPoint

Re-measured at the assigned base: 31 masked words, 120 of 120 instructions,
0x70 frame exact, prefix exact to row 13, integer temp ring identical 8 of 8.
The fp-pool lane diverges at slot 2: the target computes the second squared
term into the FP temp ring (f4) while the candidate spends an fp-pool colour
(f18) on it, so the candidate carries one FP pool web the target does not.
That is the same named-local-versus-CSE-temp law that took
overlay1AppendPathPoint from 21 words to 6, applied to the squared-distance
comparison rather than to an integer carrier. The lower block at +0xD4 shows
the mirror image: the target holds the other-kind byte in a pool colour (v1)
where the candidate uses a temp (t1).

#### 2026-09-09, lane fin-near: the u16 truncation temp is the second ring draw

In the ugen listing case 1's `u16 masked` costs two ring temps (the mask and
its 16-bit truncation), drawn as t4 then t3, and the clear draws t5; the
final code folds the pair into one `andi t3` and leaves `and t5`. Removing
the truncation (`if (flags & 8)`, or `u32`/`s32 masked`) draws t4 for the
test and t3 for the clear -- the inversion the earlier record describes --
at 4 words. The target's t3-then-t4 needs the free list ordered t3 above t4
at the arm's entry, which the angle block's frees decide; that sweep is
recorded above as exhausted. Baseline 2 retained.

#### 2026-09-09, lane win-b: the ring is a FIFO queue and the queue is the whole residual

Re-measured with a direct `tools/ido/cc` full-TU compile (byte-identical in
`.text` to the asm-processor NON_MATCHING object at this TU's real flags --
`overlay_001_tail.c` **does** carry `-Wab,-r4300_mul`; omitting it silently
costs one instruction and 80 words). 118 of 120, frame 0x70 exact, and the two
words are +0x190 `and $t?, $v0, $s7` and +0x198 `sh $t?, 0x1A8($s1)`: the
target draws `$t4`, we draw `$t5`. Every other row, both switch arms' tests,
and the whole angle block are exact.

**The mechanism, read straight out of `cc -S`.** ugen's integer temporaries are
a FIFO queue over `$8..$15,$24,$25`, ordered by *free* time, and both switch
arms draw from that one queue *in sequence*: `case 0` takes the first two, then
`case 1` continues from the third. The queue reaching the switch is set by the
angle block, whose ugen form is fixed:

    sll  $9, $3, 8     sll $10, $9, 16    sra $11, $10, 16
    sll  $12, $2, 16   sra $13, $12, 16   addu $4, $11, $13
    sll  $14, $4, 16   ... sra $15, $4, 16

`$9` frees at the second `sll`, `$10` at the `sra`, `$12` at the second `sra`,
and `$11`/`$13` together at the `addu`. So the queue is `$9,$10,$12,$11,$13` --
`case 0` gets `$9,$10` (exact, both arms' `andi`/`ori` match) and `case 1` gets
`$12,$11,$13`. That predicts, and measurement confirms:

- a two-temp `case 1` (`if (flags & 8)`) draws `$12` then `$11`, i.e. the test
  and store inverted: **4 words**;
- a three-temp `case 1` (the `u16 masked` carrier, whose truncation `and
  $x,$y,65535` as1 folds into the `andi`) draws `$12` (mask, folded away),
  `$11` (visible `andi`, correct) and `$13` (store, one too high): **2 words**.

The target needs the queue `$9,$10,$11,$12`, i.e. `$11` freed before `$12`.
`$11` is only freed by the `addu`, and `$12` is only freed by the second
operand's sign-extension `sra`, which must precede the `addu`. Making the
second operand cost no temp does free `$11` first -- and then the outer
truncation draws `$12,$13` instead of the target's `$14,$15`, moving the same
two words to +0xE0/+0xE4. The target's `sll $t6`/`sra $t7` pin five ring draws
ahead of them, the `sll $t3` at +0xD8 pins the left chain's terminal at `$11`,
and the `addu $a0` pins the sum on the pool colour; those three constraints
plus the free order are jointly unsatisfiable. **This is a structural
conflict, not a search gap.**

Exhausted here, all against the full-TU object: 960 points crossing 24 angle-sum
spellings (six `angleHigh` shift forms x plain/outer-cast/cast-read/`+=`) with
four physical line groupings of the three angle statements, five `case 1`
carrier forms and two `angleHigh` types -- floor exactly 2, reached at 576 of
them; plus 20 further `case 1` shapes (`!=0`, `==8`, `!!`, `>0`, negated with
`break`, negated with an empty `then`, compound assignment, field re-read,
nested load, a pre-computed `cleared` carrier in five types, and `u8`/`u16`/
`u32`/`s32`/`s16` masks). A pre-computed store carrier does not survive: uopt
sinks the partially dead expression back into the `if`, so the store is always
emitted after the test whatever the source order.

Next lever is a ugen free-list trace (`DKWB_UGEN_TRACE`), not another source
form. Do not re-search `case 1`, the angle spellings, or line grouping.

#### 2026-09-10, lane p1-perm: a third 2-word corner, and the count argument closed

A third arrangement exists and is strictly more informative than either
recorded one. With the two-temp `case 1` (drop the `masked` carrier) and

    angle = ((s16)((u32)angleHigh << 8) & 0xFFFF) + (u16)angle;

every word is exact except +0xD8/+0xDC: both switch arms are right, the `addu`
writes the pool colour `a0`, the sign extension is on t6/t7, and the only
residual is that the folded left chain survives on t4 where the target has t3.

The object pins four things at once -- the folded left chain must survive on
$11, the `addu` must write the pool, the sign-extension pair must be $14/$15,
and the free list reaching the switch must be ascending. as1 folds a chain onto
its LAST destination, so a survivor of $11 forces a three-temp left chain;
$14/$15 forces exactly five temps drawn before the truncation; and an `addu`
that writes the pool forces the sum to be the statement's top-level operation,
so no temp can be drawn after it. That leaves the second operand owing two
temps, and any two-instruction conversion frees its first at the second's
definition -- before the `addu` frees $11 -- which is the original inversion.

The three reachable corners are therefore exactly 3+2 (inversion,
+0x190/+0x198), 4+1 (survivor t4, +0xD8/+0xDC) and 3+1+outer (`addu` on a ring
temp, +0xDC/+0xE0). All three are two words and no fourth corner exists in C.

Newly measured and flat, ~200 candidates/second against the full-TU object:
360 cells of case-1 form x carrier type x nine left-operand spellings x five
right-operand conversions x two assignment casts; 260 cells of thirteen outer
operations x five right conversions x two types x two assignments; 24 double-
and triple-conversion spellings crossed with an `s32` carrier and an
`(s16)`-cast comparison. Every cell is 2, 4, 18, 19, 21 or worse.

The retained body is the original 3+2 corner: it is the most plausible C of the
three and no corner is numerically better.

Next lever: not C. Either uopt/ugen instrumentation that shows why the target's
free list is ascending with a three-temp chain, or a construct that draws a ring
temp after a pool-writing `addu`, which this grammar does not produce.


#### c2-o001: the ring-queue conflict survives the inert-probe family

The win-b record closes this at two words on a jointly unsatisfiable set of
free-order constraints, and calls it a structural conflict rather than a
search gap. That ruling was tested from a direction the recorded sweeps did
not cover and it holds.

242 zero-footprint reads were measured under ADR 0017, spanning eleven
placement slots from the top of the loop body through both switch arms, and 22
expressions including the two phantom-pop forms the field guide names for
buying a ring pop. None of them reaches below two words. Six placements are
byte-flat -- an already-pooled pointer read costs nothing anywhere it is put --
and every other combination regresses, most of them to seven or more.

So the ring queue reaching the switch cannot be re-ordered by adding a draw
either, which is the last cheap thing left to try on it. Two words stand.
<!-- plateau-handoff:overlay1UpdateRangeFlags:end -->
