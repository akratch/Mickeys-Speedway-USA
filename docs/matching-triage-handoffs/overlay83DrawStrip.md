<!-- plateau-handoff:overlay83DrawStrip:start -->
### `overlay83DrawStrip` plateau handoff

- source: `src/overlays/o083/overlay83DrawStrip.c`
- score: 4/77 words
- frame: frameless
- relocations: 2
- first mismatch: +0x4
- summary: Authorized JFG packet-order form regressed from 73 to 74 differences and shifted both relocations; baseline remains best and donor has no C body.

Authorized donor-pass evidence on layered base
`99496346799c96eea701dece64f1c0015b3e76aa`:

- the pinned authorization and current structured evidence are
  `c98707b3d50df6ea8d4953cc720aaadb153b9b99` and
  `e70d0e4907bee5b88c82506dc776f9c35bbac480`;
- overlay 83 uniquely owns `.text` offset `0x850..0x984`, ROM
  `0x18D0010..0x18D0144`: exactly 308 executable bytes / 77 words, frameless,
  with no padding before `overlay83Dispatch`;
- the permitted JFG donor is an assembly-only fallback at
  `src/overlays/o64/overlay_64.c`, not a C implementation. Its 304-byte
  function has the same four packet semantics and `0.538` masked four-gram
  similarity, but it cannot supply source spelling or lifetime authority;
- the one natural donor-guided experiment retained the existing saved-list
  alias and expressed the four packet writes directly in the observed store
  order. It remained exact-size and frameless but regressed from 73 to 74 raw
  and masked differences;
- that experiment also moved the candidate HI16/LO16 sites from the baseline's
  target-aligned `+0x114/+0x120` to `+0xE4/+0xEC`. It was rejected and its
  untracked object was preserved under `build/attempts/overlay83DrawStrip/`;
- the original C body was restored and freshly recompiled. It reproduces the
  77-word baseline with four matching positional words, 73 differences from
  `+0x4`, and the two target-aligned relocation sites;
- preflight still fails closed because `D_80000000` has conflicting overlay
  runtime identities. Offset, type, and symbol spelling agreement alone does
  not satisfy exact relocation identity proof;
- prior alias, type/width, first-use, expression, hoist, generic flag,
  synthetic-temporary, and ungated-permutation families were not reopened.

Next lever: obtain source-authentic packet macro/lifetime evidence from a
permitted C donor or a procedure-scoped compiler trace, and separately bind
the two candidate relocations to a unique overlay-local identity. The current
assembly-only donor falsifies direct packet-store transcription as a lever.
<!-- plateau-handoff:overlay83DrawStrip:end -->
