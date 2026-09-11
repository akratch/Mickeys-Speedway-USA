<!-- plateau-handoff:levelInit:start -->
### `levelInit` plateau handoff

- source: `src/main/level.c`
- score: 6 differing words
- frame: 0x80
- relocations: 110
- first mismatch: +0x328
- summary: 2026-09-11, lane f9-audit: 22 to 6 at delta 0. The 16-word colour term was never a colouring problem. uopt forms one web per SYMBOL (f_intfering is a bit-vector intersection of live BLOCKS, and a symbol's live blocks are the union over all its uses), so carrying the resource id in the existing `shouldPlay` local -- a2 already, decided seventh at save 40, live beside the v0/v1 tune-loop temps -- forbids c1 and c2 for the loop value with no new web and no frame change; the address web falls to a3 behind it exactly as predicted. The earlier closure held the CARRIER IDENTITY fixed while it varied everything around it: a six-cell sweep over the existing locals finds it (shouldPlay 6, tune 17, lvlCount 30, j 31, freeSlot and i move the frame). The remaining 6 words are the levelFreeAll order term (target pops mask, table, scale, sum, then one phantom; candidate pops phantom, mask, scale, table, sum), re-measured flat across 32 spellings on the new base, including index-first pointer adds and named-mask carriers in every existing local.

#### 2026-09-10, lane c4-resident: 113 to 22, and the ring term closed

The previous handoff read the whole 113 as a colouring plateau. It was not.
Two of the three residual terms are now closed and the function is byte-exact
everywhere outside the resource loop.

**How the split was found.** `CDX_FORCE` over every web this procedure decides
(57 webs x c1..c8, 258 of 456 forces applied) has exactly one cell under the
base, and it is worth 14 words, not 113. Web numbering moved since the earlier
receipt -- the c3 experiment ran against a variant carrying an extra `mode`
carrier, so its "web 105" is web **104** here. With that colour forced the
remaining 99 words were a single uniform three-step rotation of the integer temp
ring, unbroken from `+0x0358` to the end of the function: no instruction,
immediate or operand differed anywhere in the tail.

**How the ring term was located.** `DKWB_UGEN_TRACE=1` on the instrumented `cc`
emits one free-list record per allocation carrying ugen's own source line. In
proc 8 the loop body allocates ten GP temps, and each is freed one instruction
after its pop, so the list is a strict rotation. A replay of the whole recorded
event stream reproduces all 70 of the procedure's allocations with zero
mismatches, which makes the model an oracle rather than a hypothesis; run it
before guessing at any ring residual in this function. Under it the target's
register usage is only reachable at thirteen allocations, so the target makes
three the plain spelling does not.

**What buys them.** A redundant mask on an already-masked value -- `(x & M) & M`
-- folds to no instruction and still costs one pop ([L65]). It is free only where
the value lands in a ring temp: at the two later masked tests and inside the
table index it is delta 0, and at a call-argument site, where the value goes
straight to `a0`, the outer mask emits and costs 8 bytes. Three of them take
113 to 38.

**The evidence-backed half.** `levelFreeAll`, matched, in this same file, reads
the same table as `*(s16 *)(((x & 0x3FFF) << 1) + (u32) D_800C94E0)`. Adopting
that spelling in `levelInit` takes 38 to 25 and gives the address add the
target's own operand order; moving the third phantom inside its index takes it
to 22. So the subscript form is what was wrong, and the sibling function was
carrying the answer the whole time.

**What is left, and it is small.** 16 words are web 104's colour and 6 are one
allocation order. Forcing `p1:w104=c5` on the current source scores **6, delta
0**, with the entire function exact except the table-index group -- so the
colour is now the dominant term and it is fully characterised: its forbidden set
is only c3 and c4, contributed by the loop's own argument setup; c1 and c2 both
cost zero; globalcolor keeps the first strict minimum; and all twelve of web
104's interferers are decided *after* it, so reaching c5 needs two earlier
interfering webs, coloured c1 and c2. The last 6 words are one order: the target
computes the index mask, then loads the table base, then shifts, then adds base
plus scaled. Every access spelling measured loads the base first.

Eliminated, delta 0 unless noted:

- Nine loop respellings (re-reading the array in every test, `u16`/`s32`/`u32`
  typing of the id, a masked-index local, a `switch` on the top bits, explicit
  `!= 0` tests, unsigned mask constants, a pointer cursor, a function-scope
  declaration). Flat or worse; two move the frame.
- A 61-cell narrow-prototype product over the four callees. Every narrowing that
  touches a masked-argument call costs 8 bytes or more; `func_8000486C` alone is
  byte-flat.
- 27 argument spellings at the first call site; eighteen byte-identical. All-ones
  masks (`& ~0`, `& -1`, `& 0xFFFFFFFF`) fold at the front end and buy no pop --
  only an idempotent narrowing mask survives to ugen.
- 46 call-result staging shapes: 2 words and no pop, so L76 does not hold for a
  call result coalesced into `v0`.
- 92 explicit-pointer store forms (`*(void **)((off << 2) + (u32) D_800CF490)`):
  24 bytes or more; the store's induction pointer is not the lever.
- 72 natural alternatives to the doubled test masks (`== 0x8000`, a wider mask
  narrowed, a shift form, unsigned constants): none buys the pop, so the doubled
  masks stand in for whatever the original wrote there.
- A 65-cell product over the test masks and the access forms: 22 is the floor.
- Global type declarations, loop-control decorations and index decorations: flat.

Next lever, in order of value: the c1-and-c2 interference pair for web 104, worth
16; and an access spelling that computes the index before it loads the base,
worth 6.

#### 2026-09-10, lane c5-resident: the reservation lever is retired, and the 6 are levelFreeAll's 3

Nothing moved the score. What moved is what is left to try, in both terms.

**The colour term (16 words) cannot be bought, and the cliff is the reason.**
The next lever named by the previous pass was the documented one from
`docs/ido-learnings.md`: a caller-saved colour is won by making the lower
colours unavailable, and a *tested dead expression* with two references is how
you do it. On this function it does not work, and the failure is uniform rather
than fiddly. One `if (E);` anywhere is byte-identical. **Any second one costs
exactly +120 bytes and 30 words**, and the number does not move:

- nine expressions (the induction variable, the loop bound, the id, three of
  the loop's own masked tests, the comparison result, the two array reads)
  crossed with five position sets inside the loop body, all delta +120 to +132;
- both references in one basic block, and three references in one block: same;
- and the control that settles it -- two dead tests placed at the very top of
  the function, on a parameter, far outside the loop -- also +120.

54 cells, one number. Reading the traced `globalcolor` says why: in the base,
the marginal web (save 1.18, 45 interferers) takes the last callee-saved colour
`s6` with `decision=color`; with any extra web present it becomes
`decision=split` and spills, and the 30 words are its reloads. So this
procedure is at a register-pressure cliff, **no new web can be added to it for
less than 30 words**, and since the only reachable fix for web 104's colour is
one more interfering caller-saved web (globalcolor keeps the first strict
minimum, and priority cannot move a colour), the whole reservation family --
dead tests, dead stores, comma carriers, extra locals -- is retired here. A
route to the 16 words must first *remove* a web, not add one.

`if (1) { }` and `do { } while (0)` region boundaries were measured too, since
[L97] is the other way to redraw an allocation: around the resource loop's
if/else chain they are byte-identical, and around the loop body they cost the
same 30 words and leave the id on `v0`. Region boundaries do not move this
colour.

**The order term (6 words) is levelFreeAll's residual, measured, not inferred.**
Both functions read `D_800C94E0` in the same file and both are left with one
emission-order swap of the same three temps. The candidate creates the index
mask, then the scale, then the table load; the target creates the mask, then
the table, then the scale. In `levelFreeAll` that is the whole remaining 3
words; in `levelInit` it is the whole remaining 6.

The ring accounting is now exact for the `levelInit` arm. A free-list replay of
the instrumented `ugen` trace shows the loop body is a strict rotation, and the
arm makes **five** ring allocations, not four: the doubled index mask spends one
that emits nothing. The candidate's order is phantom, mask, scale, table, sum;
the target's registers force mask, table, scale, sum, phantom -- the phantom
moves to the end. Eleven access spellings were read off the object rather than
guessed, and they fall into exactly two classes, neither of them the target's:

- shift-first (`(m << 1) + base`, the retained form, and the `* 2` variant):
  mask, scale, table -- 22, the floor;
- base-first (`base + (m << 1)`), the plain subscript, the reversed subscript,
  `*(p + i)`, a `u8 *` base, and the outer-fold and comma variants of each:
  table, mask, scale -- 22 core words plus 22 more of tail phase.

`m + m` for the doubling, which is what produced the target's order in
`levelFreeAll`, puts the mask on a pool colour here instead of a ring temp and
is 111. So the two functions want the same thing and the same spelling space is
exhausted for both. Whoever solves one should re-measure the other the same
hour.

#### 2026-09-10, lane w8-bigclose: this procedure has no p2 phase, so definition order is a dead axis

The prior passes closed the 16-word colour term against reservation and against
reordering. This pass adds the reason a whole *third* family of levers -- moving
a defining statement, which [L106] makes a colour lever elsewhere -- cannot
apply here at all, and it is the cheapest question on the page ([L108]).

An instrumented `uopt` (CDX log; object byte-identical to the tree's, which is
the identity gate) records **1655 p1 records for this procedure and zero p2**.
`level.c` as a whole does emit 70 p2 records, but every one of them belongs to
one of the file's small procedures. So the caller-saved sweep that colours in
ascending web number never runs on `levelInit`, and `save = totalsave/nocs` is
the only thing that orders anything. Definition position, declaration order and
statement order are one dead axis here; ask the phase question before spending a
pass on any of them.

The trace confirms every figure already recorded and adds why the two terms move
together. Web 104 is `save` 26.666666 (`nocs` 3, `totalsave` 80) at cost 0 and is
decided **twelfth** of this procedure's decisions; of the eleven decided before
it only three interfere, and their colours are exactly the pair the closure
names, which is why its forbidden set is that pair and it takes the first free
colour. The web carrying the resource table's address is decided far later, at
`save` 2.0, which is why it currently holds the colour the target gives web 104:
fix web 104 and the address web finds that colour taken and falls one further --
the target's pair exactly. So the 16 words are one decision, and the two
interfering caller-saved webs it needs have to be decided **within the first
eleven**, which is a constraint on their `save`, not on their position.

#### 2026-09-11, lane f9-audit: the closure held the carrier identity fixed, 22 -> 6

The 16-word colour term is closed by an edit the three previous passes could
not reach, because every one of them assumed the loop value was a web of its
own. It is not, and the reason is a mechanism worth carrying to every other
"needs an interfering web I cannot add" residual.

**The mechanism, read from uopt itself.** `f_intfering` in the recompiled
`uopt` is a bit-vector intersection of two live ranges' *block* sets (the
`lineage_member` records are exactly that set: web 104 is live in seven
blocks, the loop header and the six pre-call arm blocks -- uopt splits a block
at a call, which is why the call result on v0 never forbids it). A live range
is formed per **symbol**: two def-use chains of one local are one web, and
its block set is the union. Two earlier probes on this page were read as
"byte-identical, so no effect" when they were in fact consistent with exactly
that: routing the switch value through the same s16 (byte-identical) and
carrying the bound loop's load in it (126 words, the loop value still on v0)
both put a *v0* chain into the union, which forbids nothing new.

**The lever.** `shouldPlay` is web 217: a2, save 40, decided seventh, and live
in the first tune loop's blocks beside the v0/v1 temps 223 and 224 that are
decided first. Carrying the resource id in it instead of in `resourceId`
(`shouldPlay = D_8007A0F4[off];` and the same name in the four tests) gives
the loop value a web whose forbidden set is c1..c4 -- c1/c2 from the tune
loop, c3/c4 from the arm argument setup as before -- so it takes a2, and the
address web finds a2 taken and falls to a3. That is the pair the closure
predicted, reached with no new web, no dead expression, no frame change and
no region boundary. 22 -> 6, size delta 0, all 110 relocations unchanged.

The carrier is unique among the existing locals, measured with the s16 cell
kept declared: `shouldPlay` 6, `tune` 17, `lvlCount` 30, `j` 31, `freeSlot`
+16 bytes, `i` +32 bytes. `arg1`/`arg2` as carriers (a parameter is
precoloured only on its entry chain) 504 and 31. A fresh local cannot do this,
which is why the reservation family was measured flat: it has no second use
to union with. So the variable the closure held fixed was the **carrier
identity** -- it varied spelling, reservation, region, declaration order and
statement position around a fixed set of symbols.

`s16 resourceId` stays declared and unread. The frame is `0x80` only with a
tenth cell, and the object does not say which local the original spent it on.

**The remaining 6 words are the levelFreeAll order term and they did not
move.** On the new base the target pops mask (t0), table (t1), scale (t2),
sum (t3) and one phantom (t4) inside the table-read arm; the candidate pops
phantom, mask, scale, table, sum. Re-measured flat, delta 0 unless noted:
the 20-form access-spelling lattice (plain, doubled and cast subscripts,
`*(p + i)`, `u8 *` and `u32` bases, shift-first and base-first, `* 2`),
index-first pointer adds (`*((m & 0x3FFF) + D_800C94E0)` and
`(m & 0x3FFF)[D_800C94E0]`, both base-first in the listing, 88), post-sum
phantoms (`+ 0`, `(s32)(s16)`, an `& 0xFFFFFFFF` on the address, `volatile`),
and the masked index named in `j`, `i`, `lvlCount` or `freeSlot` (88, 88, 88,
and the frame). Every base-first form is 28 (6 plus 22 words of tail phase,
one pop short); every shift-first form is 88 or 92. Nothing on this page
emits the mask before the table load without also emitting the shift before
it, so the next lever for the 6 is whatever cfe construct yields an index-add
whose index is evaluated first -- not another spelling of this one.

Whoever solves levelFreeAll's 3 solves these 6 the same hour; the carrier
finding does not transfer there (its residual is the order term alone).
**Second bounded round on the 6, same day: the mask's own carrier is not the
lever.** The shouldPlay mechanism was applied to the mask itself -- the masked
index computed once at the top of the loop body into an existing local with
other live ranges (`j`, `lvlCount`, `i`; `tune` and `freeSlot` move the
frame), with all four arms reading it and with only the table arm reading
it, shift-first and subscript spellings of the arm. Every delta-0 cell is 88
or 92, and the `cc -S` listing shows why: copy propagation substitutes the
named mask back into the address expression in every arm before live ranges
are formed, so the symbol's other uses never enter it and it never becomes a
web. L102 therefore holds for a shared symbol too, not only for a fresh one:
the carrier trick reaches a value only when it survives copy propagation
(the loop value does, because it has four distinct reads; a single-use mask
does not). Conversions on the loaded value (`(s16)`, `(s32)(s16)`, `(u16)`,
`*(u16 *)`), and `+ 0` or a bitwise-or with zero on the address after the
sum, are all exactly 6 (two of them with an lhu opcode difference the masked
count does not price); `(u8 *)` bases 28 and 88. The target's load is lh and
func_8000486C takes s32, so no conversion node is missing. levelFreeAll
re-scored at 3.

<!-- plateau-handoff:levelInit:end -->
