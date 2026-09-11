<!-- plateau-handoff:overlay101BuildPresentationB:start -->
### `overlay101BuildPresentationB` plateau handoff

- source: `src/overlays/o101/overlay101BuildPresentationB.c`
- score: 145 differing words
- frame: 0x20
- relocations: 52
- first mismatch: +0x10
- summary: 145 masked words from 157 at a size delta of +4, frame 0x20 and its ladder exact. L59 group fold; this relative already had the node-24 text store placed.

Measured 2026-09-11, lane `lane/s1-trio`, on the four-function overlay-101
presentation-builder cluster. Every number is `tools/align_symbol.py`, whose
positional figure agrees with `tools/score_symbol.py` by construction, and each
of the four was measured separately at every step.

Before and after, in the aligner's four buckets:

- A: 161 masked, 65 byte-exact, 87 naming, 4 immediate, 57 structural, delta +4.
  After 145 masked, 77 byte-exact, 106 naming, 1 immediate, 32 structural, +4.
- B: 157 masked, 69 byte-exact, 98 naming, 4 immediate, 42 structural, delta +4.
  After 145 masked, 77 byte-exact, 106 naming, 1 immediate, 32 structural, +4.
- C: 163 masked, 61 byte-exact, 87 naming, 5 immediate, 61 structural, delta +4.
  After 145 masked, 77 byte-exact, 106 naming, 1 immediate, 32 structural, +4.
- D: 151 masked, 68 byte-exact, 85 naming, 4 immediate, 53 structural, delta 0.
  After 131 masked, 82 byte-exact, 103 naming, 1 immediate, 27 structural, 0.

A, B and C are now identical in every bucket and at every insertion and
deletion offset. The frame is 0x20 on all four and its three-slot ladder
matches the target at every offset, before and after.

Three levers, all source shape, none costing an instruction.

- L59, the group fold. Every assignment group down to the node-24 header is ONE
  physical line. as1 minimises `(start_time, -aftercycles, -latency, addr,
  lineno, ready-list position)`; with a group's stores on separate lines
  `lineno` is the deciding key and emits them in source order, and the ROM
  emits several of these groups' constant materialisations reversed. Folding
  retires the key. All 1,024 fold subsets were measured on each of the four.
  The minimal paying set is the root header, the node-32 pre-call group and the
  root's second header: on B that reads 145 against 157, and folding any of the
  other four groups on top is flat on the positional count while moving three
  words out of the structural bucket into the naming one. The adopted form
  folds the first seven groups, which is the lowest structural residual
  available at 145.
- The node-24 text store goes BEFORE the counter bump, never last in the tail
  group. Worth four words on A, B and C and six on D. All 1,440 orders of the
  eight tail statements were measured on B: 145 is the floor, 80 orders reach
  it, and the worst order reads 149.
- C only, and the interesting result of the lane. C's order counter was written
  as a post-increment in one statement where A, B and D read it and write back
  `index + 1` in two. That spelling alone was worth ten words: with the fold but
  the post-increment retained C reads 159, and with the two-statement form it
  reads 149 and then 145 with the text-store placement. C's former four-word gap
  to A in the ranking was entirely this, and nothing to do with the family's
  levers.

Refuted here, each with its measurement.

- The quadruplet's call-spanning-web lever, which the campaign brief named as
  the first thing to try, does not apply. Storing handle, previousType and
  previous through the PRE-call node-32 pointer reads 184 at a size delta of
  +12 against 145. This relative's target re-reads the node-32 counter after
  the call and stores through the recomputed pointer, which is what this shape
  already does, so there is no missing callee-saved web here.
- The `volatile` casts A, C and D carried in the tail are exactly inert:
  removing all three reads the same number to the word, before and after the
  other levers. They are gone from the adopted sources.
- Counter partitioning is already separated, as the brief's caution said to
  check. Merging the two node-24 index locals reads 206 at a size delta of +8;
  inlining the node-24 index local reads 148 and the node-32 one 155.
- `register` on any of the six index locals is inert, and so is every
  statement order inside a folded group. That is the call test holding: the
  procedure issues calls, so it is p1-only and definition position, declaration
  order and statement order decide nothing.

Next lever, with the decision variable named.

The A, B and C surplus is exactly one instruction, and it is a second
materialisation of the 0xC0 colour constant for the final call's third
argument. The target materialises it once into the argument register and reads
that register for both colour stores. Read on the instrumented uopt
(`~/Desktop/dev/ido-instrumented`, `.text` confirmed byte-identical to the
stock object with traces off, procedure ordinal 0): the constant is web 198,
`type=2 dtype=8 table=192`, `save=1.000000 nocs=1`, and its `p1dec` carries
`forbidden0=0x7fc30000`, which forbids colours c1 through c9 -- the argument
register among them -- leaving c10 through c13 available, and it takes c10.
`CDX_FORCE=p1:w198=c5` is DECLINED: no `p1color` record with `forced=5`, and
the object returns byte-identical, so by L101's third kind that experiment
proves nothing rather than proving the colour unreachable. Forcing the web that
holds that colour (web 108) out of it, to c10, c11 or c12, does not hand it to
web 198 either, and reads 147, 145 and 150.

