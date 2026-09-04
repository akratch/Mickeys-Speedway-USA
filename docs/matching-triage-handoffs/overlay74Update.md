<!-- plateau-handoff:overlay74Update:start -->
### `overlay74Update` plateau handoff

- source: `src/overlays/o074/overlay74Update.c`
- score: 99/100 words
- frame: 0x60
- relocations: 8
- first mismatch: +0x124
- summary: Exact frame and register webs; only the commutative OR operand order differs after bounded source forms.

- retained result: target and candidate are exactly 400 bytes / 100 words with
  `0x60` frames. The candidate has 99/100 exact positional words; the sole raw
  and relocation-masked difference is the operand encoding of the commutative
  OR at `+0x124`. Pool, temporary, FP-pool, and FP-temporary sequences match.
- linked proof: the function-only promotion trial reports `text-differs` with
  one in-range word and zero outside differences. This proves the linked call
  and global relocation effects are exact while independently confirming the
  single owned-word residual.
- relocation surface: all eight runtime relocation offsets and types align.
  Four `gOverlay74Flags` records resolve statically; the four resident calls
  remain static-name unresolved in the fallback object, while Mickey's
  resident call surfaces authenticate their prototypes and the linked trial
  authenticates their final values.
- source facts recovered: the query result state has signed strength while the
  source object's state strength is unsigned, and the query return is tested
  for nonzero. Reusing dead incoming carriers for the hit-state pointer, count,
  and flag bits, then reusing the dead hit pointer for the loop mask, recovers
  the target frame, stack homes, and complete register allocation without the
  previously prohibited empty guards.
- bounded attempts: direct producers reached the correct frame but lost five
  instructions until the unsigned source-state conversion was restored; eight-
  and ten-entry result arrays produced `0x58` and `0x60` frames but wrong array
  homes; the final carrier map reduced the retained 39-word residual to one.
  Direct outer-OR reversal is byte-flat, while named carrier forms regress to
  29 or 38 differing words and can enlarge the frame. Artifacts are retained
  under ignored `build/wb/overlay74Update-attempts/`.
- next action: reopen only with a new C-level mechanism that reverses the final
  OR operands without naming another carrier. Do not repeat direct operand
  swaps, named temporaries, flag sweeps, forced coloring, or permutation.
<!-- plateau-handoff:overlay74Update:end -->
