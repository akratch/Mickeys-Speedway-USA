<!-- plateau-handoff:overlay101TailA6BC:start -->
### `overlay101TailA6BC` plateau handoff

- source: `src/overlays/o101/overlay101TailA6BC.c`
- score: 267/292 words
- frame: 0x58
- relocations: 44
- first mismatch: +0x0
- summary: 119 flags and ten source forms are nonexact; retained +1-word shape has one extra saved index web and 0x20 excess non-save frame.

#### 2026-09-11, coordinator: four-bucket split, no source attempt

Measured with tools/align_symbol.py on the integration branch and reproduced independently of the lane that first ran it, whose numbers were identical but whose worktree persisted nothing.

  - size delta: +4
  - positional masked: 267
  - byte-exact aligned rows: 83
  - register-naming rows: 111
  - immediate-only rows: 9
  - really-different rows: 104
  - displacement tax: 43

Frame is 0x58 against the target's 0x38, a 32-byte excess, and by L119 that is a block or temps question before it is a colouring one. Frame experiments by the measuring pass regressed, 267 to 286 and 285.

This procedure issues calls, so by the call test it is p1-only and L100's save ratio is the axis for colours the web is actually offered. CORRECTED 2026-09-12: the call test retires L106 and nothing else. Statement order still reaches as1's scheduling and ugen's emission order, neither of which the census touches, and it was decisive on every one of seven functions closed in one lane. Do not read this shard as saying statement order is the wrong axis.

<!-- plateau-handoff:overlay101TailA6BC:end -->
