<!-- plateau-handoff:overlay14ResetMode:start -->
### `overlay14ResetMode` plateau handoff

- source: `src/overlays/o014/overlay14ResetMode.c`
- score: 49/56 words
- frame: 0x30
- relocations: 18
- first mismatch: +0x14
- summary: Fresh current-alias V0 has 7 masked/11 raw; flag lattice tied; trace proves schedule residue; candidate identity proof remains pending.
- base: `33379b3934163b2d62a4dfe58249bd3561b6ce1f`
- boundary: overlay 14 `+0x498..+0x578`, 224 bytes / 56 words, no padding or
  export, five local inbound jumps
- configured V0: exact geometry, 49/56 normalized words, seven masked and
  eleven raw differences, first mismatch `+0x14`, frame `0x30` on both sides
- relocations: the target runtime surface authenticates 18 records; the
  candidate emits 18 static records with 17/18 offset/type alignment, but only
  1/18 candidate identities resolve statically and linked-runtime alignment is
  zero, so candidate identity proof remains pending
- allocator evidence: the pinned IDO 5.3 global-color trace passed `.text`,
  relocation, and symbol fidelity; it selected seven integer webs (`v0` twice,
  then `s0` through `s4`)
- exhausted controls: all 119 flag configurations tied V0; an explicit
  command-type carrier was byte-identical; an explicit `D_EC` pointer lifetime
  regressed to 55 words, frame `0x28`, and 43 normalized differences
- blocker: only the prologue saved-register/address-materialization schedule
  remains; normalized instructions from `+0x40` onward are identical
- next lever: do not repeat flags, generic permutation, carrier, or address-base
  lifetime forms without new compiler evidence for this exact schedule
<!-- plateau-handoff:overlay14ResetMode:end -->
