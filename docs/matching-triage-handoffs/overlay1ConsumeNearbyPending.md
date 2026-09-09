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
<!-- plateau-handoff:overlay1ConsumeNearbyPending:end -->
