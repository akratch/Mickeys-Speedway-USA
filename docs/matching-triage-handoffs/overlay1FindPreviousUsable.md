<!-- plateau-handoff:overlay1FindPreviousUsable:start -->
### `overlay1FindPreviousUsable` plateau handoff

- source: `src/overlays/o001/overlay_001.c`
- score: 12 differing words
- frame: frameless
- relocations: 4
- first mismatch: +0x4
- summary: structure-mismatch; lever none-known. Raw/masked 14/12, masked first +0x14; attempts 9-13 stalled. Next: fidelity-pinned CFE copy/coalescing trace.


#### c2-o001: the residual is one copy-propagation decision, and it drives all twelve words

Re-measured at 12 masked words, 14 raw, frameless, exact extent. The whole
residual is a single mechanism with three visible consequences, so it is one
question and not twelve.

The target keeps two copies of the entry count that the candidate propagates
away. It copies the count into the loop counter and into the wrap bound, tests
the counter copy rather than the loaded value, decrements the copy in place,
and recomputes the wrap bound freshly from the loaded value at the wrap site,
leaving the wrap copy dead. The candidate propagates the counter copy back to
the loaded value, which makes the counter's pre-decrement and the wrap bound
the same expression, so uopt commons them into one loop-invariant temp,
shares it between the guard and the wrap site, and eliminates the wrap copy
entirely. Three words go to the copies, one to the shared temp at the wrap
site, one to the guard test, one to the branch form, and the rest is the
register renaming that follows.

The branch form is downstream of the same fact and is not a separate lever.
The target's range guard is a plain branch whose delay slot takes the first
instruction of the fall-through -- one of the copies -- and it branches to the
shared failure block. With the copies propagated away, the candidate's
fall-through begins with a branch, nothing can fill the slot from below, and
ugen emits a branch-likely with the failure block's first load duplicated into
the delay slot. Restore the copies and the branch form follows.

Twenty-nine source forms were measured this lane and every one is byte-flat at
12 or worse. Do not repeat: the wrap bound spelled from the count variable, an
inlined `count - 1` bound, and a pre-subtracted bound variable (17 each); the
counter decrement written as `remaining = count - 1` (19) and as `-= 1`,
`--remaining` and a trailing form (flat); the guard written as `> 0`, as a
bare truth test, and with the constant on the left (flat); the two copy
assignments in both orders (flat); the wrap bound assigned after the guard,
inside the loop, and split across two statements (flat or 17); the wrap site
written as a compound add, which is correct in this ring but loses an
instruction (34); unsigned and explicitly cast variants of both locals (flat);
and the whole family of reads straight from the import in place of the local,
which does emit the copies but leaves the loaded value dead at the wrap site
and cascades the pointer colours instead (17 to 38 across six forms).

The loop shape is not the lever either. Every top-tested rewrite -- `while
(remaining--)`, `while (remaining-- != 0)`, and the `for` form, with the wrap
bound spelled three ways -- loses an instruction and scores 34, which confirms
the retained guard plus do-while as the target's own shape.

Next lever: whatever makes uopt decline to propagate a copy of a loaded value
into a guard test that immediately follows it. That is the same question
`overlay1ResolvePathPoint` asks at one word, which makes it worth answering
once rather than twice.


#### c2-o001 diagnostic: the inert-probe family is completely inert here, which classifies the residual

The same ADR 0017 inert-read family that moves `overlay1AdvanceGauge` 18 words
and `overlay1ConsumeNearbyPending` 7 was run against this function: 220
placements over eleven slots and 20 expressions, spanning every local, both
imports and the loop-invariant differences. Not one reaches below 12, and the
byte-flat ones are flat rather than merely equal-scoring.

That is a classification, not just another negative. The family is a
pool/ring population dial, so its total inertness here says the residual is
not an allocation-population fact at all. It is the copy-propagation decision
the section above names, and it will not yield to anything that only changes
how many webs compete. The same test run on `overlay1ResolvePathPoint`, whose
one word is the same class, is equally inert over 140 placements.
<!-- plateau-handoff:overlay1FindPreviousUsable:end -->
