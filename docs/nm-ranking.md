# tools/nm_ranking.py: ranking the NON_MATCHING queue by closeness

~300 KB of code across ~90 overlays and several `src/main` TUs sits behind
`#ifdef NON_MATCHING` (the compile-only escape hatch documented in the
Makefile and `docs/acceleration-survey.md` sec.13.2): a real C body compiled
under `gmake NON_MATCHING=1`, but not yet byte-identical, so the tree still
ships the `#pragma GLOBAL_ASM` fallback under normal `gmake`. Some of those
candidates are one register swap from matching; others are structurally
wrong. This tool ranks every queued function by how close its candidate
already is, so a fleet of workers can spend its time on near-misses first
instead of triaging the whole queue by hand.

## Method

### Two build paths, and why only one covers the whole queue

The natural approach -- `gmake NON_MATCHING=1 -j`, diff the resulting
`build_non_matching/` tree against a verified `expected/build/` snapshot
with objdiff-cli -- works, but not for the whole queue. 590 of this
project's C objects carry a Makefile `POSTPROCESS` override, overwhelmingly
`trim_elf_section.py`: it shortens a `.text` section header after the fact
so a function that ends on a 4-byte boundary inside a larger overlay module
doesn't drag the next subsegment's start with it. That trim step asserts
the post-compile object is *exactly* the size it was proved to be at the
matched target's size. Under `NON_MATCHING=1`, a queued function in the
same translation unit as a trimmed one compiles to its real (larger, still
wrong) size, and the trim step fails outright:

```
overlay_055/func_overlay_055_F000031C_18A1E34.c.o: cannot grow .text from 0x900 to 0x914
```

A full `gmake NON_MATCHING=1 -k -j` run hits this on most overlay TUs that
still have a queued function (~100 distinct `Error 1`s in this run). It
doesn't stop the build -- `-k` keeps going -- but every such object is
absent or stale under `build_non_matching/`, and this repository's Makefile,
`src/`, and `docs/modules.md` are out of this lane's scope, so the trim
step itself isn't something this tool can fix.

So the ranking's primary source is a **per-function isolated compile**,
which sidesteps the Makefile path entirely: for each queued function,
`tools/permute_batch.py`'s own machinery (`write_settings_toml`,
`prepare_target_asm`, `run_import`) is reused to run the same
asm-processor + IDO invocation the permuter itself runs, producing two
function-comparable objects:

- `target.o` -- the function's own `asm/nonmatchings/**/<f>.s`, assembled
  directly. This *is* the ROM's bytes; splat only stops emitting a
  function's nonmatchings `.s` once C actually implements it, so as long as
  the tree is still in `#ifdef NON_MATCHING` form the `.s` is authoritative
  (the same ordering constraint `tools/wb_compare.sh` documents).
- `base.o` -- normally the `#ifdef NON_MATCHING` C body isolated by
  permuter import and compiled with the Makefile-expanded recipe recovered by
  `tools/permute_batch.py`'s `build_recipe_for`, including per-file optimizer,
  ISA, and backend overrides. Static flag groups are only the explicit
  fail-loud fallback when that recipe cannot be recovered.

If import cannot compile the pruned source, the tool writes an untracked TU
copy that selects only this C body and retains every other function's ASM
fallback. It dry-runs the real `NON_MATCHING=1` Makefile rule and executes
only its raw asm-processor/IDO command, preserving TU-specific flags while
skipping POSTPROCESS. Consolidated candidates whose private typed externs
were lost use their last tracked pre-consolidation declaration wrapper, but
only after the current and historical candidate bodies compare identical.
Function offsets and relocations are then normalized out of that larger
object before comparison.

The failure classes fixed in this pass were: nested conditional directives;
atlas rows that name a consolidated TU rather than a function; a function
name spelled through an object-like macro; auxiliary text/data labels and
jump tables in target assembly; import pruning failures on complex C, static
symbols and local rodata; missing private declaration context after TU
consolidation; exact per-TU flag overrides; and a concurrent-import race that
removed the shared scratch parent directory.

The isolated compile is also, on its own terms, a closer proxy to "how much
work is left" than a whole-TU diff would be: it isolates exactly the
function under test, so a mismatch can't be masked or amplified by drift
elsewhere in a large multi-function overlay module.

### objdiff-cli, as supplementary context

`tools/gen_objdiff_config.py` gained a `--base-dir` option so its unit list
can point at `build_non_matching/` instead of `build/`, target side
unchanged (`expected/build/`, from a `gmake verify`-clean normal build --
the two trees mirror each other's relative paths one-for-one below the top
directory, so no other change was needed). Run against whatever fraction of
`build_non_matching/` actually built, objdiff-cli's `report generate -d`
returns a `fuzzy_match_percent` per function within the units it can parse,
which this tool reads into `objdiff_match_pct` by name.

