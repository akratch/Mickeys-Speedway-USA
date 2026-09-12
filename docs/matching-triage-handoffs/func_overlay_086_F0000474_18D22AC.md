<!-- plateau-handoff:func_overlay_086_F0000474_18D22AC:start -->
### `func_overlay_086_F0000474_18D22AC` plateau handoff

- source: `src/overlays/o086/func_overlay_086_F0000474_18D22AC.c`
- score: 9 differing words
- frame: 0xA8
- relocations: 38
- first mismatch: +0xA8
- summary: L160 carrier removal does not close the 7 naming and 2 scheduling rows; the changed-source landscape has no improving equal-size force.

#### 2026-09-10, lane w8-bigclose: p1 owns all of it, and the probe route is closed

The closure above is **confirmed**, not superseded, and that is worth recording:
the corrected form of [L100] raises the possibility that p1 does not own a small
procedure at all, so the first thing this pass did was ask ([L108]). An
instrumented `uopt` -- CDX log, object byte-identical to the tree's, which is
the identity gate -- records **1218 p1 records over 66 decisions and zero p2**.
[L106]'s ascending-web-number axis does not exist on this function, and
`save = totalsave/nocs` is the only order there is. One compile retires a whole
axis.

The traced figures are the ones already recorded: the web holding a1 is
`save` 6.666667 (`nocs` 3, `totalsave` 20) and takes that colour at cost 0 as
its only zero-cost candidate; the command pointer is `save` 2.666667 (`nocs` 3,
`totalsave` 8) at cost 1. What the trace adds is that by the time the pointer is
decided the colour is not merely taken but **forbidden** -- its forbidden mask
carries the first four caller-saved colours -- so a direct force of the pointer
onto a1 is **declined** and the object comes back byte-identical. A forced-colour
experiment on this web proves nothing about the target; only the decision order
does.

**[L109]'s zero-cost probe does not reach uopt's reference count here, and that
is a new negative.** Five reference forms on the pointer -- a discarded
`(void)p;`, a bare `p;`, a self-assignment, `p += 0;`, `p = &p[0];` and an
idempotent cast round-trip -- at 1, 4, 6, 13, 14 and 20 repetitions all leave
`totalsave` at exactly 8.000000 and the score at 29, delta 0. uopt counts
references after copy propagation, so a probe that merely names a pointer never
reaches it. Only the tested form `if (p == 0) { }` reaches the count, and six of
those are 235. The pointer's live range holds no loop, so [L109]'s x10 weighting
has no depth to sit at either.

Two axes re-measured on **this** base rather than the 51-word one, which is what
[L47] asks for:

- eleven positions for the pointer's defining statement ([L106]/[L105]): 29 at
  the four latest, then 30, 32, 55, 55, 55, 55, 60 and 641 as it moves earlier;
- ten [L107] region boundaries -- `if (1) { }` and `do { } while (0)` after the
  definition, before the advance, before the first command word, around the
  definition, around the six command writes, inside the arm, two at once, and
  the in-place advance with and without one -- all exactly 29 at delta 0. The
  reassociation that turns the advance into an offset from the base carrier is
  not opened by a region here.

The next lever is unchanged and now carries a number: the command pointer needs
`totalsave` above 20 at `nocs` 3 -- thirteen more counted references -- or a span
short enough for `nocs` 1 at its present 8. Neither is reachable from any form
measured above.

#### 2026-09-11, lane p6-mid: re-measured, and the live-range route to a1 is closed by uopt's own folding

Baseline reproduces: 2648 bytes, 662 of 662 instructions, delta 0, 29 masked
words, first mismatch +0xA8. Aligner on a register-erased shape: 640 byte-exact,
14 register naming, 2 immediate-only, 8 really different, displacement tax 5.
`register_census` (bank-corrected) reads the fourteen as two clean families,
seven where the command pointer takes a2 against the target's a1 and seven where
the case-0 carrier takes a0 against the target's v0, one global mapping, one
window, no cycle.

**Worth recording because the closure does not say it: the eight "really
different" words are not a third residual.** Read off both objects, they are the
same colour. Where the pointer holds a1 the target needs no argument move at the
two calls that pass it, so it spends a delay-slot filler at one and a separate
pointer advance at the other; the candidate, holding it in a2, spends two moves
to materialise the argument and folds the re-materialisation and the advance into
one add. Three instructions either way, which is why the size delta is 0 --
and all of them go with the colour.

**The untried axis was the pointer's live range, and it does not survive
contact.** The closure had swept spellings, defining-statement positions, region
boundaries, probes and store orders; what it had not varied is how many
definitions the pointer symbol has and where they fall relative to the two calls
whose argument setup holds a1. The target re-materialises the pointer from the
already-live base carrier after the angle call and advances it separately, which
would break the live range across the arm that sets up those arguments. Six
forms measured, all against the whole 662-instruction target:

