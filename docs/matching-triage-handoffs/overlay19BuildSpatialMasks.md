<!-- plateau-handoff:overlay19BuildSpatialMasks:start -->
### `overlay19BuildSpatialMasks` plateau handoff

- source: `src/overlays/o019/overlay19BuildSpatialMasks.c`
- score: 164/227 words
- frame: 0x80
- relocations: 0
- first mismatch: +0x58
- summary: Fresh V0 is exact-sized at 164/227 words with exact 0x80 frame and zero relocations; the stale 66-word ranking overstates the current 63-word deficit.

- geometry: Target and configured C are both `0x38C`/908 bytes/227 words with frame `0x80`; the owned Overlay 19 range is `+0xF58..+0x12E4`, ROM `0x18761B0..0x187653C`, followed by separately owned 12-byte assembly padding.
- relocation proof: Target runtime and candidate static surfaces both contain zero relocation records; count, type, offset, and identity surfaces are therefore vacuously exact, and preflight is complete.
- diagnosis: Fresh workbench comparison has 63 raw/normalized differences, two opcode mismatches, 61 register differences, and twenty shift-tolerant alignment gaps.
- history: The body remains the original guarded reconstruction from `752cefadbde9a2ffb9128690055650d33e6edcdc`; this target-named evidence refresh changes no executable source.
- next action: Preserve the fallback pending a genuinely new natural pointer/item-index coloring or independently proved source structure. Prior declaration, lifetime, load-order, alias, loop-variable, flag, and permutation families remain closed.
<!-- plateau-handoff:overlay19BuildSpatialMasks:end -->
