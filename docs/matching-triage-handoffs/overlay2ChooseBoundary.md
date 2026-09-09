<!-- plateau-handoff:overlay2ChooseBoundary:start -->
### `overlay2ChooseBoundary` plateau handoff

- source: `src/overlays/o002/overlay2ChooseBoundary.c`
- score: 186/292 words
- frame: 0x90
- relocations: 64
- first mismatch: +0x5c
- summary: Natural loop-carrier and declaration ordering cut masked differences 247 to 106 with exact size/frame and 52/64 relocation roles; direct-branch forms regress.
- baseline: 45/292 words, 0x98 frame, 62 relocations, 247 masked differences
- attempts: the complete 119-combination flag lattice did not beat canonical O2/mips2; coherent direct-branch and carrier-width forms regressed
- bounded permutation: a five-minute, gain-gated batch improved its internal score only by inserting an inert empty guard, so the result was rejected and not applied
- blocker: the candidate emits an extra `sltu` at +0x5c; removing it with natural direct-branch spellings disrupts later allocation and function geometry
- next lever: reproduce the target's direct count branch while retaining the one-carrier 0x90 frame and current stack-slot order; then resolve the remaining relocation identities
- validation: `tools/wb_compare.sh --summary-json overlay2ChooseBoundary`; `gmake verify`; `gmake cleanroom`; `gmake check-docs`; `gmake check-scoreboard`
<!-- plateau-handoff:overlay2ChooseBoundary:end -->
