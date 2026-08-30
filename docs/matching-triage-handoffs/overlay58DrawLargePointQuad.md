<!-- plateau-handoff:overlay58DrawLargePointQuad:start -->
### `overlay58DrawLargePointQuad` plateau handoff

- source: `src/overlays/o058/overlay58DrawLargePointQuad.c`
- score: 34/104 words
- frame: 0x18
- relocations: 11
- first mismatch: +0x30
- summary: Sibling reproof has the same pool/temp signature; dead physical-address/color reuse is byte-flat. Prior flags and bounded permutation are exhausted.
- base commit: `f8f3ec51a298dd0eddd0574a4313adbb1e39de9b`
- verdict: bounded plateau; zero exact bytes gained
- boundary: overlay 58 text `+0x50C8..+0x5268`, exactly 416 owned bytes with no target padding
- ABI: `void (s32, s32, s32)`; sole local caller at `+0x126C`
- configured recipe: `-O2 -mips2 -32` with `NON_MATCHING` selecting the guarded C body
- V0: exact 104-word extent and `0x18` frame on both sides; 34 positional words agree and 70 differ
- diagnosis: allocation-only residual with 70 register sites, no opcode or schedule drift, first pool divergence at slot 6, and first temporary-lane divergence at slot 0
- mechanism limit: nine of twenty substitution webs require ring-only temporary registers, so forced global coloring cannot close the full residual; web existence and temporary-queue phase both differ
- sibling: fresh `overlay58DrawPointQuad` preflight reproduces the same 104-word extent, frame, 34/104 score, first mismatch, pool signature, and temporary signature
- relocation proof: target and candidate each carry eleven records, but only ten offsets/types align; the payload high-half occurs four bytes late in C
- relocation identities: runtime authenticates the resident setup call plus the render-state, display-list, vertex-cursor, and payload roles; static fallback proof resolves only three candidate identities and zero complete aligned identities
- flags: the retained 119-row lattice is already nonexact and was not repeated
- donor check: current DKR/BK masked-skeleton scan is negative; the best size-compatible similarity is 0.062 and is not a credible body donor
- fresh hypothesis: reusing the dead physical-address scalar as the later color value is byte-identical to V0 and does not change pool position
- prior exhausted levers: qualifiers, declaration and statement order, pointer/array forms, signedness, literal forms, named color lifetimes, explicit cursor references, state wrappers, and split increments
- permutation: the retained 40-minute bounded run found only a synthetic do/while coalescing form; it is rejected and was not repeated
- validation: `tools/function_preflight.py overlay58DrawLargePointQuad --json`; `tools/wb_compare.sh --diagnose --no-build overlay58DrawLargePointQuad`; `tools/function_preflight.py overlay58DrawPointQuad --json`; `tools/skeleton_scan.py similar --target 58:+0x50C8`
- next lever: obtain new fidelity-clean allocator evidence that explains both the long-lived cursor-address pool position and the temporary-queue phase, then test only its source-authentic web-existence form; do not repeat flags, generic permutation, or forced colors
<!-- plateau-handoff:overlay58DrawLargePointQuad:end -->
