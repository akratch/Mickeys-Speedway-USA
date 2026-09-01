<!-- plateau-handoff:overlay14ResetMode:start -->
### `overlay14ResetMode` plateau handoff

- source: `src/overlays/o014/overlay14ResetMode.c`
- score: 56/56 words
- frame: 0x30
- relocations: 18
- first mismatch: relocation +0x54
- summary: Scheduler line-tie lever makes all 56 instructions exact; four raw addend words and relocation identity proof remain.
- base: `435e1fbbd7abfb5c2c5bc0d1dd16a229a92135c6`
- boundary: overlay 14 `+0x498..+0x578`, 224 bytes / 56 words, no padding or
  export, five local inbound jumps
- configured V0: exact geometry, 56/56 normalized instruction words, four raw
  addend differences, and frame `0x30` on both sides
- relocations: the target runtime surface authenticates 18 records and the
  candidate emits 18 records at the expected type/offset sites; the +0x54 call
  alias still needs runtime identity proof, while the friendly `F8`, `D8`,
  `DC`, and `E0` data aliases currently carry zero rather than section addends
- allocator evidence: the pinned IDO 5.3 global-color trace passed `.text`,
  relocation, and symbol fidelity; it selected seven integer webs (`v0` twice,
  then `s0` through `s4`)
- scheduler evidence: the native `as1 -R` trace passed text, data, rodata,
  relocation, and symbol fidelity and recorded 82 events / 49 ties; its
  prologue block showed the saved-register placement winning a source-line tie
- strict gain: placing the function header, local declaration, and `do` header
  on one physical line preserves C tokens and closes all seven normalized words
- exhausted controls: all 119 flag configurations tied V0; an explicit
  command-type carrier was byte-identical; an explicit `D_EC` pointer lifetime
  regressed to 55 words, frame `0x28`, and 43 normalized differences
- blocker: instruction scheduling is closed; exact promotion is blocked only by
  relocation identity/addend proof
- next lever: test the authenticated overlay-14 `D_F8`, `D_D8`, `D_DC`, and
  `D_E0` section identities, then prove the +0x54 call alias against ORT1652;
  do not repeat flags, permutation, carrier, or address-base lifetime forms
<!-- plateau-handoff:overlay14ResetMode:end -->
