<!-- plateau-handoff:levelFreeAll:start -->
### `levelFreeAll` plateau handoff

- source: `src/main/level.c`
- score: 114/117 words
- frame: 0x28
- relocations: 36
- first mismatch: +0x13C
- summary: Ring-index swap only; target carries the subscript signature with the index temp created before the base load, and cfe canonicalises every source form that would order it that way.

- 2026-09-09 pass, the residual reduced to one ring ordering. The three words
  are one three-temp allocation in the world-index arm of the model-release
  loop: a mask of the entry word, the table pointer's own load, and the scaled
  index. The target's ring order is mask, table, scale; the candidate's is
  mask, scale, table. Nothing else in the function differs and the frame,
  instruction count and relocations stay exact.
- The rule, measured on this site: the ring order is exactly the order of the
  surviving definitions inside the *expression*, and every spelling therefore
  lands in one of two classes. Writing the scaled index first gives mask,
  scale, table and 3 words; writing the table first gives table, mask, scale
  and 5. The target's order is neither, so no single-expression spelling can
  reach it.
- Eliminated this pass, each measured: reversed operand order on both the mask
  and the sum; multiply-by-two against a shift; byte-pointer, array-subscript,
  address-of-element and pointer-plus-index addressing; unsigned and 16-bit
  casts of the masked value; parenthesisation variants; splitting the mask into
  its own statement inside the arm; hoisting that statement to the top of the
  loop body, before the whole if-chain; a nested block scope for it; caching
  the table pointer in a local (8 words, the documented penalty for caching a
  twice-spelled global); and a comma expression that mentions the mask before
  the table, which does *not* move the order because the dead first mention is
  eliminated before web numbering.
- That last one refines the carrier-colour law: mentioning a value earlier only
  earns it an earlier colour when the earlier mention survives. A dead mention
  buys nothing.
- Next lever: the target's order implies the table pointer's address web is
  created between the mask and the scale, which no arrangement of this one
  expression produces. Look for a second surviving reference to the table
  elsewhere in the function, or for the arm's source being a call whose
  argument list, not an address expression, orders the three.

#### 2026-09-09, lane fin-near: subscript order is normalised by cfe

The reversed subscript `(index)[table]`, which in principle walks the index
before the table, and every other subscript or pointer-sum spelling of the
world-index arm (`table[index]`, `*(index + table)`, `*(table + index)`, with
and without the `s16 *` cast) all give table, mask, scale at 5 words. cfe
normalises the operand order of a subscript before uopt numbers the ring, so
the mask-table-scale order cannot come from a subscript. Baseline 3 retained.

#### 2026-09-09, lane fin-misc: the residual is a two-slot ring swap, and the
target carries the subscript signature

Read out exactly against the real object with a 64 ms direct-`tools/ido/cc`
loop (byte-identical in `.text` to the asm-processor `NON_MATCHING` object at
the flags recovered from `gmake -n build/src/main/level.c.o`), which made this
pass exhaustive rather than sampled -- about 800 scored candidates.

The three words are a swap of two adjacent ring slots and nothing else. All
three classes put the same instructions at the same positions; only the
register names move, so this is post-uopt emit order, not schedule:

- target: mask t3, table t4, scale t5, sum t6
- manual shift, `(m << 1) + (u32) table`: mask t3, table t5, scale t4, sum t6
- subscript, `table[m]`: mask t4, table t3, scale t5, sum t6

The scale is created *third* in both the target and the subscript class and
*second* in the manual class, so the target carries the subscript lowering's
signature with the index temp created before the base load. cfe evaluates a
subscript's base before its index, and canonicalises `int + ptr` to `ptr + int`
before web numbering -- which is why integer-left pointer arithmetic
(`*(m + table)`) and the reversed subscript `(m)[table]` land in the base-first
class too, and why no spelling of this one expression reaches the target.

Newly eliminated this pass, each measured:

- The local prototype of `func_80004B04` (`s16`, `s32`, `u16`, no prototype).
  Flat; `u16` costs one extra word at the call. The TU-local prototype is
  `s16` while `objects.c` defines it `s32`, so this was a real axis, not a
  formality.
- **192 physical line splits** of the arm across every token boundary in four
  address spellings -- completely flat. The line-key lever (L87) that has
  closed other functions does not reach this site.
- A **surviving** comma first operand -- `(masked = temp_v0_2 & 0x3FFF, ...)`
  inside and outside the subscript, and an embedded assignment in the index.
  All fold back. This is the lever that closed `func_8003A2C8` the same day,
  so its failure here is informative: the earlier note that only a *dead*
  first mention fails is too weak -- a surviving one fails too.
- Use-side lock breaks `^ 0`, `- 0`, `+ 0`, `* 1` and a zero bitwise-or on the index.
- Two-statement pointer-local forms (`entry = &D_800C94E0[m]`, `entry =
  D_800C94E0` then `entry[m]`): 8 words.
- `sizeof(s16)` scaling, `(u8 *)`/`(s32)`/`(u32)` base casts,
  `*(s16 **) &D_800C94E0`, `((s16 (*)[1]) D_800C94E0)[m][0]`, a bitwise-or in place of the sum
  (4 words, and the `or` is wrong), and `(u16)` on the subject.
