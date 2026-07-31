# Phase 2: Clean-Room Hardening + Cross-Title Match Mining

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** (1) Make a repeat of the ledger incident structurally impossible — violations blocked at commit time, push time, and CI, over full history, with the leak class fixed at the tool level. (2) Mine the other published Rare decomps (JFG, Perfect Dark, Banjo-Kazooie, Conker, and optionally GoldenEye/DK64) for functions matching Mickey's ROM, converting the libultra drift corridor and JFG-shared engine code into named/matched functions.

**Architecture:** Hardening = layered gates: repo-committed git hooks (activated via `core.hooksPath` in `gmake setup`), a strengthened multi-detector `cleanroom_check.sh` (mnemonic density + hex-dump + base64-blob + tracked-size heuristics), CI extended to scan every commit tree in the pushed range (not just tip), agent/contributor guidance in `CLAUDE.md`, and ledger redaction upstream in the decomp-workbench checkout. Mining = an **external** reference farm (`~/Desktop/dev/decomp-refs/`, never inside this repo) of built decomp trees whose objects feed `tools/find_known_objects.py`; adoptions follow the Phase 1 evidence-tier + provenance conventions verbatim.

**Tech Stack:** git hooks (sh), GitHub Actions, Python (detector + finder), the per-title decomp toolchains (IDO recomp variants; reuse DKR/mickey binutils where possible), Myrient No-Intro ROM set at `~/Documents/Minerva_Myrient/No-Intro/Nintendo - Nintendo 64 (BigEndian)/`.

## Global Constraints

- `gmake verify` byte-identical (SHA1 `507341c0a40ca3e9a7cee969b396ee53facfb548`) + `bash tools/cleanroom_check.sh` exit 0 + porcelain clean at every commit. Branch `phase2-hardening-mining` from master `ee9e2b8`; push to origin (akratch fork) only.
- Clean room per `docs/CLEANROOM.md` (as widened in Phase 1): the five named published retail-derived decomps are permitted sources for names and adapted bodies WITH point-of-use PROVENANCE disclosure; leaked anything is forbidden; reference repos, their baseroms, and their build outputs live OUTSIDE this repo (`~/Desktop/dev/decomp-refs/`) and are never committed.
- Naming/adoption discipline is Phase 1's, verbatim: evidence tiers declared inline per symbol (`docs/modules.md` §1); summary counts recomputed from lists; derived numbers added to `tools/check_derived_numbers.py` coverage where mechanically checkable; masked-match thresholds per §1.2 with sub-threshold adoptions individually argued.
- Reference ROMs must SHA1-match each decomp's documented baserom before building; a reference decomp that does not build cleanly gets a partial-objects fallback (compile what compiles) — documented, never forced.
- Commit trailer: `Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`.
- Workbench friction/insights → `docs/workbench-improvement-log.md` (standing goal).

---

### Task 1: Clean-room gates — commit, push, CI, and tool level

