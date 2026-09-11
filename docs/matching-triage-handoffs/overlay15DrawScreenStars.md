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
#### 2026-09-11, lane `p6-small`: the named decision variable IS reachable from source

The 2026-09-10 closure named the deciding variable exactly right -- which basic
block owns the fade load -- and then asserted that no statement placement
reaches it because the zero-trip guard is synthesised at the loop rather than
written. **That assertion is false, and the counter-example is cheap:** read the
fade scale through a `const f32 *` set before the loop and dereferenced inside
it. uopt then hoists the load as a loop invariant into the PREHEADER, below the
guard, and the entry block becomes byte-exact against the target through the
guard and its delay slot -- the first ten words of the residual close.

It does not pay yet. Measured, all size delta 0 unless stated:

- pointer view with the dereference at the loop top: 20 words (base 13).
- pointer view with the dereference inside the depth test: 33.
- pointer view with the dereference inside the viewport test, or inlined at the
  shade site: 54 and 52 -- the load is then conditional and is not hoisted.
- the same four placements without the pointer, reading the global directly:
  84 to 88, size delta +4, because the load stays in the loop.
- the read at the end of the loop body, with and without a pre-loop read, and
  with the star increment on either side of it: delta +4 or +8, never hoisted.

**What replaces the old decision variable: the preheader's hoist ORDER.** With
the fade load hoisted, the preheader holds three loop invariants -- the fade
scale and the two depth constants -- and the target emits them fade first, then
the far constant, then the near one, taking the three callee-saved float
registers descending in that order. Every form measured here emits them in the
opposite order and assigns the registers accordingly, which is what the extra
seven words are. The order is NOT the reverse of source order: swapping the two
halves of the depth test, writing both comparisons with the constant on the
left, splitting the test, and inserting an unrelated statement ahead of the
read are all byte-flat at 20 (19 points). IDO appears to hoist only invariants
that sit in the loop HEADER -- a read placed after the first branch in the body
is never hoisted, which is the same evidence from the other direction -- and
within the header the emission order is fixed by something other than
statement order.

So the residual is now two facts rather than one, and the next lane should work
the hoist order, not the block ownership. The retained body is unchanged
because 13 beats 20; the pointer-view form is a diagnostic, not an improvement.

<!-- plateau-handoff:overlay15DrawScreenStars:end -->
