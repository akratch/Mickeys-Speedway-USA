# Lessons ledger

One entry per lesson that changed how the campaign is run. Newest first.
Each entry: what was observed, what it cost, what changed because of it
(tool, gate, rule, or prompt). Numbers are the values at the time; recompute
before reusing them. See `docs/epoch14-plan.md` for the plan these feed.

## 2026-08-28

- **The overlay scaffolding was hand-maintained because nobody had checked
  whether it had to be.** `overlay_undefined_syms.us.txt` -- 2,928 lines, one
  hand-derived line per adopted overlay symbol -- is entirely derivable: a
  value line is the stored relocation addend read from the baserom at the site
  the module's own table names, an alias line is the generated splat identity
  for a module offset from `text_ownership`. Generated, it is 2,265 lines, the
  ROM is byte-identical, and 8 duplicate names and 168 shadowed assignments
  disappear. Changed: `gmake overlay-syms` writes it, `gmake
  check-overlay-syms` gates it, `tools/promotion_trial.py` regenerates it
  between compiling a candidate and linking it. Overlay candidates carrying a
  measurable number went 110/279 -> 194/279.
- **A guard that aborts the build hides the measurement it was about to
  make.** Every POSTPROCESS normalization asserts the matching object's exact
  layout, so a promoted candidate of the wrong size died at compile time and
  53 of 279 candidates reported only "a guard fired". Changed:
  `tools/postprocess_guard.py` + `PROMOTION_TRIAL=1` make the guards report and
  skip, so the same candidates now report `text-size-differs (+N bytes)` -- 18
  of the 44 are within +/-16 bytes. Never set in the normal build.
- **"build-error" is not a class, it is a refusal to look.** Splitting the
  overlay trial's failures by cause turned 169 undifferentiated errors into 85,
  of which 49 are `schedule-divergence-at-site` (a codegen problem), 15 are
  `resident-symbol-missing` (not an overlay problem at all) and 14 are
  `rom-size`. Two of the spike's named failure classes -- alias coupling and
  non-`.text` sites -- stopped occurring once the generator owned the whole
  block. Changed: `promotion_trial.py` names every cause and `--resume` keys
  results by function instead of appending a second verdict.
- **A filter that silently drops everything looks like success.** The
  synthesizer ignores relocation sites the module's table does not corroborate;
  when *every* site for a symbol was dropped it emitted no value and no
  complaint, and the caller saw only "undefined reference". Changed:
  `synthesize()` reports a symbol whose sites were all filtered. The matching
  tree reports zero, so the complaint only fires on real divergence.
- **The Makefile is not the list of what the link consumes.** Filtering build
  artifacts by whether the Makefile mentions the object's name dropped the 21
  overlay objects that reach the link through a pattern rule, which made a
  chunk of the tracked surface look unreproducible. `mickey.us.ld` names every
  input object explicitly and is the authoritative list.

- **A candidate inside a matched overlay TU un-credits the whole TU.** The
  atlas credits ownership per source file; two far-off "middle function"
  candidates cost 5,024 resolved bytes at merge. Changed: candidates go in a
  per-function file or stay as the pragma; `tools/check_match_regression.py`
  now refuses such a merge; lane prompts carry the rule.
- **A TU that fails to compile with `-DNON_MATCHING` silently drops every
  candidate in it out of the permuter sweep.** fx.c, all of overlay 1 and
  overlay 8 were locked out for days by an undefined type and conflicting
  candidate declarations; the ELF normalizations also ran in the NON_MATCHING
  tree and failed by construction. Changed: `gmake check-nonmatching-builds`
  gate, `RUN_POSTPROCESS` skipped in `build_non_matching`, `--resume` re-runs
  errored rows.
- **The closeness ranking goes stale within hours.** A "2 words" row had a
  permuter base score of 700; scratch fidelity was verified sound, so the
  ranking was the fault. Changed: regenerate `tools/nm_ranking.py` before
  every sweep relaunch; the workbench stamps rankings with the tree hash.
- **Two-thirds of the NON_MATCHING queue is structurally wrong** (246 of 360
  candidates over 32 words; 34 within 8). The permuter cannot fix shape.
  Changed: sweep spends its time on the near third; shape/reshape lanes own
  the rest; the "permuter-stuck flat" list decides whether deeper compiler
  instrumentation is funded.