Coverage here is partial in both directions this run covers:

- Only units that survived the `NON_MATCHING=1 -k` build at all (see
  above) can be in the report.
- objdiff-cli's ELF reader aborts its whole batch on the first
  POSTPROCESS-trimmed object it can't parse (`Section symbol without
  section`, or an unattributed `Symbol data out of bounds` with no file
  name to key a targeted exclusion off of -- `docs/tools.md`'s own
  documented scope limit). The brief for this lane suggested most overlay
  POSTPROCESS steps are metadata-only now and worth trying to include; a
  bisection over the ~590 POSTPROCESS-tagged objects (recursively
  re-including halves and retrying) was attempted to find the actual
  parseable subset rather than excluding all of them by blanket rule, but
  it did not converge in the time budget for this run and was killed. This
  run falls back to the same blanket exclusion `tools/objdiff_report.sh`
  itself uses (every Makefile `POSTPROCESS`-tagged object, ~590 names,
  regenerated from the Makefile, never committed -- see that script's own
  comment on why). A future run with more time, or a narrower per-overlay
  bisection, could recover more of the difference; the mechanism
  (`--base-dir`) is already in place for it.

The committed refresh omits `--objdiff-report`, so its
`objdiff_match_pct` values are `null`. Since that field is supplementary
context (it reflects the function in its real linked/whole-TU context) and
`differing_words`/`category` come from the isolated compile regardless,
every function is fully ranked either way.

### Word diff and categorization

For each resolved function, both `.text` sections are read as big-endian
32-bit words (raw, in memory only -- see "Clean-room note" below) up to
each side's own symbol size:

- **`size_bytes`**: the target (ROM) function's size.
- **`size_delta`**: `base size - target size`.
- **`differing_words`**: word positions that disagree over the shared
  prefix, plus every word past the shorter side's end.
- **`first_mismatch_offset`**: the byte offset of the first disagreeing
  word (or the shared length, if one side is a truncated/extended prefix
  of the other).
- **`category`**, in this precedence order:
  1. **`size-mismatch`** -- `size_delta != 0`. Nothing else is checked;  a
     size mismatch means the two objects aren't even comparable word-for-word
     past the point of divergence.
  2. *(equal size, no differing words at all)* -- reported as `other` with
     `differing_words: 0`: the isolated candidate is already
     byte-identical to the target in isolation. This can happen even
     though the function is still `NON_MATCHING` in the full build, when
     whatever's blocking the real match is TU-level (surrounding
     function order, a flag difference the Makefile applies only in
     context, etc.) rather than in the function's own body.
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
  5. **`reloc-mismatch`** -- equal size, and every differing word's offset
     carries a relocation entry (`objdump -r .text`) on the base or the
     target side.
  6. **`other`** -- equal size, everything else. This is the largest
     bucket in this run and is exactly the "needs a human/model
     look, no cheap mechanical explanation available" category.

Sort order for the table and the JSON: category rank (`register-only` <
`schedule-only` < `other` < `reloc-mismatch` < `size-mismatch`), then
`differing_words` ascending within a category. Register-only and
schedule-only mismatches are typically single flag-lattice or `mips_to_c`
tweaks; size and reloc mismatches usually mean a structural rewrite.

### Clean-room note

No instruction word, mnemonic, or hex byte from a decoded `.text` section
is ever written to `config/nonmatching-ranking.us.json`, printed, or
otherwise leaves the running process -- every decode exists only long
enough to produce a count (`differing_words`) or an offset
(`first_mismatch_offset`) before being discarded. `objdiff_match_pct` is a
float already computed by objdiff-cli, not ROM content.

## Reproducing this run

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
.venv/bin/python tools/nm_ranking.py --top 20 --markdown --no-table  # fleet-prompt excerpt
```

A complete pass can run long enough for another lane to promote functions
that were queued at startup. Immediately before publishing, the ranking tool
re-scans the canonical source and drops resolved and unresolved rows whose
`#ifdef NON_MATCHING` block no longer exists. The checked-in JSON is still a
historical snapshot after the ranking process exits; consumers that need the
current queue must intersect it with a fresh source scan.

## Distribution (this run)

456 queued functions (`tools/permute_batch.py discover_queue()`, validated
atlas + depth-aware source-scan union); 456 resolved, zero unresolved. Total
resolved size: 405,632 bytes (396.1 KiB) across 80
distinct overlays plus `src/main`/`src/libultra` TUs.

| Category | Count | Share of resolved |
|---|---:|---:|
| `register-only` | 23 | 5.0% |
| `schedule-only` | 4 | 0.9% |
| `other` | 223 | 48.9% |
| `reloc-mismatch` | 19 | 4.2% |
| `size-mismatch` | 187 | 41.0% |

