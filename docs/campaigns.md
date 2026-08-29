# Campaigns toward 100%

This project has three distinct completion states. They must not be collapsed
into one percentage:

- **matched C**: source compiles to the resident ROM bytes;
- **verified original assembly**: a named source-assembly object matches, so C
  is neither expected nor counted;
- **unresolved**: compiler-generated assembly, unidentified resident code, or
  an overlay that has not yet been split.

`tools/progress.py` reports all three against a whole-program text denominator.
The resident denominator comes from the linked ELF; the overlay denominator is
derived from the 107 shipped overlay headers. `verified_asm.us.txt` is an
explicit evidence ledger because a generated `.s` filename alone cannot prove
that code was originally hand-written.

The bounded post-research checks which separate the raw m2c, known-source,
same-ROM similarity, model-routing, and expert-escalation queues are recorded
in [the 2026-08-27 campaign check ledger](campaign-check-ledger-2026-08-27.md).

**Note on scope and standard.** Byte and percentage figures below predate
`docs/adr/0001-matching-standard.md` and `docs/adr/0002-no-post-compile-instruction-editing.md`.
Some Epoch 10-12 gains were later found to include objects reached through
post-compile instruction editing; see those ADRs and
`docs/acceleration-survey.md` §1 for the corrected accounting. This file
records what each campaign set out to do and what it reported achieving at
the time, not a restated post-ADR total.

**This file is a summary only.** It keeps each epoch's goal, exit criteria,
and headline results in prose, without the per-function byte ledgers,
per-target offset tables, and SHA-256 receipts the live campaign tracked
while it ran. Those detailed per-epoch ledgers are retained outside git (the
workbench's own records and `docs/modules.md`'s per-epoch sections cover the
mechanically-checkable subset); this summary is what's safe and useful to
keep in the tracked history.

## Epoch 1 — libultra source harvest (complete 2026-08-13)

Goal: exhaust the straightforward reference-backed libultra cohort, make
initialized data/rodata/BSS ownership possible, keep unsupported objects
honest, and finish with a byte-identical US ROM.

Results: 46 libultra translation units moved from extracted assembly to
matching C. Matched functions rose from 58 to 123 and resident matched text
from 14,384 to 25,548 bytes. `enddlabel` support unblocked C-owned rodata,
and measured SDK data/BSS slices now link from their owning objects. The
progress report began distinguishing matched C, verified original assembly,
and the full resident-plus-overlay denominator. The ROM rebuild stayed
byte-identical.

`xprintf` and `xldtob` stayed assembly: their published `-O3` sources need an
interprocedural optimization pass the toolchain doesn't have. `sl` stayed
assembly because the available source doesn't reproduce Mickey's object.
This records the Epoch 1 state: the 2026-08-27 JFG wring-out later established
that direct IDO `-O3 -mips2` does reproduce both complete objects when the
asm-processor front end is bypassed, and promoted them to exact C.

## Epoch 2 — overlay atlas and shared-code harvest (complete 2026-08-13)

Goal: turn the opaque overlay-module region into a stable work surface
without claiming generated assembly as decompiled code.

Results: a single generated manifest now represents all 107 shipped overlay
headers (including the empty overlay 32), recording total text, initialized,
BSS, and relocation counts. The 106 non-empty overlays became independent
buildable code segments. Cross-overlay relocations form a dependency graph
that feeds a reproducible campaign priority. The progress tool began reading
the atlas total instead of double-counting synthetic overlay symbols. DKR
v77 became a pinned, reproducible reference build alongside JFG, with an
exhaustive per-overlay donor ledger (including negative results) recorded
for both. A handful of exact and semantic donor matches were found,
including one whole-module match. The linked image and US ROM stayed
byte-identical.

## Epoch 3 — overlay frontier tranche A (complete 2026-08-13)

Continued the donor-ledger-driven overlay harvest opened in Epoch 2,
converting a further tranche of donor-backed and structurally simple overlay
functions to matching C while keeping the ROM byte-identical.

## Epoch 4 — overlay frontier tranche B (complete 2026-08-14)

A second overlay tranche following the same donor-first method as Epoch 3,
proving the semantic-spine functions and cross-overlay edges that Epoch 5
built on.

## Epoch 5 — semantic spine and leaf closure

Goal: convert the API surface proved in Epoch 4 while advancing overlay 61's
semantic cluster, targeting at least 8,192 additional matched overlay text
bytes (whole-program resolved code from 48,752 to at least 56,944 bytes,
6.00%), through a small mandatory semantic-spine set plus a nine-module
closure cohort of non-padding bytes with no cross-overlay imports.

