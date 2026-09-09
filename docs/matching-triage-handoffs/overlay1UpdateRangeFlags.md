<!-- plateau-handoff:overlay1UpdateRangeFlags:start -->
### `overlay1UpdateRangeFlags` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 2 differing words
- frame: 0x70
- relocations: 4
- first mismatch: +0x1B8
- summary: the residual is inherited from the angle block, not case 1: ugen's ring frees $12 before $11 there, so case 1 draws $12,$11. Forms exist where both switch arms are exact and the residual moves to the sum's addu destination; 1080 angle spellings floor at 2

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
<!-- plateau-handoff:overlay1UpdateRangeFlags:end -->
