# Mickey IDO 5.3 learnings

This is the reusable playbook for compiler behavior measured on Mickey. It is
not a function ledger and not a collection of folklore. An entry belongs here
only when a Mickey-exact match or a controlled experiment against Mickey's
pinned IDO 5.3 toolchain proves a principle that can guide another target.

Keep entries generic: state the symptom, mechanism, useful source lever, and
limits. Function-specific addresses, register triples, match percentages,
aligned instruction rows, and attempt history stay in `docs/resident.md`,
`docs/overlays.md`, `docs/matching-triage.md`, or ignored workbench state. ROM
bytes and disassembly never belong here.

## Diagnostic order

1. Prove the boundary, ABI, TU, configured flags, relocation surface, and
   untouched baseline before changing source.
2. Run the flag lattice before source permutation. Optimization level, ISA,
   debug mode, loop unrolling, the R4300 multiply scheduler, and the forced
   `uopt`-at-`-O1` path are measured project modes, not cosmetic switches.
3. Classify the first mismatch with the full-TU object and decomp-workbench.
   Use phase replay to decide whether C structure, `uopt` allocation, `ugen`
   allocation, or `as1` scheduling owns the residual.
4. Apply one source lever that matches the diagnosis. A higher score without a
   mechanism is not evidence and should not displace a better-understood base.
5. Rebuild through the configured project pipeline. Scratch/permuter scores
   are search hints; only exact owned bytes, relocations, linked placement, and
   the ROM comparison establish a match.

## Proven reusable behavior

### Flags and compiler phases

- The driver does not run `uopt` below `-O2`, but Mickey's
  `setglobalintmask` requires `uopt` forced into an otherwise `-O1` pipeline.
  `tools/ido-phases.py` is therefore a real flag-lattice member, not a debug
  convenience. Evidence: `docs/modules.md` section 6's flag census and
  `docs/flag-sweep.md`.
- A downstream scheduling diff can originate in an upstream register choice.
  Diagnose and replay the producing phase before attempting statement-order
  churn; forcing a target color is causal diagnosis only and is never a
  promotable build step. Evidence: `docs/breakthrough-campaign.md` Phase 2.
- Per-TU flags stay local. One exact function can establish a candidate flag
  group, but changing a shared group requires evidence and an impact review
  across every consumer. Evidence: ADR 0007 and `docs/flag-sweep.md`.
- `-Wab,-r4300_mul` can alter FP multiply hazard scheduling as well as the
  integer-multiply cases its name suggests. When otherwise exact MIPS-II code
  is one scheduler nop short between adjacent FP multiplies, test this flag as
  a focused mechanism probe; keep it only after every function in the shared
  TU, all relocations, and the linked image remain exact. Evidence: Overlay
  25's exact effect initializer in `docs/overlays.md`.

### Retained data and relocations

- An exact internal branch can lack the assembler fallback's PC16 record
  because IDO already resolved its displacement. Restore metadata only when
  the existing displacement and destination are independently authenticated:
  a same-section symbol at the branch site makes the link contribution zero.
  Assert the opcode, unchanged addend, symbol position, in-range destination
  and executable-prefix digest. This does not repair a wrong branch field or
  establish identity from instruction equality alone; require the original
  relocation tuples and linked owned-ROM comparison. Evidence: the resident
  object switch promotions in `docs/reloc-surface.md`.
- When a mixed translation unit emits the exact instruction fields but also
  creates a duplicate compiler-private literal pool, preserve the fields and
  rebind only their existing relocations to one absolute symbol for the
  retained overlay-local pool. Externalize the duplicate section only behind
  an exact payload digest, and require the complete runtime relocation surface
  plus linked ROM bytes to agree. This applies only when the source object's
  words are already exact after ordinary relocation; moving an addend into a
  LO16 field after compilation remains prohibited. Compiler-private jump-table
  relocations may use the same contract when the duplicate table's relocation
  section is removed as metadata and the retained payload is independently
  authenticated. Evidence: Overlay 8's exact channel updater and Overlay 41's
  exact item spawner in `docs/overlays.md`.

