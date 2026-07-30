# Phase 0: Clean-Room Hardening + Toolchain Bootstrap + CRC-Matching Rebuild

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** A publicly-publishable repo that splits the USA ROM with modern splat and rebuilds it byte-identically (SHA1 `507341c0a40ca3e9a7cee969b396ee53facfb548`) from disassembly.

**Architecture:** Fork of Ryan-Myers/Mickeys-Speedway-USA, modernized to the DKR decomp's conventions: pip-pinned splat (no tool submodules), prebuilt ido-static-recomp + mips binutils reused from the local DKR checkout (gitignored), splat-generated linker script, Makefile producing `build/mickey.us.z64`.

**Tech Stack:** splat64 0.35.2, spimdisasm, ido-static-recomp (IDO 5.3), mips binutils (from `~/Desktop/dev/Diddy-Kong-Racing/tools/binutils`), GNU make (`gmake` on macOS), Python 3.10+ venv.

## Global Constraints

- **Clean room, enforced now:** never commit ROMs, baseroms, extracted assets/asm, IDO binaries, or anything derived from leaked material (this includes the Star Fox Adventures July-2002 debug build and the Dinosaur Planet leaked build — symbol names from those sources are OFF LIMITS). Acceptable knowledge sources: retail ROM analysis, published matching-decomp repos built from retail ROMs (DKR, JFG, Perfect Dark, Banjo-Kazooie, Conker), official SDK/libultra documentation.
- Primary target: USA ROM, SHA1 `507341c0a40ca3e9a7cee969b396ee53facfb548`. PAL SHA1 `c583ed998a6b422a22ffd3f8376c3cef0c3710d9`, JPN SHA1 `5b4f7bad6591de2199c095352a811a2eb7fc6f53`.
- Local ROM sources: USA + JPN zips in `~/Documents/Minerva_Myrient/No-Intro/Nintendo - Nintendo 64 (BigEndian)/`; PAL zip in `~/Desktop/dev/Diddy-Kong-Racing/`.
- Compiler for later phases: IDO 5.3, `-O2 -mips1 -32` (already in stub Makefile). Phase 0 rebuild is assembly-only — no C matching yet.
- macOS host: use `gmake`, and DKR's prebuilt `tools/ido-recomp/macos` + `tools/binutils` binaries.
- Commit after every green verification step, on branch `decomp-bootstrap`. Commit messages end with the Claude co-author trailer.

---

### Task 1: Clean-room policy + ignore rules (repo hygiene)

**Files:**
- Create: `.gitignore`, `docs/CLEANROOM.md`
- Modify: `README.md` (add Clean Room section near top), `docs/superpowers/specs/2026-07-30-mickey-decomp-design.md` (remove sfadebug/Dinosaur Planet as symbol sources; they remain OK only as *format documentation* references for the reloc-table layout, never for symbol names or code)

**Interfaces:**
- Produces: `.gitignore` entries every later task relies on (`baseroms/`, `build*/`, `asm/`, `assets/`, `.venv/`, `tools/ido/`, `tools/binutils/`, `*.z64`, `expected/`, `undefined_*_auto*.txt`, `ctx.c`, `*.i`, `.splat_cache`)

- [ ] **Step 1: Write `.gitignore`**

```gitignore
# ROMs and anything extracted from them — never committed (clean room)
*.z64
*.n64
*.v64
baseroms/
asm/
assets/
bin/
expected/
.splat_cache

# Build output
build/
build_*/

# Proprietary toolchain binaries (fetched/copied locally, never committed)
tools/ido/
tools/binutils/

# Generated
undefined_funcs_auto*.txt
undefined_syms_auto*.txt
ctx.c
*.i
*.o

# Environment
.venv/
__pycache__/
.DS_Store
```

- [ ] **Step 2: Write `docs/CLEANROOM.md`**

