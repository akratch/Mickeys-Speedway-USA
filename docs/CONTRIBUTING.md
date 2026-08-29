# Contributing

## Set up once

```sh
gmake setup
```

That bootstraps the venv, installs dependencies, copies the toolchain, verifies
your baserom's SHA1, splits the ROM, and points git at `.githooks/`, which is
what activates the clean-room gates. `core.hooksPath` is per-clone
configuration, not a tracked file, so **every fresh clone needs this once**.
`gmake hooks` does that step alone.

Full build instructions are in [`README.md`](../README.md).

## Decisions

Policy questions that have already been settled (what counts as matched,
what post-compile object editing is permitted, build parallelism, work
prioritisation, source layout, matching tools, provenance, model routing,
and commit discipline) are recorded as ADRs in [`docs/adr/`](adr/README.md).
Read the index before assuming a policy question is open; if you think an
ADR's decision should change, write a new ADR rather than acting against it.

## Lane workflow

Each worker (human or agent) works in its own isolated worktree, a "lane,"
created with `tools/new_lane.sh <name>`. This creates
`../mickey-lane-<name>` on branch `lane/<name>`, sharing the untracked
toolchain, baserom, venv and vendored tool checkouts with the main
checkout by symlink, with its own `build/` and `asm/` so lanes never
contend for the same objects. Commit on your own lane branch, in small
(function-sized) commits, as work lands; hooks stay on, never
`--no-verify`. See [`docs/adr/0004-build-parallelism.md`](adr/0004-build-parallelism.md)
and [`docs/adr/0010-commit-discipline.md`](adr/0010-commit-discipline.md).

Lane isolation covers working state, not committed knowledge. A coordinator or
read-only tool may inspect committed `refs/heads/lane/*` objects through Git's
object database to avoid duplicate assignments and find newly matched siblings;
it never reads a sibling worktree, index, process, or uncommitted file. Run
`tools/lane_status.py --pending-only` for the fleet view or
`tools/lane_status.py --symbol <name>` before assignment. Its rows are
commit-message claims, not match evidence; integration repeats every normal
proof. See [ADR 0011](adr/0011-cross-lane-knowledge-and-task-budgets.md).

Non-interactive workers also receive an explicit task budget. The launcher
passes a soft deadline to the agent and tools, reserves five minutes for a
handoff, and then interrupts the exact process it launched. Expiry preserves a
best candidate/plateau; it never resets a lane or discards work.

### Three-session interactive crew

ADR 0013 defines a small persistent topology for several interactive Codex
sessions: one leader on `campaign/unchain`, plus `worker-1` and `worker-2` in
permanent isolated lanes. It adds coordination without weakening lane
ownership.

```sh
tools/crew.py init --worker worker-1=crew-worker-1 \
  --worker worker-2=crew-worker-2
tools/crew.py doctor
tools/crew.py status
tools/crew.py inbox worker-1
```

The runtime directory is `$(git rev-parse --git-common-dir)/codex-crew/`, so
all worktrees see it and none of it can be staged. `queue.md` has one writer
(the leader), each role owns one status file, and messages are immutable files
written atomically. Use `tools/crew.py send` for new messages and
`tools/crew.py ack` to acknowledge and archive one. The committed tree remains
the durable record; mailbox text is only scheduling and handoff metadata.

Workers reuse their permanent branch. Once the leader reports an integration
commit, the worker fast-forwards its lane to that commit, confirms a clean
worktree, reports release, and returns to `READY` unless an ADR 0014 pipeline
task remains active. A write-capable subagent must instead receive a separate,
disjoint child worktree; its parent worker owns the commit handoff and removes
only that clean child worktree after integration. No actor reads, edits, builds,
resets, or cleans another actor's worktree.

ADR 0014 pipelines review time without moving a handed-off branch. As soon as a
complete `HANDOFF` is accepted, the leader may send that worker one `PIPELINE`
follow-on packet naming a disjoint task and exact temporary lane. The worker
keeps its permanent lane frozen and works in its own child worktree using the
same compile permit. After the old handoff is integrated, it fast-forwards and
releases the permanent lane, then transfers the child's committed result onto
that current lane and repeats the ordinary proofs before the next handoff. One
frozen handoff plus one pipeline task is the only two-packet state; unexpected
transfer conflicts are reported, not guessed through. This keeps matching work
active during review without allowing `merge_lane.sh` to chase a moving lane
tip.

