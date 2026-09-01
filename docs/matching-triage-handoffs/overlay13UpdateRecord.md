<!-- plateau-handoff:overlay13UpdateRecord:start -->
### `overlay13UpdateRecord` plateau handoff

- source: `src/overlays/o013/overlay13ProcessRecord.c`
- score: 65/161 words
- frame: 0x20
- relocations: 5
- first mismatch: +0x20
- summary: Fresh target-named V0 retains 96 differences; the overlay13Prepare call identity remains unresolved despite exact relocation shape.

- geometry: Target and configured C are both `0x284`/644 bytes/161 words with exact `0x20` frame; the owned Overlay 13 range is `+0x284..+0x508`, ROM `0x186ED9C..0x186F020`, immediately followed by `overlay13ProcessActive` with no padding.
- ABI/flags: The configured candidate is `s16 *overlay13UpdateRecord(Overlay13Record *, s32)` under the overlay game-code `-O2 -mips2 -32` group and the canonical symbol-redefine/trim postprocess.
- relocation proof: Target and candidate each contain five records with all five offsets/types aligned. Four stable/effective identities align; the `R_MIPS_26` call at `+0x20` remains an unresolved `overlay13Prepare` proxy, so identity proof is partial.
- diagnosis: Workbench reports structure mismatch with 63 opcode differences, 31 register differences, and 92 alignment gaps; all 96 raw/masked positional differences now begin at `+0x20`, superseding stale prose `+0x2C`.
- caller/donor evidence: One Overlay 13 runtime call at `+0x550` and the overlay export authenticate the entry. Skeleton donors are weak: the best is Conker `func_151A6350` at 0.0588 Jaccard, with no unique source or relocation witness.
- history: The guarded body last changed in `17799071a3f2e122d93e466b48aca236c62d67d9`, before the stale prose commit `de899357a595`. This target-named refresh changes no executable source.
- next action: Run only a forced V0 and structured finalize maintenance pass. The 119-flag lattice, tick-loop CFG, result types, pointer lifetimes, broad permutation, and C-body mutation remain closed.
<!-- plateau-handoff:overlay13UpdateRecord:end -->
