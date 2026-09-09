<!-- plateau-handoff:overlay62Update:start -->
### `overlay62Update` plateau handoff

- source: `src/overlays/o062/overlay62Update.c`
- score: 287/294 words
- frame: 0x88
- relocations: 71
- first mismatch: +0x44
- summary: Arithmetic spelling proved: the assembler temporary shows the source multiplies, it does not shift a negated operand; on that base the residual is one pure register rotation and 2,700+ swept cells hold.
- summary: Arithmetic spelling proved: the assembler temporary shows the source multiplies, it does not shift a negated operand; on that base the residual is one pure register rotation and 2,700+ swept cells hold.

#### 2026-09-09 lane `lm-bigsingles`: the arithmetic spelling is now proved

The 287/294 baseline reproduces on this lane's assignment base. A new,
mechanism-level identity closes the first mismatch and reclassifies everything
after it.

The target's first differing word negates the timer into the **assembler
temporary**. `ugen` never allocates that register outside its own `.set noat`
sequences -- verified across every translation unit in the tree -- so the word
cannot come from register allocation at all. It is an `as1` macro expansion:
`as1` expands a multiply by a constant using that temporary as its scratch, and
two multiplies by negative constants in one block share a single negation into
it. Assembling a stand-alone probe of two such multiplies reproduces the
target's sequence, including the shared negation, exactly.

Therefore the two colour components are written in source as multiplications by
negative constants, not as shifts of a negated operand. The retained C uses the
shift spelling; that spelling makes `ugen` strength-reduce into a pool register
and can never produce the target word. Switching both components to the
multiply spelling moves the first mismatch from +0x44 to +0x50 and makes every
remaining difference a pure register rename with no insertions, deletions or
opcode changes: one carrier that the target keeps in a temporary is taken by a
named colour variable in the candidate, and the whole lane rotates by one
position for the rest of the function. Applying the multiply spelling to only
one of the two components is much worse (two separate negations), which is
itself a confirmation that both are multiplies.

The rotation did not yield to search. On the multiply base: all 720 declaration
orders, a 1,920-cell lattice of declaration order crossed with every subset of
`register` hints and with the volatile qualifier present or absent, both
assignment directions for the shared pair, separated versus chained assignment,
operand order in the multiply, constant-addend order, and the alternate
spelling of the second branch. Every cell held at the same value or regressed.
Two further facts fell out: the `register` keyword has no effect whatever on
this unit at the configured optimisation level, and reversing the shared
assignment direction costs four words.

Because the multiply form scores worse under positional word counting while
being provably the true spelling, the shift-spelled body is retained as the
better-ranked candidate and this note carries the identity. A worker resuming
this target should start from the multiply spelling regardless of its score:
the residual there is one rotation, not a mixed structural residual. The lever
to find is whatever makes the first multiply's destination a scratch rather
than the second colour variable's own colour; the candidate computes in place,
the target does not.
<!-- plateau-handoff:overlay62Update:end -->
