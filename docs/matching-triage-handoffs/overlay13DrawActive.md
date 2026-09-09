<!-- plateau-handoff:overlay13DrawActive:start -->
### `overlay13DrawActive` plateau handoff

- source: `src/overlays/o013/overlay13DrawActive.c`
- score: 162 differing words
- frame: 0x180
- relocations: 10
- first mismatch: +0x0
- summary: Fresh V0 remains one word short with 162 masked differences; only three target identities align and displaced global proxies stay unresolved.

- geometry: The owned Overlay 13 target remains `+0x874..+0xB0C`, ROM `0x186F38C..0x186F624`, `0x298`/664 bytes/166 words. The configured candidate is 660 bytes/165 words, one word short; `overlay13Call` follows immediately with no padding.
- ABI/flags: `void overlay13DrawActive(s32, s32, s32)` uses Overlay 13 game-code `-O2 -mips2 -32`. Target frame is `0x168`; candidate frame is `0x180`.
- relocation proof: Target has ten runtime records and candidate eight static records. Three offsets/types and stable/effective identities align. Five candidate records for `D_2C`, `o13GetView`, and `D_0` remain unresolved, and the target has one additional data pair.
- diagnosis: Workbench reproduces 163 raw and 162 relocation-masked differing words from `+0x0`, one fewer candidate instruction, 143 opcode mismatches, and relocation-symbol mismatch as the acceptance blocker.
- caller/donor evidence: The resident `R_MIPS_26` caller at VRAM `0x8000EF9C` and export `+0x874` authenticate the ABI. JFG `dropletDrawAll` remains the nearest donor at 0.240 Jaccard, but supplies no source or identity witness.
- identity recheck: The O13 filter specification is target-offset keyed and is not used by this object's configured postprocess, which only renames the exported function symbol. It cannot authenticate the displaced candidate global/call sites.
- authorization: Integrated authorization `e897ba5de7bd079a330e2553b2bc571ecfd12adc` permits only V0 and structured finalization. Prior source, flag, and permutation families remain closed and the guarded C body is unchanged.
- next action: Reopen only if independent data/call ownership evidence resolves the five candidate identities or another genuinely new source mechanism closes the frame and one-word deficit.
<!-- plateau-handoff:overlay13DrawActive:end -->
