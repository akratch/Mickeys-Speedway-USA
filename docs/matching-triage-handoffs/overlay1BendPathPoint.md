<!-- plateau-handoff:overlay1BendPathPoint:start -->
### `overlay1BendPathPoint` plateau handoff

- source: `src/overlays/o001/overlay_001_tail.c`
- score: 86/107 words
- frame: 0x30
- relocations: 6
- first mismatch: +0xC
- summary: The fifteen index-block words are NOT a colour question, and that overturns this shard's own framing. Records read for the first time on this function, instrumented object cmp-identical to the configured one, proc 36. The four index webs are identified by forcing each to c9 and reading which sites move: web 22 is currentIndex (ours a1, ROM v1), web 25 previousIndex (ours v1, ROM a0), web 99 the path-count carrier (ours a2, ROM a1), web 44 nextIndex (ours v0, ROM a0), and web 2 the reloaded index parameter, whose register a2 already agrees. currentIndex carries forbidden0 0x70000000, c1 c2 c3, so v1 is denied outright and a force onto it reads forced=-2. The decisive experiment: freeing every other web that holds v1 in the same run, webs 79, 49 and 25 forced to t0, t1 and a0, leaves currentIndex's mask bit-for-bit unchanged and the force still declined. So the denial is not interference with a coloured rival and no ratio, spelling, statement order or force reaches the ROM's assignment; currentIndex's live range has to change. A 36-point single force sweep over twelve integer webs against c2, c3 and c4 confirms it from the other side: fifteen were accepted and not one scores below 21, the best alternatives being 24 and 26, and the nearest joint force, previousIndex to a0 with the count to a1 and nextIndex to a0, reaches only 23. Also applied here, L146: the 24-order pointer declaration sweep this shard records as an identity was climbed on the volatile-local shape, so it is void on the address-form shape, and re-climbing it there is flat at 55 to 59 across all 24, with the best two equal to the address form's own 55. The address route does not become competitive under a fresh order climb. Next lever: a structural change to currentIndex's range, not a colour.

#### tu2-o1tail: the u8 parameter's spill slot is the residual, and it is not source-reachable so far

Re-measured at the assigned base: 25 masked words, 107 of 107 instructions,
0x30 frame exact, first mismatch +0xC. Two mechanisms were isolated and both
resisted every form tried.

