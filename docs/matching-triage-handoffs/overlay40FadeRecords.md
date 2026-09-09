<!-- plateau-handoff:overlay40FadeRecords:start -->
### `overlay40FadeRecords` plateau handoff

- source: `src/overlays/o040/overlay40FadeRecords.c`
- score: 98/101 words
- frame: 0x8
- relocations: 10
- first mismatch: +0xC
- summary: Permuter register-allocation sweep was flat in the scratch basin; next lever is a fidelity-clean p2 forced-color/permuter search for the copy-web partition.

#### 2026-09-09, lane fin-near: the v0/v1 pair is the loaded web and its copy

The three words are one pair: the candidate colours the loaded
`gOverlay40BlendCurrent` web v0 and the `current` copy v1, the target the
reverse. Loading into `current` and copying to `output`, both chained
assignments in both directions, the twice-spelled global (98 words, the
address is recomputed), swapping the two declarations, and moving the timer
load ahead of the pair (18-38) are all flat or worse; `current = output =`
and the declaration swap equal the 3-word baseline. Retained.
<!-- plateau-handoff:overlay40FadeRecords:end -->
