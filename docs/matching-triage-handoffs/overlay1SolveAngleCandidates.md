<!-- plateau-handoff:overlay1SolveAngleCandidates:start -->
### `overlay1SolveAngleCandidates` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 47/139 words
- frame: 0xA8
- relocations: 4
- first mismatch: +0x50
- summary: Two relocation offsets/types align; identities unresolved. -O2 -g3 gives 139 words/86 diffs but changes the TU prologue. Need a natural root-count preheader.

#### tu2-o1tail: the two missing instructions are a loop guard IDO did not fold

Re-measured at the assigned base: 92 masked words, 137 instructions against
the target's 139, 0xA8 frame exact, prefix exact to row 20, and every temp,
shared and fp-temp lane identical. The whole size deficit is one pair at
+0xBC: the target materialises the root count into a register and branches on
it (`li v1,2` then `beqz v1`) before entering the rotated loop, even though
uopt has already propagated the constant 2 into the test. The candidate folds
that guard away, because `sign = solutionCount + 2` with solutionCount
provably zero is a compile-time constant to the front end.

So the target's loop is a pre-tested `while` whose initial value reached the
test through copy propagation *after* branch folding had run, which is why the
constant survives in a register with a live test on it. The committed record
already reports nine loop and count forms tried, so per ADR 0018 this lane made
zero further attempts on that mechanism: the evidence rules out the form of
lever available from source. The dead `overlay1LoopControlCarrier` helper below
the function is a leftover from that search and is not called.

Remaining, after the guard: the fp-pool lane diverges at slot 13, where the
target keeps the discriminant root in f0 across the branch and the candidate
routes it through f12.
<!-- plateau-handoff:overlay1SolveAngleCandidates:end -->
