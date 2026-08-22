# Mickey's Speedway USA maximum-throughput decomp policy

This repository is an active matching-decomp campaign. Its mandatory operating
mode is maximum safe parallel throughput. Keep every available agent slot
continuously supplied with independent work, make compile-and-compare iteration
part of normal function ownership, bank exact matches quickly, and continue
until the campaign objective is complete or a concrete external blocker is
documented. An idle collaboration slot while executable campaign work is queued
is an orchestration defect and must be corrected immediately.

Read `docs/LOCAL_DECOMP_AGENT.md` before using the local model. Read
`docs/CONTRIBUTING.md` and `docs/CLEANROOM.md` before changing canonical source.

## Local-model boundary

- `n64dllm-v1` is a bounded, untrusted C-candidate specialist. It is not the
  campaign coordinator, an autonomous worker, or a collaboration slot. Invoke
  it only through a coordinator-authorized solver job that follows
  `docs/LOCAL_DECOMP_AGENT.md`.
- The campaign coordinator selects and approves the job. The solver owns its
  scratch paths, model requests, compilation, comparison, budgets, and attempt
  ledger. The model receives bounded evidence and returns one C function; it
  owns no tool or acceptance decision.
- Do not grant the local model shell access, repository discovery, canonical
  writes, git or network access, goals, todos, skills, subagents, or authority to
  select or expand work. Model output is an untrusted hypothesis, never evidence
  of a score, match, safety verdict, or integration readiness.

## Full-throttle orchestration

- Saturate every collaboration slot exposed by the runtime whenever the ready
  queue contains independent work. On the current four-slot runtime this means
  one productive coordinating primary plus three active workers. The runtime's
  actual slot count is the only agent ceiling; never impose a smaller ceiling,
  serial-worker policy, batch-size limit, or arbitrary caution quota.
- Collaboration-slot saturation applies to capable collaboration agents, not
  local-model inference. Do not count or recursively dispatch `n64dllm-v1` as a
  worker.
- The primary is both coordinator and contributor. Coordination is not a reason
  for the primary to idle: after dispatching all worker slots, it should perform
  integration, accounting, target preparation, or its own disjoint matching
  basin while monitoring worker handoffs.
- The workstation's two-job ceiling applies to simultaneous compiler jobs, not
  to agent count, candidate ownership, source analysis, object inspection, or
  orchestration. Fill every collaboration slot even while compilation itself is
  throttled. Run permitted compiles at low priority and with no more than two
  compiler jobs active across the team. Route standalone compiles and build
  commands through `tools/with_compile_token.sh`; its two atomic shared tokens
  convert contention into a short queue without idling the analysis lanes.
- Never leave a slot idle while queued Epoch, closure, source-recovery,
  relocation, compiler-basin, integration, audit, or candidate-diversification
  work exists. If the next canonical edit is serialized, workers should keep
  pursuing workbench matches or independent proofs instead of waiting.
- The primary agent is the campaign coordinator. At the start of a campaign
  turn, inspect live agent state and the authoritative worktree, maintain a
  concise plan, and assign disjoint work to every free slot immediately.
- Recycle workers immediately. When an agent finishes, inspect the handoff and
  assign its next bounded unit in the same coordinator turn. Refill the slot
  before beginning optional reporting or broad analysis. Do not wait for a batch
  barrier, a status report, or every other lane to finish.
- Agents may spawn their own subagents when a free slot exists and the child can
  own a concrete independent unit. Avoid delegation for a task whose output is
  needed before any useful parallel work can begin.
- Prefer parallel batches of independent functions, overlays, or proof stages.
  Serialize only real dependencies such as shared canonical-file edits, final
  link order, or a caller whose ABI genuinely depends on an unresolved callee.
- Keep a ready queue at least twice as deep as the available worker-slot count.
  Each live lane should have a named successor target. Rank work by expected
  exact bytes per unit time, closure value, dependency value, and confidence of
  reaching a compiler basin.
- Each assignment must be executable, not advisory: give the worker exclusive
  paths, the exact target identity and byte boundary, the expected toolchain and
  comparison commands, acceptance gates, and authority to iterate through
  compilation and object inspection without returning for routine permission.
  This routine worker authority does not pass through to `n64dllm-v1`.
