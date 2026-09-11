<!-- plateau-handoff:overlay15DrawRain:start -->
### `overlay15DrawRain` plateau handoff

- source: `src/overlays/o015/overlay_015.c`
- score: 41/54 words
- frame: 0x40
- relocations: 17
- first mismatch: +0x74
- summary: The 12 structural words are one fact, the three rain offsets. Replacing the padded `gOverlay15RainOffsets` struct with three separate `extern f32` scalars takes the aligner's structural bucket from 12 to 3 and byte-exact from 43 to 48, and brings the `ori at, zero, 0x8000` placement and the pre-call `swc1 $f8` scheduling into agreement at the same time. The single remaining defect is that the target shares one `lui $at` between the x and y loads, tagging it with y's address, while IDO as1 in this tree emits one `lui $at` per absolute reference, so the candidate is one instruction and four bytes long. Twelve spellings were measured and all twelve emit three `lui $at`: three externs, a two-element extern array plus a scalar, a two-float extern struct plus a scalar, a three-element array, file-static definitions of each of those, pointer-dereference forms, and five physical-line groupings of the three arguments. Not adopted, because it trades size delta 0 for +4 and the masked count 13 for 20. The decision variable is as1's %hi sharing between two absolute loads, not a C spelling; the next lane should stop hunting declarations.
<!-- plateau-handoff:overlay15DrawRain:end -->
