# Ranked ready queue

`tools/ready_queue.py` gives a coordinator a deterministic ranked prefix of
targets that are safe to assign. It joins five independent checks:

1. `config/nonmatching-ranking.us.json` must pass the complete canonical
   validator in `tools/nm_ranking.py`.
2. Git blame identifies the commit that last changed each row's
   `differing_words` measurement. The compiler-relevant selective-TU context
   at that commit must equal the context at the requested base ref. Other
   `NON_MATCHING` candidate bodies and comments are removed before hashing;
   shared declarations, matched code, fallback identities, and this target's
   body remain covered. A relevant later edit makes the ranking evidence stale
   even when the symbol and path still exist.
3. The owning source path must be clean in the primary worktree and index, so
   an uncommitted contributor edit cannot be assigned to another lane.
4. The exact `(file, symbol)` identity must still be present in the live
   `NON_MATCHING` source queue discovered by `tools/permute_batch.py`.
5. `tools/lane_status.py` must classify the symbol as `base-only`. This is the
   only assignable state under ADR 0011. Active ownership is scoped to the
   validated target guard and its handoff, so a historical edit to another
   guarded function in the same translation unit does not suppress this row.

The command is read-only. It reads source and committed Git objects; it does
not compile, inspect ROM text, or inspect sibling worktrees.

## Usage

```sh
python3 tools/ready_queue.py --scan 50 --top 10
python3 tools/ready_queue.py --scan 100 --top 20 --format markdown
python3 tools/ready_queue.py --scan 100 --top 20 --format json
```

Before applying `--scan`, the tool derives a bounded-effort score from exact
size, differing-word count and ratio, and mismatch mechanism. This lets a
one-word `other` mismatch outrank a very large register-only plateau without
rewriting the retained measurement snapshot. Output reports both the derived
priority rank and the original snapshot rank, plus the proof-quality class and
effort score. Stale measurement context adds a 40-point penalty and changes
the quality to `reproof-*`; such a row is safe to assign only with a mandatory
configured baseline refresh before any source edit.

The `TU batch` column gives each returned row's position and count among ready
targets in the same translation unit. Assign those rows to one worker in rank
order when ownership permits it: the content-addressed flag-sweep cache then
compiles that TU's 119 configurations once and rescores them for each function,
instead of rebuilding the same TU in separate lanes.

`--scan` bounds how many rows of that derived priority order are
examined. `--top` bounds how many assignable rows are returned, and cannot
exceed `--scan`. Processing stops as soon as either bound is reached. The
defaults are 50 scanned rows and ten returned rows; hard limits are 1,000 and
100 respectively. Ties preserve the retained snapshot order.

Unresolved ranking rows are reported as a count but are not invented into the
ranked order. Regenerate the ranking to give them measured positions.

Table output is intended for a terminal. Markdown is paste-ready for a queue
or handoff. JSON is schema version 3, uses stable field names, preserves row
order in arrays, and includes `file_batch_position`/`file_batch_size`, the
resolved base commit, limits, detailed ready/skipped rows, and summary counts.

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