Measured inert against the surplus word, all at 145: the position of the
`dimColor` assignment anywhere in its block (seven placements), its spelling
(`s32`, `u8`, `s8`, `u32`, `short`, `char`, the bare literal at every site,
`register`, decimal, octal, an explicit cast), an old-style declaration for the
callee, and L109 discarded-expression probes (OR-with-zero, AND-with-minus-one,
XOR-with-zero, in two different basic blocks, singly and doubled). Measured
regressive: `if (1) { }` and `do { } while (0)` region boundaries around the
assignment, the colour stores or the call read 171, 169 and 176. The
copy-propagation narrow on L109 is the reason the probes are inert: the value
is a constant, so uopt has already folded it and there is no symbol reference
left to count.

How much of the rest is the allocator. A greedy force ceiling on B -- every
p1 candidate web against every colour and the split path, best kept as a
prefix, repeated -- ran three productive rounds and a flat fourth: 145 to 136
to 130 to 127 masked, byte-exact 77 to 97. So about 18 words, 12 percent of the
residual, is allocator-reachable from this source shape, which is in line with
the project's measured one-sixth budget. The remaining 127 is not: 106 of the
145 are register naming and the aligner's structural rows are all schedule
order over the same instructions.

A correction to L114 as the standing brief states it. `globalcolor` DOES assign
`t3` in this procedure: web 182 and web 198 both carry `p1color` records naming
`color=10 reg=t3`, and the `p1cost` lists price `color=11 reg=t4` and
`color=12 reg=t5` as ordinary caller-saved candidates. The colour table here
reads c1 v0, c2 v1, c3 a0, c4 a1, c5 a2, c6 a3, c7 t0, c8 t1, c9 t2, c10 t3,
c11 t4, c12 t5, c13 unnamed, c14 s0, c15 s1, c16 s2 and up. So the rule that
`t3`..`t9` are never colours is too strong: in this procedure only `t7`..`t9`
and the float ring are outside the table, and a naming row spelled `t3`..`t6`
here is a colour decision a force can reach, not ring phase.

This relative already placed the node-24 text store before the counter bump and
carried no `volatile` casts, so the fold is the whole of its gain. Adopting the
other relatives' tail shape on it regresses 145 to 149, which is the same
four-word step measured in the other direction.

Lane `lane/p2-pres`, 2026-09-11, re-measured this cluster and pushed the two
axes the campaign brief named. The source is unchanged: everything below is a
negative, and each one names what it varied.

The forced-colour census reproduces exactly. 145 to 136 to 130 to 127, flat on
the fourth round, with the same prefix: `p1:w186=c11`, `p1:w31=c2`, `p1:w78=c8`,
every force checked for a `p1color` record carrying `forced` before its object
was scored. Of 587 single forces tried, 324 were accepted, 18 improved, 58 were
exactly flat and 248 regressed; the improving forces touch twelve distinct webs.

The 127 is NOT a sweep artefact. The brief's caution is that a sweep never
offers a colour another web has forbidden, so this lane swept the displacement
move directly: for every coloured web, move it off its colour (to the split
path or to any of colours 1 through 22) AND force another web onto the colour
it vacated, both forces verified accepted. 12,320 such pairs at the 127
plateau, 2,474 of them accepted, and not one beat 127. From the unforced 145
base, 15,004 pairs, 3,291 accepted, best 135 -- worse than the single-force
greedy reaches. So the interfering-web hypothesis is refuted on this function:
freeing an incumbent colour does not hand it to any other web profitably, and
127 is the allocator's floor for this source shape rather than a lower bound
on one.

Three new source axes, all flat.

- Line breaks INSIDE a folded group. The previous lane measured all 1,024
  whole-group fold subsets; it did not measure where a group is broken. All 81
  intra-group break positions across the seven folded groups were measured
  here, then greedily accumulated. Nothing raises aligned byte-exact above 77.
  One break, the first group at its fourth statement, reads 144 masked but 75
  byte-exact and 34 structural, so it trades two agreeing words for one
  positional word and is not adopted. A, C and D give the identical picture
  (144 at 75 exact, 144 at 75, and 130 at 80), which is a fourth independent
  confirmation that the trio moves together.
- Tail statement order, widened. The previous lane measured 1,440 orders of
  eight tail statements. This lane searched the whole twenty-statement tail
  under its real dependences: a constrained single-move greedy is flat at the
  first round over 209 neighbours, and 88 random restarts with hill climbing
  spent 19,630 evaluations without ever beating 145. The sibling function
  `func_overlay_101_F000C144_18E7964` moves 24 words on exactly this axis, so
  the axis is live in the family and exhausted here.
- Compiler flags. `tools/flag_sweep.py`, 119 combinations: the configured
  `-O2 -mips2 -32` is first at 145 and no override, including every loop-unroll
  level, `-Wab,-r4300_mul`, `-g3`, `-mips1`, `-mips3` and an all-phase `-O3`,
  improves on it.

One mechanism worth carrying forward, read from this procedure's own records.
A caller-saved colour costs 2 for each call the web spans beyond the first, and
once a web's `nocs` reaches 3 the `available0` mask offers NO caller-saved
colour at all -- every such web takes `s0` upward in descending `save`. That is
why the seven declined webs here all sit at `save` 0.5 with `bestcost` 2.0: the
decision variable is `totalsave` against `nocs`, and on this function no source
form reached above it. `p1:w186=c11`, which pays 9 words, wants the node-20
array base's `totalsave` above 4 at `nocs` 2, and the web has two references.
Holding that base in a local pointer instead reads 170 at a size delta of 8.

<!-- plateau-handoff:overlay101BuildPresentationB:end -->
