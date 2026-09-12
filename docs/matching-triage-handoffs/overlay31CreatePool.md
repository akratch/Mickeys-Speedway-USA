<!-- plateau-handoff:overlay31CreatePool:start -->
### `overlay31CreatePool` plateau handoff

- source: `src/overlays/o031/overlay31CreatePool.c`
- score: 11/50 words
- frame: 0x38
- relocations: 4
- first mismatch: +0x28
- summary: Accepted force pair reaches zero at delta 0; source mechanism is record a0 interference.

#### 2026-09-11, lane `f9-small`: unused homed locals are inert on this frame

The 8-byte frame gap does not come from an unused local of any type: `void *`,
`f32`, `f64`, `volatile s32`, `volatile void *`, `s32[1..3]`, `u8[4]`,
`u8[8]`, a two-word struct, an address-taken `s32` (via `state = &tmp`), and
two pointers together all measure byte-identical (13 masked). An initialised
`volatile` costs instructions. So L99's "unused f32/pointer costs frame" does
not hold here, and the quantum is not a declaration. The other 11 words are
the record pointer and counter shifted `$a0/$a1 -> $a1/$a2`; a `count` copy
local regresses to +12 bytes. Decision variable: whichever web takes `$a0`
across the loop in the target (nothing in the loop reads `$a0`, so it is a
web with no instruction, as in `func_8003A754`'s v0) -- and it may be the
same thing that owns the frame quantum.
#### 2026-09-12, lane `p9-tight`: the frame is declaration order, and the rest is one colour

**The 8-byte frame gap is the declaration order.** Moving `s32 i; s32 j;` to
the head of the list gives the target's `0x38` exactly, with the same four-slot
ladder. That is adopted: 13 masked words to **11**, size delta 0, the aligner's
immediate-only bucket 2 to 0 and byte-exact 37 to 39. The previous note was
right that no *unused* local of any type supplies the quantum, and wrong to
conclude the quantum is not a declaration -- it is the order of the used ones.
Only `i` and `j` together do it; `i` alone and `j` alone both score 15 at
frame 0x30, and swapping them to `j, i` keeps 0x38 at 11.

**The remaining 11 words are one allocator decision, and it is priced.** An
instrumented uopt whose object was `cmp`-verified byte-identical to the
configured build records seven p1 decisions, in decision order:

- `state` save 305 -> c1 `v0`
- `j` save 155 -> c2 `v1`
- the literal 15 save 50 -> c7 `t0`, `forbidden0=0x7e000000` (c1-c6: it spans
  the four-argument config call)
- `record` save 40.5 -> c3 `a0`, `forbidden0=0x61000000` (c1, c2, c7 -- all
  taken, nothing denied)
- `i` save 15.5 -> c4 `a1`
- `count` save 4.33 -> c14 `s0`, `forbidden0=0x7f000000`
- `records` save 0.67 -> c8 `t1`, `forbidden0=0x7f020000`

`CDX_FORCE=p1:w9=c4,p1:w12=c5` with `CDX_PROC` set is **accepted** (the records
read `forced=4` and `forced=5`, not `-2`) and the forced object scores **0
masked words at delta 0** against this declaration order. So the function is
exactly one colour from a match.

**Decision variable: what puts c3 in `record`'s `forbidden` mask.** Three
things it is not, each measured:

- not the save ratio -- c3 is on `record`'s offer list at every save reached,
  and raising or lowering the ratio only reorders the decisions;
- not a call denial (L142) -- neither call loads `a0` alone. The alloc call
  loads a0 and a1, the config call loads a0-a3, and the literal-15 web shows
  what that looks like: `forbidden0=0x7e000000`, c1-c6 together. A web denied
  a0 by the config call would also be denied a1, and the target's `record`
  *is* a1;
- not an extra web taking c3 that the source can create, because such a web
  would need save above 40.5, must not span either call, and must emit no
  instruction -- the target's stream touches `$a0` nowhere between the two
  calls.

What is left is interference with a pre-coloured `a0`, and the only
pre-coloured `a0` in the function is the incoming `count`, whose range ends at
the size computation four instructions in, on both sides.

Flat at 11 on this base, so the next lane need not repeat them: all 32
combinations of `if (1)` regions at five sites (around the allocation, the
outer loop, the header writes, the inner loop, the config call); empty trailing
compares on `record`, `i`, `count`, `records` and `record != 0`; L109 probes on
`i` and `count` in the outer loop; a `header` pointer aliasing `record->state`;
a `kind` local supplying the config call's four zero arguments; an indexed
form; a trailing `record = records` reset; and `state` hoisted out of the outer
loop (that one costs 4 bytes).

**One idiom reaches 9 and must not be adopted.** The empty trailing compare
that matched `func_8003A754` (`if (record != records) { }`) gives 9 here, but
by the wrong mechanism: the records show it puts `0x40000000` in `state`'s
`forbidden` mask, so `state` and `j` each shift up a colour as well and the
inner loop goes wrong in two new places. It moves `record` and `i` correctly
and everything else incorrectly.

#### 2026-09-12, lane `p23-lastmile4`: force combination independently rechecked

The fresh base is 200 bytes, 50 words, delta 0, frame 0x38, four relocations,
and 11 aligned naming rows. The 59-probe every-colour footprint has three
winners: w9=c4 scores 3, w12=c5 scores 8, and w22=c3 scores 10. The first two
are rivals over the same window, so their combination was measured rather than
assumed.

With an instrumented-object fidelity pass, the accepted pair w9=c4 plus
w12=c5 scores zero masked words at delta 0. Their signed window effects are
minus eight and minus three, and the pair is exactly additive. The named source
question remains what makes the record web’s a0 unavailable while preserving
the target frame and call geometry; the diagnostic force itself is not a
shippable source change.
<!-- plateau-handoff:overlay31CreatePool:end -->