Epoch 5 was left open on its independent module-closure exit (2 of 6 target
modules closed) even after later epochs exceeded its byte target; Epochs 6-9
continued drawing against the same cohort.

## Epoch 6 — exact-leaf recovery and blocker closure (complete)

Goal: at least 1,024 additional overlay text bytes, prioritizing the
narrowest Epoch 5 compiler blockers before harvesting further small-function
boundaries.

Results: 1,040 bytes landed, raising overlay C from 12,476 to 13,516 bytes
and whole resolved code to 56,168 / 949,944 (5.91%). Both closed Epoch 5
blockers in overlay 97 were resolved; the remaining bytes came from exact
accessors, setters, no-op ABI surfaces, and small initializers across
fourteen overlays. No exact DKR donor was found for any Epoch 6 function.
Across Epochs 5-6 the byte target stood at 7,416 / 8,192, with four cohort
modules still requiring closure.

## Epoch 7 — leaf and wrapper retirement (complete)

Goal: short, legally bounded, strongly compiler-feasible functions, ahead of
returning to the large closure blockers.

Results: 508 exact non-padding bytes across nine overlays: state
initializers, byte copies, scalar predicates, resource-release wrappers,
mode-call wrappers, and a byte-string length loop. Overlay C rose to 14,024
bytes; whole resolved code to 56,676 bytes. One nested-release candidate was
rejected on a register-coloring mismatch despite matching semantics. No
donor or naming evidence was found beyond generic patterns.

## Epoch 8 — accessor and resource-wrapper tranche (complete)

Goal: continue the leaf/wrapper harvest, prioritizing the overlay 84
accessor/initializer surface.

Results: 436 exact non-padding bytes. The overlay 84 accessor/initializer
surface (state bootstrap, active/current queries) landed using
branch-likely-matching early-return source shapes; two resource-release
wrappers landed elsewhere. Overlay C reached 14,460 bytes; whole resolved
code 57,112 bytes. Two bounded experiments (overlay 65 reset, overlay 96
bit-query) stayed non-exact and were dropped. DKR checks were donor-negative.

## Epoch 9 — overlay 68 lifecycle cluster (complete)

Goal: close out overlay 68's lifecycle API surface (allocators, release
wrappers, timer/entry helpers).

Results: 524 exact non-padding bytes, all in overlay 68: two entry
allocators, three release wrappers, a nested-flag clear, an entry-finish
helper, and a timer-start helper, all linking word-for-word. Overlay C
reached 14,984 bytes; whole resolved code 57,636 / 949,944 (6.07%). The
pinned donor ledger stayed donor-negative for overlay 68; names stayed
grounded in Mickey's own control flow.

Across Epochs 5-9 the original 8,192-byte ownership exit closed at
8,884/8,192 (exceeded), but the independent six-module closure exit
remained 2/6, carried into Epoch 10.

## Epoch 10 — double-digit breakthrough campaign (complete)

Goal: starting from 57,636 / 949,944 whole-program resolved bytes (6.07%),
reach 10.00% whole-program resolved (at least 94,995 resolved bytes, or
37,359 new exact non-padding overlay bytes) while also closing four more
Epoch 5 cohort modules (overlays 74, 85, 97, 77 primary; 23, 24, 82
fallback), keeping the ROM byte-identical, and refreshing the donor ledger
before each new semantic cluster.

Results: the campaign added 37,360 exact non-padding overlay bytes, one
byte past its hard minimum. Overlay C ownership reached 52,344 / 469,264
(11.15%); whole-program resolved text reached 94,996 / 949,944 (10.00%).
Work was drawn from a large number of overlays via a closure lane (attacking
the four named indivisible blockers), a semantic-cluster lane (continuing
overlays 68 and 84), and a volume lane (overlays 1 and 101 plus further
relocation-bounded modules). Overlays 74, 77, 85, and 97 closed fully except
for proven alignment padding, completing the cumulative Epoch 5 cohort
ledger at 6/6. The donor ledger was refreshed across all 107 overlays; the
final three semantic clusters were donor-negative. The ROM stayed
byte-identical.

## Epoch 11 — fifteen-percent offensive (complete 2026-08-22)

Goal: starting from Epoch 10's verified 94,996-byte exit, reach 142,492 /
949,944 whole-program resolved bytes, the first integer-byte total at
15.00%, requiring at least 47,496 new exact bytes and bringing overlay C to
at least 99,840 / 469,264 (21.28%). Six hard exits governed the campaign:
the byte target; closing at least eight more overlay modules; preserving
the exact ROM SHA1; refreshed DKR v77/v80 and JFG donor scans before each
target; and finishing with the atlas, donor ledger, module ledger, README
scoreboard, and clean-room checks all mutually current.

