<!-- plateau-handoff:overlay1UpdateRangeFlags:start -->
### `overlay1UpdateRangeFlags` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 31 differing words
- frame: 0x70
- relocations: 4
- first mismatch: +0x34
- summary: Exact 120-word frame and opcode order; 31 register-only words and four unresolved overlay-local call aliases remain.

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
<!-- plateau-handoff:overlay1UpdateRangeFlags:end -->