First, the spill slot. The target spills the `index` parameter to the byte of
its own incoming argument home (sp+59, the low byte of a2's slot at sp+56) and
reloads it there after the `overlay1GetPathReloc` call. The candidate's
`volatile u8 localIndex` hack produces the same store/reload pair, but into a
separate local at sp+25. Removing the hack so IDO spills the parameter itself
does not work: the instruction count drops to 106 and the residual explodes to
104 words. Also eliminated: `index = *(volatile u8 *)&index` after the call
(108 instructions, 105 words), a `volatile u8 index` parameter (108, 103), and
dereferencing the address of the volatile local (108, 106).

Second, the index-block colours. The target colours currentIndex v1,
previousIndex a0 and the path-count carrier a1; the candidate has previousIndex
v1, currentIndex a1, count a2. Eliminated: writing `currentIndex = index` in
the else branch where index is known zero, which does reproduce the target's
`move v1,a2` but costs 97 words overall; ordering the else branch with
currentIndex first (flat at 25); folding the decrement into
`previousIndex = path->count - 1` (26); and spelling the decrement as
`previousIndex = previousIndex - 1` (flat).

Three consecutive attempts on each mechanism produced no better residual and no
new identity, which is the stall. Next lever: the parameter-home spill is an
allocator decision, so it needs ugen ring or emit-order instrumentation rather
than another source form.

#### lm-o1tail: the parameter-home spill is source-reachable after all

The previous record called the spill slot an allocator decision needing ugen
instrumentation. It is not: `*(u8 *)&index = index;` written before the
`overlay1GetPathReloc` call reproduces it. Taking the parameter's address makes
IDO keep `index` in memory, and the redundant store lands on the byte of the
parameter's own incoming argument home -- the exact slot the target uses -- with
107 instructions and the 0x30 frame both held. It also puts the third argument's
save into the target's out-of-order position, second rather than third, so the
prologue's first four words become exact.

Two things it does not yet buy. The store is scheduled before the call rather
than into its delay slot, where the target puts it and where this form puts the
selector mask instead; and the reload lands in a ugen ring temp rather than the
argument register the target keeps it in, which shifts the index-block colours
downstream. Positionally that form scores 48 words against the retained hack's
25, so the `volatile u8` local stays as the candidate -- but the hack cannot
reach the target's slot at all, because its local is allocated in the local area
(sp+25) and only the parameter itself lives in the argument area. The address
form is the structurally correct route and the hack is not.

Measured and eliminated from this base, do not repeat: `volatile u8 index` as
the parameter, with the value read directly, read once into an `s32`, read once
into a `u8`, and read before the call (108 to 110 instructions); a plain
self-assignment and `*&index = index`, both eliminated before codegen (106);
`*(volatile u8 *)&index = index` before, after, and on both sides of the call
(109 to 111); a `u8 *` pointer local carrying the address (107, same 48-word
basin as the direct address form); reading back through `*(u8 *)&index` or a
named `u8`/`s32` local after the store; the volatile local declared first and
last in the list (frame 0x38 and 0x40 -- its position moves the frame); and the
plain no-hack form, which is 106 instructions because it reloads from the
argument home without ever storing to it.

Next lever: the delay-slot choice between the parameter store and the selector
mask, and then the reload's carrier. Both sit on the same three instructions.


#### p8-o001: the address lever gives the reload but not the store

`overlay1MeasureCurves` matched in this lane on the general form of what the
previous record here was reaching for: taking a parameter's address makes it
memory class, so every read is a load from its argument home and -- unlike
`volatile` -- no scheduling edge is emitted. Applied here it does not close the
gap, and the measurement says why.

Reading `index` through `*(u8 *)&index` at all four of its uses, with the
`volatile u8 localIndex` hack removed, produces 106 instructions against the
target's 107 and scores 104. Adding `*(u8 *)&index = index;` before the call
restores the count and scores 55. The store is therefore load-bearing and the
reads are not: the target genuinely stores the parameter to the byte of its own
home at `sp+59` and reloads from there, and address-taken reads alone give the
reload without the store. Also measured: the same address reads alongside the
retained volatile local (105, delta +8), and the selector read through its
address as well (104, delta -4).

So the previous record's ranking stands -- the retained hack at 21 words, the
structurally correct address form at 55 -- and the open question is unchanged
and now sharper: it is the *store*, not the reload, and specifically whether
as1 can be made to put that store in the call's delay slot where the target has
it, instead of the selector mask. Nine physical-line arrangements of the store
and the call were measured from the address-form base and every one is byte-flat
at 55: the two statements folded onto one line in both orders, one and two blank
lines between them, the store after the call, the store written volatile, and the
selector argument cast at the call. L59's line-number tie-break is live in this
unit but does not reach this pair.
#### 2026-09-12, lane `p9-tight`: the index block is not a colour question

The records were read on this function for the first time (instrumented object
`cmp`-identical to the configured one; this TU's ordinal is **proc 36**, the
37th `.ent` in the `cc -S` listing). The four index webs are identified by
forcing each to c9 and reading which sites move:

- **web 22** is `currentIndex` -- ours `a1`, the ROM's `v1`
- **web 25** is `previousIndex` -- ours `v1`, the ROM's `a0`
- **web 99** is the path-count carrier -- ours `a2`, the ROM's `a1`
- **web 44** is `nextIndex` -- ours `v0`, the ROM's `a0`
- **web 2** is the reloaded `index` parameter, and its `a2` already agrees

**`currentIndex` is denied `v1` outright.** Its `forbidden0` is `0x70000000` --
c1, c2, c3 -- and a force onto c2 reads `forced=-2`, never applied. The
decisive experiment is the one L114 asks for before calling a lever
unreachable: free the interferer first. Forcing webs 79, 49 and 25, every other
web holding `v1`, onto `t0`, `t1` and `a0` in the same run leaves
`currentIndex`'s mask **bit-for-bit unchanged** and the force still declined.
So the denial is not interference with a coloured rival, and no ratio,
spelling, statement order or force reaches the ROM's assignment. The live range
itself has to change.

That overturns this shard's framing. "The index block's pool colours" is not a
pool-priority question and the three previous records' plan -- reproduce the
ROM's `v1`/`a0`/`a1` triple by moving the assignments around -- cannot work on
this shape.

**A 36-point single-force sweep confirms it from the other side.** Twelve
integer webs against c2, c3 and c4: fifteen forces accepted, twenty-one
declined, and **not one scores below 21**. The best accepted alternatives are
24 and 26; `p1:w2=c2` costs 56 and `p1:w76=c4` costs 55 at delta -4. The
nearest joint force -- `previousIndex` to `a0`, count to `a1`, `nextIndex` to
`a0` -- reaches only **23**, worse than the retained body, because
`currentIndex` still refuses `v1`.

**L146 applied to the address-form route.** This shard records the four-pointer
declaration order as an identity, "the only order that lands both spilled stack
homes", measured over all 24. That climb was done on the `volatile u8` local
shape. Under L146 it is void on the address-form shape, which is the
structurally correct route for the byte spill, so it was re-climbed there: all
24 orders score **55 to 59**, the best two equal to the address form's own 55.
The address route does not become competitive under a fresh order climb, and
the retained hack at 21 keeps the body.

**Next lever:** a structural change to `currentIndex`'s live range, not a
colour. The mask to watch is web 22's `forbidden0`; anything that does not move
it cannot reach the ROM.

<!-- plateau-handoff:overlay1BendPathPoint:end -->
