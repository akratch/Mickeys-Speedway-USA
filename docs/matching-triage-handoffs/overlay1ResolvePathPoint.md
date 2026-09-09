<!-- plateau-handoff:overlay1ResolvePathPoint:start -->
### `overlay1ResolvePathPoint` plateau handoff

- source: `src/overlays/o001/overlay_001_end.c`
- score: 149/152 words
- frame: 0x78
- relocations: 22
- first mismatch: +0x6C
- summary: 8 -> 3 words (structure-mismatch to allocation-mismatch); residual is the else-branch pointer base and the doubled-product register


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
<!-- plateau-handoff:overlay1ResolvePathPoint:end -->