- re-assigning the pointer from the base immediately before the angle call is
  byte-identical to the base, at 29 -- uopt deletes the assignment as redundant,
  so no definition is added and the range is not broken;
- re-assigning it after that call, before the first command word, is 566 at
  delta -12, and writing the first command word through the base instead is the
  same object: the redundancy is not merely deleted, it is exploited, and three
  instructions go with it;
- re-assigning after the first command word is 623 at delta -4;
- advancing by subscript rather than by byte arithmetic on the re-assigned
  pointer is 566 at delta -12;
- passing the base address expression inline at either or both of the two calls
  that take the pointer, so that the pointer symbol is not referenced there at
  all, is byte-identical at 29 in all three combinations;
- moving the six command words to positive offsets from the base and advancing
  last is 31 at delta 0 -- naming falls 14 to 11 and structure rises 8 to 12, so
  it does reach the colour, and it pays more for it than it wins.

So the pointer's definition count is not settable from source here: any second
definition of the same value is either folded away or folded into the advance.
The requirement from the previous pass is unchanged -- the pointer web needs
`totalsave` above 20 at `nocs` 3 or a span short enough for `nocs` 1 -- and the
live-range route to it is now closed too.

#### 2026-09-12, lane p7-ovl3: the stated requirement is the wrong variable, and the arity of one call is the right one

Baseline reproduces exactly: 2,648 bytes, delta 0, 29 masked, aligned 640
byte-exact, 14 register naming, 2 immediate-only, 8 structural. The
instrumented uopt gives the command pointer as web 40 (save 2.666667, nocs 3,
totalsave 8) and the a1 holder as web 274 (save 6.666667), reproducing the
recorded figures, and its object is byte-identical to the tree's.

**The closure above asks for a save the pointer can never spend.** Three
records, none of which fits "a1 is held by a web that outranks it":

- web 40's `p1cost` list **starts at colour 5**. v0, v1, a0 and a1 are absent
  from it, not outbid in it. A save ratio cannot buy a colour that is never
  priced, so "totalsave above 20 at nocs 3, or a span short enough for nocs 1"
  would not deliver a1 even if it were met.
- web 40's interference list contains **no neighbour assigned colour 4**. The
  forbidden bit for a1 is set with nothing holding a1.
- forcing web 274 off a1 onto c5, c6 and c7 leaves web 40 on a2 in all three
  runs (211, 211 and 344 words), and a direct `CDX_FORCE=p1:w40=c4` comes back
  byte-identical, which is the declined-at-decision-time case and proves
  nothing on its own. Read with the three forced runs, it does: freeing the
  incumbent does not open the colour.

**What opens it is the arity of `ext_o0_2d98` inside the pointer's live
range.** Compiling `ext_o0_2d98(temp_a0)` in place of
`ext_o0_2d98(temp_a0, var_a1)` at both sites clears exactly bit 27 of web 40's
forbidden mask, `0x7803e000` becomes `0x7003e000`, and globalcolor reports
`bestcolor=4 bestreg=a1` for the same web at the same save. The mechanism
generalises: a web live across a call is denied the argument registers that
call loads. These calls load a0 and a1, which is why a2 and a3 stay priced
here, and the o046 residual measured the same day is the same shape with
calls that load a0 to a3 and a cost list that starts at colour 7.

**The one-argument form is not a match, and its defect is now a single named
word.** It is four bytes short: aligned 593 byte-exact, 10 naming, 6
immediate-only, 54 structural, and every naming row in that region reads a1 on
both sides. The target emits `move a1,s5` then `addiu a1,a1,12` where the
candidate folds both into `addiu a1,s5,12`. Seven advance spellings on top of
the one-argument form are identical at delta -4, so this is not an
advance-spelling question.

Worth stating because it is what makes the hypothesis admissible: `ext_o0_2d98`
is declared here with **no prototype**, and the third call to it in this same
function already passes one argument. The ROM cannot distinguish a
two-argument call whose a1 already holds the pointer from a one-argument call,
so the two-argument reconstruction at these two sites is an m2c guess rather
than evidence.

Axes this pass covered, all flat at 29 or worse: nine call-site spellings of
the two-argument form (an inline base expression at both sites, `&var_a1[0]`, a
`(u32)` cast round trip, `(void *)`, `(u8 *)`, a second local carrying the same
value, a third argument, swapped arguments), and seven advance spellings on the
one-argument form. uopt folds every call-site spelling back onto the same web
and the forbidden mask does not move.

