<!-- plateau-handoff:func_overlay_046_F0000874_188EC6C:start -->
### `func_overlay_046_F0000874_188EC6C` plateau handoff

- source: `src/overlays/o046/func_overlay_046_F0000874_188EC6C.c`
- score: 87/450 words
- frame: 0xE0
- relocations: 96
- first mismatch: +0x0
- summary: Fresh V0 confirms 1,812-byte candidate, 32-byte frame excess, and broad allocation drift; later-proven FP term rotation is byte-flat in all three scoped forms.

Fresh evidence:

- Assignment base: `d8b737b08b810602e750e195d357f542c79f407c`.
- Owned range: overlay 46 `+0x874..+0xF7C`, 1,800 bytes / 450 words.
- Configured V0: 1,812 bytes / 453 instructions, 87 of 450 positional words exact, 363 relocation-masked and 364 raw differences, first mismatch `+0x0`.
- Frame: target `0xC0`, candidate `0xE0`. Save slots occupy the same 32 bytes; non-save storage is 160 bytes in the target versus 192 in the candidate.
- Relocations: the retained runtime surface has 96 records. The fallback/full-TU diagnostic also reports broad unresolved symbol identity drift, so relocation proof remains fail-closed.
- Donors: the pinned DKR v77/v80 and JFG scans still provide no close source analogue.

New-mechanism result:

- Overlay 26's strict gain and Overlay 22's later exact match establish FP term rotation as a real Mickey IDO lever after the prior plateau closed.
- Rotating the case-1 interpolations from `start + delta * progress` to `delta * progress + start` is byte-flat.
- Applying the same rotation only in case 4 is byte-flat.
- Rotating both cases together is byte-flat. All three forms retain candidate SHA-1 `5086008b44a1`, 453 instructions, the `0xE0` frame, and 363 masked differences.
- Functional C is restored. No flag sweep or generic permutation was repeated.
- Prior constant, particle-base, flag, lifetime, register-order, and step families remain closed. Reopen only with a new source-authentic mechanism that explains the 32-byte non-save-frame excess or three-instruction structural excess; do not repeat term rotation.
<!-- plateau-handoff:func_overlay_046_F0000874_188EC6C:end -->
