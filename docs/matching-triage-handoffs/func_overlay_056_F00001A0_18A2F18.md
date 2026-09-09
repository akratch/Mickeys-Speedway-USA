<!-- plateau-handoff:func_overlay_056_F00001A0_18A2F18:start -->
### `func_overlay_056_F00001A0_18A2F18` plateau handoff

- source: `src/overlays/o056/overlay_056.c`
- score: 530 differing words
- frame: 0x1F8
- relocations: 75
- first mismatch: +0x40
- summary: Authenticated early gates and 21 typed calls remove 47 differing words; the candidate is two words long with exact frame geometry, while the minimap loops and remaining pointer identities are still broad.

- ownership: Overlay 56 text `+0x1A0..+0xAB4`, ROM `0x18A2F18..0x18A382C`, exactly 2,324 bytes. `overlay56UnpackColor` starts at `+0xAB4`; the separate `+0xAF4..+0xB00` alignment tail is not owned.
- ABI: the resident inbound at ROM `0x27F18` passes the display-list pointer address, vertex-cursor address, and the current update-rate word. It is the sole direct resident relocation to export table index 1361 at `+0x1A0`.
- configured baseline: the prior retained Mickey-only form emitted 585 instructions, exact `0x1F8` frame, 577 differing words, and first mismatch `+0x4`. The current base is `229a63af7761df4c9b0a620150c4a9c6fd9b8946`.
- early CFG: runtime identities prove three distinct gates. Fade state advances only while `D_800C3A3C == 0`; rendering returns when `gOverlay56Resource == NULL`; and a nonzero `D_800D3450` rejects game states `0/2/1/3/4` from `func_80028F54`. The retained nested spelling preserves the target branch topology and exact frame.
- call ABI: all 21 calls now have authenticated identities. The sequence is `func_80028F54`, `viGetCurrentSize`, `camStandardOrtho`, `func_80005750`, `levelGetLevel`, `frontGet2PlayerSplit`, `func_8002FB34`, `func_8002F618`, a second `viGetCurrentSize`, sine/cosine, `func_800349A4`, `func_8002A82C`, `matrixTranslate`, two `func_8002A604`, `func_80024978`, `mtxf_mul`, `mtxf_to_mtx`, and two final `func_8002F618` calls. Correcting the FP and shifted argument slots is the main strict gain.
- retained result: configured IDO emits 583 instructions versus 581 with the exact `0x1F8` frame, 530 differing words, 406 opcode mismatches, and first mismatch `+0x40`. Naming the resident base used at `+0x58` improves 533 to 530 differences but adds three target-authenticated address loads, moving the candidate from one word short to two words long.
- relocation surface: the shipped owner has 75 runtime records. All target runtime identities resolve; the extracted target object exposes 31 text records and the retained C function emits 61 static records. Relocation count, offset, and identity are not exact, so no linked-match claim is made.
- bounded attempts: the prior 119-row flag lattice and five identity forms remain exhausted. This reopen compiled explicit-return and nested gate spellings, early-only identities, typed call ABIs, combined downstream identities, and the `+0x58` base identity. Explicit and nested returns were byte-identical; the typed/combined forms supplied strict gains. No generic permutation was run.
- donor evidence: pinned DKR v77 `src/game_ui.c::hud_render_general` supplies the permitted semantic minimap phase order—fade/gates, orthographic setup, viewport offsets, map transform, reverse racer iteration, marker rendering, and state restore. Exact DKR v77/v80 and JFG object/source scans remain negative; the donor is structural evidence, not a promotable body.
- integration blocker: a guarded definition in this consolidated TU routes 5,024 previously credited C bytes through the nonmatching object, so the global atlas correctly fails stale. Session B did not rewrite it. Agent A can preserve exact C accounting with reviewed mixed ranges `+0..+0x1A0` and `+0xAB4..+0xAF4`, retain fallback `+0x1A0..+0xAB4`, and exclude padding `+0xAF4..+0xB00`; do not physically split at unaligned `+0xAB4`.
- next mechanism: reconstruct the two racer-marker loops around the DKR minimap phase skeleton while authenticating the remaining external pointer identities at resident offsets `+0x3194/+0x31A8/+0x1494/+0x1498`. Do not rerun flags, the six covered identity/ABI forms, or generic source permutation.
<!-- plateau-handoff:func_overlay_056_F00001A0_18A2F18:end -->
