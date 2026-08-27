# 0013. Three-session crew control

Status: Accepted
Date: 2026-08-27

## Context

ADR 0004 supplies isolated worktrees, ADR 0010 makes commits the durable unit
of work, and ADR 0011 permits committed-ref discovery without crossing lane
boundaries. Several independent Codex sessions can therefore work safely, but
they still need one assignment authority and a communication path that does
not depend on chat context or a shared tracked file.

Codex Goal mode keeps one session pursuing a durable objective, while Codex
subagents report to their own parent session rather than directly coordinating
independent sessions. Git worktrees provide the required source and build
isolation. The missing layer is intentionally small: a shared mailbox and an
explicit ownership protocol.

## Decision

- The standard interactive crew has one leader in `campaign/unchain` and two
  long-lived worker slots, `worker-1` in `lane/crew-worker-1` and `worker-2` in
  `lane/crew-worker-2`. This is a convenient session topology, not a campaign
  limit and does not supersede ADR 0004.
- Each top-level session creates a durable Goal-mode objective. The leader's
  goal is queue ownership and integration. Each worker's goal is the repeated
  assignment, handoff, release, and ready cycle; becoming idle does not finish
  that goal while the crew remains active.
- `tools/crew.py` stores runtime state under
  `$(git rev-parse --git-common-dir)/codex-crew/`. Every worktree shares that
  directory, and Git can never stage it. The state is coordination metadata,
  never clean-room or match evidence.
- The leader alone owns `queue.md`, the integration branch, and assignment
  messages. Each actor alone replaces its own `status/<role>.md`. Inbox
  messages are immutable, uniquely named files written atomically; processing
  one produces an acknowledgement or follow-up and archives the original.
- The normal task lifecycle is `READY -> ASSIGNED -> ACKED -> WORKING ->
  HANDOFF -> INTEGRATED -> RELEASED -> READY`. `BLOCKED` and `PLATEAU` are
  explicit terminal outcomes for one assignment, not permission to discard
  its work.
- An assignment names an exact ownership surface: symbols and/or paths, base
  commit, expected evidence, deadline, and stopping condition. The leader
  checks committed lane refs before assignment and never gives overlapping
  ownership to both workers.
- Workers commit only on their own permanent lane. After the leader integrates
  a handoff, the worker fast-forwards that same lane to the supplied integration
  commit, confirms it is clean, reports `RELEASED`, and becomes `READY`. The
  leader never edits, resets, cleans, or removes a worker's worktree.
- Subagents are read-only by default. A write-capable subagent receives a
  disjoint child task in a separate child worktree and returns commits to its
  parent worker. That worker verifies and integrates the commits, then removes
  only its own clean, released child worktree. Subagents never write the
  top-level mailbox or claim a second top-level task.
- Each worker slot owns at most one compile-heavy process at a time, including
  all of its subagents. The leader observes any stricter workstation safety
  limit and does not use integration work to exceed it.
- Durable findings remain in commits and the existing tracked knowledge files.
  Mailbox state can be deleted and rebuilt without losing project work.

## Consequences

- Three sessions can coordinate through ordinary files and short commands,
  without a service, daemon, database, or shared source checkout.
- Permanent worker worktrees eliminate repeated setup and accumulated lane
  churn. Only short-lived child worktrees are cleaned up, by the worker that
  created them and only after their commits are retained.
- One-writer files and immutable messages avoid normal merge and overwrite
  races. There is no authentication layer; compliance is procedural and
  reviewable through message authorship and lane refs.
- ADR 0011's prohibition on reading or acting on another lane's in-flight
  work remains unchanged. The mailbox communicates state; it does not grant
  cross-lane access.
