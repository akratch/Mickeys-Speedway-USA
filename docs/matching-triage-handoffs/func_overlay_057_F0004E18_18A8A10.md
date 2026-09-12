<!-- plateau-handoff:func_overlay_057_F0004E18_18A8A10:start -->
### `func_overlay_057_F0004E18_18A8A10` plateau handoff

- source: `src/overlays/o057/func_overlay_057_F0004E18_18A8A10.c`
- score: 203/1208 words
- frame: 0x140
- relocations: 373
- first mismatch: +0x100
- summary: Writing the choice loop as a for over source rather than a do-while with the step in the body is 205 -> 203 at delta 0, byte-exact 1038 -> 1040, register naming 102 -> 100, structure unchanged at 68; the loop keyword is the whole of it and six other tail spellings read 205 to 354. The prior packet's named decision variable, which register holds outputIndex, is not reachable from the source: a 1,131-form L115 reuse lattice measures six different locals at exactly 205 for that index, so carrier identity there is inert. Statement order is a move-one fixed point on the new shape at 1,703 compiles; the unguarded climb's 202 is the same two-call swap the prior lane rejected, and it is rejected again. What is left is 68 really-different rows against 100 naming rows, and the census reads per-iteration consumption over seven windows, so structure comes first.

## 2026-09-12 (lane `p11-big`): the target's bound is the global's end, read off the object

Still 217 masked at size delta 0; byte-exact 1028 -> 1029, register naming 112
-> 114, immediate 10, really different 68 -> 66. The frame is 0x140 on both
sides and the ladder is exact from +0x11C down to +0x78.

### A correction to note 4 in the source

Note 4 says the choice loop must NOT be bounded on `&gO57MiddleChoices[4]`,
because naming the global's end makes it a loop invariant that uopt parks in a
callee-saved register, from where it reaches the two tail reads. Read directly
off the target object, that is not what the target does. The target
materialises `&gO57MiddleChoices[4]` into **a3** before the loop and closes with
`addiu` on the cursor, `sltu` against a3, `bnez` at the BOTTOM of the loop.
`a3` is caller-saved and the loop makes no call, so there is no invariant to
park. What the target spends its callee-saved registers on instead is
`outputIndex`, in **s2**; the candidate has `outputIndex` in `a0` and parks
`&gO57MiddleChoices` in `s6`, copying it to `v0` for the walk, where the target
materialises it straight into `v0`.

So the `&sourceState[4]` bound is a spelling that happens to be exact-sized,
not the target's shape, and the structure it leaves behind is two differences
in the loop body: the candidate advances `source` at the top and stores through
`-1(source)` while the target advances it at the bottom in the branch delay
slot, and the candidate loads `choice->active` twice where the target loads it
once into `v1` and uses that one register for both the test and the store.

### Re-measured at this home layout (note 4's numbers predate note 5, so L146 voids them)

- `} while (choice < &gO57MiddleChoices[4]);` -- 259, delta +4.
- `} while (choice != &gO57MiddleChoices[4]);` -- 256, delta -4.
- `} while (source != &sourceState[4]);` -- 253, delta -4.
- caching `choice->active` in a local, current bound -- 516, delta -8.
- caching it with the global bound -- 552, delta -4, and identical at 552 for
  `choiceActive`, `rank` and `input`, so the carrier is still not the variable.
- caching it with `!=` on the global bound -- 260, delta -12, naming 103.
- the two tail `gO57MiddleChoices[0]` reads spelled `(*gO57MiddleChoices)`, or
  read through a reset `choice` cursor -- byte-identical, so L131 does not
  split that address web.

### Adopted

`active = &activePlayers[9]; for (i = 9; i >= 0; i--) { *active-- = 1; }` in
place of the index fill. Note 4 rejected this at 282 words and +4 bytes; at the
fixed home layout it is 217 at delta 0 with byte-exact 1028 -> 1029 and really
different 68 -> 66, and it is the target's own shape (`li v1,9`, `move v0,v1`,
`sb`, `addiu a0,a0,-1`, `bnez v1`, `addiu v1,v1,-1`). The split
`*active = 1; active--;` spelling measures identically.


