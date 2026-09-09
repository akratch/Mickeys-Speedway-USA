<!-- plateau-handoff:overlay1ResolvePathPoint:start -->
### `overlay1ResolvePathPoint` plateau handoff

- source: `src/overlays/o001/overlay_001_end.c`
- score: 151/152 words
- frame: 0x78
- relocations: 22
- first mismatch: +0x6C
- summary: One word: the else-branch base reads the raw call-result web (v0) where the target reads the saved copy (s4); the doubled product is closed


#### 2026-09-09, lane win-b: 8 -> 3 words, three named mechanisms

Measured against the full-TU object with a direct `tools/ido/cc` compile
(byte-identical in `.text` to the asm-processor NON_MATCHING object at this
TU's real flags -- note `overlay_001_end.c` does **not** carry
`-Wab,-r4300_mul`, unlike `overlay_001_tail.c`). Three independent residual
groups, closed in order:

1. **The m2c `register u32 groupAddress` carrier was the two relocation-order
   words at +0x090/+0x094.** Replacing `groupAddress = (u32)&D_1D88;
   *(s32 *)groupAddress = 0x3F;` and `(*(s32 *)groupAddress)--;` with plain
   `D_1D88 = 0x3F;` and `D_1D88--;` makes the twice-spelled global materialise
   its own address into the pool colour and interleaves the `%hi` of D_1D88
   between the `lui $at` and the `sw` exactly as the target does. Two words.
   Physical line grouping is **inert** here: all 32 groupings of the six head
   statements produce a byte-identical object, so the fx.c `%lo`-ordering
   lever does not apply to this block.
2. **The declaration census moved `result` when the carrier was dropped.**
   Losing one 4-byte local put `result` at 0x64 instead of 0x60 while the frame
   stayed 0x78, costing the `addiu $s3, $sp, ...` and the `lh ..., ...($sp)`.
   All 720 orders of the six locals were scored: the ones that put `result`
   fifth are exact, and `index, record, point, scanIndex, result, product` is
   the adopted order.
3. **The induction pointer's first textual encounter decides which `record` web
   the loop-entry pointer draws from.** With `point = &record->x[2];` before
   the `do`/`while` and `point++` inside, uopt feeds the loop-entry pointer
   from the raw call-result web (`$v0`) and as1 schedules it two rows early.
   Writing the loop as `do { point = &record->x[scanIndex]; ... scanIndex++; }
   while (scanIndex < record->count);` -- semantically identical, and strength
   reduction rebuilds the same `addiu $s0, $s0, 2` induction step -- moves the
   first encounter inside the loop, puts the loop-entry pointer on the saved
   web (`$s4`), and restores the +0x148/+0x14C/+0x150 ordering. Four words
   became one.

Residual, both `allocation-mismatch`:

- **+0x124**, the `else`-branch `point`: target `addiu $s0, $s4, 0x4`, we emit
  `addiu $s0, $v0, 0x4`. uopt copy-propagates the raw call-result web into the
  `$36` block because nothing redefines `$2` on the path from the call to it,
  while the in-loop copy is forced onto `$s4` by the call inside the loop.
  Exhausted: six spellings of the base (`&record->x[2]`, `record->x + 2`,
  `&record->x[index]`, `(s16 *)((u8 *)record + 4)`, `&record->x[0] + 2`,
  `&record[0].x[2]`) crossed with six spellings of the pre-`if` assignment, an
  `index = record->count` carrier, an empty `else`, an `else` that re-uses
  `scanIndex`, a `record->count < 2` inversion, and a second
  `Overlay1PoolRecord *` alias -- all 720 declaration orders each. Floor 3.
- **+0x1C8/+0x1CC**, the doubled product: target `addu $v1, $v0, $v0;
  sltiu $at, $v1, 0x11`, we emit `addu $v0, $v0, $v0`. The `cc -S` listing
  shows why the space is only two-valued: `product = product + product;`
  assigns back to the same ugen expression register and emits `addu $2,$2,$2`,
  while every form that makes the doubled value a distinct value
  (`if ((u32)(product + product) >= ...)`, `product * 2`, `product << 1`, a
  second declared local, `u32 product`) demotes it to a **ring** temp
  (`addu $9,$2,$2`) and shifts every later ring temp by one, costing 11 more
  words. `$v1` is ugen's second statement-result register, taken only when
  `$2` still holds a live statement value; no source form reached it.

