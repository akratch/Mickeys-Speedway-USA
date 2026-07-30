# Mickey's Speedway USA — Full Matching Decompilation: Design

**Date:** 2026-07-30
**Goal:** A 100% byte-matching decompilation of Mickey's Speedway USA (N64, Rare 2000), in the mold of the finished DKR decomp.

## Decisions (approved 2026-07-30)

- **Base repo:** Fork of `Ryan-Myers/Mickeys-Speedway-USA` (this repo). The stub is infrastructure-only (2 named symbols, no `src/`, auto-detected file boundaries), by the author of the active Jet Force Gemini decomp. We build on it and keep the upstream relationship; outreach to Ryan framed as reviving his skeleton.
- **Versions:** All three retail versions are on disk and SHA1-verified against the stub's manifests: USA `507341c0…`, JPN `5b4f7bad…`, PAL `c583ed99…`. **USA (NMLE) is the primary matching target** (community standard, NTSC); PAL/JPN configs are kept buildable, DKR-style.
- **End goal:** Full matching decomp. Ports come later, on top of source truth.

## Orientation findings the design rests on

- **Engine lineage:** DKR → Jet Force Gemini → Mickey → Star Fox Adventures. Mickey and JFG share the identical four-module layout (`main/main.c`, `front/front.c`, `track/track.c`, `clone/clone.c`), the same runtime overlay linker (`ERROR:MIPS_HI16 without matching MIPS_LO16`, `Unimplemented linkage operation`), the same `ra=0x666` profiler string and `ObjSetupObject` object system. ~25% of Mickey's static segment is byte-identical to JFG's in aligned 16-byte runs (PAL vs JFG US measurement; true shared-source fraction higher).
- **ROM layout (PAL; USA expected analogous):** `0x1000`–`0x86990` uncompressed boot+main code → vram `0x80000400` (entry clears BSS at `0x80085d90`, jumps to `0x800212e0`); ~`0x16b0000`–`0x18fc000` uncompressed overlay code + reloc tables (`front`/`track`/`clone`); middle ~21 MB compressed assets (Rare `1172`/`1173` deflate; SubDrag's Rareware tool supports MSUSA).
- **Compiler:** IDO 5.3 `-O2 -mips1 -32` (already configured in this repo's Makefile) — the proven toolchain for every matched Rare N64 decomp (DKR 100%, BK 100%, PD >99%, GoldenEye ~93%, Conker WIP).
- **Local assets:** DKR repo at `~/Desktop/dev/Diddy-Kong-Racing` (100% matched source, working splat 0.35.2 venv, macOS ido-recomp binaries, asm-differ/m2c/asm-processor/permuter, `tools/decomp-workbench`); Dinosaur Planet decomp at `~/Desktop/dev/dp64` (same Rare DLL/reloc system); SFA debug-build decomp (`sfadebug`, GitHub) as a symbol-name source for inherited code.

## Architecture of the effort

- **Phase 0 — Bootstrap.** Modernize this repo's tooling (populate submodules or vendor DKR's pinned tools; build ido-static-recomp for macOS), stage baseroms, run splat on USA, achieve a CRC-matching rebuild from pure disassembly (`.s` → ROM). Success: `make` produces `mickey.us.z64` with matching SHA1.
- **Phase 1 — libultra + boot.** Identify the libultra version; port sources from DKR/JFG (largely byte-identical objects); match entrypoint/OS init. Success: libultra TUs matched, boot path in C.
- **Phase 2 — Symbol porting at scale.** Binary-similarity mapping of Mickey functions onto DKR's 100% tree and JFG's active tree (decomp-workbench relocation-aware comparison is the intended instrument). Import names, structs, and candidate C wholesale; permuter + asm-differ close the deltas introduced by engine evolution. This phase is the accelerant and runs continuously alongside later phases.
- **Phase 3 — Overlay system.** Reverse the reloc-table format of the runtime linker (reference: Dinosaur Planet DLL tooling, JFG repo), teach splat the `front`/`track`/`clone` segments, integrate overlay builds + relocation emission into the Makefile. Success: overlays rebuild byte-identically.
- **Phase 4 — Assets.** `1172`/`1173` decompress/recompress with matching output (Conker repo documents the matching-gzip approach; DKR's `dkr_assets_tool` is the structural model). Success: full asset extraction, shiftable build.
- **Ongoing:** objdiff + decomp.dev CI progress reporting; multi-version (PAL/JPN) kept green; coordinate with Ryan-Myers (JFG) to share engine symbols both ways.

## Error handling / risk

- **Non-shiftable interim states:** until Phase 4, rebuilds reinsert original compressed asset blobs verbatim — standard practice, keeps CRC checks meaningful.
- **Compiler-flag drift:** if IDO 5.3 `-O2` fails systematically on early functions, test per-file `-O1`/`-g` overrides (JFG does this for libultra) before questioning the toolchain.
- **PAL/USA divergence:** all measurements so far are PAL; Phase 0 re-verifies layout offsets against USA before configs are trusted.
- **ROM/legal hygiene:** no ROMs, extracted assets, or IDO binaries committed; baseroms stay gitignored.

## Testing

- Continuous: SHA1/CRC match of rebuilt ROM per version; asm-differ zero-diff as the per-function match criterion; objdiff report as the progress metric in CI.
- Behavioral spot-checks in an emulator for NON_MATCHING experiments only; matching builds need no behavioral testing by construction.
