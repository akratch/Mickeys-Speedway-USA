# Mickey's Speedway USA decomp: agent operating policy

This repository is an active matching-decomp campaign. Decisions governing
how it's worked are recorded in `docs/adr/` (start at `docs/adr/README.md`);
this file is the operating summary. Where this file and an ADR disagree, the
ADR is authoritative: file a new ADR to change policy, don't just edit this
file out from under it.

Read `docs/CONTRIBUTING.md` and `docs/CLEANROOM.md` before changing
canonical source. `docs/LOCAL_DECOMP_AGENT.md` is historical: the local
`n64dllm-v1` lane was evaluated and retired (0 improvements across 5
targets; see `docs/adr/0009-model-routing-and-agent-operation.md`). Don't
route matching work to it.

## The lane model

As many workers as there is independent work; there is no fixed slot count
to saturate. Each worker operates in its own isolated worktree, created with
`tools/new_lane.sh <name>`, on branch `lane/<name>`. Lanes give every worker
its own `build/` and `asm/`, so workers never contend for the same objects
and never need a job ceiling to avoid stepping on each other
(`docs/adr/0004-build-parallelism.md`).

- Build with `gmake -j$(sysctl -n hw.ncpu)` (or the machine's real core count) inside
  your lane. There is no campaign-mandated compiler-job ceiling.
- Never touch another lane's worktree, branch, or in-flight files. If you
  need something another lane owns, ask for a handoff rather than editing
  it directly.
- The primary/coordinating agent's job is to keep the ready queue populated
  and assign disjoint targets to free lanes, not to police a slot count.
- Full two-phase `gmake verify` run against a SHARED worktree (not a lane) goes through `tools/with_verify_lock.sh`, now in `campaign/unchain`. A lane's own `gmake verify` needs no lock: each lane already has its own `build/`.

## Commit discipline

Commit on your own lane branch as work lands: small, function-sized
commits, not an end-of-session dump (`docs/adr/0010-commit-discipline.md`).
A commit is one exact function plus its symbol-table line and atlas row, or
an equivalent coherent unit.

- Hooks are always on. **Never pass `--no-verify`.** If a hook fails, fix
  the underlying issue and make a new commit; don't bypass it.
- Run `gmake check-docs` and `gmake cleanroom` before every commit (no build
  needed); run `gmake verify` before anything you believe changes matching
  status. See `CLAUDE.md`'s "Before every commit" section for the full list.
- Lane branches integrate through the existing campaign/unchain flow.
  Nothing should exist only in a working directory between integration
  points, which is how a month of work once went unswept by the clean-room
  gates and nearly got lost to a bad `checkout`.

## Validation is part of the job

- Source reconstruction is the start of a decomp task, not its completion.
  A function isn't done until it's compiled, byte-compared, and given a
  clear verdict: matched (ADR 0001), `NON_MATCHING`, `NON_EQUIVALENT`, or a
  recorded plateau after the attempt cap (ADR 0009).
- Normal in-scope validation doesn't need repeated permission prompts:
  compilation, assembly, linking, ELF inspection, section extraction,
  object/byte comparison, hashing, donor/provenance scans, atlas/progress
  checks, and ROM rebuilds are all ordinary parts of owning a function.
- Never execute a generated game binary or ROM, launch an emulator, or use
  a GUI/device workflow as decomp validation.
- Matched means: `tools/ido/cc`'s untouched output for the C in the tree,
  linked at the real offset, is byte-identical to the ROM
  (`docs/adr/0001-matching-standard.md`). No instruction word may be edited
  after compilation to reach a match. See
  `docs/adr/0002-no-post-compile-instruction-editing.md` for exactly what
  post-compile step is and isn't permitted. A function only reachable via a
  prohibited step is written as `#ifdef NON_MATCHING` C over
  `#pragma GLOBAL_ASM`, not silently counted as matched.
- Require exact owned bytes and exact relocation count/type/offset/identity.
  Exact size, a high objdiff score, or semantic agreement alone is
  non-exact.
- Separate executable bytes from target padding, compiler section alignment,
  and already-matched islands and data. Only unmatched executable bytes ever
  become new credit; source-only surface and padding never do.
- Synthetic-address and zero-valued proxy links are diagnostic only. Every
  promotion extracts the owned range from the linked output and byte-compares
  it against the same offsets in the baserom.
- Preserve every meaningful attempt: source, object, score, first mismatch,
  and rationale. Don't overwrite the best artifact without retaining it.

## Clean-room rules

- Use Mickey's own headers, symbols, source, assembly, objects, relocation
  metadata, and ROM bytes as authority. Follow the `PROVENANCE` disclosure
  rules in `docs/CLEANROOM.md` for any permitted published-decomp material;
  if provenance is uncertain, quarantine the candidate and continue with
  Mickey-only evidence.
- Never add ROM images, extracted assets, disassembly, instruction
  listings, hexdumps, machine-word arrays, or workbench ledgers to tracked
  files. `CLAUDE.md`'s "Nothing ROM-derived is ever tracked in git" section
  is the full rule; the pre-commit hook enforces it.
- Identify overlay functions by `(overlay, section, offset)` and exact
  ROM/text ownership; a shared synthetic VMA is not a unique identity
  across overlays.
- dp64 and `sfadebug` names/code/comments remain prohibited until the
  project owner rewrites `docs/CLEANROOM.md` (`docs/adr/0008-provenance.md`).

## Matching workflow

1. Confirm exact symbol boundaries, TU ownership, ABI/callers, current
   pragma/body, per-file flags, padding, and relocation surface.
2. Check the near-match oracle (coddog / `skeleton_scan.py`,
   `docs/adr/0007-matching-tools.md`) for the closest donor or already-matched
   sibling and put it in context before writing a candidate. `mips_to_c.sh`
   is a draft aid, never authority.
3. Compile the unmodified baseline immediately. Record size, exact words,
   first mismatch, frame/register shape, and relocations.
4. Classify the mismatch before editing: boundary/alignment, ABI/type,
   CFG/branch-likely, expression association, register allocation, loop
   spelling, instruction-set/flag mode, or relocation binding. Try the flag
   lattice before hand permutation; run the permuter only as a bounded batch
   job, never inside your own reasoning loop. Global toolchain flags, shared
   headers, symbol policy, yaml layout, and data placement are not changed
   merely to improve one candidate; a flag change that survives the sweep
   becomes canonical only with target evidence and an impact review across
   every consumer of that flag group.
5. Iterate with coherent hypotheses, preserving semantics, widths,
   signedness, and call order. Don't improve a score by inventing guards,
   merging unproved identities, or patching object bytes.
6. Stop at ~10 attempts without an exact match and record a plateau: best
   candidate, score, first mismatch, and what's blocking it
   (`docs/adr/0009-model-routing-and-agent-operation.md`).
7. Promote an exact result into the canonical split as its own commit.
   Rebuild the configured object and re-prove the function, boundary,
   relocation table, linked overlay, and full ROM.
8. Run donor/provenance, atlas/YAML, derived-number, progress, and
   clean-room gates before banking bytes or claiming a closure.

## Evidence and reporting

- Every claim in `docs/modules.md`/`symbol_addrs.us.txt` states its
  evidence tier (A byte-identity, B call graph, C string correspondence, D
  structural inference, `docs/modules.md` §1). A claim with no stated
  method is a bug in the document.
- Derived numbers (counts, percentages, sizes) are recomputed from the
  tree, never carried forward from an earlier message or file version.
- Lead reports with measurable outcomes: exact words/bytes, boundaries,
  relocations, linked status, remaining deficit. Distinguish
  source-reviewed, compiled, object-exact, canonical-staged, and ROM-exact
  states; never call a partial score a match.
- Every handoff states files changed, commits (with hashes), commands run,
  best score and first mismatch, unresolved blockers, and the next concrete
  action.
