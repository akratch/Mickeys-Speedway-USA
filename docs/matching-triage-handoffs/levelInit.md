<!-- plateau-handoff:levelInit:start -->
### `levelInit` plateau handoff

- source: `src/main/level.c`
- score: 22 differing words
- frame: 0x80
- relocations: 110
- first mismatch: +0x250
- summary: 113 words to 22; both terms are now closed against every lever this project has, with the interference structure measured rather than inferred. 16 words are web 104's colour (the s16 resourceId takes c1/v0, the target wants c5/a2, and forcing p1:w104=c5 scores 6 at delta 0). Its forbidden set is only c3 and c4 from the loop's own argument setup, so it needs two interfering caller-saved webs on c1 and c2. Adding them is barred by the measured register-pressure cliff -- any second dead expression is +120 bytes and 30 words across 54 cells -- and reordering existing ones is barred by the traced interference list: of web 104's twelve interferers exactly one holds a caller-saved colour, at save 2.5 against 26.7, and the other eleven are callee-saved because they live across a loop that calls. The span lever that closed overlay 86 does not apply either: web 104's p1cost list begins at colour 1, so v0 is offered and its span is already short. Naming the masked index or the store address to manufacture the missing webs fails a third way -- uopt re-materialises a cheap masked value at each use, leaving the loop body byte-identical and only paying the twelfth declaration's frame cell (24 words, 24, and 29 for both). The other 6 words are levelFreeAll's 3: exactly two ucode orders are reachable, shift-first mask/scale/table and base-first table/mask/scale, the target needs mask/table/scale, and uopt forward-substitutes any hoisted index local back into the address expression across block boundaries, L97 regions and condition commas alike.

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

<!-- plateau-handoff:levelInit:end -->
