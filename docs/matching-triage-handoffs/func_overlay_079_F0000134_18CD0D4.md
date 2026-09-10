<!-- plateau-handoff:func_overlay_079_F0000134_18CD0D4:start -->
### `func_overlay_079_F0000134_18CD0D4` plateau handoff

- source: `src/overlays/o079/func_overlay_079_F0000134_18CD0D4.c`
- score: 198/882 words
- frame: 0xB8
- relocations: 88
- first mismatch: +0x7C
- summary: integer ring closed. Two composed edits, each a regression alone, take 288 to 229: spelling the race-state bit test as (flags << 0xD) & 0x80000000U == 0 burns the ugen ring temp the target burns between +0x54 and +0x58 (328 alone), and routing state->target->state through the already-declared spawned gives that pointer the target's pool colour instead of a ring temp (374 alone). Splitting dx = state->targetX - object->x into dx = state->targetX; dx -= object->x at the mode-0 dot product then closes 31 more. The remaining 198 are 167 fp rows: the fp scratch ring is still one web short from +0x368, and roughly 81 of them are the +0xB80 tail that cannot be bought back at 882 instructions
#### Region partition

Re-measured against the configured TU: 882 words on each side, size delta 0,
198 relocation-masked differing words in 18 regions. This is the largest
function in the tree with a residual under 200, and the partition is heavily
concentrated:

- 101 words at +0xB68..+0xD4C -- the tail the prior handoff estimated at
  roughly 81
- 21 at +0x4F8..+0x598
- 16 at +0x7A4..+0x814
- 15 at +0xA14..+0xA84
- 11 at +0x868..+0x8B8 and 11 at +0x494..+0x4C4
- 5 at +0x630..+0x654
- 3 at +0x8EC..+0x8FC
- 2 each at +0x7C..+0x88, +0x368..+0x374, +0x9DC..+0x9E8, +0xB1C..+0xB20
- single words at +0x3D8, +0x68C, +0x770, +0x948, +0xAC8

The tail is not "unbuyable at 882 instructions". It is one web colour, and it
is the same one that holds `func_overlay_027_F0000064_187BA3C` at 40 words:
the multiplier reloaded from 100(sp) and used at nearly every product in the
tail is f12 in the candidate and f16 in the target. A register histogram over
the whole function makes the collision explicit -- the candidate names f12
eleven times more often than the target and f16 nine times fewer, while every
other register's count agrees to within three. The target does use f12 as an
ordinary temporary in the tail; it simply does not give it to this web.

The remaining regions are ring phase offsets of the kind at +0x4F8, where the
operand structure is exact and only the temporaries rotate, plus a handful of
delay-slot placements around the multiply latencies in the tail.

Order of attack: the f12/f16 web decides half the residual on its own, and a
solution transfers directly to overlay 27. The forms already measured flat on
that axis in overlay 27 -- declaration order, operand commutativity, statement
order, loop form, `register`, carrier locals, physical line folding -- do not
need re-running here. What moved it in overlay 27 was extending the web's
lifetime across a call, which bought the correct ring at the cost of a spill
pair; overlay 79 is frameless-prologue and large enough that the same trade
may be worth measuring here even though it was not adoptable there.
#### Where the fp divergence actually begins, and what it rules out

The first floating-point divergence is the two-word region at +0x368. Every
floating-point instruction before it is byte-identical on both sides, and at
that point both f2 and f16 are free -- f2's previous use ends at +0x2BC and it
is not redefined until +0x368 in the target and +0x498 in the candidate. The
target takes f2, the candidate takes f16, and from there the ring runs one
position apart for the rest of the function, which is where the tail's f12
comes from.

That matters because it bounds the search. A two-way choice between two free
registers, reached through identical preceding code, cannot be produced by the
spelling of any expression before it. It comes from the function's web set --
what the allocator was handed before it emitted anything.

Two exhaustive receipts on that:

- Single-move declaration order is flat. All 342 single-position moves of the
  nineteen locals were measured; none scored below 198, and the four ties at
  198 are permutations that leave every frame slot where it was.
- The local set is already exact. Inserting one unused `f32` at any of the
  twenty positions costs 47 words (198 to 245) at size delta 0, because every
  frame slot below the insertion shifts. The frame the candidate produces is
  the frame the target has, so no local can be added and, by the same
  argument, none removed.

So the remaining lever is neither the declaration list nor the expression
spelling. It is whatever makes one existing web claim f16 across +0x368 in the
target. Overlay 27 shows one thing that does it -- extending the web's
lifetime across a call -- at the cost of a spill pair; here the tail's carrier
already round-trips through 100(sp) at every use cluster, so the same trade
may not cost anything extra. That is the first thing to measure.
<!-- plateau-handoff:func_overlay_079_F0000134_18CD0D4:end -->
