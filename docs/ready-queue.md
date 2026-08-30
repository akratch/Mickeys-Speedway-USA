# Ranked ready queue

`tools/ready_queue.py` gives a coordinator a deterministic ranked prefix of
targets that are safe to assign. It joins five independent checks:

1. `config/nonmatching-ranking.us.json` must pass the complete canonical
   validator in `tools/nm_ranking.py`.
2. Schema-v2 rows carry the selective-TU source digest recorded by
   `nm_ranking.py`; legacy rows recover the same evidence from the Git-blamed
   measurement commit. That evidence must equal the context at the requested
   base ref. Other `NON_MATCHING` candidate bodies and comments are removed
   before hashing; shared declarations, matched code, fallback identities, and
   this target's body remain covered. A relevant later edit makes the ranking
   evidence stale even when the symbol and path still exist.
3. The owning source path must be clean in the primary worktree and index, so
   an uncommitted contributor edit cannot be assigned to another lane.
4. The exact `(file, symbol)` identity must still be present in the live
   `NON_MATCHING` source queue discovered by `tools/permute_batch.py`.
5. `tools/lane_status.py` must classify the symbol as `base-only`. This is the
   only assignable state under ADR 0011. Active ownership is scoped to the
   validated target guard, its source handoff, its exact legacy-ledger rows,
   and `docs/matching-triage-handoffs/<symbol>.md`. A historical edit to
   another guarded function or another symbol's shard does not suppress this
   row; malformed or source-mismatched target shards fail closed.

The command is read-only. It reads source and committed Git objects; it does
not compile, inspect ROM text, or inspect sibling worktrees. One queue run
batch-resolves source definitions, indexes each unintegrated lane's committed
path delta, and parses shared legacy-ledger evidence once. Per-symbol checks
then inspect only lanes that changed that exact source, shard, or legacy row.
This preserves the fail-closed verdict while avoiding one full ref/history
walk per ranking row.
It resolves the primary checkout from `git worktree list --porcelain`; running
the command inside an integration or worker lane therefore still protects
uncommitted source edits in the main checkout.

## Usage

```sh
python3 tools/ready_queue.py
python3 tools/ready_queue.py --scan 100 --top 20 --format markdown
python3 tools/ready_queue.py --scan 100 --top 20 --format json
python3 tools/ready_queue.py --scan 100 --top 20 --format maintenance
python3 tools/ready_queue.py --focus retained-data --scan 50 --top 10
```

Before applying `--scan`, the tool derives a bounded-effort score from exact
size, differing-word count and ratio, and mismatch mechanism. Schema-v3 rows
use the relocation-masked residual for this cost while preserving the raw
count and both first-mismatch offsets in JSON and terminal output. This lets a
one-word `other` mismatch outrank a very large register-only plateau without
rewriting the retained measurement snapshot. Output reports both the derived
priority rank and the original snapshot rank, plus the proof-quality class and
effort score. Stale measurement context adds a 40-point penalty and changes
the quality to `reproof-*`; such a row is safe to assign only with a mandatory
configured baseline refresh before any source edit.

The `retained-data` focus is a compile-free ownership-triage view. It selects
equal-sized `reloc-mismatch` rows: every currently differing word is at a
relocation-bearing offset. Recent Overlay 41 and Overlay 45 closures showed
that this signature can identify a compiler-private pool or jump table whose
payload already belongs to retained overlay data. The view is deliberately a
lead, not a match claim: a worker must still authenticate the retained payload,
prove every static and runtime relocation identity, and byte-compare the linked
owner and full ROM. Relocation rows also carry a lower default effort penalty,
so promising ownership fixes are no longer buried below generic codegen rows.

The `TU batch` column gives each returned row's position and count among ready
targets in the same translation unit. Assign those rows to one worker in rank
order when ownership permits it: the content-addressed flag-sweep cache then
compiles that TU's 119 configurations once and rescores them for each function,
instead of rebuilding the same TU in separate lanes.

`--scan` bounds how many rows of that derived priority order are
examined. `--top` bounds how many assignable rows are returned, and cannot
exceed `--scan`. Processing stops as soon as either bound is reached. The
default scan ceiling is the 1,000-row hard limit, so a large prefix of active
or exhausted evidence cannot produce a false-empty queue; the default return
limit remains ten rows. Set `--scan` explicitly for a deliberately shallower
maintenance probe. The hard return limit is 100. Ties preserve the retained
snapshot order.

Unresolved ranking rows are reported as a count but are not invented into the
ranked order. Run `tools/nm_ranking.py --refresh-stale` to compile just those
rows together with changed and newly queued identities.

Table output is intended for a terminal. Markdown is paste-ready for a queue
or handoff. JSON is schema version 4, uses stable field names, preserves row
order in arrays, and includes `file_batch_position`/`file_batch_size`, the
resolved base commit, limits, detailed ready/skipped rows, and summary counts.
The `maintenance` format suppresses ready and exhausted rows and gives a
compact list of `stale-ledger`, `active`, and `dirty-worktree` blockers. Its
`class` column and JSON `maintenance_class` distinguish lane ownership,
primary-worktree dirt, malformed structured evidence, and prose-only plateaus
that need fresh measurement. The summary aggregates the same values under
`maintenance_classes`. Use them to schedule evidence reproof separately from
mechanical shard repair or lane release. A lane that merely predates a
handoff-only repair on the base is not active: target evidence is compared
with that lane's merge base before ownership is assigned.

## Fail-closed behavior

Rows with changed compiler contexts are visibly marked `reproof-*` and ranked
behind equally promising current evidence; their stale score is never claimed
as current. An owning path changed in the primary worktree or index is skipped
as `dirty-worktree`. Rows classified `active`, `already-integrated/exhausted`, or `stale-ledger`
are skipped and summarized separately. A ranking identity no longer present
in the live queue is summarized as `not-live`; use the ranking prune or
regeneration flow to remove it.

If a symbol is live or committed under a source path that disagrees with its
ranked path, the entire command fails instead of guessing which identity was
intended. Malformed ranking documents, duplicate live identities, unknown
lane-status states, unreadable refs, and invalid bounds also fail closed.

A valid report with no assignable rows exits 1. Input or evidence errors exit
2. A report containing at least one assignable target exits 0.
