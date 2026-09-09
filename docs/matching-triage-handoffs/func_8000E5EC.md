<!-- plateau-handoff:func_8000E5EC:start -->
### `func_8000E5EC` plateau handoff

- source: `src/main/track.c`
- score: 185 differing words
- frame: 0xe8
- relocations: 56
- first mismatch: +0x0
- summary: Mickey m2c call, pointer and loop reconstruction does not improve 185 differences. Next: source-attributed stack-home evidence.

#### Mickey m2c structural audit, 2026-09-08

- Fresh assignment gate returned `base-only`. Baseline: 209 candidate / 205 target words, 185 raw/masked differences, first +0x0, frame 0xe8 / 0xd8, 56/56 relocations with 9 exact offset/type/identity sites. Workbench verdict `structure-mismatch`, playbook `constant-audit`; the first differing immediate is frame extent. Visibility flags, buffer stride, module constant and dispatch modes agree with Mickey.
- The fresh Mickey m2c draft recovers the existing list-generation, visibility and rendering call identities. All reverse loops were checked against branch-delay semantics: m2c's printed post-decrement normalization must not replace the current count-correct C literally. The typed track pointer also has a target reload after the list-generation calls, which the raw draft elides.
- Five new structural forms test that explicit track reload/reuse, short-circuited segment-count load, typed record/pointer-list buffers, captured segment byte before nested dispatch, and separate draw countdown. Measurements: 40: 208 words, 187 differences, frame 0xe8; 41: 210 words, 192 differences, frame 0xe8; 42: 210 words, 192 differences, frame 0xe8; 43: 213 words, 210 differences, frame 0xf0; 44: 213 words, 210 differences, frame 0xf8. None improves the retained 185-word baseline or provides an additional unresolved identity. The pointer/call and CFG reconstruction space has been exhausted for this draft; the prior flag and lexical-policy plateau remains closed.
- Retained best is the unchanged guarded baseline with its original assembly fallback. All five candidates and complete full-TU object/score artifacts are retained under ignored `build/wb/tu-track/func_8000E5EC/`. Zero new matching bytes.
- Next concrete lever: source-attributed compiler home/lifetime evidence for the 128-byte list and call-crossing locals. The historical compiler trace lacks stack homes and attributable webs; neither that trace nor this m2c draft justifies an allocator experiment.
- Validation: full-TU comparisons after each attempt, finalizer, `gmake verify`, `gmake cleanroom`, and `gmake check-docs` before the plateau commit.

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
