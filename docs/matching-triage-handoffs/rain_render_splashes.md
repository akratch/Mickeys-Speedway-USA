<!-- plateau-handoff:rain_render_splashes:start -->
### `rain_render_splashes` plateau handoff

- source: `src/main/weather.c`
- score: 340 differing words
- frame: 0xA8
- relocations: 53
- first mismatch: +0x0
- summary: Target 404 words/frame 0xB8, candidate 403/0xA8; ten natural forms and 119 flag modes leave declaration-lifetime allocator geometry.
- ownership: resident text 0x8003BBF8 through 0x8003C248, 0x650 bytes; the next symbol is `rain_lightning`, so there is no owned tail padding.
- ABI and flags: one `s32` update-rate argument, compiled by the configured main-TU `-O2 -mips2 -32` mode; no weather postprocess changes executable instructions.
- best measured shape: target 404 words and frame 0xB8; candidate 403 words and frame 0xA8, with 341 raw and 340 relocation-masked differing words. The first mismatch is the frame adjustment.
- aligned residual: 16 structural, 227 register, and 4 constant sites, with four insertions and five deletions. The target homes the address-taken height result at stack offset 0x84; the candidate uses 0x9C.
- relocation evidence: target and candidate each carry 53 static relocations. The workbench cannot authenticate runtime identity for the target's two `D_7C6A8` tuples, so exact relocation identities remain unproved and this candidate is not promotable.
- retained gains: restored the Mickey `D_7C6A8` display-list relocation, corrected the inner-loop initialization order, recovered the player field type, restored the target display-list expression association, and reused the splash pointer across the spawn and render phases.
- bounded attempts: declaration reordering, explicit height-result and countdown aliases, commutative coordinate forms, and the 119-mode flag lattice were nonexact. The countdown alias regressed to 407 words and was reverted.
- oracle evidence: the nearest skeleton result was JFG `func_8005CC28` at 0.232 and is assembly-only; DKR supplied only broad rain-display idioms, not a shape-compatible donor.
- next lever: recover original local declaration scopes and lifetimes so the height-result home and saved-register pool match before considering bounded permutation.
<!-- plateau-handoff:rain_render_splashes:end -->
