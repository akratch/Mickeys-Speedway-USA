<!-- plateau-handoff:func_overlay_101_F000AB4C_18E636C:start -->
### `func_overlay_101_F000AB4C_18E636C` plateau handoff

- source: `src/overlays/o101/overlay101TailAB4C.c`
- score: 579/638 words
- frame: 0x48
- relocations: 108
- first mismatch: +0x10
- summary: 579 masked from 636 and the size overrun +108 to -8, on the o101 builder carrier shape; the surplus is now exactly one instruction per text row at a 0xB0 stride, which is a ring-draw count to be read from the instrumented ugen, not a colour.

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

This procedure issues calls, so by the call test it is p1-only and L100's save ratio is the axis for colours the web is actually offered. CORRECTED 2026-09-12: the call test retires L106 and nothing else. Statement order still reaches as1's scheduling and ugen's emission order, neither of which the census touches, and it was decisive on every one of seven functions closed in one lane. Do not read this shard as saying statement order is the wrong axis.

<!-- plateau-handoff:func_overlay_101_F000AB4C_18E636C:end -->
