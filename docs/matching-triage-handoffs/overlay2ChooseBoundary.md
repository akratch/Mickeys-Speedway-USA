<!-- plateau-handoff:overlay2ChooseBoundary:start -->
### `overlay2ChooseBoundary` plateau handoff

- source: `src/overlays/o002/overlay2ChooseBoundary.c`
- score: 97/292 words
- frame: 0x90
- relocations: 64
- first mismatch: +0x5C
- summary: p1-only; 95% coherent ring. Direct-count rewrite regressed to 260; line self-assignment text-flat at 97.
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

#### 2026-09-11, lane p5-ovl: p1 trace and ring probe

The fresh baseline is 97/292 relocation-masked words at unchanged 0x90 frame
and 64 relocations: 225 exact, 44 register naming, 17 immediate-only, and 7
structural. Instrumented text is stock-identical; the trace has 51 p1
decisions and no p2. Register substitutions are 95% coherent and form one
closed t-register ring, so the cycle is not an incoherent mapping. The target's
first structural row uses a direct decrement/branch shape; an explicit
pre-body decrement changed geometry and regressed to 260/292. A self-assigned
inner-loop pointer, tested as a zero-byte ring-phase probe, was text-identical
and remained 97/292. Both probes were reverted; no source improvement was
proved.

<!-- plateau-handoff:overlay2ChooseBoundary:end -->
