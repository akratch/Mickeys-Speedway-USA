<!-- plateau-handoff:overlay1FindBestRecord:start -->
### `overlay1FindBestRecord` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 18/30 words
- frame: frameless
- relocations: 4
- first mismatch: +0x4
- summary: Five fresh forms stalled at 12 differing words; pool-position needs new selected-type/value web-formation evidence; prior exhausted routes remain closed.

#### ADR 0018 resume at the assigned base

The configured full translation unit was rebuilt before source edits. Its
120-byte, 30-word, frameless candidate differs at 12 raw and 12
relocation-masked words, first at +0x4. The owned interval is overlay 1 text
+0x7B64 through +0x7BDC, excluding the latter endpoint. No executable bytes
are newly matched. The best candidate remains guarded by `NON_MATCHING`.

Workbench diagnosis: `allocation-mismatch`, lever `pool-rotation`, routed to
`pool-position` and `forced-color-oracle`. The visible residual is one
selected-type/value pool-register bijection; the four temporary assignments
are exact. The prior caller-saved trace and IDO 5.3 law L83 make web formation
and numbering relevant, rather than a save-cost dial. The configured object
also passed a same-object workbench comparison. The refreshed near-match scan
found no useful donor: the highest masked four-gram similarity was 0.098.

Five distinct natural source attempts tested whether existing semantic values
could form the required allocation without the previously exhausted declaration
or scope changes. Each was compiled in the configured full TU and archived
with its source, object, score, first mismatch and rationale in ignored lane
artifacts:

1. Explicit countdown snapshot: 30 words, 12 raw/masked differences; byte-flat
   against baseline.
2. Direct loop-invariant global type access instead of the named cache:
   30 words, 15 raw/masked differences; changes the pool order and schedule,
   but leaves the value carrier in the wrong register.
3. Load the record value before the type filter: 30 words, 13 raw/masked
   differences; moves the load without obtaining the desired allocation.
4. Separate zero-value and ranked-value acceptance branches: 33 words,
   25 raw/masked positional differences; retains the wrong pool assignment
   and adds control-flow instructions.
5. Unsigned selected-type carrier, matching the existing equality conversion:
   30 words, 12 raw/masked differences; byte-flat against baseline.

Every attempt remains frameless and first differs at +0x4. Stopping evidence:
five consecutive attempts produced no better retained residual, new identity,
or decisive causal elimination for the remaining allocation mechanism. The
baseline is still the last best result; no combination or generic batch was
justified. This is a measured stall, not the earlier fixed-attempt-cap stop.
The original candidate was restored and remeasured after the source comment
update; it remains at the baseline score.

The candidate contains four static relocations. A fresh ROM-table decode
confirms four runtime LOCAL records, 884 through 887, at the candidate's exact
relative offsets and types. The assembled fallback exposes only the D_220
pair as ELF relocations; its D_1D88 pair is represented differently. Thus the
workbench metadata mismatch is separate from the twelve allocation sites.
No candidate proxy identity was newly bound or claimed exact. The prior
D_220/D_1D88 identity evidence and shared-record-type promotion requirement
remain applicable. No linked C or full-ROM C proof is claimed. The standalone
relocation comparison required an absent canonical ELF, so runtime counts and
offsets were checked directly using the existing ELF and overlay-table readers.

Next lever: source evidence for a different selected-type/value live-range
partition that changes caller-saved web numbering while preserving the current
hoist and loop schedule. Reopen only with changed evidence; do not repeat these
five forms, the 119 flags, prior allocator trace, three prior declaration/scope
forms, a generic permutation batch, or post-compile instruction editing.

Validation: configured `tools/wb_compare.sh --summary-json`, workbench diagnosis
and guides, same-object comparison, `skeleton_scan.py similar`, and direct runtime
relocation census completed. `tools/finalize_plateau.py` wrote the source/shard
metadata and passed `gmake cleanroom`, then stopped at `gmake check-docs`:
`plateau_handoff_audit.py` reports the unrelated `func_8000FAE0` shard stale.
Its source summary omits the final period present in its shard summary. Both
files are byte-identical to assignment base
`aaae09a635367cd68f10a1f6150a04d1ca69e055`; this lane did not modify them.
A second ordinary `gmake cleanroom check-docs` confirms the same result.
The plateau is preserved in a normal hook-checked lane commit, with this gate
failure explicitly outstanding rather than reported as passing. Canonical
function body and fallback selection are unchanged; no closing ROM rebuild was
needed for this report-only state. Coordinator action before integration:
reconcile the unrelated source/shard punctuation and rerun `gmake check-docs`.

<!-- plateau-handoff:overlay1FindBestRecord:end -->
