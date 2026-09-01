<!-- plateau-handoff:overlay101BuildBorder:start -->
### `overlay101BuildBorder` plateau handoff

- source: `src/overlays/o101/overlay101BuildBorder.c`
- score: 41/79 words
- frame: 0x88
- relocations: 3
- first mismatch: +0x44
- summary: Exact geometry/frame and 38 differences reproduce; all 3 sites align, but two local R_MIPS_26 identities remain unresolved.

- layered evidence: canonical base `13a926fee7866f851f186edd331174d872a8e1ec`; maintenance authorization `579ce8ffbc73f1289f309e563a83ec22fe160324`; pinned source `c8a15f205a8847ebc6ebf7dc2c0aeb5f97574eb2`; no prior exact-symbol ledger existed.
- boundary and ABI: overlay 101 text `+0x2DC0..+0x2EFC`, exactly 316 bytes / 79 words, owned by `src/overlays/o101/overlay101BuildBorder.c`; the next owner begins at `+0x2EFC`. The configured signature takes a display-list cursor plus seven signed scalar arguments.
- fresh geometry: target and candidate are both 79 words with exact `0x88` frames. Fresh configured C retains 41/79 positional words and 38 raw and relocation-masked differences. The first ordinary instruction mismatch is `+0x44`.
- relocation surface: target and candidate each have three `R_MIPS_26` records at exact offsets `+0x3C`, `+0x6C`, and `+0x124`. The final call resolves exactly to resident `+0x38F30`. The two earlier calls target overlay 101 local `+0x2CE4`, but candidate static identity remains unresolved, so stable/effective identity proof is 1/3 and preflight is partial. Its first unresolved-identity site is `+0x3C`.
- diagnosis: workbench remains `structure-mismatch`, with exact instruction count/frame, nine structural, three schedule, and 33 register aligned sites. The first source-level cascade follows the first local call and rotates both pool and temporary webs through the geometry stores.
- maintenance scope: no body mutation, flags, or permutation was run. The historical volatile-record, boundary-local, flag-lattice, and permuter families remain closed.
- next action: authenticate candidate local `R_MIPS_26` identities using overlay-plus-section owner evidence before further source matching; after identity closure, reopen only with new geometry-store lifetime evidence.
<!-- plateau-handoff:overlay101BuildBorder:end -->
