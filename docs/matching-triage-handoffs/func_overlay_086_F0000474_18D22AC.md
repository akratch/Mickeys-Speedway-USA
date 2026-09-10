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

<!-- plateau-handoff:func_overlay_086_F0000474_18D22AC:end -->
