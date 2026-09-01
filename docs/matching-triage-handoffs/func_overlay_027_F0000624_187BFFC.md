<!-- plateau-handoff:func_overlay_027_F0000624_187BFFC:start -->
### `func_overlay_027_F0000624_187BFFC` plateau handoff

- source: `src/overlays/o027/overlay_027.c`
- score: 59/254 words
- frame: 0x98
- relocations: 15
- first mismatch: +0x8
- summary: Fresh V0 is exact-sized; 15/15 relocation counts/types but ambiguous and shifted proxies close identity proof. Prior forms closed; body untouched.

Fresh maintenance evidence on base
`0df1962d37471fe5d81d38a31b0a65df8b7b7da8`:

- the assembly fallback and adjacent function boundary bind overlay 27 `.text`
  `+0x624..+0xA1C`, ROM `0x187BFFC..0x187C3F4`: `0x3F8` / 1,016
  executable bytes with no target padding before `overlay27UpdateCoordinates`;
- the ABI remains `void (O27Command **, void *, s16 *, O27Object *)`.
  ROM-table export 1189 owns `overlay:27:+0x624`; resident relocation 31 at
  ROM `0xA654` is its sole authenticated inbound call, and no overlay-local
  SYMBOL relocation targets that export;
- configured IDO 5.3 `-O2 -mips2 -32` emits exactly 1,016 bytes / 254 words.
  Target and candidate frames are both `0x98`; 59 positional words match,
  leaving 195 relocation-masked / 196 raw differences, first `+0x8`;
- workbench classifies 46 aligned structural, three schedule, 145 register,
  and three constant residuals, with 18 insertions and 18 deletions. The
  dominant early divergence is the known object/child pool-home swap;
- target runtime and candidate static surfaces each contain 15 relocations:
  seven `R_MIPS_26`, four `R_MIPS_HI16`, and four `R_MIPS_LO16`. Several
  schedules are shifted, and candidate proxy `D_80000050` has no unique
  runtime identity, so `function_preflight.py` correctly remains fail-closed;
- the fresh donor scan is exact-negative. Its weak top results are PD
  `filemgr_render_perfect_head_thumbnail` (`0.0675`), JFG `fxDrawCone`
  (`0.0667`), and DKR `func_80080E90` (`0.0629`); none supplies a defensible
  source-shape adoption;
- the existing flag lattice, allocation/lifetime/order forms, and bounded
  permuter remain exhausted. This evidence pass did not mutate the body or
  run new flags or permutation.

Next lever: first obtain a non-circular overlay-local identity for
`D_80000050` and the shifted relocation sites. Only a newly authenticated
source or allocator mechanism may then reopen the body; do not repeat the old
register/lifetime family or weaken relocation identity checks.
<!-- plateau-handoff:func_overlay_027_F0000624_187BFFC:end -->
