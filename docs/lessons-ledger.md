# Lessons ledger

One entry per lesson that changed how the campaign is run. Newest first.
Each entry: what was observed, what it cost, what changed because of it
(tool, gate, rule, or prompt). Numbers are the values at the time; recompute
before reusing them. See `docs/epoch14-plan.md` for the plan these feed.

## 2026-08-28

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
