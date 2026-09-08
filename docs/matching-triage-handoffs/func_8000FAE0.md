<!-- plateau-handoff:func_8000FAE0:start -->
### `func_8000FAE0` plateau handoff

- source: `src/main/track.c`
- score: 20 differing words
- frame: 0x10
- relocations: 2
- first mismatch: +0x1C
- summary: Pinned donor source has no matching C body; declaration probe flat and loop probe regressed; next lever is an authenticated donor C body

- Reopen audit (2026-09-08): the local Jet Force Gemini checkout is exactly
  `efd5abb1c79636e297b831f7c2d5bf47eac39c0c`, with an unchanged tracked
  `src/track.c`. That file retains `trackGetBlock` as `GLOBAL_ASM`; there is
  no newly matched donor C body to adapt for this target. The near-match
  oracle ranks `trackGetBlock` first (300 bytes, masked 4-gram Jaccard 0.0561).
  The donor scan is structural evidence, not a byte-identity or name adoption.
- ABI and ownership: Mickey takes three single-precision coordinates and
  returns a segment index. Its resident range is VRAM `0x8000FAE0..0x8000FBD8`,
  ROM `0x106E0..0x107D8`, with 248 executable bytes and no owned padding.
  Both games scan signed-halfword bounding boxes, but JFG selects distance
  to the vertical midpoint while Mickey accepts containment immediately and
  otherwise measures distance to the nearest vertical bound. Importing the
  donor behavior would therefore change Mickey's semantics.
- Fresh configured full-TU baseline: `-O2 -mips2 -32 -Wab,-r4300_mul` with
  `-DNON_MATCHING`, 62 target and candidate words, 42 identical words, 20 raw
  and relocation-masked differences, first `+0x1C`, and matching 16-byte frames.
  Both relocation records agree in relative offset, type and symbol identity:
  the HI16/LO16 pair for `D_800792E8`. This is compiled nonexact evidence;
  the candidate remains guarded and earns zero new matching bytes.
- Workbench: comparison `allocation-mismatch`; mechanism `allocation`,
  playbook `register-role-audit`, owning pass unknown. The forced-color-oracle
  guide says a trace or authenticated reservation state is required before
  any allocator intervention; neither is available or authorized here.
- Attempt evidence: attempt 1 reordered x/z bound declarations to the target
  load order and remained at 20 differing words. Attempt 2 removed the loop
  condition carrier and regressed to 61 instructions, 27 differing words,
  7 opcode mismatches, and a relocation mismatch; the best candidate was
  restored. The report-only m2c sweep found no eligible bare GLOBAL_ASM because
  this symbol already has a guarded candidate.
- ADR 0018 stopping evidence: the permitted new-donor-C hypothesis remains
  disproved by the pinned source; the only source probe preserving shape was
  flat and the alternative was a measured regression. Repeating declaration,
  flag, allocator, or permuter work would not test the authorized mechanism.
  No claim is made that the function is inherently unmatchable.
- Next concrete lever: obtain a published matching `trackGetBlock` C body and
  re-prove its ABI and vertical-selection differences against Mickey. An
  allocator investigation would need separately authorized mechanism and
  authenticated register-role evidence.
- Evidence retained locally in ignored `build/wb/`: baseline source/object,
  comparison summary, provenance receipt, diagnosis and donor-oracle result.
  Commands: `tools/wb_compare.sh --summary-json func_8000FAE0`, workbench
  `diagnose` on the configured `build_non_matching/src/main/track.c.o`,
  workbench `guide forced-color-oracle` and `guide register-role-audit`,
  `python3 tools/m2c_sweep.py --symbol func_8000FAE0 --fresh`, and
  `tools/finalize_plateau.py` with the measured fields above.
<!-- plateau-handoff:func_8000FAE0:end -->
