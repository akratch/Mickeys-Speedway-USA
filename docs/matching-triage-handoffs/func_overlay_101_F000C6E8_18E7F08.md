<!-- plateau-handoff:func_overlay_101_F000C6E8_18E7F08:start -->
### `func_overlay_101_F000C6E8_18E7F08` plateau handoff

- source: `src/overlays/o101/overlay101TailC6E8.c`
- score: 124 differing words
- frame: 0x18
- relocations: 91
- first mismatch: +0x8
- summary: one commutative pointer-add word closed; 31 opcode/allocator differences and opaque runtime identities remain
- assignment base: `23dd98ede1da869df2df0948c80efeeb403caa6e`
- owned range: overlay 101 `+0xC6E8..+0xCBDC`, 1,268 bytes / 317 words, with no target-size delta
- baseline: 131 raw and 125 relocation-aware positional differences; normalized distance 117; first raw mismatch `+0x8` and first relocation-masked mismatch `+0x34`
- retained result: 130 raw and 124 relocation-aware positional differences; normalized distance 116; exact 317-word size and exact `0x18` frame
- relocation proof: target and candidate each emit 91 runtime records; 86/91 offsets and types align, one identity is statically stable, and 90 candidate identities remain unresolved, so preflight correctly fails closed
- classification: workbench reports `mixed(constant:7, structural:48, schedule:33, register:81)` with 31 opcode differences; the retained form removes the sole commutative-order difference
- attempts: five bounded source hypotheses—explicit root constants, selector declaration order, pointer-add operand order, selector carrier width, and a persistent root pointer. Three were byte-flat, the root pointer regressed to 327 words, and only the pointer-add ordering improved. The historical flag lattice and root-order work were not repeated; no generic permuter or new flag sweep ran
- retained improvement: spell the queue end as count plus cursor, matching the target's commutative operand order and closing one positional/register word without changing semantics
- next action: reopen only with new source evidence for the root-initialization constant-preload schedule and selector/temp allocation web; do not repeat these five forms or the prior flag lattice
- JFG utility: the overlay donor scan reports no JFG candidate for overlay 101. JFG `src/overlays/o19/overlay_19.c::pauseUpdate` is the nearest skeleton at only 0.0691 and remains assembly-only, so it is context rather than a credible source or insertion point; there is no JFG ledger entry to publish from this plateau
<!-- plateau-handoff:func_overlay_101_F000C6E8_18E7F08:end -->
