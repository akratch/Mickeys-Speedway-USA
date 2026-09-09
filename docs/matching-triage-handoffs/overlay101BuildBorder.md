<!-- plateau-handoff:overlay101BuildBorder:start -->
### `overlay101BuildBorder` plateau handoff

- source: `src/overlays/o101/overlay101BuildBorder.c`
- score: 41/79 words
- frame: 0x88
- relocations: 3
- first mismatch: +0x44
- summary: Four geometry aliases regress to frame 0x90 and 61 differences; retain the 0x88-frame baseline.

- boundary and ABI: overlay 101 text `+0x2DC0..+0x2EFC`, exactly 316 bytes / 79 words, owned by `src/overlays/o101/overlay101BuildBorder.c`; the next owner begins at `+0x2EFC`. The configured signature takes a display-list cursor plus seven signed scalar arguments.
- retained geometry: target and retained candidate are both 79 words with exact `0x88` frames. The retained C has 41/79 positional words and 38 raw and relocation-masked differences; its first ordinary instruction mismatch is `+0x44`.
- relocation surface: target and candidate each have three `R_MIPS_26` records at exact offsets `+0x3C`, `+0x6C`, and `+0x124`. Fresh preflight resolves all three candidate identities: the first two calls are overlay 101 local `+0x2CE4`, and the final call is resident `+0x38F30`. Stable and effective identity proof is therefore 3/3 and complete.
- diagnosis: workbench remains `structure-mismatch`, with exact instruction count/frame, nine structural, three schedule, and 33 register aligned sites. The first source-level cascade follows the first local call and rotates both pool and temporary webs through the geometry stores.
- bounded attempt: four post-call aliases for `x`, `y`, `width`, and `height` tested the apparent four-slot pool deficit. IDO retained 79 instructions but grew the frame to `0x90` and regressed to 61 differing words, so the exact-source baseline was restored. The candidate object and comparison artifact remain under ignored `build/wb/overlay101BuildBorder-attempts/`.
- exhausted families: volatile records, complete-record and target-store ordering, direct typed-array access, explicit right/bottom and six-boundary locals, geometry register qualifiers, the full flag lattice, permutation, and the four-alias lifetime split.
- next action: reopen source matching only with new evidence for a narrower individual geometry-web lifetime; do not repeat multi-alias or boundary-local families.
<!-- plateau-handoff:overlay101BuildBorder:end -->
