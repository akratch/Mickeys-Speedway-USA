<!-- plateau-handoff:func_overlay_057_F0004E18_18A8A10:start -->
### `func_overlay_057_F0004E18_18A8A10` plateau handoff

- source: `src/overlays/o057/func_overlay_057_F0004E18_18A8A10.c`
- score: 217/1208 words
- frame: 0x140
- relocations: 373
- first mismatch: +0x100
- summary: The target's choice loop IS bounded on &gO57MiddleChoices[4], materialised into a3 and tested at the bottom, which refutes the note that bounding there parks an invariant in a callee-saved register; the &sourceState[4] bound in the source is a spelling that happens to be exact-sized, not the target's shape. The activePlayers fill is now the target's countdown pointer walk, which a previous pass rejected at 282 words and +4 bytes and which at the fixed home layout is 217 at delta 0, byte-exact 1028 -> 1029, really different 68 -> 66. The decision variable is which register holds outputIndex: s2 in the target against a0 here, and freeing one callee-saved register is what makes the cached choice->active load affordable.

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

<!-- plateau-handoff:func_overlay_057_F0004E18_18A8A10:end -->
