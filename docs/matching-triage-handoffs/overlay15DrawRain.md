<!-- plateau-handoff:overlay15DrawRain:start -->
### `overlay15DrawRain` plateau handoff

- source: `src/overlays/o015/overlay_015.c`
- score: 13/54 words
- frame: 0x40
- relocations: 17
- first mismatch: +0x74
- summary: The residual is exactly one instruction, and the previous note's decision variable is refuted. Three separate extern f32 scalars reproduce the target byte-for-byte apart from one extra lui at: six words where the ROM spends five, everything else in the block agreeing including the ori at zero 0x8000 placement, the pre-call lwc1 and lw argument scheduling, and all four swc1 orderings. as1 does NOT share a high half. Measured directly: two references to the SAME extern in two basic blocks emit two lui at with identical R_MIPS_HI16 relocations and as1 leaves both, so the ROM's shared high half is not an assembler merge and no C spelling can ask for one. What the ROM spends is a third lowering neither reachable path produces, a hi-only base kept in at with the lo folded into TWO displacements (128 and 132) and a second lui for the third (136). The two paths IDO does take here were confirmed on isolated probes: a symbol referenced once becomes an as1 absolute macro at two words per load, a symbol referenced twice or more makes uopt build a full base register at lui plus addiu plus one word per load. The base-register decision is not splittable from source: an if(1) region, a do-while(0) region and a volatile cast all leave it, and only a real branch splits it, which then yields two separate macros rather than one shared high half. Eighteen further spellings measured flat beyond the previous note's twelve, including all four absolute-literal forms, which prove the ROM's addresses are relocatable symbols rather than assemble-time constants because *(f32 *)0x80 collapses to lwc1 128(zero) with no lui at all at delta -8. Next lane: the variable is uopt's address-constant lowering, shared with overlay15MoveStars in this same TU, and register pressure remains the one untested hypothesis for making uopt decline the base register.

#### 2026-09-12, lane `p9-tight`: as1 does not share a high half, so the stated decision variable is refuted

The previous note's residual measurement stands: three separate `extern f32`
scalars reproduce the target byte-for-byte except for **one extra `lui $at`**,
six words where the ROM spends five. The `ori at, zero, 0x8000` placement, the
pre-call `lwc1 $f8` / `lw $a3` scheduling and all four `swc1` orderings agree in
that form; the tail is shifted one word, which is the whole of its 20 against
the base's 13.

**What is refuted is the cause.** as1 does not merge `lui $at`. Measured on an
isolated probe: two references to the *same* extern struct, placed in two basic
blocks so uopt cannot pool them, emit two `lui $at` carrying identical
`R_MIPS_HI16` relocations against that one symbol, and as1 leaves both. So the
ROM's shared high half is not an assembler merge, and no C spelling can ask an
assembler for something it does not do. "Stop hunting declarations" was right;
the reason given for it was not.

**What the ROM actually spends** is a third lowering that neither reachable path
produces: one `lui` holding a `%hi`-only base with the `%lo` folded into *two*
displacements (128 and 132), then a second `lui` for the third (136). IDO takes
exactly two paths here, both confirmed on isolated probes:

- a symbol referenced **once** becomes an as1 absolute macro, `lui $at, %hi(S)`
  plus a load carrying `%lo(S)` in its displacement: two words per load;
- a symbol referenced **twice or more** makes uopt build a full base register,
  `lui` plus `addiu` plus one word per load.

The base-register decision is not splittable from source. An `if (1)` region, a
`do { } while (0)` region and a `volatile` cast all leave it in place on the
two-reference probe; only a real branch splits it, and that yields two separate
macros rather than one shared high half.

Eighteen further spellings measured flat beyond the previous note's twelve,
none below the base's 13 at delta 0 and none reaching five words for the three
loads: three externs (20, +4), a two-element extern array plus a scalar (24,
+4), a two-float extern struct plus a scalar (24, +4), a three-element array
(13, 0), the padded struct with z split out (24, +4), four L131 spelling splits
that give the two same-symbol references different written forms (24, +4 each),
a union with the pair reached through two different member paths (24, +4), the
pair reached as `*(&x + 1)` (24, +4), two padded-struct spelling splits (13, 0
each), and an `if (1)` region added to the three-extern form (20, +4).

**The absolute-literal forms are worth recording as a negative with a reason.**
`*(f32 *)0x80` and its array spelling collapse to `lwc1 $f18, 128($zero)` with
no `lui` at all, at delta **-8**. That is not a near miss, it is proof that the
ROM's three addresses are relocatable symbols rather than assemble-time
constants, which retires the whole absolute-literal family.

**Decision variable for the next lane:** uopt's choice between the absolute
macro and the base register, which is the same variable blocking
`overlay15MoveStars` two functions up this file (its note reaches it from the
other side, as a uopt address-constant decision, and names register pressure as
the untested hypothesis). Nothing in declarations, groupings, regions,
qualifiers or argument order moves it. The one axis neither lane has tried is
raising register pressure in the call's own block so that no allocatable base
register is free.

<!-- plateau-handoff:overlay15DrawRain:end -->
