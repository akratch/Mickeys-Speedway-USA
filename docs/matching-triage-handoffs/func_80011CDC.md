<!-- plateau-handoff:func_80011CDC:start -->
### `func_80011CDC` plateau handoff

- source: `src/main/track.c`
- score: 327 differing words
- frame: 0xd0
- relocations: 11
- first mismatch: +0x0
- summary: Mickey m2c reproduces existing edge/endpoint tests; no new structural identity. Next: source-proved texture-global and counter lifetimes.

#### Mickey m2c structural audit, 2026-09-09

- The assignment gate returned `base-only` for the authorized Mickey-only reconstruction mechanism. A fresh configured full-TU baseline and workbench diagnosis retain 344 candidate versus 342 target words, 327 differing words, first +0x0, candidate frame 0xd0 versus target 0xc0, and 11 candidate versus 15 target relocation records with 2 exact offset/type/identity records. Verdict `structure-mismatch`, playbook `constant-audit`.
- The m2c draft reproduces the existing edge test followed by the two conditional endpoint tests, including the minimum-distance checks, normal/position output order, metadata lookup and texture-byte access. The record stride is 0x2c and the direction begins at record offset 0x18. Calls resolve to the existing `func_80012234` and `func_80012574` implementations. No missing branch, constant, call identity or field interpretation was exposed.
- The draft's extra copies of the return flag around calls describe spills in the generated program; they are not evidence for new source locals. The named plane differences have already been tested in the committed prior plateau. No allocation rewrite, repeated flag lattice, or permuter trial was substituted for the authorized mechanism.
- Stop evidence: zero new source attempts. The recovered data flow already exists in the guarded candidate; the remaining texture-global hoist, loop-counter spill and frame excess are the previously documented source-lifetime problem. Source and configured object are preserved with the fresh baseline under ignored `build/wb/tu-track/func_80011CDC/`.
- Next concrete lever: source-attributed lifetimes explaining the unhoisted texture global and loop counter, authenticated against Mickey's ABI, under a new allocation/source-lifetime authorization. Preserve the existing plane-difference evidence.
- Validation: configured full-TU `wb_compare.sh`, workbench `diagnose` and `guide constant-audit`, `tools/finalize_plateau.py`, and `gmake verify cleanroom check-docs`. The original `NON_MATCHING`/`GLOBAL_ASM` guard remains; zero matching bytes are claimed.

#### Pinned donor audit, 2026-09-08

- Assignment gate: `base-only` at `32a75d648e8954f7455897fb8f16a8ef5f05df11` for this exact symbol and source path.
- Jet Force Gemini `efd5abb1c79636e297b831f7c2d5bf47eac39c0c` has 12 C implementations and 53 assembly placeholders in `src/track.c`. Its implemented routines do not supply this target's body. The closest masked track-object hit, `func_800175A0` (Jaccard 0.0800), corresponds by verified TU order to the still-assembly `func_80017794_18394` in that pinned source. This is diagnostic structural context, not an adopted name, ABI identity, or exact donor match.
- Fresh configured full-TU measurement: 342 target words and 344 candidate words; 327 raw and 327 relocation-masked differences; first +0x0; target frame 0xc0, candidate frame 0xd0. There are 11 candidate versus 15 target relocation records, with 2 stable identities at matching offsets/types.
- Workbench verdict `structure-mismatch`, playbook `constant-audit`. The existing diagnosis still requires source-shape/lifetime evidence; the donor-only reopen does not authorize substituting a flag, allocator, or permuter mechanism.
- ADR 0018 stop: zero new source attempts for this target. The pinned source disproves the new matched-donor-C hypothesis. No unchanged flag lattice or previously exhausted source family was repeated. The compiled candidate is unchanged and remains behind `NON_MATCHING` and its original `GLOBAL_ASM` fallback; zero new matching bytes are credited.
- Next concrete lever: a published matched counterpart with an authenticated ABI and useful source lifetimes, followed by Mickey-specific field, branch and call proof. The remaining general levers below require separately authorized changed evidence.
- Commands: `tools/wb_compare.sh --summary-json func_80011CDC`, workbench `diagnose` on the configured full-TU object, the track-object masked skeleton audit, `tools/finalize_plateau.py`, `gmake cleanroom`, and `gmake check-docs`. Source, object, scores, first mismatch and previous handoff are retained under ignored `build/wb/`; no instruction rows are included here.

#### Prior committed plateau evidence (historical)


- source: `src/main/track.c`
- score: 15/342 words
- frame: 0xD0
- relocations: 11
- first mismatch: +0x0
- summary: Plane-difference locals improve 339 to 327 diffs and recover target saves; texture-global hoisting still spills the counter (344 vs 342 words, 11 vs 15 relocs).
<!-- plateau-handoff:func_80011CDC:end -->
