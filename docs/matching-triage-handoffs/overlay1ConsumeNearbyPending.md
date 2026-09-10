<!-- plateau-handoff:overlay1ConsumeNearbyPending:start -->
### `overlay1ConsumeNearbyPending` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 31 differing words
- frame: 0x48
- relocations: 1
- first mismatch: +0x40
- summary: a volatile counter with if (count--) and no inner object copy makes the opcode schedule exact (structure-mismatch to allocation-mismatch); the 31 register words are six counter reads uopt webs and the target spends as ring temps

#### tu2-o1tail: the countdown form is settled and is not the residual

Re-measured at the assigned base: 31 masked words, 69 of 69 instructions,
0x48 frame exact, prefix exact to row 14. The target computes count-1 before
the guard branch and stores it in the branch's delay slot, then reloads the
counter from its stack slot inside the loop, which reads as `if (count--)`.
It is not: `if (count--)` and `if (count-- != 0)` and a pre-tested
`while (count--)` each add one instruction (70) and take the residual to 51.
The current `if (count != 0) { count--; do ... while (count--); }` is the only
form of the four that holds 69 instructions.

The real residual is the block starting at +0x60, where the target reloads the
counter from sp+60 and recomputes the element address, while the candidate
keeps the object pointer live and reuses it. Next lever is that reload, not the
loop shape.


#### c2-o001: declaration order is not the pool-priority lever

The previous record's next lever was "find what makes uopt reserve those two
pool colours for `other` and `otherState`". Declaration order is not it, and
that is now measured rather than assumed: all 120 permutations of the five
locals were compiled from the retained base. None improved on 31; the retained
order and one neighbour are the joint best, three permutations reach 32, and
the rest run from 33 to 36. Do not repeat the declaration lattice.

Worth recording alongside it: `overlay1AdvanceGauge` in the middle TU carries
the identical shape at 25 words -- the target spending its pool colours on the
loop-body pointers and leaving the counter and a loop-invariant constant to
the ring, the candidate spending one extra pool colour and sliding every ring
value one position. Two functions, two TUs, one mechanism. Whatever buys or
sells that pool web will close both.


#### c2-o001 diagnostic: seven of the 31 words are pool population, and 24 are not

The previous section's next lever asked what makes uopt reserve the two low
pool colours for the loop-body pointers. An ADR 0017 inert-diagnostic sweep
answers half of it and bounds the other half. Nothing here is adopted.

198 single zero-footprint reads across eleven placement slots were measured.
The best is a state-pointer read placed between the list getter and the
counter guard, or the equivalent pair of loop-body pointer reads placed before
the radius computation: 31 words to 24, at 69 of 69 instructions and delta 0.
That is the pool web the earlier record predicted, and it is worth seven
words. A four-round beam over 735 further combinations finds nothing below 24,
so the remaining 24 words are a different mechanism and not more of the same
population effect.

This matters for `overlay1AdvanceGauge` in the middle TU as well, which the
previous section pairs with this one. There the identical diagnostic family is
worth eighteen of twenty-five words and leaves seven. So the two functions do
share the pool-population mechanism, but they do not share the residual after
it: this one keeps three quarters of its residual and that one keeps under a
third. Treat them as one lever and two different remaining causes.
<!-- plateau-handoff:overlay1ConsumeNearbyPending:end -->
