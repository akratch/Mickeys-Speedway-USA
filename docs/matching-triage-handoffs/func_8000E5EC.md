<!-- plateau-handoff:func_8000E5EC:start -->
### `func_8000E5EC` plateau handoff

- source: `src/main/track.c`
- score: 185 differing words
- frame: 0xe8
- relocations: 56
- first mismatch: +0x0
- summary: JFG efd5abb has no matched counterpart C; zero new attempts. Prior mechanisms stay closed. Next: matched donor source with Mickey ABI proof.

#### Pinned donor audit, 2026-09-08

- Assignment gate: `base-only` at `32a75d648e8954f7455897fb8f16a8ef5f05df11` for this exact symbol and source path.
- Jet Force Gemini `efd5abb1c79636e297b831f7c2d5bf47eac39c0c` has 12 C implementations and 53 assembly placeholders in `src/track.c`. Its implemented routines do not supply this target's body. The closest masked track-object hit, `trackUpdateLighting` (Jaccard 0.0479), corresponds by verified TU order to the still-assembly `trackUpdateLighting` in that pinned source. This is diagnostic structural context, not an adopted name, ABI identity, or exact donor match.
- Fresh configured full-TU measurement: 205 target words and 209 candidate words; 185 raw and 185 relocation-masked differences; first +0x0; target frame 0xd8, candidate frame 0xe8. There are 56 candidate versus 56 target relocation records, with 9 stable identities at matching offsets/types.
- Workbench verdict `structure-mismatch`, playbook `constant-audit`. The existing diagnosis still requires source-shape/lifetime evidence; the donor-only reopen does not authorize substituting a flag, allocator, or permuter mechanism.
- ADR 0018 stop: zero new source attempts for this target. The pinned source disproves the new matched-donor-C hypothesis. No unchanged flag lattice or previously exhausted source family was repeated. The compiled candidate is unchanged and remains behind `NON_MATCHING` and its original `GLOBAL_ASM` fallback; zero new matching bytes are credited.
- Next concrete lever: a published matched counterpart with an authenticated ABI and useful source lifetimes, followed by Mickey-specific field, branch and call proof. The remaining general levers below require separately authorized changed evidence.
- Commands: `tools/wb_compare.sh --summary-json func_8000E5EC`, workbench `diagnose` on the configured full-TU object, the track-object masked skeleton audit, `tools/finalize_plateau.py`, `gmake cleanroom`, and `gmake check-docs`. Source, object, scores, first mismatch and previous handoff are retained under ignored `build/wb/`; no instruction rows are included here.

#### Prior committed plateau evidence (historical)


- source: `src/main/track.c`
- score: 185 differing words
- frame: 0xE8
- relocations: 56
- first mismatch: +0x0
- summary: 119 flags flat; fidelity-clean proc 23 has 34 integer decisions but no stack homes or source-attributed webs, so no lexical experiment is justified
<!-- plateau-handoff:func_8000E5EC:end -->
