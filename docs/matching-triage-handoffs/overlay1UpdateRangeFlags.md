<!-- plateau-handoff:overlay1UpdateRangeFlags:start -->
### `overlay1UpdateRangeFlags` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 41 differing words
- frame: 0x70
- relocations: 4
- first mismatch: +0x34
- summary: Inlining the two Y temporaries makes the frame exact; 31 register sites and four unresolved call identities remain after five natural forms; no permuter.
- assignment base: `58a7171f2004b2b693a19b244c66222a7183222a`
- owned range: Overlay 1 `+0x67C0..+0x69A0`, 480 bytes / 120 instructions, with no size delta
- baseline: 34 raw differing words, candidate frame `0x78` versus target `0x70`, and first mismatch `+0x0`; the opcode schedule was already exact
- retained improvement: Remove the two named Y temporaries and use the object fields directly. This preserves the two height predicates while reducing the hidden local area by eight bytes and making the frame exactly `0x70`.
- current comparison: 41 raw differing words and normalized distance 32. The raw count includes unresolved call targets; all 120 opcodes remain aligned, with 31 register sites (12 FP) and no instruction-count delta. The first non-relocation allocation mismatch is `+0xA0`.
- relocation proof: All four `R_MIPS_26` sites align by function-relative offset and type, with no relocation-metadata mismatch, but 0/4 stable identities resolve. The target identities are Overlay 1 `+0x0`, `+0x0`, `+0x4B4`, and `+0x19B8` at function offsets `+0x34`, `+0xCC`, `+0x194`, and `+0x19C` respectively.
- attempts: Five coherent forms were compiled. Splitting the angle call through a new local regressed the frame with no allocation gain; two squared-distance lifetime forms were rejected because they changed the later angle-call arguments; reusing the dead horizontal-range variable for the call result compiled identically. No generic permuter was run because no semantics-preserving rewrite reduced the allocator residue.
- mechanism: `decomp-workbench` reports `allocation-mismatch`; the first FP-pool divergence is at `+0xA0`, the first temp-FIFO divergence at `+0xD4`, and the first general pool divergence at `+0xDC`.
- next action: Reopen only with new FP-pool/temp-FIFO evidence or authenticated aliases for the four runtime call identities. Do not repeat the discarded distance rewrites or start a generic sweep.
- DKR/JFG: The pinned DKR v77/v80 and JFG overlay scans report no Overlay 1 donor. The full skeleton scan has no hit at `+0x67C0`, and coddog found no exact 20-instruction window, so this candidate does not support a JFG public-ledger entry.
<!-- plateau-handoff:overlay1UpdateRangeFlags:end -->
