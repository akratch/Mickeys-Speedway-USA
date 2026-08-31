<!-- plateau-handoff:func_overlay_060_F0000000_18B9DD8:start -->
### `func_overlay_060_F0000000_18B9DD8` plateau handoff

- source: `src/overlays/o060/overlay60Initialize.c`
- score: 184/205 words
- frame: 0x60
- relocations: 76
- first mismatch: +0x6C
- summary: Fidelity-clean traces expose no attributable source web; the scheduler-selected reorder loses one exact word, so functional baseline is restored.

#### Trace-authenticated remeasurement

- Base `c569709545242358f160b290dcf39e9d6fba14f5`; owner is Overlay 60
  `+0x0..+0x334` with no owned padding, and the next owner begins at `+0x334`.
  ORT index 1424 and the sole resident inbound call at `vram:0x80038D5C`
  authenticate the linked owner.
- Fresh configured C and target are both 820 bytes / 205 words with frame
  `0x60`. The restored baseline is 184/205 relocation-normalized words (46
  literal object-word differences), first mismatch `+0x6C`.
- Target and candidate each emit 76 relocation records. Runtime target
  synthesis identifies all 76 roles and addends. Fail-closed static comparison
  aligns 75/76 offsets and types but resolves 0/76 candidate identities; the
  shifted coordinate `LO16` is the positional miss. This is partial evidence,
  not relocation proof for promotion.
- The pinned UOPT source hash matched the supported toolchain, and the traced
  object passed exact `.text`, `.data`, `.rodata`, relocation, and symbol
  fidelity checks against the stock object. Procedure 0 records 14 allocator
  decisions (13 integer and one FP), but all 14 webs remain run-local and
  unattributed. The producer reports no stack-home evidence, so the allocator
  trace does not authorize a source carrier or lifetime change.
- A fidelity-clean native scheduler trace records 339 events. It identifies a
  line/tie choice in the loop: the current descriptor constants are selected
  before the coordinate loads, whereas the target schedules the coordinate
  address/loads first. Moving the natural coordinate loads ahead of the
  descriptor constants retained 205 words/frame `0x60` and improved normalized
  structural distance from 21 to 18, but regressed direct positional agreement
  from 184 to 183 words. It was rejected and preserved only as an untracked
  build artifact.
- Explicit scalar coordinate carriers retained 205 words but grew the frame to
  `0x68` and regressed to 121/205 words. It was rejected. No flag sweep or
  permutation batch was run because the trace did not produce a strict exact-
  word gain.
- Functional baseline C is restored. Resume only with a calibrated producer
  that can attribute the relevant allocator web or stack home to source, or
  target-side scheduler evidence that distinguishes a source-authentic
  dependency. Do not repeat descriptor ordering, coordinate temporaries,
  loop-base placement, exhausted flags, or generic permutation.

Validation evidence was generated with `tools/function_preflight.py`,
`tools/wb_compare.sh --summary-json`, the fidelity-gated allocator receipt and
stack-home tools, and an `as1 -R` scheduler capture. Build evidence remains
untracked under `build/o060-trace/`.
<!-- plateau-handoff:func_overlay_060_F0000000_18B9DD8:end -->