- A multiword counter update can have correct arithmetic but the wrong address
  materialization when a donor's separate read and write names are collapsed
  into one extern. Under the measured resident IDO settings, a TU-defined
  destination and a separate extern read reproduce a different load/store
  sequence. An extern destination, including a fixed-size array, still emits
  an additional high-half address materialization for the second store.
  A native weak read alias of the actual TU-defined object can preserve the
  donor's spelling while binding both accesses to the same storage. Use this
  only when the donor and target establish that identity: prove the linked
  alias address, every relocation, initializer and owned data extent. Migrating
  the real initializer may require trimming only compiler alignment padding;
  a second allocated object or a masked instruction score is not a substitute
  for proving shared storage and the full ROM. Evidence: the scheduler
  retrace reconstruction in [the resident census](resident.md).

### Allocation and source shape

- A command-pointer load followed by a separate cursor update can produce a
  different allocation from `command = (*cursor)++`, even when both advance
  by exactly one command. Paired full-TU traces showed the post-increment form
  create an additional interfering web in p1; the stock command instructions
  then used the required color. The resulting two-store scheduling residual
  closed by placing the independent header and payload assignments on one
  physical source line. Preserve the pointer type, increment size, evaluation
  count, and complete command writes. A forced color alone changed the store
  order and was not exact, so remeasure the whole sequence after changing
  allocation. This is a measured source-form lever, not a rule that every late
  return-register mismatch comes from a callee prototype. Evidence: the exact
  resident sprite-segment renderer in `docs/resident.md`.
- Correct frame size does not prove the source's stack objects. A callee that
  reads and writes a full vector, or consumes a transform with a later frame
  field, can expose a draft's undersized arrays and disconnected locals.
  Reconstruct those aggregates from the callee's authenticated accesses before
  tuning homes or colors; separate volatile scalars are not a substitute for
  the object actually passed. Reusing a consumed input parameter for a later
  result can then remove a surplus home when the target stores that result in
  the parameter's caller home. Prove that no subsequent path needs the input
  value, and validate every read field and aggregate extent. Evidence: the
  exact resident sprite-segment renderer in `docs/resident.md`.
- When a table index local adds an unwanted colored value before a call,
  carry the final table result into the call instead. A nested lookup assigned
  before argument setup can leave the intermediate index temporary while the
  result takes its call-argument register at zero measured color cost. Paired
  full-TU traces with stock-output fidelity confirmed this in the exact resident
  ROM-section DMA helper recorded in `docs/resident.md`. Preserve signed index
  widening, lookup order, and evaluation count. Directly nesting the lookup in
  the call or overwriting the incoming parameter can instead add a spill or
  store; neither is equivalent as an allocation experiment. This is a carrier
  placement lever, not proof of a target register's allocator role from its name.
- When a masked unsigned bit controls a branch, spelling the test as an
  unsigned left shift followed by a right shift can change UGEN's temporary
  phase even when optimization folds the final right shift out of the branch
  schedule. In one exact switch dispatcher, this natural bit extraction kept
  every opcode and relocation site fixed while repairing a shared temporary
  carrier. Apply it only when the source object's unsigned width and selected
  bit are proved; a signed shift, wider type, or value-consuming expression is
  not equivalent. Require exact configured words, frame, relocations, linked
  owner, and full ROM. Evidence: Overlay 7's exact mode dispatcher in
  `docs/overlays.md`.
- A small register-only switch residual can come from a named selector even
  when both functions are frameless. Putting a single-use selector expression
  directly in the switch removed its named carrier and restored exact output
  under unchanged full-TU flags. Try this only after measuring the frame and
  relocation surface, and preserve the expression's integer conversions and
  evaluation count. The paired source result proves allocation sensitivity,
  not which compiler phase caused it or a general rule for all switches.
  Evidence: the exact object-data sizing switch in `docs/resident.md` and its
  configured relocation and ROM proof in `docs/reloc-surface.md`.
- Possible colorability is not per-function ownership. The workbench's static
  IDO 5.3 temporary-only classification of integer t0 through t5 conflicts with
  its own decoded color map and with a controlled Mickey function whose count
  and next-frame values occupy colored t0 and t2. Do not use that profile as
  proof that a target register is uncolorable or that UGEN alone owns a residual.
  Bind actual role evidence separately to each compared input; missing target
  evidence must remain missing. A FIFO replay over a subset chosen from observed
  allocation returns proves only consistency with that subset, not that it is
  the complete available pool. Check initialization and reservation semantics
  independently, and never infer reservation merely from absent allocations or
  absent surviving instructions. A later source carrier can change early output
  while early expression records remain unchanged, so demand order is not the
  only explanation to test. Evidence and the withdrawn overclaim are recorded
  in the `func_80020D8C` two-source handoff. Workbench commits `028a8eaf` and
  `1da7f333` now separate possible colors, conditional per-input reservations,
  and trace-directed ownership; independent review and real-input acceptance
  preserve the scalar comparisons while withholding unsupported causal advice.
