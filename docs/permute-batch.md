# tools/permute_batch.py: batch-running the permuter over the NON_MATCHING queue

`docs/adr/0007-matching-tools.md` requires decomp-permuter to run only as a
**bounded batch job**, never inside an agent's own reasoning loop. This tool
is that job runner: it enumerates every queued `NON_MATCHING` function,
imports and permutes each one under a wall-clock cap, and reports what came
back -- score, whether a zero-diff candidate turned up, and (opt-in) whether
that candidate survived promotion into a real, verified match. See
`docs/tools.md` for the underlying decomp-permuter/objdiff setup this
builds on, and `tools/permute.sh` for the single-function version this
generalizes (this script does not shell out to `permute.sh`; it reimplements
the same import/permute steps inline so it can handle the overlay naming
quirk below and the `#ifdef NON_MATCHING` queue shape, which `permute.sh`
was not written for).

## Usage

```sh
# see what's queued, without running anything
tools/permute_batch.py --list

# one function, report-only (default: no source is touched)
tools/permute_batch.py --function overlay1GetEntry --minutes 12

# one function, and splice + verify a zero-score candidate if found
tools/permute_batch.py --function overlay1GetEntry --minutes 12 --apply

# a whole overlay, 3 functions at a time, capped at 15 min each
tools/permute_batch.py --overlay 9 --jobs 3 --minutes 15 --apply

# the first 20 queued functions project-wide
tools/permute_batch.py --limit 20 --jobs 4 --minutes 15 --apply

# extra permuter.py flags go after --
tools/permute_batch.py --function overlay1GetEntry -- --best-only

# overlay functions only, with the relocation-annotated target that makes
# their score mean something (see "Overlay targets carry no relocations")
tools/permute_batch.py --overlays-only --order ranking --minutes 15 --apply

# omit explicitly exhausted plateaus from this run (one symbol per line;
# blank lines and whole-line comments are allowed)
tools/permute_batch.py --exclude-file parked-functions.txt --list
```

`--jobs` is concurrent *functions*; each function's own `permuter.py`
still gets its own `-j <threads>` (`--permuter-threads`, default: split
`ncpu - 2` evenly across `--jobs` so the total thread count stays around
`ncpu - 2` regardless of how many functions run at once). `--build-jobs`
is the separate `gmake -jN` used only when `--apply` promotes a zero-score
result (default: `ncpu`, since a promotion rebuild is not concurrent with
another function's permuter run within the same `--jobs` slot... though it
*can* overlap with another slot's permuter run, so don't set `--build-jobs`
to the full core count if also running `--jobs > 1` with `--apply` -- the
two searches don't currently coordinate CPU budget with each other's
promotion rebuilds).

Every run (even a `--list`-free normal run) writes `build/permuter/summary.json`
and `build/permuter/summary.txt` incrementally, one function at a time, so a
killed batch still leaves a readable partial result. Each attempted import
gets a fresh `build/permuter/<function>/runs/<run-id>/` directory; a retry does
not overwrite an earlier candidate or its diagnostics.
Any preexisting `nonmatchings/<function>` scratch is moved into that run as
`preexisting-import-<id>`, and interrupted or failed imports are retained as
`failed-import-<id>`. These directories may contain manual best candidates;
the runner never deletes them on retry.

### Durable search receipts

`--resume` consults content-addressed receipts under
`$(git rev-parse --git-common-dir)/mickey-sweep-receipts/v3/`. Successful
search knowledge survives a lane's removal and is available to other lanes
through this common directory. The old local summary remains a report and
never decides whether a search can be skipped.

Before looking up a receipt, the runner imports and compiles the baseline.
Its key includes the function's source path, symbol, overlay, `.text` offset,
and ROM offset; the original TU; actual preprocessed/pruned source, compile
script, annotated target and scratch settings; the complete recovered IDO
argument list; toolchain and permuter content hashes; and the search caps,
thread count, annotation mode and forwarded arguments. Source, target and
settings are hashed byte-for-byte, including expanded path literals. The
baseline evidence binding also hashes the raw saved compile script. The importer
object is an output, not a search input: IDO's `.mdebug` embeds a unique
preparation path, so that object is retained and bundle-verified without entering
the search key. Absolute script paths therefore conservatively separate lane keys,
even when known cwd prefixes normalize alike in the older command fingerprint.
Durable evidence remains recoverable across lanes; cross-lane skip hit rate is
deliberately secondary to exact input binding. A changed header that changes preprocessed
source, compiler, target, setting, or source context therefore schedules a new
search. Resident identities use their linked text address as the offset.

This preparation cost deliberately remains on resumed entries: it verifies
the effective inputs instead of guessing which includes or preprocessing
switches mattered. `--list --resume` only lists entries awaiting that check;
it does not compile or promise that every displayed entry will be searched.
In an executing resumed or deep batch, skipped receipts do not consume
`--limit`; repeated limited passes therefore advance to unsearched entries.

