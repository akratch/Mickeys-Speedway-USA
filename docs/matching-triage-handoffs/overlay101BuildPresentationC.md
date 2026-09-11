<!-- plateau-handoff:overlay101BuildPresentationC:start -->
### `overlay101BuildPresentationC` plateau handoff

- source: `src/overlays/o101/overlay101BuildPresentationC.c`
- score: 143 differing words
- frame: 0x20
- relocations: 52
- first mismatch: +0x10
- summary: 143 masked words from 145 at a size delta of +4, frame 0x20 and its ladder exact. The byte-length local is u8; the 106 naming rows are measured NOT to be a ring cycle.

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

The post-increment order-counter spelling was this relative's entire gap to A
and B. With it retained, the fold alone stops at 159.

Lane `lane/p2-pres`, 2026-09-11, re-measured this function alongside its
relatives. The source is unchanged; everything here is a negative, and the
full working is in
`docs/matching-triage-handoffs/overlay101BuildPresentationB.md`.

- The forced-colour census reproduces: 145 to 136 to 130 to 127 on `p1:w186=c11`, `p1:w31=c2`, `p1:w78=c8`, measured on the B relative and identical here at every step. Every force was checked for a
  `p1color` record carrying `forced` before its object was scored.
- The intra-group line-break axis, which the previous lane did not measure (it
  measured whole-group fold subsets), is flat. All 81 break positions
  across the folded groups: nothing raises aligned byte-exact above 77.
  One break, the first group at its fourth statement, reads 144 masked but 75 byte-exact and 34 structural, so it trades two agreeing words for one positional word and is not adopted.
- The tail statement order, widened from the previous lane's eight statements
  to the whole tail under its real dependences: 19,630 constrained orders
  searched by single-move greedy plus random restarts, never beating
  145. The displacement-pair sweep that refutes the interfering-web hypothesis was run on the B relative, which is identical to this one in every bucket and at every insertion offset.

Lane `lane/p4-pres`, 2026-09-11. One adoption and four measured negatives, two
of them against axes the dispatch named as this cluster's live edges.

Adopted: the byte-length local is `u8`, not `s8`, the store takes an `(s8)`
cast and the redundant mask in the opacity expression goes. 145 to 143 masked
on each of the three, aligned byte-exact 77 to 79, register naming 106 to 104,
immediate 1, structural 32, size delta still +4, and all three remain identical
in every bucket. A 540-cell type lattice was measured -- the callee's declared
return type across six spellings, the local's across six, the store cast across
three and the opacity mask across five -- and every cell that reaches 143 has
the local `u8` and the store cast `(s8)`, with the return type indifferent. The
change is semantically exact: the value is only ever truncated to eight bits, so
`& 0xFF` becomes redundant once the local says so, and the `(s8)` cast is what
draws the extra ugen temp. The same axis is worth 67 and 81 words on the
1520-byte relatives in this family, where nobody had swept it either; two words
is all it is worth here.

The 106 naming rows are NOT a closed cycle, which answers the question the
dispatch asked and closes that direction. `tools/register_census.py` prints a
four-cycle over t1, t0, t4 and t3, but that is its dominant mapping and the
dominant mapping discards fan-out. Every heavy register fans out: ours `t7`
goes to their `t6` eleven times, `t5` eight and `t9` six; `t4` to `t3` eleven
and `t8` eight; `t8` to four different registers; `t1` to three. Resolved by
offset at the adopted plateau, 80 of 133 pair-instances -- 60 percent -- are
consistent with one global permutation; the best single prefix/suffix split, at
+0x1EC, reaches 75 percent; and six windows are needed before every window is
internally a permutation, at boundaries +0x11C, +0x194, +0x1D4, +0x250 and
+0x2A0. The picture is the same before the type adoption, 78 of 135 and the
same six boundaries, so it is a property of the function rather than of one
plateau. So these rows are
per-region colour, not one ring-phase fact, L127 has nothing here to put back in
phase, and what remains is a colouring question per web. For contrast, the case
that instrument was built for had 195 of 279 words in a single four-cycle, and
the 1520-byte relatives two functions away resolve to one clean one-position
shift at 83 percent -- so the instrument does separate the two, and this
function is the scattered kind.

