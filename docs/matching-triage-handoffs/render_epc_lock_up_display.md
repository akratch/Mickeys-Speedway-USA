<!-- plateau-handoff:render_epc_lock_up_display:start -->
### `render_epc_lock_up_display` plateau handoff

- source: `src/main/diCpu.c`
- score: 62/344 words
- frame: 0x50
- relocations: 127
- first mismatch: +0x144
- summary: Baseline retained; final-report variants regressed and cited JFG revision has no diCpu source

2026-09-11 causal re-check: baseline remains 283 byte-exact words, 57
register-naming words, one immediate-only word, and four really-different
words, with zero displacement tax. Candidate and target have 344 words, a
0x50 frame, matching eight-slot ladders, 127 relocations each, and 125 exact
relocation identities.

Attempt 28 moved the two final report blocks as one unit because the
workbench found a four-word block shifted by 16 words. It regressed to 72
masked differences: 276 byte-exact, 54 register naming, one immediate-only,
and 17 really different. The whole-report order is not the source of the
shifted block.

Attempt 29 kept the evaluation order but saved the level result in the
already-dead `sp44` local before delaying its report. It regressed to 80
differences at 346 candidate words and added two stack-slot accesses. Delayed
reporting through the dead out-parameter local is refuted.

The explicitly authorized public JFG donor revision `efd5abb1c79636e297b831f7c2d5bf47eac39c0c`
was resolved read-only. Its changed paths are README and tooling only, with no
`diCpu.c` source, so it cannot reopen this function's body reconstruction.
The temp mapping is 68 percent coherent and its cycle is expressly not a
ring-phase precondition. The next actionable evidence is a JFG revision that
actually supplies this function's C body, or a source mechanism that delays
only the level report without adding a live value across the final call.
<!-- plateau-handoff:render_epc_lock_up_display:end -->
