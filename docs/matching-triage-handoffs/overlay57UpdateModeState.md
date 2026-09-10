<!-- plateau-handoff:overlay57UpdateModeState:start -->
### `overlay57UpdateModeState` plateau handoff

- source: `src/overlays/o057/overlay57UpdateModeState.c`
- score: 5/354 words
- frame: 0x30
- relocations: 59
- first mismatch: +0x108
- summary: 21 fell to 5 by reordering uopt's colouring, not by respelling the loop: globalcolor takes webs in descending save = totalsave/nocs (references, x10 inside a loop, over a span bucket), and the target's entries v0 / count v1 / dead-copy a0 is exactly the order entries > count > dead copy, while the candidate measured 14 < 17 < 22 because one cfe temporary served the post-decrement in BOTH arms and entries and count were separate block-scoped symbols in each; declaring entries ONCE at function scope merges its two webs (save 14 -> 21) and naming the dead copy prev per arm splits its shared web (save 22 -> 11), each a 31-word regression alone and 5 together; the 5 left are 3 words of a store uopt sinks past the early return (forcing residency lands it but re-colours 18 by raising eligible's save above the timer address web's) and 2 words of an as1 line-order tie at +0x108 that no legal statement order can reverse.
<!-- plateau-handoff:overlay57UpdateModeState:end -->
