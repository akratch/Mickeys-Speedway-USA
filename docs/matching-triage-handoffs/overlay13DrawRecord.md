<!-- plateau-handoff:overlay13DrawRecord:start -->
### `overlay13DrawRecord` plateau handoff

- source: `src/overlays/o013/overlay13DrawRecord.c`
- score: 100 differing words
- frame: 0x38
- relocations: 18
- first mismatch: +0x30
- summary: local declaration order fixes the frame; remaining structure/register web and unresolved relocation identities need new evidence
- assignment base: `6ce89d6b`
- owned range: overlay 13 `+0x580..+0x874`, 756 bytes / 189 words
- baseline: 108 differing positional words, `0x40` frame, first mismatch `+0x0`
- retained score: 100 differing positional words / 89 of 189 raw words exact; the masked 97-word residual is only a floor because 19 relocation sites name different symbols
- frame proof: exact `0x38`; declaring `savedScale` before the two pointer locals removes the candidate-only eight-byte non-save area
- relocation proof: target object has 10 text relocations and the candidate has 18; workbench reports 13 metadata mismatches, and preflight fails closed on eight unresolved candidate static identities
- mismatch proof: first mismatch is `+0x30` in the unlinked object comparison
- attempts: all 119 flag combinations and seven coherent declaration, pointer-type, command-store, constant-materialization, and expression-tree forms; no generic permuter was run
- residual: 28 opcode, 69 register, and six relocation-layout diff sites; workbench still classifies the function as `structure-mismatch`, so it is not an allocator-only near miss
- next action: reopen only with new evidence for the overlay-relative relocation identities or the render-state lifetime/register web; do not repeat the completed flag lattice or these source forms
- JFG: `func_overlay_1_00100678_1ED4198` in `src/overlays/o1/overlay_1.c` is the 0.245 masked 4-gram lead and has the same state split, color setup, vertex/triangle emission, optional phase-gated draw, and cleanup sequence. It remains assembly-only and is 768 bytes, with different render-state/data offsets. If Mickey reaches exact C, this body would be useful as the starting source at JFG's existing pragma for that symbol; at this plateau it is not release-grade and should not enter the public JFG ledger.
<!-- plateau-handoff:overlay13DrawRecord:end -->