The paste-ready role prompts live in `.codex/crew/`. They also carry the
occupied-workstation limits: never launch tests, generated programs, browsers,
emulators, simulators, or GUI workflows; any permitted compilation is low
priority and limited to two jobs, with only one compile-heavy process per
worker slot.

## Known sources and reusable compiler knowledge

When evidence identifies likely SDK or standard-library code, begin with the
permitted official source and exhaust plausible versions, compiler flags,
conditional paths, and TU/object boundaries before writing custom C. Mickey's
bytes remain authority and `PROVENANCE` remains mandatory. Reusable IDO behavior
proved by a Mickey-exact result or controlled local experiment belongs in
[`ido-learnings.md`](ido-learnings.md); function-specific attempts stay in the
resident, overlay, and triage ledgers. See
[ADR 0012](adr/0012-known-sources-and-reusable-knowledge.md).

## The clean-room rule

Nothing ROM-derived is ever tracked in git: no disassembly, no instruction
text, no hexdumps or byte arrays of ROM bytes, no extracted assets, no ROM
images, no decompilation-workbench ledgers. [`CLEANROOM.md`](CLEANROOM.md) is
the policy: what may be consulted, what may be adopted, and how adopted
material is disclosed. Read it before taking a name or a function body from
another project.

It has gone wrong once: two workbench `ledger.jsonl` files reached the remote
with the ROM's own disassembly inside their diff-site records, and the fix was
a history rewrite. The gates below exist so that cannot recur.

## The gates

| Where | What it scans |
|---|---|
| `.githooks/pre-commit` | the index, exactly what the commit would record |
| `.githooks/pre-push` | every commit tree in the push, not just the tip |
| `.github/workflows/cleanroom.yml` | the same, on every push and pull request |

All three run `tools/cleanroom_check.sh`, which looks for ROM/asset paths,
tracked files under `.decomp-workbench/` other than a campaign manifest,
non-text blobs, oversized files, MIPS instruction text, bare machine-word
runs, machine words written adjacently or spread across the 32-bit space in
any encoding, hexdump-shaped lines, base64 blobs by longest run and by volume,
and an aggregate word budget across each tree. Thresholds are measured against
this repository's whole history on one side and the purged ledgers plus an
evasion fixture set on the other; the numbers and their margins are in
`tools/cleanroom_detectors.py`.

The first two layers are client-side and the third is after the fact: hooks
are per-clone configuration, `--no-verify` steps over them, and by the time CI
speaks the objects are already published. They are depth against mistakes, not
a barrier against a determined bypass.

**Server-side, on `master`:** the GitHub ruleset `protect-master`
(id `20111399`, active) blocks force-push (`non_fast_forward`) and branch
deletion. That is the one layer that cannot be stepped over with `--no-verify`,
but it guards the *branch*, not the *content*: GitHub rejected a push ruleset
restricting by file path, extension or size with "only org-owned repos can have
push rules", which this personal fork is not. A required status check on
`master` remains the only way to block a bad push before it lands rather than
after, and would mean routing changes through pull requests; that workflow
change has not been made. One consequence: purging content from `master`'s
history again means disabling `protect-master` first and re-enabling it after.

Run it yourself before committing:

```sh
gmake cleanroom                                   # the worktree
gmake cleanroom CLEANROOM_ARGS=--staged           # the index
gmake cleanroom CLEANROOM_ARGS="--range A..B"     # a commit range
```

### What they catch, and what they don't

Measured against 400-word real-ROM fixtures, the content rules catch asm
listings, every hexdump format, C arrays (including `u`/`UL` suffixes and
underscore separators), machine words in prose or hex ranges or 16-bit halves
or octal or decimal, escaped byte strings, base64/base64url/base32/ascii85
(wrapped, or split across files), JSON ledgers, and leaks spread thinly across
a tree.

They do **not** catch:

