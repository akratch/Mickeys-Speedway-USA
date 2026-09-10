<!-- plateau-handoff:levelInit:start -->
### `levelInit` plateau handoff

- source: `src/main/level.c`
- score: 22 differing words
- frame: 0x80
- relocations: 110
- first mismatch: +0x250
- summary: 113 words to 22, and the residual is now two named terms with every byte outside the resource loop exact. The 99-word term was never colour: it was a uniform three-step integer temp-ring phase, and the loop body needs three GP allocations more than the plain spelling makes. Two are bought by a redundant mask on an already-masked value (L65: it folds to no instruction and still pops the ring), and the third by the same fold inside the table index. The access spelling is the evidence-backed part: levelFreeAll, matched, reads this same table as *(s16 *)((i << 1) + (u32)base), and that form -- not the subscript -- gives the address add its target operand order. Removing any one of the three costs 87 words of tail phase, so they compose and none is an improvement alone. What is left is 16 words of the one colouring decision plus 6 in the table-index group. The colour is unchanged and now dominant: a full force sweep of this base (57 webs times c1 to c8, 258 applied) again has exactly one cell under it -- web 104, the s16 resourceId, forced from c1 to c5 -- and it scores 6, delta 0, with the whole function exact except that group. Its cost record says c1 and c2 both cost zero and only c3 and c4 are forbidden, by the loop's own argument setup, so reaching c5 needs interfering webs already coloured c1 AND c2, and all twelve of web 104's interferers are decided after it. The last 6 words are one allocation order: the target computes the index mask before it loads the table base, and every spelling of the access measured so far loads the base first. See the shard for the eliminated families.

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
<!-- plateau-handoff:levelInit:end -->
