<!-- plateau-handoff:func_8004EED0:start -->
### `func_8004EED0` plateau handoff

- source: `src/main/block_4F4E0.c`
- score: 56/84 words
- frame: 0x20
- relocations: 22
- first mismatch: +0xA8
- summary: Exact 84-word opcode/frame shape; 28 register-only words and one shifted static tuple remain after the first ROM load; permuter 225->85 was nonexact.

Fresh evidence on base `1783e591a5a7436c6b9b148171805995c9fb3bde`:

- the unique resident owner is `0x8004EED0..0x8004F020` / ROM
  `0x4FAD0..0x4FC20`, exactly `0x150` bytes with no target padding before
  `func_8004F020`;
- configured IDO `-O2 -mips2 -32` emits the exact 84-instruction extent and
  `0x20` frame. The prefix through `+0xA4` is exact; all 28 remaining sites
  are register-only allocation differences after the first
  `piRomLoadSection` call;
- target and candidate each emit 22 static relocations and all 22 candidate
  identities resolve. Twenty-one offsets, types, and effective identities
  align; the `D_800D6AD8` HI16 is displaced by one instruction;
- a linked promotion diagnostic using the authenticated resident `.main`
  range confirms 28 differing words inside the owner and zero differences
  outside it. The assembly fallback remains canonical;
- combining the first ROM-load call with the following pointer assignment and
  introducing a named section-size local in both declaration orders are
  byte-neutral. A ten-minute bounded permuter run improved its internal score
  from 225 to 85 but did not reach zero; its best artifact relied on an
  uninitialized value multiplied by zero and was rejected as semantically
  invalid. No generated source was applied.

Next lever: capture one fidelity-clean UGEN allocation trace at the post-call
temporary-ring divergence, then test only a source-authentic phase or lifetime
change supported by that trace. Do not repeat the bounded permutation or the
three byte-neutral source forms above.
<!-- plateau-handoff:func_8004EED0:end -->
