# 0017. Causal exploration and risk-proportional validation

Status: Accepted
Date: 2026-09-06

## Context

The September 6 stocktake found infrastructure and score improvements without
new matched functions. Some scalar winners introduced undefined behavior;
another search reached review before an unsupported baseline context became
visible. ADR 0016 permits broad bounded sweeps but limits expensive-model
matching to frame-exact candidates with at most eight differing words and one
attempt. That restriction can exclude the structural investigation needed to
escape a plateau while leaving repeated automated searches available.

The owner authorized a bounded, hypothesis-driven exploration of a function
family, using the existing workbench before expanding tooling. Exploration
permission and acceptance evidence must be distinct.

## Decision

- A coordinator may assign a **causal exploration packet** to one isolated lane.
  The packet names an anchor function, relevant matched siblings and potential
  follow-ons, exact ownership/base, authenticated ABI and source evidence,
  competing hypotheses, predicted observables, and prior attempts not to repeat.
  A family is a source/compiler relationship, not a quota of nearby scores.
- Allow 60–90 minutes of investigation, with an explicit short proof/handoff
  grace period. Retain ADR 0009's approximately ten distinct source-attempt cap
  per function; use a small set of hypotheses and count their variants. Baseline
  measurements and tool failures are not new source hypotheses. Expiry preserves
  evidence, not permission to restart the same search under a new label.
- For such a recorded packet, this ADR supersedes ADR 0016's frame-exact,
  eight-word and single-attempt restrictions. It does not reopen every plateau.
  ADR 0011's committed source/handoff pins, active-lane exclusions and zero-exit
  assignment gate still apply separately to each function before source edits.
- Prove the baseline feedback loop before exploring: configured full-TU build,
  owned extent, relocation identities, and any importer/emitter used must be
  faithful. The context comparator must accept the **actual compiler input
  compared with itself**. An unverifiable self-comparison is a preparation
  blocker, not permission to waive context review or buy a random search.
- Before each source attempt, record the cause being tested, predicted change
  and invariants. Prefer existing phase replay, source/def-use probes and
  matched sibling controls. Label heuristic attribution separately from a
  measured cause. Negative results should eliminate an explanation, not merely
  append another score. Do not rerun an exhausted flag lattice without changed
  evidence about its inputs or interpretation.
- Isolated, explicitly labelled source-shaping diagnostics may use defined,
  semantically inert forms to test allocation or scheduling. This narrowly
  supersedes the prohibition on adding inert forms during nonexact exploration;
  it does not establish that an original debug macro or source idiom existed.
  Prove the input domain and evaluation behavior first. Uninitialized or
  out-of-bounds reads, signed-overflow assumptions, new observable/volatile
  accesses, invented identities and post-compile instruction edits remain
  prohibited. Dummy padding is not promotable source reconstruction.
- Keep diagnostic source, objects, scores, mismatch locations and reasoning in
  ignored lane evidence. A nonexact diagnostic is never adopted as a better
  canonical body merely because its score improves. An exact inert spelling
  still requires independent semantic review, disclosure and the cleanup queue,
  plus every ordinary acceptance proof. Forced compiler behavior is diagnostic
  only; only stock compiler output can earn credit.
- Rank admissible candidates separately from scalar minima. Track instruction
  shape, size/frame and relocation surface, and reject known-invalid mutation
  classes; a static filter or unchanged declaration context is not semantic
  proof. Preserve useful alternatives instead of overwriting them with an
  inadmissible winner.
- Random search is an optional bounded experiment, not the default follow-up.
  Declare its time/gain stop rule accurately. The existing `flat-minutes` is
  not a rolling stagnation timer; a rolling timer, if introduced, is opt-in and
  must retain time-to-best evidence and the cost of missed late improvements.
- Validation follows the state changed. Run focused checks during diagnostics;
  unchanged report-only work needs evidence preservation and a clean-state
  check, not blanket closing rebuilds. All existing pre-commit, hook, promotion
  and integration gates remain mandatory. Tool changes receive relevant tests
  and full landing gates; independent review should add distinct evidence.
- Tooling work must unblock a named experiment within its budget. Prefer the
  existing workbench and narrow adapters over a new framework. Report matched
  functions/bytes separately from hypotheses resolved and tooling repaired;
  none of the latter satisfies a matching quota.

## Consequences

AGENTS.md and the contributor guide distinguish default sweeps from explicitly
authorized causal packets. The cleanup queue remains an exact-result policy,
not authority to promote diagnostics. Existing pin schemas and lane isolation
are unchanged. The first trial must report its actual outcome and reusable
evidence before a mechanism is scaled to follow-on functions. Clean-room
provenance, semantic correctness, exact relocation and linked-range checks,
full-ROM verification, private/public separation and ADRs 0001/0002 are unchanged.
