# 0014. Pipelined handoff work

Status: Accepted
Date: 2026-08-27

## Context

ADR 0013 freezes a worker's permanent lane after `HANDOFF` until the leader
integrates it and the worker reports `RELEASED`. That makes the reviewed branch
stable, but full integration proofs take several minutes and leave the worker
with no productive source work during review. Continuing on the permanent lane
is not safe: its moving tip could make `merge_lane.sh` validate or merge commits
that were not in the accepted handoff.

The repository already has the required isolation primitive. A worker may own
a short-lived child worktree, and commits are the durable transfer unit. The
follow-on task can therefore proceed on a separate branch while the permanent
lane remains frozen at the exact handoff commit.

## Decision

- A worker normally owns one permanent-lane task. Once that task reaches a
  complete `HANDOFF`, the leader immediately dispatches at most one disjoint
  follow-on packet with message type `PIPELINE` instead of waiting for
  `RELEASED`.
- A `PIPELINE` packet has every normal assignment field plus an exact temporary
  lane name/path. The leader checks committed lane refs and all active ownership
  before sending it. Pipeline ownership may not overlap the frozen handoff, the
  other worker, or canonical work in progress.
- The worker ACKs the packet, creates its own child worktree at the named base,
  and works only there. The permanent lane, its index, build products, and
  branch ref remain frozen until the leader integrates the accepted handoff.
- `PIPELINED` means one permanent-lane handoff is awaiting integration while one
  isolated follow-on task is active. It is the only permitted two-packet state
  for a worker; no second follow-on task is dispatched.
- The child uses the same worker-slot compile permit. It checks the mailbox
  between bounded tool calls and yields that permit when integration needs the
  slot. The worker can continue read-only analysis while its compiler permit is
  yielded.
- On `INTEGRATED`, the worker fast-forwards the clean permanent lane and sends
  `RELEASED` for the old task. If the pipeline task is unfinished, it remains
  `WORKING` in the child worktree. When its result is committed, the worker
  transfers those commits onto the now-current permanent lane, regenerates only
  the standard derived artifacts if needed, and repeats all normal proofs there
  before sending the next `HANDOFF`.
- The worker removes only its own clean pipeline worktree, and only after every
  unique commit is retained on the permanent lane. Unexpected transfer
  conflicts are reported rather than guessed through.
- A worker with neither writable matching work nor a protocol action performs
  read-only next-target research and mailbox monitoring. Empty mailbox state is
  expected monitoring, never completion or a durable-goal blocker.

## Consequences

- Review latency no longer idles a matching worker or moves the branch being
  reviewed. `merge_lane.sh` continues to integrate the stable permanent lane.
- `tools/crew.py` gains `PIPELINE` messages and the `PIPELINED` actor state.
- The leader queue may contain one frozen handoff row and one pipelined row for
  the same worker. This narrow exception supersedes ADR 0013's one-assignment
  wording; all other ownership and release rules remain in force.
- Pipeline setup and transfer add small Git-worktree overhead, but keep source,
  builds, and match claims isolated and reviewable.