- Restructuring the third `else if` into a nested `else` with the mask hoisted
  above the inner `if`, and hoisting it to the top of the loop body, crossed
  with all four address spellings.

Tooling: **the permuter is not usable on this function.** Its scratch reports
`base score = 20` against the real object's 3 -- a 6.7x disagreement -- and it
exits without producing a candidate. Recorded under the "permuter scores do not
transfer" blind spot.

Donor: JFG's `levelFreeAll` has no model-release loop, and none of the five
permitted decomps contains the `0x3FFF` resource-tag idiom. The matched sibling
`levelInit` in this same TU spells it `D_800C94E0[resourceId & 0x3FFF]`, which
compiles into the base-first class here -- so the natural spelling is not the
answer at this site.

Next lever: the only remaining route to {mask, table, scale} is a subscript
whose index temp is created before the base load, and cfe canonicalises every
source form that would order it that way. So the reason must come from outside
the expression -- a second, partially dead reference to `D_800C94E0` that uopt
sinks into this arm (which would create its web late), or evidence that the
base is not this global at all.

#### 2026-09-10, lane p1-perm: the free-list hypothesis is closed

The only two ways a ring residual can arise are a different within-expression
emission order and a different free list reaching the site. The second is now
ruled out. Reading the register sequence of the loop body on both sides: the
head and the if-chain draw t7, t8, t9, t0, t1 and t2 in that order before the
arm, and the arm then draws four **fresh** registers t3, t4, t5, t6. Nothing is
recycled, so the list reaching the arm is plain cycle order on both sides and
the three words are entirely ugen's order inside the one expression. No
perturbation of earlier frees can reach the target, which removes the whole
class of "spend a temp earlier" experiments.

That leaves the recorded impossibility: the target's creation order is mask,
table, scale; cfe evaluates a `+` strictly left to right and canonicalises
`int + ptr` to `ptr + int`, so a two-operand address expression gives either
mask, scale, table (index-first) or table, mask, scale (base-first) and never
mask, table, scale.

Also newly measured and flat at three words: **256 line-grouping cells** --
every subset of the four arms with its body joined onto its own `if`/`else`
line, crossed with joining `temp_v0_2 = D_8007A0F4[i];` onto the first `if`
line, joining `D_800CF490[i] = NULL;` onto the closing brace line, and four
address spellings. This is a different space from the recorded 192 line
*splits* of the arm, and it is equally inert; the L87 line-key lever does not
reach this site from either direction.

#### 2026-09-10, lane c2-resident: the expression space is closed, exhaustively

Still 3, and the residual is confirmed unchanged: the mask, the table load and
the scale occupy the same three positions on both sides and only their creation
order differs, so this is uopt/ugen emission order and nothing else.

The candidate creates the mask first, then the scale, then the table; the
base-first class creates the table first, then the mask, then the scale; the
target creates the mask first, then the *table*, then the scale. Reading the
compiler's own listing (`cc -S` at the TU's flags) makes the three classes
visible directly, and it is a faster instrument than the score for this
residual: the class is legible in one line, before the assembler's scheduling
hides it.

Newly measured and eliminated this pass, about **1,030 further candidates**, all
of them landing in the same two classes (3 or 5) and none below:

- A **960-cell cross product**: six base spellings (`(u32)`, `(s32)`, `(u8 *)`,
  a double-dereference through the symbol's own address, an address-of-element,
  and the raw pointer) x fifteen index spellings (plain, four integer casts,
  five arithmetic-identity locks, an unsigned constant, a complement of the
  inverse mask, and a shift-pair sign extension) x six scale spellings (shift,
  multiply, `sizeof`, two casts of the shifted value, and self-addition) x both
  written operand orders, plus every subscript, reversed-subscript and
  pointer-sum form of each index spelling.
- **Compound and nested address shapes**: `*&a[i]`, `(&a[i])[0]`, `(&a[0])[i]`,
  a `char *` base, a re-cast base, three-term sums with an added zero in every
  position, a double negation of the sum, an array-typed pointer cast, and a
  `volatile`-qualified read of the base pointer (the last is +1 instruction).
- **Two-statement forms**, re-measured against the compiler listing rather than
  the score: a hoisted `s32`/`u32` index folds back into the base-first
  expression with an *identical compiler listing* over the whole arm, so
  hoisting is not a near miss but an exact no-op, and `register` on the index
  does not change that. `s16` and `u16` index locals cost 8 and 0 bytes respectively and both
  regress.

What this pass adds beyond the elimination: the target's order requires the
mask to be a ring temporary that is *emitted before the address expression
begins*, while remaining single-use. Every construct that emits it early
(a second surviving use, a comma, an assignment inside the index) makes it a
uopt symbol instead, which takes a pool colour rather than a ring number, and
every construct that keeps it a ring temporary emits it inside the expression.
Confirmed on the self-addition spelling, whose listing shows exactly the
target's *order* with the mask on a pool colour and an add where the target has
a shift. So the remaining question is sharper than "which spelling": what makes
IDO emit a single-use masked value as its own ring temporary ahead of the
expression that consumes it. A scan of every matched `main` TU's compiler
listing for the target's signature found five instances of "index materialised
before the base load", and in all five the index was an *already-live variable*
(a loop counter or a parameter), never a mask computed at the site.
<!-- plateau-handoff:levelFreeAll:end -->