The L126 copy carrier is flat here. 245 carriers were measured over all thirteen
locals: each value copied into a fresh local of its own type, the copy placed at
every statement boundary after the value's definition, and read by each single
downstream use and by all of them at once. Nothing beats 145, and every one of
the 245 leaves the size delta at +4 -- so the copy really is byte-free exactly
as L126 says, and byte-free is all it is on this function. One corner is open
and worth naming: four pointer locals could not be reached because their
definition and all their uses sit on the same folded physical line, so no
"downstream" boundary exists for them. They are all pre-call and the surplus is
post-call.

The nocs lever reaches 138 at size delta 0 and is SEMANTICALLY INVALID. This is
recorded because it looks like a breakthrough and the next lane will find it
again. A second definition of `previousType` placed after the root's childType
store, or of `previous` after the root's child store, reads 138 at delta 0
against 145 at +4, and a greedy second round is flat there. But by that point
the root fields hold this node's own new values, so the node would link to
itself instead of to the previous chain entry: it is trap 8 in the standing
brief, a sweep offering a candidate that scores better and means something
different. The same trap swallowed a second lattice: respelling the chain-head
reads produced 136 at delta 0 across 304 cells, and every cell at 136 or 138
turned out to read a root field after that field had been overwritten.

What those invalid cells do establish is a fact about the target, and it is the
useful part. Deleting the previous-link LOAD closes the size delta exactly. So
the trio's surplus instruction is not only the second materialisation of the
colour constant that the earlier shard named -- it is reachable from the load
side as well, and a source form that legitimately removes that load would close
the +4. The chain head is a known value at the node-24 push, since the root's
childType and child were set to 1 and the node-20 pointer earlier in this same
function and nothing between writes either -- but spelling them as those
constants is strongly regressive, and that is measured rather than assumed:
193 at a size delta of 8 with both spelled as literals, 189 at +4 with only the
previous pointer spelled that way, and 166 at +8 with the pointer carried
through a local of its own.

The tail statement order is now exhausted exhaustively rather than by search.
The previous lane covered 19,630 orders by constrained greedy plus random
restarts and never beat 145; the type adoption moved the plateau, and a plateau
that moves invalidates every order result taken above it. So this re-enumerated
the tail COMPLETELY: all 4,480 permutations of the eight tail statements that
respect the four real dependences -- each chain-head read happens before the
root field it reads is overwritten, and before it is stored into the node. 143
is the floor, the adopted order sits on it, and the worst valid order reads 149.
Those four dependences are exactly what every 136 and 138 cell above violates,
which is why this sweep cannot emit one.

Next lever, with the decision variable named. The +4 is one surplus instruction
and there are now TWO independent routes to it rather than the one the earlier
shard named. The first is that shard's: a second materialisation of the 0xC0
colour constant, web 198 at `save=1.0 nocs=1` whose `p1dec` carries
`forbidden0=0x7fc30000`, forbidding c1 through c9 -- so a force on it is
silently declined and, by L101's third kind, proves nothing. The second is the
previous-link LOAD, which every invalid variant above deletes to reach size
delta 0, and which no lattice has yet attacked from a semantically valid form.
The load side is the newer and the cheaper-looking of the two.

Order the work that way rather than by bucket size. The 104 naming rows are a
separate question and the census above says they are per-web colour, so they
will not fall to one edit; and the previous lane's forced-colour ceiling of 127
was measured on a source shape that no longer exists, so it should be
re-derived, not inherited, once the +4 is closed.

<!-- plateau-handoff:overlay101BuildPresentationC:end -->
