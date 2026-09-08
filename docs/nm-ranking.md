# tools/nm_ranking.py: ranking the NON_MATCHING queue by closeness

Hundreds of kilobytes of code across many overlays and several `src/main` TUs sit behind
`#ifdef NON_MATCHING` (the compile-only escape hatch documented in the
Makefile and `docs/acceleration-survey.md` sec.13.2): a real C body compiled
under `gmake NON_MATCHING=1`, but not yet byte-identical, so the tree still
ships the `#pragma GLOBAL_ASM` fallback under normal `gmake`. Some of those
candidates are one register swap from matching; others are structurally
wrong. This tool ranks every queued function by how close its candidate
already is, so a fleet of workers can spend its time on near-misses first
instead of triaging the whole queue by hand.

## Method

### Configured full-TU measurement

The primary measurement compiles each original translation unit once with the
expanded `NON_MATCHING=1` Makefile command. Only the object output path changes.
The complete ordered compiler arguments, including per-TU `-D` and `-I`,
source filename, relative include resolution, and source-line metadata remain
the inputs the build actually uses. Compilation runs only the raw
asm-processor/IDO command, excluding POSTPROCESS. It never touches source
timestamps and never reuses a previous object after a failed compile.

For each queued function, the tool extracts that symbol's span from this full
TU object and compares it with its own assembled `asm/nonmatchings/**/<f>.s`
target. A shared TU therefore costs one compile per refresh, while the scoring
still concerns one exact function identity. Sibling source changes invalidate
the TU's measurements because they can change its compilation context.

The former isolated-import path discarded flags outside a small codegen
subset; its historical-declaration fallback could compile a different source
context from the one recorded as fresh. Neither supplies current ranking rows
now. A failed configured TU or absent compiled symbol is an unresolved
measurement requiring repair, not permission to substitute an older source.

These are object-level measurements. A zero raw or relocation-masked word
count is not a canonical match declaration: ownership, relocation identity,
configured linked output, and ROM verification still govern promotion.

### objdiff-cli, as supplementary context

An optional `--objdiff-report` contributes per-function
`fuzzy_match_percent` from objdiff-cli. Its coverage depends on which build
objects the report contains and which ELF objects objdiff can parse. It never
replaces the configured compile measurement or proves freshness of the report
itself. Omit it to keep `objdiff_match_pct` null. The generated region records
the actual retained coverage. `tools/gen_objdiff_config.py --base-dir` can point
objdiff at the separate `build_non_matching/` tree.

### Word diff and categorization

For each resolved function, both `.text` sections are read as big-endian
32-bit words (raw, in memory only -- see "Clean-room note" below) up to
each side's own symbol size:

- **`size_bytes`**: the target (ROM) function's size.
- **`size_delta`**: `base size - target size`.
- **`differing_words`** and **`first_mismatch_offset`**: the backward-compatible
  raw byte comparison. The count covers disagreeing positions in the shared
  prefix plus every word past the shorter side's end; the offset identifies
  the first such position (or the shared length for a pure size difference).
- **`relocation_masked_differing_words`** and
  **`relocation_masked_first_mismatch_offset`**: the same positional evidence
  after masking only bits owned by a relocation on either object. The mask
  removes 26 payload bits for `R_MIPS_26`, 16 for the ordinary 16-bit
  relocation families, and all 32 for `R_MIPS_32`; opcode and register bits
  still have to agree. Unknown relocation kinds fail closed and mask nothing;
  missing or extra words remain mismatches. A `null` masked count means a
  retained schema-v1/v2 measurement has not yet been refreshed, never that it
  is exact.
- **`category`**, in this precedence order:
  1. **`size-mismatch`** -- `size_delta != 0`. Nothing else is checked;  a
     size mismatch means the two objects aren't even comparable word-for-word
     past the point of divergence.
  2. *(equal size, no differing words at all)* -- reported as `other` with
     `differing_words: 0`: the selected configured-TU symbol's raw words
     agree with the assembled target. This is an object-level observation;
     relocation identity and linked ROM ownership still require proof.
  3. **`schedule-only`** -- equal size, and the target's words are a
     permutation of the candidate's (`Counter(base_words) ==
     Counter(target_words)`, order differs). A same-instructions,
     different-order mismatch is exactly the shape IDO's scheduler is
     sensitive to (`docs/tools.md`'s own permuter case study describes a
     `perm_sameline` fix of this shape).
  4. **`register-only`** -- equal size, and every differing word decodes
     to the same opcode/function/immediate once register-select fields
     are masked out (R-type rs/rt/rd, or I-type rs/rt; J-type is left
     unmasked since it has no register fields to begin with). This is a
     coarse approximation -- COP1/COP0 fields share the R-type layout and
     are masked the same way, undistinguished from GPR swaps -- but it's
     conservative in the direction that matters: it only ever
     under-counts `register-only`, never mis-labels a real semantic
     difference as one.
  5. **`reloc-mismatch`** -- equal size, at least one raw word differs, and
     masking the union of base/target linker-owned fields removes every
     difference. An opcode or register difference at a relocation site
     remains visible and therefore cannot receive this category.
  6. **`other`** -- equal size, everything else. This is the largest
     bucket in this run and is exactly the "needs a human/model
     look, no cheap mechanical explanation available" category.

Sort order for the table and the JSON: category rank (`register-only` <
`schedule-only` < `other` < `reloc-mismatch` < `size-mismatch`), then
relocation-masked differing words ascending within a category, then raw
`differing_words`. A legacy row with no masked measurement uses its raw count
for the masked sort position. Categories describe residual bytes, not expected
cost or source reachability. Exhausted register/schedule near-matches may need
a new measured lever; a size mismatch may be a small boundary problem. Read
current attempt history and the assignment gate before choosing another pass.

### Clean-room note

No instruction word, mnemonic, or hex byte from a decoded `.text` section
is ever written to `config/nonmatching-ranking.us.json`, printed, or
otherwise leaves the running process -- every decode exists only long
enough to produce raw and relocation-masked counts or first-mismatch offsets
before being discarded. `objdiff_match_pct` is a float already computed by
objdiff-cli, not ROM content.

## Reproducing this run

Before selecting by expected yield, audit the complete live queue with
`tools/nm_ranking.py --check-freshness` (add `--json` for exact identity lists).
This gate runs no compiler and fails on missing, retired, unresolved, or stale
rows. Context version 5 binds each measurement to its complete original TU,
expanded Makefile compile command, compiler and preparation-tool contents,
transitive literal headers, pragma assembly (including literal nested assembly
includes), literal command-file operands, and extracted target. It requires the configured
tools and extracted assembly. Source/header text retains comments and blank
lines because __LINE__ and IDO source-line metadata can affect output. Inactive
include branches are conservatively included, and macro includes fail closed.
Missing literal includes in inactive SDK branches are recorded as absent; if a
file appears at a searched location, the context changes. Unrelated headers
outside the include closure do not invalidate a TU. Updating a tool invalidates
all affected measurements, even when source text is unchanged.

The retained ranking is the refresh cache: exact full-context matches skip
compilation. Older context versions need a one-time remeasurement. Both
before and after a refresh, the tool recomputes commands, tools, headers,
targets, and live source membership; changed inputs abort publication. Recipe
expansion batches every distinct TU into one make dry run. Header parsing is
shared within each context pass. Neither operation touches source timestamps.

`tools/ready_queue.py --selection expected-yield` and `--selection
high-confidence` require this complete source coverage before applying focus,
scan, or top limits. Default selection remains available for maintenance and
reports coverage explicitly; `--format maintenance` includes every unranked
identity. Run `tools/nm_ranking.py --refresh-stale` to compile all changed, new,
and unresolved identities and remove retired rows. Pruning alone cannot prove
that the remaining measurements cover the current queue.

```sh
gmake extract
gmake -j$(sysctl -n hw.ncpu)
gmake verify
./tools/make_expected.sh
gmake NON_MATCHING=1 -k -j$(sysctl -n hw.ncpu)   # -k: some POSTPROCESS objects will fail, see above

# objdiff-cli report against build_non_matching/, with the standard
# POSTPROCESS exclusion (tools/objdiff_report.sh's own approach, pointed at
# the NON_MATCHING tree instead of build/):
grep -oE '^\$\(BUILD_DIR\)/\$\(SRC_DIR\)/[A-Za-z0-9_/]+\.c\.o: POSTPROCESS' \
    Makefile | sed -E 's#\$\(BUILD_DIR\)/\$\(SRC_DIR\)/#src/#; s/: POSTPROCESS$//' \
    | sort -u > tools/objdiff_exclude.txt
.venv/bin/python tools/gen_objdiff_config.py --base-dir build_non_matching > objdiff.json
tools/objdiff/objdiff-cli report generate -p . -o /tmp/nm_report.json -f json -d

# restore objdiff.json to the normal build/ tree afterward:
.venv/bin/python tools/gen_objdiff_config.py > objdiff.json

# the ranking itself:
.venv/bin/python tools/nm_ranking.py --jobs 12 \
    --out config/nonmatching-ranking.us.json --no-table
# Optional linked/whole-TU context:
.venv/bin/python tools/nm_ranking.py --objdiff-report /tmp/nm_report.json --jobs 12
.venv/bin/python tools/nm_ranking.py --show-retained --top 20 --markdown  # fleet-prompt excerpt; no compilation

# Fast maintenance between full compile passes: remove snapshot rows whose
# exact source/symbol identity is no longer guarded by NON_MATCHING.
.venv/bin/python tools/nm_ranking.py --prune-stale

# Incremental maintenance: retain source-proven rows, compile only changed,
# newly queued, or previously unresolved identities, then merge and re-sort.
.venv/bin/python tools/nm_ranking.py --refresh-stale --jobs 2

# A bounded checkpoint compiles at most five stale identities. Deferred old
# measurements stay explicitly unproven and deferred new identities stay in
# the unresolved list; rerun without --limit to finish the refresh.
.venv/bin/python tools/nm_ranking.py --refresh-stale --limit 5 --jobs 2

# Regenerate or verify the marked human-readable snapshot without compiling.
.venv/bin/python tools/nm_ranking.py --write-doc
.venv/bin/python tools/nm_ranking.py --check-doc
```

A complete pass can run long enough for another lane to promote functions
that were queued at startup. Immediately before publishing, the ranking tool
re-scans the canonical source and drops resolved and unresolved rows whose
`#ifdef NON_MATCHING` block no longer exists. The checked-in JSON is still a
historical snapshot after the ranking process exits; consumers that need the
current queue must intersect it with a fresh source scan. `--prune-stale`
performs that intersection in place without invoking the compiler or requiring
decomp-permuter. It keys every row by the exact `(source file, symbol)` pair,
validates that resolved and unresolved identities are unique, writes the JSON
atomically, and refuses malformed unresolved rows rather than guessing which
function they describe. It only removes stale rows and normalizes the retained
counts: newly added `NON_MATCHING` functions remain unranked and are reported.

`--refresh-stale` closes that gap without replaying the whole queue. Each
resolved schema-v3 row records a SHA-256 of the configured full-TU inputs
described above. The digest uses unpadded base64url split
into four-character groups, so the clean-room scanner cannot mistake a dense
table of hexadecimal digests for machine words. Shared declarations, macros,
local data, matched code, every candidate body, and physical source lines remain
covered. The refresh validates the complete retained
document, discovers the live exact `(file, symbol)` queue, and classifies rows
as follows:

- an embedded digest equal to current context is retained without compilation;
- older source-only rows require remeasurement before carrying a current
  configured-input receipt;
- changed rows, unresolved rows, and newly queued identities are compiled via
  the ordinary `process_item` path;
- identities no longer live are removed.

Schema v3 adds relocation-masked evidence and a derived coverage count.
Schema-v1/v2 documents remain readable. Missing masked evidence is itself a
bounded-refresh reason, so repeated incremental refreshes converge to full
coverage. A source-proven old row deferred by `--limit` retains its valid raw
measurement/context and receives explicit `null` masked fields; real masked
values appear only after that row is compiled, never by inference.

The tool re-discovers and re-hashes the complete queue after compilation. Any
membership/context race or selected per-item error exits nonzero before the
JSON or generated documentation is replaced. A bounded `--limit` applies only
to compile work: deferred stale measurements remain present without a context
digest, and deferred new identities are added to `unresolved_functions`, so
the snapshot still covers the complete live queue without claiming those rows
are current. The canonical JSON and marked documentation are fully rendered
before their atomic replacements. A supplementary `--objdiff-report` applies
to newly measured rows; omitted refreshes keep proven-fresh retained values and
leave refreshed values null.