Each exact key has a nonblocking kernel lock. A duplicate active search is
reported as busy and can be retried; a crashed process releases its lock.
Completed and best scalar receipts are written atomically, with immutable
per-attempt records. A separate worktree lock prevents two batches from
sharing one lane's importer scratch or summary. Scalar receipts reference
immutable SHA-256-addressed ZIP bundles in that same ignored common directory.
Bundles retain the original importer preparation files, original TU,
target, complete recipe/settings, saved compile script, best source/object,
and the attempt's local files (including failed/partial outputs and logs).
None of these private artifacts is tracked by Git. Older scalar-only or
unbound bundles cannot suppress a new search.

The importer's `base.c` and `base.o` are explicitly **not claimed as a compiled
pair**: import compiles different AST-rendered text with a line directive, and
the final scratch recipe may add later postprocessing. An owned transparent
compile wrapper captures the first actual synchronous search compilation's
unchanged temporary input and output as `baseline/compiled.c` and `compiled.o`.
It forwards the original compiler arguments to the final saved recipe, and the
measurement record associates that pair with the search's first baseline score.
Subsequent candidate and extension compilations cannot replace that capture.
No vendor files are changed. This capture relies on the inspected permuter API
(baseline construction precedes worker search and uses `permuter*` temporary
files); unsupported or incomplete captures fail closed.

The permuter normally saves best source without its object. The runner compiles
those unchanged bytes once through the saved compile script, under the remaining
whole-batch deadline. This is evidence compilation, not instruction editing or
promotion proof. Its output path is precreated, as required by the importer's
`realpath`-based script. Best-object metadata labels this as a diagnostic
recompilation, not the original worker-scored object: path-sensitive behavior
such as `__FILE__` has not been proved equivalent. Extensions cannot replace
the archived original baseline.
Compile failure, cancellation or deadline exhaustion preserves partial source
evidence but leaves the search retryable. Preservation allows 120 MiB payload
within a 128 MiB encoded archive, at most 4096 entries, and 512-byte member names;
storage faults or oversized attempts fail closed and retain the lane-local
originals for manual recovery. Bounded filesystem preservation may continue
after the search deadline; its single directory scan has a five-second deadline
and stops immediately on byte/entry exhaustion. A failed publication never
starts a second full scan. It launches no compiler after the search deadline.
Filesystem calls cannot be forcibly interrupted, and hard kill or power loss
before atomic publication still has no durability guarantee.

Both resume and descending-context selection verify the bundle's content hash,
manifest, required files and each member hash. The manifest is bound to the
exact receipt key, context, and baseline input hashes; substituting a valid
bundle from another search also fails. Missing/corrupt bundles reject
reuse. Writes publish atomically without replacing an existing bundle; concurrent
identical writers converge on the same immutable content. Symlink and path-escape
inputs are rejected, and archives are never automatically extracted or executed.
For explicit recovery, `ReceiptStore.read_bundle(bundle_hash)` returns a verified
mapping of relative names to bytes; `require_complete=False` permits inspection
of partial evidence. Copy only selected entries to a new owned directory. Saved
scripts retain their original context paths and are evidence, not portable
executables: a fresh lane still prepares its current inputs before reuse.

Only a successful bounded search is reusable. Import/compile faults, nonzero
permuter exit, a missing base score, whole-batch interruption, failed promotion,
and failed commit all remain retryable. A zero-score result without verified
promotion remains retryable as well. The best scalar result references its
durable bundle in `best.json`; failed, partial or unavailable lower-score
attempts are retained separately in `partial-best.json` and immutable attempt
records, without replacing the usable-best pointer. The scratch path remains diagnostic and is not needed to read preserved
evidence after lane removal. A receipt records search completion, not matching credit: the ordinary
relocation, linked-range, canonical-build and ROM gates still decide promotion.

`--deep` selects contexts with a successful descending receipt under any
previous caps, so longer follow-up searches still require current source and
tool identities. `--resume` with the same caps skips completed searches; omit
it to deliberately rerun the same inputs with a new random search.

The queue also has a mandatory outer wall-clock bound: `--max-total-minutes`
defaults to 120 minutes. At that deadline every active permuter process group
is terminated through the same TERM/KILL cleanup used by the per-function
cap, and no further queue entries are scheduled. Parallel mode only keeps
`--jobs` entries in flight instead of submitting the entire queue up front.
The summary marks interrupted searches with `stopped_batch`, and `--resume`
retries interrupted entries as well as entries the bounded pass did not
reach. Preparation, promotion proofs and Git hooks also observe the remaining
batch time. SIGINT/SIGTERM cancel scheduling and active commands. Process-group
cleanup includes child workers after a failed parent exits; it allows up to
15 seconds for TERM before KILL. A failed promotion then has a separate
30-second recovery grace, without starting another full build.
Set a larger positive
outer cap deliberately for a longer attended pass; zero and negative values
are rejected because this runner's purpose is bounded search.

