<!-- plateau-handoff:overlay1FindType47ByAngle:start -->
### `overlay1FindType47ByAngle` plateau handoff

- source: `src/overlays/o001/overlay_001.c`
- score: 10 differing words
- frame: 0x78
- relocations: 6
- first mismatch: +0x8
- summary: Eighteen masked words to ten, and the residual is two named allocator decisions with a price on each. The TU carries -Wab,-r4300_mul as of this lane, adopting the diagnostic the earlier notes recorded: it removes the entire structural half, the really-different bucket goes 5 to 0 and the displacement tax 7 to 0, and the surplus at +0x88 against the missing at +0xA8 is gone. The other two guarded functions in this TU are bit-for-bit unmoved at 3 and 12 and gmake verify passes, which is the safety property the Makefile block states. The structural half was never a source question: cc -S shows ugen already emitting the target's order, mul.s into f12 then mov.s into f14 then the call, and what differed was as1's delay-slot choice, so no statement or physical-line arrangement could reach it. Forcing both remaining colours on the flag build scores 0 masked at delta 0. First the start carrier, web 14, save 1.0, nocs 2, totalsave 2, forbidden c1 c15 c16 c18 all merely taken: it is the last integer decision and takes the lowest free colour c2 v1 where the ROM takes c3 a0, and since c2 is forbidden to nothing here and the ROM never uses v1, what is needed is a web that TAKES c2 first. Second the float pair: scale web 28 has totalsave 11 against angle web 44 at 10, one unit, the weight of scale's out-of-loop definition, so scale is decided first and takes the lower colour while the ROM has angle lower. Flat at 10 on the flag build: all six declaration orders of the three f32 locals and all six of the three s32 locals, nine call line arrangements, a product temporary reusing difference, an indexed cursor, a scale-first cursor line, empty trailing compares keeping angle and start live, and two discarded difference-equals-angle seeds. The seeds are folded by cfe and the records prove it rather than inferring it: angle's totalsave stays 10 and scale's 11 in every seeded form, so L109 supplies no float spelling here.
- assignment base: `05cbca2025f87f1b4b670eaa73c6cd181ffd0dc7`
- owned range: Overlay 1 `+0x1AC..+0x2D4`, ROM `0x184C58C..0x184C6B4`, exactly 296 bytes / 74 instructions with no target padding
- configured V0: The restored full-TU `-O2 -mips2 -32` body has the exact `0x78` frame and extent, 56/74 positional words, 18 relocation-masked differences, 19 raw differences, ten opcode mismatches, four alignment gaps, and first mismatch `+0x8`.
- flag lattice: All 119 flag identities were attempted and 53 compiled/extracted. `-O2 -mips2 -32 -Wab,-r4300_mul` is the sole strict diagnostic gain at 64/74 positional words, ten masked / eleven raw differences, exact frame and extent, and zero opcode mismatches or alignment gaps. The flag affects every guarded function in this mixed TU, and `tu_flag_impact.py` fails closed on an unrelated ambiguous fallback, so no Make policy change is supported.
- mechanism: Under the diagnostic flag, all integer and FP temporary lanes are exact. Six residual sites exchange the long-lived `angle` and `scale` FP pool colors (`f22`/`f24`), and four exchange the loaded `start` carrier (`a0`/`v1`). Configured code additionally retains the structural schedule divergence removed by the flag.
- relocation proof: Candidate and target each have six records, and all six offsets, types, identities, and addends agree. Runtime metadata authenticates resident `func_8000572C` at `+0x34`, local-data pairs at `+0x44/+0x48` and `+0x60/+0x68`, and local `overlay1WrapOffset` at `+0xAC`. Direct resident naming plus the partial-surface resolver close the formerly ambiguous identities; linked code equality remains fallback-only because 18 instruction words differ.
- ABI and callers: The exact owner has no export and one authenticated local inbound at Overlay 1 `+0x3C54`. That caller passes additional O32 arguments, but declaring them on this callee grows and radically regresses the object; the target compile-time shape is the retained one-`f32` signature.
- attempts: Nine coherent source forms covered FP and index declaration order, explicit angle carriage, `register` priority on scale and angle, guard/index coalescing, loop-local scale and difference lifetimes, and the caller-surface four-argument declaration. Five were byte-flat; the explicit carrier and both narrow scopes regressed; the four-argument form changed extent and regressed broadly. V0 was restored byte-for-byte. The historical bounded permuter was not repeated because no natural source form produced a strict gain.
- donors: The pinned DKR v77, DKR v80, and JFG overlay scans all classify Overlay 1 as `none`. The nearest permitted function skeleton is only 0.126 and supplies no credible body or naming evidence.
- next action: Reopen only with new source-authentic evidence for the paired FP-pool and start-carrier allocation mechanism. Do not repeat this lattice, these nine forms, or the historical generic permutation batch.


#### c2-o001: the pool residual is the angle/scale priority pair, and scale's placement does not move it

Re-measured at 18 masked words, 19 raw, exact 0x78 frame and extent, and the
residual narrows to one swap with a tail. The two long-lived callee-saved FP
webs exchange colours: the target gives the incoming angle the lower of the
contended pair and the phase scale the higher, and the candidate does the
reverse. The limit and the zero constant take the same colours on both sides,
and the save slots for all four registers are identical, so this is a
priority order between two webs and not a frame or a ring question. The
eight-word block before the wrap call is downstream of the same swap, since
each side moves whichever register it gave the angle into the second argument.