- **Flat-at-six-minutes was flat-at-twenty on every measured run.** Changed:
  `--flat-minutes 6` early stop (flat = no output strictly better than base),
  ~3x more functions per hour; score-trend extension covers descending runs.
- **Parent-only kills leave permuter worker pools orphaned.** Three batches
  of idle workers after one morning. Changed: process-group kill at the cap
  (host runner and workbench `run_owned`).
- **Overlay promotions need the atlas regenerated or the bytes are never
  credited.** Changed: the runner runs `overlay-atlas-write` + digest and
  stages them on overlay promotions.
- **Static compiler-flag tables are wrong for TUs that look default.**
  `lights.c` carries `-Wab,-r4300_mul`; `gmake -n` (with continuation lines
  joined) is the only authoritative source. Changed: `build_recipe_for()`;
  the earlier 0/38 permuter farm result was discarded as unsound.
- **Lane target lists must come from `#ifdef NON_MATCHING` ownership, not
  from names.** Ranking rows use friendly names, `.s` files use splat
  auto-names; two lanes were assigned already-owned functions. Changed:
  bare = pragma outside any NON_MATCHING block, computed by
  `permute_batch.discover_queue`.
- **Codex conversion by function class.** Tiny resident functions with
  donors match directly (5 gzip/boot in 50 min); mid-size resident bodies
  yield shape candidates only, which the permuter then closes when they are
  within ~10 words (`func_80003760`: 8 words → exact in 74 s); reshaping
  far-off display-list overlay candidates mostly did not move. Changed: the
  pipeline is shape lane → sweep; lanes get a per-function time budget, not
  the three-plateau stop, and a load threshold of 14.
- **The permuter's process load starves anything gating at load 10.** Sweep
  alone sits at 8–11 on 14 cores. Changed: lanes gate at 14; sweep runs two
  4-thread searches.
- **Shared checkouts are not safe for two agents.** Two workbench agents
  collided on one branch switch. Changed: every agent works in its own
  `git worktree`; merges are fast-forwarded from a throwaway worktree.
- **Public mirroring needs its own scrub list.** Agent passes left `CREW-`
  task ids in source comments and integration-branch names in scripts.
  Changed: scan for `/Users/`, `lane/`, Codex/luna/crew/Claude/Opus/Fable,
  `CREW-`, `campaign/unchain`, scratchpad, `.codex` before every public push.
- **"Proven pass-owned" verdicts fell to a sound permuter twice more today**
  (`func_8004D40C`, `overlay31CreateConfig`). Standing rule: no wall verdict
  before a sound sweep of that function; the workbench verdict text now
  routes allocation ties to the permuter.