| Hole | Measured limit |
|---|---|
| a padded sub-threshold trickle | the per-file word limit is 192, so one file carrying **191 machine words, about 764 bytes of ROM**, passes if it is padded enough to keep the aggregate's 12-words/KiB rate floor from counting it; eight such files carry about **6 KB** with the aggregate budget reading zero |
| digest-shaped hex strings | the first 64 per file are exempt; **87 digests, about 2.7 KiB per file** on the measured region, up to 141 / ~4.4 KiB on others, since what trips is the spread of the words they decode to |
| fewer than 64 hex halves pairs in a file | 63 pairs, about 252 bytes, is not caught *by that route* (the fixture is still caught by the layout rules) |
| deliberate steganography | undecidable in general |

Detecting arbitrarily-encoded data is undecidable, and these rules are
calibrated for mistakes rather than adversaries. [`CLEANROOM.md`](CLEANROOM.md)
lists the limits with their measurements, and names what is load-bearing: the
path whitelist and manifest schema check, the ROM path and binary rules, the
tool-level ledger redaction, this policy, and `protect-master`.

**Do not use `--no-verify`**, and do not lower a threshold to get a file
through. If a file is a real false positive, restructure it or add an
allowlist entry in `tools/cleanroom_detectors.py` with a reason. If something
ROM-derived is already committed, it must be rewritten out of history; a
commit that deletes the file still ships its bytes to anyone who fetches.

## Evidence discipline

Every claim in `docs/modules.md` states how it was established, using the four
tiers in its §1: **A** byte-identity, **B** call graph, **C** string
correspondence, **D** structural inference. Declare the tier inline, per
symbol, in both `docs/modules.md` and `symbol_addrs.us.txt`. Tier A has an
adoption threshold (§1.2); an adoption below it is argued individually in that
section's table.

Derived numbers (matched function and byte counts, percentages, segment
sizes) are recomputed from the lists they summarise, never copied forward.
`gmake check-docs` re-derives the mechanically checkable ones and fails on
drift.

### Overlay donor-first workflow

Before naming or decompiling any overlay function:

1. identify it as `(overlay, section, offset)` from
   `config/overlays.us.json`; the shared synthetic VMA is not a unique runtime
   address;
2. run `gmake overlay-donors-scan-check`, which rechecks every overlay against
   the pinned DKR v77, secondary DKR v80, and JFG object surfaces;
3. consult DKR first for game-code structure and terminology because it is the
   complete closest-lineage decomp, then use JFG where its overlay/runtime
   layout is closer;
4. cite the exact object/name when bytes match, or label the comparison
   semantic when only systems, strings, or call shape correspond. A donor's
   generated placeholder is never an adopted name.

`gmake overlay-donors` is the farm-free integrity check for the committed
107-row-per-donor ledger. The scan-check is the stronger maintainer check and
requires the out-of-tree reference builds described in `references.md`.

## Checks

**Before committing, at minimum:** `gmake verify && gmake cleanroom && gmake check-docs`.
`cleanroom` also runs automatically at commit/push if `gmake hooks` has been
run; nothing else is wired into a hook.

The Progress block that `gmake scoreboard`/`gmake check-scoreboard` maintain
now carries DKR's five lines (decompiled, handwritten ASM, GLOBAL_ASM
remaining, NON_MATCHING, NON_EQUIVALENT), byte-weighted, with a range
counting as decompiled only if its object carries no instruction-altering
post-compile step. See
[`docs/adr/0001-matching-standard.md`](adr/0001-matching-standard.md) and
[`docs/adr/0003-scoreboard.md`](adr/0003-scoreboard.md). `README.md`'s
generated block is the current numbers; recompute rather than quoting them
here.

### `gmake NON_MATCHING=1`: the compile-only escape hatch