- Never stop merely because a candidate is difficult, a scalar score stalls, or
  several hypotheses fail. Continue with evidence-driven experiments, delegate
  a second independent approach, or switch one lane to another ready target
  while preserving the blocker and best artifact.

## Mandatory dispatch loop

The primary repeats this loop throughout every active campaign turn:

1. Inspect live agents, newly finished handoffs, the dirty worktree, and current
   authoritative accounting.
2. Refill every free slot from the ranked ready queue with a disjoint target and
   exclusive workbench ownership.
3. Give each worker authority and commands to inspect, compile, compare, iterate,
   and produce either an exact promotion packet or a reproducible plateau.
4. Coordinate the two shared compiler-job tokens with
   `tools/with_compile_token.sh`; agents waiting for a token keep doing source,
   relocation, CFG, scoring, or experiment-design work.
5. Integrate exact packets one ownership unit at a time while workers continue;
   do not create a global pause for canonical banking.
6. Update accounting and gates after authoritative changes, harvest all newly
   completed lanes, immediately recycle them, and repeat.

Status collection, documentation, and planning are supporting work, not batch
barriers. Unless no executable work remains, every update should leave the pool
fully saturated and every live lane advancing a concrete target.

## Non-interference and ownership

- The worktree may contain concurrent human and agent experiments. Never clean,
  reset, checkout, stash, delete, stage, commit, push, or broadly reformat them.
- Give every worker an exclusive output directory or exact canonical-file set.
  All other agent-owned paths are read-only unless ownership is explicitly
  transferred by the coordinator.
- Workbench candidates, objects, scores, and handoffs belong under ignored
  `build/wb/` directories. Canonical `src/`, config, Makefile, symbols, atlas,
  and documentation changes are serialized through the coordinator.
- Before editing a shared file, inspect its current contents and dirty state.
  Preserve unrelated changes and merge concurrent packet results deliberately.
- Use `apply_patch` for hand-authored file changes. Generated compiler, linker,
  extraction, and comparison artifacts may be produced by their configured
  tools inside the worker's exclusive directory.

## Validation is part of the job

- Source reconstruction is the start of a decomp task, not its completion.
  Unless a task explicitly requests source-only analysis, the function owner
  continues through configured compilation, exact byte comparison, relocation
  inspection, source-shape iteration, and a clear per-function verdict.
- Normal in-scope validation does not require repeated permission prompts. The
  campaign authorizes low-impact command-line compilation, assembly, linking,
  ELF inspection, section extraction, object/byte comparison, hashes, donor and
  provenance scans, atlas/progress checks, and ROM rebuilds needed to prove the
  assigned decomp work.
- For local-model candidates, that authority belongs to the solver. The local
  model never constructs or selects shell or build commands. The coordinator
  passes the solver an exact configured compiler argv beginning with
  `tools/with_compile_token.sh`, and uses the candidate's exclusive ignored
  scratch directory.
- Run independent compile/compare basins concurrently within the workstation's
  two-low-priority-compiler-job limit. Assembly, section extraction,
  disassembly, relocation inspection, hashing, and byte comparison are ordinary
  worker operations, not scarce coordinator-only work. Workers that cannot take
  a compiler slot should continue source analysis, experiment design, target
  census, or handoff preparation instead of idling.
- Never execute a generated game binary or ROM. Never launch an emulator,
  simulator, browser, GUI application, device workflow, watch mode, or unrelated
  test suite as decomp validation.
- Use the repository's vendored IDO and MIPS binutils, configured Makefile
  rules, `diff.sh`, and exact target objects. A standalone workbench compile is
  allowed when it reproduces the relevant configured compiler flags and the
  handoff records the distinction; final acceptance still requires the
  configured object and linked repository path.
- Treat synthetic-address and zero-valued proxy links as diagnostic evidence
  only. Every GO packet and canonical promotion must also extract the exact
  owned range from the linked output and compare it byte-for-byte, with a
  recorded hash, against the same ROM offsets in the canonical baserom. A proxy
  match alone never proves relocation addends or earns campaign credit.
- Preserve every meaningful attempt: source, object, owned text, score, first
  mismatch, relocation table, and rationale. Do not overwrite the best artifact
  without retaining it.

## Clean-room and target identity

