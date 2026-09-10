<!-- plateau-handoff:func_80028FCC:start -->
### `func_80028FCC` plateau handoff

- source: `src/main/main.c`
- score: 17/27 words
- frame: 0x18
- relocations: 3
- first mismatch: +0x1C
- summary: Not a donor question: the target is cfe's short-circuit-or value expansion with its carrier on the return register, and v0 is struck from that carrier web's candidate list because every operand's definition reads a call result (L101), which two probes here demonstrate directly.

#### Owned pass, 2026-09-10 (lane/o7-small): the shape is known, not a donor

Still ten, and the previous "next lever is a later permitted donor C body" is
superseded. The target's shape is now read out of ugen and named.

The target emits, once per arm:

a normalise of the call result into a RING temp `tN`, a short-circuit branch
on `tN` straight to the epilogue, and a copy of `tN` into `v0` in that
branch's delay slot -- with `tN` running `t6, t7, t8` across the three arms.
That is exactly cfe's short-circuit-or value expansion (L51) -- `v = a;
if (v) goto L; v = b; ...` -- with two properties the retained candidate
cannot reproduce together:

1. the carrier is the **return register**, so each arm pays its own `move v0`,
   and the branch target is the epilogue rather than a join above it; and
2. the operand is a **ring temp**, not a coloured web.

Written as a bare short-circuit-or of the three calls, the same three-node
expansion appears in `cc -S` -- normalise into a temp, copy the temp to the
carrier, branch on the temp -- but uopt colours the operand `v1` and the
carrier `a0`, the two coalesce into one register, and the result is 25
instructions with a single `move v0,a0` at the join. Written as three `if
(ret) return ret;` statements it is 29: an inverted branch plus a `b` to the
epilogue per arm. The target is 27, exactly between them, and no cell of the
lattice below lands there.

**Named decision variable, with the record.** The carrier web's colour. v0 is
not *forbidden* to it -- it is not offered, because the web's span reaches a
call result (L101). Two probes in this TU demonstrate the gate directly rather
than by inference:

- replacing only the first operand with `arg0 != 0`, so that one operand is
  not a call result: it is normalised straight into `$2`.
- replacing all three operands with tests on `arg0`, so the function makes no
  call at all: the operands take `$2` twice.

Change only the first operand back to a call and `$2` disappears from the
assignment. All three of this function's operands are call results, so no
spelling of *this* function can put the carrier on v0.

Exhausted this pass, all at 25 or 29 instructions, never 27: a 1,440-cell cross
product of six operand spellings (a bare call, `!= 0`, `!!`, a doubled `!= 0`,
`> 0`, and a `(u32)` cast before the test) crossed with four groupings, four
declaration paddings, five result forms (returned directly, through an `s32` or
`u32` local, normalised again inside or outside) and three dead-expression
reservation webs; plus, singly,
the written-out value expansion with one, two and three named carriers, `goto`
and `do {} while (0)` and `while (1)` exits, an assignment inside the `if`, a
conditional-expression chain, a bitwise-or chain, and nine carrier types
(`s32`, `u32`, `s16`, `u8`, `u16`, `s8`, `int`, `unsigned int`, `long`).

**Next lever.** Either an instrumented `p1cost` capture confirming v0's absence
from this carrier's candidate list -- which would close the symbol as
unreachable by any source edit -- or a construct that gives the carrier a
definition not derived from a call result while keeping all three `jal`s and
their argument setup byte-exact. The donor line is closed: JFG's
`mainAnyoneHas` is still GLOBAL_ASM at efd5abb and supplies no C body.
<!-- plateau-handoff:func_80028FCC:end -->
