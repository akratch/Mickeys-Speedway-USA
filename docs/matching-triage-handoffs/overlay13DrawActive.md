<!-- plateau-handoff:overlay13DrawActive:start -->
### `overlay13DrawActive` plateau handoff

- source: `src/overlays/o013/overlay13DrawActive.c`
- score: 162 differing words
- frame: 0x180
- relocations: 10
- first mismatch: +0x0
- summary: Fresh V0 is one word short with 162 masked differences; frame is 0x180 versus 0x168, and only three of ten target relocation identities align.

- geometry: The owned Overlay 13 target is `+0x874..+0xB0C`, ROM `0x186F38C..0x186F624`, `0x298`/664 bytes/166 words. The configured candidate is 660 bytes/165 words, one word short; `overlay13Call` follows immediately with no target padding.
- ABI/flags: `void overlay13DrawActive(s32, s32, s32)` uses Overlay 13 game-code `-O2 -mips2 -32`. Target frame is `0x168`; candidate frame is `0x180`.
- relocation proof: Target has ten records and candidate eight. Three offsets/types and stable/effective identities align: the first data HI16/LO16 pair and final Overlay 13 call. Five candidate records for `D_2C`, `o13GetView`, and `D_0` remain unresolved, and the target carries an additional second data pair.
- diagnosis: Workbench reports 163 raw and 162 relocation-masked differing words from `+0x0`, one fewer candidate instruction, 143 opcode mismatches, and relocation-symbol mismatch as the acceptance blocker.
- caller: One resident `R_MIPS_26` caller at VRAM `0x8000EF9C` and the Overlay 13 export at `+0x874` authenticate the entry and three-integer ABI.
- donors: Fresh masked-skeleton search finds JFG `dropletDrawAll` at 0.240 Jaccard and `fxDrawLevelEffects` at 0.151. These are structural leads only and do not authenticate source or relocation identities.
- history: Current source is pinned by `3d8237dfd540cc6e9ff33613cf96448660648b43`. Prior array-order, ABI/lifetime, flag-lattice, and bounded permutation families remain exhausted. This refresh changes no executable source.
- next action: Authorize only fresh configured V0 plus structured finalization. Reopen source work only for independent relocation ownership evidence or another genuinely new compiler/source mechanism.
<!-- plateau-handoff:overlay13DrawActive:end -->
