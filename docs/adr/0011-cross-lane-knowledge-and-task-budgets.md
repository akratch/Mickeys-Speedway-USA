# 0011. Cross-lane knowledge and task budgets

Status: Accepted
Date: 2026-08-27

## Context

ADR 0004 isolated every worker in its own worktree and ADR 0010 made small,
frequent lane commits the unit of durable work. That removed build contention
and data-loss risk, but it left two coordination gaps. A match committed on one
lane could remain invisible to other workers until integration, causing
duplicate work and withholding a newly useful sibling. Separately,
`tools/codex_lane.sh` launched workers without a wall-clock budget even though
ADR 0009 capped matching attempts and required a plateau handoff.

Chris Lewis reported both effects in the
[84-day Snowboard Kids campaign](https://blog.chrislewis.au/decompiling-a-nintendo-64-game-in-84-days/):
searching every worktree made fresh matches immediately useful without forcing
continuous merges, while an explicit deadline helped agents divide time
between permutation, reasoning, and a final handoff. Mickey already has the
stronger prerequisite for a safer implementation: useful work is committed as
it lands, so coordination need not inspect another lane's in-flight files.

## Decision

- A coordinator and read-only coordination tools may inspect **committed**
  `refs/heads/lane/*` objects through Git's object database. They may list
  commits, changed paths, and commit-message match claims, and may read a
  committed candidate for use as a sibling after its provenance is clear.
- No worker or tool may read, edit, build, clean, reset, or otherwise act on
  another lane's working directory, index, process state, or uncommitted files.
  Integration and ownership still require the ordinary lane handoff.
- A cross-lane match claim is scheduling information, not match evidence. It
  cannot earn scoreboard credit until the receiving/integration lane repeats
  the configured object, relocation, linked-range, and full-ROM proof.
- Before assigning a target, the coordinator checks committed lane refs for an
  existing claim. Fresh committed siblings are considered alongside the
  external near-match oracle before a new candidate is written.
- A current plateau may be reopened only by a tracked, symbol-keyed
  authorization that pins its full source and handoff commit IDs and names the
  new mechanism. The assignment gate validates both commits and returns ready
  only while that exact pair remains current; the next source or handoff commit
  consumes the authorization automatically. A missing handoff may be pinned as
  null for one maintenance remeasurement, but malformed or foreign evidence
  remains closed. Active lane ownership still wins.
- Every non-interactive agent run receives a soft wall-clock deadline in its
  prompt and environment plus a short hard-stop grace period. It reserves the
  end of the budget for a commit or plateau handoff and does not start a tool
  call whose own cap cannot fit in the remaining time.
- Budgets are chosen by work class and may grow as easy work disappears; there
  is no single campaign-wide duration. Expiry preserves the best meaningful
  attempt and records the ADR 0009 plateau. It never authorizes destructive
  reset or silent loss of work.
## Consequences

- `tools/lane_status.py` provides the committed-ref view and labels its output
  as claims rather than proof. It never opens sibling worktree paths.
- `config/lane-reopen-authorizations.us.json` records rare, reviewable
  exceptions without making exhausted targets generally assignable. A
  null-ledger exception is valid only while its source pin remains the latest
  exact target guard or evidence commit.
- `tools/codex_lane.sh` accepts `--minutes`, exports the soft/hard deadlines,
  injects the handoff rule into the prompt, and records the runner exit status
  as before.
- `AGENTS.md` and `docs/CONTRIBUTING.md` state the narrow read-only exception
  and task-budget rule.
- This ADR does not weaken lane ownership, clean-room provenance, ADR 0001's
  exact-match standard, or ADR 0010's integration gates.
