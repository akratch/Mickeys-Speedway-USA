<!-- plateau-handoff:overlay58DrawSegmentStrip:start -->
### `overlay58DrawSegmentStrip` plateau handoff

- source: `src/overlays/o058/overlay58DrawSegmentStrip.c`
- score: 99/201 words
- frame: 0x88
- relocations: 8
- first mismatch: +0xC
- summary: Term rotation regresses; operand reversal and explicit dereference are flat.
- assignment base: `9c62568e60deef2170043478e9473e5b42fd27b4`
- owned range: overlay 58 `+0x4C04..+0x4F28`, 804 bytes / 201 words
- fresh baseline: 99 of 201 words exact, 102 positional differences, first mismatch `+0xC`; exact `0x88` frame
- relocation proof: target and candidate each emit eight records with 8/8 offsets and types aligned; all eight candidate static identities remain unresolved, so preflight fails closed
- new mechanism evidence: the later exact Overlay 22 result rotates independent floating-point terms and uses one explicit `(*pointer).field` operand spelling; the Overlay 41 learning independently proves term rotation and commutative scalar-multiply order as valid IDO levers
- bounded probes: rotating the distance sum from `dx, dy, dz` to `dy, dz, dx` regresses to 91 of 201 words and moves the first mismatch to `+0x4`; reversing both perpendicular scalar multiplies is byte-flat at 99 of 201; an explicit dereference at the first floating-point-derived vertex store is also byte-flat
- exhausted prior work: ten cursor, alias, color-carrier, declaration/lifetime, scalar-carrier, and command-store forms plus all 119 flag combinations remain closed; no permutation was run because none of the three new probes produced a strict gain
- retained source: the original negative-index cursor spelling remains best and fixes one dead-pointer advance immediate without changing write order or semantics
- blocker: workbench still reports broad allocation mismatch, with the global-cursor/color and floating-point pools cyclically recolored; relocation identities also require resolution before any future promotion
- next action: do not reopen the prior ten forms, flag lattice, or these three FP-order probes; require a new source-authentic lever for the global-cursor/color web or whole floating-point pool
- donor check: no credible JFG use location exists; the nearest cross-title skeleton remains Conker at 0.048, with no exact coddog window or source analogue
<!-- plateau-handoff:overlay58DrawSegmentStrip:end -->
