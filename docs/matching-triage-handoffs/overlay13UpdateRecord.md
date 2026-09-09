<!-- plateau-handoff:overlay13UpdateRecord:start -->
### `overlay13UpdateRecord` plateau handoff

- source: `src/overlays/o013/overlay13ProcessRecord.c`
- score: 65/161 words
- frame: 0x20
- relocations: 5
- first mismatch: +0x20
- summary: Authorized V0 reproduces 96 differences; current tooling still cannot authenticate the overlay13Prepare call proxy.

- geometry: Target and configured C remain exact at `0x284`/644 bytes/161 words with `0x20` frame; the owned Overlay 13 range is `+0x284..+0x508`, ROM `0x186ED9C..0x186F020`, followed by `overlay13ProcessActive` with no padding.
- ABI/flags: The configured candidate is `s16 *overlay13UpdateRecord(Overlay13Record *, s32)` under overlay game-code `-O2 -mips2 -32` and the canonical symbol-redefine/trim postprocess.
- relocation proof: Target and candidate each contain five records with all five offsets/types aligned. Four stable/effective identities align; the `R_MIPS_26` call at `+0x20` remains unresolved because no canonical `overlay13Prepare` definition/alias or exact sibling witness exists.
- diagnosis: Workbench reproduces 96 raw/masked positional differences from `+0x20`, with 63 opcode differences, 31 register differences, and 92 alignment gaps.
- caller/donor result: One Overlay 13 runtime call at `+0x550` and the export authenticate the entry. Donors remain weak: Conker `func_151A6350` leads at 0.0588 Jaccard and supplies no source or relocation witness.
- maintenance result: Current proxy tooling yielded no genuine lever, so no C-body, flag, or permutation hypothesis was attempted under the authorization.
- next action: Preserve the fallback pending unique same-overlay call-proxy evidence or another explicitly authorized mechanism. Previously exhausted loop, result-type, pointer-lifetime, flag, and permutation families remain closed.
<!-- plateau-handoff:overlay13UpdateRecord:end -->
