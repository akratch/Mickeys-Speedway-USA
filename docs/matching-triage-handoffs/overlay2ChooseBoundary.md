<!-- plateau-handoff:overlay2ChooseBoundary:start -->
### `overlay2ChooseBoundary` plateau handoff

- source: `src/overlays/o002/overlay2ChooseBoundary.c`
- score: 195/292 words
- frame: 0x90
- relocations: 64
- first mismatch: +0x5C
- summary: Declaring to the target frame ladder read off the objects (side1 116, side2 112, bestScore 104, axis 96, endpoint 92) takes 106 masked words to 97 with every stack home exact; the extra sltu at +0x5C and 44 naming rows remain.
- baseline: 45/292 words, 0x98 frame, 62 relocations, 247 masked differences
- attempts: the complete 119-combination flag lattice did not beat canonical O2/mips2; coherent direct-branch and carrier-width forms regressed
- bounded permutation: a five-minute, gain-gated batch improved its internal score only by inserting an inert empty guard, so the result was rejected and not applied
- blocker: the candidate emits an extra `sltu` at +0x5c; removing it with natural direct-branch spellings disrupts later allocation and function geometry
- next lever: reproduce the target's direct count branch while retaining the one-carrier 0x90 frame and current stack-slot order; then resolve the remaining relocation identities
- validation: `tools/wb_compare.sh --summary-json overlay2ChooseBoundary`; `gmake verify`; `gmake cleanroom`; `gmake check-docs`; `gmake check-scoreboard`
#### 2026-09-11, lane f10-mid: the closure held the declaration list fixed, and the list was wrong

Masked 106 to 97 at delta 0, aligner byte-exact 209 to 225, immediate-only 33
to 17, naming 44 unchanged, structural 7 unchanged. `gmake verify` passed with
the guarded assembly still linked; no bytes are credited.

The retained closure named the extra `sltu` at +0x5C as the blocker and kept
"the current stack-slot order". The aligner said otherwise: 33 of the 106
words were immediate-only, and a stack-home census of both objects showed the
candidate resident at 84, 108 and 120 where the target is resident at 92, 96
and 104. `side1` is address-taken, so its home is pinned at 112 in the
candidate and observed at 116 in the target; reading the roles of the other
slots off the objects gives the target ladder as side1 116, side2 112,
bestScore 104, axis 96, endpoint 92 against the candidate's endpoint 120,
axis 116, side1 112, side2 108, bestScore 84. That is a pure [L99] readout:
one fewer local declared above side1, and axis and endpoint declared below
bestScore. Four declaration orders satisfying it all reach every home and all
score 97; the slots between (120, 108, 100) are register-class and inert.

The function contains calls, so it is p1-only and declaration order decides
no colour here; it decides homes, and that is what it was never used for.
What remains is the +0x5C `sltu` (structural, 7 rows) and 44 naming rows,
whose decision variable is the p1 save ratio ([L100]); the p1 records have
not been read for this function.

<!-- plateau-handoff:overlay2ChooseBoundary:end -->