Full canonical ranking writes and canonical `--prune-stale` and
`--refresh-stale` runs update the marked region below in the same invocation.
`--write-doc` is the source-only repair command; `--check-doc` validates the complete JSON schema and exact
`(file, symbol)` identity uniqueness before comparing the rendered region.
`gmake check-docs` runs that check, so edited prose, category summaries, ranked
rows, and unresolved tables cannot silently diverge from the persisted JSON.
Text outside the markers remains authored and is preserved byte-for-byte.

<!-- NM_RANKING_GENERATED_BEGIN -->
## Current generated ranking snapshot

> Generated by `tools/nm_ranking.py --write-doc` from
> `config/nonmatching-ranking.us.json`. Do not edit this region by hand;
> `--check-doc` and `gmake check-docs` fail on any drift.

The snapshot contains **433 queued identities**: **433 resolved measurements** and **0 unresolved identities**. Resolved target size totals **503,368 bytes (491.6 KiB)**.

Resolved rows span **72 overlays** and **2 resident TU groups** (`libultra`, `main`).

A supplementary objdiff report was not supplied; `objdiff_match_pct` covers **0 / 433** resolved rows.

Persisted configured-TU input evidence covers **433 / 433** resolved rows. Rows without it are retained legacy or bounded-refresh measurements and must be treated as requiring reproof.

Relocation-masked mismatch evidence covers **433 / 433** resolved rows. The raw count preserves literal object differences; the masked count removes only known linker-owned fields to expose the remaining code-generation mismatch. Neither replaces linked byte-identity proof.

### Category distribution

| Category | Count | Share of resolved |
|---|---:|---:|
| `register-only` | 21 | 4.8% |
| `schedule-only` | 1 | 0.2% |
| `other` | 199 | 46.0% |
| `reloc-mismatch` | 1 | 0.2% |
| `size-mismatch` | 211 | 48.7% |

### Differing-word thresholds

| Threshold | Count |
|---|---:|
| raw `differing_words <= 5` | 18 |
| relocation-masked `differing_words <= 5` | 21 |
| raw `differing_words <= 10` | 38 |
| relocation-masked `differing_words <= 10` | 39 |
| raw `differing_words <= 20` | 66 |
| relocation-masked `differing_words <= 20` | 67 |

### Complete ranked queue

Rank is the persisted `functions` array order. The exact identity is
(`file`, `symbol`), so repeated symbol spellings in different translation
units remain distinct.

