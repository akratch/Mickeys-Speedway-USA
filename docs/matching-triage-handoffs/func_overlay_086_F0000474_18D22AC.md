<!-- plateau-handoff:func_overlay_086_F0000474_18D22AC:start -->
### `func_overlay_086_F0000474_18D22AC` plateau handoff

- source: `src/overlays/o086/func_overlay_086_F0000474_18D22AC.c`
- score: 29 differing words
- frame: 0xA8
- relocations: 38
- first mismatch: +0xA8
- summary: 51 fell to 29 because the lever named as "one interfering caller-saved web removed" is a web SHORTENED, at no width. The two +0x48 dereference sites shared one cfe temporary, giving uopt a single six-reference web (save 30) that spans both switch arms and is therefore live across a call -- and a web live across a call has v0 struck from its candidate list entirely, which the traced p1cost rows show and no CDX_FORCE can override, so the target's v0 was never a colouring choice. Carrying the case-0 site in the already-declared temp_a0 splits that temporary: the case-2/3 site becomes its own one-block web, stops crossing a call, regains v0 and takes it, and the ten-row state-byte family falls into v1 behind it (51 to 30, delta 0, frame unmoved, no new declaration, L44). Writing the 0xC command word through the pointer before the advance is the target's own sh 0(s5) shape and closes one more. The 29 left are 7 words of temp_a0's own web, still spanning the head block's +0x40 reads and the f690 results and so still barred from v0 -- the only free head-block carrier, spA0, costs 579 words and 4 bytes -- and 22 words of the command pointer, whose a1 is held by a web that needs it (forcing it off costs 211 words and 4 bytes) and outranks it 6.67 to 2.67.

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

<!-- plateau-handoff:func_overlay_086_F0000474_18D22AC:end -->
