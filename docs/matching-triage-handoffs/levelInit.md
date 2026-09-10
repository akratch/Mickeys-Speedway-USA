<!-- plateau-handoff:levelInit:start -->
### `levelInit` plateau handoff

- source: `src/main/level.c`
- score: 113 differing words
- frame: 0x80
- relocations: 110
- first mismatch: +0x238
- summary: The 113 words are two independent terms and only the smaller one is a colouring decision. A full force sweep of this tree -- 57 uopt webs times colours c1 to c8, 258 forces applied -- puts exactly one result under the base: web 104, the s16 resourceId of the resource loop, forced from c1 to c5, scores 99 at delta 0. Its cost record says why no spelling reaches it: only c3 and c4 are forbidden, by the loop's own argument setup, c1 and c2 both cost zero, and globalcolor takes the first strict minimum, so c5 needs interfering webs already coloured c1 AND c2, and all twelve of web 104's interferers are decided after it. So the lever there is two earlier interfering webs, not one, and it is worth 14 words. The other 99 are not colour at all: with that colour forced they are a uniform three-step integer temp-ring phase running unbroken from +0x0358 to the end. Source-line-stamped ugen free-list records (DKWB_UGEN_TRACE, proc 8) show the loop body allocating exactly ten GP temps -- two at line 425, one each at 426, 428 and 430, four at 431, one at the line 422 test -- each freed one instruction after its pop, so the free list is a strict rotation and the head advances by the allocation count alone. The target's body reaches two ring registers the candidate never pops and reuses two others, which forces thirteen allocations: three of the target's emit no instruction, at rotation positions 4, 6 and 12, immediately after the three argument sites whose whole argument is the masked id, lines 427, 429 and 433. So the decision variable is three zero-instruction GP allocations at those argument sites, worth the 88 tail words. See the shard for the eliminated families.

#### 2026-09-10, lane c4-resident: the residual split, and the colour search closed

The previous handoff read the whole 113 as a colouring plateau. It is not. Two
measurements separate it:

- **The colour is worth 14 words and no more.** `CDX_FORCE` over every web this
  procedure decides (57 webs x c1..c8, 258 of 456 forces actually applied) has
  exactly one cell under the base: `p1:w104=c5` scores 99, delta 0. Everything
  else ties 113 or regresses. Web numbering moved since the earlier receipt --
  the c3 experiment ran against a variant carrying an extra `mode` carrier, and
  its "web 105" is web **104** on the committed tree.
- **The other 99 words are one ring phase.** With the colour forced, every
  remaining word from `+0x0358` to the end of the function is the same
  three-step rotation of the integer temp ring; no instruction, immediate or
  operand differs anywhere in the tail.

Why the colour is not spelling-reachable, from its own cost record: web 104's
forbidden set at decision time is only c3 and c4, contributed by the loop's own
argument setup, not by any coloured web. c1 and c2 both cost zero, and
globalcolor keeps the first strict minimum, so c1 wins. Reaching c5 needs
interfering webs already coloured c1 **and** c2 -- two, not one -- and all
twelve of web 104's interferers are decided *after* it in the save ordering, so
none of them can contribute a forbidden bit. That is the shape of the lever.

The ring term is the bigger prize and is now localised to three source sites.
`DKWB_UGEN_TRACE` on the instrumented `cc` emits one free-list record per
allocation carrying ugen's own source line. In proc 8 the loop body allocates
ten GP temps: two at the array read, one at each of the three masked tests, four
at the `D_800C94E0` branch, one at the loop test. Each is freed one instruction
after it is popped, so the list is a strict rotation and the head advances by
the allocation count alone -- which is what makes the target's count readable
off its register usage. The target's body never reaches two of the registers the
candidate pops and reuses two others, which is only possible at thirteen
allocations. Three of the target's therefore emit nothing, at rotation positions
4, 6 and 12: after each of the three call bodies whose entire argument is the
masked id. The fourth body's argument is an array element, and it allocates the
same four temps in both.

Eliminated at the 113 base, delta 0 unless noted:

- Nine loop respellings: re-reading the array in every test, `u16`/`s32`/`u32`
  typing of the id, a masked-index local, a `switch` on the top bits, explicit
  `!= 0` tests, unsigned mask constants, a pointer cursor, and a function-scope
  declaration. Flat at 113 or worse; the re-read and function-scope forms move
  the frame.
- A 61-cell narrow-prototype product: `s16`/`u16`/`u8`/`s8` first parameters
  over all subsets of the four callees. Every narrowing that touches the three
  masked-argument calls costs 8 bytes or more; `func_8000486C` alone is
  byte-flat. So the phantom allocation is not a parameter-width conversion.
- 27 argument spellings at the first call site, eighteen of them byte-identical
  (`& 0x3FFFU`, `(u32)`, `(s32)`, reversed operands, `* 1`, `^ 0`, `- 0`,
  `>> 0`, `<< 0`, unary `+`, a redundant zero disjunction, and the same decorations
  moved onto the id).
  Every one that does advance the ring also emits an instruction.
- 46 call-result staging shapes (`res = func(...); D_800CF490[off] = res;` over
  all subsets of the four bodies, block and loop scope). All cost 2 words and
  buy no pop, so L76's "naming a value costs a ring pop" does not hold for a
  call result coalesced into `v0`.
- Seven loop-control and eighteen index decorations (`+ 0`, `(s32)`, `* 1`,
  `*(base + i)`, `*(&a[i])`, `i += 1`, `i = i + 1`, an explicit `s16 *` cast):
  all byte-identical.
- Global type declarations: destination table as `s32`/`u32`/`void **`, id array
  as `u16`, map array as `u16`/`s32`, count as `u32`/`s16`, index as
  `u32`/`s16`. Flat or worse.

Constructs that *do* advance the ring, none free: `((resourceId >> 14) & 3) == 3`
buys one pop at delta 0 but trades the constant-compare pair for a shift pair, so
it is instruction-different; `(off < D_800CF508) != 0` buys two pops for one
instruction; `resourceId = D_8007A0F4[off] & 0xFFFF` buys three for three; the
shift form of the second and third tests costs 4 to 8 bytes each.

Next lever: one construct at a call-argument site that allocates a GP temp and
emits nothing. The redundant-mask fold of L65 needs a narrow *destination* and
there is none at these sites, so the fold has to come from somewhere else --
worth reading other functions' traces for a line whose allocation count exceeds
its instruction count, rather than guessing more spellings here.
<!-- plateau-handoff:levelInit:end -->