Decision variable for the next lane: with a1 available to the pointer web, what
source form emits the target's separate `move a1,s5` at +0x17C instead of the
folded `addiu a1,s5,12`. The record that blocks it is the four-byte deficit,
not a colour.

#### 2026-09-12, lane p8-arity: the arity lever lands, and the four-byte deficit is a split advance

Baseline reproduced exactly before anything was changed: 2,648 bytes, delta 0,
29 masked, aligned 640 byte-exact / 14 naming / 2 immediate / 8 structural, and
the instrumented toolchain's `.text` byte-identical to the tree's stock object.
Web 40's `p1cost` list starts at colour 5 (a2), with a3, t0 to t5 and an unnamed
c13 at cost 1.0 and s5 to s8 at 33.0; colours 1 to 4 are absent from the list,
exactly as recorded.

**Which spanned call denies a1, measured one call at a time.** The function has
two `ext_o0_2d98(temp_a0, var_a1)` sites inside the pointer's range, one in the
state == 2 arm and one in the `0x24 == 0` else arm.

- both dropped to one argument: forbidden mask 0x7803e000 -> 0x7003e000,
  `bestcolor=4 bestreg=a1` at the same save 2.666667;
- the else-arm site alone dropped: same mask, same colour;
- the case-2 site alone dropped: mask unchanged at 0x7803e000, pointer still on
  a2 -- but the argument move vanishes from the delay slot at +0x164 and the
  object goes to 28 masked at delta 0.

So the denial is not the union of the spanned calls' argument loads; **one of
the two calls carries it and the other does not**, and the arity of that one is
the lever. That refines L142 and is worth carrying to the next function.

**The four-byte deficit is a fold, and two in-place advances stop it.** uopt
folds the pointer's rematerialisation into its advance, emitting
`addiu a1,s5,12` where the target emits `move a1,s5` then `addiu a1,a1,12`.
Writing the advance as two successive in-place steps leaves the copy standing
and combines only the constants: `*var_a1++ = 0xC;` followed by `var_a1 += 5;`
reproduces both target instructions at their exact offsets, delta returns to 0
and the score falls to 15. Every two-step spelling measured is byte-identical
(`+= 3; += 3`, `++; += 5`, `+= 1; += 5`, `+= 2; += 4`, `+= 5; += 1`,
`+= 5; ++`, `&var_a1[1]` then `&var_a1[5]`, and the two byte-arithmetic forms),
so the adopted spelling is chosen for being the natural display-list idiom
rather than for its bytes. A single advance is one instruction in all nine
spellings of it.

**The five payload stores are then a physical-line question.** All 120
permutations were compiled. The two that reach 9 both put the `-2` spA0 store
last; `-0xA, -8, -6, -4, -2` is the memory order and is the adopted one. The
inherited order scores 15 and the worst permutation 33.

Adopted state: 9 masked, delta 0, frame 0xA8 unmoved, aligned 653 byte-exact /
7 naming / 0 immediate / 2 structural, no insertions or deletions, first
structural difference +0xA8 and first naming difference +0x330.

**Measured flat on the one-argument base, and none of it is worth repeating:**
41 forms, every one returning a byte-identical object -- four advance spellings,
four store spellings, seven L97 region placements (`if (1) { }` and
`do { } while (0)` after the definition, inside the arm, at the end of the arm,
before and after the angle call, after the store) plus two region wrappings of
the definition and of the store-and-advance pair, definition positions 217
through 222, an extra reaching definition in the then arm, in the else arm and
in both, and an integer-carrier round trip through `temp_a0`. Statement-order
moves of the `ext_o0_2a470` call are 23 to 193 and all worse.

`CDX_FORCE=p1:w40=s` is **accepted** once `CDX_PROC=0` is also set -- without
it the force is silently ignored and the record reads `forced=-2`, which is the
same byte-identical trap L101 names. Forced, it gives delta +4 at 444
aligned-exact, so the target does not split web 40.

**The 16-cell carrier lattice for the remaining temp_a0 web is closed.** Sites
{case-0 +0x48 read, case-2/3 +0x3E0 read} against carriers {temp_a0, var_a1,
spA0, inlined at either line} measure 9 at the retained form and 39 to 596
everywhere else. Routing either site through `var_a1` breaks the a1 colouring
outright (first mismatch moves to +0x70) and `spA0` costs 8 bytes.

Decision variables for the next lane, both named and both small:

- 7 words: `temp_a0`'s web takes a0 where the target reads v0, at the case-0
  +0x48 read (+0x330) and the +0x3E0 pointer (+0x630). The web still spans call
  results, so v0 is absent from its cost list for the L101 reason rather than
  outbid, and the carrier lattice above says no existing local splits it. What
  has not been tried is the same arity reading applied to *this* web: enumerate
  the calls inside its range and ask which one denies it, the way the pointer
  web's denial turned out to rest on one call rather than all of them.
- 2 words: at +0xA8 as1 emits the `move a1,s5` one slot after `sh t8,44(s0)`
  and the target one slot before. Definition positions 217 to 222 are flat, so
  the L59 key is not the definition's own line; the next thing to vary is the
  physical line of the `0x2C` store beside it.


**Both remaining families are bounded, and neither is a colouring decision.** A
full greedy force ceiling over the procedure -- all 64 p1 decisions against
colours 1 to 22 and 24 to 30 and the split path, 1,470 objects of which 938 were
accepted and 532 declined byte-identically -- finds nothing better than 9, with
the best accepted cell at 12. Web 28 is the merged `temp_a0` symbol: forcing it
to t0 moves the case-0 +0x48 read, the case-2/3 +0x3E0 read and the
`ext_o0_2d98` argument together, and its cost list offers v1 at infinite cost,
a0 at zero (the argument affinity), t0 to t5, c13 and s3 to s8, omitting v0, a1,
a2 and a3. v0 is L101's call-result denial rather than interference, so neither a
force nor an arity change reaches it and the web must instead become shorter --
for which the only split is a twelfth declaration, and that is a 0xB0 frame. The
+0xA8 slot is not a source-order tie either: all 120 permutations of the five
head-block statements including the pointer's definition score exactly 9, as do
twelve physical-line foldings of adjacent pairs.

#### 2026-09-12, lane w1-1: L160 carrier removal does not supply the missing colour

The assignment gate returned base-only. The configured stock baseline and the
instrumented unforced function text were byte-identical. Baseline and retained
result: 662 candidate and target words, delta 0, 9 raw and masked differences;
aligned 653 exact, 7 naming, 0 immediate and 2 structural. No insertions or
deletions. Both frames are 0xA8 and all 21 used stack offsets agree. The
relocation count is 38; this is diagnostic candidate evidence, not a linked
promotion or new matching credit.

The old shard provides a sweep receipt but no saved footprints path, and this
lane received no baseline footprints artifact. The old baseline was not swept
again. After deleting the case-3 pointer carrier, the source changed while its
object retained the same nine-word residual. That changed source was measured
with web_footprint --every-colour: 35 coloured webs represented, 232 actual
forces plus one no-second-colour row. There were 171 accepted equal-size cells,
none below 9; 167 moved windows and 4 were flat. The report was re-rendered with
--report for L159. There is no improving winner to pack, so the measured
single-force floor remains 9. This is not a proof about arbitrary interacting
forces or other source forms.

Eight source attempts preserved separately with configured objects and aligned
per-window deltas:

- Generate the case-0 pointer from an integer-typed field load: 39 masked,
  delta 0. Removing this use from the declared carrier regresses other windows.
- Generate the case-3 pointer at its test and access: 9, delta 0. The declared
  integer web changes from save 20.5 over four blocks to save 14 over three,
  but the four naming rows in that region remain. The source really changes
  the allocator records; identical output does not mean the edit was lost.
- Combine both generated pointer forms: 41, delta 0.
- Remove all sound-handle and state uses from the integer declaration, leaving
  an angle-only carrier: 41, delta 0, the same object as the preceding form.
- Remove the angle declaration and reuse the command cursor only after its
  command-writing lifetime: 44, delta 0. The frame does not buy an improvement.
- Add a carrier used only for the case-0 pointer: 44, delta 0.
- Give that carrier pointer type and use it for both link accesses: 44,
  delta 0. Type separation still does not produce the wanted output.
- Generate the update-scale constant and reuse an already-dead output float
  in the sound tail, freeing a declaration for a separate link pointer: 166,
  delta +28, including seven surplus instructions in the tail. Rejected.

L160's limitation here is explicit: removing the declared integer carrier's
pointer or sound roles does not independently reproduce the desired short v0
web. The case-0 forms disturb already-agreeing head allocation; the case-3
removal changes ownership without changing its emitted naming rows. The two
head scheduling rows also survive every tested form. No edit is adopted.
The retained source is the incoming nine-word candidate. The final three
carrier/typing/home variants made no improvement and exposed no further
source lever; this packet stops on that stall instead of repeating the old
colour, order or spelling sweeps. The next source question is how to obtain a
short generated link-load web without changing the shared head expression
allocation. No zero-scoring force-to-source route was established.

<!-- plateau-handoff:func_overlay_086_F0000474_18D22AC:end -->