### Statement order, run to a fixed point: 217 -> 205

A move-one hill climb over every run of three or more consecutive single-line
non-call statements in the body, re-climbed from its own output until it
reported no move (three passes). 217 -> 205 masked at size delta 0, byte-exact
1029 -> 1038, register naming 114 -> 102, immediate 10 -> 11, really different
66 -> 68, frame 0x140 on both sides.

Four orders move: `textureNodes[0].texture` ahead of `.alternate` and `.x`
ahead of `.y` in the label loop's store group, `gO57MiddleData31A8 = 0;` ahead
of `gO57MiddleData31B8 = gO57MiddleData31B4;`, and
`gO57MiddleData31E8 = gO57MiddleCourseIds[...]` after the three stores that
follow it.

One candidate the sweep offered is worth a further word and was REJECTED:
swapping `func_80028D24(0);` with `func_80028540(gO57MiddlePlayerCount);`
reorders two calls. The sweep tool now refuses to move any statement containing
a call at all.

### Next lever

Free one callee-saved register so `outputIndex` can take it. The candidate's
`s6` holds `&gO57MiddleChoices` across the loop only because it materialises
the base there and copies to `v0`; the target does not. That is the register
the cached-load form wants, and the cached-load form is what fixes the loop's
two remaining structural differences. Do not re-run: the six bound spellings
above, the three carriers for the cached load, the tail-read respellings, any
array dimension (L112 was swept and rejected), or the declaration census (it is
exact from +0x11C down to +0x78).

Validation: `gmake verify` printed 507341c0a40ca3e9a7cee969b396ee53facfb548;
`gmake cleanroom` and `gmake check-docs` pass. The candidate remains
`NON_MATCHING`, so no bytes are credited.


## 2026-09-12 (lane `p12-close`): the loop keyword is worth two words, 205 -> 203

Adopted one edit; four other axes are closed by measurement. All numbers from
the configured full TU through `nm_ranking`, the same comparator
`tools/score_symbol.py` reports.

  - before: 205 masked of 1208 words, size delta 0, byte-exact 1038, register
    naming 102, immediate only 11, really different 68, displacement tax 24.
  - after: 203 masked, size delta 0, byte-exact 1040, register naming 100,
    immediate only 11, really different 68.

### Adopted: the choice loop is a `for`, not a `do`/`while` with the step in the body

Writing the loop as
`for (source = sourceState; source < &sourceState[4]; source++)`, with the
`choice++` left in the body, is worth two words at an unchanged size delta and
an unchanged structural bucket. Nothing else in the loop changed: the same
bound on `sourceState` rather than on the global's end, the same body, the same
statement order.

Seven tail spellings were measured against the 205 baseline, and the keyword is
the whole of it. `for` with `<` reads 203; the shipped `do`/`while` with the
step in the body, advancing `source` inside the condition with `<`, and a
separate down-counter all read 205; `for` with `!=` reads 245; an up-counter to
four reads 248; and advancing inside the condition with `!=` reads 354. That
last spread is the reason to record all seven: the same loop written six other
ways is between equal and 149 words worse.

### The carrier identity of `outputIndex` is inert, so the named lever is not a source variable

The previous packet named the decision variable as "which register holds
`outputIndex`: s2 in the target against a0 here". A full L115 reuse lattice over
the three loops of the confirm block says the identity of that local reaches
nothing. The lattice varied, independently, the countdown fill's counter, the
second fill loop's index and the `activePlayers` scan cursor over `i`,
`outputIndex` and the ten spare function-scope `s32` locals -- 1,131 compiled
forms, on the pre-adoption shape:

  - the second fill loop's index is worth exactly 205 for six different locals:
    `outputIndex`, `currentGroup`, `nextSelection`, `characterId`, `nextValue`
    and `cursorValue`. Six distinct carriers, one number, so the choice loop's
    counter and the second fill's index are not required to be the same web and
    choosing between them costs nothing.
  - the scan cursor is forced: `i` is the only spelling at 205 and every other
    local is 208 or worse.
  - the countdown fill's counter is forced the same way: `i` at 205, every other
    local at 208.

