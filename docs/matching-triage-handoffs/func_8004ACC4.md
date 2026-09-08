<!-- plateau-handoff:func_8004ACC4:start -->
### `func_8004ACC4` plateau handoff

- source: `src/main/fx.c`
- score: 14 differing words
- frame: frameless
- relocations: 12
- first mismatch: +0x14
- summary: JFG efd5abb counterpart remains assembly-only; structure-buckets has no proved lever. Resume with new callback/trap source; configured 14/28 retained.

Reopening audit (2026-09-08): PROVENANCE: Jet Force Gemini public decomp,
`src/fx.c` at `efd5abb1c79636e297b831f7c2d5bf47eac39c0c`, counterpart
`func_8006F874_70474`. The pinned file contains only its `GLOBAL_ASM`
placeholder. Its last change is `6ff6177` (2026-08-09), adding an include
and a scratch reference for the unrelated `setupClearScreen`. The companion
`src/fx.h` supplies no implementation. This revision therefore supplies no
new callback/trap or counter source topology for this target. No donor body
was adopted; Mickey remains the authority.

Configured full-TU baseline: 112 bytes, 28 words, size delta zero, 14 raw
and relocation-masked differences, first +0x14, frameless. Target and
candidate each carry 12 relocations; 8 tuples agree in relative offset,
type and symbol identity. Diagnosis is `mixed(structural:10, register:5)`,
playbook `structure-buckets`, lever `none-known`. The flag/context parity
lever was checked by the requested full-TU lattice; allocator-only edits
are not justified by this structural evidence.

One requested TU lattice evaluated 119 rows: 53 scored and 66 were rejected
by the asm-processor wrapper (30 unsupported MIPS1 optimization combinations,
36 lacking a supported optimization/debug selector). Thus coverage is
partial, with no exact scored row. MIPS1 at O2 gives a diagnostic 12/28 raw
and masked residual with the same extent and relocation deficit; it worsens
`func_8004AF68` to 41 differing words and `func_80048760` to 47, each four
bytes longer. Both sibling scores came from compile-free rescoring of the
same content-addressed cache. No flag change is proposed; the configured
14-word candidate remains the retained source.

Stopping evidence: zero source attempts, one requested flag experiment.
The authorized new-donor mechanism has no new target implementation; the
committed ten source families remain exhausted. This is the explicit
early-exhaustion stop, not a five-attempt stall. Next action requires a
new published C body or source-authentic callback/trap and counter-web
evidence, followed by fresh assignment authorization and full-TU proof.

Evidence retained under ignored `build/m14-fx-batch/` and `build/wb/`:
baseline source/object, configured summary and diagnosis, lattice logs and
JSON, diagnostic comparison and relocation counts. Commands: `wb_compare.sh`,
`decomp-workbench diagnose`, `guide structure-buckets`, one `flag_sweep.py`
run plus two `--rescore` calls, and `finalize_plateau.py`. No candidate was
promoted or given linked-C/ROM-exact credit.

<!-- plateau-handoff:func_8004ACC4:end -->
