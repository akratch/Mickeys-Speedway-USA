<!-- plateau-handoff:overlay1UpdateAimedTransient:start -->
### `overlay1UpdateAimedTransient` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 32 differing words
- frame: 0x88
- relocations: 43
- first mismatch: +0x0
- summary: O32 address carrier halves the raw residual; frame, early-load schedule, and five relocation offsets remain after ten forms and 119 flags; no permuter.
- assignment base: `ab2e28755e75281263cff6b4846893469a252f61`
- owned range: Overlay 1 `+0x6D4C..+0x7130`, 996 bytes / 249 instructions, with no size delta
- baseline: 64 raw differing words and 57 positional differences, 39 opcode mismatches, eight alignment gaps, and frame `0x88` versus target `0x80`; the runtime surface had 43 target records versus 45 candidate records, with 30 offset/type positions aligned
- retained improvement: Carrying the shared-world address as an O32 `u32` keeps its two dereferences in the target saved-register lifetime and removes the candidate's duplicated address materialization. Moving `savedState` before `state` then aligns two stack-home constants. Together these changes reduce the raw residual from 64 to 32 words and the positional residual from 57 to 23.
- current comparison: Exact 996-byte extent and 249 instructions, 32 raw differing words, 23 positional differences, 15 opcode mismatches, and eight alignment gaps. The frame remains `0x88` versus target `0x80`; the first mismatch is the frame adjustment at `+0x0`.
- relocation proof: Target and candidate now both carry 43 runtime records. A diagnostic comparison aligns 38/43 offsets and types and 33/43 stable identities, with 34 candidate identities resolved. Strict `function_preflight.py` still fails closed because shared-TU aliases including `overlay1ReadSelection` have ambiguous runtime identities; these diagnostic counts are not an exact relocation claim.
- attempts: Ten coherent forms were compiled: two constant-load lifetime forms, `register`, generic `void **`, volatile-pointee and declaration-initializer world references, an O32 integer address carrier, initialized address/world declarations, and saved-state declaration order. The volatile and constant-lifetime forms regressed structure; three reference spellings were byte-flat. The O32 carrier and declaration order are the only retained gains.
- flags: The 119-case lattice completed with the configured `-O2 -mips2 -32 -Wab,-r4300_mul` row best at 23 positional differences; `-g3` regressed to 25. No global flag change is supported.
- mechanism: `decomp-workbench` reports `structure-mismatch`. FP pool and temporary lanes are exact; the general temporary lane is exact and the general pool first differs at one early world/object load. The remaining source-visible blocks are the early address/load placement, one stack home, and the no-source motion constant load around the first angle call.
- next action: Reopen only with a source-authentic typed declaration for the shared-world symbol, evidence for the original stack-home lifetime, or an authenticated constant/angle-call spelling. Do not repeat these ten forms or run an unbounded generic permutation search.
- DKR/JFG: The exact atlas route has no function-sized row for this old mixed-TU fallback, so a read-only raw skeleton comparison was used. Its nearest DKR and JFG shapes are only 0.0308 and 0.0303; coddog found no exact chunk. JFG's assembly-only `src/overlays/o95/overlay_95.c::flamethrowerConstrain` is the nearest size-compatible row (980 bytes, 0.0303), while `src/overlays/o113/overlay_113.c::missileControl` and `src/overlays/o67/overlay_67.c::mantismissileControl` score 0.0078 and 0.0119. The Mickey candidate may illustrate a generic three-pass intercept estimate, but it is not a donor or public-ledger-grade JFG lead.
<!-- plateau-handoff:overlay1UpdateAimedTransient:end -->