| Rank | File | Symbol | Overlay/TU | Category | Target bytes | Raw diff | Masked diff | Raw first | Masked first | Size delta | Objdiff% |
|---:|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|
| 1 | `src/main/menu.c` | `func_8003A2C8` | `main` | `register-only` | 128 | 1 | 1 | 20 | 20 | 0 | — |
| 2 | `src/overlays/o074/overlay74Update.c` | `overlay74Update` | `o074` | `register-only` | 400 | 1 | 1 | 292 | 292 | 0 | — |
| 3 | `src/main/objects.c` | `func_800056A4` | `main` | `register-only` | 76 | 2 | 2 | 36 | 36 | 0 | — |
| 4 | `src/overlays/o020/overlay20RemoveEntry.c` | `overlay20RemoveEntry` | `o020` | `register-only` | 212 | 2 | 2 | 108 | 108 | 0 | — |
| 5 | `src/main/level.c` | `levelFreeAll` | `main` | `register-only` | 468 | 3 | 3 | 316 | 316 | 0 | — |
| 6 | `src/main/objects.c` | `func_80004454` | `main` | `register-only` | 316 | 3 | 3 | 80 | 80 | 0 | — |
| 7 | `src/main/objects.c` | `func_8000471C` | `main` | `register-only` | 320 | 3 | 3 | 80 | 80 | 0 | — |
| 8 | `src/overlays/o040/overlay40FadeRecords.c` | `overlay40FadeRecords` | `o040` | `register-only` | 404 | 3 | 3 | 12 | 12 | 0 | — |
| 9 | `src/main/fx.c` | `func_800498FC` | `main` | `register-only` | 400 | 9 | 9 | 208 | 208 | 0 | — |
| 10 | `src/overlays/o019/overlay19ClassifyEdge.c` | `overlay19ClassifyEdge` | `o019` | `register-only` | 480 | 10 | 10 | 312 | 312 | 0 | — |
| 11 | `src/main/saves.c` | `func_8002C69C` | `main` | `register-only` | 112 | 11 | 11 | 16 | 16 | 0 | — |
| 12 | `src/main/charControl.c` | `func_8001D960` | `main` | `register-only` | 880 | 16 | 16 | 356 | 356 | 0 | — |
| 13 | `src/main/models.c` | `func_80020D8C` | `main` | `register-only` | 192 | 17 | 17 | 56 | 56 | 0 | — |
| 14 | `src/main/track.c` | `func_8000FAE0` | `main` | `register-only` | 248 | 20 | 20 | 28 | 28 | 0 | — |
| 15 | `src/main/particles.c` | `func_80041CE4` | `main` | `register-only` | 612 | 27 | 27 | 72 | 72 | 0 | — |
| 16 | `src/overlays/o019/overlay19BuildAdjacency.c` | `overlay19BuildAdjacency` | `o019` | `register-only` | 492 | 41 | 41 | 124 | 124 | 0 | — |
| 17 | `src/overlays/o022/overlay22RemoveObject.c` | `func_overlay_022_F0000D30_1878E38` | `o022` | `register-only` | 364 | 43 | 43 | 16 | 16 | 0 | — |
| 18 | `src/overlays/o016/overlay_016.c` | `overlay16ApplyGradient` | `o016` | `register-only` | 580 | 60 | 60 | 60 | 60 | 0 | — |
| 19 | `src/main/particles.c` | `func_8003D25C` | `main` | `register-only` | 672 | 68 | 68 | 80 | 80 | 0 | — |
| 20 | `src/main/particles.c` | `func_8003D4FC` | `main` | `register-only` | 4,272 | 73 | 73 | 720 | 720 | 0 | — |
| 21 | `src/overlays/o057/overlay57UpdateModeState.c` | `overlay57UpdateModeState` | `o057` | `register-only` | 1,416 | 87 | 87 | 220 | 220 | 0 | — |
| 22 | `src/overlays/o001/overlay_001_head.c` | `overlay1MeasureCurves` | `o001` | `schedule-only` | 316 | 27 | 27 | 12 | 12 | 0 | — |
| 23 | `src/main/particles.c` | `func_8003E8D8` | `main` | `other` | 560 | 1 | 1 | 56 | 56 | 0 | — |
| 24 | `src/overlays/o097/overlay97InitScale.c` | `overlay97InitScale` | `o097` | `other` | 576 | 1 | 1 | 208 | 208 | 0 | — |
| 25 | `src/overlays/o001/overlay_001_middle.c` | `overlay1FindNextAngle` | `o001` | `other` | 200 | 3 | 2 | 56 | 60 | 0 | — |
| 26 | `src/overlays/o001/overlay_001_middle.c` | `overlay1FindPreviousAngle` | `o001` | `other` | 200 | 3 | 2 | 56 | 60 | 0 | — |
| 27 | `src/overlays/o007/overlay_007_tail.c` | `overlay7DispatchSelection` | `o007` | `other` | 240 | 5 | 2 | 4 | 4 | 0 | — |
| 28 | `src/overlays/o001/overlay_001_head.c` | `overlay1InterpolatePath` | `o001` | `other` | 332 | 7 | 2 | 8 | 148 | 0 | — |
| 29 | `src/main/diCpu.c` | `func_80045BBC` | `main` | `other` | 240 | 8 | 2 | 24 | 188 | 0 | — |
| 30 | `src/overlays/o011/overlay11UpdateMenu.c` | `overlay11UpdateMenu` | `o011` | `other` | 1,204 | 16 | 2 | 20 | 312 | 0 | — |
| 31 | `src/overlays/o009/overlay_009.c` | `func_overlay_009_F0000CE4_186735C` | `o009` | `other` | 648 | 5 | 4 | 200 | 200 | 0 | — |
| 32 | `src/overlays/o022/overlay22InitializeObject.c` | `func_overlay_022_F0000000_1878108` | `o022` | `other` | 688 | 5 | 5 | 204 | 204 | 0 | — |
| 33 | `src/overlays/o041/overlay41AddSlot.c` | `func_overlay_041_F0001650_1888988` | `o041` | `other` | 220 | 6 | 6 | 16 | 16 | 0 | — |
| 34 | `src/overlays/o033/overlay33InitializeBuffers.c` | `overlay33InitializeBuffers` | `o033` | `other` | 324 | 8 | 6 | 4 | 116 | 0 | — |
| 35 | `src/main/diprint.c` | `debug_text_width` | `main` | `other` | 264 | 7 | 7 | 56 | 56 | 0 | — |
| 36 | `src/overlays/o038/func_overlay_038_F0000000_1885D10.c` | `func_overlay_038_F0000000_1885D10` | `o038` | `other` | 340 | 7 | 7 | 72 | 72 | 0 | — |
| 37 | `src/overlays/o036/overlay36CheckNearbyHeight.c` | `func_overlay_036_F0000818_1883CD0` | `o036` | `other` | 252 | 8 | 7 | 0 | 0 | 0 | — |
| 38 | `src/overlays/o062/overlay62Update.c` | `overlay62Update` | `o062` | `other` | 1,176 | 8 | 7 | 68 | 68 | 0 | — |
| 39 | `src/overlays/o009/overlay_009.c` | `func_overlay_009_F0000540_1866BB8` | `o009` | `other` | 516 | 9 | 8 | 76 | 76 | 0 | — |
| 40 | `src/overlays/o001/overlay_001_end.c` | `overlay1ResolvePathPoint` | `o001` | `other` | 608 | 10 | 8 | 144 | 144 | 0 | — |
| 41 | `src/main/audiomgr.c` | `func_80002134` | `main` | `other` | 84 | 9 | 9 | 24 | 24 | 0 | — |
| 42 | `src/main/models_5B300.c` | `func_8005A948` | `main` | `other` | 376 | 9 | 9 | 64 | 64 | 0 | — |
| 43 | `src/overlays/o043/overlay43ComputeMotion.c` | `func_overlay_043_F00010A8_188B078` | `o043` | `other` | 220 | 9 | 9 | 116 | 116 | 0 | — |
| 44 | `src/main/font.c` | `func_8004BA8C` | `main` | `other` | 184 | 10 | 10 | 24 | 24 | 0 | — |
| 45 | `src/main/main.c` | `func_80028FCC` | `main` | `other` | 108 | 10 | 10 | 28 | 28 | 0 | — |
| 46 | `src/main/models_5B300.c` | `func_8005A7A0` | `main` | `other` | 424 | 10 | 10 | 0 | 0 | 0 | — |
| 47 | `src/main/textures_35024.c` | `func_8003484C` | `main` | `other` | 84 | 10 | 10 | 8 | 8 | 0 | — |
| 48 | `src/overlays/o068/overlay68CheckKind.c` | `overlay68CheckKind` | `o068` | `other` | 320 | 10 | 10 | 80 | 80 | 0 | — |
| 49 | `src/main/menu_3B1A0.c` | `func_8003A754` | `main` | `other` | 124 | 12 | 12 | 16 | 16 | 0 | — |
| 50 | `src/overlays/o001/overlay_001_tail.c` | `overlay1FindBestRecord` | `o001` | `other` | 120 | 12 | 12 | 4 | 4 | 0 | — |
| 51 | `src/overlays/o001/overlay_001.c` | `overlay1FindPreviousUsable` | `o001` | `other` | 160 | 14 | 12 | 4 | 20 | 0 | — |
| 52 | `src/overlays/o015/overlay_015.c` | `overlay15DrawScreenStars` | `o015` | `other` | 420 | 14 | 13 | 52 | 56 | 0 | — |
| 53 | `src/overlays/o031/overlay31CreatePool.c` | `overlay31CreatePool` | `o031` | `other` | 200 | 14 | 13 | 0 | 0 | 0 | — |
| 54 | `src/overlays/o015/overlay_015.c` | `overlay15DrawRain` | `o015` | `other` | 216 | 18 | 13 | 8 | 116 | 0 | — |
| 55 | `src/main/fx.c` | `func_8004ACC4` | `main` | `other` | 112 | 14 | 14 | 20 | 20 | 0 | — |
| 56 | `src/main/memory.c` | `func_8002B524` | `main` | `other` | 464 | 14 | 14 | 224 | 224 | 0 | — |
| 57 | `src/overlays/o014/overlay14CreateValue.c` | `overlay14CreateValue` | `o014` | `other` | 384 | 15 | 14 | 84 | 164 | 0 | — |
| 58 | `src/overlays/o029/overlay29InitializeObject.c` | `func_overlay_029_F000042C_187D6DC` | `o029` | `other` | 408 | 15 | 15 | 20 | 20 | 0 | — |
| 59 | `src/overlays/o041/overlay41UpdateColorRecords.c` | `func_overlay_041_F0000124_188745C` | `o041` | `other` | 392 | 20 | 15 | 48 | 64 | 0 | — |
| 60 | `src/overlays/o001/overlay_001_tail.c` | `overlay1InitializeGaugeObjects` | `o001` | `other` | 296 | 16 | 16 | 48 | 48 | 0 | — |
| 61 | `src/overlays/o017/overlay17DrawStrip.c` | `overlay17DrawStrip` | `o017` | `other` | 476 | 16 | 16 | 0 | 0 | 0 | — |
| 62 | `src/overlays/o043/overlay43FilterImage.c` | `overlay43FilterImage` | `o043` | `other` | 172 | 16 | 16 | 4 | 4 | 0 | — |
| 63 | `src/overlays/o007/overlay_007.c` | `overlay7AcquireEntry` | `o007` | `other` | 384 | 17 | 16 | 8 | 260 | 0 | — |
| 64 | `src/main/anim.c` | `func_8005716C` | `main` | `other` | 320 | 18 | 18 | 84 | 84 | 0 | — |
| 65 | `src/overlays/o001/overlay_001.c` | `overlay1FindType47ByAngle` | `o001` | `other` | 296 | 19 | 18 | 8 | 8 | 0 | — |
| 66 | `src/main/menu_3B1A0.c` | `func_8003A5A0` | `main` | `other` | 224 | 19 | 19 | 0 | 0 | 0 | — |
| 67 | `src/main/objects.c` | `func_800069E8` | `main` | `other` | 284 | 19 | 19 | 140 | 140 | 0 | — |
| 68 | `src/overlays/o027/overlay_027.c` | `overlay27UpdateCoordinates` | `o027` | `other` | 260 | 19 | 19 | 0 | 0 | 0 | — |
| 69 | `src/overlays/o075/overlay75UpdateMovingObject.c` | `overlay75UpdateMovingObject` | `o075` | `other` | 1,216 | 19 | 19 | 84 | 84 | 0 | — |
| 70 | `src/overlays/o059/overlay59Advance.c` | `overlay59Advance` | `o059` | `other` | 1,048 | 22 | 19 | 4 | 4 | 0 | — |
| 71 | `src/main/particles.c` | `func_8003F154` | `main` | `other` | 1,188 | 21 | 21 | 524 | 524 | 0 | — |
| 72 | `src/overlays/o003/overlay3SelectScoredObject.c` | `overlay3SelectScoredObject` | `o003` | `other` | 472 | 21 | 21 | 72 | 72 | 0 | — |
| 73 | `src/overlays/o001/overlay_001_tail.c` | `overlay1AppendPathPoint` | `o001` | `other` | 432 | 22 | 21 | 32 | 32 | 0 | — |
| 74 | `src/overlays/o033/overlay33PresentAndSwap.c` | `overlay33PresentAndSwap` | `o033` | `other` | 156 | 22 | 21 | 16 | 16 | 0 | — |
| 75 | `src/overlays/o060/overlay60Initialize.c` | `func_overlay_060_F0000000_18B9DD8` | `o060` | `other` | 820 | 46 | 21 | 124 | 124 | 0 | — |
| 76 | `src/main/fx.c` | `func_80048760` | `main` | `other` | 484 | 23 | 23 | 8 | 8 | 0 | — |
| 77 | `src/overlays/o001/overlay_001_tail.c` | `overlay1UpdateAimedTransient` | `o001` | `other` | 996 | 32 | 23 | 0 | 0 | 0 | — |
| 78 | `src/overlays/o057/overlay57UpdateModeTrigger.c` | `overlay57UpdateModeTrigger` | `o057` | `other` | 376 | 34 | 23 | 16 | 56 | 0 | — |
| 79 | `src/overlays/o005/overlay_005.c` | `overlay5InitializeAudio` | `o005` | `other` | 932 | 24 | 24 | 0 | 0 | 0 | — |
| 80 | `src/main/models.c` | `func_80020E4C` | `main` | `other` | 452 | 25 | 25 | 12 | 12 | 0 | — |
| 81 | `src/overlays/o001/overlay_001_tail.c` | `overlay1BendPathPoint` | `o001` | `other` | 428 | 25 | 25 | 12 | 12 | 0 | — |
| 82 | `src/overlays/o096/overlay96Unregister.c` | `overlay96Unregister` | `o096` | `other` | 136 | 25 | 25 | 0 | 0 | 0 | — |
| 83 | `src/main/fx.c` | `func_8004AF68` | `main` | `other` | 208 | 26 | 26 | 16 | 16 | 0 | — |
| 84 | `src/overlays/o001/overlay_001_middle.c` | `overlay1AdvanceGauge` | `o001` | `other` | 168 | 26 | 26 | 20 | 20 | 0 | — |
| 85 | `src/overlays/o092/overlay92FindNearestCourse.c` | `func_overlay_092_F0000068_18D5F88` | `o092` | `other` | 672 | 26 | 26 | 4 | 4 | 0 | — |
| 86 | `src/main/objects.c` | `func_80006448` | `main` | `other` | 236 | 27 | 27 | 28 | 28 | 0 | — |
| 87 | `src/overlays/o002/func_overlay_002_F0001A94_185888C.c` | `func_overlay_002_F0001A94_185888C` | `o002` | `other` | 868 | 51 | 27 | 76 | 312 | 0 | — |
| 88 | `src/main/block_4F4E0.c` | `func_8004EED0` | `main` | `other` | 336 | 28 | 28 | 168 | 168 | 0 | — |
| 89 | `src/main/objects.c` | `func_80008028` | `main` | `other` | 240 | 28 | 28 | 36 | 36 | 0 | — |
| 90 | `src/main/charControl.c` | `func_8001D880` | `main` | `other` | 144 | 29 | 29 | 4 | 4 | 0 | — |
| 91 | `src/overlays/o041/overlay41EnqueueTransition.c` | `func_overlay_041_F000195C_1888C94` | `o041` | `other` | 420 | 29 | 29 | 8 | 8 | 0 | — |
| 92 | `src/overlays/o034/overlay34CreateRecord.c` | `overlay34CreateRecord` | `o034` | `other` | 500 | 32 | 30 | 0 | 0 | 0 | — |
| 93 | `src/overlays/o001/overlay_001_tail.c` | `overlay1ConsumeNearbyPending` | `o001` | `other` | 276 | 31 | 31 | 64 | 64 | 0 | — |
| 94 | `src/overlays/o001/overlay_001_tail.c` | `overlay1UpdateRangeFlags` | `o001` | `other` | 480 | 31 | 31 | 160 | 160 | 0 | — |
| 95 | `src/overlays/o098/overlay98CollectUniqueY.c` | `overlay98CollectUniqueY` | `o098` | `other` | 324 | 32 | 32 | 104 | 104 | 0 | — |
| 96 | `src/overlays/o029/overlay29HandleEffects.c` | `func_overlay_029_F00010C4_187E374` | `o029` | `other` | 1,028 | 33 | 33 | 124 | 124 | 0 | — |
| 97 | `src/overlays/o071/func_overlay_071_F0000870_18CA390.c` | `func_overlay_071_F0000870_18CA390` | `o071` | `other` | 728 | 33 | 33 | 112 | 112 | 0 | — |
| 98 | `src/main/saves.c` | `packInit` | `main` | `other` | 460 | 34 | 34 | 160 | 160 | 0 | — |
| 99 | `src/overlays/o003/overlay3RunCachedModeAction.c` | `overlay3RunCachedModeAction` | `o003` | `other` | 452 | 34 | 34 | 132 | 132 | 0 | — |
| 100 | `src/overlays/o041/overlay41UpdateProgress.c` | `func_overlay_041_F0001298_18885D0` | `o041` | `other` | 460 | 34 | 34 | 232 | 232 | 0 | — |
| 101 | `src/overlays/o060/overlay60ReassignChoiceSlots.c` | `overlay60ReassignChoiceSlots` | `o060` | `other` | 212 | 35 | 35 | 4 | 4 | 0 | — |
| 102 | `src/overlays/o098/overlay98CheckObject.c` | `overlay98CheckObject` | `o098` | `other` | 444 | 35 | 35 | 56 | 56 | 0 | — |
| 103 | `src/main/charControl.c` | `func_8001BBB4` | `main` | `other` | 600 | 38 | 38 | 0 | 0 | 0 | — |
| 104 | `src/overlays/o101/overlay101BuildBorder.c` | `overlay101BuildBorder` | `o101` | `other` | 316 | 38 | 38 | 68 | 68 | 0 | — |
| 105 | `src/main/main.c` | `func_80029274` | `main` | `other` | 348 | 39 | 39 | 8 | 8 | 0 | — |
| 106 | `src/overlays/o002/overlay2QueryNode.c` | `overlay2QueryNode` | `o002` | `other` | 1,012 | 52 | 40 | 64 | 88 | 0 | — |
| 107 | `src/main/track.c` | `func_80010900` | `main` | `other` | 588 | 41 | 41 | 20 | 20 | 0 | — |
| 108 | `src/overlays/o073/overlay73Draw.c` | `func_overlay_073_F0000D70_18CB830` | `o073` | `other` | 312 | 41 | 41 | 28 | 28 | 0 | — |
| 109 | `src/main/objects.c` | `func_80007C68` | `main` | `other` | 472 | 42 | 42 | 124 | 124 | 0 | — |
| 110 | `src/overlays/o008/overlay_008.c` | `func_overlay_008_F0004CF0_1862A48` | `o008` | `other` | 1,080 | 48 | 43 | 376 | 376 | 0 | — |
| 111 | `src/overlays/o033/overlay33BuildDisplayList.c` | `overlay33BuildDisplayList` | `o033` | `other` | 1,232 | 46 | 46 | 920 | 920 | 0 | — |
| 112 | `src/main/matrix.c` | `MatrixMultiplyVec4` | `main` | `other` | 212 | 47 | 47 | 0 | 0 | 0 | — |
| 113 | `src/overlays/o029/overlay29DrawGroups.c` | `overlay29DrawGroups` | `o029` | `other` | 516 | 47 | 47 | 76 | 76 | 0 | — |
| 114 | `src/overlays/o001/overlay_001.c` | `overlay1ActivateObject` | `o001` | `other` | 352 | 50 | 47 | 0 | 0 | 0 | — |
| 115 | `src/main/joy.c` | `joyRead` | `main` | `other` | 636 | 49 | 48 | 24 | 24 | 0 | — |
| 116 | `src/overlays/o008/overlay_008.c` | `func_overlay_008_F0000894_185E5EC` | `o008` | `other` | 1,524 | 57 | 48 | 204 | 1,064 | 0 | — |
| 117 | `src/overlays/o099/overlay99RenderSortedEntries.c` | `overlay99RenderSortedEntries` | `o099` | `other` | 932 | 51 | 49 | 0 | 0 | 0 | — |
| 118 | `src/overlays/o017/overlay17AdvanceChain.c` | `overlay17AdvanceChain` | `o017` | `other` | 588 | 51 | 51 | 24 | 24 | 0 | — |
| 119 | `src/overlays/o014/overlay14LoadRelocatedValue.c` | `overlay14LoadRelocatedValue` | `o014` | `other` | 376 | 53 | 52 | 4 | 8 | 0 | — |
| 120 | `src/overlays/o009/overlay_009.c` | `func_overlay_009_F00010B4_186772C` | `o009` | `other` | 1,128 | 59 | 52 | 136 | 136 | 0 | — |
| 121 | `src/main/audio_manager_36D0.c` | `func_80003480` | `main` | `other` | 376 | 53 | 53 | 0 | 0 | 0 | — |
| 122 | `src/main/charControl.c` | `func_8001CB84` | `main` | `other` | 1,820 | 53 | 53 | 436 | 436 | 0 | — |
| 123 | `src/overlays/o046/overlay46UpdateSequence.c` | `func_overlay_046_F0000120_188E518` | `o046` | `other` | 1,268 | 70 | 54 | 4 | 120 | 0 | — |
| 124 | `src/overlays/o008/overlay_008.c` | `func_overlay_008_F000291C_1860674` | `o008` | `other` | 1,444 | 60 | 56 | 0 | 0 | 0 | — |
| 125 | `src/overlays/o089/overlay89InitializeEffect.c` | `overlay89InitializeEffect` | `o089` | `other` | 820 | 58 | 58 | 64 | 64 | 0 | — |
| 126 | `src/overlays/o035/overlay35BuildGridMasks.c` | `func_overlay_035_F0000770_1882450` | `o035` | `other` | 976 | 59 | 59 | 44 | 44 | 0 | — |
| 127 | `src/main/spranim.c` | `effectboxControl` | `main` | `other` | 772 | 60 | 60 | 72 | 72 | 0 | — |
| 128 | `src/overlays/o057/overlay57Draw32A0.c` | `overlay57Draw32A0` | `o057` | `other` | 832 | 63 | 60 | 0 | 0 | 0 | — |
| 129 | `src/main/objects.c` | `func_8000BB84` | `main` | `other` | 260 | 61 | 61 | 0 | 0 | 0 | — |
| 130 | `src/main/diCpu.c` | `render_epc_lock_up_display` | `main` | `other` | 1,376 | 62 | 62 | 324 | 324 | 0 | — |
| 131 | `src/overlays/o101/overlay101DrawTransformed.c` | `overlay101DrawTransformed` | `o101` | `other` | 664 | 62 | 62 | 124 | 124 | 0 | — |
| 132 | `src/overlays/o008/overlay_008.c` | `func_overlay_008_F0002640_1860398` | `o008` | `other` | 732 | 63 | 62 | 184 | 184 | 0 | — |
| 133 | `src/overlays/o027/overlay_027.c` | `func_overlay_027_F0000064_187BA3C` | `o027` | `other` | 1,472 | 63 | 62 | 24 | 24 | 0 | — |
| 134 | `src/main/objects.c` | `func_80006FA0` | `main` | `other` | 376 | 63 | 63 | 12 | 12 | 0 | — |
| 135 | `src/overlays/o019/overlay19BuildSpatialMasks.c` | `overlay19BuildSpatialMasks` | `o019` | `other` | 908 | 63 | 63 | 88 | 88 | 0 | — |
| 136 | `src/main/saves.c` | `rumbleTick` | `main` | `other` | 1,372 | 64 | 64 | 244 | 244 | 0 | — |
| 137 | `src/overlays/o014/overlay14PrepareInputState.c` | `overlay14PrepareInputState` | `o014` | `other` | 524 | 67 | 65 | 52 | 52 | 0 | — |
| 138 | `src/main/menu.c` | `func_80038878` | `main` | `other` | 340 | 66 | 66 | 20 | 20 | 0 | — |
| 139 | `src/main/fx.c` | `func_80049000` | `main` | `other` | 596 | 69 | 69 | 0 | 0 | 0 | — |
| 140 | `src/overlays/o058/overlay58DrawLargePointQuad.c` | `overlay58DrawLargePointQuad` | `o058` | `other` | 416 | 70 | 70 | 48 | 48 | 0 | — |
| 141 | `src/overlays/o058/overlay58DrawPointQuad.c` | `overlay58DrawPointQuad` | `o058` | `other` | 416 | 70 | 70 | 48 | 48 | 0 | — |
| 142 | `src/main/gameVi.c` | `func_800336A8` | `main` | `other` | 780 | 71 | 71 | 0 | 0 | 0 | — |
| 143 | `src/overlays/o036/overlay36ChooseWeightedState.c` | `func_overlay_036_F0000A60_1883F18` | `o036` | `other` | 680 | 71 | 71 | 60 | 60 | 0 | — |
| 144 | `src/overlays/o083/overlay83DrawStrip.c` | `overlay83DrawStrip` | `o083` | `other` | 308 | 73 | 73 | 4 | 4 | 0 | — |
| 145 | `src/overlays/o086/func_overlay_086_F0000474_18D22AC.c` | `func_overlay_086_F0000474_18D22AC` | `o086` | `other` | 2,648 | 73 | 73 | 112 | 112 | 0 | — |
| 146 | `src/main/lights.c` | `func_80018F08` | `main` | `other` | 820 | 75 | 75 | 0 | 0 | 0 | — |
| 147 | `src/overlays/o040/overlay40BuildFrame.c` | `overlay40BuildFrame` | `o040` | `other` | 324 | 75 | 75 | 4 | 4 | 0 | — |
| 148 | `src/overlays/o101/overlay101DrawPanel.c` | `overlay101DrawPanel` | `o101` | `other` | 1,072 | 79 | 79 | 180 | 180 | 0 | — |
| 149 | `src/overlays/o007/overlay_007_tail.c` | `overlay7UpdateOwnerMode` | `o007` | `other` | 556 | 81 | 81 | 16 | 16 | 0 | — |
| 150 | `src/overlays/o014/func_overlay_014_F0001540_1870E18.c` | `func_overlay_014_F0001540_1870E18` | `o014` | `other` | 752 | 83 | 82 | 136 | 136 | 0 | — |
| 151 | `src/overlays/o060/func_overlay_060_F0002F54_18BCD2C.c` | `func_overlay_060_F0002F54_18BCD2C` | `o060` | `other` | 888 | 84 | 84 | 100 | 100 | 0 | — |
| 152 | `src/overlays/o015/overlay_015.c` | `overlay15InitStars` | `o015` | `other` | 760 | 89 | 89 | 124 | 124 | 0 | — |
| 153 | `src/main/fx.c` | `func_800470B0` | `main` | `other` | 596 | 90 | 90 | 68 | 68 | 0 | — |
| 154 | `src/main/objects.c` | `func_80007E40` | `main` | `other` | 488 | 92 | 92 | 24 | 24 | 0 | — |
| 155 | `src/overlays/o057/overlay57SmoothAndCheckDistance.c` | `overlay57SmoothAndCheckDistance` | `o057` | `other` | 800 | 97 | 92 | 36 | 36 | 0 | — |
| 156 | `src/overlays/o057/overlay57EaseAndLatch.c` | `overlay57EaseAndLatch` | `o057` | `other` | 884 | 101 | 92 | 20 | 44 | 0 | — |
| 157 | `src/overlays/o038/func_overlay_038_F000047C_188618C.c` | `func_overlay_038_F000047C_188618C` | `o038` | `other` | 876 | 93 | 93 | 192 | 192 | 0 | — |
| 158 | `src/overlays/o031/overlay31BuildLookupTables.c` | `func_overlay_031_F0000000_187F520` | `o031` | `other` | 744 | 94 | 94 | 52 | 52 | 0 | — |
| 159 | `src/overlays/o013/overlay13ProcessRecord.c` | `overlay13UpdateRecord` | `o013` | `other` | 644 | 96 | 96 | 44 | 44 | 0 | — |
| 160 | `src/overlays/o083/overlay83BuildBatch.c` | `overlay83BuildBatch` | `o083` | `other` | 672 | 97 | 97 | 0 | 0 | 0 | — |
| 161 | `src/overlays/o013/overlay13DrawRecord.c` | `overlay13DrawRecord` | `o013` | `other` | 756 | 100 | 97 | 48 | 84 | 0 | — |
| 162 | `src/overlays/o002/func_overlay_002_F0000C90_1857A88.c` | `func_overlay_002_F0000C90_1857A88` | `o002` | `other` | 1,420 | 108 | 98 | 0 | 0 | 0 | — |
| 163 | `src/overlays/o058/overlay58DrawSegmentStrip.c` | `overlay58DrawSegmentStrip` | `o058` | `other` | 804 | 102 | 102 | 12 | 12 | 0 | — |
| 164 | `src/overlays/o050/overlay50Initialize.c` | `func_overlay_050_F0000000_1896970` | `o050` | `other` | 740 | 112 | 104 | 60 | 192 | 0 | — |
| 165 | `src/overlays/o002/overlay2ChooseBoundary.c` | `overlay2ChooseBoundary` | `o002` | `other` | 1,168 | 112 | 106 | 92 | 92 | 0 | — |
| 166 | `src/overlays/o052/overlay52Initialize.c` | `func_overlay_052_F0000000_189A670` | `o052` | `other` | 1,264 | 119 | 115 | 44 | 44 | 0 | — |
| 167 | `src/overlays/o015/overlay_015.c` | `overlay15InitStarsAndPalette` | `o015` | `other` | 988 | 120 | 120 | 4 | 4 | 0 | — |
| 168 | `src/main/level.c` | `levelInit` | `main` | `other` | 2,064 | 122 | 122 | 0 | 0 | 0 | — |
| 169 | `src/overlays/o101/overlay101TailC6E8.c` | `func_overlay_101_F000C6E8_18E7F08` | `o101` | `other` | 1,268 | 130 | 124 | 52 | 52 | 0 | — |
| 170 | `src/overlays/o002/func_overlay_002_F0001364_185815C.c` | `func_overlay_002_F0001364_185815C` | `o002` | `other` | 756 | 125 | 125 | 8 | 8 | 0 | — |
| 171 | `src/overlays/o009/overlay_009.c` | `func_overlay_009_F0000000_1866678` | `o009` | `other` | 1,344 | 131 | 125 | 60 | 60 | 0 | — |
| 172 | `src/overlays/o025/overlay_025.c` | `overlay25UpdateEffect` | `o025` | `other` | 1,036 | 127 | 127 | 0 | 0 | 0 | — |
| 173 | `src/overlays/o017/overlay17CreateChain.c` | `overlay17CreateChain` | `o017` | `other` | 784 | 130 | 130 | 0 | 0 | 0 | — |
| 174 | `src/main/fx.c` | `func_8004A10C` | `main` | `other` | 628 | 132 | 132 | 0 | 0 | 0 | — |
| 175 | `src/overlays/o017/overlay17CalculateEndpoints.c` | `overlay17CalculateEndpoints` | `o017` | `other` | 792 | 133 | 133 | 36 | 36 | 0 | — |
| 176 | `src/overlays/o063/overlay63UpdateEffects.c` | `overlay63UpdateEffects` | `o063` | `other` | 1,400 | 142 | 139 | 88 | 364 | 0 | — |
| 177 | `src/overlays/o079/func_overlay_079_F0000FA0_18CDF40.c` | `func_overlay_079_F0000FA0_18CDF40` | `o079` | `other` | 736 | 144 | 143 | 56 | 60 | 0 | — |
| 178 | `src/overlays/o068/overlay68DrawSortedEntries.c` | `overlay68DrawSortedEntries` | `o068` | `other` | 852 | 146 | 146 | 0 | 0 | 0 | — |
| 179 | `src/overlays/o101/overlay101BuildPresentationD.c` | `overlay101BuildPresentationD` | `o101` | `other` | 824 | 157 | 156 | 16 | 16 | 0 | — |
| 180 | `src/main/particles.c` | `func_80040B88` | `main` | `other` | 1,208 | 157 | 157 | 0 | 0 | 0 | — |
| 181 | `src/overlays/o092/func_overlay_092_F0000308_18D6228.c` | `func_overlay_092_F0000308_18D6228` | `o092` | `other` | 1,832 | 158 | 157 | 148 | 148 | 0 | — |
| 182 | `src/main/shadows.c` | `func_800180B4` | `main` | `other` | 824 | 159 | 159 | 52 | 52 | 0 | — |
| 183 | `src/overlays/o038/overlay38UpdateParticles.c` | `func_overlay_038_F0000154_1885E64` | `o038` | `other` | 808 | 160 | 159 | 32 | 32 | 0 | — |
| 184 | `src/main/level.c` | `levelGetCounts` | `main` | `other` | 1,036 | 160 | 160 | 56 | 56 | 0 | — |
| 185 | `src/main/menu.c` | `func_80039E34` | `main` | `other` | 1,048 | 161 | 161 | 20 | 20 | 0 | — |
| 186 | `src/overlays/o100/overlay100DrawMotion.c` | `overlay100DrawMotion` | `o100` | `other` | 972 | 161 | 161 | 0 | 0 | 0 | — |
| 187 | `src/main/track.c` | `func_8001398C` | `main` | `other` | 1,320 | 162 | 162 | 96 | 96 | 0 | — |
| 188 | `src/overlays/o057/overlay57UpdateSelection.c` | `overlay57UpdateSelection` | `o057` | `other` | 1,132 | 166 | 162 | 48 | 76 | 0 | — |
| 189 | `src/overlays/o058/overlay58FinalizePackedStatus.c` | `overlay58FinalizePackedStatus` | `o058` | `other` | 1,216 | 178 | 177 | 24 | 24 | 0 | — |
| 190 | `src/main/track.c` | `func_8000F198` | `main` | `other` | 996 | 185 | 185 | 0 | 0 | 0 | — |
| 191 | `src/main/track.c` | `func_800140CC` | `main` | `other` | 868 | 187 | 187 | 0 | 0 | 0 | — |
| 192 | `src/main/anim.c` | `func_80051364` | `main` | `other` | 1,148 | 192 | 192 | 0 | 0 | 0 | — |
| 193 | `src/overlays/o027/overlay_027.c` | `func_overlay_027_F0000624_187BFFC` | `o027` | `other` | 1,016 | 196 | 195 | 8 | 8 | 0 | — |
| 194 | `src/main/camera.c` | `func_80022FD4` | `main` | `other` | 1,476 | 203 | 203 | 0 | 0 | 0 | — |
| 195 | `src/overlays/o068/overlay68UpdateAnimation.c` | `overlay68UpdateAnimation` | `o068` | `other` | 1,424 | 208 | 208 | 28 | 28 | 0 | — |
| 196 | `src/main/rcpFast3d.c` | `func_8002EBE0` | `main` | `other` | 1,020 | 218 | 218 | 0 | 0 | 0 | — |
| 197 | `src/main/menu.c` | `func_80038E1C` | `main` | `other` | 1,116 | 219 | 219 | 216 | 216 | 0 | — |
| 198 | `src/overlays/o101/overlay101TailB544.c` | `func_overlay_101_F000B544_18E6D64` | `o101` | `other` | 1,264 | 222 | 222 | 72 | 72 | 0 | — |
| 199 | `src/overlays/o041/overlay41UpdateCurveObject.c` | `func_overlay_041_F0000854_1887B8C` | `o041` | `other` | 2,552 | 225 | 222 | 120 | 120 | 0 | — |
| 200 | `src/overlays/o011/func_overlay_011_F00022E8_186AB30.c` | `func_overlay_011_F00022E8_186AB30` | `o011` | `other` | 1,068 | 231 | 227 | 4 | 4 | 0 | — |
| 201 | `src/overlays/o036/overlay36UpdateInteractiveEntity.c` | `overlay36UpdateInteractiveEntity` | `o036` | `other` | 1,220 | 238 | 235 | 24 | 44 | 0 | — |
| 202 | `src/overlays/o007/func_overlay_007_F0000324_185C1AC.c` | `func_overlay_007_F0000324_185C1AC` | `o007` | `other` | 1,392 | 242 | 238 | 0 | 0 | 0 | — |
| 203 | `src/overlays/o101/overlay101TailC144.c` | `func_overlay_101_F000C144_18E7964` | `o101` | `other` | 1,444 | 249 | 246 | 16 | 16 | 0 | — |
| 204 | `src/main/models.c` | `func_8002057C` | `main` | `other` | 1,368 | 249 | 249 | 0 | 0 | 0 | — |
| 205 | `src/overlays/o066/overlay66SmoothAndDraw.c` | `func_overlay_066_F0000040_18C64A8` | `o066` | `other` | 1,184 | 251 | 249 | 4 | 132 | 0 | — |
| 206 | `src/overlays/o101/func_overlay_101_F000571C_18E0F3C.c` | `func_overlay_101_F000571C_18E0F3C` | `o101` | `other` | 1,772 | 263 | 263 | 44 | 44 | 0 | — |
| 207 | `src/overlays/o101/func_overlay_101_F00063F8_18E1C18.c` | `func_overlay_101_F00063F8_18E1C18` | `o101` | `other` | 1,520 | 296 | 296 | 44 | 44 | 0 | — |
| 208 | `src/overlays/o058/func_overlay_058_F0000000_18AF1E8.c` | `func_overlay_058_F0000000_18AF1E8` | `o058` | `other` | 1,472 | 306 | 296 | 0 | 0 | 0 | — |
| 209 | `src/overlays/o101/func_overlay_101_F000512C_18E094C.c` | `func_overlay_101_F000512C_18E094C` | `o101` | `other` | 1,520 | 298 | 298 | 44 | 44 | 0 | — |
| 210 | `src/overlays/o101/func_overlay_101_F0005E08_18E1628.c` | `func_overlay_101_F0005E08_18E1628` | `o101` | `other` | 1,520 | 298 | 298 | 44 | 44 | 0 | — |
| 211 | `src/overlays/o099/func_overlay_099_F0000DDC_18DA38C.c` | `func_overlay_099_F0000DDC_18DA38C` | `o099` | `other` | 1,408 | 331 | 331 | 4 | 4 | 0 | — |
| 212 | `src/overlays/o098/overlay98RenderReflections.c` | `overlay98RenderReflections` | `o098` | `other` | 1,556 | 333 | 332 | 0 | 0 | 0 | — |
| 213 | `src/overlays/o101/overlay101TailBA34.c` | `func_overlay_101_F000BA34_18E7254` | `o101` | `other` | 1,808 | 394 | 393 | 0 | 0 | 0 | — |
| 214 | `src/overlays/o064/overlay64GenerateTexture.c` | `func_overlay_064_F0000000_18C3B28` | `o064` | `other` | 1,680 | 401 | 401 | 0 | 0 | 0 | — |
| 215 | `src/main/charControl.c` | `func_8001DD70` | `main` | `other` | 2,132 | 430 | 430 | 0 | 0 | 0 | — |
| 216 | `src/main/objects.c` | `func_8000590C` | `main` | `other` | 2,876 | 538 | 538 | 436 | 436 | 0 | — |
| 217 | `src/overlays/o101/func_overlay_101_F00069E8_18E2208.c` | `func_overlay_101_F00069E8_18E2208` | `o101` | `other` | 3,852 | 617 | 614 | 136 | 136 | 0 | — |
| 218 | `src/overlays/o008/overlay_008.c` | `func_overlay_008_F0001294_185EFEC` | `o008` | `other` | 5,036 | 654 | 636 | 0 | 0 | 0 | — |
| 219 | `src/main/track.c` | `func_80010B4C` | `main` | `other` | 2,712 | 668 | 668 | 0 | 0 | 0 | — |
| 220 | `src/overlays/o050/func_overlay_050_F0000334_1896CA4.c` | `func_overlay_050_F0000334_1896CA4` | `o050` | `other` | 6,300 | 1,422 | 1,422 | 0 | 0 | 0 | — |
| 221 | `src/overlays/o047/func_overlay_047_F0000B30_1891948.c` | `func_overlay_047_F0000B30_1891948` | `o047` | `other` | 8,672 | 1,975 | 1,972 | 4 | 4 | 0 | — |
| 222 | `src/main/runlink.c` | `runlinkInit` | `main` | `reloc-mismatch` | 584 | 1 | 0 | 276 | — | 0 | — |
| 223 | `src/libultra/block_6F3E0.c` | `func_8006E7E0` | `libultra` | `size-mismatch` | 4 | 2 | 2 | 0 | 0 | 4 | — |
| 224 | `src/main/flash_58C10.c` | `func_8005807C` | `main` | `size-mismatch` | 116 | 2 | 2 | 108 | 108 | -8 | — |
| 225 | `src/main/frontend_37D50.c` | `func_80037BF4` | `main` | `size-mismatch` | 128 | 17 | 17 | 36 | 36 | -4 | — |
| 226 | `src/main/vehicle_sounds.c` | `func_80058250` | `main` | `size-mismatch` | 88 | 19 | 19 | 0 | 0 | 16 | — |
| 227 | `src/overlays/o001/overlay_001_tail.c` | `overlay1HandleCachedMode` | `o001` | `size-mismatch` | 128 | 24 | 23 | 4 | 20 | 4 | — |
| 228 | `src/main/block_506D0.c` | `func_8004FAD0` | `main` | `size-mismatch` | 336 | 29 | 29 | 68 | 68 | -4 | — |
| 229 | `src/overlays/o015/overlay_015.c` | `overlay15MoveStars` | `o015` | `size-mismatch` | 216 | 33 | 30 | 48 | 48 | 16 | — |
| 230 | `src/main/matrix.c` | `func_8002B040` | `main` | `size-mismatch` | 136 | 34 | 34 | 0 | 0 | 4 | — |
| 231 | `src/main/fx.c` | `func_80049E4C` | `main` | `size-mismatch` | 676 | 36 | 36 | 24 | 24 | -8 | — |
| 232 | `src/main/menu_3B1A0.c` | `func_8003A7D0` | `main` | `size-mismatch` | 176 | 37 | 37 | 16 | 16 | -12 | — |
| 233 | `src/main/objects.c` | `func_8000A39C` | `main` | `size-mismatch` | 656 | 37 | 37 | 4 | 4 | 4 | — |
| 234 | `src/main/objects.c` | `func_80006EE4` | `main` | `size-mismatch` | 188 | 40 | 40 | 8 | 8 | -4 | — |
| 235 | `src/overlays/o034/overlay34RemoveRecord.c` | `overlay34RemoveRecord` | `o034` | `size-mismatch` | 176 | 42 | 41 | 0 | 0 | -8 | — |
| 236 | `src/main/flash_58C10.c` | `func_800580F0` | `main` | `size-mismatch` | 204 | 42 | 42 | 12 | 12 | -8 | — |
| 237 | `src/main/fx.c` | `func_80048080` | `main` | `size-mismatch` | 356 | 43 | 43 | 116 | 116 | -4 | — |
| 238 | `src/main/lights.c` | `func_80019DE8` | `main` | `size-mismatch` | 252 | 45 | 45 | 68 | 68 | 4 | — |
| 239 | `src/overlays/o048/overlay48InitializeState.c` | `overlay48InitializeState` | `o048` | `size-mismatch` | 228 | 47 | 47 | 0 | 0 | -16 | — |
| 240 | `src/main/frontend_37D50.c` | `func_80037414` | `main` | `size-mismatch` | 580 | 59 | 59 | 16 | 16 | 4 | — |
| 241 | `src/main/fx.c` | `func_80046EC4` | `main` | `size-mismatch` | 440 | 60 | 60 | 104 | 104 | 4 | — |
| 242 | `src/overlays/o002/overlay2ClassifyBoundary.c` | `overlay2ClassifyBoundary` | `o002` | `size-mismatch` | 316 | 62 | 61 | 4 | 20 | 4 | — |
| 243 | `src/main/memory.c` | `func_8002B7AC` | `main` | `size-mismatch` | 252 | 62 | 62 | 4 | 4 | -4 | — |
| 244 | `src/main/objects.c` | `func_80006B04` | `main` | `size-mismatch` | 316 | 63 | 63 | 20 | 20 | -4 | — |
| 245 | `src/main/track.c` | `func_8000D820` | `main` | `size-mismatch` | 344 | 65 | 65 | 52 | 52 | -8 | — |
| 246 | `src/main/camera.c` | `func_80024978` | `main` | `size-mismatch` | 332 | 67 | 66 | 8 | 8 | 4 | — |
| 247 | `src/main/saves.c` | `func_8002CF6C` | `main` | `size-mismatch` | 352 | 77 | 77 | 8 | 8 | -12 | — |
| 248 | `src/main/block_4F4E0.c` | `func_8004EC60` | `main` | `size-mismatch` | 328 | 79 | 79 | 0 | 0 | 4 | — |
| 249 | `src/main/objects.c` | `func_80005548` | `main` | `size-mismatch` | 348 | 81 | 81 | 24 | 24 | -8 | — |
| 250 | `src/main/objects.c` | `func_8000831C` | `main` | `size-mismatch` | 424 | 81 | 81 | 64 | 64 | -4 | — |
| 251 | `src/main/anim.c` | `func_8005776C` | `main` | `size-mismatch` | 420 | 83 | 83 | 36 | 36 | -4 | — |
| 252 | `src/main/matrix.c` | `func_8002AB78` | `main` | `size-mismatch` | 268 | 83 | 83 | 0 | 0 | 68 | — |
| 253 | `src/main/sched.c` | `__scHandleRetrace` | `main` | `size-mismatch` | 1,636 | 84 | 84 | 984 | 984 | -4 | — |
| 254 | `src/overlays/o015/overlay_015.c` | `overlay15UpdateMovingStars` | `o015` | `size-mismatch` | 412 | 87 | 84 | 36 | 48 | 28 | — |
| 255 | `src/overlays/o001/overlay_001_head.c` | `overlay1BuildObjectMappings` | `o001` | `size-mismatch` | 592 | 88 | 88 | 0 | 0 | -12 | — |
| 256 | `src/overlays/o026/overlay26DrawGroups.c` | `func_overlay_026_F0001158_187B550` | `o026` | `size-mismatch` | 536 | 88 | 88 | 76 | 76 | -4 | — |
| 257 | `src/main/diCpu.c` | `func_80046BCC` | `main` | `size-mismatch` | 424 | 89 | 89 | 44 | 44 | 4 | — |
| 258 | `src/main/matrix.c` | `func_8002AA50` | `main` | `size-mismatch` | 296 | 90 | 90 | 0 | 0 | 68 | — |
| 259 | `src/overlays/o001/overlay_001_tail.c` | `overlay1SolveAngleCandidates` | `o001` | `size-mismatch` | 556 | 92 | 92 | 168 | 168 | -8 | — |
| 260 | `src/main/frontend_37D50.c` | `func_800376CC` | `main` | `size-mismatch` | 472 | 95 | 95 | 0 | 0 | 4 | — |
| 261 | `src/main/objects.c` | `func_80004590` | `main` | `size-mismatch` | 396 | 96 | 96 | 0 | 0 | 20 | — |
| 262 | `src/main/models_5B300.c` | `func_8005ABA8` | `main` | `size-mismatch` | 444 | 97 | 97 | 56 | 56 | -4 | — |
| 263 | `src/main/track.c` | `func_800133FC` | `main` | `size-mismatch` | 384 | 98 | 98 | 0 | 0 | 12 | — |
| 264 | `src/overlays/o089/overlay89UpdateStateAndParticles.c` | `overlay89UpdateStateAndParticles` | `o089` | `size-mismatch` | 544 | 98 | 98 | 0 | 0 | 4 | — |
| 265 | `src/main/particles.c` | `func_8004054C` | `main` | `size-mismatch` | 500 | 101 | 101 | 44 | 44 | -4 | — |
| 266 | `src/overlays/o063/overlay63UpdateSequence.c` | `overlay63UpdateSequence` | `o063` | `size-mismatch` | 428 | 102 | 102 | 20 | 20 | -4 | — |
| 267 | `src/main/frontend_37D50.c` | `func_800378A4` | `main` | `size-mismatch` | 468 | 103 | 103 | 20 | 20 | -20 | — |
| 268 | `src/overlays/o099/overlay99BuildHeightGrid.c` | `overlay99BuildHeightGrid` | `o099` | `size-mismatch` | 456 | 104 | 104 | 44 | 44 | 4 | — |
| 269 | `src/main/font.c` | `func_8004C690` | `main` | `size-mismatch` | 584 | 105 | 105 | 0 | 0 | -8 | — |
| 270 | `src/main/track.c` | `func_8000D3B8` | `main` | `size-mismatch` | 440 | 105 | 105 | 4 | 4 | -4 | — |
| 271 | `src/overlays/o068/overlay68RebuildSecondaryEntry.c` | `overlay68RebuildSecondaryEntry` | `o068` | `size-mismatch` | 488 | 105 | 105 | 0 | 0 | -8 | — |
| 272 | `src/main/weather_tail.c` | `func_8003C80C` | `main` | `size-mismatch` | 472 | 106 | 106 | 16 | 16 | -4 | — |
| 273 | `src/main/sched.c` | `func_80030610` | `main` | `size-mismatch` | 768 | 113 | 112 | 0 | 0 | 4 | — |
| 274 | `src/main/frontend_37680.c` | `func_80036AB0` | `main` | `size-mismatch` | 432 | 113 | 113 | 0 | 0 | 28 | — |
| 275 | `src/main/track.c` | `func_8000D1B8` | `main` | `size-mismatch` | 512 | 114 | 114 | 48 | 48 | 8 | — |
| 276 | `src/main/runlink.c` | `runlinkFreeCode` | `main` | `size-mismatch` | 736 | 117 | 117 | 0 | 0 | -4 | — |
| 277 | `src/main/matrix.c` | `func_8002AC84` | `main` | `size-mismatch` | 396 | 118 | 118 | 0 | 0 | 80 | — |
| 278 | `src/main/textures_35024.c` | `func_80034448` | `main` | `size-mismatch` | 856 | 118 | 118 | 88 | 88 | 4 | — |
| 279 | `src/overlays/o041/overlay41ProcessEntry.c` | `func_overlay_041_F0001464_188879C` | `o041` | `size-mismatch` | 492 | 120 | 120 | 16 | 16 | 20 | — |
| 280 | `src/overlays/o001/overlay_001_tail.c` | `overlay1AdvancePath` | `o001` | `size-mismatch` | 648 | 121 | 121 | 16 | 16 | 4 | — |
| 281 | `src/main/fx.c` | `wakeDraw` | `main` | `size-mismatch` | 708 | 122 | 122 | 0 | 0 | -4 | — |
| 282 | `src/main/track.c` | `func_800103D4` | `main` | `size-mismatch` | 640 | 122 | 122 | 0 | 0 | -4 | — |
| 283 | `src/main/fx.c` | `fxScreenEffect` | `main` | `size-mismatch` | 588 | 123 | 123 | 8 | 8 | -28 | — |
| 284 | `src/overlays/o031/overlay31InitializeParticleAssets.c` | `func_overlay_031_F00002E8_187F808` | `o031` | `size-mismatch` | 528 | 123 | 123 | 16 | 16 | 4 | — |
| 285 | `src/main/runlink.c` | `ProcessRelocationEntry` | `main` | `size-mismatch` | 584 | 126 | 126 | 0 | 0 | 4 | — |
| 286 | `src/overlays/o054/overlay54Initialize.c` | `func_overlay_054_F0000000_189ECA0` | `o054` | `size-mismatch` | 972 | 144 | 127 | 84 | 240 | 4 | — |
| 287 | `src/overlays/o034/overlay34SortAndDraw.c` | `overlay34SortAndDraw` | `o034` | `size-mismatch` | 760 | 129 | 128 | 20 | 20 | -4 | — |
| 288 | `src/main/spranim.c` | `func_8001B798` | `main` | `size-mismatch` | 700 | 131 | 131 | 0 | 0 | -16 | — |
| 289 | `src/overlays/o044/overlay44UpdateFrameCache.c` | `overlay44UpdateFrameCache` | `o044` | `size-mismatch` | 748 | 133 | 133 | 0 | 0 | -4 | — |
| 290 | `src/main/matrix.c` | `func_8002AE10` | `main` | `size-mismatch` | 348 | 138 | 138 | 0 | 0 | 208 | — |
| 291 | `src/overlays/o037/overlay37Render.c` | `overlay37RenderEffect` | `o037` | `size-mismatch` | 856 | 139 | 139 | 0 | 0 | -4 | — |
| 292 | `src/main/weather.c` | `snow_render` | `main` | `size-mismatch` | 568 | 142 | 142 | 0 | 0 | 16 | — |
| 293 | `src/main/models.c` | `func_80020B10` | `main` | `size-mismatch` | 636 | 143 | 143 | 0 | 0 | -4 | — |
| 294 | `src/main/track.c` | `func_8000DB34` | `main` | `size-mismatch` | 688 | 146 | 146 | 56 | 56 | -8 | — |
| 295 | `src/overlays/o010/overlay10Initialize.c` | `overlay10Initialize` | `o010` | `size-mismatch` | 688 | 147 | 147 | 0 | 0 | 156 | — |
| 296 | `src/main/frontend_37D50.c` | `func_800371BC` | `main` | `size-mismatch` | 600 | 148 | 148 | 44 | 44 | 36 | — |
| 297 | `src/main/objects.c` | `func_80009F74` | `main` | `size-mismatch` | 720 | 149 | 149 | 0 | 0 | -8 | — |
| 298 | `src/main/audiomgr.c` | `func_80001740` | `main` | `size-mismatch` | 836 | 154 | 153 | 0 | 0 | -24 | — |
| 299 | `src/main/fx.c` | `func_80049B14` | `main` | `size-mismatch` | 824 | 154 | 154 | 8 | 8 | 4 | — |
| 300 | `src/main/track.c` | `func_80012658` | `main` | `size-mismatch` | 708 | 154 | 154 | 0 | 0 | -8 | — |
| 301 | `src/main/lights.c` | `func_80019AB8` | `main` | `size-mismatch` | 736 | 155 | 155 | 32 | 32 | -4 | — |
| 302 | `src/overlays/o008/overlay_008.c` | `func_overlay_008_F0001000_185ED58` | `o008` | `size-mismatch` | 660 | 159 | 159 | 8 | 8 | -4 | — |
| 303 | `src/overlays/o001/overlay_001_tail.c` | `overlay1TransitionState` | `o001` | `size-mismatch` | 948 | 160 | 160 | 32 | 32 | -4 | — |
| 304 | `src/main/track.c` | `func_80010654` | `main` | `size-mismatch` | 684 | 162 | 162 | 0 | 0 | 4 | — |
| 305 | `src/overlays/o013/overlay13DrawActive.c` | `overlay13DrawActive` | `o013` | `size-mismatch` | 664 | 163 | 162 | 0 | 0 | -4 | — |
| 306 | `src/overlays/o101/overlay101BuildPresentationB.c` | `overlay101BuildPresentationB` | `o101` | `size-mismatch` | 832 | 163 | 162 | 16 | 16 | 4 | — |
| 307 | `src/overlays/o101/overlay101BuildPresentationC.c` | `overlay101BuildPresentationC` | `o101` | `size-mismatch` | 832 | 166 | 165 | 36 | 36 | 4 | — |
| 308 | `src/overlays/o101/overlay101BuildPresentationA.c` | `overlay101BuildPresentationA` | `o101` | `size-mismatch` | 832 | 167 | 166 | 16 | 16 | 4 | — |
| 309 | `src/main/fx.c` | `func_800479D4` | `main` | `size-mismatch` | 772 | 167 | 167 | 0 | 0 | -20 | — |
| 310 | `src/main/textures_354C8.c` | `func_80036544` | `main` | `size-mismatch` | 608 | 172 | 172 | 0 | 0 | 80 | — |
| 311 | `src/main/fx.c` | `func_80047304` | `main` | `size-mismatch` | 740 | 175 | 175 | 4 | 4 | -8 | — |
| 312 | `src/main/textures_354C8.c` | `func_800355A0` | `main` | `size-mismatch` | 1,076 | 177 | 177 | 0 | 0 | -4 | — |
| 313 | `src/overlays/o020/func_overlay_020_F0001148_1877720.c` | `func_overlay_020_F0001148_1877720` | `o020` | `size-mismatch` | 828 | 180 | 178 | 32 | 32 | -8 | — |
| 314 | `src/overlays/o066/func_overlay_066_F00004E0_18C6948.c` | `func_overlay_066_F00004E0_18C6948` | `o066` | `size-mismatch` | 816 | 181 | 181 | 0 | 0 | -12 | — |
| 315 | `src/overlays/o001/func_overlay_001_F0003258_184F638.c` | `func_overlay_001_F0003258_184F638` | `o001` | `size-mismatch` | 800 | 183 | 182 | 0 | 0 | -48 | — |
| 316 | `src/main/track.c` | `func_8000E5EC` | `main` | `size-mismatch` | 820 | 185 | 185 | 0 | 0 | 16 | — |
| 317 | `src/overlays/o020/overlay20UpdateGrid.c` | `overlay20UpdateGrid` | `o020` | `size-mismatch` | 860 | 185 | 185 | 0 | 0 | 4 | — |
| 318 | `src/overlays/o014/func_overlay_014_F0001830_1871108.c` | `func_overlay_014_F0001830_1871108` | `o014` | `size-mismatch` | 804 | 186 | 186 | 0 | 0 | 4 | — |
| 319 | `src/overlays/o011/func_overlay_011_F0001E4C_186A694.c` | `func_overlay_011_F0001E4C_186A694` | `o011` | `size-mismatch` | 1,180 | 189 | 186 | 216 | 216 | -4 | — |
| 320 | `src/main/track.c` | `func_80012234` | `main` | `size-mismatch` | 832 | 187 | 187 | 12 | 12 | 12 | — |
| 321 | `src/overlays/o087/func_overlay_087_F0000128_18D3090.c` | `func_overlay_087_F0000128_18D3090` | `o087` | `size-mismatch` | 1,896 | 195 | 193 | 44 | 44 | -16 | — |
| 322 | `src/main/objects.c` | `func_80006534` | `main` | `size-mismatch` | 820 | 198 | 198 | 0 | 0 | 8 | — |
| 323 | `src/overlays/o099/overlay99ApplySegment.c` | `overlay99ApplySegment` | `o099` | `size-mismatch` | 920 | 203 | 202 | 0 | 0 | -4 | — |
| 324 | `src/main/anim.c` | `func_800573C8` | `main` | `size-mismatch` | 932 | 203 | 203 | 44 | 44 | -8 | — |
| 325 | `src/main/main.c` | `func_80026FB4` | `main` | `size-mismatch` | 1,652 | 207 | 207 | 56 | 56 | 24 | — |
| 326 | `src/main/track.c` | `func_80011980` | `main` | `size-mismatch` | 860 | 211 | 211 | 0 | 0 | 8 | — |
| 327 | `src/main/anim.c` | `func_80056DD8` | `main` | `size-mismatch` | 916 | 214 | 214 | 0 | 0 | -12 | — |
| 328 | `src/main/fx.c` | `fxSPDPRipple` | `main` | `size-mismatch` | 928 | 224 | 224 | 8 | 8 | 8 | — |
| 329 | `src/main/lights.c` | `func_8001953C` | `main` | `size-mismatch` | 1,016 | 225 | 225 | 0 | 0 | 32 | — |
| 330 | `src/main/objects.c` | `func_80004C28` | `main` | `size-mismatch` | 952 | 230 | 230 | 0 | 0 | 4 | — |
| 331 | `src/main/charControl.c` | `func_8001EC44` | `main` | `size-mismatch` | 952 | 231 | 231 | 0 | 0 | 12 | — |
| 332 | `src/main/track.c` | `func_800115E4` | `main` | `size-mismatch` | 924 | 231 | 231 | 0 | 0 | 20 | — |
| 333 | `src/overlays/o020/func_overlay_020_F000038C_1876964.c` | `func_overlay_020_F000038C_1876964` | `o020` | `size-mismatch` | 1,080 | 256 | 256 | 0 | 0 | -24 | — |
| 334 | `src/main/camera.c` | `func_80023598` | `main` | `size-mismatch` | 1,136 | 263 | 263 | 0 | 0 | 8 | — |
| 335 | `src/overlays/o057/func_overlay_057_F00060F8_18A9CF0.c` | `func_overlay_057_F00060F8_18A9CF0` | `o057` | `size-mismatch` | 1,764 | 278 | 263 | 0 | 0 | -16 | — |
| 336 | `src/main/textures_354C8.c` | `func_800349A4` | `main` | `size-mismatch` | 1,088 | 265 | 265 | 8 | 8 | 8 | — |
| 337 | `src/overlays/o101/overlay101TailA6BC.c` | `overlay101TailA6BC` | `o101` | `size-mismatch` | 1,168 | 267 | 267 | 0 | 0 | 4 | — |
| 338 | `src/main/objects.c` | `func_80009AA8` | `main` | `size-mismatch` | 976 | 268 | 268 | 0 | 0 | 100 | — |
| 339 | `src/main/shadows.c` | `func_80017BCC` | `main` | `size-mismatch` | 1,256 | 271 | 271 | 0 | 0 | -4 | — |
| 340 | `src/main/rcpFast3d.c` | `func_8002FB34` | `main` | `size-mismatch` | 1,436 | 272 | 272 | 64 | 64 | -20 | — |
| 341 | `src/main/objects.c` | `func_8000784C` | `main` | `size-mismatch` | 1,052 | 273 | 273 | 0 | 0 | 56 | — |
| 342 | `src/overlays/o101/func_overlay_101_F0002510_18DDD30.c` | `func_overlay_101_F0002510_18DDD30` | `o101` | `size-mismatch` | 1,172 | 276 | 276 | 0 | 0 | -52 | — |
| 343 | `src/main/rcpFast3d.c` | `func_8002F618` | `main` | `size-mismatch` | 1,308 | 279 | 279 | 52 | 52 | 4 | — |
| 344 | `src/main/particles.c` | `func_80041530` | `main` | `size-mismatch` | 1,824 | 280 | 280 | 0 | 0 | 4 | — |
| 345 | `src/overlays/o035/func_overlay_035_F00001E0_1881EC0.c` | `func_overlay_035_F00001E0_1881EC0` | `o035` | `size-mismatch` | 1,424 | 283 | 283 | 120 | 120 | -12 | — |
| 346 | `src/main/shadows.c` | `func_80017660` | `main` | `size-mismatch` | 1,388 | 288 | 288 | 88 | 88 | 12 | — |
| 347 | `src/main/track.c` | `func_8001357C` | `main` | `size-mismatch` | 1,040 | 289 | 289 | 8 | 8 | 244 | — |
| 348 | `src/main/models.c` | `func_8001FC50` | `main` | `size-mismatch` | 1,332 | 300 | 300 | 0 | 0 | -12 | — |
| 349 | `src/main/shadows.c` | `func_80017140` | `main` | `size-mismatch` | 1,312 | 303 | 303 | 72 | 72 | -20 | — |
| 350 | `src/main/track.c` | `func_8000DFBC` | `main` | `size-mismatch` | 1,584 | 304 | 304 | 72 | 72 | 8 | — |
| 351 | `src/overlays/o043/func_overlay_043_F0000BE4_188ABB4.c` | `func_overlay_043_F0000BE4_188ABB4` | `o043` | `size-mismatch` | 1,220 | 307 | 307 | 4 | 4 | 28 | — |
| 352 | `src/overlays/o079/func_overlay_079_F0000134_18CD0D4.c` | `func_overlay_079_F0000134_18CD0D4` | `o079` | `size-mismatch` | 3,528 | 308 | 308 | 88 | 88 | 4 | — |
| 353 | `src/main/objects.c` | `func_800084C4` | `main` | `size-mismatch` | 1,372 | 314 | 314 | 0 | 0 | 20 | — |
| 354 | `src/main/frontend_37D50.c` | `func_80037C74` | `main` | `size-mismatch` | 1,308 | 324 | 324 | 0 | 0 | -76 | — |
| 355 | `src/main/track.c` | `func_80011CDC` | `main` | `size-mismatch` | 1,368 | 327 | 327 | 0 | 0 | 8 | — |
| 356 | `src/overlays/o044/func_overlay_044_F0000580_188BDE0.c` | `func_overlay_044_F0000580_188BDE0` | `o044` | `size-mismatch` | 1,396 | 330 | 330 | 0 | 0 | -8 | — |
| 357 | `src/main/fx.c` | `wakeUpdate` | `main` | `size-mismatch` | 1,592 | 334 | 334 | 0 | 0 | -8 | — |
| 358 | `src/main/objects.c` | `func_80004FE0` | `main` | `size-mismatch` | 1,384 | 337 | 337 | 0 | 0 | 4 | — |
| 359 | `src/overlays/o012/func_overlay_012_F00003A8_186D628.c` | `func_overlay_012_F00003A8_186D628` | `o012` | `size-mismatch` | 1,384 | 340 | 340 | 16 | 16 | 12 | — |
| 360 | `src/main/weather.c` | `rain_render_splashes` | `main` | `size-mismatch` | 1,616 | 341 | 340 | 0 | 0 | -4 | — |
| 361 | `src/overlays/o001/overlay_001_tail.c` | `func_overlay_001_F0003750_184FB30` | `o001` | `size-mismatch` | 1,784 | 346 | 341 | 76 | 88 | 4 | — |
| 362 | `src/main/fx.c` | `wakeAllocate` | `main` | `size-mismatch` | 1,404 | 345 | 345 | 12 | 12 | -32 | — |
| 363 | `src/overlays/o057/func_overlay_057_F0000000_18A3BF8.c` | `func_overlay_057_F0000000_18A3BF8` | `o057` | `size-mismatch` | 2,388 | 371 | 345 | 8 | 8 | -12 | — |
| 364 | `src/main/audiomgr.c` | `func_80001BF4` | `main` | `size-mismatch` | 1,344 | 350 | 350 | 0 | 0 | 144 | — |
| 365 | `src/overlays/o046/func_overlay_046_F0000874_188EC6C.c` | `func_overlay_046_F0000874_188EC6C` | `o046` | `size-mismatch` | 1,800 | 364 | 363 | 0 | 0 | 12 | — |
| 366 | `src/main/anim.c` | `func_80054B3C` | `main` | `size-mismatch` | 1,480 | 365 | 365 | 0 | 0 | 4 | — |
| 367 | `src/main/frontend_37D50.c` | `func_80038190` | `main` | `size-mismatch` | 1,472 | 366 | 365 | 0 | 0 | -104 | — |
| 368 | `src/main/textures_354C8.c` | `func_80035F48` | `main` | `size-mismatch` | 1,532 | 371 | 371 | 0 | 0 | -84 | — |
| 369 | `src/main/models.c` | `func_8001F520` | `main` | `size-mismatch` | 1,604 | 376 | 376 | 8 | 8 | -56 | — |
| 370 | `src/main/models_5B300.c` | `func_8005AF14` | `main` | `size-mismatch` | 1,840 | 377 | 377 | 0 | 0 | 12 | — |
| 371 | `src/overlays/o008/overlay_008.c` | `func_overlay_008_F00042A8_1862000` | `o008` | `size-mismatch` | 1,788 | 384 | 384 | 68 | 68 | -68 | — |
| 372 | `src/main/charControl.c` | `func_8001C4C0` | `main` | `size-mismatch` | 1,612 | 386 | 386 | 0 | 0 | -8 | — |
| 373 | `src/overlays/o002/func_overlay_002_F0001DF8_1858BF0.c` | `func_overlay_002_F0001DF8_1858BF0` | `o002` | `size-mismatch` | 1,840 | 387 | 387 | 40 | 40 | 4 | — |
| 374 | `src/main/fx.c` | `func_800475E8` | `main` | `size-mismatch` | 1,004 | 390 | 390 | 0 | 0 | 780 | — |
| 375 | `src/main/objects.c` | `func_80008B94` | `main` | `size-mismatch` | 1,676 | 397 | 397 | 12 | 12 | 4 | — |
| 376 | `src/main/charControl.c` | `func_8001E5C4` | `main` | `size-mismatch` | 1,664 | 413 | 410 | 0 | 0 | -16 | — |
| 377 | `src/main/objects.c` | `func_80009414` | `main` | `size-mismatch` | 1,684 | 411 | 411 | 0 | 0 | -172 | — |
| 378 | `src/overlays/o046/func_overlay_046_F0001228_188F620.c` | `func_overlay_046_F0001228_188F620` | `o046` | `size-mismatch` | 1,844 | 416 | 415 | 92 | 92 | -96 | — |
| 379 | `src/main/anim.c` | `func_80055104` | `main` | `size-mismatch` | 1,780 | 425 | 425 | 0 | 0 | -64 | — |
| 380 | `src/main/main.c` | `func_80028564` | `main` | `size-mismatch` | 1,956 | 427 | 426 | 4 | 4 | 12 | — |
| 381 | `src/overlays/o022/func_overlay_022_F00002B0_18783B8.c` | `func_overlay_022_F00002B0_18783B8` | `o022` | `size-mismatch` | 1,996 | 431 | 430 | 4 | 4 | 8 | — |
| 382 | `src/overlays/o001/func_overlay_001_F0002B4C_184EF2C.c` | `func_overlay_001_F0002B4C_184EF2C` | `o001` | `size-mismatch` | 1,804 | 433 | 433 | 0 | 0 | -16 | — |
| 383 | `src/overlays/o029/func_overlay_029_F00005C4_187D874.c` | `func_overlay_029_F00005C4_187D874` | `o029` | `size-mismatch` | 2,332 | 438 | 438 | 4 | 4 | -4 | — |
| 384 | `src/overlays/o008/overlay_008.c` | `func_overlay_008_F0000058_185DDB0` | `o008` | `size-mismatch` | 2,108 | 442 | 438 | 44 | 112 | -64 | — |
| 385 | `src/main/diCpu.c` | `func_80045D34` | `main` | `size-mismatch` | 1,836 | 440 | 439 | 0 | 0 | -32 | — |
| 386 | `src/main/objects.c` | `func_80007118` | `main` | `size-mismatch` | 1,836 | 445 | 445 | 0 | 0 | -388 | — |
| 387 | `src/main/shadows.c` | `shadowGenerate` | `main` | `size-mismatch` | 2,040 | 445 | 445 | 12 | 12 | 40 | — |
| 388 | `src/overlays/o055/func_overlay_055_F000031C_18A1E34.c` | `func_overlay_055_F000031C_18A1E34` | `o055` | `size-mismatch` | 2,324 | 445 | 445 | 60 | 60 | -24 | — |
| 389 | `src/overlays/o057/func_overlay_057_F0004460_18A8058.c` | `func_overlay_057_F0004460_18A8058` | `o057` | `size-mismatch` | 1,976 | 450 | 449 | 4 | 4 | -144 | — |
| 390 | `src/overlays/o084/func_overlay_084_F0000314_18D07F4.c` | `func_overlay_084_F0000314_18D07F4` | `o084` | `size-mismatch` | 1,856 | 450 | 449 | 60 | 60 | 16 | — |
| 391 | `src/main/textures_354C8.c` | `func_80034E54` | `main` | `size-mismatch` | 1,868 | 461 | 461 | 0 | 0 | -32 | — |
| 392 | `src/overlays/o101/func_overlay_101_F0008128_18E3948.c` | `func_overlay_101_F0008128_18E3948` | `o101` | `size-mismatch` | 2,100 | 464 | 461 | 4 | 4 | -4 | — |
| 393 | `src/overlays/o101/func_overlay_101_F000895C_18E417C.c` | `func_overlay_101_F000895C_18E417C` | `o101` | `size-mismatch` | 2,100 | 464 | 461 | 4 | 4 | -4 | — |
| 394 | `src/overlays/o101/func_overlay_101_F0009190_18E49B0.c` | `func_overlay_101_F0009190_18E49B0` | `o101` | `size-mismatch` | 2,100 | 464 | 461 | 4 | 4 | -4 | — |
| 395 | `src/main/font.c` | `func_8004B1DC` | `main` | `size-mismatch` | 2,224 | 466 | 465 | 48 | 48 | -32 | — |
| 396 | `src/overlays/o101/func_overlay_101_F00078F4_18E3114.c` | `func_overlay_101_F00078F4_18E3114` | `o101` | `size-mismatch` | 2,100 | 472 | 469 | 4 | 4 | -4 | — |
| 397 | `src/overlays/o001/overlay_001_head.c` | `overlay1LoadBuildRecords` | `o001` | `size-mismatch` | 2,288 | 491 | 470 | 52 | 52 | -92 | — |
| 398 | `src/overlays/o061/func_overlay_061_F0000B84_18BFF4C.c` | `func_overlay_061_F0000B84_18BFF4C` | `o061` | `size-mismatch` | 2,548 | 499 | 476 | 0 | 0 | -64 | — |
| 399 | `src/overlays/o008/overlay_008.c` | `func_overlay_008_F00034A0_18611F8` | `o008` | `size-mismatch` | 3,592 | 506 | 482 | 28 | 28 | -4 | — |
| 400 | `src/overlays/o035/func_overlay_035_F0000B40_1882820.c` | `func_overlay_035_F0000B40_1882820` | `o035` | `size-mismatch` | 2,112 | 486 | 486 | 4 | 4 | -48 | — |
| 401 | `src/main/objects.c` | `func_8000B3CC` | `main` | `size-mismatch` | 1,976 | 489 | 489 | 0 | 0 | -268 | — |
| 402 | `src/main/track.c` | `func_8000E920` | `main` | `size-mismatch` | 2,168 | 491 | 491 | 56 | 56 | 64 | — |
| 403 | `src/overlays/o026/func_overlay_026_F00001A0_187A598.c` | `func_overlay_026_F00001A0_187A598` | `o026` | `size-mismatch` | 2,424 | 510 | 510 | 156 | 156 | -16 | — |
| 404 | `src/overlays/o019/overlay19BuildPlanes.c` | `overlay19BuildPlanes` | `o019` | `size-mismatch` | 2,128 | 512 | 512 | 72 | 72 | -40 | — |
| 405 | `src/main/track.c` | `func_8001291C` | `main` | `size-mismatch` | 2,192 | 527 | 527 | 0 | 0 | -20 | — |
| 406 | `src/overlays/o043/func_overlay_043_F0000324_188A2F4.c` | `func_overlay_043_F0000324_188A2F4` | `o043` | `size-mismatch` | 2,240 | 529 | 529 | 64 | 64 | -12 | — |
| 407 | `src/overlays/o056/overlay_056.c` | `func_overlay_056_F00001A0_18A2F18` | `o056` | `size-mismatch` | 2,324 | 530 | 530 | 80 | 80 | 8 | — |
| 408 | `src/overlays/o011/func_overlay_011_F0000150_1868998.c` | `func_overlay_011_F0000150_1868998` | `o011` | `size-mismatch` | 2,248 | 538 | 535 | 0 | 0 | -120 | — |
| 409 | `src/overlays/o047/func_overlay_047_F0000000_1890E18.c` | `func_overlay_047_F0000000_1890E18` | `o047` | `size-mismatch` | 2,512 | 553 | 548 | 104 | 200 | 16 | — |
| 410 | `src/main/shadows.c` | `func_80016890` | `main` | `size-mismatch` | 2,224 | 553 | 553 | 4 | 4 | 28 | — |
| 411 | `src/overlays/o090/overlay_090.c` | `func_overlay_090_F00000FC_18D4BF4` | `o090` | `size-mismatch` | 2,592 | 575 | 575 | 0 | 0 | -36 | — |
| 412 | `src/overlays/o012/func_overlay_012_F0000910_186DB90.c` | `func_overlay_012_F0000910_186DB90` | `o012` | `size-mismatch` | 2,444 | 581 | 581 | 0 | 0 | -20 | — |
| 413 | `src/overlays/o001/func_overlay_001_F0001D78_184E158.c` | `func_overlay_001_F0001D78_184E158` | `o001` | `size-mismatch` | 2,508 | 587 | 584 | 0 | 0 | -112 | — |
| 414 | `src/overlays/o057/func_overlay_057_F0001020_18A4C18.c` | `func_overlay_057_F0001020_18A4C18` | `o057` | `size-mismatch` | 2,392 | 585 | 585 | 0 | 0 | -32 | — |
| 415 | `src/overlays/o045/func_overlay_045_F0001158_188D5B0.c` | `func_overlay_045_F0001158_188D5B0` | `o045` | `size-mismatch` | 2,696 | 593 | 593 | 0 | 0 | 28 | — |
| 416 | `src/overlays/o057/func_overlay_057_F0001AE8_18A56E0.c` | `func_overlay_057_F0001AE8_18A56E0` | `o057` | `size-mismatch` | 3,532 | 645 | 614 | 4 | 352 | -132 | — |
| 417 | `src/overlays/o101/overlay101TailAB4C.c` | `func_overlay_101_F000AB4C_18E636C` | `o101` | `size-mismatch` | 2,552 | 636 | 636 | 4 | 4 | 108 | — |
| 418 | `src/main/anim.c` | `func_800563B4` | `main` | `size-mismatch` | 2,596 | 637 | 637 | 28 | 28 | -160 | — |
| 419 | `src/overlays/o057/func_overlay_057_F0004E18_18A8A10.c` | `func_overlay_057_F0004E18_18A8A10` | `o057` | `size-mismatch` | 4,832 | 718 | 663 | 0 | 0 | -16 | — |
| 420 | `src/overlays/o065/overlay65UpdateParticles.c` | `overlay65UpdateParticles` | `o065` | `size-mismatch` | 2,880 | 696 | 689 | 12 | 12 | -40 | — |
| 421 | `src/main/vehicle_sounds.c` | `func_8005830C` | `main` | `size-mismatch` | 3,048 | 700 | 699 | 0 | 0 | -16 | — |
| 422 | `src/overlays/o053/func_overlay_053_F0000240_189DBE8.c` | `func_overlay_053_F0000240_189DBE8` | `o053` | `size-mismatch` | 2,544 | 710 | 709 | 0 | 0 | 308 | — |
| 423 | `src/overlays/o058/func_overlay_058_F00005FC_18AF7E4.c` | `func_overlay_058_F00005FC_18AF7E4` | `o058` | `size-mismatch` | 3,316 | 731 | 724 | 0 | 0 | -12 | — |
| 424 | `src/overlays/o073/func_overlay_073_F0000190_18CAC50.c` | `func_overlay_073_F0000190_18CAC50` | `o073` | `size-mismatch` | 3,040 | 756 | 755 | 16 | 16 | 24 | — |
| 425 | `src/overlays/o065/func_overlay_065_F0000C38_18C4EA0.c` | `func_overlay_065_F0000C38_18C4EA0` | `o065` | `size-mismatch` | 3,548 | 824 | 824 | 48 | 48 | -108 | — |
| 426 | `src/main/anim.c` | `func_80053868` | `main` | `size-mismatch` | 4,820 | 1,166 | 1,165 | 0 | 0 | -268 | — |
| 427 | `src/overlays/o001/overlay_001_tail.c` | `func_overlay_001_F000438C_185076C` | `o001` | `size-mismatch` | 6,168 | 1,203 | 1,196 | 36 | 36 | -4 | — |
| 428 | `src/overlays/o054/overlay54TailA.c` | `func_overlay_054_F00005AC_189F24C` | `o054` | `size-mismatch` | 6,376 | 1,417 | 1,414 | 0 | 0 | -24 | — |
| 429 | `src/overlays/o101/func_overlay_101_F0003A58_18DF278.c` | `func_overlay_101_F0003A58_18DF278` | `o101` | `size-mismatch` | 5,844 | 1,421 | 1,421 | 52 | 52 | 8 | — |
| 430 | `src/overlays/o052/overlay52TailB.c` | `func_overlay_052_F000063C_189ACAC` | `o052` | `size-mismatch` | 6,748 | 1,598 | 1,595 | 304 | 304 | -12 | — |
| 431 | `src/main/anim.c` | `func_800517E0` | `main` | `size-mismatch` | 7,232 | 1,786 | 1,785 | 0 | 0 | -344 | — |
| 432 | `src/overlays/o060/overlay60Prefix.c` | `func_overlay_060_F0000334_18BA10C` | `o060` | `size-mismatch` | 11,156 | 2,519 | 2,511 | 4 | 4 | -12 | — |
| 433 | `src/overlays/o058/func_overlay_058_F000138C_18B0574.c` | `func_overlay_058_F000138C_18B0574` | `o058` | `size-mismatch` | 14,456 | 3,696 | 3,693 | 0 | 0 | 532 | — |

