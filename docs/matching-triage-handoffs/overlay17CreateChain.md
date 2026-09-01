<!-- plateau-handoff:overlay17CreateChain:start -->
### `overlay17CreateChain` plateau handoff

- source: `src/overlays/o017/overlay17CreateChain.c`
- score: 66/196 words
- frame: 0x88
- relocations: 7
- first mismatch: +0x0
- summary: Fresh exact-size V0 retains 130 differences; candidate frame is 0x88 versus target 0x80 and two template identities remain unresolved.

- geometry: Target and configured C are both `0x310`/784 bytes/196 words; the owned Overlay 17 range is `+0x318..+0x628`, ROM `0x1873CD0..0x1873FE0`, followed by `overlay17ReleaseChain` with no padding. Target frame is `0x80`; candidate frame is `0x88`, entirely in non-save bytes.
- ABI/flags: The configured constructor takes owner/count/material/scale, four `f32` geometry values, and four color bytes, returns `Overlay17Chain *`, and uses overlay game-code `-O2 -mips2 -32`.
- relocation proof: Target has seven records and candidate five; only three offsets/types and one stable/effective identity align. The candidate template address at `+0xA8/+0xB4` remains an unresolved HI16/LO16 pair, so count, position, and identity proofs are partial.
- diagnosis: Workbench reports structure mismatch with 41 opcode differences, 77 register differences, and 28 alignment gaps; all 130 raw/masked positional differences begin at entry.
- caller/donor evidence: One resident caller at VRAM `0x8001C70C` and the overlay export authenticate the entry. Donors are weak: Conker `func_1502A8A0` leads at 0.0574 Jaccard, with no source or relocation witness.
- history: The guarded body and latest prose are both pinned by `1e385889f2a11285eb9d4f4050f955c1de5d64e3`. This exact-symbol evidence refresh changes no executable source.
- next action: Run only forced V0 and structured finalize maintenance. Prior frame-local, flag, permutation, and C-body mutation families remain closed.
<!-- plateau-handoff:overlay17CreateChain:end -->
