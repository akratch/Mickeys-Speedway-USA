<!-- plateau-handoff:func_overlay_008_F0004CF0_1862A48:start -->
### `func_overlay_008_F0004CF0_1862A48` plateau handoff

- source: `src/overlays/o008/overlay_008.c`
- score: 39 differing words
- frame: -0x90
- relocations: 15
- first mismatch: +0x1C4
- summary: Every stack home is now exact, the normal aggregate included; the residual is one extra FP pool web at the normal-vector products, and carrier, operand-order, volatile-placement, line-join and statement-group lattices are all flat.
2026-09-09 lane `lane/tu2-o008` maintenance evidence (base `304d363e`, score unchanged at 43
relocation-masked words / 48 raw, 270 of 270 instructions, exact `0x90` frame):

- The residual is now localized. Every integer lane is identical (pool 49/49, temp
  14/14, shared 11/11) and the only stack-home difference in the whole function is
  the 12-byte surface-normal aggregate, which the target homes four bytes below the
  candidate's placement; the neighbouring 12-byte point aggregate and all four
  compiler temp homes agree exactly. The rest is one FP colour exchange carried by
  two webs of sixteen sites each, plus the known literal-pool offset difference.
- Declaration order is eliminated as the lever for that home. Fifteen orders were
  measured: each 4-byte scalar moved individually between the two aggregates, the
  address-taken pair moved together and separately, the aggregate pointer moved
  between them, and both aggregates reordered against each other. Every order
  scored at or above the 43-word baseline (68, 65, 80 basins) and not one moved the
  aggregate's home or the frame. This is a size-class/region effect, not a position
  in the declaration list.
- Widening the surface-normal aggregate from 12 to 16 bytes does place it at the
  target displacement, which confirms the region reading, but it grows the frame by
  eight bytes and the score to 87. Rejected; the target's aggregate is 12 bytes.
- The declared-but-unused `register f32` local reserves no home: removing it and
  relocating it produce the same object. It is decorative and misleading.
- A 50-minute three-worker decomp-permuter run, roughly twelve times the previously
  recorded 241-second batch, produced no improvement over its base. That run also
  measured the scratch as unfaithful for this translation unit: the runner reports
  the digest-guarded post-compile pass is not replicated, and the scratch base score
  is 406 against a measured 43/48-word residual. Permuter scores do not transfer
  here, and a permuter result on this TU bounds the tool, not the function.
- Resume with allocator evidence rather than source search: the open question is why
  the aggregate region is packed four bytes differently, and no source form reached
  through declaration order, storage class or aggregate width has moved it.
2026-09-09 lane `lane/w3-o008`: 43 -> 39 masked words, and every stack home is now
exact.

- The four-byte surface-normal displacement is closed. The frame is a byte-granular
  declaration census: IDO homes each auto at `frame_top - k` in source declaration
  order (function scope first, then inner blocks in source order), an `s16` takes
  two bytes rather than four, and the whole auto region rounds to eight. The target
  carries four more bytes above the aggregate and four fewer below it, so two edits
  compose -- the normal aggregate takes a fourth float, and the blend tail's loop
  counter is spelled from `updateRate` instead of held in its own block local.
  Neither moves the frame alone; the eight-byte rounding hides a single four-byte
  step, which is why the previous lane read "removing a local does not shed the
  frame" as evidence against the census. It is evidence for it.
- Contrary to the previous handoff, the dead `register f32 motionTarget` is NOT
  decorative: it reserves an auto slot, and removing it moves every home below it
  and costs 22 words. The five `register` qualifiers, the doubled `surfaceHeight`
  assignment and the block-local `factor` are the genuinely inert ones, and are
  now removed.
- Widening the aggregate is byte-identical to inserting a four-byte local above it,
  so the two readings of the source cannot be distinguished from the object. The
  widened-struct form is retained because a surface query filling a plane is the
  more plausible original.
- The remaining 39 words are one extra FP pool web, and the in-source plateau
  comment carries the full exhaustion list: carrier relocation, operand order,
  volatile placement over four members, 37 physical line joins, and three
  statement-group permutations, all flat or worse.
<!-- plateau-handoff:func_overlay_008_F0004CF0_1862A48:end -->
