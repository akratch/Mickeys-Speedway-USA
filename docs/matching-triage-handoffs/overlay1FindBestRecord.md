<!-- plateau-handoff:overlay1FindBestRecord:start -->
### `overlay1FindBestRecord` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 28/30 words
- frame: frameless
- relocations: 2
- first mismatch: +0x4
- summary: schedule and all six colours exact; residual is the %hi fold into the load's own destination register; permuter flat 30min, five load spellings inert

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


#### tu2-o1tail: the emit order is source order, and an inert XOR unlocks the colour

The previous record named "separate emit-order evidence" as the next lever.
That evidence exists now, and it says the emit order was never separable:
uopt colours pool webs in the order of their first surviving definition in
source statement order, and ugen emits them in that same order, so schedule and
colour move together and cannot be traded against each other by reordering
statements. Measured both ways in this lane: with `group = D_1D88;` ahead of
`remaining = 31;` the schedule is exact and the two webs are swapped; with the
statements the other way round the colours are exact and the schedule is wrong.

Declaration order, `register`, and a dead store to `remaining` ahead of `group`
are each inert -- a dead store is eliminated before web numbering and reserves
no colour, whereas `value = 0`, which is likewise never read, does reserve one
(it is the a1 web, and dropping it shifts every later colour).

The lock breaks with an inert operation at the *use*, not at the definition:
`record->flags.bits.group == (group ^ 0)` keeps the statement order that gives
the exact schedule and restores both colours. The permuter found the XOR; the
operand order was found by hand, since writing it as `(group ^ 0) == field`
costs one further word at the comparison (law L67). Bitwise-or with zero,
addition of zero, masking with -1, and moving the inert operation either to the
definition or to the field side are all worse at 10 words; `(0 ^ group)`,
`(u32)` casts and the negated-inequality spelling are equivalent at 2.

Residual, 2 words: the target materialises the high half of D_1D88 into a3,
the load's own destination, while the candidate materialises it into a separate
colour (a1, shared with the `value` web) and loads into a3. One consistent web
substitution across both sites; the workbench reads it as
`uopt-coalescing-tie-break` and calls the lever unreachable from source.

Eliminated here, do not repeat: dereferencing the address of D_1D88, an s32
cast, and inert XOR/OR at the definition (all byte-flat at 2 words);
declaration permutation of `group` to three positions; `register` on `group`,
on `remaining`, and its removal from `value`; a dead `remaining = 0` before
`group`; `remaining = 31` written twice around `group`. A 30-minute permuter
run from this base, at score 20, found nothing.

Tooling note: `tools/permute.sh` cannot resolve this function by its friendly
name, because the TU carries twelve `GLOBAL_ASM` pragmas and the sole-fallback
rule needs exactly one. The run above was obtained by copying the splat-named
fallback to a friendly-named one under the gitignored `asm/` tree and renaming
the label inside it.

- first mismatch: +0x4
- summary: schedule and all six colours exact; residual is the %hi fold into the load's own destination register; permuter flat 30min, five load spellings inert
<!-- plateau-handoff:overlay1FindBestRecord:end -->
