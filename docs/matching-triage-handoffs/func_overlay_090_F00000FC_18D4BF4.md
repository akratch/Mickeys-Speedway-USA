<!-- plateau-handoff:func_overlay_090_F00000FC_18D4BF4:start -->
### `func_overlay_090_F00000FC_18D4BF4` plateau handoff

- source: `src/overlays/o090/overlay_090.c`
- score: 575 differing words
- frame: 0xE0
- relocations: 58
- first mismatch: +0x0
- summary: Reconstruct state-machine CFG and local lifetimes to add nine instructions while reducing non-save frame use by 24 bytes.

Fresh maintenance evidence (base `f6d1bdbe8338886e3cce45818144bf68cbec5720`):

- Ownership is overlay 90 `.text` offset `0xFC..0xB1C`, exactly 2,592 executable bytes; the separate `0xB1C..0xB20` owner is padding and receives no credit.
- The configured guarded candidate uses IDO 5.3, `-O2 -mips2 -32`, and ABI `void (Overlay90Owner *, s32)`. Its exported entry at overlay offset `0xFC` has one resident `R_MIPS_26` inbound caller at VMA `0x8000B110`.
- Fresh V0 is 639 words (2,556 bytes), nine words short of the 648-word target. The candidate frame is `0xE0`; the target frame is `0xC8`. Save slots agree at 76 bytes, while non-save storage is 148 versus 124 bytes.
- Candidate and target each have 58 relocation records. Only 10 offset/type sites and one stable/effective identity align; 27 candidate static identities remain unresolved, so relocation identity is not exact.
- Workbench verdict is `structure-mismatch`: 575 positional/raw differences, 105 alignment gaps, and first mismatch `+0x0`. Donor scan found no credible sibling (best retained donor score `0.052`).
- Historical work already exhausted four 119-variant flag lattices, the `-O2 -mips2 -Wo,-loopunroll,0` form, and five coherent source rounds covering local/cache lifetimes, vector spelling, sound-pitch storage, loop inlining, and animation-value scope. No bounded permutation is justified without a natural strict gain.
- Next lever: reconstruct the state-machine CFG and the remaining local lifetimes together so IDO emits nine additional instructions while eliminating 24 bytes of non-save frame storage; isolated cache ablations previously reduced only eight bytes and worsened structure.
<!-- plateau-handoff:func_overlay_090_F00000FC_18D4BF4:end -->