- A traced free request is not necessarily a queue transition. In the pinned
  UGEN producer, FREE/FORCE_FREE entry hooks precede conditional mutation, and
  a successful free can append directly without invoking the separate ADD hook.
  REMOVE is also conditional on membership; MOVE_END belongs to the used list,
  not the free FIFO. Source-authenticated initial ADD/REMOVE records established
  different initial membership in a controlled pair, but did not make its
  entire dynamic queue history observable. Dropping requests and relying only
  on ADD would lose real frees; treating every request as an append invents
  others. Keep initialization proof separate from complete replay, and require
  successful-transition evidence for the latter. The compiler-source binding
  and limits are recorded in the `func_80020D8C` reservation audit.
- Reusing a masked index in an existing predicate can change its allocation
  class, not merely its temporary demand order. In a controlled full-TU pair,
  an equivalent predicate shared the low mask with a table access. Faithful
  stock/trace-off/trace-on output and named procedure traces showed that the
  shared value became colored across the branch, its separate body calculation
  disappeared, and GP allocator-result events decreased. Switching only between byte
  arithmetic and typed indexing still swapped the pointer/scale temporary roles
  in both variants. Diagnose these two effects separately: tree order can be
  controllable while cross-branch reuse destroys the target's instruction shape.
  A predicate equivalence needs its complete input/path domain proved; do not
  infer storage validity from a bounded index or add observable guards. This
  negative control is not an exact-source recipe or target compiler trace.
  Evidence: the predicate-demand controls in the `levelFreeAll` handoff shard.
- A swap of two temporary-register roles need not be a FIFO phase error.
  Final scheduling can hide which expression requested each temporary first.
  Faithful stock/trace-off/trace-on controls and unchanged stock pass replay
  showed identical FIFO initialization and return sequences, but different
  expression-to-return assignments. Moving an unused cursor offset from a
  call-result initializer into an existing nonempty-range arm put the bounds
  demand before the index-scale demand in both pre- and post-optimization
  streams and closed the register-only residual. Inspect demand order before
  adding a phantom allocation or pursuing colored-register changes. Preserve
  the unconditional helper call, output writes and all observable accesses;
  postpone arithmetic only when it is unused on the excluded path and defined
  on the retained path. This is not proof of the target's original source or
  compiler trace, nor a claim that the intermediate streams differ only by
  ordering. Exact output still depended on retained predicate grouping; an
  unused declaration and equivalent relational spelling were independent flat
  controls. Evidence: the matched record lookup in
  `src/overlays/o001/overlay_001.c`, with its exact mixed-TU and linked-ROM range
  recorded in the overlay atlas and remaining spelling in the cleanup queue.
