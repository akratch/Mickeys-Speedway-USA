<!-- plateau-handoff:overlay58FinalizePackedStatus:start -->
### `overlay58FinalizePackedStatus` plateau handoff

- source: `src/overlays/o058/overlay58FinalizePackedStatus.c`
- score: 88/304 words
- frame: 0x48
- relocations: 48
- first mismatch: +0x18
- summary: p1-only; 76% coherent. 29 p1 decisions, no p2; candidate homes 24/60/68 versus target 28/36/48/64; probes flat.
- assignment base: `ccbd4a78b29afb17ad817dd9228f774012b7d9ac`
- owned range: overlay 58 `+0x5554..+0x5A14`, 1,216 bytes / 304 words; the following `+0x5A14..+0x5A20` range is separately owned padding
- baseline: exact 304-word geometry and `0x48` frame, with 178 raw differences, 177 relocation-masked differences, first raw mismatch `+0x8`, and first masked mismatch `+0x18`
- retained score: 127/304 relocation-masked words match; evaluating the desired-rank condition before the loop bound lowers the workbench normalized structural distance from 177 to 176 without changing size, frame, calls, or semantics
- relocation proof: target and candidate each emit 48 records; 42 offsets/types align, but all 48 candidate identities remain unresolved and zero stable identities align, so `function_preflight.py` correctly returns partial and fails promotion closed
- mismatch proof: the retained aligned view classifies 125 register, 62 structural, seven schedule, and nine constant sites; the residual is broad rather than a near-exact allocator-only tail
- attempts: ten bounded source hypotheses covered loop-condition order, explicit pointer/rank carriers, extended-mode nesting, declaration order, captured-player reuse, stack-home ordering, next-count lifetime, and `for`/`do` loop forms; the complete 119-row flag lattice found no exact result and tied canonical `-O2 -mips2 -32` for best at 177 masked differences
- retained improvement: preserve the desired-first first-loop condition; every other source form regressed geometry, frame, or difference count, and no generic permuter was run
- next action: reopen only with new evidence for the stack-home/declaration web and the mask/shift carrier allocation, together with authenticated candidate relocation identities; do not repeat these ten forms or the completed flag lattice
- JFG: no credible insertion point was found. The pinned JFG overlay donor row is `none`; the nearest JFG skeleton, `func_overlay_3_00304968_1ED9E48`, has only 0.032 similarity and supplies neither a source analogue nor reusable code
#### 2026-09-11, lane f10-mid: the memory-resident set differs, which no home order reaches

Still 88. The function contains calls; the instrumented uopt (text confirmed
identical) records 29 p1 decisions and no p2 records, so declaration order
decides no colour here. The closure's "stack-home order" axis is real for
homes ([L99]) but the census shows the sets differ, not the order: with frame
0x48 on both sides the candidate is resident at 24, 60 and 68 and the target
at 28, 36, 48 and 64, three items against four. Which webs are left in memory
is a p1 colour-or-split decision, so the variable is the save ratio of the
web the target spills and the candidate colours; the p1dec records are in the
lane log and were not joined to symbols this pass. The six L109 probes already
in the source move exactly these ratios, so the next pass should read the
records before adding a seventh.

#### 2026-09-11, lane p5-ovl: joined p1 records and closed two new probes

The fresh baseline remains 88/304 relocation-masked words, with aligned
buckets 227 exact, 41 register naming, 14 immediate-only, and 26 structural;
the frame is 0x48 and the relocation count is 48. The instrumented object is
text-identical to stock. Its 29 p1 decisions have no p2 decisions; the
webdetail frame ladder identifies candidate resident homes 24(sp) = web 99,
60(sp) = web 84, and 68(sp) = webs 0/299. The target object instead uses
28, 36, 48, and 64(sp), so the discrepancy is a resident-set difference, not
home order. The target disassembly labels the distinct homes as records, loop
count, current, and post-call count; the candidate shares one home for the two
count webs. A post-increment count probe and an early records-pointer probe
both remained exactly 88/304 with identical buckets and were reverted.
Forcing candidate p1 count/current webs to split regressed to 100--272 masked
words, so those controls do not identify the target path. Candidate source and
trace evidence is now preserved; no exact source spelling was proved.

<!-- plateau-handoff:overlay58FinalizePackedStatus:end -->
