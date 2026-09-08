<!-- plateau-handoff:wakeAllocate:start -->
### `wakeAllocate` plateau handoff

- source: `src/main/fx.c`
- score: 345 differing words
- frame: 0x98
- relocations: 3
- first mismatch: 0xc
- summary: JFG efd5abb remains assembly-only; zero source attempts. Need new initialization homes and buffer-loop topology evidence.


Reopening audit (2026-09-08), evidence D: PROVENANCE inspection of Jet Force
Gemini public decomp `src/fx.c` and `src/fx.h` at
`efd5abb1c79636e297b831f7c2d5bf47eac39c0c` found no new target C body.
No donor source, names or values were adopted. Mickey remains authoritative.

Configured full-TU measurement: candidate 1372 bytes / 343 words,
target 1404 bytes / 351 words; size delta -32 bytes.
Raw and relocation-masked differences are 345 and 345, respectively,
first mismatch +0xC; candidate/target frames are
0x98/0x90. Candidate/target static relocation counts are
3/3; 0 tuples agree in function-relative offset, type and symbol.
These are fallback-object comparisons, not linked-C promotion evidence.

Workbench comparison: `structure-mismatch`. Diagnosis: `mixed(constant:25, structural:33, register:116)`;
playbook `constant-audit`, lever `stack-home`. The named guides were read.
This heuristic routing supplies no new donor evidence to reopen exhausted forms.
Next concrete lever requires new wakeAllocate donor C exposing initialization order, early stack homes and buffer-loop topology,
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

<!-- plateau-handoff:wakeAllocate:end -->