## Scratch fidelity (2026-08-28)

A permuter score-0 is only a match if the scratch object it was scored
against is bit-identical to the object the project build produces for that
TU. Three fidelity faults were found in `tools/permute.sh` on 2026-08-27
(`docs/matching-triage.md`) and are now applied by this runner too:

| Fault | Effect before | Fix in `permute_batch.py` |
|---|---|---|
| importer default `-mips1`, static flag groups | searched the wrong ISA; per-file flags, defines, and includes dropped | `build_recipe_for()` reads the complete real IDO argument tail from `gmake -n -W <source> <obj>` without touching source timestamps; batch searches reject unsupported wrappers and guessed fallback recipes |
| no post-compile `objcopy --redefine-sym` | track.c results never transferred | the TU's objcopy chain is appended to the scratch `compile.sh`, retargeted at `$OUTPUT`; digest-guarded `.py` passes are skipped and listed in `build/permuter/<fn>/recipe.txt` |
| scorer normalises stack offsets | false 0 on a spill at the wrong slot | `--stack-diffs` is always passed |

Branch destinations are also mandatory scoring inputs. The vendor CLI defaults
to ignoring them, even though its programmatic options default to checking
them. The runner always passes `--no-ignore-branch-targets`, including extension
runs, and records it in the search receipt. A current resident replay retained
exact instruction count and frame but scored zero only while its one incorrect
branch destination was ignored; strict scoring correctly retained a nonzero
residual. Old relaxed searches do not establish strict-search completion.

The 2026-08-25 farm result in "Cost and match rate" below (0 hits in 38
searches) predates all three fixes and is not evidence about the queue.

Other runner behaviour added at the same time: `--order ranking` (default)
runs the closest functions first by `config/nonmatching-ranking.us.json`
`differing_words`; `--resume` skips only identical completed search receipts
after baseline preparation and carries report rows forward;
`--extend-minutes N` re-seeds from the best candidate and runs once more when
a capped search was still descending (best result in the last third of the
window); `--load-threshold L` (default 9) waits for headroom before every
permuter launch and promotion build; `--commit` (with `--apply`) commits each
verified promotion as `Match <fn> (permuter)`, including its changed derived
metadata and plateau retirement through an isolated index. The
permuter is niced. Batch infrastructure, promotion and commit failures produce
a nonzero exit status instead of being reported as successful searches.

### Safe sweep wrapper

`tools/permute_sweep.sh` requires an explicit mode and lane name. The old
implicit-promotion invocation (`permute_sweep.sh LANE`) is rejected. Help,
including `-h` or `--help` anywhere in the arguments, exits before directory
resolution, Git calls, lane creation or builds. No arguments show help too.

```sh
# Report only: no candidate application or commits; no automatic extension.
tools/permute_sweep.sh --report-only my-sweep -- \
    --function myFunction --minutes 3 --extend-minutes 0 --max-total-minutes 10

# Opt in explicitly to the linked-proof and function-sized commit workflow.
tools/permute_sweep.sh --promote my-sweep -- --minutes 20
```

Batch arguments require the `--` separator. The production batch parser
validates them before any Git call; forwarded `--apply`, `--commit`, `--list`
and their accepted abbreviations are rejected. For queue listing, invoke
`tools/permute_batch.py --list` directly. Report-only mode never adds application
or commit flags; `--promote` adds both explicitly.

Lane names are limited to 1-64 lowercase letters, digits, underscores and
hyphens, with an initial letter or digit. Existing lanes must be registered
linked worktrees in the same repository, at the expected path (including the
normal `.noindex` alias), on exactly `lane/LANE`, with clean tracked state and
no index lock. Detached heads, foreign repositories/paths, orphaned branch
names, and divergent or unintegrated lane commits fail closed before resync.
Git directory/index environment overrides are unsupported. A missing lane is
created from the pinned integration commit; an existing lane only fast-forwards.
These checks assume the operator exclusively owns the named lane; they are not
a lock against a separate actor checking out another branch concurrently.

Both modes extract, warm-build and verify the base before searching. Only
promotion mode performs the final extraction/build/progress pass. Existing
resource defaults remain two searches, four permuter threads each, six build
jobs, a 20-minute search plus optional 20-minute descending extension, a
six-minute flat stop, a 120-minute whole-batch cap and load threshold 13.
Pass smaller caps explicitly when assigned. Prior scratch and uniquely named
logs are retained; the wrapper never resets histories or cleans user files.

Regression checks for these guarantees are
`python3 tools/test_sweep_receipts.py`,
`python3 tools/test_permute_batch_deadline.py` and
`python3 tools/test_promotion_transaction.py`. Wrapper CLI checks are
`python3 tools/test_permute_sweep_cli.py`; they use the real argument parser
and disposable Git worktrees with synthetic build/search subprocess stubs.
These checks use disposable repositories,
synthetic compiler settings and short-lived test subprocesses, without a ROM.

