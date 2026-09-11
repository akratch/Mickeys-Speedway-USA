<!-- plateau-handoff:func_overlay_101_F000B544_18E6D64:start -->
### `func_overlay_101_F000B544_18E6D64` plateau handoff

- source: `src/overlays/o101/overlay101TailB544.c`
- score: 158/316 words
- frame: 0x38
- relocations: 38
- first mismatch: +0x9C
- summary: 158 masked from 189 at exact size and frame; the pre-call direct counter read, per-row text folds and a seven-block statement-order climb. Multiset was never exact here.

Measured 2026-09-11, lane `lane/p2-quad`, alongside the four-function builder
quadruplet. This TU is the same family over different data but had never had
the quadruplet's levers applied.

Before: 189 masked, aligned 146 byte-exact, 138 register naming, 3 immediate
only, 37 really different, size delta 0, frame 0x38 exact.
After: 158 masked, aligned 177 byte-exact, 80 register naming, 1 immediate
only, 71 really different, size delta 0, frame 0x38 exact, displacement tax 6.

Read the bucket movement carefully: the positional masked count falls by 31 but
`really different` rises from 37 to 71. That is the aligner re-bucketing a
shifted schedule, not new structure. Instruction count and frame are unchanged
and exact on both sides, and the register-erased instruction multiset is 8
words out at 189 and 10 at 158, so it was never exact on this function. Any
size or multiset claim about this symbol has to be re-derived here; do not
inherit the quadruplet's "multiset exact" line, which is true of them and not
of this one.

What paid, each measured on its own:

- The pre-call read of the node counter spelled as the global inside the
  pointer expression rather than through an index local, 189 to 184. This is
  the L115/L101 lever the four siblings carry and this TU did not. The
  post-call read must keep the index local: making it direct as well reads 185,
  and making only the post-call read direct reads 201.
- The same lever on the text rows is per row, not global. All sixteen
  combinations were measured. Rows 3 and 4 pay, 184 to 174 together; row 1
  costs 119 and row 2 costs 2, so applying it to all four reads 294.
- Statement order over seven blocks, the two root headers, the chain group and
  the four text rows, 174 to 158, by pairwise-swap hill climbing with four
  random restarts. The climb also joins the two-line opacity statement onto one
  physical line in the two unfolded rows, which is an L59 fold and part of the
  win.

Measured regressive: splitting the single shared `index` local into separate
node and text counters reads 285 with a size delta of -16, consistent with the
siblings' finding that merging or splitting the counter locals is not free.

Not re-measured here, inherited as an argument rather than a number: the
siblings' dependence-graph closure on the node group. as1 orders every memory
reference after a preceding store whose base register differs, so the ROM's
both-old-link-loads-first block needs a carried value, and every carrier
spelling measured on the siblings is given a globalcolor colour where the ROM
has ring registers. The same block shape is present here, so the same argument
should hold, but a lane that wants to act on it should re-derive it on this TU
rather than take it on trust.

<!-- plateau-handoff:func_overlay_101_F000B544_18E6D64:end -->