```markdown
# Clean-Room Policy

This project aims to be a fully public, legally distributable matching
decompilation. To keep it publishable:

## Never committed to this repository
- ROM images or partial ROM dumps (any region/version)
- Extracted assets, disassembly (`asm/`), or binary blobs derived from a ROM
- Proprietary compiler binaries (IDO) or SDK binaries

## Prohibited knowledge sources
Code, symbol names, comments, or structure may NOT be derived from:
- Any leaked Rare/Nintendo source code
- The leaked Star Fox Adventures debug build (July 2002) or its symbols
- The leaked Dinosaur Planet build or its symbols
- Any other unreleased/leaked build of any game

Contributors who have studied leaked Rare source code should not
contribute matched C implementations.

## Permitted knowledge sources
- Analysis of retail ROMs you legally possess
- Published matching-decomp repositories built from retail ROMs
  (Diddy Kong Racing, Jet Force Gemini, Perfect Dark, Banjo-Kazooie,
  Conker's Bad Fur Day) and their symbol naming
- Official Nintendo 64 SDK documentation and libultra headers as
  distributed in existing public decomp projects
- Emulator tracing/debugging of retail ROMs

## Build inputs
The build requires you to supply your own legally dumped ROM
(`baseroms/`), verified by SHA1 before use.
```

- [ ] **Step 3: Add Clean Room section to `README.md`** (after the title): three lines — no ROMs/assets committed, bring your own verified dump, see `docs/CLEANROOM.md`.

- [ ] **Step 4: Amend spec** — in the design spec's "Local assets" and Phase 2/3 text, delete `sfadebug` and Dinosaur Planet as *symbol sources*; keep the Dinosaur Planet public decomp repo only as documentation of the overlay/reloc mechanism ("format reference, no symbol import"). Add one-line pointer to `docs/CLEANROOM.md` under Decisions.

- [ ] **Step 5: Verify no tracked violations**

Run: `git ls-files | grep -iE '\.z64|\.n64|baserom|assets/|asm/' ; git status --porcelain`
Expected: grep returns nothing; status shows only the intended new/modified files.

- [ ] **Step 6: Commit**

```bash
git add .gitignore docs/CLEANROOM.md README.md docs/superpowers/specs/2026-07-30-mickey-decomp-design.md
git commit -m "Enforce clean-room policy (docs + ignore rules)"
```

### Task 2: Baseroms staged and SHA1-verified

**Files:**
- Create: `tools/verify_baseroms.sh`, `baseroms/` (gitignored contents)

**Interfaces:**
- Produces: `baseroms/mickey.us.z64`, `baseroms/mickey.pal.z64`, `baseroms/mickey.jpn.z64` (exact paths later tasks and yaml use); `tools/verify_baseroms.sh` exits 0 iff all present ROMs match.

- [ ] **Step 1: Write `tools/verify_baseroms.sh`**

```bash
#!/usr/bin/env bash
# Verifies any baseroms present against known-good SHA1s. Exits nonzero on mismatch.
set -u
declare -A want=(
  [mickey.us.z64]=507341c0a40ca3e9a7cee969b396ee53facfb548
  [mickey.pal.z64]=c583ed998a6b422a22ffd3f8376c3cef0c3710d9
  [mickey.jpn.z64]=5b4f7bad6591de2199c095352a811a2eb7fc6f53
)
rc=1
for f in "${!want[@]}"; do
  p="$(dirname "$0")/../baseroms/$f"
  [ -f "$p" ] || continue
  got=$(shasum -a 1 "$p" | cut -d' ' -f1)
  if [ "$got" = "${want[$f]}" ]; then echo "OK  $f"; rc=0
  else echo "BAD $f (got $got)"; exit 2; fi
done
exit $rc
```

- [ ] **Step 2: Stage ROMs** — `mkdir -p baseroms`; unzip the three source zips (paths in Global Constraints) and move/rename the `.z64` files to the `baseroms/` names above.

- [ ] **Step 3: Verify**

Run: `bash tools/verify_baseroms.sh`
Expected: three `OK` lines, exit 0. Also `git status --porcelain` must NOT list `baseroms/` (gitignore working).

- [ ] **Step 4: Commit** — `git add tools/verify_baseroms.sh && git commit -m "Add baserom SHA1 verification"`

### Task 3: Modern splat splits the USA ROM