- Instruction order inside a basic block is decided by `as1`'s list
  scheduler, not by `ugen`, and among ready instructions it prefers the
  lower source line stamped on each emitted record. A loop-invariant that
  `ugen` hoists (a table or base address, a loop constant) carries the loop
  header's line, so an initializer written on the line above the loop is
  scheduled before the hoisted address even when the target executes it
  after. Placing the initializer on the header line (`count = 7; do {`)
  moves it behind the hoist; conversely, splitting a loop-header initializer
  onto the preceding line can move the initialization ahead of the branch and
  leave the hoisted address as its delay-slot fill. Evidence:
  `overlay40UpdateEntries` (44/46 to exact), the three scheduled words of
  `overlay57HandleModeInput`, and the exact resident texture-table initializer.
  The first two were found with the workbench's ugen emit-provenance trace on
  2026-09-02. It does not apply to register renames, to delay-slot fills chosen
  by latency (`func_8001A154`'s `li -1`), or to relocation-surface differences.
- The lexical start of a conditional block can be an allocation boundary for
  a loop-invariant expression. If IDO hoists an invariant value into a saved
  register, adding save/restore and move instructions, initialize the existing
  local at the start of the only branch that consumes it; IDO may keep a
  per-iteration temporary and recover the smaller frame. This is valid only
  when the rejected path never observes the value and the ordinary object,
  relocations, linked owner, and ROM all remain exact. Evidence: Overlay 34's
  exact record updater in `docs/overlays.md`.
- Equivalent nonzero tests can affect register selection for an unsigned
  halfword local. A controlled paired full-TU IDO 5.3 build with `-O2 -mips2`
  changed only `if (value)` to `if (value != 0)`, preserving the input pathname,
  every line break, declarations, and flags. Only register fields changed;
  instruction geometry, frame, relocations, neighboring functions, and data
  stayed identical. When a narrow condition is implicated in a small allocation
  residual, isolate these natural spellings before changing unrelated locals.
  This proves source-shape sensitivity, not which compiler phase causes it;
  the effect is not established for other scalar widths, flags, or TU contexts.
  Evidence: controlled paired compiler experiment, 2026-09-06. Matching still
  requires the ordinary configured object and complete linked-ROM proof.
- The NUMBER of declared locals, not only their order, sets where a
  call-crossing value is homed: declared locals take descending homes from
  the frame top in declaration order and compiler temporaries sit below
  them, so removing a named local and letting a call-crossing common
  subexpression stay a compiler temporary moves it one word lower
  (`overlay34InitStorage`), reusing an existing local as the carrier drops
  the declared count by one and lowers a later spill by one slot
  (`func_overlay_026_F0000B18_187AF10`), and declaring a pair after the
  local whose slots it must follow lands both on the retail homes
  (`overlay84InitializeAndUpdate`). All three were exact on 2026-09-03.
- A never-read scalar declaration may still displace compiler-created stack
  homes even when its initializer is optimized away. When opcode, register,
  frame, and relocation surfaces already agree and every residual is the same
  one-word home displacement, remove only a local whose initializer is proved
  defined and side-effect-free and whose value is never observed. In an exact
  resident FX closure this moved three call-crossing homes by one word without
  changing any other instruction. Recheck the ordinary object, complete
  relocation identities, linked owned range, and full ROM; this lever does not
  apply to locals whose initialization or lifetime is observable. Evidence:
  the exact `func_8004ADE8` closure, 2026-09-08.
- Removing one redundant input alias can disturb exact stack homes while
  removing two together recovers both the frame and the surviving homes.
  Declaration placement can expose the intermediate state: a correct frame
  and named locals, with only compiler temporaries displaced by one word.
  Removing the remaining alias can then move those temporaries without
  changing the rounded frame size. A regressing single-alias deletion does
  not rule out that paired change when new frame and home measurements
  support it. Apply only to aliases that are never modified or escaped;
  preserve the parameter types, call order and memory accesses, and require
  ordinary compiler bytes, relocation identities and linked-ROM proof.
  Evidence: the Overlay 26 effect-handler closure in `docs/overlays.md`,
  2026-09-08.
- Used scalar copies can recover an omitted declared stack region without
  retaining the memory traffic of an array. For a movement operation that
  consumes each displacement twice, separate float copies preserved the
  instruction and register sequence while enlarging the frame; an equivalent
  three-element array retained extra loads/stores. Once the frame agreed,
  moving a used bound declaration before the call-crossing coordinates placed
  their homes. Distinguish frame extent from home position and measure them
  separately. This is a source-level layout observation under stock IDO 5.3
  `-O2 -mips2`, supported by configured object and linked-ROM identity; it does
  not justify unused padding or new volatile accesses.
- Declaration position can shrink the emitted frame without reducing the
  declaration count. Moving a spilled return pointer earlier recovered both
  the frame and its home while later declarations remained in the source.
  Distinguish the front end's reserved offsets from the final frame: declaration
  count alone is not a lower bound after optimization. Measure actual stack
  accesses and frame extent before deleting useful locals. This was proved
  with a paired stock full-TU IDO 5.3 build and subsequent linked-ROM identity;
  the available web-detail trace covered only part of the frame and did not
  establish which pass removed the unused tail region.
- A register swap confined to a later loop can come from a local shared with
  an earlier loop. In a clear-then-copy routine, using the copy source as the
  clearing cursor joined both lifetimes into the higher-priority pointer web.
  A forced color merely moved the residual into the clear loop. Reusing the
  destination for both writing phases instead left the read source local to
  copying and made stock output exact. Paired fidelity-proved p1 traces kept
  the same two priority levels but exchanged which source home occupied each.
  Inspect every use of the shared local before treating a late swap as an
  isolated tie-break; this lever requires real, compatible pointer roles and
  does not justify new accesses or forced compiler output in canonical builds.
- A one-local ablation can move temporary spill homes while leaving the frame
  unchanged because the frame rounds to eight bytes. In a loader whose register
  and instruction sequence already agreed, removing redundant aliases in pairs
  reduced the frame while restoring every emitted spill offset. Direct repeated
  array expressions preserved common-subexpression carriers without the named
  locals; deriving one byte offset from another changed those carriers and
  regressed allocation. Read frame extent and spill offsets separately, and
  preserve independent index expressions when removing their aliases. This was
  measured with stock full-TU IDO 5.3 builds and confirmed by linked-ROM identity;
  it does not establish the same behavior for addressed locals or expressions
  with intervening writes or calls.
- A mixed integer/pointer sentinel can cost both a declared home and a second
  constant carrier even when the stored bits are identical. On a proved
  32-bit raw-word table, model each physical word with an integer/pointer
  union, reuse a pointer parameter only after its original value is dead, and
  write both sentinel words through the same pointer member. IDO then keeps one
  typed constant web; combining that with repeated scaled indexing can remove
  the named index and sentinel homes together, reducing the frame by one
  eight-byte quantum while retaining the call-crossing spill. This is valid
  only when the word and pointer widths and all-ones representation are proved,
  the original pointer has no later use, and the ordinary object, complete
  relocation surface, linked owned range, and full ROM are exact. Evidence:
  the exact `func_800347A0` closure, 2026-09-08.
- ugen's integer temp ring is consumed one pop per compiler temporary, and
  the `DKWB_UGEN_TRACE` pop sequence shows the count directly: reading a
  struct field through a local costs a pop that a direct read does not, an
  index scaled twice costs one more pop than an index scaled once (type the
  table as pairs), and a pool-carried accumulate (`x = a; x += b * c;`)
  changes a pop. Two pops off means two of these, and the fixes only work
  together (`overlay20UpdateObjectResource`, `func_overlay_070_F00000D8`).
- A named common-subexpression carrier can consume an otherwise invisible
  ugen temporary even when optimization leaves the same instruction shape.
  In an exact initializer, spelling a just-written pointer chain as
  `array[0] = call(); array[1] = array[0] + size; end = array[1] + size;`
  instead of carrying the call and first addition through locals aligned the
  later integer temporary ring without changing the frame or relocation
  surface. Use this only when no intervening call, volatile access, or
  aliasing write can change the read-back values; require exact configured
  code and linked-ROM proof. Evidence: the exact `func_8004E8E0` closure in
  `docs/resident.md`, 2026-09-08.
- A known-zero byte read can survive as allocator state after its value folds
  away. In an exact release routine, writing zero to a status byte and then
  assigning `status | 1` in the next conditional emitted the same constant
  value as literal `1`, but kept one otherwise invisible UGEN temp-ring pop
  and selected the unsigned OR-immediate form. That one pop aligned the whole
  following flag chain. Apply this only when the store-to-read path has no
  intervening call or aliasing write and the zero value is proved; verify the
  configured words, relocation identities, linked range, and full ROM.
  Evidence: Overlay 47's exact release routine, 2026-09-08.
- A block-local clamp carrier can restore stack-home placement before a later
  narrowing conversion fixes a temp-ring phase. In an exact paired result, a
  scoped word local used for the clamp value moved four existing homes without
  changing frame size, while placing a proved in-range `s16` conversion inside
  the following negate rotated seven terminal temporary registers without
  adding an instruction. Treat these as separate levers: use the carrier only
  for a real value with the same lifetime, and use the inner conversion only
  after proving that every reachable operand already fits the narrow type.
  Out-of-range or otherwise implementation-dependent conversions are not
  interchangeable. Require exact configured code, relocation identities,
  linked bytes, and full ROM. Evidence: the exact
  `overlay94UpdateController` promotion.
- Hoisted loop-invariant addresses are materialized in ugen's birth order,
  so a bound kept in a local born before the count global is issued first;
  spelling the bound inline in the loop test hoists it after the count
  (`func_overlay_014_F0000000_186F8D8`).
- An existing nonvolatile cursor load can affect a temporary-register residual
  through source order alone. Moving the once-only load between two independent
  coordinate assignments made stock full-TU IDO output exact without changing
  command order or stores. For this symptom, consider moving the existing load
  across pure local or by-value parameter assignments; preserve its guard and
  reject crossings involving aliasing writes, calls, volatile access, or escaped
  locals. Keep arithmetic and its defined-input domain unchanged, including
  signed-overflow limits; do not add a read or broaden when it executes. This
  proves source-order sensitivity, not a new internal ugen phase mechanism.
  Require unchanged declarations and exact ordinary code, relocation identities,
  linked bytes, and full ROM. Evidence: the exact resident clear-buffer closure
  in `docs/resident.md`.
- Declaration order can determine stack-home order for call-crossing locals.
  When the operation sequence is exact but spill offsets differ, reorder
  semantically independent declarations before inventing extra state.
  Address-taken local arrays and structs can retain the same
  declaration-relative placement even when preceding scalar and cursor locals
  are colored into registers, so moving an existing aggregate past those
  declarations can change only its frame offset. Producer traces can also show
  that lexical pointer aliases receive automatic homes despite later coloring;
  removing aliases that merely name direct array accesses may shrink the frame
  and move a surviving address-taken scalar to its retail home. Apply that
  narrower lever only when the direct accesses preserve evaluation and update
  order. Evidence: the exact resident collision-handler cohort in
  `docs/resident.md`, plus the exact tile-command builder, spawn-entry
  initializer, and Overlay 98 accepted-entry collector in `docs/overlays.md`.
- Expression association, signedness, and width affect IDO's internal values
  even when C semantics appear equivalent. Preserve the ABI and inferred data
  model; use typed rewrites as a diagnosed lever, not an arbitrary score nudge.
  Evidence: the exact animation, save, and collision cohorts in
  `docs/resident.md`.
- Reusing a word-sized parameter for a value that is narrowed only at its
  eventual byte store can preserve IDO's full-width register web and compact
  frame. Introducing a separate byte local may instead truncate at entry and
  create a byte-sized stack home. Apply this only when every intervening use
  has the same proven value domain and the callee ABI accepts the narrowing;
  reject it unless code, relocations, and linked bytes remain exact. Evidence:
  Overlay 84's exact current-resource activator in `docs/overlays.md`.
- A merged TU or a shared placeholder prototype can change caller code while a
  standalone callee looks exact. Treat symbol binding, visible prototypes, and
  TU ownership as part of the compiler input. Evidence: the merged-TU blocker
  class in `docs/matching-triage.md`.
- An ignored integer return is still part of IDO's allocation input. A caller
  can retain a small register-selection mismatch when its declaration says
  `void` but the callee returns an integer; restoring the authenticated return
  type can close that mismatch without consuming the result. Review the
  winning translation unit's declarations as well as its function body when
  transferring a search result. This is an ABI correction, not permission to
  vary return types for a better score: prove the callee's return behavior and
  check every affected caller, configured object, relocation and linked ROM.
  Evidence: Overlay 4's exact object-motion update and its scoped header.
- Under O32, a single 64-bit integer argument occupies an aligned `a0`/`a1`
  pair. IDO materializes the two halves of a constant zero independently, so a
  target with two adjacent argument-register zero loads can indicate one
  `u64`/`OSTime` parameter rather than two scalar parameters. Use relocation or
  callee metadata to prove that ABI before changing a prototype; equal register
  contents alone are insufficient. Evidence: Overlay 18's exact startup loader
  in `docs/overlays.md`.
- An algebraically zero integer read can be a caller-saved coloring lever. On
  a path that already returns zero, spelling the result as `value * 0` kept the
  value's web live through IDO's allocation decision while still folding to a
  zero return instruction; a two-web forced-color probe identified the cause,
  and the same stock source then matched without a force. This is specific to
  the measured `-O2 -mips2` basin: use it only when zero is semantically
  required, and reject it if instruction count, relocations, or linked bytes
  move. Evidence: Overlay 1's exact value-cache manager in `docs/overlays.md`.
- Expression shape can affect allocation even when the changed subexpression
  is constant. A bounded search paired unsigned XOR-zero around a complete
  constant RGB packing group with an equivalent logical-negation zero test;
  together they produced exact stock full-TU output. Unlike the value-read
  example above, this XOR introduces no new live input. When transferring the
  retained expression, preserve unsigned types, side effects, evaluation count,
  and the one-line display-list macro block grouping. This proves the paired
  result, not either change's individual cause, a precise compiler phase, or
  recovered original source. Do not generalize to undefined expressions or
  altered side effects; require exact owned bytes, relocation identities and
  linked ROM. Evidence: the exact cone-drawing closure in `docs/resident.md`.
- A source copy can decouple load order from FP coloring when two equal-cost
  webs tie. Load the first value through the local that must receive the first
  color, copy it to its lasting local, then overwrite the first local with the
  second value; IDO can coalesce the copy away while preserving the opposite
  load and color orders. This applies only when the copy and overwrite are
  semantically exact and the stock object emits no move or spill; reject the
  shape unless instruction count, relocations, linked bytes, and the full ROM
  remain exact. Evidence: Overlay 8's exact scale-output body in
  `docs/overlays.md`.
- When two independent initializations need reversed caller-saved colors but
  separate statements let the scheduler split a relocated address pair,
  preserve their evaluation order in one comma expression. IDO can retain the
  desired web-formation chronology while scheduling the address pair together;
  separate-statement orders may independently fix either coloring or schedule
  without fixing both. Apply this only to semantically independent
  initializations, and reject it unless instruction count, relocation identity,
  linked bytes, and the full ROM all remain exact. Evidence: the exact camera
  override search in `docs/resident.md`.
- For a commutative address calculation, source operand order can select which
  producer receives each block-local temporary without changing the final
  `addu`. When only the base load and scaled-index shift exchange ring
  registers, spell the proven-equal expression as integer-plus-pointer or
  pointer-plus-integer to match their evaluation order; reject the lever if any
  other word, relocation, or linked byte moves. Evidence: the exact Huffman
  table builder in `docs/resident.md`.
- When a computed magnitude is immediately tested and then inverted, assigning
  it through the eventual scale local before the test can preserve IDO's
  floating-point carrier web. Testing one local and assigning the reciprocal to
  another may split otherwise equivalent live ranges and recolor every
  downstream operation. Use this only when both locals represent exactly the
  same value at the test and no path observes the intermediate separately;
  require exact code, relocation identities, linked bytes, and full ROM.
  Evidence: Overlay 1's exact motion-point resolver in `docs/overlays.md`.
- In a floating-point coefficient sum, a mathematically equivalent term order
  can change IDO's expression tree and the temporary schedule of later
  polynomial evaluation. When a residual is confined to that evaluation,
  preserve the target's inferred association by moving the proved independent
  leading term to the end of the source sum; also test operand order on a
  commutative scalar multiply. For an independent difference of products,
  spelling `a*b-c*d` as `-(c*d)+(a*b)` can rotate one term without changing
  the result; an explicit `(*pointer).field` may also preserve a remaining
  operand-order choice that the arrow spelling does not. These levers apply
  only when the arithmetic domain and evaluation order are proved
  interchangeable, and they must be rejected unless the configured object,
  relocations, linked range, and full ROM are exact. Evidence: Overlay 41's
  exact curve sampler and Overlay 22's exact plane resolver in
  `docs/overlays.md`.
- When otherwise exact single-precision code differs only in a multiply's
  operand order, an identity product on an already-computed operand,
  `(x * 1.0f) * y`, can change IDO's emitted operand order without adding an
  instruction. This expression-shape effect is measured; the optimization
  pass responsible is untraced. Treat it as a narrow source-spelling lever,
  not a general floating-point equivalence rule: do not duplicate side effects
  or volatile reads, assume exceptional-value behavior, or substitute a double
  literal. Reject the form unless instruction count, frame, relocation
  identities, linked bytes, and the full ROM remain exact. Evidence: the exact
  Overlay 29 point-projection TU, `src/overlays/o029/overlay29ProjectPoint.c`.
- A display-list macro invoked with a postincremented cursor keeps its local
  command pointer, cursor advance, and field writes on the invocation's source
  line. IDO can then schedule the cursor advance before the writes and choose a
  different order for the command-word stores and constant materializations.
  Hand-expanding those operations across separate lines preserved the same
  instruction multiset but not the schedule; delaying both cursor advances let
  IDO coalesce two commands and removed three instructions. Use the native macro
  only with its complete header context (`_SHIFTL` must be a macro, not an
  undeclared call), and reject it unless command semantics, relocations, linked
  bytes, and the full ROM remain exact. Evidence: the exact resident
  `func_80034920` display-list reset.
- An inlined mask expression can preserve instruction count, frame and opcode
  order while changing register operands throughout later display-list code.
  Reusing an existing integer local for successive width and height masks,
  as established by a permitted donor, removed that register residual without
  adding instructions. The measured cause is the source carrier; no allocator
  pass or temporary-pool mechanism was established by a trace. Apply this only
  to proved disjoint lifetimes, with unchanged expression types, memory
  accesses and arithmetic. It does not justify dead locals or an arbitrary
  declaration search. Evidence: the exact resident track draw coordinator in
  `docs/resident.md`, including its configured full-TU donor comparison and
  linked/runtime relocation proof.
- Across consecutive clear loops, an unchanged parsed C tree can still emit a
  different address-materialization schedule when physical line boundaries
  change. A relevant span may begin inside one loop, cross its closing brace,
  condition and intervening statements, and end at another loop's opening
  brace. Preserve the complete span through candidate emission, not merely
  adjacent assignments; retain every expression, statement and scope as
  searchable C. Exact lexical/AST correspondence can establish the boundaries,
  but only a fresh compile establishes source-carriage fidelity. The compiler
  pass responsible for this measured sensitivity remains untraced. Ordinary
  local pointer-assignment spellings can also affect the result; when spelling
  and grouping change together, do not attribute the exact result to either
  lever alone. Require unchanged semantics, exact owned bytes and relocation
  identities, linked-range and full-ROM proofs. Evidence: the matched initialization
  routine in `src/main/anim.c`.
- Typing an interleaved two-word cache as an array of structs can make IDO
  strength-reduce an index loop into advancing entry and byte-offset pointers;
  differently typed sentinel fields can also materialize separate copies of
  the same `-1`. Keeping the externally proved storage as a flat word array and
  spelling its ID/pointer slots as `(i << 1)` and `(i << 1) + 1` preserves the
  index, emits the per-iteration `i << 3`, reuses that byte offset for cleanup,
  and can share one sentinel carrier. Apply this only when allocation, element
  width, and both slot meanings are independently authenticated; require exact
  frame, relocations, linked bytes, and full ROM. Evidence: the exact resident
  `func_800359D4` sprite release.
- A canceling pointer increment/decrement between two groups of stores can be
  a zero-instruction allocation lever. IDO eliminates the net-zero arithmetic
  but retains enough of the pointer lifetime split to change a two-web
  caller-saved coloring; a bounded permuter found this after natural source
  already had the exact opcode, frame, and relocation shapes. Use the form
  only when the intermediate pointer is valid within the independently proved
  array and no alias can observe it, disclose it as inert, and keep a cleanup
  task for a natural equivalent. Require the ordinary project object, linked
  range, and full ROM to remain exact. Evidence: the exact resident
  `func_80035ADC` sprite-frame builder.

- Preserve authenticated array identity when reconstructing entry loops.
  Under unchanged configured full-TU flags, direct integer indexing into a
  bounded global entry array restored source-pointer hoisting and four-entry
  unrolling that an advancing destination pointer did not produce. In the
  same reconstruction, direct indexed digit stores preserved shared quotient
  and remainder calculation without staging extra locals; mixed stores
  through a less specific cursor had duplicated division work. These paired
  builds prove sensitivity to the source's object/index representation. An
  alias-analysis explanation is an inference, not a traced compiler cause.
  Check actual element widths, array bounds, and read/write order before
  applying the lever; do not change storage layout or invent a larger object.
  Compare the affected loop or call interval as well as whole-function size,
  because duplicated arithmetic can cancel a missing loop elsewhere.
  Equal size or an equal relocation count still does not establish a match.
  Evidence: the controlled reconstruction recorded in the
  [overlay 52 HUD handoff](matching-triage-handoffs/func_overlay_052_F000063C_189ACAC.md).

### Search fidelity and false floors

- A permuter zero is a hypothesis until the project pipeline verifies it.
  Function isolation must reproduce the TU's flags and metadata-only object
  steps, and stack-slot differences must be scored. Evidence:
  `docs/matching-triage.md` section "Permuter scratch fidelity."
- Do not call a register/scheduler residual unreachable until the correct ISA,
  faithful scratch build, bounded permuter, and workbench route have all been
  tried. Several Mickey functions first classified as hard scheduler walls
  became exact after those preconditions were repaired. Evidence:
  `docs/matching-triage.md`.
- Permuter output is a source-spelling probe, not a style authority. Preserve
  an exact but inert spelling when necessary, disclose it in source, and put
  naturalization in `docs/cleanup-queue.md`; never keep a semantic mutation
  merely because it scores better. Evidence: ADR 0007 and the cleanup queue.

## Adding a learning

Add a short entry only after the result is reproducible. Cite the durable
Mickey evidence, state where the lever does *not* apply, and prefer updating
the upstream decomp-workbench field guide when the behavior is toolchain-wide.
A one-function curiosity belongs in that function's handoff, not here.
