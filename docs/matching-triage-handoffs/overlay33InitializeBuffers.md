<!-- plateau-handoff:overlay33InitializeBuffers:start -->
### `overlay33InitializeBuffers` plateau handoff

- source: `src/overlays/o033/overlay33InitializeBuffers.c`
- score: 76/81 words
- frame: 0x38
- relocations: 25
- first mismatch: +0x74
- summary: Five words are two as1 delay-slot fill choices; uopt sinks the partially dead `original` copy past the null test, leaving the store as the only fill above the branch. The coupled addu is closed by an operand-weight cast.

#### 2026-09-09, lane fin-misc: the cluster is two delay-slot fills

Measured against the real object with a 64 ms direct-`tools/ido/cc` loop
(byte-identical in `.text` to the asm-processor `NON_MATCHING` object at the
flags from `gmake -n`, once relocations are dropped -- the overlay
post-process consumes 20 of the 25, so a reloc-blind score is the faithful
one here). About 4,000 scored candidates; every one of them 6/81.

Five of the six words are the same seven instructions in two orders:

- target: `sw`, `beqz`, `move`(delay), `andi`, `beqz`, `li -64`(delay), `and`
- candidate: `beqz`, `sw`(delay), `andi`, `beqz`, `move`(delay), `li -64`, `and`

Both branch delay slots are filled differently. The target spends
`original = allocation` on the first and `li at,-64` on the second; the
candidate spends the volatile store on the first and the copy on the second.
The cause is a sink: `original` is dead on the allocation-failure path, so uopt
moves it below the null test and the store is then the only fill candidate
above the branch. Hoisting the copy above the test instead lets uopt delete the
matching `move` at the join, costing a word (80 against 81). The sixth word,
the `addu` at +0xDC (`addu t6,v0,t5` against `addu t6,t5,v0`), never moves
independently of the cluster.

Newly eliminated this pass, each measured:

- A **3,750-cell lock-break cross product** -- `^ 0`, `+ 0`, `* 1` and a
  zero bitwise-or at
  the store, the null test, the `original` copy, the mask test and the mask's
  source. Every cell 6/81. (These were decisive on `func_8003A2C8` the same
  day, so the flatness is evidence, not an untried lever.)
- Every physical line join of adjacent statements in the body, exhaustively.
- Twelve spellings of the second aligned-buffer sum, including re-reading
  `gOverlay33AlignedBuffers[0]`, commuting the operands, `<< 1`,
  `(width * (height * 2))` and `+=`. Word 55 never moves; commutation is
  byte-flat, confirming the 2026-09-04 measurement.
- Declaration census: the unused local at all six positions and as `s32`,
  `void *` and `f32`. Flat.
- `gOverlay33Allocation` **without** `volatile`: also 6/81, so the qualifier is
  not load-bearing for this residual.
- All 24 orders of the four head stores.
- Control-flow shape: early return, inverted `if`/`else`, a `goto` pair, and a
  single pre-tested `while` for the initialise loop. The `while` form is also
  81 words and 6/81 -- a cleaner spelling with no gain.
- Carrier role swaps between `allocation` and `original`: 14 to 52 words.
- Flags: `-Wab,-r4300_mul`, `-Wo,-loopunroll,0`, `-Wo,-loopunroll,4` and
  `-Wo,-nogb` are all 6/81; `-g3` is 19/81.

Next lever: stop uopt sinking `original` past the null test while keeping it a
register copy -- make it live on the failure path, or find the `besttime` input
that makes as1 prefer a copy over a store for the first delay slot.
<!-- plateau-handoff:overlay33InitializeBuffers:end -->
