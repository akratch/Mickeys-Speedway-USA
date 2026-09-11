<!-- plateau-handoff:fxScreenEffect:start -->
### `fxScreenEffect` plateau handoff

- source: `src/main/fx.c`
- score: 121 differing words
- frame: 0x30
- relocations: 10
- first mismatch: 0x8
- summary: 123 to 121 masked on two web-span edits and one store-topology edit, size delta -28 to -16, 143 of 147 words, frame 0x30 on both sides with an identical 14-slot ladder. The decisive instrument was the instruction multiset, not the aligner: at baseline the candidate differed from the target by exactly five register copies, one shift and one store and nothing else; it now reads three copies and one shift. What paid, each measured alone: moving the cursor initialiser inside the guarded block, one word; giving the limit a guard-local carrier, one word; and re-reading the display-list pointer between the two adjacent writebacks so the second is not folded into a dead store, one word. Decision variable reached: the remaining three copies are ugen colour assignments, not authorable symbols -- every value the target copies is already a named symbol here and coalesces, and the target's copies survive because the destination colour differs from the producer temp, which is downstream of allocation. The next lever is an allocator trace on the four preheader webs read against L100 save ratios; this is a p1-only procedure, so declaration order and statement order are not axes. Refuted: the prior note asking for stack-argument preload topology evidence mis-states the gap, because both sides load all five stack arguments and the frames and slot ladders were already identical. Also refuted, the caveat this lane was dispatched with: the geometry did not have to close first, it was already closed. After the edits the register census resolves into one clean four-cycle over t9, t6, t7 and t8 across 34 sites where it previously showed two incoherent cycles, so closing the size deficit surfaced an L127 ring-phase fact underneath. Axes covered are listed in the shard.


Reopening audit (2026-09-08), evidence D: PROVENANCE inspection of Jet Force
Gemini public decomp `src/fx.c` and `src/fx.h` at
`efd5abb1c79636e297b831f7c2d5bf47eac39c0c` found no new target C body.
No donor source, names or values were adopted. Mickey remains authoritative.

Configured full-TU measurement: candidate 560 bytes / 140 words,
target 588 bytes / 147 words; size delta -28 bytes.
Raw and relocation-masked differences are 123 and 123, respectively,
first mismatch +0x8; candidate/target frames are
0x30/0x30. Candidate/target static relocation counts are
10/10; 7 tuples agree in function-relative offset, type and symbol.
These are fallback-object comparisons, not linked-C promotion evidence.

Workbench comparison: `structure-mismatch`. Diagnosis: `mixed(structural:131, register:25)`;
playbook `structure-buckets`, lever `none-known`. The named guides were read.
This heuristic routing supplies no new donor evidence to reopen exhausted forms.
Next concrete lever requires new screen-effect donor C showing a source-authentic stack-argument preload,
then a fresh gate and configured full-TU comparison.

Stopping evidence: zero source attempts; the authorized donor mechanism supplies
no new implementation. This is the assignment's early-exhaustion stop, not a
five-attempt stall. The prior TU lattice is not repeated: since its recorded
`func_8004ACC4` audit at `4be95a3d`, this TU differs only in EOF handoff comments.
See [the anchor donor audit](func_80049E4C.md) for the full delta and batch disposition.

Ignored evidence is retained in `build/tu-fx-audit/` and `build/wb/`: source and
full-TU object, gate receipts, summaries, diagnoses and relocation tuples.
Commands: `lane_status.py --symbol`, `wb_compare.sh --summary-json`, workbench
`diagnose` and `guide`, and `finalize_plateau.py`. The unchanged guarded C stays
NON_MATCHING and receives zero new exact bytes.

Prior plateau evidence retained (historical measurements and exhausted forms):

- relocation identity: Candidate and target each have 10 static relocations; 7 offsets, types, and identities align in fallback-static evidence.
- flag lattice: All 119 combinations were nonexact. The `-O2 -g3 -mips2` diagnostic was 141 words with 131 differing words and first mismatch `+0x4`; its remaining ABI/structure mismatch does not justify a translation-unit flag change.
- donor result: Mickey-only `m2c` reproduced the stale candidate; JFG provides assembly-only structural context, and the similarity scan found no credible exact C donor.

<!-- plateau-handoff:fxScreenEffect:end -->
