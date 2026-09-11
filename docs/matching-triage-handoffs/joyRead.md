<!-- plateau-handoff:joyRead:start -->
### `joyRead` plateau handoff

- source: `src/main/joy.c`
- score: 47/159 words
- frame: 0x38
- relocations: 55
- first mismatch: +0x2C
- summary: Declaring i before unusedMsg made the frame ladder exact, 48 to 47; the rest is the two loops' register webs
#### 2026-09-12 (lane `lane/p7-res2`): the frame ladder is exact; the residual is two loop endpoint symbols

48 to 47, and the stack is now byte-identical in shape. **Declaring `s32 i;`
before `OSMesg unusedMsg;` moves the message local's home down one slot and
makes `frame_census` report the same four-rung ladder on both sides**
(+0x3C +0x38 +0x30 +0x14, frame 0x38). Before the swap the message local sat
one word higher and the aligner carried two immediate-only words; it now
carries one, and byte-exact went 113 to 114.

Eight declaration forms were measured for that slot and only this one pays:
the message local as a two- or three-element array with the call taking the
array name (64 each), a leading `void *` pad (64), a leading `f32` pad (64), a
trailing pad (64), and a one-element array (48, byte-flat). So it is the
*order* of the two declarations, not an array length: L112's free parameter is
not the lever here and L99's declaration ladder is.

**The remaining 47 is 40 register-naming, 1 immediate, 5 really-different, and
the cause is named.** Both of this function's copy loops are reassociated into
pointer form, and in each the candidate and the target disagree only about
which of the two pool registers the cursor takes against the endpoint. The
decisive observation is in the second loop, over the connected-pad flags:

- the target's endpoint register is *also* both outer arguments of the
  save-action call that follows, so that call sets up **no arguments at all** —
  the endpoint, the loop's TRUE constant and the counter pointer are already in
  a0, a1 and a2 when the call is reached;
- the candidate materialises the endpoint as the flag array's own base plus its
  length, which is a different address constant from the counter's address, so
  the two cannot be one web, and the call pays two register copies. The
  candidate spends those two words and saves one on the loop test (an equality
  test against an exactly-reachable bound where the target tests `<`), which is
  why the sizes agree at 159 while the aligner reports one surplus and one
  missing instruction.

That makes the decision variable **the identity of each loop's endpoint address
constant**, not any statement ordering: the flag array's end and the counter's
address are the same address, and the target's compiler knew it because the two
objects were adjacent *in its own translation unit*. In this tree the pad and
button arrays are `extern`, so IDO must compute base-plus-length. Twenty-five
loop spellings — counted `for`, `do`/`while`, `while`, `!=` against `<`, and an
L97 `if (1)` region around either loop, crossed both ways — are one object at
47, and the `!=` forms are 49. **No source form inside this function reaches
it**; the lever is which translation unit owns
`D_800CF370`/`D_800CF388`/`D_800CF3B8`/`D_800CF3BC`, and moving that ownership
is a tree-level change, not a lane edit.

Axes covered this pass: eight message-local declaration/padding forms; twenty-
five loop-spelling forms including both L97 region openers. Not covered: giving
this TU the definitions of the four pad/flag arrays so IDO can see the
adjacency, which is the named next lever and needs a coordinator decision
because it moves `.bss` ownership.
<!-- plateau-handoff:joyRead:end -->
