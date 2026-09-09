<!-- plateau-handoff:levelFreeAll:start -->
### `levelFreeAll` plateau handoff

- source: `src/main/level.c`
- score: 114/117 words
- frame: 0x28
- relocations: 36
- first mismatch: +0x13C
- summary: Needs ring order mask, global, shift; every single-statement spelling gives mask/shift/global or global/mask/shift and uopt normalises statement splits.

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
<!-- plateau-handoff:levelFreeAll:end -->
