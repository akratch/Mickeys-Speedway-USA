<!-- plateau-handoff:func_overlay_029_F0000EE0_187E190:start -->
### `func_overlay_029_F0000EE0_187E190` plateau handoff

- source: `src/overlays/o029/overlay29ProjectPoint.c`
- score: 92/121 words
- frame: 0x70
- relocations: 3
- first mismatch: +0x0
- summary: New resolver proves 3/3 identities exact; remaining 29-word allocation/frame cascade names no unique natural lever.

- layered evidence: canonical tool merge `e9890238417f69494b40918602eaaef8ea8e1b6e`; one-shot authorization `10d9f7f44a57542bec78ec5abdd75fd5cdbf2301`; pinned source `819c9f9a3b717545574dc318cd56d4797fbb2c20`; pinned prior plateau `fda619c419f8444585de86e87c9e3eaa088b1975`.
- boundary and geometry: overlay 29 text `+0xEE0..+0x10C4`, exactly 484 bytes / 121 words. Configured C reproduces the exact instruction count with no owned padding; candidate frame is `0x70` versus target `0x68`.
- relocation proof: preflight is complete with 3 candidate and 3 target records. Offset/type, stable identity, and effective identity align 3/3. The local HI16/LO16 pair at `+0x20/+0x24` resolves uniquely to overlay 29 local data with addend 20; the call at `+0x10C` resolves to the resident square-root routine. No shared-VMA inference is used.
- fresh residual: 92/121 positional words are exact, 29 relocation-masked words differ, first mismatch is `+0x0`, and workbench reports `allocation-mismatch`. Save-slot use agrees; the candidate has eight extra bytes of non-save frame storage.
- actionable diagnosis: complete relocation identity proof removes the former promotion blocker but does not isolate a source lever. The residual contains a frame-wide constant cascade, ten FP allocation sites, and three independent commutative-order sites. Because no one natural edit is uniquely supported, the authorization's optional source experiment was not exercised.
- closed search: flags, permutation, prior pointer/expression forms, fake carriers, and score-only scaffolds remain closed. The guarded body is unchanged.
- next action: reopen only with Mickey-authenticated original declaration/lifetime evidence that explains both the eight-byte non-save-frame excess and the first divergent FP pool web; relocation identity work is complete.
<!-- plateau-handoff:func_overlay_029_F0000EE0_187E190:end -->