**Files:**
- Create: `requirements.txt`, `.venv/` (gitignored)
- Modify: `mickey.us.yaml` (modernize options; correct segment layout against measured USA ROM facts)

**Interfaces:**
- Consumes: `baseroms/mickey.us.z64` (Task 2)
- Produces: `asm/`, `mickey.us.ld`, `undefined_*_auto.us.txt`; a `mickey.us.yaml` whose `target_path: baseroms/mickey.us.z64` and `ld_script_path: mickey.us.ld` the Makefile (Task 5) consumes.

- [ ] **Step 1: Write `requirements.txt`** (mirror DKR's pins)

```
splat64[mips]==0.35.2
spimdisasm==1.36.1
```

- [ ] **Step 2: Create venv + install** — `python3 -m venv .venv && .venv/bin/pip install -r requirements.txt`

- [ ] **Step 3: Measure USA ROM layout before trusting the 2023 yaml.** With a short Python script (scratch, not committed): read the entrypoint code at 0x1000 exactly as done for PAL (lui/addiu pairs) to extract BSS start/size and the jump target; scan for the overlay-region boundary (mipsiness heuristic) and the end-of-data offset. Record the numbers in the yaml comments. Expected shape (verify, don't assume): entry vram 0x80000400, main code start 0x1050, `bss_size` near the stub's `0x52D10`.

- [ ] **Step 4: Modernize `mickey.us.yaml`** — keep the stub's segment splits; update to splat 0.35.x option names (DKR's `ver/splat/dkr.us.v77.yaml` is the style reference): set `target_path: baseroms/mickey.us.z64`, add `ld_script_path: mickey.us.ld`, `migrate_rodata_to_functions: True`, `asm_function_macro: glabel` etc. per DKR; declare the overlay region and compressed-asset middle as `bin` segments for now (Phase 3/4 split them properly).

- [ ] **Step 5: Run splat**

Run: `.venv/bin/python -m splat split mickey.us.yaml`
Expected: exits 0; `asm/` populated; `mickey.us.ld` created. Iterate on yaml errors (splat names the offending option/segment) until clean.

- [ ] **Step 6: Commit** — `git add requirements.txt mickey.us.yaml && git commit -m "Modernize splat config; split US ROM with splat 0.35"` (asm/ stays untracked).

### Task 4: Assembler + IDO toolchain operational on macOS

**Files:**
- Create: `tools/setup_toolchain.sh`, `tools/binutils/` + `tools/ido/` (gitignored binaries)

**Interfaces:**
- Produces: `tools/binutils/mips-*-as`, `mips-*-ld`, `mips-*-objcopy` (or DKR's actual prefix — discover and record it); `tools/ido/cc` (IDO 5.3). Task 5's Makefile uses exactly these paths.

- [ ] **Step 1: Write `tools/setup_toolchain.sh`** — copies from a DKR checkout when present, else prints download instructions:

```bash
#!/usr/bin/env bash
set -eu
DKR="${DKR_PATH:-$HOME/Desktop/dev/Diddy-Kong-Racing}"
cd "$(dirname "$0")"
if [ -d "$DKR/tools/binutils" ]; then
  rsync -a "$DKR/tools/binutils/" binutils/
  rsync -a "$DKR/tools/ido-recomp/macos/" ido/
  echo "Toolchain copied from $DKR"
else
  echo "DKR checkout not found. Get binutils via DKR's tools/get-binutils.sh"
  echo "and IDO 5.3 from https://github.com/decompals/ido-static-recomp releases."
  exit 1
fi
```

- [ ] **Step 2: Run it and smoke-test**

Run: `bash tools/setup_toolchain.sh && ls tools/binutils | head && tools/ido/cc -version 2>&1 | head -2`
Expected: assembler/linker binaries listed (note the exact target-triple prefix); IDO identifies as 5.3. If DKR's macos dir layout differs (inspect first), adjust the rsync source paths to reality and note it in the script comment.

- [ ] **Step 3: Assemble one file as proof** — pick `asm/` entrypoint `.s`; run DKR-style: `tools/binutils/<prefix>-as -march=vr4300 -32 -mabi=32 -G0 -o /tmp/entry.o <file>`; expect exit 0.

- [ ] **Step 4: Commit** — `git add tools/setup_toolchain.sh && git commit -m "Add toolchain bootstrap script (DKR-sourced binutils + IDO 5.3)"`

### Task 5: Full-ASM rebuild matches the ROM

**Files:**
- Modify: `Makefile` (rewrite build flow around modern splat output; keep the stub's IDO variables for later C phases)
- Create: `tools/n64crc.c` compile step if stub's copy needs it (stub already has `tools/n64crc.c`)

**Interfaces:**
- Consumes: `mickey.us.ld` (Task 3), toolchain paths (Task 4), `baseroms/mickey.us.z64` (Task 2)
- Produces: `gmake` default target → `build/mickey.us.z64`; `gmake verify` → SHA1 comparison against Global Constraints value.

- [ ] **Step 1: Rewrite Makefile core flow** — targets: `all` (build ROM), `verify` (shasum compare + `OK`/`FAIL` echo), `clean`, `setup` (venv + toolchain + baseroms verify + splat). Rules: every `asm/**/*.s` → `.o` via binutils as (flags from Task 4 Step 3); every splat-emitted `.bin` → `.o` via objcopy (`-I binary -O elf32-bigmips`) exactly as the generated ld script expects; link with `-T mickey.us.ld` (+ `undefined_syms` files via `-T`); objcopy ELF → binary; pad to 32MB (0x2000000) with 0xFF; run n64crc. Use DKR's Makefile as the idiom reference, but keep this one minimal — no version matrix yet.

- [ ] **Step 2: Build**

Run: `gmake -j8`
Expected: `build/mickey.us.z64` exists, 33554432 bytes. Iterate on link errors (usually yaml segment gaps or missing bin objects) by fixing the yaml/ld and re-splitting.

- [ ] **Step 3: Verify byte-identity**

Run: `gmake verify` (i.e. `shasum -a 1 build/mickey.us.z64` vs `507341c0a40ca3e9a7cee969b396ee53facfb548`)
Expected: match. If CRC-only mismatch, n64crc step is misordered; if content mismatch, `cmp -l build/mickey.us.z64 baseroms/mickey.us.z64 | head` localizes the offset — map it back to the yaml segment and fix.

- [ ] **Step 4: Commit** — `git add Makefile && git commit -m "Rebuild US ROM byte-identically from disassembly"`

### Task 6: Push + record state

**Files:**
- Modify: `README.md` (build instructions: setup → gmake → verify; current status line)

**Interfaces:**
- Consumes: everything green above.

- [ ] **Step 1: Update README** — replace stub build notes with: prerequisites (macOS: brew make; python3), `bash tools/setup_toolchain.sh`, stage baseroms, `gmake setup && gmake && gmake verify`; status: "US ROM rebuilds byte-identically (OK-boot, 0% C)". Keep Ryan-Myers attribution prominent.

- [ ] **Step 2: Final clean-room sweep**

Run: `git ls-files | grep -iEv '\.(md|yaml|sha1|txt|sh|py|c|inc|gitignore|gitmodules)$|Makefile|LICENSE' ; git log --stat decomp-bootstrap ^origin/master 2>/dev/null | grep -iE 'z64|baserom|asm/'`
Expected: nothing suspicious; every tracked file is text/config/scripts.

- [ ] **Step 3: Commit README, push branch**

```bash
git add README.md && git commit -m "Document Phase 0 build; record status"
git push -u origin decomp-bootstrap
```

## Self-review notes

- Spec coverage: Phase 0 bullet of the spec (toolchain, splat, CRC rebuild) → Tasks 2–5; new clean-room requirement → Task 1 (incl. spec amendment); progress-CI and objdiff deferred to a later plan deliberately (YAGNI for a pure-ASM rebuild).
- The stub's `.sha1` manifest files double as the Task 2/5 truth values — plan states the values inline so workers never need the stub context.
- Types/paths consistent: `baseroms/mickey.us.z64`, `mickey.us.ld`, `tools/binutils/`, `tools/ido/cc` used identically across Tasks 2–5.
