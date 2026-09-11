<!-- plateau-handoff:func_8004EED0:start -->
### `func_8004EED0` plateau handoff

- source: `src/main/block_4F4E0.c`
- score: 28/84 words
- frame: 0x20
- relocations: 22
- first mismatch: +0xA8
- summary: Pure ring residual; the base-plus-index order is reachable with a u32 cast and costs a word, and the phase is a reordered free list, not a counter

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
#### 2026-09-12 (lane `lane/p7-res2`): the base-plus-index order is reachable and costs a word; the ring phase is not a counter

Still 28, and the shape is unchanged: 84 instructions, frame 0x20, 56 byte-exact
and 28 register-naming words with nothing immediate-only and nothing really-
different. The 2026-09-09 shard asks for "one fidelity-clean UGEN allocation
trace at the post-call temporary-ring divergence". That is still the right
request, and this pass narrows what the trace has to explain.

**The one structural difference in the residual is reachable from source, and
adopting it costs a word.** The candidate reassociates the two-word table
access into index-plus-base where the target has base-plus-index. Adding the
pointer and the scaled index as `u32` flips it -- confirmed by reading the
objects, the addend's operand order is the target's afterwards -- and the score
goes 28 to 29, because the flip rotates the temporaries one further place in the
opposite direction. Five spellings of that cast were measured (both operands
cast, the base alone, the index alone, a shift in place of the multiply, and an
address-of a typed array index) and all five are 29. So the operand-order lever
from the 2026-09-11 laws applies here and is real, but it is not free on this
function, and it must be composed with a phase lever rather than adopted alone.

**The phase is not a single counter, which rules out the simplest L127 reading.**
Read in address order, the last block after the final call is a clean +2 for
every temporary, and so is the block before it; the block between them drifts
+2, +4, +6, +8, +8; and the first temporary after the first call is -2. The
prefix through +0xA4 is byte-exact and draws the same three temporaries on both
sides, so the divergence is entirely in draws the instruction stream does not
show. Counting visible allocations between the first call and the tail gives
eleven on each side, so **the two sides make the same number of visible draws
and still diverge** -- the free list is being reordered, not advanced, and a
"consume N more temporaries" edit cannot be the whole answer.

Levers measured and flat, 126 forms in total:

- 78 identity-operation probes (or-with-zero, and-with-minus-one on the index,
  a self-assignment of the loaded word) at three insertion points crossed with
  two pointer spellings and three offset spellings: every one byte-identical to
  its base. L135 again -- uopt folds them before the web builder, so they are
  not probes. The two probes that are not identities (re-masking the table index
  and a self-assignment of the section pointer) add 12 to 20 bytes of real
  instructions.
- 48 local-naming forms over L76's axis: the language result named or the call
  inlined into the switch, the first section offset named or written in place,
  the section base named or read directly, the table pointer named or inlined,
  and the loaded word named or inlined -- crossed with the `u32` cast. Naming
  the base is 38; everything else is 28 or, with the cast, 29.

Resume with the free-list trace, and read it for *order* rather than for a count:
the question this pass leaves is why eleven draws on each side hand out different
registers, which is a question about when each temporary is returned to the list,
not about how many are taken from it.
<!-- plateau-handoff:func_8004EED0:end -->
