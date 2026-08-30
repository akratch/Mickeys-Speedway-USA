# Worker checkpoints

Long bounded lanes publish coordination state through `tools/crew.py`. The
records live under `$(git rev-parse --git-common-dir)/codex-crew/heartbeats/`,
so every worktree can see them but Git cannot stage them. Each replacement is
an atomic rename. A checkpoint is scheduling telemetry, never match evidence.

The launcher seeds the target, assignment base, soft deadline, and an
unmeasured attempt-zero checkpoint. Schema 3 keeps schema 1 and 2 readable,
but replaces score prose with an all-or-nothing measured result. A measured
record has these non-negative integer fields:

- `target_words`, `candidate_words`
- `raw_differing_words`, `relocation_masked_differing_words`
- `candidate_relocations`, `target_relocations`
- `exact_relocation_identities`

It also has `promotion_state`: `compiled`, `object-exact`,
`canonical-staged`, or `rom-exact`. An unmeasured record stores null for all
seven integers and uses `unmeasured`. Partial metric sets fail closed.

After the baseline, after each material source attempt, and before a long
bounded tool call, capture the existing concise workbench report under the
ignored build tree and ingest it directly:

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
  --candidate-relocations 6 \
  --target-relocations 6 \
  --exact-relocation-identities 6 \
  --mismatch-class frame-allocation
```

The current `mickey-wb-summary-v1` producer supplies word geometry, raw and
relocation-masked differences, matched-word consistency, symbol identity, and
whether an exact object comparison is admissible. It does not yet carry total
relocation or exact-identity counts, so those three flags remain mandatory for
that report version. If a compatible producer includes those integers in a
`relocations` object, do not repeat them on the command line. Any explicit
metric that disagrees with imported JSON is rejected.

The import accepts only a fresh (at most 15 minutes old), regular, non-symlink
file at a canonical repository-relative `build/` path that Git ignores. It
caps input at 64 KiB, requires the assigned target to equal the report's
requested, target, or candidate symbol, and checks a report worker when one is
present. Malformed geometry, matched counts, exact claims, timestamps, or
numeric invariants fail before the heartbeat is replaced. The report path,
digests, bytes, disassembly, and relocation rows are never stored.

For a measurement that did not come from `wb_compare.sh`, supply all fields
explicitly:

```sh
python3 tools/crew.py checkpoint \
  --worker "$MICKEY_HEARTBEAT_WORKER" \
  --progress "canonical promotion and ROM proof complete" \
  --attempt-count 4 \
  --target-words 108 \
  --candidate-words 108 \
  --raw-differing-words 0 \
  --relocation-masked-differing-words 0 \
  --candidate-relocations 6 \
  --target-relocations 6 \
  --exact-relocation-identities 6 \
  --promotion-state rom-exact \
  --mismatch-class exact
```

`object-exact`, `canonical-staged`, and `rom-exact` require equal word
geometry, zero raw and masked differences, equal relocation counts, and every
identity exact. A workbench summary derives only `compiled` or `object-exact`;
it derives `object-exact` only when its authenticated relocation block also
proves every identity, and otherwise retains `compiled` even when the words
are equal. It cannot claim canonical staging or a ROM proof. `--best-score` remains for
legacy unmeasured updates, but is refused when numeric evidence is present;
the tool generates an unambiguous concise score from the integers instead.

`heartbeat` remains an alias-compatible spelling of `checkpoint`. Omitted
assignment and checkpoint fields retain their prior values. Attempt counts may
not decrease within one assignment. `--eta-unix` may add an expected handoff
time; the soft `deadline_unix` is always present. Reading schema 1 or 2 fills
the new metrics with null and `promotion_state=unmeasured`; the next update
writes schema 3 without trying to reinterpret legacy score prose.

A coordinator polls one worker or the full directory as one compact JSON line:

```sh
python3 tools/crew.py heartbeat-status --json --check
python3 tools/crew.py heartbeat-status --worker worker-1 --json --check
```

The report includes target/base/commit, attempt count, all normalized numeric
fields, promotion state, generated best score, mismatch class, progress age,
ETA/deadline deltas, and a `current`, `terminal`, `stale`, or `malformed`
health value. Legacy records expose null numeric fields rather than ambiguous
derived values. Its top-level `ok` is false for stale or malformed state. JSON
polling exits nonzero for stale or malformed state; `--check` adds the same
stale-state behavior to the human table. Malformed records always exit
nonzero. Invalid schemas, types, timestamps, worker/file identities,
checkpoint fields, and regressing attempt counts fail closed.

Staleness means active progress exceeded the selected age threshold, the soft
deadline or ETA passed, or the progress timestamp is implausibly in the future.
It is a signal to request a graceful checkpoint and handoff. Neither polling
nor updating a checkpoint sends a signal or stops a process.
