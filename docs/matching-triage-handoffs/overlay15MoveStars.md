<!-- plateau-handoff:overlay15MoveStars:start -->
### `overlay15MoveStars` plateau handoff

- source: `src/overlays/o015/overlay_015.c`
- score: 30/54 words
- frame: 0x40
- relocations: 25
- first mismatch: +0x30
- summary: The +16-byte address-lowering mismatch is fully localized; all size-closing BSS partitions regress aligned residuals.

#### 2026-09-11, lane `lane/p4-xfer`: refuted, and the size delta is now fully accounted

Unchanged at 30 relocation-masked words, size delta +16, frame 0x40, three
relocation-artifact words. Aligner: 37 byte-exact, 4 register naming, 3
immediate only, 15 really different. No edit adopted.

**The transfer from `overlay15DrawRain` is refuted because its precondition is
already satisfied here.** DrawRain's structural bucket fell 12 to 3 by
dissolving a padded struct into separate extern scalars. This sibling's nine
bounds are already nine separate extern scalars. It is not a function the
mechanism can move; it is already in the state the mechanism moves a function
toward, and what is left is precisely the defect the mechanism could not fix in
DrawRain either.

**The size delta is now fully accounted.** The ROM spends five overhead words
on the nine bound loads and this candidate spends nine. Four words is sixteen
bytes, which is the whole delta; nothing else contributes.

**The cost model, which explains why an earlier declaration sweep read flat.**
A symbol referenced once costs one overhead word, because the low half folds
into the load's displacement. A symbol referenced twice or more makes uopt
build a base register, costing two. So four pair aggregates plus one lone
scalar also costs nine, exactly as nine singles do. That sweep was flat by
construction, not by accident, and repeating it is waste. The reachable span
is eighteen words for nine singles down to eleven for a single aggregate over
all nine, with the ROM at fourteen; the single aggregate measures 42 masked at
delta -12, three words short.

**The ROM's overhead count is reachable, and reaching it does not help.** All
nineteen contiguous partitions of the nine arguments satisfying two per group
plus one per single equals five were enumerated and measured. Every one closes
the size delta to exactly zero. None improves masked; the best is 34. Closing
the size actually moves the object further away: byte-exact falls 37 to 30 and
really different rises 15 to 23. A lane that treats the size delta as the thing
to fix here will make the function worse.

**The decision variable, restated correctly.** The DrawRain note describes this
as as1 sharing a high half between two absolute loads. That is wrong on both
halves: as1 never shares, and the ROM's shared base is not a peephole artefact
-- it stays live across roughly fourteen scheduled instructions. This is a uopt
address-constant decision, and the variable is uopt's choice between an
assembler-temporary shared high half costing one word and an allocatable base
register costing two. Every spelling reachable from declarations, groupings and
argument order picks the base register. Register pressure is the obvious
untested hypothesis and is where the next pass should go; do not spend another
pass on declarations, groupings or argument order.

<!-- plateau-handoff:overlay15MoveStars:end -->