### Unresolved identities

None.
<!-- NM_RANKING_GENERATED_END -->

## Recommended batching for the fleet

1. Check complete ranking freshness and the current lane/plateau ledger before
   assigning work. A near-exact residual with an exhausted attempt history is
   not a ready target until a new mechanism or authorized bounded sweep exists.
2. Use register/schedule categories to choose the next diagnostic tool, not to
   promise cheap wins. Preserve exact extent, frame, relocation evidence, and
   prior flat experiments alongside the scalar word score.
3. Group independent work by useful shared context while keeping symbol
   ownership disjoint. Prefer measured new levers and source-hash-new sweeps;
   stop bounded attempts with a reproducible plateau.
4. Separate relocation/boundary reproof from creative source reconstruction.
   A zero object score still needs real-offset linked proof. Structural work
   receives an explicit budget and stop condition under ADR 0009.

## Authored campaign notes

The observations below are retained campaign context, not generated ranking
rows. Current queue membership and measurements live only in the marked region
above and its JSON source.

At the time of its recorded investigation, `overlay1FindBestRecord` was a
size-exact register-allocation near miss that the snapshot classified as
`other`: the extracted target
object omits the selected-type HI16/LO16 pair that the shipped runtime table
and candidate both retain. Current configured full-TU V0 is frameless and
18/30 words with 12 `a1`/`a3` pool-register differences from `+0x04`; it
belongs beside the 12-word rows operationally. All 119 flag configurations
were attempted, seven O2/MIPS-II rows tie V0, and none is exact. A
fidelity-clean proc-38 allocator trace plus all three permitted natural
declaration/scope forms leave the same object, so the fallback is parked
pending a new allocator-order mechanism. The earlier exact claim depended on
prohibited post-compile field edits and remains inadmissible.

