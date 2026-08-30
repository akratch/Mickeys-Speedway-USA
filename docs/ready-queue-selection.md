# Ready-queue selection modes

`tools/ready_queue.py` keeps its historical bounded-effort order unless a
coordinator explicitly passes `--selection`. The opt-in views answer two
different scheduling questions without weakening `lane_status.py`'s
fail-closed assignment verdict:

```sh
python3 tools/ready_queue.py --selection expected-yield
python3 tools/ready_queue.py --selection high-confidence
python3 tools/ready_queue.py --selection broad-structural
python3 tools/ready_queue.py --format maintenance
```

Only a `base-only` row is ever returned as assignable. Selection changes the
order or eligible evidence tier; it does not reinterpret `active`, exhausted,
stale-ledger, dirty-worktree, or missing-live-source states.

## Expected-yield score

The score is a deterministic 0–100 scheduling heuristic, not a match
probability or byte-exact evidence. It adds six visible components:

| Component | Points | Evidence |
|---|---:|---|
| exact candidate/target geometry | 20 | `size_delta == 0`; nonzero deltas receive at most 9 |
| residual ratio | 35 | differing words divided by the larger candidate/target geometry |
| mismatch mechanism | 20 | near-codegen classes lead broad structural classes |
| bounded function size | 15 | full through 64 words, then square-root decay |
| current source context | 5 | the ranking source digest must equal the selected base |
| relocation-masked measurement | 5 | masked residual when present; raw otherwise |

JSON schema version 5 exposes `expected_yield_score`, `residual_ratio`,
`residual_basis`, `exact_geometry`, `selection_class`, and
`selection_blockers` on every examined row. The non-default terminal and
Markdown views show the score, tier, residual, geometry, and mechanism.

`expected-yield` orders high-confidence rows first, then broad structural
rows, then stale rows requiring reproof. Within each tier, higher score wins;
the retained effort score and snapshot rank provide deterministic ties.

## Fail-closed tiers

A row is `high-confidence` only when all of these are true:

- its source-context measurement is present and fresh;
- candidate and target sizes are identical;
- relocation-masked differing-word evidence is present;
- its category is register-only, schedule-only, relocation-mismatch, or
  allocation-mismatch;
- its masked residual is at most 0.35;
- its expected-yield score is at least 70.

No amount of points can compensate for a missing predicate.
`--selection high-confidence` therefore returns an empty queue when the close
rows are already exhausted; it does not promote a large generic candidate to
fill `--top`. `--selection broad-structural` is the explicit view for current,
assignable rows outside that tier. Stale ranking evidence belongs to neither
view and must be remeasured first.

## Maintenance view

`--format maintenance` scans the complete selected prefix even after the
ready return limit is full. It records stale ranking reproofs, prose-only
plateaus, malformed or stale structured evidence, active lanes, dirty owning
paths, and retired ranking identities with one concrete next action each.
In particular:

- `ranking-reproof` calls for `nice -n 15 .venv/bin/python
  tools/nm_ranking.py --refresh-stale --jobs 2` before source edits;
- `prose-needs-remeasurement` calls for a configured baseline measurement
  followed by `tools/finalize_plateau.py`;
- `stale-structured-evidence` calls for reviewing the source marker and then
  refreshing its exact-symbol shard with
  `tools/plateau_handoff_audit.py --write`.

`summary.maintenance_scan_complete` says whether the selected rows were fully
examined. A deliberately smaller `--scan` remains a hard bound. These are
coordination actions, not matching claims; ADR 0001's object, relocation,
linked-range, and ROM proofs remain unchanged.
