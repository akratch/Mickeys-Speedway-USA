<!-- plateau-handoff:overlay83DrawStrip:start -->
### `overlay83DrawStrip` plateau handoff

- source: `src/overlays/o083/overlay83DrawStrip.c`
- score: 4/77 words
- frame: frameless
- relocations: 2
- first mismatch: +0x4
- summary: Fresh exact-size V0 reproduced 4/77 positional words and 73 differences; two relocation sites align, but D_80000000 runtime identity is ambiguous.

Fresh maintenance evidence on base
`3b2d4b9e3b8f6fdf3fbf7184258d341ef66edc7f`:

- overlay 83 uniquely owns `.text` offset `0x850..0x984`, ROM
  `0x18D0010..0x18D0144`: exactly `0x134` / 308 executable bytes with no
  padding before `overlay83Dispatch`;
- the ABI is `void (Overlay83Command **, Overlay83Strip *)`; no C caller,
  inbound function relocation, or overlay export identifies a direct caller;
- configured IDO 5.3 `-O2 -mips2 -32` emits an exact-size, frameless 77-word
  V0. It matches four positional words and differs in 73 raw and masked words
  from `+0x4`;
- workbench classifies the candidate as structure mismatch with 39 aligned
  structural and 39 register residuals, 17 insertions and 17 deletions. The
  target preserves the display-list argument in `a3` before the first branch;
- target and candidate both emit `R_MIPS_HI16`/`R_MIPS_LO16` sites at
  `+0x114/+0x120` under the spelling `D_80000000`. Offset, type, and spelling
  align, but preflight fails closed because that synthetic name has conflicting
  overlay runtime identities; exact relocation identity is not proved;
- the donor oracle finds JFG `func_overlay_64_04000954_1F8636C` at 304 bytes
  with masked four-gram similarity `0.538`. This is a strong diagnostic lead,
  not source authority or match proof; the actual source pass must inspect it
  under the project's provenance rules;
- historical alias, type/width, first-use, expression, and hoist forms did not
  preserve the required packet schedule. The old permuter's best gains used
  synthetic expression/control temporaries and remain inadmissible.

Next lever: one bounded, provenance-compliant donor-guided pass focused on the
saved display-list lifetime and packet macro expansion. Keep the exhausted
forms, generic flag lattice, synthetic temporaries, and ungated permutation
closed; relocation promotion also requires a unique overlay-local identity.
<!-- plateau-handoff:overlay83DrawStrip:end -->