Six placements of the scale read were measured this lane and none of them
moves the pair: hoisted above the guard (24), given its own statement inside
the guard (19), read directly at the multiply (59 and one instruction longer),
made the function's first statement (35), packed ahead of the cursor
computation on the same line (20), and read inside the loop (70 and one
instruction longer). The retained packed form remains the best row.

The residual is therefore a uopt web-priority tie between a parameter that is
live from entry and a loop-invariant read that is live from the guard, both
used exactly once. Reopen with an extra interfering web or a measured
priority dial, not with another placement.


#### p8-o001: the float pool swap is priced at 4 of the 18 words

The records were read for the first time here. This procedure issues calls, so
p1 only; the instrumented object scores 18/19 exactly as the configured one
does. Four `class=2` float webs are coloured, in descending save: web 5
(save 7.0, totalsave 21) takes colour 30, web 28 (3.667) colour 31, web 44
(3.333) colour 32 and web 77 (3.333) colour 33. The seven integer webs run
`v0`, then `s0` through `s4`, then `v1`.

A force sweep of all four float webs against colours 28 to 35 plus the split
path, and then nine two- and three-way combinations of them, bottoms out at
**14**, reached by `p1:w28=c32` alone. So the contested float-pool ordering the
previous record describes is worth 4 words, not 18, and the remaining 14 are
elsewhere -- consistent with the aligner, which reads 5 really-different words
and a displacement tax of 7 with one instruction surplus at +0x88 against one
missing at +0xA8.

Two things follow. The pool priority is a smaller prize than it looked, and any
further work should start from the surplus/missing pair rather than from the
`f22`/`f24` exchange. And the L142 arity lever does not apply: the contested
colours here are callee-saved float registers, which that law does not reach.

Also eliminated, from the newly-matched `overlay1MeasureCurves` lever (an
address-taken parameter is reloaded from its home with no volatile scheduling
edge): reading `angle` through `*(f32 *)&angle` at the call is 64 and a word
short, `scale` through its address is 68, both together 56 and two words short,
`volatile f32 angle` is 74, and `volatile f32 scale` is 64. The lever does not
transfer to this function.
#### 2026-09-12, lane `p9-tight`: the diagnostic flag is adopted, and the rest is two priced colours

`-Wab,-r4300_mul` is now set on this TU in the Makefile. It takes the function
from 18 masked words to **10** at size delta 0, and what it removes is the
*whole* structural half: the aligner's really-different bucket goes 5 to 0, the
displacement tax 7 to 0, and the surplus instruction at `+0x88` against the
missing one at `+0xA8` disappears. The safety property is the one the Makefile
block already states -- the other two guarded functions in this TU are
bit-for-bit unmoved at 3 and 12 masked words, and `gmake verify` prints the
expected SHA1 with the flag in place. `tu_flag_impact.py` still fails closed
here, on `overlay1ActivateObject`'s two GLOBAL_ASM fallbacks, exactly as the
2026-08 note recorded; the ROM is the evidence instead.

**The structural half was never a source question.** `cc -S` shows ugen already
emitting the target's order -- `mul.s $f12` , then `mov.s $f14`, then the call.
What differed was as1's delay-slot choice: without the flag as1 hoists the
`mov.s $f14` out of the unsigned-to-float conversion block and fills the delay
slot with the multiply instead. That is why nine call line arrangements, a
product temporary and every placement of the scale read were flat, and it
retires "start from the surplus/missing pair" as a *source* lead.

**What is left is exactly two allocator decisions, and forcing both on the flag
build scores 0 masked words at delta 0** (`p1:w14=c3,p1:w28=c32`, four
`forced=` acceptances in the records, instrumented object `cmp`-identical to
the configured one).

- **The start carrier.** Web 14, save 1.0, nocs 2, totalsave 2, `forbidden0`
  `0x4001a000` = c1, c15, c16, c18 -- all merely taken. It is the last integer
  decision and takes the lowest free colour, c2 `v1`; the ROM takes c3 `a0`.
  c2 is not forbidden to any web in this procedure, and the ROM's stream never
  uses `v1`, so what is needed is a web that *takes* c2 ahead of it.
- **The float pair.** `scale` (web 28) has totalsave 11 against `angle`'s
  (web 44) 10 -- one unit, the weight of `scale`'s out-of-loop definition --
  so `scale` is decided first and takes the lower colour. The ROM has `angle`
  lower. `angle` needs one more unit of weight, or `scale` one less.

Measured flat at 10 on the flag build, so the next lane need not repeat them:
all six declaration orders of the three `f32` locals and all six of the three
`s32` locals; nine call line arrangements including the fully folded form; a
product temporary reusing `difference` (17 before the flag, 10 after); an
indexed `&objects[start]` cursor (11); a scale-first cursor line (11); empty
trailing compares keeping `angle` (13) and `start` (16) live to the exit; an
`if (angle != angle) { }` probe (19); and two discarded `difference = angle`
seeds meant to raise `angle`'s weight.

**Those seeds are the useful negative.** They are folded by cfe, and the
records prove it rather than inferring it: `angle`'s totalsave stays 10 and
`scale`'s 11 in every seeded form, only the web numbers move. So L109 supplies
no float spelling here, and the one unit of weight has to come from a real
reference.

<!-- plateau-handoff:overlay1FindType47ByAngle:end -->
