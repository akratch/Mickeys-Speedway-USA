<!-- plateau-handoff:overlay84InitializeAndUpdate:start -->
### `overlay84InitializeAndUpdate` plateau handoff

- source: `src/overlays/o084/overlay84InitializeAndUpdate.c`
- score: 172/179 words
- frame: 0x58
- relocations: 5
- first mismatch: +0x14
- summary: Comma grouping closes structural and allocation differences. Seven stack-home offsets and three static identities remain.

#### Fresh comma-mechanism reproof (2026-08-31)

- assignment base: `1427b882`; owned overlay 84 range `+0x48..+0x314`
  remains 716 bytes / 179 words, with the next C owner beginning at `+0x314`;
  the candidate symbol is exactly 716 bytes and its object has four bytes of
  section alignment after the symbol.
- configured V0 reproduced 167/179 positional words, exact `0x58` frame, five
  correctly placed `R_MIPS_26` records, and the prior four-word initialization
  scheduling residual.
- the post-plateau exact comma-expression mechanism was applied only to the
  independent node-angle and height initializations. It removed every opcode,
  structural, register, and FP mismatch, retaining 172/179 words. A broader
  same-order comma chain returned to 167/179 and was rejected. The completed
  119-flag lattice, declaration shuffles, and permutation family were not
  repeated.
- the seven remaining text differences are the `start`/`end` stack-home
  offsets: target `sp+0x40/sp+0x44`, candidate `sp+0x48/sp+0x4C`. Reconstruct
  the authentic source for the intervening two-word home; do not add another
  padding local or repeat declaration-order guesses.
- all five runtime identities are authenticated. Static candidate inference
  resolves the refresh and resource calls, but still fails closed for
  `overlay84GetNodes` -> resident `+0x52DC`, `overlay84Atan2` -> resident
  `+0x2A4C0`, and `overlay84UpdateCurrent` -> overlay 84 `+0x314`.
- one resident caller at `0x8000B0E8` remains ABI-consistent. Pinned DKR and
  JFG donor scans remain negative; the historical JFG structural result was
  only 6.95% and assembly-only.
<!-- plateau-handoff:overlay84InitializeAndUpdate:end -->
