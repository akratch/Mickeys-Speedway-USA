<!-- plateau-handoff:func_8005ABA8:start -->
### `func_8005ABA8` plateau handoff

- source: `src/main/models_5B300.c`
- score: 2 differing words
- frame: frameless
- relocations: 0
- first mismatch: +0x3C
- summary: 111/111 words, every register and branch target exact; the last two words are one as1 delay-slot decision at +0x3C.

#### Owned evidence and result, 2026-09-09

47 differing words to 2, at 111/111 words, frameless, zero relocations, with
every register name and every branch target now agreeing. Four source
artefacts closed 45 of them and none was an allocator question:

- `temp_f0_2` cached `instance->frameValue` for two tests that no store
  separates. The cache costs a `mov.s`; the target reads the field at both
  sites and lets uopt common the load, and the `nop` the target shows at that
  hazard slot is exactly what the copy was filling. Worth 27 words.
- The two blend stores were written sub-then-div. The target allocates the
  div's FP temporary first, so the source computes `blendStart` before
  `blendEnd`; both read only locals, so the order is free. Worth 4 words.
- The null test spelled through a cached `temp_a1` gave the loaded pointer a
  copy and exchanged `a1`/`a2` across both frame carriers and their two later
  uses. Testing `temp_v0->frame == NULL` directly lets the load keep `a1` and
  the surviving carrier take the copy into `a2`. Worth 7 words, and it is the
  opposite of the lever the previous handoff filed: the second declared
  pointer was the *cause* of the exchange once the size question was closed.
- `var_v1 = 1` written before the inner `if` of each arm rather than once
  after it moved no instruction at all, but made as1 duplicate the join's
  `move v0,v1` into two annulled delay slots the target leaves as `nop`.
  Worth 5 words, and it is the proof that this class is source-reachable.

`temp_f2_2` was an m2c-only second name; one carrier serves both `blendEnd`
reads. Removing it, and the now-dead `temp_a1`, is byte-inert.

Remaining: one as1 delay-slot decision at `+0x3C`. The target branches `beqz`
with a `nop` to a block whose first scheduled instruction is
`mul.s $f18,$f14,$f12`; as1 turns the same branch into `beqzl`, duplicates
that multiply into the annulled slot and retargets past it, which leaves the
original copy unreachable and both sides at 111 words. Twenty-eight further
shapes of the else-block head, the transition test, the declaration list,
the comparison spellings and the placement of the product are flat at 2.
Resume on the ugen text as1 consumes, not on the schedule: the `var_v1`
result above proves a statement move that changes no emitted instruction can
still flip this decision.

Tooling: the permuter's isolated scratch for this TU compiles the function at
112 words against the real object's 111, so its base score of 400 is a false
reading and nothing measured there transfers. `tools/permute.sh` on
`models_5B300.c` should not be trusted until that is fixed.

<!-- plateau-handoff:func_8005ABA8:end -->
