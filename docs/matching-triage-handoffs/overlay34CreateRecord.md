<!-- plateau-handoff:overlay34CreateRecord:start -->
### `overlay34CreateRecord` plateau handoff

- source: `src/overlays/o034/overlay34CreateRecord.c`
- score: 95/125 words
- frame: 0x28
- relocations: 12
- first mismatch: +0x0
- summary: Fresh linked baseline is text-differs in=30/out=0; an explicit resource local folds away, shortens to 124 words, and regresses linked output to in=109/out=1222.

#### Epoch 15 linked near-shape pass (2026-09-04)

- assignment base: `58fac8f8`. Fresh configured preflight reproduces the
  retained 125-word body, 30 relocation-masked and 32 raw positional
  differences, first mismatch `+0x0`, and frame `0x28` versus target `0x30`.
  All 12 candidate and target relocation records still agree by offset, type,
  effective identity, and addend.
- a fresh linked promotion trial classifies the retained C as `text-differs`,
  with 30 in-range words and zero outside the owned function. This is not a
  match; exact linked overlay and ROM evidence still comes from the assembly
  fallback.
- workbench diagnosis reports that the target colors one more integer pool
  web and owns one more eight-byte frame quantum. The one authorized
  structural reshape declared the semantically natural resource result local
  and used it for the candidate store, null test, and two dimension loads.
  IDO folds that local instead of allocating a home: the function becomes 124
  words in the same `0x28` frame, with 109 masked and 110 raw positional
  differences.
- the reshaped linked trial is `text-differs` with 109 in-range and 1,222
  outside words; only 5/12 relocation sites remain aligned. The original C is
  restored. This mechanism does not explain the target's extra web or frame
  home and is now exhausted; resume only with direct new evidence for the
  missing producer/home identity.

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
