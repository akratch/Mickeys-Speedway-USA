<!-- plateau-handoff:func_80046BCC:start -->
### `func_80046BCC` plateau handoff

- source: `src/main/diCpu.c`
- score: 68/106 words
- frame: 0x40
- relocations: 3
- first mismatch: +0x2C
- summary: Size closed at 106/106: folding the m2c-only var_s0 into var_s2 removes the extra saved-carrier copy; 41 differing words, relocations exact.

#### Size-question closure, 2026-09-09

- The `+4` overrun was one saved-carrier copy at the loop head. With the m2c
  draft's `var_s0` declared, uopt split `var_s2`'s web: the masked character
  was computed into a caller-saved carrier and then copied into the callee-
  saved one, where the target writes the callee-saved register directly.
  `var_s0` is not a source variable -- it is that copy. Deleting the
  declaration and spelling every one of its reads as `var_s2` (the two values
  are equal on every path, since each arm assigns one from the other) removes
  the copy and leaves the compiler free to create the same `move` itself.
- Configured full-TU: 106 candidate versus 106 target words (was 107),
  instruction delta 0 (was +1), 41 differing words (was 89), first `+0x2C`,
  frame `0x40` both sides, and the three relocation sites now agree in offset,
  type and identity (the previous form shifted all three).
- Measured and rejected on the way: moving `var_s4 += 1` down beside the
  pointer read (51 positional rows but the delta stays +1); dropping only the
  `var_s2 = var_s0` write-back (+1, 92); combining that move with the fold
  (delta 0 but 80 rows, because the freed register changes the hoist set);
  swapping the `var_s2 = 0` / `var_s3 = 0` initialisers; masking or not masking
  `temp_s6`; testing both bounds on one variable; incrementing at the read with
  `*var_s4++` and `*++var_s4`; and `var_v0 &= 0xFF` in place (+2).
- What is left is allocation, not size or structure: the fold frees one
  callee-saved register, so the candidate hoists a third loop-invariant
  constant (`0x78`) that the target materialises inline with `li at`, and the
  whole saved-register assignment rotates behind it. Nesting the last test,
  swapping the last two tests, an unused declared local at either end of the
  list, and a `u8` `var_v0` are all byte-inert against that (48 rows each).
  Resume with evidence for the ninth callee-saved web rather than more
  spelling permutation.
<!-- plateau-handoff:func_80046BCC:end -->
