# Phase 1: First Matched Functions, Ontology Seed, Workbench Loop

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Real matched C in the tree — libultra TUs plus a set of hard game functions byte-matched and fully validated — with the project's symbol/struct ontology started and the decomp-workbench integrated as on DKR.

**Architecture:** Extend the Phase 0 Makefile with IDO 5.3 C compilation + asm-processor GLOBAL_ASM workflow (DKR's pattern). Convert splat subsegments from `asm` to `c` one file at a time; each conversion must keep `gmake verify` green (byte-identical ROM). Matching workflow per function: m2c draft → IDO build → asm-differ zero-diff → for stubborn cases, decomp-workbench campaigns/trace analysis. Ontology accrues in `symbol_addrs.us.txt`, `include/` headers, and `docs/modules.md`.

**Tech Stack:** IDO 5.3 (`tools/ido/cc`), asm-processor, asm-differ, m2c (all vendored under `tools/` as submodule-free copies or venv installs — implementer picks the DKR-consistent route), decomp-workbench (`pip install -e` from the DKR checkout or the standalone `~/Desktop/dev/n64-decomp-workbench`), splat 0.35.2.

## Global Constraints

- Clean room per `docs/CLEANROOM.md`: symbol names/code only from retail-ROM analysis and public retail-derived decomps (DKR, JFG, PD, BK, Conker). DP public decomp = reloc-format facts only. No leaked-source material, ever.
- **`gmake verify` must pass (SHA1 `507341c0a40ca3e9a7cee969b396ee53facfb548`) at every commit.** A function is "matched" only when the full-ROM hash is identical with its C compiled in — no NON_MATCHING commits in this phase.
- Matched C compiles with IDO 5.3 `-O2 -mips1 -32` unless evidence forces per-file flags (document any deviation in the file header comment and the report).
- USA ROM facts (established): static segment `0x1000`–`0x86640` → vram `0x80000400`; BSS `0x80085A40` size `0x52D10`; entry jump `0x800211A0`; libultra byte-identical-to-DKR region ROM `0x6fc50`–`0x75e20`; overlays `0x16B0000`–`0x18F1FE0` untouched this phase.
- Recon reference (read-only): scratchpad `usa-recon.md`; JFG public decomp clone in scratchpad `jfg-decomp/` (its `runLink.c` documents the linker mechanism from retail JFG — usable).
- Commit trailer: `Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`.
- Workbench improvement ideas: append to `docs/workbench-improvement-log.md` (created in Task A) — one dated bullet per idea, concrete enough to act on later ("compare.py: X was awkward because Y; suggest Z").

---

### Task A: C-compilation infrastructure proven end-to-end on one file

**Files:**
- Modify: `Makefile` (IDO C rule + asm-processor pipeline; SRC_DIRS handling), `mickey.us.yaml` (one subsegment `asm`→`c`), `symbol_addrs.us.txt` (names for the chosen TU's functions)
- Create: `src/<first-file>.c` (implementer-chosen simplest static-segment file — prefer a small self-contained one, e.g. a libc-style TU near the libultra region), `tools/setup_tools.sh` additions if asm-processor/m2c need vendoring, `docs/workbench-improvement-log.md` (seed with header)
- Test: `gmake verify` byte-identical; asm-differ configured (`diff_settings.py` updated for the new build layout) and demonstrably working on one function

**Interfaces:**
- Consumes: Phase 0 Makefile (as/objcopy/ld flow), `tools/ido/cc` (`--version` works), `include/macro.inc`
- Produces: the `CC_RULE` pattern later tasks reuse: `.venv` + `tools/asm-processor` invocation wrapping `tools/ido/cc -O2 -mips1 -32 -G0` (exact flags recorded in Makefile); GLOBAL_ASM macro convention; `./diff.sh <symbol>` working asm-differ entrypoint

- [ ] **Step 1:** Vendor/install asm-processor + asm-differ + m2c runnable from this repo (DKR-consistent; document the route chosen). Do not commit binaries; scripts/requirements entries only.
- [ ] **Step 2:** Pick the first TU (justify choice in report: small, few relocs, clear boundaries). Convert its yaml subsegment to `c`, re-split, create `src/` file with `#pragma GLOBAL_ASM` stubs for every function (DKR convention), wire Makefile so the TU compiles through asm-processor+IDO and links in place of the raw `.s`.
- [ ] **Step 3:** `gmake verify` → byte-identical with the all-GLOBAL_ASM TU. Commit ("C build infrastructure: first TU scaffolded").
- [ ] **Step 4:** Decompile ONE function of that TU to real C (m2c draft, fix to match), verified: asm-differ zero diff AND `gmake verify` identical. Commit ("first matched function: <name>").
- [ ] **Step 5:** Update `diff_settings.py`/`diff.sh` so `./diff.sh <symbol>` works against the new build; prove with the matched function. Commit.

### Task B: libultra sweep — the byte-identical corridor

**Files:**
- Modify: `mickey.us.yaml` (libultra-region subsegments → `c`/`hasm`), `symbol_addrs.us.txt` (libultra names), `Makefile` (libultra dirs)
- Create: `libultra/src/...` TUs ported from DKR's libultra tree (same license/provenance — DKR repo's libultra is SDK-derived source already public in every N64 decomp; keep DKR's file layout), `include/PR/...` headers as needed

**Interfaces:**
- Consumes: Task A's CC_RULE + GLOBAL_ASM convention; recon's identified region ROM `0x6fc50`–`0x75e20` (16 confirmed functions: bcopy, bzero, bcmp, cache ops, TLB/SR routines, sqrtf, etc.)
- Produces: named + matched libultra TUs; the boundary map of libultra vs game code recorded in `symbol_addrs.us.txt` and yaml comments

- [ ] **Step 1:** Map every function in ROM `0x6fc50`–`0x75e20` to its DKR/libultra source name (byte-compare against DKR build objects; names from DKR's public tree — clean). Extend the map outward until byte-identity stops. Record all names in `symbol_addrs.us.txt`; re-split.
- [ ] **Step 2:** Port matching TUs from DKR's `libultra/` (respecting its per-file opt levels — some are `hasm`), converting yaml subsegments file-by-file; after each file `gmake verify` must stay identical. Commit per coherent batch, not per file.
- [ ] **Step 3:** Report: how many functions/TUs matched, where libultra actually starts/ends in Mickey, surprises (version drift vs DKR's libultra — flag any function that is NOT byte-identical rather than forcing it).

### Task C: Hard-function wins with the workbench

**Files:**
- Modify: `symbol_addrs.us.txt`, `mickey.us.yaml` (affected subsegments → `c`), `docs/workbench-improvement-log.md`
- Create: `src/` TUs containing the target functions (GLOBAL_ASM for neighbors, real C for targets)

**Interfaces:**
- Consumes: Task A infra; workbench installed (`pip install -e` from DKR checkout `tools/decomp-workbench` or `~/Desktop/dev/n64-decomp-workbench` — record which)
- Produces: ≥5 nontrivial matched game functions, each validated by (a) asm-differ zero diff, (b) `gmake verify` byte-identity, (c) a workbench `compare` run recorded in the report

- [ ] **Step 1:** Target selection (report the shortlist with rationale): the runtime overlay linker function cluster (the code around the `MIPS_HI16`/`Unimplemented linkage operation` strings — high-value: unlocks Phase 3, and JFG's public `runLink.c` gives the reference implementation shape); the entry/boot init at `0x800211A0`'s callee chain; one `ObjSetupObject`-adjacent object-system function; at least one floating-point-heavy function (classic IDO-ordering difficulty — that's where the workbench earns its keep).
- [ ] **Step 2:** For each target: m2c draft → iterate with asm-differ → if stuck >30 min of iteration, switch to workbench campaigns (candidate ranking) / trace analysis exactly as the DKR workflow; every workbench friction point becomes a log bullet in `docs/workbench-improvement-log.md`.
- [ ] **Step 3:** Each match commits separately with `gmake verify` green ("match <symbol> (<module>)").
- [ ] **Step 4:** Any target that resists 5 workbench campaigns: park it as GLOBAL_ASM with a `// NONMATCHING-notes:` comment block documenting every attempt insight; report it honestly — parked ≠ failed silently.

### Task D: Ontology seed

**Files:**
- Create: `docs/modules.md` (module map: what lives where — static main vs front/track/clone overlays, libultra region, asset region; every claim with ROM/vram ranges), `include/game/*.h` (first structs)
- Modify: `symbol_addrs.us.txt` (systematic naming pass), `docs/workbench-improvement-log.md`

**Interfaces:**
- Consumes: everything matched in A–C; recon file; DKR's symbol naming conventions (public); JFG's public symbol names where Mickey's code is JFG-derived
- Produces: the naming convention later phases follow (document it at top of `docs/modules.md`): `module_功能` style decisions, struct naming, where DKR/JFG names are adopted vs Mickey-specific names invented

- [ ] **Step 1:** Cross-reference every matched function + every string-adjacent function against DKR and JFG public trees; adopt their names where the code is demonstrably the same routine (byte-identity or near-identity), prefix-invent otherwise. Populate `symbol_addrs.us.txt` (hundreds of names is the right ambition here, not dozens).
- [ ] **Step 2:** First structs into `include/game/`: whatever the matched functions proved (object header fields, linker's reloc-entry struct — format from retail Mickey bytes + JFG public source). Each struct field documented with evidence (function + instruction that proves it).
- [ ] **Step 3:** Write `docs/modules.md`; commit.

### Task E: Progress metric + wrap

**Files:**
- Create: `tools/progress.py` update or objdiff wiring (implementer judgment: smallest thing that prints "N/M functions matched, X% of static segment bytes in C") 
- Modify: `README.md` (status line with real numbers), ledger/report

- [ ] **Step 1:** Make progress measurable with one command; record the Phase 1 exit numbers in README.
- [ ] **Step 2:** Final clean-room sweep (same command as Phase 0 Task 6); push branch.

## Self-review notes
- Every task keeps the byte-identical invariant — no drift risk between "matched" claims and reality; the controller can spot-verify any claim with `gmake verify`.
- Task B before C so the hard-function work happens with libultra names already in the context (call-site readability).
- Workbench-improvement logging is embedded in A (file creation), C (main source of friction), D — satisfying the standing goal without a dedicated ceremony task.
