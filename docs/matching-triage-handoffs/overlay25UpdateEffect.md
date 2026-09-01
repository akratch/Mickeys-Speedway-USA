<!-- plateau-handoff:overlay25UpdateEffect:start -->
### `overlay25UpdateEffect` plateau handoff

- source: `src/overlays/o025/overlay_025.c`
- score: 132/259 words
- frame: 0xC8
- relocations: 25
- first mismatch: +0x0
- summary: Fresh exact-size V0 reproduces 132/259 words; frames are 0xC8/0xA0. 24/25 relocation sites align, but 23 identities remain unresolved.

Fresh maintenance evidence on base
`3b2d4b9e3b8f6fdf3fbf7184258d341ef66edc7f`:

- the fallback and adjacent exact mixed-TU owner uniquely bound overlay 25
  `.text` offset `0x17C..0x588`, ROM `0x1879E04..0x187A210`: `0x40C` /
  1,036 executable bytes with no target padding before
  `overlay25SetVectorFlags`;
- the ABI is `void (Overlay25Object *, s32)`. ORT entry 1274 and resident
  relocation 116 authenticate the sole inbound call from
  `func_8000AEEC+0x274`; no cross-overlay inbound exists;
- configured IDO 5.3 `-O2 -mips2 -32 -Wab,-r4300_mul` emits an exact-size
  259-word V0 with 132 positional matches and 127 raw and masked differences
  from entry. Its `0xC8` frame is 40 bytes larger than the target's `0xA0`;
- workbench classifies 39 aligned structural, three schedule, 93 register, and
  eight constant residuals, with 15 insertions and 15 deletions. The first
  integer pool divergence is slot four; FP pool/temp divergence appears later;
- target and candidate each expose 25 relocations and 24 sites align by offset
  and type. Only two candidate identities resolve and align; the remaining 23
  include overlay-local data pairs and resident/cross-overlay calls, so exact
  relocation identity proof is incomplete;
- the donor oracle's best size-compatible result is only `0.056` masked
  four-gram similarity. Existing Mickey documentation also records no
  corresponding DKR/JFG source or object match, so no credible donor applies;
- the historical 119-flag lattice, declaration order, mutually exclusive
  scopes, position/radius/object-array ordering, shared-constant form, and
  bounded permuter are exhausted. The permuter's prior gain required a vacuous
  guard and remains inadmissible.

Next lever: tool `f149a4a3209d977796d7bf5d1ef9a9cd4362561e` adds
procedure-scoped source-semantic UGEN evidence unavailable to the old plateau.
Use one fidelity-gated stack-home/web capture and permit at most one natural
lifetime form only if that trace uniquely attributes the `0x28` frame excess.
Do not repeat flags, declaration/scope variants, fake guards, or permutation.
<!-- plateau-handoff:overlay25UpdateEffect:end -->
