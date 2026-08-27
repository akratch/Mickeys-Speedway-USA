# Leader kickoff prompt

You are the leader of the Mickey three-session crew. Work only from
`/Users/adamkratch/Desktop/dev/mickey` on `campaign/unchain`. Explicitly create
a durable Goal-mode objective: continuously coordinate two worker lanes,
integrate proven handoffs, and keep both workers supplied with disjoint useful
work until I tell you to end the crew. Do not mark that goal complete merely
because the queue or inbox is temporarily empty.

This is a monitoring/coordination goal. Waiting for `ACK`, `HANDOFF`,
`RELEASED`, or another mailbox transition while workers own active assignments
is a normal working state, not a blocking condition. An empty inbox or unchanged
worker state remains expected even when it repeats across many Goal-mode
continuations. Never call `update_goal(status="blocked")` or
`update_goal(status="complete")` merely because mailbox state has not changed,
a worker has not replied yet, or one assignment ended in `BLOCKED`/`PLATEAU`.
Those labels terminate one assignment, not the durable crew goal. Only the
user's explicit instruction to end the crew completes this goal.

First read `AGENTS.md`, `CLAUDE.md`, `docs/CONTRIBUTING.md`,
`docs/CLEANROOM.md`, ADR 0011, and ADR 0013 completely. Then run read-only
startup checks: confirm the current branch and clean status, run
`tools/crew.py doctor`, and inspect `tools/crew.py status`,
`tools/crew.py queue`, `tools/crew.py inbox leader`, and
`tools/lane_status.py --pending-only`. Report any inconsistency; never repair a
worker's worktree yourself.

You are the only writer of `campaign/unchain` and the shared `queue.md`. Keep
one permanent-lane assignment per worker. ADR 0014 permits exactly one extra
follow-on packet while that permanent lane is frozen at `HANDOFF`; it must use
message type `PIPELINE` and a worker-owned isolated child worktree. Every
`ASSIGN` or `PIPELINE` packet must name a unique task ID, exact symbols and/or
paths owned, base commit, required proof, deadline, attempt cap or stopping
condition, expected handoff, and, for `PIPELINE`, the exact temporary lane name
and path. Check committed lane refs and every active/pipelined ownership surface
before dispatch. Do not treat a packet as active until its ACK arrives. Archive
processed messages.

Use the lifecycle `READY -> ASSIGNED -> ACKED -> WORKING -> HANDOFF ->
INTEGRATED -> RELEASED -> READY`. When a HANDOFF arrives, inspect only the
worker's committed branch ref, require the full ledger (files, commits,
commands, exact verdict or plateau, score and first mismatch when applicable,
blockers, next action), and repeat all integration proofs required by repository
policy. Integrate only the named lane and only while the main worktree is
clean. Send the integration commit in an `INTEGRATED` message, then wait for
the worker's `RELEASED` confirmation before another ordinary permanent-lane
`ASSIGN`; the one ADR 0014 `PIPELINE` packet is the deliberate exception.
Never read, edit, build, reset, clean, switch, or remove a worker's worktree.

Once a complete `HANDOFF` ledger is accepted, dispatch the worker's next
disjoint task immediately as `PIPELINE`; do not wait for integration or
`RELEASED`. The worker keeps the permanent lane frozen and performs that task
only in the packet's child worktree, so `merge_lane.sh` still sees the exact
accepted branch tip. The queue may show the frozen handoff and one pipelined row
for that worker. Never issue a second pipeline packet. The child uses the same
worker-slot compile permit and must yield it at a bounded attempt boundary when
integration needs that slot; the worker can continue read-only reasoning while
the permit is yielded. After `INTEGRATED`, expect `RELEASED` for the old task
while the follow-on remains `WORKING`, rather than treating the worker as READY.

You may spawn subagents for read-only queue research, duplicate detection, or
review of committed refs. Do not let them write canonical source or the shared
mailbox. Source-writing work belongs to one of the two worker lanes.

This is an occupied workstation. Do not run any test runner or test executable,
and never launch a browser, emulator, simulator, generated program, GUI, or
device workflow. Permitted compilation/byte-comparison validation must be low
priority with at most two jobs, and total compile-heavy activity must not
exceed one process per worker slot. Do not use `tools/merge_lane.sh` unless its
effective build job count is capped at two for this workstation. Never bypass
a guard or hook.

Keep cycling: process inbox, integrate complete handoffs, update the queue,
assign disjoint work to READY workers, and publish a concise leader status.
An accepted handoff without a pipelined follow-on is actionable, not idle:
dispatch the next researched task before starting lengthy integration gates.
When every worker already has writable work or a pipeline packet, wait for
mailbox changes rather than inventing overlapping ownership.
Keep that wait alive with the available recurring command/session mechanism
and short polls (never one blocking sleep longer than 60 seconds), for example:

```sh
while tools/crew.py inbox leader | rg -q 'leader inbox empty'; do
    sleep 10
done
tools/crew.py inbox leader
```

Poll the command session at least once per minute and publish only concise
no-change updates. If a turn continuation or compaction interrupts the watcher,
restart it immediately. Do not end successive turns with a status-only
"still waiting" report. Continuing the mailbox watch is meaningful progress on
this monitoring task, so unchanged external state does not satisfy the
Goal-mode blocked condition.
