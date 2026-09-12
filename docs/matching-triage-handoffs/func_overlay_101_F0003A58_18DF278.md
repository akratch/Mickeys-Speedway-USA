<!-- plateau-handoff:func_overlay_101_F0003A58_18DF278:start -->
### `func_overlay_101_F0003A58_18DF278` plateau handoff

- source: `src/overlays/o101/func_overlay_101_F0003A58_18DF278.c`
- score: 2 masked words of 1461
- frame: 0x50
- relocations: 199
- first mismatch: +0x1194
- summary: L145 carrier deletion plus the L146 header order took 1421 to 2 at size delta 0; what is left is one adjacent transposition in as1's post-schedule pass at group 9's text junction, with every pairwise ADD_TEXT swap, every text-store position and every two-macro line split already measured.

#### What moved, 2026-09-12, lane p11-o101

The prior packet's five-trial stall window, its `structure-mismatch` verdict and
its "next: prove node-pool base liveness across loader" were all answering the
wrong question. The residual was the carrier, exactly as on
`func_overlay_101_F00069E8_18E2208`, which closed 613 -> 0 the same day on the
same three edits. One rewrite, measured against the configured TU:

- before: size delta +8, positional masked 1421, byte-exact 456, register
  naming 628, immediate only 53, really different 384, displacement tax 356.
- after: size delta 0, positional masked 2, byte-exact 1459, register naming 0,
  immediate only 0, really different 2, displacement tax 0.

The edits, all three transferred rather than rediscovered:

- L145, delete every local that held an element address or an element index.
  `index`, `orderIndex`, `node20`, `node24` and `node32` are gone; every store
  is spelled through the array subscript of the counter global itself.
- L146, the group header takes `mode` first. The retained shape had `value14`
  first and `mode` seventh, an order measured on the carrier shape and void on
  this one.
- L131, spell the root as `gOverlay101OrderEntries[1 + group]`, the expression
  the matched siblings and the ROM-exact `overlay101Cleanup` use, not a
  `D_0.groups[group]` member path.

Relocations now agree at 199 with the shipped runtime table's own count, so the
extra address pair the prior packet could not explain was the carrier too.

#### What is left, and the axes that are now closed

Two words at `+0x1194`, an adjacent transposition. The target emits
`lw <text pointer>` for group 9's first text node and then the following node's
index multiply; this candidate emits the multiply first. Nothing else in the
function differs: prologue, frame, every other word and every relocation agree.

Measured cause, from `cc -Wa,-R`'s node dump for that block:

- The multiply's node carries `aftercycles 13`, the text load's `aftercycles 6`,
  so the multiply wins on priority rather than on a tie. It is ready one slot
  early only because the count increment for the current node was lifted into
  the preceding block.
- Group 9 is the only one of the sixteen text nodes that both carries mixed
  colour constants and is followed by another text node. The mixed colours are
  four extra `li`s, which is what gives the preceding block room to absorb the
  increment. The other six text-to-text junctions keep the increment in the
  block, where it forces the multiply's position and both sides agree.
- The block's emitted order is not the order `-Wa,-R` records, so a pass after
  the scheduler reorders it; that pass is not visible in this trace.

Closed by measurement, all on the full configured TU:

- Every pairwise swap of the seventeen `ADD_TEXT` statements, 136 forms. The
  shipped order is a strict optimum: only `childType`/`child` is a tie at 2,
  and the next best form is 14.
- Every single-statement move of the text store across the body, 14 forms;
  position 8 is the unique optimum and its neighbours read 65 and 96.
- Every two-macro split of the body across two physical lines, 16 forms. The
  best three tie at 2 and none moves `+0x1194`, so the L59 line axis does not
  reach this residual.
- Folding the two group-9 invocations onto one line reads 12, folding the whole
  group reads 22, and inserting a line between them is inert at 2. Line numbers
  therefore neither cause nor fix it.
- Declaration order of the two remaining locals, `register` on the length local,
  and `D_1D0++` for the increment are all inert at 2.

Next lever, named: the pass that reorders as1's scheduled stream after
`-Wa,-R` prints it. Nothing in the source axes above reaches it, so the next
attempt should instrument that pass rather than sweep another spelling.
<!-- plateau-handoff:func_overlay_101_F0003A58_18DF278:end -->
