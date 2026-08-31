<!-- plateau-handoff:overlay34CreateRecord:start -->
### `overlay34CreateRecord` plateau handoff

- source: `src/overlays/o034/overlay34CreateRecord.c`
- score: 95/125 words
- frame: 0x28
- relocations: 12
- first mismatch: +0x0
- summary: A fidelity-gated whole-itable ladder confirms distinct record/current homes, but current-based testing is byte-flat; the eight-byte frame deficit remains.

#### Whole-itable loop-current reproof (2026-08-31)

- assignment base: `d63836a7`; configured V0 reproduces 500 bytes / 125
  words, 95/125 relocation-aware positional words, 32 raw and 30 masked
  differences, first mismatch `+0x0`, frame `0x28` versus target `0x30`, and
  the previously authenticated 12 relocation identities.
- IDO 5.3 `uopt.c` was regenerated from static-recompiler revision
  `9c242adc` at the pinned `b0058f15...` source digest, instrumented with the
  shipped global-color profile and preserved whole-itable patch, and built in
  a lane-local toolchain copy. With tracing enabled, `.text`, `.data`,
  `.rodata`, relocations, and symbols are identical to stock output.
- the baseline ladder contains eight slots, including six automatic homes.
  It confirms that `record` and `current` already remain distinct frontend
  homes; the missing frame quantum is therefore not explained by the source
  having collapsed those declarations.
- the single authorized source form changed the active test from
  `record->active` to the just-assigned `current->active`. It is byte-identical
  in `.text`, retains 125 words and frame `0x28`, and leaves all comparison
  metrics unchanged. The compiler proves the two expressions equivalent
  before the missing allocation decision.
- no second ordering form was attempted because the first form produced no
  strict frame or word gain. Preserve the retained source and reopen only
  when a producer trace identifies the missing automatic home or delayed-bound
  carrier directly; do not repeat flags, loop spellings, or broad permutation.
<!-- plateau-handoff:overlay34CreateRecord:end -->
