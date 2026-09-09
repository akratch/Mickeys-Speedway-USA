<!-- plateau-handoff:overlay63UpdateSequence:start -->
### `overlay63UpdateSequence` plateau handoff

- source: `src/overlays/o063/overlay63UpdateSequence.c`
- score: 102 differing words
- frame: 0x20
- relocations: 39
- first mismatch: +0xC
- summary: Census is exactly `move -1`: the target copies the poll result into v1 and reuses v0 for the sequence pointer, and the candidate exchanges the two carriers and materialises no copy. 88 of 107 words already align. Twelve source forms and the whole flag lattice are flat, so the lever has to make the sequence pointer take v0.
<!-- plateau-handoff:overlay63UpdateSequence:end -->
