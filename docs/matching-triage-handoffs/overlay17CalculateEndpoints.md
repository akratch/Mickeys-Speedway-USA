<!-- plateau-handoff:overlay17CalculateEndpoints:start -->
### `overlay17CalculateEndpoints` plateau handoff

- source: `src/overlays/o017/overlay17CalculateEndpoints.c`
- score: 65/198 words
- frame: 0x58
- relocations: 3
- first mismatch: +0x24
- summary: Fresh V0 retains 133 differences; call proxies have no exact sibling witness, zero identities resolve, and only one relocation site aligns.

- geometry: Target and configured candidate remain exactly `0x318`/792 bytes/198 words. The owned Overlay 17 range is `+0x0..+0x318`, ROM `0x18739B8..0x1873CD0`, followed immediately by `overlay17CreateChain` with no padding.
- ABI/flags: `void overlay17CalculateEndpoints(Overlay17ChainHead *, f32 *, f32 *, f32 *, f32 *, f32 *, f32 *)` uses Overlay 17 game-code `-O2 -mips2 -32`. Target and candidate frames are both `0x58`.
- relocation proof: Target and candidate each have three `R_MIPS_26` records. Target sites are `+0x68`, `+0x108`, and `+0x1D4`; candidate sites are `+0x68`, `+0x10C`, and `+0x1D8`. One offset/type aligns and zero candidate identities resolve.
- diagnosis: Workbench reports 65/198 matching words, 133 raw/masked differences from `+0x24`, 88 opcode mismatches, and no instruction-count delta. Its acceptance basis is relocation-symbol mismatch.
- callers: Overlay 17 calls at `+0x51C` and `+0x75C` authenticate the entry and seven-argument ABI.
- donors: Fresh masked-skeleton search remains weak: Conker `func_1512DEA4` leads at 0.086 Jaccard, followed by Conker `func_151B118C` at 0.084 and Perfect Dark `model_update_chr_info` at 0.080; none supplies a source or relocation witness.
- proxy recheck: `overlay17TransformReloc` and `overlay17SqrtReloc` occur only in this unmatched function. No exact same-overlay sibling authenticates either name, and current tooling correctly refuses target-assisted identity inference or offset normalization.
- next action: Reopen only if independent exact ownership evidence authenticates the call proxies or a genuinely new source mechanism explains the two four-byte call-site displacements. Prior source, flag, and permutation families remain closed.
<!-- plateau-handoff:overlay17CalculateEndpoints:end -->
