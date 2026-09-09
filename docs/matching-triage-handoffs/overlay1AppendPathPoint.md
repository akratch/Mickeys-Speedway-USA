<!-- plateau-handoff:overlay1AppendPathPoint:start -->
### `overlay1AppendPathPoint` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 102/108 words
- frame: 0x28
- relocations: 8
- first mismatch: +0x134
- summary: prefix exact to row 51 and the temp ring identical 24/24; residual is one extra pool web at the anchor CSE and the anchorX colour


#### tu2-o1tail: named locals take pool colours, inlined expressions take temps

21 masked words down to 6, at the same 108 instructions and 0x28 frame, from
one law measured three times in this translation unit: a value held in a named
local is given a uopt pool colour at its first surviving definition, while the
same value spelled as the field or global access again is CSE'd into the temp
ring and numbered after every named local. The pool lane is the readout: when
the candidate carries one more pool entry than the target, the source has one
named local the original did not.

Two carriers were wrong here. `index = state->count` was a named local taking
v0 where the target wants dx; spelling `state->count` at each of its three
sites moves it into the CSE carrier and puts dx back in v0 (21 to 14 words).
`anchorDy` was likewise named; writing it inline as
`(s16)(pointY - overlay1AnchorY)` twice lets IDO evaluate it once into the
shared ring, exactly as the target does, and the temp lane becomes identical
24 of 24 (14 to 6 words). Dropping the `(s16)` cast is not equivalent: the
uncast form loses two instructions and scores 22.

Residual, 6 words, all in the anchor block at +0x134: the target colours
`anchorX` v1 and reaches the squared distance through a shared-ring temp,
while the candidate spends one more pool colour there and shifts anchorX to
a0. Eliminated, do not repeat: dropping the `anchorX` local (15 words),
defining anchorDx before anchorX (15), inert XOR at the comparison (6, flat),
swapping the comparison operands (6, flat), reusing the `dy` local for the
anchor delta (11), an s32-cast pair (6, flat), a function-scope `anchorDy`
(14), and swapping the two addends of the squared sum (31, one extra
instruction). A 30-minute permuter run from the 6-word base was flat.
<!-- plateau-handoff:overlay1AppendPathPoint:end -->
