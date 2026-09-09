<!-- plateau-handoff:func_overlay_011_F0001E4C_186A694:start -->
### `func_overlay_011_F0001E4C_186A694` plateau handoff

- source: `src/overlays/o011/func_overlay_011_F0001E4C_186A694.c`
- score: 109/295 words
- frame: 0x40
- relocations: 87
- first mismatch: +0x10
- summary: Fresh V0 on 151b0d05 reproduces -1w and 87-site ambiguity; caller/donors unchanged, no new identity; body untouched.

Fresh maintenance evidence on base
`151b0d05ab3be01542f419cc21371de475a12c64`:

- the atlas owner and adjacent fallback bind overlay 11 `.text`
  `+0x1E4C..+0x22E8`, ROM `0x186A694..0x186AB30`: `0x49C` / 1,180
  executable bytes with no padding before `func_overlay_011_F00022E8_186AB30`;
- the ABI is `void (s32)`. The sole authenticated caller remains the
  same-overlay dispatcher at `overlay:11:+0x984`, ROM `0x18691CC`; the target
  has no export, resident inbound, or cross-overlay inbound relocation;
- configured IDO 5.3 `-O2 -mips2 -32` again emits 1,176 bytes / 294 words
  against the 1,180-byte / 295-word target. Both frames are `0x40`; 109
  positional words match, leaving 186 relocation-masked / 189 raw
  differences, first `+0x10`;
- workbench again classifies 18 aligned structural, 18 schedule, 55 register,
  and three constant residuals with six insertions and seven deletions. The
  candidate remains one instruction short and its early pool/temp web differs;
- target runtime and candidate static text surfaces each contain 87
  relocations: 31 `R_MIPS_26`, 28 `R_MIPS_HI16`, and 28 `R_MIPS_LO16`.
  Shifted sites and ambiguous candidate identity for
  `func_overlay_011_F0000000_1868848` keep `function_preflight.py`
  fail-closed; the identity refresh found no sound new resolver;
- the donor scan remains exact-negative. Weak top results are BK
  `mapModel_opa_draw` (`0.0706`), PD `endscreen_continue` (`0.0670`), and DKR
  `func_80084854` (`0.0568`); none supports source adoption;
- the retired carrier-schedule rewrite is not admissible. Prior base-offset,
  choice-alias, pointer/index, loop, declaration, flag, and permutation forms
  remain closed. This authorized pass did not mutate the body.

Next lever: recover the missing natural instruction while preserving the
`0x40` frame, then obtain non-circular identities for the same-overlay call
and shifted switch/data sites. Do not restore the retired carrier rewrite or
weaken relocation and ownership checks.
<!-- plateau-handoff:func_overlay_011_F0001E4C_186A694:end -->
