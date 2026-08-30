# Worker checkpoints

Long bounded lanes publish coordination state through `tools/crew.py`. The
records live under `$(git rev-parse --git-common-dir)/codex-crew/heartbeats/`,
so every worktree can see them but Git cannot stage them. Each replacement is
an atomic rename. A checkpoint is scheduling telemetry, never match evidence.

The launcher seeds the target, assignment base, soft deadline, and an
unmeasured attempt-zero checkpoint. After the baseline, after each material
source attempt, and before a long bounded tool call, the worker updates it:

```sh
python3 tools/crew.py checkpoint \
  --worker "$MICKEY_HEARTBEAT_WORKER" \
  --progress "attempt 3: changed declaration order" \
  --attempt-count 3 \
  --best-score "187/217 words differ" \
  --mismatch-class frame-allocation
```

`heartbeat` remains an alias-compatible spelling of `checkpoint`. Omitted
assignment and checkpoint fields retain their prior values. Attempt counts may
not decrease within one assignment. `--eta-unix` may add an expected handoff
time; the soft `deadline_unix` is always present.

A coordinator polls one worker or the full directory as one compact JSON line:

```sh
python3 tools/crew.py heartbeat-status --json --check
python3 tools/crew.py heartbeat-status --worker worker-1 --json --check
```

The report includes target/base/commit, attempt count, best score, mismatch
class, progress age, ETA/deadline deltas, and a `current`, `terminal`, `stale`,
or `malformed` health value. Its top-level `ok` is false for stale or malformed
state. JSON polling exits nonzero for stale or malformed state; `--check` adds
the same stale-state behavior to the human table. Malformed records always exit
nonzero. Invalid schemas, types, timestamps, worker/file identities, checkpoint
fields, and regressing attempt counts fail closed.

Staleness means active progress exceeded the selected age threshold, the soft
deadline or ETA passed, or the progress timestamp is implausibly in the future.
It is a signal to request a graceful checkpoint and handoff. Neither polling
nor updating a checkpoint sends a signal or stops a process.
