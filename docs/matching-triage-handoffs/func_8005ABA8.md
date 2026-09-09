<!-- plateau-handoff:func_8005ABA8:start -->
### `func_8005ABA8` plateau handoff

- source: `src/main/models_5B300.c`
- score: 47 differing words
- frame: frameless
- relocations: 0
- first mismatch: +0x8
- summary: Size closed at 111/111: the second frame carrier is a field re-read, not a cached local. Residual is a pairwise a1/a2 exchange and its FP rotation.

#### Owned evidence and result

The size question is closed. The earlier candidate was one instruction short
because the source cached `temp_v0->frame` in a single local: the target loads
that field into a scratch register and copies it into a second, lasting
carrier, and one local cannot produce the copy. Reading the field again into a
second declared pointer, after the null test, supplies it -- the two-carrier
field re-read in `docs/ido-learnings.md`. The candidate is now 111 words
against the target's 111, frameless with no relocations on either side, and
its instruction census matches.

Reversing the two blend statements (`temp_f0` before `temp_f2`) then corrected
the `div.s`/`sub.s` operand order, moving the residual from 49 to 47 positional
words. That is the second confirmed lever: the web created first takes the
first FP colour.

What did not move it, measured in this pass:

- Spelling the later use as `temp_v0->frame` at the use site, with no second
  declared local, adds two words rather than a copy: uopt commons the read but
  the value stays in one carrier. The second *declaration* is what splits it.
- Reversing which pointer takes the load and which takes the re-read, swapping
  the two declarations, and both together, are byte-identical to the current
  form (four probes, all 47).

Remaining residual: `a1` and `a2` are exchanged on the two frame carriers, and
the FP pool rotates in the same pattern (`f0`/`f2`, `f6`/`f8`, `f16`/`f18`).
Every difference is a register name; the opcodes, the order and the branch
targets agree except the loop-entry test, where the target's `beqz`/`nop` is
the candidate's `beqzl` with the hoisted `arg1 * arg2` in the slot. That
multiply is the tell: as1 fills the annulled slot from the join block, so the
question is what the join block starts with, not how the test is written.

<!-- plateau-handoff:func_8005ABA8:end -->