## Queue discovery

Two sources, unioned:

1. `config/overlays.us.json`'s per-module `text_ownership` rows (written by
   `tools/overlay_atlas.py`), each carrying a mechanically-derived
   `"nonmatching"` flag -- true iff the source file for that byte range still
   has an `#ifdef NON_MATCHING` guard around the named function
   (`overlay_atlas.is_nonmatching_source`). This is authoritative for
   overlay functions once the atlas has been regenerated after a conversion.
2. A direct scan of `src/**/*.c` for `#ifdef NON_MATCHING` blocks, matched
   by function-definition shape. This catches anything not yet reflected in
   the atlas -- in practice, at the time this was written, two `src/main/`
   functions (`MatrixMultiplyVec4`, `ProcessRelocationEntry`) that predate
   the atlas's `nonmatching` flag and aren't overlay code at all, so the
   atlas will never carry a row for them.

Before ordering, the runner pins the local `campaign/unchain` ref to one commit
and checks the selected source paths there (override with `--integration-ref`;
disable with `--no-integration-ref-filter`). It does not fetch. A stale lane
entry is skipped only when that commit defines the same function unguarded and
its stable assembly fallback is absent everywhere under `src/`. Missing refs,
paths, moved/renamed fallbacks, and definitions the conservative parser cannot
identify remain queued. This prevents a long-running lane from rediscovering a
match integrated after the lane was created without treating absence as proof
of resolution. `--list` and explicit `--function` requests print each skipped
identity and the pinned commit.

`--overlay N` filters on the atlas's `overlay` number (source-scan-only
items outside `src/overlays/` have no overlay number and are excluded by
this filter); `--function NAME` and `--limit K` apply after that.

`--exclude-file PATH` is repeatable and removes exact function names before
ordering. It is deliberately opt-in: an exclusion is only operator scheduling,
not evidence that a function matches or should remain parked forever. Missing
files and malformed names are fatal instead of silently changing the run.

## The overlay naming quirk

