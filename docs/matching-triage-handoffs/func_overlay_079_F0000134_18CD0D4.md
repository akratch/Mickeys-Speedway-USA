<!-- plateau-handoff:func_overlay_079_F0000134_18CD0D4:start -->
### `func_overlay_079_F0000134_18CD0D4` plateau handoff

- source: `src/overlays/o079/func_overlay_079_F0000134_18CD0D4.c`
- score: 198/882 words
- frame: 0xB8
- relocations: 88
- first mismatch: +0x7C
- summary: integer ring closed. Two composed edits, each a regression alone, take 288 to 229: spelling the race-state bit test as (flags << 0xD) & 0x80000000U == 0 burns the ugen ring temp the target burns between +0x54 and +0x58 (328 alone), and routing state->target->state through the already-declared spawned gives that pointer the target's pool colour instead of a ring temp (374 alone). Splitting dx = state->targetX - object->x into dx = state->targetX; dx -= object->x at the mode-0 dot product then closes 31 more. The remaining 198 are 167 fp rows: the fp scratch ring is still one web short from +0x368, and roughly 81 of them are the +0xB80 tail that cannot be bought back at 882 instructions
## 2026-09-11 phase census and force-sweep verdict (lane `lane/p9-alloc`)

Two cheap measurements, both of which close a lever rather than open one.
Nothing in the candidate changed.

**Phase census.** The instrumented IDO 5.3 uopt -- `.text` first checked
byte-identical to the tree's own object for this TU -- emits **77 p1 decisions
for this procedure (39 `color`, 38 `split`) and zero p2 records.** So L106 has
no axis here: web numbering, definition position, declaration order and
statement order cannot move a colour in this function, and only L100's
`save = totalsave/nocs` ratio decides anything p1 decides.

**And p1 decides almost nothing of this residual.** Splitting the 148 aligned
naming rows by register bank: **139 name only floating-point registers, 7 name
only integer registers, and 2 name both.** That agrees with this page's own
earlier reading of "167 fp rows", and it explains the sweep result below,
because L38 says p1 never colours into the local fp ring.

**Force-sweep verdict.** Forcing every colour each web's own `p1cost` record
declares available -- 174 (web, colour) cells, each force confirmed accepted in
the decision records rather than assumed -- moves the count at all in exactly
**one** cell, and that one is worth 4 words (198 -> 194). A greedy search
stops there. So **p1 colouring reaches 4 of the 148 naming rows and no more.**

The consequence for the next lane is a routing decision, not a new idea: this
function is a **ugen fp-scratch-ring** problem (L13, L23, L61, L92), and no
amount of save-ratio work, declaration reordering or web partitioning on the
p1 side can reach it. The dominant family is a three-cycle in the scratch ring,
`$f6 -> $f10 -> $f8 -> $f6`, at 32, 32 and 31 slots, first differing around
+0x4F8, with a separate `$f12 -> $f16` family of 12 from +0x3D8. A ring phase
is set by the order in which expression temporaries are allocated and freed, so
the levers are expression shape and commutative operand order at the sites
*before* +0x3D8, not anything downstream of them.

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

## 2026-09-11 closure re-test (lane `lane/f10-fell`): the +0x368 choice is a save tie and it does not own the tail

Measured with the instrumented uopt, `.text` byte-identical to the tree's
object, 77 p1 decisions and no p2 for this procedure. The candidate change
here is none.

The web defined at +0x368 is web 143 (class 2, save 5/2 = 2.5, decision
color, f16). Its `p1cost` list holds only c28 and c29 at 3.0 and the
callee-saved colours at 47.25; its forbidden mask is 0xf0, which is f0 f2 f12
f14. So f2 was never a colour the force sweep above could try on it, and the
sweep's "4 rows" bound does not cover this decision. f2 is held by webs 267,
346 and 401, each at save 3.0 and each decided before 143. Forcing web 346
off f2 (accepted, to f14) lets 143 take f2 unforced and the count goes 198 to
196: the two rows at +0x368 and +0x374 close and nothing after them moves.
That falsifies the reading above that the ring runs one position apart from
+0x368 onward and feeds the tail. One net occurrence on web 143 would tie the
f2 webs at 3.0 and win the tie on web number, which is the same mechanism
that matched func_80008B94 on this lane.

