<!-- plateau-handoff:render_epc_lock_up_display:start -->
### `render_epc_lock_up_display` plateau handoff

- source: `src/main/diCpu.c`
- score: 51/344 words
- frame: 0x50
- relocations: 127
- first mismatch: +0x18
- summary: The residual is a measured ugen integer ring phase; three argument hoists removed three draws and the rest wants one phantom pop at the top.

#### 2026-09-12, lane p9-mid: 62 -> 51, with the ring measured rather than inferred

The closure asks for "a JFG revision that actually supplies this function's C
body, or a source mechanism that delays only the level report". Neither was
needed. The residual is a ugen integer ring phase, and this is the first time
that ring has been read directly on this project rather than inferred from a
register census.

Aligner buckets before: 283 byte-exact, 57 register naming, 1 immediate-only, 4
really different, at 62 masked, size delta 0, zero displacement tax. After: 293
byte-exact, 49 register naming, 0 immediate-only, 2 really different, at 51 and
the same size. More agreement on every axis; nothing was traded.

**The frame is not the question.** `tools/frame_census.py` prints two identical
eight-slot ladders with no slot either side uses alone, so the whole residual is
allocation, as the closure said.

**The ring, measured.** The instrumented-ugen free-list trace (DKWB_UGEN_TRACE)
over this translation unit reports this function as procedure ordinal 9 with 32
integer allocations and 247 frees. Its draw order is **t6, t7, t8, t9, t0, t1,
t2, t3, t4, t5**, wrapping -- which is not the natural numeric order and is why
the register census could not read a clean cycle out of it (68 percent
coherence, three windows). Each allocation is stamped with the source line that
consumed it, so the phase is settable per statement. This is the instrument the
closure should have asked for.

**Three hoists, each removing one draw.** Putting a printf's stack-passed
argument into the already-declared `value` local before the call removes the
ring draw that argument would otherwise take: the exception-address line takes
62 to 56, the cause-table lookup 56 to 53, and the tick counter on the first
line 53 to 51. A greedy subset search over eighteen such hoists -- every
single-line cpuXYPrintf in the function that passes an argument on the stack --
converges at 51.

**One tension, stated rather than hidden.** The third hoist puts the first
line's stack argument in `value`'s own colour where the shipped code uses a ring
temp, so it disagrees at three sites it previously agreed on while agreeing at
five more. It wins on both buckets and the schedule does not move, so it is
retained, but a form that removes that draw *without* naming the value would be
strictly better and is the first thing the next lane should look for.

**The decision variable.** The shipped code's first visible ring temp is t7 --
the *second* entry in the list ugen builds, not the first. So it makes one draw
this candidate does not, before a single instruction is emitted, and the whole
remaining 49-word naming residual is that one offset propagating. A phantom pop
at the very top of the function is the shape wanted: a construct that allocates
an integer ring temp and emits nothing. The or-with-zero family does that
elsewhere in this tree; placed on `value` at four points here it is byte-inert,
so it is not that form at this site.

**Also measured and flat:** hoisting either runlink result argument, both orders
of that pair, `s32` and pointer declarations for the three out-parameters,
dropping the `value` local entirely, three spellings of the register-block
pointer, moving `value` or `regs` to the head of the declaration list (both
cost 10 words), spelling the -1 compare as a literal, and hoisting the
region-line argument (78).

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