Nine rows from that historical run were subsequently promoted and ceased to
be search candidates: `overlay3FindClosestObject`, `overlay40AddEntry`,
`overlay43SubmitChildren`, `func_80038750`, `partUpdateTriggers`,
`func_8001A154`, `overlay1UpdateValueCache`,
`func_overlay_041_F0000000_1887338`, and `overlay80UpdateContact`. Their
canonical source and function-specific ledgers carry the exact proofs; stale
generated ranking entries must not put them back into the ready queue.

`func_80021504` is unguarded matched C and no longer a ranking candidate.
Retained configured C owns 133 words with frame `0x28` and 43 candidate
relocation tuples. Its linked range, complete camera TU, and resident `.main`
section are byte-identical to ROM. The retained full `.bin` predates the
object and independent target relocation metadata is absent, so this is a
reproof-only integrity target rather than living search.

`func_80021718` is already canonical C: retained configured C owns 37
words, frame `0x28`, and 14 candidate relocation tuples. Those tuples
reproduce all 37 linked ELF words, and the post-object linked range and
complete camera TU are byte-identical to ROM. No caller is proven;
ROM-table row 453 is an unreferenced export, not inbound evidence. The
full `.bin` predates the object and independent target relocation metadata
is absent, so this remains a reproof-only target rather than living search.

`func_800219D0` is a reproof-only matched function, not a ranking candidate.
The retained pre-comment configured object owns 104 words with frame `0x8`
and eight HI16/LO16 records. Its raw function agrees with the retained
`NON_MATCHING=1` object, and its linked range, complete camera TU, and resident
`.main` section are byte-identical to ROM. The later canonical change added
comments only, so no source/codegen search is warranted. The exact retained
whole `.bin` predates the object and is historical rather than causal proof;
fresh current-source object, link, and full-bin comparisons remain due.

`func_800320F0` (the function formerly routed under the JFG donor alias
`runlinkEnsureJumpIsValid`) has since been promoted: retained canonical C is
101 words with 21 relocations, and its linked ROM range
`0x32CF0..0x32E84` is byte-identical. It is no longer an unmatched ranking
candidate.
