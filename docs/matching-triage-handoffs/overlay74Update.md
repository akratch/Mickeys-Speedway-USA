<!-- plateau-handoff:overlay74Update:start -->
### `overlay74Update` plateau handoff

- source: `src/overlays/o074/overlay74Update.c`
- score: 99/100 words
- frame: 0x60
- relocations: 8
- first mismatch: +0x124
- summary: All register lanes match; CFE spelling owns the OR residual. Next lever: a new C spelling reversing OR without operand swap or named carrier.

- retained result: target and candidate are exactly 400 bytes / 100 words with a
  `0x60` frame. The candidate has 99/100 exact positional words; the sole raw
  and relocation-masked difference is the commutative OR at `+0x124`. Pool,
  temporary, FP-pool, and FP-temporary sequences match.
- linked proof: the function-only promotion trial reported one in-range word
  and zero outside differences, confirming the single owned-word residual.
- relocation surface: all eight runtime relocation offsets, types, and
  identities are exact under the configured alias preflight.
- ABI and carrier repairs recover the target frame, stack homes, and complete
  register allocation. Direct operand reversal and named-carrier forms were
  already flat or regressive in the retained bounded search.
- fresh diagnostic: `register-permutation` routing is not applicable to the
  current candidate because every register lane is identical; ownership is
  CFE spelling and the remaining source-reachable difference is commutative.
- stopping evidence: no new attempt was authorized because the committed
  handoff records direct swaps, named temporaries, forced coloring, and
  permutation as exhausted. Reopen only for a genuinely new C spelling that
  reverses the final OR without those mechanisms.
<!-- plateau-handoff:overlay74Update:end -->
