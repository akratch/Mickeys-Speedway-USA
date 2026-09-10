<!-- plateau-handoff:overlay2QueryNode:start -->
### `overlay2QueryNode` plateau handoff

- source: `src/overlays/o002/overlay2QueryNode.c`
- score: 214/253 words
- frame: 0x68
- relocations: 51
- first mismatch: +0x40
- summary: Not a colouring residue: 168 single-web globalcolor forces leave it at 39 or worse. The shipped short-circuit blocks emit sltu/bnez/move, the signature of a C short-circuit or and not of an if-return-1; the short-circuit spelling reproduces that triple exactly in the object and costs one duplicated node->side load per block, so it lands eight bytes short. The boolean shape and the duplicated load are wanted together and no spelling yet gives both.

#### 2026-09-09 lane `w2-bigA`: 40 to 39, and the residual is four named sites

One strictly better cell: writing the split test as `D_50 == D_54` rather than
`D_54 == D_50` fixes the operand order of the `bne` that implements it. The
baseline is now 214/253 relocation-masked words (51 raw), 51 of 51 relocations
emitted, frame 0x68, size delta 0.

The remaining 39 words are four sites, each localized to a specific choice.
Recording them so the next worker searches the right space:

- **Leaf-loop join, 7 words.** After the loop, the target rematerializes the
  leaf count as a zero into the argument register it is returned from; the
  candidate instead keeps a second live copy of the counter, made in the loop
  tail. Both sides carry the counter in the same register up to that point, and
  the two forms have the same instruction count. Eliminated: `register` on the
  counter (inert), both orders of the counter update against the cursor
  increment, `!= 0` on the loop condition, moving the pre-decrement into the
  initialiser, reordering the four head statements, and an explicit `count = 0`
  after the loop (+5). `return 0` in place of `return count` shortens the
  function by a word and collapses to 151 -- the leaf-zero hypothesis is dead.

- **Tail-call node selection, 4 words.** The target never writes the node
  parameter back; it computes the chosen child into a scratch register and
  passes it. The `node = node->sideN` form keeps it in the parameter's own
  saved register. A conditional expression removes the write-back and is
  structurally right, but colours the temp `v0` where the target has `a0`, so
  the site stays at 4. So this is now a colour question, not a structure one:
  the target spends one more colour before this temp than the candidate does.

- **Recursive OR block, 14 words (both mirror copies).** Two register renames,
  one adjacent load/store swap, and the bitwise-or's operand order. That order
  is **not** source-reachable: all four combinations of the two or-expressions
  are byte-identical, so uopt normalises the operand order and the difference
  has to come from the carrier the second operand is loaded into (the target
  uses `a0`, the candidate a temp). The single-element array holding the first
  result is load-bearing -- a plain scalar loses a word and collapses to 99.

- **Recursive short-circuit block, 14 words (both mirror copies).** The target
  computes `!= 0` into a temp with `sltu`, branches on the temp, and returns it
  from the branch delay slot. The candidate emits an inverted test plus a
  separate branch and a literal 1. Naming the boolean does produce the `sltu`
  -- verified in the object -- but every carrier tried inverts the branch
  polarity and costs a word: a fresh local (107, and the frame grows), and
  reusing `leafResult`, `remaining`, `count`, or the result array (78-79). A
  doubled `!= 0` is 80. The short-circuit-or spelling produces the `sltu` and is a word
  shorter overall but branches to a shared tail instead of returning from the
  delay slot (110-120).

A 48-cell cross of the comparison order, three node-selection forms, all four
or-operand orders, and both loop-tail orders floors at 39 with no interaction:
these knobs are independent and flat. The next lever should be a colour
argument, not another spelling sweep -- three of the four sites reduce to "the
target spends `a0` where the candidate spends a pool temp or a saved register".

<!-- plateau-handoff:overlay2QueryNode:end -->
