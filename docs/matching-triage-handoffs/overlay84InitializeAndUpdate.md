<!-- plateau-handoff:overlay84InitializeAndUpdate:start -->
### `overlay84InitializeAndUpdate` plateau handoff

- source: `src/overlays/o084/overlay84InitializeAndUpdate.c`
- score: 172/179 words
- frame: 0x58
- relocations: 5
- first mismatch: +0x14
- summary: Whole-itable ladders prove both pointer aliases own homes, but removing either regresses or moves start/end away from retail.

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

#### Allocator stack-home trace reproof (2026-08-31)

- assignment base: `11919d20`; the configured V0 freshly retains 716 bytes / 179
  words, 172/179 relocation-masked words, frame `0x58`, five relocations, and
  first mismatch `+0x14`. All opcode, CFG, integer-register, and FP-register
  lanes remain exact; the seven residuals are still only the two home offsets.
- IDO 5.3 `uopt.c` was regenerated from the repository-recorded static
  recompiler revision whose source hash matches the workbench's pinned profile.
  The instrumented and traces-off objects passed the section, relocation, and
  symbol fidelity gate; the function mapped uniquely to optimizer procedure 0.
- the one authorized detailed capture reported 19 allocator decisions. The
  stack-home classifier found zero producer-emitted virtual or final homes and
  therefore refused to infer ownership from opaque optimizer words. With no
  authenticated carrier or lifetime mechanism, no source probe was attempted.
- next lever: add and calibrate a producer hook that explicitly records virtual
  and final stack-home ownership, repeat the fidelity controls, then reopen only
  if that evidence names an authentic two-word carrier or lifetime overlap.

#### Whole-itable frame-ladder reproof (2026-08-31)

- assignment base: `c90298fa`; configured V0 again reproduced 716 bytes / 179
  words, 172/179 positional words, frame `0x58`, five offset/type-aligned
  relocations, and first mismatch `+0x14`. The seven residuals remain only the
  `start`/`end` home pair: target `sp+0x40/sp+0x44`, candidate
  `sp+0x48/sp+0x4C`.
- IDO 5.3 `uopt.c` was regenerated at the pinned `b0058f15...` source digest,
  instrumented with the shipped globalcolor profile plus the preserved
  `CDX_SYMTAB=1` patch, and compiled in a lane-local toolchain copy. Both the
  traces-off and traces-on objects passed `.text`, `.data`, `.rodata`,
  relocation, and symbol fidelity against stock output.
- the baseline whole-itable ladder contains 15 slots. Declaration-order mapping
  identifies distinct homes for `initialNode` at `sp+0x50`, `start`/`end` at
  `sp+0x4C/sp+0x48`, and `node` at `sp+0x40`; this directly proves that both
  transient pointer aliases own automatic homes even though they later color.
- focused form 1 removed `node` and used `nodes[i]` directly. It retained frame
  `0x58` but grew to 182 instructions and 168 positional differences, so the
  reusable loop alias is required for the current exact instruction shape.
  Focused form 2 removed `initialNode` and used `state->nodes[0]` directly. Its
  fidelity-gated ladder lost exactly one slot and moved `start`/`end` upward to
  `sp+0x50/sp+0x4C`, opposite the required direction; it also shrank to 178
  instructions with 81 positional differences.
- the two evidence-led forms both strictly regressed, so the combined form was
  not attempted. Alias elimination is closed for this source: a future reopen
  needs a new authentic construct that places two homes before `start`/`end`
  while preserving both alias-driven instruction shapes, not another padding
  local, declaration shuffle, flag sweep, or broad permutation.
<!-- plateau-handoff:overlay84InitializeAndUpdate:end -->
