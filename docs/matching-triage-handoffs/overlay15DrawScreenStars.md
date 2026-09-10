<!-- plateau-handoff:overlay15DrawScreenStars:start -->
### `overlay15DrawScreenStars` plateau handoff

- source: `src/overlays/o015/overlay_015.c`
- score: 92/105 words
- frame: 0x58
- relocations: 10
- first mismatch: +0x18
- summary: Ten source-authentic command, fade-lifetime, and packed-expression forms found no gain; entry scheduling and local-data identity proof remain.
- assignment base: `3ccc02a1`
- owned range: overlay 15 `+0x500..+0x6A4`, 420 bytes / 105 words
- retained evidence: configured full-TU C is 92/105 positional words exact, with 13 relocation-masked differences, 14 raw differences, and the exact target `0x58` frame
- relocation proof: fallback-static has 10 candidate and 10 target records, seven offset/type alignments, zero canonically resolved candidate identities, and zero exact identity alignments; the surface is incomplete, so no object-exact or promoted-linked claim is available
- diagnosis: all 22 integer pool lanes and all five FP pool lanes agree; the earliest causal residual is the initial setup/count/star carrier schedule at `+0x18`, followed by the fade load crossing the zero-count branch and two independent packed display-list OR schedules
- attempts: ten bounded source-authentic forms covered removal and splitting of the one-use initial-command alias, loop-scoped and guarded fade loads, a guarded do-loop, direct initialized-data ownership, rectangle association, packed-word temporaries, and delayed command increment; every form was neutral or regressed geometry, frame, or positional score
- prior search limit: do not repeat the semantically invalid inverse-depth/fade alias from the old permuter result; it changed the shade calculation and was correctly rejected
- JFG insertion use: JFG overlay 5 `starfieldDrawSP` at `+0x50C` is the 0.333 masked-skeleton lead within the strongly corroborated module correspondence. Mickey's retained C supplies a concrete reconstruction scaffold for JFG's depth clip (`8.0f..300.0f`), perspective projection about screen center, viewport rejection, depth-based grayscale fade, paired fill-rectangle command emission, display-list cursor update, and final list handoff phases.
- JFG limits: JFG's body is assembly-only and 508 bytes versus Mickey's 420 bytes; Mickey's local-data identities are not yet authenticated and its shorter control path is not evidence for JFG-specific setup, data layout, relocation names, or extra behavior. Use the scaffold at JFG's existing pragma as phase-level insertion guidance, then prove every JFG byte and relocation independently.
- next action: reopen only with a natural entry-lifetime mechanism or canonical local-data ownership that moves the first setup/fade schedule while preserving the exact frame and geometry; treat packed-command association as a later independent lever

#### Re-open under law L90 (2026-09-10, lane/c3-reopen2)

- **L90 does not apply to this loop.** The exit test is a countdown against
  zero, not an inequality against a bound, so there is no hoistable K and
  nothing for uopt's induction rewrite to do. Measured rather than asserted:
  an explicit guarded do-while scores 21, the same guard with the fade load
  moved inside it scores 22, and a pre-decrement do-while costs 12 bytes and
  scores 93. The retained countdown is the optimum at 13.
- lever set the closure actually searched: ten command, fade-lifetime and
  packed-expression forms, including removal and splitting of the initial
  command alias, loop-scoped and guarded fade loads, a guarded do-loop,
  direct initialized-data ownership, rectangle association, packed-word
  temporaries, and a delayed command increment.
- re-tested on the two axes the closure named as still open. Fade placement:
  nine fresh positions, seven statement positions and two declaration
  positions. Every earlier position scores 16 to 19 and moving the read inside
  the loop costs +4 bytes, because the display-list stores block the hoist.
  The retained position is the unique optimum. Command association: five
  fresh forms -- store order within each command, store order between the two
  commands, the constant moved to the tail of the or-chain, and folding both
  cursor increments into one -- all flat at 13 or worse.
- one change adopted. The star pointer is now read through this TU's
  established pointer-view idiom, the same one `overlay15MoveStars` already
  uses for the same global, which is also what the target's own stored addend
  at that site says. Raw differences fall from 14 to 13 and the artifact
  column from 1 to 0; the masked residual is unchanged at 13 and the owner
  geometry and frame are unchanged. This is a relocation-addend fidelity
  correction, not a byte gain, and it is recorded as such.
- the deciding variable, named: which basic block owns the fade load. The
  target's loop preheader owns it, below the zero-trip guard, where the
  loop-invariant constants also land; the candidate's entry block owns it,
  above the guard. That is a uopt block-ownership decision, and no statement
  placement reaches it because the guard is synthesised at the loop, not
  written. The rest is two independent packed-command association schedules
  and one temp-ring slot.
- **verdict: the closure was correct and remains correct under L90**, with one
  raw-word fidelity correction adopted.
<!-- plateau-handoff:overlay15DrawScreenStars:end -->
