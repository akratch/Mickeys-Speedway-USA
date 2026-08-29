# Ranked ready queue

`tools/ready_queue.py` gives a coordinator a deterministic ranked prefix of
targets that are safe to assign. It joins three independent checks:

1. `config/nonmatching-ranking.us.json` must pass the complete canonical
   validator in `tools/nm_ranking.py`.
2. The exact `(file, symbol)` identity must still be present in the live
   `NON_MATCHING` source queue discovered by `tools/permute_batch.py`.
3. `tools/lane_status.py` must classify the symbol as `base-only`. This is the
   only assignable state under ADR 0011.

The command is read-only. It reads source and committed Git objects; it does
not compile, inspect ROM text, or inspect sibling worktrees.

## Usage

```sh
python3 tools/ready_queue.py --scan 50 --top 10
python3 tools/ready_queue.py --scan 100 --top 20 --format markdown
python3 tools/ready_queue.py --scan 100 --top 20 --format json
```

`--scan` bounds how many rows of the retained `functions` ranking are
examined. `--top` bounds how many assignable rows are returned, and cannot
exceed `--scan`. Processing stops as soon as either bound is reached. The
defaults are 50 scanned rows and ten returned rows; hard limits are 1,000 and
100 respectively. Ranked rows keep their original order and rank number.

Unresolved ranking rows are reported as a count but are not invented into the
ranked order. Regenerate the ranking to give them measured positions.

Table output is intended for a terminal. Markdown is paste-ready for a queue
or handoff. JSON is schema version 1, uses stable field names, preserves row
order in arrays, and includes the resolved base commit, limits, detailed
ready/skipped rows, and summary counts.

## Fail-closed behavior

Rows classified `active`, `already-integrated/exhausted`, or `stale-ledger`
are skipped and summarized separately. A ranking identity no longer present
in the live queue is summarized as `not-live`; use the ranking prune or
regeneration flow to remove it.

If a symbol is live or committed under a source path that disagrees with its
ranked path, the entire command fails instead of guessing which identity was
intended. Malformed ranking documents, duplicate live identities, unknown
lane-status states, unreadable refs, and invalid bounds also fail closed.

A valid report with no assignable rows exits 1. Input or evidence errors exit
2. A report containing at least one assignable target exits 0.
