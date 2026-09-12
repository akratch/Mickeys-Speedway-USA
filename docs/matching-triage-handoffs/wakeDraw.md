<!-- plateau-handoff:wakeDraw:start -->
### `wakeDraw` plateau handoff

- source: `src/main/fx.c`
- score: 121/176 words
- frame: 0x50
- relocations: 1
- first mismatch: +0x0
- summary: Independent E7 command field scheduling removes one residual word; frame and outer-index lifetime remain unresolved.


Reopening audit (2026-09-08), evidence D: PROVENANCE inspection of Jet Force
Gemini public decomp `src/fx.c` and `src/fx.h` at
`efd5abb1c79636e297b831f7c2d5bf47eac39c0c` found no new target C body.
No donor source, names or values were adopted. Mickey remains authoritative.

Configured full-TU measurement: candidate 704 bytes / 176 words,
target 708 bytes / 177 words; size delta -4 bytes.
Raw and relocation-masked differences are 122 and 122, respectively,
first mismatch +0x0; candidate/target frames are
0x50/0x88. Candidate/target static relocation counts are
1/1; 1 tuples agree in function-relative offset, type and symbol.
These are fallback-object comparisons, not linked-C promotion evidence.

Workbench comparison: `structure-mismatch`. Diagnosis: `mixed(constant:9, structural:22, register:62)`;
playbook `constant-audit`, lever `stack-home`. The named guides were read.
This heuristic routing supplies no new donor evidence to reopen exhausted forms.
Next concrete lever requires new wakeDraw donor C exposing the outer-index spill and xStep lifetime,
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

<!-- plateau-handoff:wakeDraw:end -->