So "free a callee-saved register so `outputIndex` can take it" cannot be
executed by giving `outputIndex` a different carrier -- carrier identity is
measured inert here. Whatever puts that value in s2 is upstream of the local.

### The choice loop body is a local optimum on shape

Six body shapes, each measured alone against the 205 baseline:

  - advance `choice` at the bottom and bound on `&gO57MiddleChoices[4]` -- 249.
  - test `choice->active` before storing it to `*source` -- 514.
  - name `gO57MiddleCharacterIds[choice->tableIndex]` once in a local, which is
    read twice in the body -- 246 for `valueA` and 547 to 549 for
    `characterId`, `nextValue`, `cursorValue` and `oldValue`. The 300-word
    spread across carriers is real and none of them reaches the baseline.
  - walk `gO57MiddleOutput` with the `output` pointer instead of the
    `outputIndex` subscript -- 249.
  - drop the `choice` cursor and subscript the global, `gO57MiddleChoices[i]`
    -- 249.

The two loads of `choice->active` the previous packet wanted removed are what
the store through `*source` forces: `sourceState` is a stack array reached
through a pointer and uopt does not disambiguate it from the global, so the
second read reloads. Every form measured that removes the reload costs more than
it buys.

### The countdown fill's branch kind is not settable from the loop spelling

The target closes that loop on `bnez`, this candidate on `bgez`. Eleven
spellings measured: `for (i = 9; i >= 0; i--) activePlayers[i] = 1;` is
byte-identical at 205, the two pointer-bound forms are 247 and 248, every
post-decrement-test form that does produce `bnez` (`while (i-- != 0)`,
`for (i = 9; i-- != 0;)`, the split `*active = 1; active--;` variant) is 259,
`--i != 0` from 10 is 261, ascending index is 261, and `i != -1` is 658. So the
`bnez` shape is reachable and costs 54 words elsewhere; the branch kind is not
the lever on its own.

### What is still open

The residual is 68 really-different rows against 100 naming rows, and the
register census reads 64 percent coherence over seven windows, which
`docs/LANE_BRIEF.md` says is per-iteration consumption rather than one ring
phase. Read that as: the naming bucket here is downstream of the structural one,
so the order is structure first. The structural sites are paired insertions and
deletions, all local, measured on the pre-adoption shape:

  - candidate surplus at +0xDE0 two words against target surplus at +0xDD0 two
    words -- a 16-byte move, not an extra instruction.
  - candidate +0xF80 one word against target +0xF74 one word -- the countdown
    fill's branch, above.
  - candidate +0xFBC four words against target +0x1008 two words and +0x1164 two
    words -- the choice loop body.
  - target-only singles at +0x11C0, +0x11CC, +0x11D4 and +0x1248 against
    candidate-only singles at +0x11DC, +0x11E8 two words and +0x1204.

### Statement order is a fixed point on the new shape, and the unguarded gap is the old rejected swap

L146 says a statement-order optimum belongs to the shape, so the previous
packet's fixed point was void the moment the loop keyword changed and had to be
re-climbed. `tools/blockclimb.py`, guarded, 1,703 compiles, one pass:
**203 -> 203, no move improves.** The new shape is a move-one fixed point too.

`--probe-unguarded` reaches 202 in one move, and that move is the one the
previous lane already found and rejected: swapping `func_80028D24(0);` with
`func_80028540(gO57MiddlePlayerCount);`, two calls in the other order. Measured
directly in the new shape at 202, and rejected again. Recording it because the
one word it is worth is real evidence about the target's shape at that point,
not because the order is adoptable.

Do **not** re-run: the L115 carrier lattice above, the six body shapes, the
seven loop-tail spellings, the eleven fill spellings, the six bound spellings or
the tail respellings from the previous packet, or the declaration census.

Validation: `gmake verify` printed 507341c0a40ca3e9a7cee969b396ee53facfb548,
and `tools/gates.sh --staged` passed all four gates. The candidate remains
`NON_MATCHING`, so no bytes are credited by the two-word gain.

<!-- plateau-handoff:func_overlay_057_F0004E18_18A8A10:end -->
