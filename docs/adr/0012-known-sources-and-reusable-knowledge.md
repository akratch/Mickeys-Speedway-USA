# 0012. Known sources and reusable compiler knowledge

Status: Accepted
Date: 2026-08-27

## Context

ADR 0005 established donor availability before raw byte size, and ADR 0008
permits official Nintendo 64 SDK source distributed in the named public
decompilations. Mickey's first libultra harvest demonstrated the return: known
source and measured object boundaries moved a large cohort without bespoke
reverse engineering. The remaining workflow still said only to check the
nearest donor, which let an agent recognize library code and then unnecessarily
rewrite it instead of testing the actual versioned source.

The same loss occurs with compiler knowledge. Mickey's allocator-trace campaign
has produced reusable IDO behavior, but its durable findings are spread across
per-function ledgers. The
[84-day Snowboard Kids report](https://blog.chrislewis.au/decompiling-a-nintendo-64-game-in-84-days/)
independently found that agents needed an explicit known-library-source rule
and improved when proven IDO behavior was collected into a generic playbook.

## Decision

- When exact-object, naming, library-signature, corridor, or call-surface
  evidence makes a function likely SDK or standard-library code, use permitted
  official library source as the starting point. Exhaust plausible source
  versions, compiler flag groups, conditional-compilation paths, and
  object/TU boundaries before
  writing a custom implementation.
- Record a short plateau when that matrix is exhausted so later workers do not
  repeat it. The matrix is diagnostic: Mickey's bytes and relocations remain
  authority, and no source is promoted without ADR 0001's full exact proof and
  ADR 0008's required provenance.
- Reusable IDO behavior is promoted only from a Mickey-exact result or a
  controlled Mickey toolchain experiment into `docs/ido-learnings.md`.
  Per-function scores, addresses, register assignments, and attempt history
  stay in their existing ledgers.
- Each playbook entry states its symptom, mechanism, useful source lever,
  limits, and durable Mickey evidence. One-function curiosities and unverified
  external folklore do not enter the playbook.
- Toolchain-wide findings should also be proposed to the decomp-workbench field
  guide; Mickey's playbook records only the local evidence and routing rule.

## Consequences

- `AGENTS.md` gains an explicit library-source-first branch before m2c or custom
  source reconstruction.
- `docs/ido-learnings.md` becomes the compact reusable context loaded for IDO
  matching work; the resident/overlay ledgers remain the detailed record.
- This decision does not broaden the clean-room source list or permit bulk
  transcription. `docs/CLEANROOM.md` and ADR 0008 remain authoritative.
