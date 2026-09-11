<!-- plateau-handoff:func_overlay_101_F000AB4C_18E636C:start -->
### `func_overlay_101_F000AB4C_18E636C` plateau handoff

- source: `src/overlays/o101/overlay101TailAB4C.c`
- score: 636/638 words
- frame: 0x48
- relocations: 108
- first mismatch: +0x4
- summary: V0 is 665/638 words (+27), frame 0x48 exact, and overruns its 2552-byte owner by 108 bytes; relocation identity proof is unavailable.

#### 2026-09-11, coordinator: four-bucket split, no source attempt

Measured with tools/align_symbol.py on the integration branch and reproduced independently of the lane that first ran it, whose numbers were identical but whose worktree persisted nothing.

  - size delta: +108
  - positional masked: 636
  - byte-exact aligned rows: 150
  - register-naming rows: 271
  - immediate-only rows: 9
  - really-different rows: 278
  - displacement tax: 78

Frame 0x48 exact on both sides while the size is 108 bytes over, so the surplus is body rather than layout. Structural and naming are near parity, which is unusual in this overlay.

This procedure issues calls, so by the call test it is p1-only: definition position, declaration order and statement order decide nothing here, and L100's save ratio is the axis.

<!-- plateau-handoff:func_overlay_101_F000AB4C_18E636C:end -->
