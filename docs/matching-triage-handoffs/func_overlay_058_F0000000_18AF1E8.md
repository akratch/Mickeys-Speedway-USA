<!-- plateau-handoff:func_overlay_058_F0000000_18AF1E8:start -->
### `func_overlay_058_F0000000_18AF1E8` plateau handoff

- source: `src/overlays/o058/func_overlay_058_F0000000_18AF1E8.c`
- score: 295/368 words
- frame: 0x78
- relocations: 95
- first mismatch: +0x0
- summary: Target block solved at 52 bytes, thirteen declarations; six carriers must merge and the surviving positions are pinned.

## 2026-09-11 the target's block solved: thirteen declarations, positions pinned (lane `lane/o11-frames`)

Adopted: a declaration reorder, byte-inert.  295 masked, 109 aligned byte-exact,
165 naming, 18 immediate-only, 87 really-different, delta 0, before and after.
Four choices for which local carries the second position measured identical.

**The frame identity.**  Both objects put the argument build at 0x00..0x17 and
the `s0`/`ra` saves at 0x18/0x1C, and both leave the same twelve bytes of
temporary above them.  So the 24-byte gap is the declaration block and nothing
else: 76 bytes here, 52 in the target.  That is thirteen four-byte declarations
against this candidate's nineteen -- **exactly six**, which sharpens the earlier
note on this page from "five or six cells".

**Where the surviving thirteen sit.**  Reading the target's own slot traffic
against its frame top:

- 1 at -4, spilled once and reloaded once around the two split calls
- 2 at -8, no home traffic
- 3 through 8 at -12 .. -32, the six address-taken out-parameters
- 9 at -36, the order state: stored once right after the first call and read
  back from its home **eight** times
- 10, 11, 12 at -40, -44, -48, no home traffic
- 13 at -52, spilled once and reloaded once around the same two split calls

The adopted order reproduces positions 1 and 3 through 9 of that ladder; 10
through 13 follow once six declarations are gone.

**Two things this changes about the standing description.**

The candidate keeps `state` in a callee-saved register and touches its home
three times; the target does not give it a register at all and re-reads it
eight times.  That is an allocation difference, not a spelling one, and it is
why `state` is ninth in the target's list and eighth here.

The target spills **two** locals around the split-value calls, at the top and
the bottom of its block; this candidate spills only `i`.  The thirteenth
declaration is therefore a second value live across those calls -- a cursor or
class pointer this candidate keeps in an `s` register.

**Probes measured, not adopted.**  Merging `tagSource` into `entry` (301
masked, 103 exact, 171 naming) and `limit` into `count` (294 masked, 105 exact,
168 naming) each take the frame to 0x70 at delta 0; both together also reach
0x70, since seventeen and eighteen cells round the same.  Only thirteen cells
reach 0x60.  Neither merge is evidently the target's, and picking six wrong ones
costs more than the frame buys -- the ladder above is the constraint to solve
against.
<!-- plateau-handoff:func_overlay_058_F0000000_18AF1E8:end -->
