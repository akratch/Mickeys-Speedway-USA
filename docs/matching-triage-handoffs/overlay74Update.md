<!-- plateau-handoff:overlay74Update:start -->
### `overlay74Update` plateau handoff

- source: `src/overlays/o074/overlay74Update.c`
- score: 99/100 words
- frame: 0x60
- relocations: 8
- first mismatch: +0x124
- summary: Exact frame and all eight runtime identities; only the commutative OR operand order differs after bounded source forms.

- retained result: target and candidate are exactly 400 bytes / 100 words with
  `0x60` frames. The candidate has 99/100 exact positional words; the sole raw
  and relocation-masked difference is the operand encoding of the commutative
  OR at `+0x124`. Pool, temporary, FP-pool, and FP-temporary sequences match.
- historical linked proof: the function-only promotion trial reported `text-differs` with
  one in-range word and zero outside differences. This proves the linked call
  and global relocation effects are exact while independently confirming the
  single owned-word residual.
- relocation surface: fresh full-TU preflight now resolves all eight runtime
  relocation offsets, types, and identities. The four resident calls use their
  authenticated resident names in C; the pure-alias manifest renames those
  actual undefined compiler symbols back to the existing overlay proxies.
  An independently reproduced raw object and its configured alias pass have
  identical instruction bytes, and both prove the complete eight-record
  runtime surface. The alias pass preserves the stored-call values, rather
  than treating those values as identity evidence.
- ABI repair: Mickey's resident query consumes its third argument as f32 bits,
  corroborated by the matched `rangetriggerControl` caller. The source object's
  z field and query parameter are therefore both f32; changing only the
  declaration would incorrectly convert the old integer carrier. The paired
  repair and four call-name changes preserve every physical source line and
  all 400 instruction bytes of the preceding candidate. Fresh raw and masked
  comparisons still have exactly one difference at `+0x124`; no expression,
  flag, or OR-operand variation was attempted, and the guard remains active.
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
  The current body already has a completed modern flat sweep; tool-identity
  changes alone do not justify repeating it. This ABI/binding repair consumes
  its narrowly pinned authorization and does not authorize another attempt.
<!-- plateau-handoff:overlay74Update:end -->
