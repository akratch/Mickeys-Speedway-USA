<!-- plateau-handoff:overlay1AdvanceGauge:start -->
### `overlay1AdvanceGauge` plateau handoff

- source: `src/overlays/o001/overlay_001_middle.c`
- score: 25 differing words
- frame: 0x40
- relocations: 3
- first mismatch: +0x18
- summary: register-only at 25 words; the volatile pad moved last closes the stack home, and one extra pool web is the whole residual.


#### c2-o001 diagnostic: 18 of the 25 words are pool/ring population, and that is now measured

Under ADR 0017 an isolated inert-diagnostic sweep was run against the retained
25-word candidate to size the mechanism rather than to improve the score. The
diagnostics are NOT adopted -- the cleanup queue's policy and ADR 0017 both
forbid adopting an inert form into a nonexact body -- but the number they
produce is the useful result, because it says how much of the residual is
allocation population and how much is something else.

Five zero-footprint reads placed in the loop take the candidate from 25 words
to 7 at 42 of 42 instructions, an exact 0x40 frame and delta 0 throughout:
a global read after the object dereference, a second one after the state
dereference, a count read before the loop latch, an index read between the
latch and the cursor decrement, and a loop-value read at the top of the
guarded block. Every one of the five is load-bearing -- all 31 subsets were
measured and dropping any single probe costs between 2 and 18 words -- so this
is one accumulating population effect, not five independent lucky hits.

What the diagnostic buys, in order: the three long-lived constants (the
multiplier and the two clamp bounds) move off the pool and onto the ring and
the import's address web lands on the target's colour (25 to 19), then the
loop-index and object colours align (19 to 13, then 18), then the clamp
arithmetic and the state pointer (13 to 9 to 7).

The 7-word floor is a clean three-value class swap and nothing else: the
target gives the reloaded count a pool colour and takes the scaled-index and
the import's value from the ring, and the candidate takes the count from the
ring and spends the pool colour on the import's value instead. That floor is
firm against everything tried around it -- 375 further declaration
arrangements at the diagnostic base including a spare local at all eight
positions and the volatile pad at all seven; 268 further probe additions and
substitutions across seven placement slots and 39 expressions; every carrier
form for the count through the two existing integer locals; and 36 multi-read
count-priority forms, which is lever 9's dial and it does not turn here.

The bar for the next lane is therefore precise: find the single structural
difference that puts the reloaded count in a pool colour and the import's
value in the ring. Eighteen of the twenty-five words follow from it, and the
remaining seven are that difference itself.
<!-- plateau-handoff:overlay1AdvanceGauge:end -->