- Use Mickey's own headers, symbols, source, assembly, objects, relocation
  metadata, and ROM bytes as authority. Do not import source, names, or layouts
  from unauthorized repositories or other games.
- Follow the `PROVENANCE` disclosure rules in `docs/CLEANROOM.md` for any
  permitted published retail-derived decomp material. If provenance is
  uncertain, quarantine the candidate and continue with Mickey-only evidence.
- Do not add ROMs, extracted assets, disassembly, instruction listings,
  hexdumps, machine-word arrays, or workbench ledgers to tracked source files.
  Keep sensitive/generated evidence in existing ignored locations.
- Identify overlay functions by `(overlay, section, offset)` and exact ROM/text
  ownership. Synthetic VMAs are not unique identities across overlays.
- Separate executable bytes, target padding, compiler section alignment, exact
  neighboring islands, and data. Only unmatched executable bytes can become new
  campaign credit.

## Matching workflow

1. Confirm exact symbol boundaries, translation-unit ownership, ABI/callers,
   current pragma/body, per-file flags, padding, and relocation surface.
2. Build an independent typed semantic candidate from Mickey-local evidence.
   Use `./mips_to_c.sh <symbol>` only as a draft aid, never as authority.
   Treat local-model output as an untrusted hypothesis. Before compilation,
   reject malformed output, the wrong function, inline assembly, preprocessor
   directives, or a clear violation of fixed source facts. Review semantic
   invariants before promotion; frame, object size, instruction shape, and
   relocations are compiled outcomes.
3. Compile the unmodified baseline immediately with the measured/configured
   toolchain. Record function size, exact words, first mismatch, frame/register
   shape, call sites, and relocations.
4. Classify mismatch topology before editing: boundary/alignment, ABI/type,
   CFG/branch-likely, expression association, declaration/lifetime/register
   allocation, loop spelling, instruction-set/flag mode, or relocation binding.
5. Iterate with coherent hypotheses. Preserve semantics, widths, signedness,
   NaN/overflow behavior, delay-slot effects, uninitialized invariants, call
   order, and unchecked hazards. Do not improve a score by inventing guards,
   merging unproved identities, ignoring words, or patching candidate bytes.
6. Require exact owned bytes and exact relocation count/type/offset/identity.
   Exact size, a high word score, or semantic agreement alone is nonexact.
7. Promote a natural exact workbench result into the canonical split in a
   reversible, target-local change. Rebuild its configured object and re-prove
   the exact function, boundary, relocation table, linked overlay, and full ROM.
8. Run donor/provenance, atlas/YAML, derived-number, progress, clean-room, and
   worktree-hygiene gates before banking bytes or claiming a closure.

## Canonical-change discipline

- Do not change global toolchain flags, post-processing, shared headers, common
  structures, symbol policy, YAML layout, or data placement merely to improve
  one candidate. Such changes require target evidence and impact review across
  every consumer, but they are not categorically forbidden when the evidence
  proves they are canonical.
- Integrate one exact ownership unit at a time. Preserve neighboring assembly
  and padding, link after each unit, and keep rollback straightforward.
- A shared semantic body does not imply a shared object verdict. Compile,
  relocate, link, and credit each overlay/function independently.
- Never count source-only surface, padding, already matched islands, substitute
  routes, or duplicate packets as verified progress.

## Campaign reporting

- Lead with measurable outcomes: exact words/bytes, boundaries, relocations,
  linked status, closures, and remaining hard deficit.
- Distinguish source-reviewed, compiled, standalone object-function exact,
  configured-object exact, canonical staged, linked exact, ROM exact, and
  banked states. Never call a partial score a match.
- Local-model prose never establishes any of those states. Only
  controller-recorded artifacts and authoritative comparisons may do so.
- Every worker handoff must state files changed, commands run, best score and
  first mismatch, unresolved identities/blockers, and the next concrete action.
- A source-only reconstruction is an intermediate artifact unless the assignment
  explicitly says otherwise. Do not report a function lane complete until its
  owner has compiled it, compared owned bytes and relocations, and either proved
  exactness or recorded a reproducible best candidate and genuine blocker.
- Keep the central campaign portfolio, validation ladder, execution route, and
  completion audit synchronized whenever authoritative accounting changes.
