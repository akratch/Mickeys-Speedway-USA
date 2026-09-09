# 0016. Raw-assembly TU pipeline and standing sweep

Status: Accepted
Date: 2026-09-01

## Context

Epoch 14 left two different kinds of unresolved work behind. Guarded
`NON_MATCHING` bodies have source and ownership but are mostly exhausted under
ADR 0011, while compiler-generated functions in splat `asm` subsegments have
neither a C translation unit nor a surface that the ordinary compile/compare
tools can vary. Treating those raw blocks one function at a time makes every
worker rediscover translation-unit flags, declarations, and data ownership.

The fidelity fixes recorded under ADR 0007 also changed the status of the
permuter. `tools/permute_sweep.sh` is a bounded batch mechanism that compiles
ordinary source and tests untouched compiler output; it is not an agent
reasoning loop or an instruction-editing postprocess. A current plateau may
therefore still be valid sweep input even though ADR 0011 correctly prevents
an unspecific agent reopen.

Finally, broad expensive-model passes have poor expected value on structural
or allocation-wide residuals. The useful escalation surface is the narrow
tail where the frame is already exact, relocation masking leaves only a few
words, and the workbench identifies a source-level lever.

## Decision

- A splat `asm` subsegment whose functions are compiler output is converted to
  C ownership before any contained function is decompiled. Carve the whole
  block into one C translation unit containing one `#pragma GLOBAL_ASM` stub
  per function, preserve the configured flags and order, and prove that the
  ownership-only change leaves the ROM byte-identical.
- A block may be divided at 16-byte-aligned function starts when parallel work
  needs smaller translation units. Such boundaries are **working splits**, not
  measured file boundaries. The YAML comment must say so explicitly and must
  not claim object identity or source-file evidence that has not been proved.
- The fidelity-proven ADR 0007 batch sweep, invoked through
  `tools/permute_sweep.sh`, is standing authorization over every guarded
  `#ifdef NON_MATCHING` candidate, including a current plateau. It remains a
  bounded batch tool and all ordinary candidate, relocation, linked-range,
  and ROM proofs still apply.
- A sweep-discovered exact promotion is proof under ADR 0001, not a plateau
  reopen under ADR 0011. The sweep supplies a new exact result rather than an
  authorization to resume open-ended manual attempts. A sweep run which does
  not improve a plateau writes no new handoff, marker, attempt history, or
  other tracked noise.
- Expensive models are used only when the configured candidate is frame-exact,
  has at most eight relocation-masked differing words, and a workbench verdict
  names a concrete source lever. Each qualifying target receives one attempt.
  Tool faults and match-bearing merge conflicts remain operational exceptions,
  not a widening of the matching queue.

## Consequences

- Raw compiler blocks gain an ordinary build surface without receiving any
  matching credit. The initial carve is infrastructure: all contained
  functions remain assembly-backed until separately proved exact.
- Translation-unit context is established once per block, so later function
  lanes can work from the same declarations, flags, ordering, and ownership
  proof. A working split stays visibly provisional until independent evidence
  establishes a real file boundary.
- ADR 0011 continues to block generic or mechanism-free manual reopens. It
  does not suppress an exact result produced by the standing sweep, and failed
  sweep passes do not churn the plateau ledgers.
- Expensive-model capacity is concentrated on small, diagnosed residuals;
  frame-divergent, broad structural, and allocation-only candidates return to
  TU shaping or the batch sweep.
- `docs/epoch15-plan.md` supplies the initial waves and machine limits. This
  ADR supplies the durable policy after that campaign ends.