Differing-word thresholds (resolved functions, any category):

| Threshold | Count |
|---|---:|
| `differing_words <= 5` | 43 |
| `differing_words <= 10` | 90 |
| `differing_words <= 20` | 150 |

`objdiff_match_pct` is `null` for this refresh because no supplementary
objdiff report was supplied.

## Recommended batching for the fleet

1. **`register-only` and `schedule-only` first (27 functions)** -- the
   cheapest category by construction: the candidate already has the right
   instructions, just the wrong register allocation or schedule. These are
   flag-lattice/`mips_to_c` tweaks, not rewrites, and most are also small
   (`differing_words <= 20` for all but a handful). Assign these
   individually; a single worker can plausibly clear several per session.
2. **`other` with `differing_words <= 10` next.** These are structurally
   close (same size, no mechanical explanation available) but small enough
   that a worker can read the whole diff by hand quickly. Batch by overlay
   where more than one queued function shares a TU, since the worker
   already has that file's context loaded.
3. **`reloc-mismatch` (19 functions).** Usually a symbol/addend binding
   question (wrong `%hi`/`%lo` target, or a static vs. extern reference) --
   narrow enough to hand off as its own small batch, distinct skill from
   the register/schedule fixes.
4. **`size-mismatch` last, sorted by `differing_words` ascending within
   the category.** The best `size-mismatch` candidates (`differing_words`
   near the low end of that bucket) are worth a look before the worst
   `other` candidates, but the category itself signals a structural
   rewrite (an inlined helper, a duplicated/dropped instruction, a
   different loop shape) rather than a tweak -- expect these to need more
   than one attempt, and route them to workers/models with more budget per
   function (`docs/adr/0009` model-routing).
5. **No isolation backlog remains.** All discovered functions participate
   in the same sorted fleet queue.

## Living register-only near misses (retained snapshot)

These are the latest audit-corrected configured full-TU or isolated
measurements. The sortable `config/nonmatching-ranking.us.json` remains the
generated historical snapshot; regenerate it when compile capacity is free.

| name | overlay/TU | differing_words | first_mismatch_offset | size | size_delta | category |
|---|---|---:|---:|---:|---:|---|
| `overlay40FadeRecords` (historical scalar; object absent, HEAD pending) | o040 | 3 | 12 | 404 | 0 | register-only |
| `func_8003A2C8` (pre-cleanup diagnostic spelling) | main | 5 | 12 | 128 | 0 | register-only |
| `overlay74Update` (pre-cleanup diagnostic; clean score unknown) | o074 | 6 | 12 | 400 | 0 | register-only |
| `overlay20UpdateObjectResource` (canonical isolated scalar; stale local configured object, HEAD pending) | o020 | 8 | 176 | 392 | 0 | register-only |
| `func_8002CF6C` (pre-cleanup diagnostic; clean score unknown) | main | 9 | 204 | 352 | 0 | register-only |
| `overlay19ClassifyEdge` (current-body isolated scalar; object/full-TU context absent) | o019 | 10 | 312 | 480 | 0 | register-only |
| func_overlay_079_F0001290_18CE230 | o079 | 12 | 200 | 492 | 0 | register-only |
| `func_80020D8C` (prior-layout masked diagnostic) | main | 13 | 56 | 192 | 0 | register-only |
| `overlay1FindType5ByKey` (pre-HEAD/current-body V0) | o001 | 17 | 28 | 156 | 0 | register-only |
| func_overlay_041_F0000000_1887338 | o041 | 26 | 84 | 292 | 0 | register-only |

All ten remaining rows are `register-only` and size-exact -- the cheapest tier the queue
has to offer right now.

`overlay1FindBestRecord` is another size-exact register-allocation near miss,
but the historical generated snapshot classifies it as `other`: the extracted
target object omits the selected-type HI16/LO16 pair that the shipped runtime
table and candidate both retain. Runtime-normalized comparison is 18/30 words
with 12 `a1`/`a3` carrier differences from `+0x04`; it belongs beside the
12-word rows operationally. Historical isolated and prior-layout full-TU C
agree, but current-layout C is uncompiled and every current metric is unknown.
The earlier exact claim depended on prohibited post-compile field edits. Run
119 configurations, one trace, and at most three trace-supported forms, then
park if all three are flat.

Seven other rows from the displayed run have since been promoted and are no
longer search candidates: `overlay3FindClosestObject`, `overlay40AddEntry`,
`overlay43SubmitChildren`, `func_80038750`, `partUpdateTriggers`,
`func_8001A154`, and `overlay1UpdateValueCache`. Their canonical source and
function-specific ledgers carry the exact proofs; stale generated ranking
entries must not put them back into the ready queue.

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
