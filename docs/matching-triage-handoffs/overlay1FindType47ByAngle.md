<!-- plateau-handoff:overlay1FindType47ByAngle:start -->
### `overlay1FindType47ByAngle` plateau handoff

- source: `src/overlays/o001/overlay_001.c`
- score: 18 differing words
- frame: 0x78
- relocations: 6
- first mismatch: +0x8
- summary: Diagnostic r4300_mul reaches 64/74; all six relocation identities are exact, while angle/scale and start allocation webs remain.
- assignment base: `05cbca2025f87f1b4b670eaa73c6cd181ffd0dc7`
- owned range: Overlay 1 `+0x1AC..+0x2D4`, ROM `0x184C58C..0x184C6B4`, exactly 296 bytes / 74 instructions with no target padding
- configured V0: The restored full-TU `-O2 -mips2 -32` body has the exact `0x78` frame and extent, 56/74 positional words, 18 relocation-masked differences, 19 raw differences, ten opcode mismatches, four alignment gaps, and first mismatch `+0x8`.
- flag lattice: All 119 flag identities were attempted and 53 compiled/extracted. `-O2 -mips2 -32 -Wab,-r4300_mul` is the sole strict diagnostic gain at 64/74 positional words, ten masked / eleven raw differences, exact frame and extent, and zero opcode mismatches or alignment gaps. The flag affects every guarded function in this mixed TU, and `tu_flag_impact.py` fails closed on an unrelated ambiguous fallback, so no Make policy change is supported.
- mechanism: Under the diagnostic flag, all integer and FP temporary lanes are exact. Six residual sites exchange the long-lived `angle` and `scale` FP pool colors (`f22`/`f24`), and four exchange the loaded `start` carrier (`a0`/`v1`). Configured code additionally retains the structural schedule divergence removed by the flag.
- relocation proof: Candidate and target each have six records, and all six offsets, types, identities, and addends agree. Runtime metadata authenticates resident `func_8000572C` at `+0x34`, local-data pairs at `+0x44/+0x48` and `+0x60/+0x68`, and local `overlay1WrapOffset` at `+0xAC`. Direct resident naming plus the partial-surface resolver close the formerly ambiguous identities; linked code equality remains fallback-only because 18 instruction words differ.
- ABI and callers: The exact owner has no export and one authenticated local inbound at Overlay 1 `+0x3C54`. That caller passes additional O32 arguments, but declaring them on this callee grows and radically regresses the object; the target compile-time shape is the retained one-`f32` signature.
- attempts: Nine coherent source forms covered FP and index declaration order, explicit angle carriage, `register` priority on scale and angle, guard/index coalescing, loop-local scale and difference lifetimes, and the caller-surface four-argument declaration. Five were byte-flat; the explicit carrier and both narrow scopes regressed; the four-argument form changed extent and regressed broadly. V0 was restored byte-for-byte. The historical bounded permuter was not repeated because no natural source form produced a strict gain.
- donors: The pinned DKR v77, DKR v80, and JFG overlay scans all classify Overlay 1 as `none`. The nearest permitted function skeleton is only 0.126 and supplies no credible body or naming evidence.
- next action: Reopen only with new source-authentic evidence for the paired FP-pool and start-carrier allocation mechanism. Do not repeat this lattice, these nine forms, or the historical generic permutation batch.
<!-- plateau-handoff:overlay1FindType47ByAngle:end -->
