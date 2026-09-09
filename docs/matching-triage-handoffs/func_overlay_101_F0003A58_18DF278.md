<!-- plateau-handoff:func_overlay_101_F0003A58_18DF278:start -->
### `func_overlay_101_F0003A58_18DF278` plateau handoff

- source: `src/overlays/o101/func_overlay_101_F0003A58_18DF278.c`
- score: 1421/1461 words
- frame: 0x50
- relocations: 201
- first mismatch: +0x34
- summary: Calls and reset input recovered; 1463/1461 words, 201/199 relocations. Five structural trials stalled; next: prove node-pool base liveness across loader.

Reconstruction packet on base `0d21245897f0469ec630a49a07024b5e51ad04b4`:

- Ownership is overlay 101 `.text` `+0x3A58..+0x512C`, ROM
  `0x18DF278..0x18E094C`: 5,844 executable bytes, 1,461 words. The candidate
  contains 1,463 words (5,852 executable bytes), an eight-byte overrun;
  compiler section alignment is not part of that deficit.
- Fresh configured full-TU baseline and restored best both measure 1,421
  raw and relocation-masked differing words, first `+0x34`, frame `0x50`
  on both sides. Workbench verdict is `structure-mismatch`, with 1,128
  positional opcode differences and normalized distance 1,888. These are
  nonexact object comparisons, not linked or ROM-exact C.
- The shipped runtime table owns 199 relocation records: 27 calls and
  86 address pairs. The candidate has 201: 27 calls and 87 address pairs.
  The ordinary extracted fallback object exposes only 161 records because
  several relocated addresses appear as numeric operands. An ignored,
  relocation-annotated target restores all 199 records; ordinary assembly
  and a diagnostic link reproduce all 5,844 original owned bytes. That
  receipt validates target preparation, not a candidate promotion.
- After a canonical build, authenticated workbench summary generation
  fails closed with `candidate function escapes TU ownership`. Direct
  configured-object comparison still measures the nonexact candidate.
  Exact candidate relocation identities are therefore unproved; no numeric
  checkpoint substitutes invented identity counts for this refusal.

Call/global/type evidence banked in `5ede91fe` (tier B: decoded runtime
relocations and retail callee/caller analysis):

- One sprite load calls resident `func_800355A0`, with asset ID and flags.
- Nine screen loads call resident `func_80036DD0`, with one screen index.
- Sixteen length queries call overlay 101 `overlay101ByteLength` at
  `+0xCEA8`; its matched body returns `s32`, not the previous `s8` guess.
- The final call is overlay 101 `overlay101Reset` at `+0x1BB4`, taking a
  pointer. Its argument is initialized-data offset `+0x340` (module offset
  `+0xD220`), distinct from the BSS sprite-node pool at BSS `+0x340`
  (module offset `+0x12F80`). The retained source uses a separate extern.
- Root, input table, order array, and the three node pools were identified
  separately. All repeated accesses to each counter in this function share
  the same runtime identity; the adjacent presentation-builder A/B aliases
  are not evidence for splitting this target's counters.
- Node strides and fields remain 20, 24 and 32 bytes. Typed m2c inspection
  corroborates the eleven groups, one sprite node, nine screen nodes,
  sixteen text nodes, coordinates, colors, unsigned byte conversion,
  chain links, and call order. No donor body was adopted. The nearest
  skeleton scores were weak (best 0.056), not source-adoption evidence.

Preparation and attempts (counts are differing words, not matched words):

- The actual preprocessed baseline passed its self-context comparison.
  `generate_ctx.sh` succeeded, but the all-header m2c context failed on
  an unrelated unsized array. A minimal full-TU type context and the
  independently proved relocation annotation produced the typed draft.
  Every draft and generated diagnostic stays ignored.
- Attempt 1 recovered the four callees, length return type and reset input:
  1,463 words, 1,421 differences, frame `0x50`, 201 relocations. This is
  the retained best and the last new runtime-identity evidence.
- Attempt 2 used a typed 32-byte pool: instruction-flat at the same score.
- Attempt 3 separated the sprite index: 1,464 words, 1,442 differences.
- Attempt 4 used direct count expressions: 1,464 words, 1,436 masked and
  1,437 raw differences, still 201 relocations.
- Attempt 5 retained explicit pool/count pointers: 1,464 words, 1,444
  differences, 205 relocations. It did not isolate the target live range.
- Attempt 6 reconstructed shared old-head snapshots and head publication:
  1,510 words, 1,480 differences, 237 relocations.
- Attempt 7 scoped each node construction: 1,364 words, 1,455 differences,
  97 relocations. It removes many address loads that the target retains.
- Attempt 8 compiled the complete typed draft only as an ignored diagnostic:
  1,396 words, frame `0x40`, 1,458 differences against the annotated target.
  It folds opacity multiplies and introduces float copies.
- Attempt 9 normalized the draft's unsigned conversions and integer-derived
  opacity scale: 1,394 words, frame `0x50`, 1,457 differences against the
  annotated target. This remains an ignored diagnostic, not adopted C.
- Attempt 10 reconstructed the root dimension/mode publication order in
  the compact candidate: 1,463 words, 1,422 differences. The original best
  was restored and recompiled afterward.

Stopping evidence: attempts 6 through 10 are the five-trial stall window.
They produced neither a better best residual nor another proved identity,
and did not conclusively eliminate a causal explanation for the extra
address pair. A changed schedule or failed spelling alone is not a newly
eliminated causal hypothesis. There was no flag lattice, permuter, instrumented
compiler experiment, or post-compile instruction modification in this packet.

Workbench routing: `structure-mismatch`, `structure-buckets`; diagnosis's
`next:` says to fix constants first, structure second, register classes last.
The relevant structure levers were constant audit, context parity and
liveness near a call. Existing flag/line-assignment plateaus remain closed.
The baseline's instruction census equals the target's except for one extra
address pair; scoped or unshared reconstructions substantially disturb that
surface. This is evidence against missing logical operations, not a proof
that every possible C spelling has been exhausted.

Next concrete lever: a separately authorized allocation investigation of
why the BSS 32-byte pool base is rematerialized after `func_800355A0`, whereas
the target retains it across that call. Establish a compiler-liveness receipt
before selecting another source lever. Do not repeat the structural forms
above or equate a synthetic-address link with a promotion. Any eventual
candidate must fit ownership, prove all relocation identities and offsets,
and pass the owned linked-range comparison and full-ROM gate.

Validation: `tools/finalize_plateau.py` records the guarded candidate;
`gmake verify` reproduces the expected US ROM hash with GLOBAL_ASM canonical;
`gmake check-nonmatching-builds` passes all 243 candidate-bearing TUs.
`gmake cleanroom`, `gmake check-docs` and the active commit hook pass.
No matched bytes, symbol promotion or atlas credit is claimed. Source/object,
score, mismatch and rationale artifacts for all attempts are retained under
ignored `build/reconstruction/`; typed drafts are under ignored `m2cfiles/`.
<!-- plateau-handoff:func_overlay_101_F0003A58_18DF278:end -->
