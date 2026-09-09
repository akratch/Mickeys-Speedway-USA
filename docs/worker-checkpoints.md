# Worker checkpoints

Long bounded lanes publish coordination state through `tools/crew.py`. Records
live under `$(git rev-parse --git-common-dir)/codex-crew/heartbeats/`, so every
worktree can see them but Git cannot stage them. Each replacement is atomic.
A checkpoint is scheduling telemetry, never match evidence.

## Current and best measurements

Schema 4 distinguishes the latest **current** measurement from the retained
**best** measurement. Existing top-level metric fields describe current
source. `best_result` contains the independently validated best fields,
`current_score` formats current, and `best_score` formats best. Current and
best also retain separate mismatch classes. A regressing attempt therefore
remains visible without erasing the candidate that should be restored.

Each measured result is all-or-nothing:

- `target_words`, `candidate_words`
- `raw_differing_words`, `relocation_masked_differing_words`
- `candidate_relocations`, `target_relocations`
- `exact_relocation_identities`
- `promotion_state`: `compiled`, `object-exact`, `canonical-staged`, or
  `rom-exact`

An unmeasured result has null integers and `promotion_state=unmeasured`.
Partial sets fail closed. `object-exact`, `canonical-staged`, and `rom-exact`
require equal word geometry, zero raw and masked differences, equal relocation
counts, and every relocation identity exact.

A current result replaces best only when its comparison key is strictly
greater. The key, in order, is:

1. later promotion state (`unmeasured` through `rom-exact`);
2. exact target/candidate word geometry;
3. fewer relocation-masked differences, then fewer raw differences;
4. a complete relocation-identity surface, then the exact-identity fraction;
5. equal candidate/target relocation counts, then more exact identities;
6. smaller candidate/target word-count delta.

An exact tie retains the earlier best and its artifact. The ordering is
deterministic and never consults score prose, timestamps, or attempt count.

Schemas 1, 2, and 3 remain readable. They normalize to unmeasured defaults or,
for schema 3, treat its sole numeric result as both current and best. The next
successful update writes schema 4 without guessing numeric meaning from legacy
score prose.

## Recording a checkpoint

After the baseline, after each material attempt, and before a long bounded
call, ingest a fresh concise workbench report from the ignored build tree:

```sh
symbol=func_8001C114
summary=build/wb/$symbol.checkpoint-summary.json
mkdir -p build/wb
nice -n 15 tools/wb_compare.sh --summary-json "$symbol" > "$summary"

python3 tools/crew.py checkpoint \
  --worker "$MICKEY_HEARTBEAT_WORKER" \
  --progress "attempt 3: changed declaration order" \
  --attempt-count 3 \
  --wb-summary "$summary" \
  --mismatch-class frame-allocation
```

`mickey-wb-summary-v1` supplies word geometry, raw and relocation-masked
differences, symbol identity, admissibility, and authenticated relocation
counts when available. If an older compatible report omits relocation totals,
supply all three explicitly. Any explicit metric that disagrees with imported
JSON is rejected.

The import accepts only a fresh (at most 15 minutes old), regular, non-symlink
file at a canonical ignored `build/` path. It caps input at 64 KiB and checks
the assigned target and optional report worker. Paths, digests, instruction
rows, bytes, and disassembly are never stored in the heartbeat.

For a measurement not produced by `wb_compare.sh`, supply all fields:

```sh
python3 tools/crew.py checkpoint \
  --worker "$MICKEY_HEARTBEAT_WORKER" \
  --progress "canonical promotion and ROM proof complete" \
  --attempt-count 4 \
  --target-words 108 --candidate-words 108 \
  --raw-differing-words 0 --relocation-masked-differing-words 0 \
  --candidate-relocations 6 --target-relocations 6 \
  --exact-relocation-identities 6 \
  --promotion-state rom-exact \
  --mismatch-class exact
```

A workbench summary derives only `compiled` or `object-exact`; it derives
`object-exact` only when authenticated relocation evidence proves every
identity. It cannot claim canonical staging or a ROM proof. `--best-score`
remains only for legacy unmeasured updates and is refused with numeric metrics.

## Archiving and restoring a best candidate

When a strict new best is accepted, checkpointing can archive its exact source,
candidate object, and summary under Git-common ignored state:

```sh
python3 tools/crew.py checkpoint \
  --worker "$MICKEY_HEARTBEAT_WORKER" \
  --progress "attempt 5: new best" --attempt-count 5 \
  --wb-summary "build/wb/$symbol.checkpoint-summary.json" \
  --mismatch-class register-allocation \
  --archive-best \
  --source src/main/example.c \
  --candidate-object build/src/main/example.c.o
```

The manifest binds hashes and modes for all three files to worker, target,
assignment base, branch, worktree, HEAD, and their repository-relative paths.
Source must be one exact tracked C path; object and summary must be regular,
non-symlink files below ignored `build/`. The archived summary must reproduce
the accepted measurement. A tie or regression updates current while preserving
the old best and archive.

Restore only the recorded source with:

```sh
python3 tools/crew.py restore-best --worker "$MICKEY_HEARTBEAT_WORKER"
```

Restore refuses worktree, branch, assignment-base, target, path, manifest,
content hash, mode, symlink, ancestry, staged-source, or unmerged-source drift.
It validates every archived payload, copies the displaced current source into
the artifact's backup directory, then atomically replaces only the recorded
source path. Its JSON receipt includes the exact
`restore-best --recover-backup ARTIFACT_ID/BACKUP_ID` command that reverses
that source replacement. Recovery revalidates the original artifact-manifest
hash and the same provenance before writing.

After restore, rebuild the comparison and refresh current in one command:

```sh
python3 tools/crew.py reprove-best \
  --worker "$MICKEY_HEARTBEAT_WORKER" \
  --progress "restored best and re-proved object" \
  --attempt-count 5 \
  --mismatch-class register-allocation
```

`reprove-best` first requires the source hash to equal the archive, then runs
`tools/wb_compare.sh --summary-json` through `nice -n 15` with
`MAKEFLAGS=-j2` and the campaign's two-job environment. It imports the
resulting summary as current; the ordinary strict-best comparator still
decides whether best changes.

Artifacts and backups are local recovery state, not canonical match evidence,
and are never committed. Archival currently requires a summary carrying enough
relocation evidence to reproduce the accepted result. Reproof establishes only
the workbench/object state; canonical staging, linked-range comparison, and
full-ROM proof remain separate promotion gates.

## Polling and staleness

`heartbeat` remains an alias-compatible spelling of `checkpoint`. Omitted
assignment and checkpoint fields retain prior values. Attempt counts cannot
decrease within an assignment. A coordinator can poll compact JSON:

```sh
python3 tools/crew.py heartbeat-status --json --check
python3 tools/crew.py heartbeat-status --worker worker-1 --json --check
```

The report exposes current and best scores/results separately, plus target,
base, commit, attempt count, mismatch classes, progress age, ETA/deadline, and
health. Staleness means progress exceeded the selected age, deadline or ETA
passed, or a timestamp is implausibly in the future. It requests a graceful
checkpoint and handoff; polling and updating never signal or stop a process.
