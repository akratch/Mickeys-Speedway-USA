# 0009. Model routing and agent operation

Status: Accepted
Date: 2026-08-24

## Context

`docs/acceleration-survey.md` §8 measured the local `n64dllm-v1` lane
(llama.cpp, ~17 GiB Q4_K_M, 49k context): across 5 targets and 18 samples,
8 were protocol-valid, 2 compiled, and **0 produced an improvement** over
the manual basin; every lane was graded "Harmful" and the server was
stopped on 2026-08-23. This agrees with published data cited in the same
section: a LoRA on LLM4Decompile-9B over 23.4k byte-exact N64 scratches
reached 10/53 exact, while the Macabeus benchmark (a frontier model, 12
attempts, compiler feedback in the loop) reached 74% overall on Animal
Forest — 90/70/50% on easy/medium/hard. A comparable sibling project
(Snowboard Kids 2) finished its last ten functions with a frontier coding
model at its highest reasoning setting, not a local specialist.

The pattern that actually won everywhere surveyed: a frontier model, real
compiler feedback in the loop, a hard cap of 10-12 attempts before moving
on, and a similarity-first schedule that puts the closest already-matched
sibling function in context as the worked example.

Separately, the old `AGENTS.md` built its entire orchestration model around
a fixed "four-slot runtime" and forbade agents from committing at all
(§7), which left the whole of August's work — 704 untracked paths, 23
modified tracked files — living only in a working directory with no hooks
ever having run on any of it (see ADR 0010).

## Decision

- The local `n64dllm-v1` lane is **retired** from the matching workflow.
  `docs/LOCAL_DECOMP_AGENT.md` is kept as historical documentation of what
  was tried and why it didn't work, not as an active operating contract.
- Matching work is routed to frontier models with compiler feedback in the
  loop (compile, compare, iterate), not to a local specialist.
- A **hard cap of ~10 attempts per function**: after roughly ten iterations
  without an exact match, stop and record a plateau — the best candidate,
  its score, first mismatch, and a concrete note on what's blocking it —
  rather than continuing to grind.
- **Similarity-first scheduling**: before starting a function, put its
  closest already-matched sibling (by the near-match oracle, ADR 0007) in
  context as the worked example.
- Cheap models are routed to mechanical, non-matching passes: renames,
  header consolidation, scoreboard regeneration, and similar bulk
  transformations that don't require compiler-feedback iteration.
- Every agent works in its own lane worktree (`tools/new_lane.sh`,
  ADR 0004) and commits its own work on its own lane branch in
  function-sized commits (ADR 0010), never with `--no-verify`.

## Consequences

- `AGENTS.md`'s local-model sections (the boundary rules, the
  `n64dllm-v1`-specific dispatch language) are removed; one sentence points
  at `docs/LOCAL_DECOMP_AGENT.md` as historical record.
- `AGENTS.md`'s "four-slot runtime," "saturate every collaboration slot,"
  and "never impose a smaller ceiling" language is replaced by the lane
  model: as many workers as there is independent work, each in its own
  lane, with no fixed slot count to saturate (ADR 0004).
- The plateau record from a capped-out function becomes the seed for the
  next attempt (a different model, a different hypothesis, or a permuter
  batch per ADR 0007) rather than being discarded.
