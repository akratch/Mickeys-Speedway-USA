# Mickey IDO 5.3 learnings

This is the reusable playbook for compiler behavior measured on Mickey. It is
not a function ledger and not a collection of folklore. An entry belongs here
only when a Mickey-exact match or a controlled experiment against Mickey's
pinned IDO 5.3 toolchain proves a principle that can guide another target.

Keep entries generic: state the symptom, mechanism, useful source lever, and
limits. Function-specific addresses, register triples, match percentages,
aligned instruction rows, and attempt history stay in `docs/resident.md`,
`docs/overlays.md`, `docs/matching-triage.md`, or ignored workbench state. ROM
bytes and disassembly never belong here.

## Diagnostic order

1. Prove the boundary, ABI, TU, configured flags, relocation surface, and
   untouched baseline before changing source.
2. Run the flag lattice before source permutation. Optimization level, ISA,
   debug mode, loop unrolling, the R4300 multiply scheduler, and the forced
   `uopt`-at-`-O1` path are measured project modes, not cosmetic switches.
3. Classify the first mismatch with the full-TU object and decomp-workbench.
   Use phase replay to decide whether C structure, `uopt` allocation, `ugen`
   allocation, or `as1` scheduling owns the residual.
4. Apply one source lever that matches the diagnosis. A higher score without a
   mechanism is not evidence and should not displace a better-understood base.
5. Rebuild through the configured project pipeline. Scratch/permuter scores
   are search hints; only exact owned bytes, relocations, linked placement, and
   the ROM comparison establish a match.

## Proven reusable behavior

### Flags and compiler phases

- The driver does not run `uopt` below `-O2`, but Mickey's
  `setglobalintmask` requires `uopt` forced into an otherwise `-O1` pipeline.
  `tools/ido-phases.py` is therefore a real flag-lattice member, not a debug
  convenience. Evidence: `docs/modules.md` section 6's flag census and
  `docs/flag-sweep.md`.
- A downstream scheduling diff can originate in an upstream register choice.
  Diagnose and replay the producing phase before attempting statement-order
  churn; forcing a target color is causal diagnosis only and is never a
  promotable build step. Evidence: `docs/breakthrough-campaign.md` Phase 2.
- Per-TU flags stay local. One exact function can establish a candidate flag
  group, but changing a shared group requires evidence and an impact review
  across every consumer. Evidence: ADR 0007 and `docs/flag-sweep.md`.

### Allocation and source shape

- Declaration order can determine stack-home order for call-crossing locals.
  When the operation sequence is exact but spill offsets differ, reorder
  semantically independent declarations before inventing extra state.
  Address-taken local arrays and structs can retain the same
  declaration-relative placement even when preceding scalar and cursor locals
  are colored into registers, so moving an existing aggregate past those
  declarations can change only its frame offset. Evidence: the exact resident
  collision-handler cohort in `docs/resident.md`, plus the exact tile-command
  builder and spawn-entry initializer in `docs/overlays.md`.
- Expression association, signedness, and width affect IDO's internal values
  even when C semantics appear equivalent. Preserve the ABI and inferred data
  model; use typed rewrites as a diagnosed lever, not an arbitrary score nudge.
  Evidence: the exact animation, save, and collision cohorts in
  `docs/resident.md`.
- A merged TU or a shared placeholder prototype can change caller code while a
  standalone callee looks exact. Treat symbol binding, visible prototypes, and
  TU ownership as part of the compiler input. Evidence: the merged-TU blocker
  class in `docs/matching-triage.md`.
- Under O32, a single 64-bit integer argument occupies an aligned `a0`/`a1`
  pair. IDO materializes the two halves of a constant zero independently, so a
  target with two adjacent argument-register zero loads can indicate one
  `u64`/`OSTime` parameter rather than two scalar parameters. Use relocation or
  callee metadata to prove that ABI before changing a prototype; equal register
  contents alone are insufficient. Evidence: Overlay 18's exact startup loader
  in `docs/overlays.md`.
- An algebraically zero integer read can be a caller-saved coloring lever. On
  a path that already returns zero, spelling the result as `value * 0` kept the
  value's web live through IDO's allocation decision while still folding to a
  zero return instruction; a two-web forced-color probe identified the cause,
  and the same stock source then matched without a force. This is specific to
  the measured `-O2 -mips2` basin: use it only when zero is semantically
  required, and reject it if instruction count, relocations, or linked bytes
  move. Evidence: Overlay 1's exact value-cache manager in `docs/overlays.md`.
- A source copy can decouple load order from FP coloring when two equal-cost
  webs tie. Load the first value through the local that must receive the first
  color, copy it to its lasting local, then overwrite the first local with the
  second value; IDO can coalesce the copy away while preserving the opposite
  load and color orders. This applies only when the copy and overwrite are
  semantically exact and the stock object emits no move or spill; reject the
  shape unless instruction count, relocations, linked bytes, and the full ROM
  remain exact. Evidence: Overlay 8's exact scale-output body in
  `docs/overlays.md`.

### Search fidelity and false floors

- A permuter zero is a hypothesis until the project pipeline verifies it.
  Function isolation must reproduce the TU's flags and metadata-only object
  steps, and stack-slot differences must be scored. Evidence:
  `docs/matching-triage.md` section "Permuter scratch fidelity."
- Do not call a register/scheduler residual unreachable until the correct ISA,
  faithful scratch build, bounded permuter, and workbench route have all been
  tried. Several Mickey functions first classified as hard scheduler walls
  became exact after those preconditions were repaired. Evidence:
  `docs/matching-triage.md`.
- Permuter output is a source-spelling probe, not a style authority. Preserve
  an exact but inert spelling when necessary, disclose it in source, and put
  naturalization in `docs/cleanup-queue.md`; never keep a semantic mutation
  merely because it scores better. Evidence: ADR 0007 and the cleanup queue.

## Adding a learning

Add a short entry only after the result is reproducible. Cite the durable
Mickey evidence, state where the lever does *not* apply, and prefer updating
the upstream decomp-workbench field guide when the behavior is toolchain-wide.
A one-function curiosity belongs in that function's handoff, not here.