The tail's carrier reloaded from 100(sp) is web 213 (save 5/2 = 2.5, f12),
and in the base its list is c26 and c29 with f16 forbidden, because web 143
takes the 2.5 tie on web number and holds f16 first. With 346 forced off f2
and 213 forced to f16 the count is 533, with web 70 also on f16, so the tail is
not one force away from that state; it is a second decision (213 against 70
and 143) to be measured after the first is closed from source.

#### 2026-09-11, lane p7-fp: this is not overlay 27's problem, and the ladders say so

Sent here as the paired half of overlay 27's single-allocator-decision lane, on
the premise that the two share a mechanism because this candidate names f12
eleven times more than its target and f16 nine times fewer. The premise does not
hold. Baseline reproduces at 3528 bytes, 882 of 882 instructions, delta 0, 198
masked, with the per-file `-Wab,-r4300_mul` override in the harness; the
instrumented toolchain's text section is byte-identical to the tree's object.

**The buckets are a different shape.** Aligned against a register-erased target:
703 byte-exact, 148 register naming, 3 immediate-only, 30 really different, with
a displacement tax of 17 and two surplus and two missing instructions. Overlay
27 at the same moment is 320, 46, 0, 2 with no displacement tax and no surplus
or missing words. So this function still has structure to close; overlay 27 has
none.

**The float residual is mostly a ring rotation, not a colour.** The float bank
alone: f6 to f10 at 32 sites, f10 to f8 at 32, f8 to f6 at 31. That is one
closed three-cycle over the scratch pool accounting for 95 of the 148 naming
rows. The f12-to-f16 substitution the dispatch quoted is 12 sites. Coherence is
72 percent over ten source registers and the tool wants ten windows, which is
per-iteration consumption rather than one global phase. Overlay 27's float
residual is the opposite: one substitution, f12 to f16, at seven sites, and the
rest of its ring follows from that single colour.

**The float ladder.** This procedure emits p1 records only and no p2, so it is a
calling procedure and the axis is the save ratio. Its allocator makes 77
decisions, 45 of them class 2. Twenty float webs take a colour and twenty-five
split. Saves run from 5.4 down to 0.04; the largest is 5.4 at five components
and the second 5.0. There is no dominant carrier: overlay 27 has one web at 7.1
carrying 46 of its 48 words, and here the top four webs together carry a
fraction of the residual. The callee-saved colours cost 47.25 to 47.55 apiece
rather than overlay 27's 20.25, which is the call count showing through, and it
is why so many webs split rather than reach for one.

**The colour table is the same table.** Forcing the save-7.0 single-component
web onto each colour and reading the float histogram gives c26 f12 and c27 f14,
and c30 brings f20 into the function for the first time. Its forbidden mask of
0xc8 predicts declines at c24, c25 and c28 and all three are declined, which
confirms the bit encoding of 31 minus colour here as well. Combined with the
decode done on overlay 27 this pass, the table is c24 f0, c25 f2, c26 f12,
c27 f14, c28 f16, c29 f18, c30 f20, c31 f22 on both procedures, with f4, f6, f8
and f10 outside it. **So the note above reading a 0xf0 mask as f0, f2, f12, f14
is right**, and the overlay 27 shard's older claim of c24 f8 and c25 f10 was the
wrong one; it has been corrected there.

**What that means for whoever takes this next.** A solution to overlay 27 will
not transfer, because overlay 27's whole residual is one globalcolor tie and
this one's is a scratch-ring phase plus thirty structural words. The ring
three-cycle is the thing to attack first and it is an L127 question, not an
L100 one. The f12-to-f16 rows are a real but small colour component and the
forbidding rules named in the overlay 27 shard this pass apply to them: a float
argument register is forbidden to a web live in the call's block at or before
the call, and the float return register f0 is forbidden to a web whose range
reaches a call result.

<!-- plateau-handoff:func_overlay_079_F0000134_18CD0D4:end -->
