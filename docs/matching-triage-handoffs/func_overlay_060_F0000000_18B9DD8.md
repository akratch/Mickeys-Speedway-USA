<!-- plateau-handoff:func_overlay_060_F0000000_18B9DD8:start -->
### `func_overlay_060_F0000000_18B9DD8` plateau handoff

- source: `src/overlays/o060/overlay60Initialize.c`
- score: 184/205 words
- frame: 0x60
- relocations: 76
- first mismatch: +0x6C
- summary: scheduler and allocator residue; reorder and coordinate-index probes regressed or stayed flat at 21 normalized differences

#### Trace-authenticated remeasurement

- Base `c569709545242358f160b290dcf39e9d6fba14f5`; owner is Overlay 60
  `+0x0..+0x334` with no owned padding, and the next owner begins at `+0x334`.
  ORT index 1424 and the sole resident inbound call at `vram:0x80038D5C`
  authenticate the linked owner.
- Fresh configured C and target are both 820 bytes / 205 words with frame
  `0x60`. The restored baseline is 184/205 relocation-normalized words (46
  literal object-word differences), first mismatch `+0x6C`.
- Target and candidate each emit 76 relocation records. Runtime target
  synthesis identifies all 76 roles and addends. Fail-closed static comparison
  aligns 75/76 offsets and types but resolves 0/76 candidate identities; the
  shifted coordinate `LO16` is the positional miss. This is partial evidence,
  not relocation proof for promotion.
- The pinned UOPT source hash matched the supported toolchain, and the traced
  object passed exact `.text`, `.data`, `.rodata`, relocation, and symbol
  fidelity checks against the stock object. Procedure 0 records 14 allocator
  decisions (13 integer and one FP), but all 14 webs remain run-local and
  unattributed. The producer reports no stack-home evidence, so the allocator
  trace does not authorize a source carrier or lifetime change.
- A fidelity-clean native scheduler trace records 339 events. It identifies a
  line/tie choice in the loop: the current descriptor constants are selected
  before the coordinate loads, whereas the target schedules the coordinate
  address/loads first. Moving the natural coordinate loads ahead of the
  descriptor constants retained 205 words/frame `0x60` and improved normalized
  structural distance from 21 to 18, but regressed direct positional agreement
  from 184 to 183 words. It was rejected and preserved only as an untracked
  build artifact.
- Explicit scalar coordinate carriers retained 205 words but grew the frame to
  `0x68` and regressed to 121/205 words. It was rejected. No flag sweep or
  permutation batch was run because the trace did not produce a strict exact-
  word gain.
- Functional baseline C is restored. Resume only with a calibrated producer
  that can attribute the relevant allocator web or stack home to source, or
  target-side scheduler evidence that distinguishes a source-authentic
  dependency. Do not repeat descriptor ordering, coordinate temporaries,
  loop-base placement, exhausted flags, or generic permutation.

Validation evidence was generated with `tools/function_preflight.py`,
`tools/wb_compare.sh --summary-json`, the fidelity-gated allocator receipt and
stack-home tools, and an `as1 -R` scheduler capture. Build evidence remains
untracked under `build/o060-trace/`.

#### Re-open under laws L90 / L92 (2026-09-10, lane/c3-reopen2)

- lever set the closure actually searched: descriptor ordering, explicit
  coordinate carriers, loop-base placement, the exhausted flag rows, generic
  permutation, and a native scheduler trace. All of those move where the loop
  body's statements sit; none of them changes the exit test uopt builds.
- **L90's scope limit measured here, with magnitude.** This loop's bound is a
  register-resident constant, which is the case where L90 says the spellings
  are *not* interchangeable. Writing the inequality as a disequality costs
  +4 bytes and 175 masked words -- test replacement drops the counter and
  tests a cursor against a computed limit, and the counter is live inside the
  body, so the rewrite cannot pay for itself. The counted `for` form scores 28.
  The retained guarded do-while is the optimum at 21.
- **L92 measured inert.** The loop's only commutative site is the coordinate
  base plus the scaled index, and it already emits base-first, which is what
  the weight rule requires of an array base against a scaled local. There is no
  second commutative site in the function.
- adjacent law checked because it was cheap: L95's indexed named-array form for
  the coordinate pair costs -4 bytes, so the retained byte-offset carrier is
  also required.
- the deciding variable, named: two decisions remain. First, as1's emission
  order for the two hoisted address materialisations against the counter
  initialisation -- the target completes both address halves before it zeroes
  the counter, the candidate interleaves the counter between them, which is
  two structural rows plus the pool-slot shift that follows them. Second, the
  target spends one more *coloured* web than the candidate on the inner-pointer
  load inside the loop, where the candidate spends a block-local temporary;
  that one substitution carries the temp-ring divergence in the rest of the
  body.
- what was tried against those variables and failed: naming the inner pointer
  as a local, in three shapes and three declaration positions, regresses to
  85 -- so the target's extra web is not a declared local. Swapping the
  counter initialisation with the object-cursor initialisation regresses to 28.
  Hoisting the coordinate base into a named local, and moving the coordinate
  computation to the head of the body, are both byte-flat at 21.
- **verdict: the closure was correct and remains correct under L90 and L92.**
  The artifact column is 25 against a real residual of 21, so raw word counts
  on this function remain meaningless; work the masked count only.
<!-- plateau-handoff:func_overlay_060_F0000000_18B9DD8:end -->