splat auto-names every overlay function it disassembles from the ROM
`func_overlay_MMM_FOOOOOOO_ROMADDR` -- every overlay module shares one
synthetic VMA (`tools/overlay_atlas.py`'s `SYNTHETIC_VMA`), so spimdisasm
can't derive a unique name from the address alone. A converted function's
`#ifdef NON_MATCHING` branch already carries the friendly name a human (or
`TEXT_SUBSEGMENTS`) gave it, e.g. `overlay1GetEntry`; its `#else` branch's
`#pragma GLOBAL_ASM(...)` still points at the auto-named `.s` file, whose
`glabel` is the auto name, not the friendly one.

decomp-permuter's `import.py` derives the function name it hunts for in the
C source from the target `.s` file's `glabel` line. Handed the auto-named
`.s` directly, it looks for `func_overlay_...` in the (preprocessed) C and
fails with `"...not found in base.c"` even though the function is right
there under a different name. `permute_batch.py` works around this without
touching `import.py` (out of this lane's ownership) or `asm/` itself
(gitignored, never written to in place): it copies the target `.s` into the
function's own `build/permuter/<fn>/target.s` scratch file and renames the
`glabel`/`endlabel` pair to the friendly name before handing that copy to
`import.py`. That is a label rename -- metadata, not an instruction word --
exactly the class of edit `docs/adr/0002-no-post-compile-instruction-editing.md`
already permits for the project's own `objcopy --redefine-sym` POSTPROCESS
steps.

Some queue items don't hit this at all: `overlay16ApplyGradient` (overlay
16) does, but `func_overlay_028_F00004D8_187CDA8` (overlay 28) is still
carrying its auto name as the *friendly* name too (no human name assigned
yet), so the rename is a no-op there. The script handles both uniformly --
it always renames to whatever the `#ifdef NON_MATCHING` branch's own
function name is, whether or not that differs from the `.s` label.

## Overlay targets carry no relocations, and what to do about it (2026-08-28)

Until this section was written the runner shipped `--resident-only`, because
for an overlay function the permuter's score was not a measurement of
anything. The historical pre-`OSTime` `overlay18Load` candidate differed from
the shipped ROM by **two words** and scored **700**.

### Why

`docs/reloc-surface.md` §1: an overlay module ships *unrelocated*. What the
ROM stores at a relocation site is the record's **stored addend**, and every
`SYMBOL` `R_MIPS_26` record stores immediate zero. So splat's disassembly of
an overlay function assembles into a target object with **no relocations at
all**: each cross-module call reads back as a `jal` at the module's own
synthetic base (i.e. against whatever symbol happens to sit at module offset
0 -- often the function itself), and each address materialization as a bare
`lui`/`addiu` pair carrying the stored addend as a literal.

The candidate object, compiled from C, carries the honest thing: an
`R_MIPS_26` against `overlayNNCommonReloc`, a `%hi`/`%lo` pair against
`gOverlayNNSomething`.

decomp-permuter's scorer ignores a symbol-name difference only when the
candidate's field looks like a symbol **and the target line also carries a
relocation** (`scorer.py`: `field_matches_any_symbol(nf) and
old_line.has_symbol`). The target line carries none, so every relocation site
in the function was scored as a mnemonic-level `replace` -- one insertion and
one deletion penalty, 200 points a site. The score measured the *number of
relocation sites*, not the distance to the ROM.

### The fix: annotate the target with the module's own relocation table

`tools/reloc_surface.py`'s `permuter_annotation()` gives the scratch target
the relocations the shipped module says are there, and renames the
candidate's placeholders to match, so both sides render **identically** at
every corroborated site. `permute_batch.py` calls it from
`annotate_overlay_scratch()` right after `import.py`, for overlay functions
only.

For each relocation the candidate's own base object carries inside the
function, the site's object offset maps to a module offset; a site the
module's `reloc1`/`reloc2` tables do **not** name is not a relocation site in
the shipped image and is left alone (`docs/reloc-surface.md` §2). A site the
tables do name gets a canonical, ROM-derived identity:

| site | canonical name | why |
|---|---|---|
| `HI16`+`LO16` pair | `__ovval_<link value>` | the value `synthesize()` derives at that site -- the ROM's stored words minus the object's own addend. Two placeholders the surface values identically produce the same linked words, so they are the same symbol as far as the link is concerned |
| `R_MIPS_26`, `SYMBOL` record | `__ovcall_o<overlay>_<offset>` | the stored immediate is always zero and carries no identity, so the record's own `overlayRomTable` entry -- the callee's overlay and offset -- names it |
| `R_MIPS_26`, `JUMP` record | `__ovjump_<module offset>` | an intra-module call; the stored immediate *is* the target's offset |

The target `.s` copy is rewritten to spell those names symbolically
(`jal __ovcall_o0_26934`, `lui $t0,%hi(__ovval_00000080)`,
`addiu $t0,$t0,%lo(__ovval_00000080)`), which makes the assembler emit real
relocations; the candidate's placeholder symbols are renamed to the same
names by `objcopy --redefine-sym` steps appended to the scratch's
`compile.sh`, alongside the ones `replicate_objcopy` already writes there.

Three properties are worth stating because they are what make this a
measurement rather than a fudge:

- **It is not a relaxed scorer.** The canonical name comes from the ROM's
  own record, not from the candidate's symbol table, so a candidate that
  calls the *wrong* placeholder at a site still scores a penalty.
- **A symbol whose sites disagree is left alone entirely.** If one site wants
  `__ovval_...` and another `__ovcall_...`, the symbol has no canonical
  identity; annotating half its sites would make the target disagree with the
  candidate at the other half, silently. Those are reported in
  `build/permuter/<fn>/annotation.txt`.
- **Any failure falls back to the previous behaviour.** If the annotated `.s`
  does not assemble, the unannotated target is restored and reassembled; the
  run continues with the old, pessimistic score. `--no-overlay-annotate`
  forces that path for before/after measurement.

Nothing ROM-derived is written anywhere tracked: the rewritten `.s` lives in
the gitignored permuter scratch, exactly like the label rename above.

### Measured (2026-08-28, lane `lane/ovperm`)

Base score before and after annotation, against
`tools/promotion_trial.py`'s in-range word count for the same function:

| function | ov | trial words | base before | base after | differing rows |
|---|---:|---:|---:|---:|---|
| `overlay18Load` (pre-`OSTime`) | 18 | 2 | 700 | **400** | 62 -> 2 |
| `overlay7DispatchSelection` | 7 | 2 | 75 | **10** | 14 -> 2 |
| `overlay97InitScale` | 97 | 1 | 10 | 10 | 1 -> 1 |
| `overlay84AdvanceCurrent` | 84 | 2 | 41 | **16** | 7 -> 2 |
| `overlay40FadeRecords` | 40 | 3 | 75 | **25** | 11 -> 3 |

These are historical lane measurements. `overlay7DispatchSelection`'s scalar
is scheduling evidence for stale pre-identity C, not current C or promotion
proof. For `overlay40FadeRecords`, configured full-TU and isolated objects for
the preceding body survive and confirm 98/101 words, frame `0x8`, and one
three-site register web. Current source applies the uncompiled natural
semantic-role repair identified by that web, so the historical scalar is
baseline evidence rather than a score for the new body.

`overlay18Load` is no longer a close-candidate queue item. Its O32
`osSetTime(OSTime)` repair produced unguarded canonical C for Overlay 18
`+0x000..+0x1F4`; retained configured and linked-overlay artifacts corroborate
all 125 words and 60 runtime records. A fresh current-HEAD whole-ROM reproof is
still required, so this historical scalar is neither current search evidence
nor standalone promotion proof.

`overlay97InitScale` has no site the module's table names inside it, so
nothing is annotated and nothing changes -- which is the honest answer, and
it already scored its one real word.

The scores that remain are ordinary permuter penalties over the rows that
actually differ (`overlay18Load`'s 400 is one `li`/`move` pair scored as a
replace; `overlay84AdvanceCurrent`'s 16 is a stack-offset difference). What
changed is that they are now *about the function's codegen*.

**Zero means zero.** The check that matters is the other direction: a
candidate whose C is already exact must score 0. `overlay62Initialize`
(o062, matched, 15 placeholder symbols, 30 relocation sites) was temporarily
re-wrapped as a `NON_MATCHING` candidate around its own exact C and
re-split. Unannotated it scored **150** over 30 differing rows; annotated it
scored **0** over 0. The wrapper was reverted afterwards.

### Promotion is still the linked ROM

A score of 0 on the annotated scratch says the candidate's instruction
schedule matches the target. For an overlay that is necessary and not
sufficient, and `--apply` does not treat it as sufficient. The promotion path
splices, rebuilds the object, regenerates the relocation surface
(`gmake overlay-syms`, since a promoted body may reference placeholders whose
values are synthesized from the objects), rebuilds, and accepts only on
`gmake verify` -- byte-identical ROM -- followed by
`tools/promotion_proof.py <symbol> --json`. That wrapper requires complete
post-promotion ownership, linked words/frame and exact relocation count,
type/offset and effective identity. A linked-byte-only oracle is insufficient.
Missing authoritative resident or overlay ownership fails closed; promotion
does not invent symbol or ownership rows to pass the gate.

An overlay symbol-generation pass can mutate an object by rebinding its symbol
table. Promotion therefore retains that generated object as
`promotions/<id>/after-symbol-generation.o`, removes exactly the affected TU
object from its build path, and rebuilds it through the ordinary configured
Make target. `check-overlay-syms` then checks the fresh object without mutation,
before linking, ROM verification, or strict function proof. A missing persistent
normalization recipe now fails even if the generator-mutated object was exact.
The fresh or partial compiler output is retained as `configured-rebuild.o`,
including on failure/cancellation; the generated object is never restored over
it to make proof pass. Missing, nonregular, or symlinked object ownership fails
closed. No other object's path is removed, and no build policy is adopted
automatically. After rollback, the normal extract/rebuild requirement still
applies to these now-stale ignored build artifacts.

Between ROM verification and post-promotion proof, `gmake prune-asm` removes
the fallback that the promoted C no longer names. Resident source edits alone
do not invalidate the split stamp; leaving that file behind makes preflight
select fallback mode even after the C has linked exactly. This is ordinary
extracted-file lifecycle management, not target-byte editing. If a later gate
fails and source is restored, run `gmake extract` and rebuild before reusing
the restored fallback's artifacts.

The splice also regenerates `config/overlays.us.json`: a spliced candidate
flips that TU's mechanically-derived `nonmatching` flag, and
`overlay_atlas.py --check` is a prerequisite of `build/.splat-stamp` and so of
everything, which means a stale atlas kills the promotion build before it
compiles anything. Promotion journals the target C, its plateau shard, atlas,
YAML, generated overlay symbols, donor digest and README before the first
write. Generator failures, timeouts and cancellation restore these owned
changes. Independent subsequent source edits are retained with a reverse
three-way merge; overlapping edits are left untouched and flagged for manual
recovery. This relies on exclusive lane ownership during generators: a second
writer modifying a generator's declared output while that generator runs cannot
be distinguished from the generator itself. Do not run competing writers.

The serialization lock spans splice, proofs, derived gates and optional
commit. `--commit` refuses preexisting changes on promotion paths; unrelated
staged and unstaged work is preserved. Its private index includes exactly the
promotion delta. A normal hooked commit runs on an owned detached HEAD sharing
the repository's objects, configuration and hooks. Its resulting tree, parent
and message must equal the proved values before compare-and-swap publication
to the exact lane branch captured at transaction start. A competing commit or
branch switch fails closed. The real index's standard writer lock spans copying,
comparison, branch verification, CAS publication and atomic index replacement,
preventing normal checkout in that interval. Recovery holds the same lock from
the selected-branch check through ref/index reconciliation and file rollback.
A changed branch or unavailable recovery lock preserves files and backups for
manual review, even if the new branch's committed source equals the candidate;
unrelated staged entries
are retained. A failed commit is retained at `refs/sweep-recovery/<id>`; recovery
undoes only this transaction's branch advance if it was already published.
Foreign commits touching promotion paths are preserved for manual review,
never reset. A hook that changes the private index's tree creates only recovery
evidence, never a promoted result. Before private staging, the runner compares
origin and detached Git configuration without displaying its values. Different
worktree-specific or conditional configuration fails closed rather than silently
dropping signing, identity, filter or hook policy; use a reviewed manual commit
workflow for such unsupported contexts.

Before-images and any merge-conflict evidence remain in ignored
`build/permuter/<function>/promotions/<id>/`; the search candidate stays in its
original run directory. Failed promotion earns no credit and remains retryable.
Build artifacts can be stale after rollback and should be rebuilt before use.
Hard kill, storage failure, overlapping edits or recovery-grace expiry can
require manual restoration from the retained evidence; automatic rollback is
not a guarantee against those conditions.

A score-0 overlay candidate can still fail `verify`. The module's data is
placed by the runtime, not by this link, so a promotion can move a word the
scratch never modelled: a datum landing at a different module offset, an
alias the surface now resolves elsewhere, a digest-guarded POSTPROCESS pass
the scratch could not replicate. When that happens rollback leaves the function
a **candidate**, and the reported reason identifies the failed command and
recovery evidence. `tools/promotion_trial.py --function <symbol>` can then
classify it (`text-differs` / `text-size-differs` / a named `build-error` cause)
in `build/promotion-trial.json`. "Scored 0
but did not verify" is a trial result to route, not a failure to hide.

### Running the overlay pool

```sh
# overlay functions only, closest first, one at a time, machine-safe
tools/permute_batch.py --overlays-only --order ranking --jobs 1 \
    --permuter-threads 4 --minutes 15 --flat-minutes 5 \
    --load-threshold 12 --limit 25 --apply --commit
```

`--overlays-only` is the complement of the older `--resident-only`, which
existed only because overlay scores were meaningless. It no longer is.

## The `-DNON_MATCHING` define, not source surgery

A queued function's `.c` file still has both branches
(`#ifdef NON_MATCHING <candidate> #else <pragma> #endif`); the project's own
Makefile already has an escape hatch for selecting the candidate branch,
`gmake NON_MATCHING=1` (`-DNON_MATCHING` in `DEFINES`). `permute_batch.py`'s
generated per-function `permuter_settings.toml` adds that same define to
`compiler_command` and hands `import.py` the **unmodified** `.c` file --
never a hand-edited copy. The real C preprocessor resolves every
`#ifdef NON_MATCHING` block in the translation unit consistently, which
matters for a `.c` file with more than one queued function in it (several
of the lane `cx-nm-*` conversions batch multiple functions per overlay into
one file).

The batch extracts the complete ordered IDO argument list from the actual
object recipe using `gmake -n -W <source> <object>`. Per-TU defines, include
ordering, ISA and optimization switches therefore come from the configured
build, not a source-directory classifier. Legacy path-based helpers are not
accepted as batch search evidence. An unsupported compiler wrapper is an
operational failure requiring explicit recipe support, not permission to
substitute plausible flags or claim that its phase overrides were searched.

The standalone flag lattice uses that same configured context and changes
only its declared axes. Failed combinations remain failures in its coverage
report; a zero-score supported combination does not make a partially supported
lattice complete. See [flag-sweep.md](flag-sweep.md).

## Promotion (`--apply`)

Every reference project and this project's own `docs/adr/0007` are explicit
that a permuter score of 0 is a *candidate*, not a match, until it is
compiled by the real toolchain, linked at its real address, and
byte-compared. Without `--apply`, that's all this script reports: base
score, best score, whether zero was reached. With `--apply`, a zero score
triggers:

1. **Splice**: the winning `output-0-*/source.c` is the whole pruned
   translation unit import.py built around the candidate, not just the
   function -- `permute_batch.py` extracts just the function definition
   (balanced-brace scan from the signature, the same technique
   `tools/permute.sh` already uses to recover a function body) and replaces
   the *entire* `#ifdef NON_MATCHING ... #else ... #endif` block for that
   function with it, dropping the guard, the stale candidate, and the
   `#pragma GLOBAL_ASM` fallback.
2. **Rebuild**: for overlays, generate the symbol surface, retain and remove
   exactly the affected object, rebuild its configured Make target, and run
   read-only `check-overlay-syms`. Then `gmake -j<build-jobs>` rebuilds the ROM.
3. **Verify**: `gmake verify` (byte-identical ROM rebuild against the
   pinned SHA1).
4. **Fallback lifecycle**: `gmake prune-asm` removes orphaned extracted
   fallbacks after the compiled ROM has been proved exact.
5. **Complete promoted-function proof**:
   `tools/promotion_proof.py <function> --json` checks owned geometry, linked
   words/frame and exact relocation count, offsets/types and effective identities.

Failure reports the function as `zero-found`, not `promoted`, and invokes the
owned-file recovery described above. Conflicting edits or changed branches
require manual recovery from retained before-images; they are not overwritten.
Re-extract and rebuild after rollback if fallback artifacts were pruned.

**What this does not do automatically**: remove a now-dead
`POSTPROCESS = ... objcopy --redefine-sym func_overlay_...=<friendly>`
Makefile rule. Once the real C function (already named the friendly name)
compiles directly, that rule's rename becomes a no-op at best and a
reference to a symbol the object no longer emits at worst -- but whether a
given object's `POSTPROCESS` line is *only* that rename, or bundles it with
something still load-bearing (a second `--redefine-sym` for an unrelated
symbol, a `trim_elf_section.py` step, per §"Makefile context" in the pilot
commit), is a per-object judgment call this script does not make for you.
Check the object's `POSTPROCESS` line by hand after a promotion and drop it
if -- and only if -- the whole line was that one now-redundant rename.

**Winning context is not automatically transferred or compared.** The public
`promote(item, winning_source, ...)` interface receives a pruned/preprocessed
winner but no authenticated original import context. Comparing that winner
directly with raw canonical C would confuse ordinary preprocessing/pruning
with a changed prototype or type; an extension can also replace scratch
`base.c`, so that mutable file is not necessarily the original baseline.
Review declaration/context changes independently against target ABI evidence
before body-only transfer. A future detector needs to bind an immutable initial
prepared baseline alongside the winner and compare parsed non-function context,
without automatically adopting shared headers or declarations. Full-TU and ROM
gates remain mandatory even when the scratch score is zero.

## Cost and match rate (measured)

**Pilot, this lane's own queue item**, `overlay1GetEntry` (o001, 0x30 bytes
/ 12 instructions), 12-minute cap, `-j 6`, `--apply`:

| function | base | best | zero-diff | promoted | wall-clock |
|---|---|---|---|---|---|
| `overlay1GetEntry` | 330 | 190 (-42%) | no | no | 721s (ran to the cap) |

**Cross-lane sample**, three functions from `lane/cx-nm-2`'s overlay 2
conversion (not yet merged into this lane's own queue -- checked out onto a
throwaway branch reset to that lane's `HEAD`, tested, then discarded; no
commit of theirs was kept), 8-minute cap, `-j 4`, report-only (`--apply`
omitted deliberately -- these functions are not this lane's to promote):

| function | base | best | zero-diff | promoted | wall-clock |
|---|---|---|---|---|---|
| `func_overlay_002_F0001A94_185888C` | 570 | 530 (-7%) | no | no | 482s (ran to the cap) |
| `overlay2ClassifyBoundary` | 2540 | 1320 (-48%) | no | no | 482s (ran to the cap) |
| `overlay2ChooseBoundary` | 3295 | 1995 (-39%) | no | no | 482s (ran to the cap) |

This also proved the queue-discovery and naming-quirk handling against a
tree with several other lanes' conversions merged in at once:
`--list` against the `cx-nm-2` checkout correctly surfaced 19 queued
functions across 7 overlay/`main` sources without duplicates, including
`func_overlay_002_F0001A94_185888C` -- a function whose `#ifdef
NON_MATCHING` branch is itself still named with splat's auto name (no
friendly name assigned yet), the other shape the naming-quirk rename needs
to handle correctly (a no-op rename, rather than skipping the rename step).

**Combined: 4 functions run, 0 zero-score (0%), 0 promoted (0%), 0
errored.** Every run reduced the score (7-48%) but none reached zero within
its cap. Four data points is not a reliable match-rate estimate for a
~310-function queue, but it is enough to report honestly: at these caps
(8-12 min, 4-6 permuter threads), the permuter is closing part of the gap
on functions of this size (roughly a hundred bytes to half a kilobyte) but
not resolving them outright, on this small a sample. `overlay1GetEntry`
specifically was originally reached only through
`normalize_elf_instructions.py` (three register-field edits, disqualified
by `docs/adr/0002`), so a genuine compiler-only match existing for it at
all was not guaranteed going in. Re-run this section's numbers once a
meaningful slice of the real queue has gone through
(`tools/permute_batch.py --limit 20 --jobs 4 --minutes 15 --apply` against
this lane's own merged queue is a reasonable next batch) and replace this
table rather than layering a second one on top of it, per `CLAUDE.md`'s
"derived numbers are recomputed, never remembered.""

## Recommended default cap

Start at **`--minutes 15 --jobs <ncpu/6 to ncpu/8>`** (leaving
`--permuter-threads` at its default split, so total permuter threads stay
around `ncpu - 2`): most of these functions are the small end of overlay
code (tens to a few hundred bytes; `overlay1GetEntry` itself is 0x30 bytes),
where a permuter search either finds a small perturbation quickly or is
unlikely to converge within any reasonable cap because the mismatch is
structural (register allocation shape, scheduling) rather than a `perm_*`
knob can reach -- in which case 15 minutes and 60 rarely differ in outcome,
per the caution `docs/adr/0007` records from the Snowboard Kids 2 project
about not mistaking permuter search time for the fix. Raise the cap for a
specific function only after confirming from its `permuter.log` that the
score is still trending down near the cap, not plateaued.