Next lever for both sites is a uopt web-split observation (CDX/`p1dec`), not
another source spelling: the question is what keeps the raw call-result web
from reaching the `$36` block.

#### 2026-09-10, lane p1-perm: 3 -> 1 word, the doubled product is closed

Measured with a direct `tools/ido/cc` full-TU compile, confirmed byte-identical
in `.text` to `build_non_matching/src/overlays/o001/overlay_001_end.c.o` and
cross-checked against `tools/wb_compare.sh --summary-json`, which now reports
`differing_words: 1`, `matched_words: 151`, `register_mismatches: 1`,
`opcode_mismatches: 0`, `instruction_delta: 0`. About 190 scored candidates a
second at eight workers.

**Scoring note that changed the reading of this function.** Six of the nine raw
differing rows are splat rendering an overlay-internal address as a bare
`lui`/`%lo` literal with no relocation where the C build emits HI16/LO16
against the extern. They link to the same bytes and the workbench normalises
them; a naive word diff counts them and reports nine. Any comparison on this
overlay must tolerate exactly that pairing -- one side relocated, the other a
literal -- and nothing else.

**The doubled product (+0x1C8/+0x1CC) is closed.** The target computes the
product into ugen's first statement register and the doubling into its second
(`mflo $v0; addu $v1, $v0, $v0; sltiu $at, $v1, 17`). Spelling the product
expression **twice** and letting uopt fold the two into one multiply reaches
that exactly:

    product = ((result[0] - point[0]) * (result[0] - point[32])) +
              ((result[0] - point[0]) * (result[0] - point[32]));

The earlier note that "no source form reached `$v1`" was measuring the wrong
axis. `product = product + product` writes back in place because the
destination is already `product`'s pool colour `$v0`; every form that makes the
doubled value a *fresh* value (`product * 2`, `product << 1`, an inline
`(product + product)` in the test, a seventh declared local at any of seven
positions and seven types) demotes it to a ugen ring temp. A repeated
*subexpression*, folded by CSE, is neither: it leaves the product live as a
statement value, so ugen takes its second statement register for the sum. This
is a new form of the "repeat the read" lever -- repeated for its **statement
register**, not for a copy or a ring slot.

A seventh declared local is separately excluded: the local block is already at
its 8-byte step, so any seventh scalar grows the frame from 0x78 to 0x80 and
costs 24 words in all 49 position x type cells.

**Remaining: one word at +0x124**, `addiu $s0, $s4, 4` against
`addiu $s0, $v0, 4`. This is the pre-branch materialisation of the else
branch's `point`. `cc -S` reads the cause directly: in the `$36` block ugen
emits `addu $16, $2, 4` while the loop's own setup two blocks earlier emits
`addu $16, $20, 4` from the same source expression. `record` has one definition
and uopt splits it at the call into a raw `$v0` web and a saved `$s4` copy
(`move $20, $2` in the branch-delay slot); in the target the raw web's last use
is the `lbu $v1, 192($v0)` count read and the else use belongs to the saved
web, while here the else use is still in the raw web. as1 then hoists the whole
`$36` block above the branch in both.

Newly eliminated this pass, each measured against the full-TU object and each
flat at one word:

- all 720 declaration orders, re-run because the product edit renumbers webs;
- 76 tested dead expressions (`if (E);`) over 19 expressions x four placements
  (before the `if`, first and last in the else arm, after the decrement).
  **Every one emits nothing and changes nothing** -- on this function the
  single-mention fold is total, which is the opposite of its behaviour on
  `levelFreeAll`, where the same construct reserved a pool colour;
- 48 else-branch cells: six base spellings x `index`/literal x four arm
  orderings;
- 13 structural forms of the if/else: empty else, `count < 2` inversion, the
  else body reached by `goto` and placed textually *after* the call (so text
  order is not what assigns the web), a shared `point` before the `if`, an
  alias local, and the same-expression else;
- six null-test spellings, `register` on `record`, seven pre-`if` base
  spellings, five orders of the three statements before the `if`;
- 54 base-decay cells (`&record->x[i]` against `record->x + i`) crossed over
  the loop, the if tail and the else arm, to make the two uses share an
  address CSE;
- 20 physical line groupings of the head statements crossed with the else arm.

Next lever: not a source spelling. The question is uopt's live-range split
rule -- what puts the `$36` use on the raw web here and on the saved web in the
target -- and it wants a `p1dec`/CDX web trace or a print in uopt's splitter,
not another lattice.
<!-- plateau-handoff:overlay1ResolvePathPoint:end -->
