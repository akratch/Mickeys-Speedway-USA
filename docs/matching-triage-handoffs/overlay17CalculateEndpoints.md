<!-- plateau-handoff:overlay17CalculateEndpoints:start -->
### `overlay17CalculateEndpoints` plateau handoff

- source: `src/overlays/o017/overlay17CalculateEndpoints.c`
- score: 65/198 words
- frame: 0x58
- relocations: 3
- first mismatch: +0x24
- summary: Fresh exact-size V0 retains 133 differences; frame is exact, but three call proxies remain unresolved and only one relocation offset aligns.

- geometry: Target and configured candidate are both `0x318`/792 bytes/198 words. The owned Overlay 17 range is `+0x0..+0x318`, ROM `0x18739B8..0x1873CD0`, followed immediately by `overlay17CreateChain` with no padding.
- ABI/flags: `void overlay17CalculateEndpoints(Overlay17ChainHead *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *)` uses the Overlay 17 game-code group `-O2 -mips2 -32`. Target and candidate frames are both `0x58`.
- relocation proof: Target and candidate each have three `R_MIPS_26` records. The target sites are `+0x68`, `+0x108`, and `+0x1D4`; candidate sites are `+0x68`, `+0x10C`, and `+0x1D8`. Only one offset/type aligns and all three candidate proxy identities remain unresolved.
- diagnosis: Workbench reports 65/198 matching words, 133 raw/masked differences from `+0x24`, 88 opcode mismatches, and no instruction-count delta. Its acceptance basis is relocation-symbol mismatch.
- callers: Two Overlay 17 `R_MIPS_26` callers at `+0x51C` and `+0x75C` authenticate the entry and seven-output-pointer ABI.
- donors: Fresh masked-skeleton search is weak: Conker `func_1512DEA4` leads at 0.086 Jaccard, followed by Conker `func_151B118C` at 0.084 and Perfect Dark `model_update_chr_info` at 0.080; none supplies a source or relocation witness.
- history: The current guarded body is pinned by `8b9b9a9fcd06507d928d66bccd5ee29c37bd36f2`; the earlier frame, flag, and permutation families remain exhausted. This evidence refresh changes no executable source.
- next action: Permit only a forced configured V0 and structured finalization pass. Reopen source matching only after independently authenticated call-proxy identities or another genuinely new source mechanism.
<!-- plateau-handoff:overlay17CalculateEndpoints:end -->