Every function ADR 0001/0002 demoted from "matched" to `NON_MATCHING` keeps
its C body under `#ifdef NON_MATCHING`, with the original
`#pragma GLOBAL_ASM` preserved under `#else` (ADR 0002's Consequences). That
guard is normally off, so the ordinary build still links the `GLOBAL_ASM`
fallback and stays byte-identical. `gmake NON_MATCHING=1` flips it: every
converted TU compiles its real C body instead, into a **separate build tree**
(`build_non_matching/`, never `build/`) so those objects can never be
mistaken for, or sit next to, the ones `gmake verify` checks. It is a
compile-only smoke test — proof the C is not obviously wrong, not a matching
claim. `gmake verify` refuses to run under `NON_MATCHING=1` (`the error is
literal: "verify does not run under NON_MATCHING=1"`), exactly DKR's own
guard for the same escape hatch. Unset it and rebuild before running
`verify`.

### Auditing post-compile steps: `tools/postprocess_audit.py`

`tools/postprocess_audit.py` is what makes ADR 0002 enforceable rather than
aspirational. It does not parse the Makefile by hand; it asks `gmake -p -q`
to expand every rule and reads the target-specific `POSTPROCESS = ...`
assignments straight out of that expansion, then classifies each one:

- **`altered`** — touches instruction words (`normalize_elf_instructions.py`,
  `normalize_o63_*.py`, `resize_elf_function.py`,
  `extend_elf_function_to_text.py`, `patch_elf_words.py`). Forbidden by ADR
  0002; any object in this class cannot count as decompiled on the
  scoreboard.
- **`metadata`** — everything ADR 0002 permits: ELF header/ABI bits, symbol
  renames, section trimming, relocation filter/rebind. Safe to credit.

Run it three ways:

```sh
tools/postprocess_audit.py            # table to stdout
tools/postprocess_audit.py --write    # refresh config/postprocess-audit.us.json
tools/postprocess_audit.py --check    # fail if that JSON is stale
```

`config/postprocess-audit.us.json` is the committed result: one row per
object carrying a `POSTPROCESS` override, its class, tool list, and
(where known) its `(overlay, offset, size)` ownership joined from
`config/overlays.us.json`. As of this pass its `summary.by_class` reads
`{"metadata": 619}` — zero `altered` objects — which is the mechanical proof
that the ADR 0002 conversion reached every object in the tree, not just the
functions this lane's prose describes.

### Report-only m2c sweep: `tools/m2c_sweep.py`

`tools/m2c_sweep.py` inventories every `GLOBAL_ASM` pragma but attempts only
bare pragmas: fallbacks already paired with `NON_MATCHING` or
`NON_EQUIVALENT` C are reported separately rather than treated as functions
with no candidate. For each eligible function it gives m2c a preprocessed copy
of the owning TU, substitutes the draft into a scratch copy of that complete
TU, asks Make for the real object's effective compiler flags, and runs one
low-priority IDO compile at a time. Extracted jump tables are discovered and
passed to m2c as additional inputs when needed.

```sh
nice -n 15 .venv/bin/python tools/m2c_sweep.py --inventory-only --fresh
nice -n 15 .venv/bin/python tools/m2c_sweep.py --fresh
```

All candidates, objects, logs, and reports stay under the ignored
`build/m2c_sweep/` directory. The tool never edits canonical source or
promotes a result. Its strongest verdicts are `scratch_text_exact` and
`scratch_relocation_exact`; even a scratch relocation-exact candidate still
needs the canonical full-TU, configured post-process, linked owned-range, and
ROM proofs required by ADR 0001 before promotion. `--resume` continues an
interrupted report without recompiling recorded identities, and repeated
`--symbol` arguments provide a bounded targeted run.

### Lane helpers: `new_lane.sh`, `merge_lane.sh`, `codex_lane.sh`

- **`tools/new_lane.sh <name> [--no-extract] [base-branch]`** creates
  `../mickey-lane-<name>` on branch `lane/<name>` from `base-branch`
  (default `campaign/unchain`), symlinking the untracked toolchain, baserom,
  venv and vendored tool checkouts in rather than copying them, and (unless
  `--no-extract`) runs the splat extract so the lane can build immediately.
  Each lane gets its own `build/`/`asm/`. It resolves the primary checkout
  through Git's common directory even when invoked from another lane, and
  fails instead of installing a dirty symlink when a tracked submodule cannot
  be initialized from the shared local module store.
- **`tools/merge_lane.sh <name>`** integrates one lane back into the current
  branch: it rebuilds the lane from clean and requires `verify`/`check-docs`
  to pass there first, runs the clean-room range scan over the lane's
  commits, merges `lane/<name>`, and resolves the two files that always
  conflict by *regenerating* them instead of taking either side — the README
  scoreboard block and the overlay atlas — then re-runs
  `verify`/`check-docs`/`overlay-atlas`/`check-scoreboard` on the merged
  result. It exits non-zero and leaves the merge in progress if anything else
  conflicts or a gate fails, rather than guessing a resolution. Set
  `MICKEY_BUILD_JOBS` and `MICKEY_BUILD_NICE` when local workstation policy
  requires a lower compiler concurrency or priority; the three-session crew
  uses two jobs and niceness 15.
- **`tools/codex_lane.sh <name> <prompt-file> [--minutes N] [--no-extract]`**
  creates a lane with `new_lane.sh` and launches a detached, non-interactive
  `codex exec` worker inside it; the worker commits on `lane/<name>` like any
  other worker. The default soft budget is 180 minutes (overridable by
  `CODEX_MINUTES` or `--minutes`), followed by a five-minute hard-stop grace
  period. `MICKEY_TASK_BUDGET_SECONDS`, `MICKEY_TASK_DEADLINE_UNIX`, and
  `MICKEY_TASK_HARD_DEADLINE_UNIX` are available to the worker and its tools.
  Progress, effective prompt, final message and exit status land in
  `<lane>/.codex-run.log`, `<lane>/.codex-effective-prompt.md`,
  `<lane>/.codex-last.md`, and `<lane>/.codex-status` (all gitignored).
- **`tools/lane_status.py [--base REF] [--pending-only] [--symbol NAME]`**
  reports unintegrated commits and `Match <symbol>` claims from committed lane
  refs only. `pending` means the base still has that symbol's `GLOBAL_ASM` while
  the claiming lane does not; it is a coordination hint, never a replacement
  for integration validation.

### Integration housekeeping: `fix_stale_externs.py`, `refresh_atlas_digest.py`

- **`tools/fix_stale_externs.py`** rewrites `func_<VRAM>` references in
  `src/main`, `src/libultra`, and `include/game` whose address has since
  been given an adopted name in `symbol_addrs.us.txt`. Lanes name functions
  independently; without this, a merge can leave one lane's `extern ...
  func_8002E148(...)` stale against another lane's adopted name for the same
  address, and the link fails after integration. Overlays are excluded —
  their extern names are lane-owned and link through the overlay relocation
  model, not the resident symbol table.
- **`tools/refresh_atlas_digest.py`** refreshes just the `sha256` field
  `config/overlay-donors.us.json` stores over `config/overlays.us.json`,
  for a layout-only atlas regeneration (ownership rows, nonmatching flags)
  that doesn't actually change any donor result. The full `--write` donor
  rescan needs the out-of-tree reference farm and fails if a reference
  checkout has moved past its pin; this script is the documented workaround
  when only the digest went stale.

### `docs/modules.md` / `docs/overlays.md` split

On 2026-08-24, section 5 of `docs/modules.md` (the overlay system) moved
into its own file, `docs/overlays.md`, keeping the same `5.x` numbering so
existing cross-references still resolve. `docs/modules.md` keeps a stub
pointer at section 5. A lane branched before the split, editing the old
whole file, conflicts with that move on merge; **`tools/resolve_modules_split.py`**
performs the merge the split intended — it three-way merges the non-section-5
half and the section-5 half independently (via `git merge-file`) against
their respective destination files, and exits 1 if either half still has
conflict markers. Run it inside an in-progress merge, in place of resolving
`docs/modules.md`/`docs/overlays.md` conflicts by hand.

| Command | Checks | Needs a build? | Enforced by |
|---|---|---|---|
| `gmake verify` | ROM rebuilds byte-identically (SHA1 `507341c0a40ca3e9a7cee969b396ee53facfb548`) | yes | manual |
| `gmake cleanroom` | no ROM-derived content in the worktree/index/range | no | pre-commit, pre-push, CI |
| `gmake check-docs` | derived numbers in the docs (`docs/modules.md` etc.) match the tree | no† | manual |
| `gmake check-scoreboard` | README's generated Progress block matches what the tree produces right now | yes | manual only; CI runs the `--check-partial` subset, which does not need a build (see [`scoreboard.yml`](../.github/workflows/scoreboard.yml) and the note below) |
| `gmake audit-decoders` | the clean-room content detectors aren't inventing words that aren't there (run after touching `tools/cleanroom_detectors.py`, not instead of `cleanroom`) | no | manual |
| `gmake check-fixtures` | the other direction: real ROM in every encoding at every wrap width is *still caught*, which `audit-decoders` is structurally blind to. Fixtures are synthesized from `baseroms/mickey.us.z64` at run time and never written to disk, so it can never run in CI. Run it with `audit-decoders`, not instead of it | no (needs a baserom) | manual |
| `gmake check-reference-builds` | a local reference-decomp farm still hashes to the digests `tools/reference-builds.lock` pins, i.e. is the farm the 190 tier-A names were mined from. Needs the farm, which is out of tree by design, so it can never run in CI. `gmake reference-builds` rebuilds one from the pins; see [`references.md`](references.md) | no (needs the farm and its baseroms) | manual |
| `gmake overlay-tables` | decodes the four overlay ROM blocks and re-asserts the layout `docs/modules.md` §5.3 states: the reloc count word, 370 of 375 call sites holding a real `jal`, the 107-fold module gap arithmetic. Needs a baserom, so it can never run in CI | no (needs a baserom) | manual |
| `gmake overlay-atlas` | regenerates the canonical 107-module manifest and 106 generated yaml code segments in memory and fails on drift | no (needs a baserom) | manual |
| `gmake overlay-donors` | validates complete DKR v77/v80 and JFG results for all 107 overlays against pinned metadata; does not need the farm | no | `check-docs` |
| `gmake overlay-donors-scan-check` | reruns those object comparisons and fails if the committed donor ledger differs | no (needs the reference farm and Mickey baserom) | manual before overlay adoption |
| `gmake prune-asm` | deletes the `asm/` files splat orphaned: `asm/<tu>.s` for a subsegment that is now `c`, and any `asm/nonmatchings/` file no `#pragma GLOBAL_ASM` names. Run by every split, so it is rarely invoked by hand; without it `gmake progress` under-reports | no | every `extract`/split |
| `gmake progress` | the same matched-function/byte/symbol counts as the scoreboard, without touching README.md | yes | manual |
| `gmake scoreboard` | regenerates README's Progress block from the tree (run it, then commit, whenever matching progress changes) | yes | manual |
| `gmake clean` | removes `build/` only | no | manual |
| `gmake distclean` | `clean`, plus the *extracted* state: `asm/`, `assets/`, the linker script and the auto-generated `undefined_*.us.txt`. Recovering needs `gmake extract`, which needs a baserom | no | manual |

† `check-docs`'s jump-table count needs `asm/` and is skipped, not failed,
before `gmake extract` has run.

**Why `check-scoreboard` can't be a full CI job.** It needs a linked ELF, and
producing one needs `gmake extract` to split `asm/` out of a baserom, which
[`CLEANROOM.md`](CLEANROOM.md) forbids committing. `--check-partial` (wired
into [`scoreboard.yml`](../.github/workflows/scoreboard.yml)) is the strongest
subset that runs without one: it recomputes the block's two non-ELF-derived
figures (the adopted-symbol count from `symbol_addrs.us.txt` and the
matched-TU list from `src/`) using the real generator, and separately checks
the committed block's own arithmetic (each ratio's percentage against its own
numerator/denominator, the per-area rows summing to the total row). It cannot
catch drift in the ELF-derived figures: functions/bytes matched, the per-area
breakdown. Only `gmake check-scoreboard`, run locally by whoever matched the
function, catches that.

```sh
gmake verify            # byte-identical: 507341c0a40ca3e9a7cee969b396ee53facfb548
gmake cleanroom         # exit 0
gmake check-docs        # derived numbers still true
gmake check-scoreboard  # README's Progress block still matches the tree
git status              # clean
```
