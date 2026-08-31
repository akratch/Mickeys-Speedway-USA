<!-- plateau-handoff:overlay1TransitionState:start -->
### `overlay1TransitionState` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 160 differing words
- frame: 0x50
- relocations: 13
- first mismatch: +0x20
- summary: ten forms exhausted; selector chain restored two words, but phase and pointer forms stayed flat; structural/register residual and 13 unresolved identities
- assignment base: `f8f3ec51a298dd0eddd0574a4313adbb1e39de9b`
- identity: Overlay 1 export-table row 1340 at `+0x3FD8`; the fallback proves the owned `0x3B4`-byte range through the next function at `+0x438C`, with no target padding
- ABI and flags: `void (Transform *, State *, s32)` under the tail TU's configured `-O2 -mips2 -32 -Wab,-r4300_mul`; target and retained frames are both `0x50`
- callers: one authenticated resident `R_MIPS_26` inbound; no additional resident, local, or cross-overlay caller was found
- oracle: the permitted reference scan found no credible donor; the best masked four-word similarity was 0.031 and unrelated in structure
- baseline: 234/237 words, 162 raw and relocation-masked differences, first `+0x20`, exact `0x50` frame
- retained candidate: 236/237 words, 160 raw and relocation-masked differences, first `+0x20`, exact `0x50` frame; `index = state->selectorA = 3` with a literal second selector store retains the target's narrow-index computation
- relocation proof: target and candidate each carry 13 records; 11 offsets/types align, but all 13 candidate identities remain unresolved in the consolidated TU, so preflight is partial and this is not promotable
- attempts: original and delayed phase carriers plus declaration reordering were byte-flat; removing the wide carrier regressed to frame `0x48` and 166 differences; three selector-chain forms measured 165, 161, and 160 differences; the full chain also regressed to frame `0x58`; a phase register hint and compound pointer advance were byte-flat
- residual: one-word size deficit with mixed early dispatch, transform-copy scheduling, and register allocation; no permutation was run after the ten-source-form cap
- next action: resume only with new evidence for the early phase carrier/return shape, the retained point-base update, or complete consolidated-TU relocation aliases; do not repeat these ten forms
<!-- plateau-handoff:overlay1TransitionState:end -->