The campaign planned a portfolio rather than a single target: a primary
slate exposing 57,480 bytes against the 47,496-byte gate, a diversified
compact reserve of 41,552 bytes, and a further ranked large-function ladder,
together several times the hard target's size.

Results: the campaign closed at 144,292 / 950,332 resolved bytes (15.18%)
and 101,252 / 469,264 overlay C bytes (21.58%), against a milestone ladder
(A through E) tracking bytes landed, modules closed, and the final
integrity/documentation pass.

## Epoch 12 — twenty-percent full-thrusters campaign (complete)

Goal: starting from Epoch 11's verified 144,292 / 950,332 resolved bytes
(15.18%), reach the first integer-byte total at or above 20.00%,
190,067 resolved bytes, requiring at least 45,775 new exact non-padding
overlay bytes and bringing overlay C to at least 147,027 / 469,264 (31.33%).
Six hard exits mirrored Epoch 11's: the byte target, closing at least eight
more overlays, preserving the exact ROM SHA1, keeping every generated
ledger and scoreboard current, requiring guarded real-offset linked equality
(not semantic equivalence or a proxy link) for every promoted function, and
a final clean-room and full-ROM data-comparison pass.

The measured unresolved non-padding surface at campaign start was 364,940
bytes across 67 overlays. Five primary volume modules (overlays 101, 1, 58,
57, 8) exposed 127,960 bytes; an independent fifteen-module closure cohort
exposed a further 21,464 bytes, together over three times the hard byte
gate. Work was organized into a closure-card wave run concurrently with
targeted volume tranches per module.

Results: the campaign's final recorded checkpoint reached 317,900 / 950,332
resolved text (33.45%) and 274,860 / 469,264 overlay C (58.57%), well past
its 20.00%/31.33% hard exits. As `docs/acceleration-survey.md` §1 later
found, a substantial share of Epoch 10-12's overlay gains reached exactness
through post-compile instruction editing rather than untouched compiler
output; `docs/adr/0001-matching-standard.md` and
`docs/adr/0002-no-post-compile-instruction-editing.md` correct the standard
going forward, and the honest current total is what `gmake scoreboard`
reports today, not the figures recorded here at campaign close.

## Epoch 13 (2026-08-24): unchained

The owner reviewed `docs/acceleration-survey.md` in full and, the same day,
wrote ten ADRs (`docs/adr/0001` through `0010`) settling every open policy
question the survey raised: what counts as matched, that no instruction word
is ever edited after compilation to reach one, build parallelism, work
prioritisation, one-TU-per-overlay source layout, the permitted matching
tools, provenance from the five named reference decomps, model routing and
agent operation, and commit discipline. `docs/adr/README.md` and `AGENTS.md`
now govern the campaign instead of ad hoc practice discovered mid-session.

The immediate mechanical consequence was the ADR 0001/0002 rescoring: every
object `tools/postprocess_audit.py` finds an instruction-altering
`POSTPROCESS` step on moved from matched to `NON_MATCHING`, and
`config/postprocess-audit.us.json` now records zero such objects remaining
(`{"metadata": 621}`) — the conversion is complete, not partial. Work
continues on `campaign/unchain` in per-worker lanes (`tools/new_lane.sh`/`merge_lane.sh`), the
compile-job ceiling is gone, decomp-permuter and objdiff-cli are installed
tools rather than survey findings, and the `n_audio` library adoption from
PD/BK/JFG bodies (ADR 0008) is most of the way done. `docs/modules.md`
section 5 split out to `docs/overlays.md` the same day, for the same reason
the survey flagged the file's size. `docs/acceleration-survey.md`'s
"Status, 2026-08-24 evening" section carries the current numbers and the
remaining queue: the NON_MATCHING backlog now belongs to the permuter and to
hand restructuring, and overlay consolidation (ADR 0006) has a named first
cohort but has not yet landed.

## Epoch 14 (2026-08-28): search, don't stare

Per-function agent grinding flattened (08-26 to 08-28: ~50 matches, then a
night of sixty plateau notes and none), while every productive event in the
same window was a tooling fix that let the permuter crack functions hand
analysis had called walled. The epoch therefore routes work by tool:
`tools/permute_sweep.sh` runs a fidelity-proven permuter over the entire
NON_MATCHING queue (never sound-searched before: the 2026-08-25 farm predates
the flag/objcopy/stack-diff fixes), Codex lanes produce frame-exact shapes
for the bare pool as permuter input, and expensive models touch only the
sweep's measured permuter-stuck list. `docs/epoch14-plan.md` carries the
pools, phases, standing rules and exit criteria; lessons feed the workbench
backlog with a spike per phase boundary.