- **Candidates in one TU drift into conflicting declarations.** Two lanes
  declared the same function with different parameter types (an earlier
  caller candidate's extern vs a later definition), so the merged TU failed
  the NON_MATCHING build; the matched callers' top-level prototype is the
  ABI truth. Changed: candidates adapt to the existing prototype with casts
  inside the body; `merge_lane.sh`/`finish_merge.sh` run
  `check-nonmatching-builds` before committing a merge; lane prompts carry
  the rule.
- **Two lanes editing the same TU always conflict at merge, even on disjoint
  functions.** Neighbouring candidate blocks collide line-for-line and each
  merge cost an agent pass to resolve. Changed: assign lanes by translation
  unit, not by function list; when a TU must be shared, the second lane
  waits for the first to integrate.
- **The permuter is the wrong oracle for overlay functions.** The splat
  target spells every overlay call relocation with a placeholder symbol and
  the real link resolves calls through the module's relocation table, so a
  candidate with identical instruction words still scores hundreds (`overlay18Load`:
  2 real words, permuter base 700). Changed: `tools/promotion_trial.py`
  promotes each candidate in place and classes it by the LINKED ROM
  (exact / text-exact / text-differs / build-error); the sweep runs
  `--resident-only`; overlay routing uses the trial's numbers.
- **Overlay promotions are gated by relocation scaffolding, not C.** Most
  overlay candidates fail to link (undefined `*Reloc` placeholders, digest-
  guarded ELF passes) because every matched overlay function carries a
  bespoke POSTPROCESS rule hand-derived from the target relocation table.
  A generic relocation-surface synthesizer (from the atlas census) is the
  lever for the whole 299 KB pool; a feasibility spike is running.
- **The overlay relocation surface is a pure function of the baserom and
  the atlas.** Modules ship unrelocated, so each placeholder symbol's value
  is the stored addend at its sites (`R_MIPS_26`: `0xF0000000 | imm26<<2`;
  `HI16/LO16`: `(hi<<16)+sext(lo)`; `R_MIPS_32`: the word) minus the
  object's own addend. `tools/reloc_surface.py --audit` reproduced
  1,773/1,773 hand-derived values and 979/982 link-defined ones; 14 of 19
  blocked candidates linked with zero collateral. Changed: the surface is
  generated, not hand-written per function (implementation in progress);
  the bespoke POSTPROCESS ritual stops being the gate on 299 KB.
- **Six Codex lanes plus two permuter passes crashed the workstation** (load
  15–17 sustained; the earlier freeze threshold was ~20 but sustained ~15
  with build bursts was enough). Changed: hard cap of four Codex lanes at a
  time, one permuter pass, total load target ≤ 12; launch lanes one at a
  time, 60 s apart, after `splat extract` of the previous one has finished.
- **An object-level "instruction words identical" verdict is not an overlay
  oracle either.** A lane reported 15 overlay candidates at 0 words by
  diffing objects with relocation-bearing words masked; the linked ROM still
  showed the original 1–4 words, and the 1-word case was plain codegen
  (`addiu a2,a3,2` vs `addiu a2,a1,0x3e`, no relocation). Changed:
  `tools/promotion_trial.py` now records, per in-range word, the target word,
  the built word and the relocation at that site; overlay lanes measure with
  the trial only and commit "trial-exact" when it reports zero.
- **Why the permuter scored overlay functions in the hundreds.** The splat
  target assembles with no relocations at all (`jal <self>`, `lui/addiu …,0`),
  the candidate carries symbols, and decomp-permuter ignores symbol-name
  differences only when both sides carry one. `overlay18Load`: score 700,
  two real words. Changed: `tools/reloc_surface.py`'s `permuter_annotation()`
  rewrites the scratch target at exactly the sites the module's own
  `reloc1`/`reloc2` tables name, and renames the candidate's placeholders to
  the same ROM-derived identities, so both sides render identically there.
  `overlay18Load` 700 -> 400 (62 differing rows -> 2, its two real words);
  `overlay7DispatchSelection` 75 -> 10; `overlay40FadeRecords` 75 -> 25;
  `overlay84AdvanceCurrent` 41 -> 16. `tools/permute_batch.py --overlays-only`
  replaces `--resident-only` as the routing flag; promotion still goes
  through the linked build.
- **A "the score improved" claim is not the check that matters; "an exact
  candidate scores 0" is.** The first annotation scheme named a `SYMBOL`
  HI16/LO16 site by the record's `overlayRomTable` entry, which is *not* the
  value stored at the site, so one symbol read through a `SYMBOL` record and
  written through a `LOCAL` one got two names and neither side matched. Every
  before/after number still improved; only re-wrapping an already-matched
  overlay function (`overlay62Initialize`, exact C) as a `NON_MATCHING`
  candidate exposed it -- 150, not 0. Changed: a HI16/LO16 pair is named by
  the link *value* `synthesize()` derives at the site, a symbol whose sites
  disagree is left unannotated rather than half-annotated, and the
  matched-function round trip (score must be 0) is the acceptance test for
  any future change to the annotator.
- **`objcopy` refuses two `--redefine-sym` arguments sharing a target name,
  and a shell script without a trailing newline swallows the next command.**
  Both failed silently: the renames simply did not happen and the score stayed
  high with no error anywhere. Changed: colliding renames go in successive
  `objcopy` invocations, and the scratch's `compile.sh` is newline-terminated
  before anything is appended (`replicate_objcopy` only terminates its last
  line when it wrote one, so a TU with an unreplicable POSTPROCESS ends
  mid-line).