**Files:**
- Create: `.githooks/pre-commit`, `.githooks/pre-push`, `CLAUDE.md`, `docs/CONTRIBUTING.md` (short)
- Modify: `tools/cleanroom_check.sh` (multi-detector + `--staged` and `--range` modes), `.github/workflows/cleanroom.yml` (range scan), `Makefile` (`setup` wires `core.hooksPath`; `cleanroom` target passes through modes), `docs/workbench-improvement-log.md`
- Outside repo (separate deliverable, own commit in that repo): ledger redaction in `~/Desktop/dev/n64-decomp-workbench` (and/or the DKR checkout's `tools/decomp-workbench`) so campaign ledgers store masked/hashed words instead of raw ROM instruction text

**Interfaces:**
- Produces: `tools/cleanroom_check.sh --staged` (pre-commit: scans the staged index), `--range A..B` (pre-push/CI: scans every commit tree in range), default = worktree scan (unchanged behavior). Exit nonzero with a named-file, named-detector message on any hit.

- [ ] **Step 1: Detector upgrade.** Extend `cleanroom_check.sh` beyond mnemonic density: (a) hex-dump detector (lines dominated by hex byte pairs/words at high density — ROM bytes leak as hexdumps too); (b) base64-blob detector (long base64 runs in text files); (c) tracked-binary-and-size guard (any tracked file > 256KB or non-text flags for review with a named allowlist); (d) `.decomp-workbench` path guard (only `manifest.json` may ever be tracked under it). Each detector prints file + detector name + first offending line number. Tune on the current tree (must pass) and on the historical ledger content (must fail — test via `git show` of the pre-purge backup SHA if still reachable, else synthesize a fixture from asm/ content in a temp dir, never committed).
- [ ] **Step 2: Modes.** Add `--staged` (scan `git diff --cached` blobs) and `--range A..B` (iterate `git rev-list`, scan each commit's tree via `git ls-tree`/`git cat-file`, dedup by blob SHA for speed). Default mode unchanged.
- [ ] **Step 3: Hooks.** `.githooks/pre-commit` runs `tools/cleanroom_check.sh --staged`; `.githooks/pre-push` reads the stdin ref lines and runs `--range remote-sha..local-sha` per pushed ref (handle new-branch case: range from merge-base with origin/master, fallback full branch). Both fail closed with instructions. `gmake setup` runs `git config core.hooksPath .githooks`. Hooks must be fast (<5s typical) — the blob-SHA dedup matters.
- [ ] **Step 4: CI range scan.** `cleanroom.yml`: `fetch-depth: 0`, run `--range` over the push/PR range (use `github.event.before`/PR base; fallback to full history scan on new branches, which the blob dedup makes tolerable).
- [ ] **Step 5: `CLAUDE.md`** (agent guidance, terse): clean-room rules for any AI/human session in this repo — never track ROM-derived content (asm dumps, instruction text, hexdumps, ledgers); run `gmake cleanroom` before committing; hooks are active via setup and must never be bypassed with `--no-verify` (state it); provenance/evidence-tier conventions pointer to `docs/modules.md` §1 and `docs/CLEANROOM.md`; derived counts are recomputed, never written from memory. `docs/CONTRIBUTING.md`: the same for humans + build quickstart pointer.
- [ ] **Step 6: Tool-level fix (the class kill).** In the standalone workbench checkout `~/Desktop/dev/n64-decomp-workbench` (inspect first; if the campaign/ledger writer lives only in the DKR checkout's `tools/decomp-workbench`, patch there): change ledger writing so per-site "target" fields store masked word values or short hashes sufficient for comparison bookkeeping, never reconstructable instruction sequences; add a regression test. Commit in THAT repo with trailer (do not push unless the repo has an obvious origin the user owns — akratch remotes only; report what you did). Then record in mickey's `docs/workbench-improvement-log.md` that the improvement landed, closing the incident entry.
- [ ] **Step 7: Prove the gates.** Demonstrate each gate firing: staged-fixture → pre-commit blocks; range containing a synthetic bad commit on a scratch branch → pre-push blocks and `--range` catches it; delete scratch branch after. Record transcripts in the report. `gmake verify` + full checks green; commit.

### Task 2: Reference farm — build the donor decomps (external)

**Files:**
- Create (in repo): `tools/setup_reference_builds.sh` (documented, idempotent driver: clones/updates each reference repo into `~/Desktop/dev/decomp-refs/<name>`, stages its baserom from the Myrient path after SHA1 verification, runs its build, and writes an object inventory manifest `<name>.objects.txt`), `docs/references.md` (per-title: repo URL, commit pinned, baserom SHA1, build result, object counts, license/provenance notes)
- Outside repo: `~/Desktop/dev/decomp-refs/{jfg,perfect_dark,banjo-kazooie,conker}` (+ optional `007`, `dk64` if time permits)

**Interfaces:**
- Consumes: Myrient ROM directory; the JFG clone already in scratchpad (re-clone fresh into decomp-refs for pinning).
- Produces: per-title object inventories consumed by Task 3's finder runs; `docs/references.md` as the provenance record.

- [ ] **Step 1:** JFG (`Ryan-Myers/Jet-Force-Gemini`): stage `Jet Force Gemini (USA)` baserom (zip in DKR checkout root; verify against the repo's documented SHA1), attempt full build; on failure, partial-objects fallback (their repo structure may permit per-TU compilation). Inventory objects.
- [ ] **Step 2:** Perfect Dark (`n64decomp/perfect_dark`): stage its baserom from Myrient; build (it's mature; follow its README; it may need its own IDO variant — its repo bundles/downloads recomp binaries). Inventory.
- [ ] **Step 3:** Banjo-Kazooie (`n64decomp/banjo-kazooie`): same. Conker (`mkst/conker`): same (also note its rzip tooling location for later asset work). Optional if smooth: GoldenEye, DK64 (gitlab).
- [ ] **Step 4:** `docs/references.md` written with every pin/SHA1/result; commit repo-side files only. NOTHING from decomp-refs enters the repo — run `cleanroom_check` and confirm.

### Task 3: Mining pass — drift corridor + cross-title sweep

**Files:**
- Modify: `symbol_addrs.us.txt`, `mickey.us.yaml` (subsegment conversions where TUs match), `docs/modules.md` (map/tier updates), `src/libultra/**` (new matched TUs where byte-identity + flags reproduce), `docs/references.md` (per-title match yields)
- Possibly create: `src/` TUs for newly matched code

**Interfaces:**
- Consumes: Task 2 inventories; Phase 1's drift list (`task-B-report.md` §6 is gone with the workspace — the drift facts live in `symbol_addrs.us.txt` comments and `docs/modules.md` §4); `tools/find_known_objects.py`.

- [ ] **Step 1: Corridor drift conversion.** Run the finder against each reference title's libultra objects over the full static segment. For every hit meeting §1.2 thresholds: adopt name with tier + provenance (which title's build matched, byte-identical vs masked). Convert byte-identical C TUs to matched C where the flags reproduce them (per-file flag evidence, Phase 1 style). Expect: much of the 22% drift corridor falls; the scattered below-corridor matches consolidate.
- [ ] **Step 2: Game-code sweep.** Finder against each title's game objects (the DKR game-object run found 90 — repeat for JFG especially, where engine overlap is largest; PD/BK/Conker likely yield shared library-ish code: gzip/rzip, math, printf family). Adopt per thresholds; every JFG-derived body adaptation follows the runlink.c PROVENANCE pattern.
- [ ] **Step 3: Bookkeeping.** `gmake progress` + `gmake check-docs` green; modules.md corridor/map sections updated (counts recomputed from lists); drift list updated to what remains unexplained; workbench log updated with finder friction/ideas (e.g. the `romocc` column if it helps this pass — implementing small finder improvements is in scope when they pay for themselves immediately).
- [ ] **Step 4: Commit in coherent batches; final `distclean` rebuild verify; push branch.**

### Task 4: Wrap — metrics, README, merge prep

- [ ] **Step 1:** README status refresh with new metric numbers + one line on the reference-mining capability; `docs/references.md` linked.
- [ ] **Step 2:** Full-history `cleanroom_check --range` over the whole branch (proves the new gate on real history); push. Final whole-branch review follows via the controller.

## Self-review notes
- Task 1 makes the incident class impossible at four layers before Task 3 generates the largest volume of new cross-title-derived content — ordering is deliberate.
- Reference material stays outside the repo entirely; only scripts, documentation, and evidence-tiered adoptions enter git — the same boundary DKR references already follow.
- "Optional" titles (GE/DK64) are explicitly droppable; the plan succeeds on JFG+PD+BK+Conker or even a subset, with yields documented per title.
