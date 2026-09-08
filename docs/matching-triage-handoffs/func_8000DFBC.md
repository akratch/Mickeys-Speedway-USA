<!-- plateau-handoff:func_8000DFBC:start -->
### `func_8000DFBC` plateau handoff

- source: `src/main/track.c`
- score: 304 differing words
- frame: 0x70
- relocations: 51
- first mismatch: +0x48
- summary: JFG efd5abb has no matched counterpart C; zero new attempts. Prior mechanisms stay closed. Next: matched donor source with Mickey ABI proof.

#### Pinned donor audit, 2026-09-08

- Assignment gate: `base-only` at `32a75d648e8954f7455897fb8f16a8ef5f05df11` for this exact symbol and source path.
- Jet Force Gemini `efd5abb1c79636e297b831f7c2d5bf47eac39c0c` has 12 C implementations and 53 assembly placeholders in `src/track.c`. Its implemented routines do not supply this target's body. The closest masked track-object hit, `func_80014B6C` (Jaccard 0.1044), corresponds by verified TU order to the still-assembly `func_80014EBC_15ABC` in that pinned source. This is diagnostic structural context, not an adopted name, ABI identity, or exact donor match.
- Fresh configured full-TU measurement: 396 target words and 398 candidate words; 304 raw and 304 relocation-masked differences; first +0x48; target frame 0x70, candidate frame 0x70. There are 51 candidate versus 51 target relocation records, with 35 stable identities at matching offsets/types.
- Workbench verdict `structure-mismatch`, playbook `constant-audit`. The existing diagnosis still requires source-shape/lifetime evidence; the donor-only reopen does not authorize substituting a flag, allocator, or permuter mechanism.
- ADR 0018 stop: zero new source attempts for this target. The pinned source disproves the new matched-donor-C hypothesis. No unchanged flag lattice or previously exhausted source family was repeated. The compiled candidate is unchanged and remains behind `NON_MATCHING` and its original `GLOBAL_ASM` fallback; zero new matching bytes are credited.
- Next concrete lever: a published matched counterpart with an authenticated ABI and useful source lifetimes, followed by Mickey-specific field, branch and call proof. The remaining general levers below require separately authorized changed evidence.
- Commands: `tools/wb_compare.sh --summary-json func_8000DFBC`, workbench `diagnose` on the configured full-TU object, the track-object masked skeleton audit, `tools/finalize_plateau.py`, `gmake cleanroom`, and `gmake check-docs`. Source, object, scores, first mismatch and previous handoff are retained under ignored `build/wb/`; no instruction rows are included here.

#### Prior committed plateau evidence (historical)


- source: `src/main/track.c`
- score: 304 differing words
- frame: 0x70
- relocations: 51
- first mismatch: +0x48
- summary: Exact frame and relocation count; 2-word structural drift remains, and flag sweep lacks unique resident ownership metadata.
<!-- plateau-handoff:func_8000DFBC:end -->
