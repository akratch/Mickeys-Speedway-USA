<!-- plateau-handoff:func_overlay_101_F000C6E8_18E7F08:start -->
### `func_overlay_101_F000C6E8_18E7F08` plateau handoff

- source: `src/overlays/o101/overlay101TailC6E8.c`
- score: 106 differing words
- frame: 0x18
- relocations: 91
- first mismatch: +0x4C
- summary: Two uopt region boundaries took 124 masked words to 119 at exact size and 0x18 frame. 2026-09-11, lane p2-close, 119 -> 106 with immediate-only words 2 -> 0 and structural 31 -> 26: four L59 group folds, chosen by measuring all 1,024 subsets of the ten foldable assignment groups, plus a no-op `& 0xFF` on the queue copy, which folds away at zero byte cost but still makes ugen allocate one more expression temporary and so moves the caller-saved free list one place. The shard's older claim that the target holds `selector` in a1 and this candidate a2 is stale: both hold it in a1 now, and the visible difference is the queue-byte base, t7 there against a2 here. The residue is 75 register-naming words, and it is p1 colouring in the root-initialisation region.
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
#### 2026-09-11, lane f10-mid: p1-only, and the a1 holder is web 101

Still 119. Frame 0x18 and every stack home agree on both sides, so the
residual is allocation and ring phase only. The instrumented uopt (text
confirmed identical) records 31 p1 decisions and no p2 records: the function
contains calls, so "selector declaration order" measured an axis it does not
have. The closure says the target holds the selector in a1; in the candidate
a1 goes to web 101 (save 5.0, totalsave 10, nocs 2, cost 0) and later to
web 4 (save 0.667, nocs 3). The decision variable is therefore the ratio
that orders the selector's web against web 101 ([L100]); a symbol-boundary
edit or an L109 probe on the selector is the lever, and neither has been
measured. The web numbers are from this lane's own compilation and must be
re-derived before forcing.

<!-- plateau-handoff:func_overlay_101_F000C6E8_18E7F08:end -->
