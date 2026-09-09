<!-- plateau-handoff:func_overlay_060_F0000334_18BA10C:start -->
### `func_overlay_060_F0000334_18BA10C` plateau handoff

- source: `src/overlays/o060/overlay60Prefix.c`
- score: 4 differing words
- frame: 0x198
- relocations: 838
- first mismatch: +0x182c
- summary: NON_MATCHING: code identical to the target; residual is one spilled records-panel temp homed at 88(sp) instead of 96(sp) (spill-slot ordinal one notch low).

This packet reopened only decompiler-assisted structural reconstruction from
Mickey's extracted assembly. The prior size-deficit plateau was read before
editing, and the configured baseline was reproduced at assignment base
`cbd755d9b2e828b5f4c05a4efe81d70353d205cb`.

Measured outcome (configured full TU, compiled `NON_MATCHING` C):

- Owned target: overlay 60, text `[0x0334, 0x2EC8)`, ROM
  `[0x18BA10C, 0x18BCCA0)`, 11156 executable bytes / 2789 words.
  The terminal return delay slot is included; section padding adds no credit.
- Baseline: 3484 bytes / 871 words, 2775 raw and relocation-masked differing
  words, first mismatch `+0x8`, frame `0x198`, 92 compiler relocations.
  The target has 838 runtime relocations: 244 calls and 297 HI/LO pairs.
- Retained candidate: 11144 bytes / 2786 words, 2519 raw and 2511
  relocation-masked differing words, first mismatch `+0x4`, frame `0x198`,
  864 compiler relocations: 244 calls and 310 HI/LO pairs. The generated
  score above is the masked workbench value, not a matched-word count.
- The size deficit fell from 7672 bytes to 12 bytes. This is source and
  structural progress only: zero new matched-byte credit, no atlas or
  symbol-table promotion, and no canonical linked-C or full-ROM proof.
- An ordinary diagnostic link of untouched compiler output at module offset
  `0x334`, using the authenticated stored addends, differs from the owned
  baserom range in 2512 words, beginning at `+0x4`. Its import aliases and
  compiler-owned case-table placement are diagnostic, not canonical proof.

Recovered evidence and source decisions:

- Tier B: all 244 target call records resolve to 63 distinct callee identities.
  The candidate has the same ordered call identities. A static traversal also
  agrees on every next-call adjacency set and all ten dispatch-entry first-call
  sets. This check abstracts branch predicates; it is call-CFG evidence, not
  whole-function semantic equivalence or matching acceptance.
- Tier B/D: real `ControlActor`, `AnimPath`, matrix and graphics types replace
  draft scalar guesses. Save slots contain four eight-byte records with a
  word time, three initial bytes and a character byte. The two coordinate
  tables have four signed-halfword XY records. Settings reads and
  byte-preserving writes use the recovered bitfield positions.
- The reconstruction restores all ten panels, four detail modes, record and
  character loops, model updates, graphics commands, settings and input paths.
  Source was written as ordinary C from Mickey-only evidence. The raw m2c
  draft remains ignored and was not copied into tracked code or comments.
- Draft mistakes corrected before retaining the candidate include reloading
  the selected model and object after the animation callback, rechecking the
  current detail state after navigation/Back callbacks, preserving word-sized
  title/list coordinates, and reading enabled entries as unsigned bytes.
  Initial conversion preserves the target's two narrowed results and full
  third result; model setup passes a floating-point zero.
- The rank lookup uses a byte span that includes the preceding byte, keeping
  the observed predecessor-byte read defined. IDO folds this to a different
  address/addend spelling. It must not be replaced by an out-of-bounds read
  solely to recover the target relocation.
- The physical display-list alias denotes overlay-local data plus its physical
  bias. It is not a resident global. Its canonical alias and runtime relocation
  binding remain unproved; a zero-valued proxy link cannot close this task.

Relocation and remaining-mechanism limits:

- The explicit Mickey runtime-identity adapter resolves every candidate record.
  Its diagnostic multiset intersection is 808 of 838 target records; 862 of
  864 candidate type/identity pairs occur somewhere in the target, and only
  155 identities align at the same offsets. None of these aggregate facts
  means the required offset/type/identity table is exact.
- The standard relocation-surface path still conflicts with the resident-name
  namespace for `D_800D312C`. The ignored adapter records the authenticated
  runtime namespace explicitly; no shared tool or symbol policy was changed.
  Checkpoints conservatively report zero acceptance-proven exact identities.
- Most extra relocation pairs reload the menu-owner global whose address the
  target retains, while the candidate retains the settings address instead.
  The remaining inter-call length differences also include stack homes,
  register choices, branch duplication and the rank address spelling.
- Initial workbench routing was `structure` / `structure-buckets`: recover
  complete control flow and typed identities before local allocation work.
  Final verdict remains `structure-mismatch`. The `constant-audit` footer was
  checked against Mickey's literals and explicit diagnostic bindings; its
  remaining aligned constants include address and stack-placement differences.
  No flag lattice, permuter, instrumented allocator, instruction editing,
  added padding or inert source-shaping mechanism was substituted.

Stopping evidence and retained alternatives:

- Attempt 26 established the best admissible positional score: 2511 masked
  differing words, 2786 words, frame `0x198`. Attempt 28's complete settings
  view ties that compiled result and is the retained source.
- Attempts 27 through 31 are five consecutive non-improving source-form checks:
  separate character index (2703 masked differences, 2769 words); one complete
  settings view (2511, 2786); early readiness returns (2511, 2786); inner detail
  equality chain (2643, 2784); explicit object-scan increment (2511, 2786).
  All have frame `0x198`. These variants established no new identity or causal
  mismatch classification and did not eliminate the remaining global-address
  or stack-context mechanism. The best residual stayed fixed throughout.
- Earlier structural gains remain useful even where a positional score fell:
  both four-coordinate loops now compile with the target region lengths, and
  separating stereo, screen and preview values recovered the target frame.
  The guarded source was banked continuously in `8df7af84`, `e0eadbdf` and
  `d0526960`; this handoff commit preserves the final best reconstruction.
- Every material attempt retains source, configured object, score, first
  mismatch and hypothesis under ignored `build/whale1/attemptNN/`. Baseline,
  draft, typed context, identity analysis and final receipts are under ignored
  `build/whale1/`; generated drafts are also under ignored `m2cfiles/`.
  The retained baseline object's digest was checked against its original
  measurement. Do not overwrite these alternatives during integration.

Validation and next action:

- Commands run include `tools/wb_compare.sh` against the configured full TU,
  `decomp-workbench diagnose`, `guide structure-buckets`, `guide constant-audit`,
  `generate_ctx.sh`, and `mips_to_c.sh`. The full generated context exposed
  unsupported declarations in m2c, so a narrow Mickey-only typed context was
  used for draft analysis. Scratch identity, memory-width, call-CFG and owned
  range diagnostics provide the additional receipts described above.
- `gmake check-nonmatching-builds` passed all 243 candidate-bearing TUs.
  `gmake cleanroom` and `gmake check-docs` passed, including the plateau audit.
  `tools/finalize_plateau.py` guarded and recorded this measured candidate.
- `gmake verify` failed at the same pre-existing overlay 94 canonical-link
  error reproduced before any source edit. `overlay94UpdateController.c`
  has unresolved resident calls and truncated call relocations; the final
  log again names `func_800254FC`, `func_8002A878`, `func_8002565C` and
  `func_8002A910`, plus truncated calls to `func_8005ABA8`, `func_8005AF14`,
  `func_80019AB8` and `func_8002B040`. No full-ROM verification passed.
  Logs remain in ignored `build/whale1/final/`.
- Next concrete lever: a separately authorized causal packet should explain
  why the menu-owner address loses its persistent register while the settings
  address gains one, using this full reconstructed TU and the stack-resident
  enabled-count value as the baseline. This assignment did not authorize that
  allocator experiment. Keep the rank byte-span and physical-list alias
  questions explicit rather than masking their relocation differences.
- Before promotion, resolve canonical overlay proxies and case-table placement,
  require exact owned bytes plus exact relocation count/type/offset/identity,
  extract the linked owned range against the same baserom offsets, and obtain
  a passing `gmake verify`. The existing overlay 94 canonical-link failure
  also needs a separate owner; this lane did not edit that source.

#### 2026-09-09 dedicated grind (lane/whale-o060)

Measured with a direct `tools/ido/cc` compile at the TU's real flags, verified
byte-identical in `.text` to the asm-processor `NON_MATCHING` object. Scores
are relocation-masked; "identical rows" is the shift-tolerant count of exactly
identical rows (Lever 48).

- Start: 2503 differing words, 1354 identical rows, size delta -12,
  mnemonic census within 45, `&gOverlay60Data0A8` rematerialised 24 times.
- End (committed `03d269b3`): 97 differing words, 2647 identical rows, size
  delta 0, census within 4, first mismatch `+0x9a4`. Every saved register is
  coloured as the target (A8 s2, 160 s0, 2B8 s6, 14C s3, 158 s4, 2B4 s5,
  150 s7, 2B0 s8); the settings word is rematerialised 28 times as in the
  target; the case-3 counter is uncoloured in v1 and spilled to 372(sp)
  fourteen times as in the target. The first 0x9a4 bytes are positionally
  identical modulo relocations.

Identities proved (each flips the residual when reverted):

- Tier B: the settings word, its bytes at +0x13/+0x14 and the halfwords at
  +4/+6/+8.. are one object at `D_800D3128`. The overlay's runtime relocation
  table resolves all 28 sites in this function to symidx 0x62b with addends
  0, 2, 4, 6, 8, 10, 12, 19 and 20. Spelled as one struct, the settings
  address stops being promoted and is rematerialised exactly as the target.
- Tier B (colouring): the loop index and the case-2/case-6 value are one
  variable; the case-3 counter is also the records panel's glyph-loop index
  (a fresh variable there loses the v1 spill and 750 words); the case-6
  wide-adjust value is not the loop index (target keeps it in a0); one
  `previewMode` copy of `gOverlay60Data150` serves case 4, the records
  panel, the preview panel and the character panel's do/while walks
  (splitting it in any direction regresses by 700+ words); the copy is
  refreshed after navigation (the target's delay-slot reloads; ours had
  stored the stale value into `gOverlay60Data130`).
- Tier D (frame census): fourteen scalar locals above `minutes` with the
  counter ninth put the spill on the target's 372 slot; the eighth scalar's
  use is unrecovered (`spare`). A fresh pointer local below the arrays costs
  65 words wherever it is declared, so the pointer census is exact too.
- The four settings-bit toggles are `(f ^ 1) & 1`: the comparison-style
  RHS allocates the value temps before the merge's byte read (byte read
  takes t8 as in the target) while keeping `xori`; `^= 1` alone displaces
  the temp ring for the rest of the function (1010 -> 140 words).
- Statement orders fixed by the target's delay slots: `sprintf` before
  `icon = 0x4A`; `glyph[0]` stored before `glyph[1]`; `for (i = 0,
  count = 0; ...)` in case 3.

Mechanism of the owner-address promotion (the previous plateau's cause):

- uopt promotes a global address to a saved register only when its weighted
  reference count is large relative to the function's unit count, counted
  over the whole function (blocks split at calls; blocks after the last use
  still count; loop uses weigh about eight plain uses; CSE-folded duplicates
  count once). In a P1-style probe with the switch removed, one added
  conditional block or one call anywhere in the function drops the
  promotion; each extra use buys about two blocks.
- In the full body the address is shredded not by that cutoff but by the
  colouring race: with the settings word promoted and the loop counters
  shared, every saved register has an occupant somewhere inside the owner's
  live range. The identities above leave s2 free.
- Eliminated: the settings address as a competitor (removing every settings
  reference leaves the owner shredded), declaration order of globals and of
  locals (inert), static linkage, the search-loop store, ghost loops,
  distinct-symbol spellings, and every flag set. `-Wo,-loopunroll,0` and
  `-sopt` promote the owner only by perturbing the same race and are wrong
  elsewhere (-396 and +104 bytes).

Remaining residual and the next lever:

- All 92 remaining positional differences are one register-class cascade.
  In the preview panel's model loop, ours forms a CSE web for the rank
  table base (`gOverlay60Data12B + 1`, two uses) and hoists it into s5;
  `previewMode` then falls to v1 everywhere, `limit` to a0 instead of v1,
  and the 14C value to a0 instead of v1. The target keeps `previewMode` in
  s5 everywhere and rematerialises the rank constants three times inside the
  loop (`lui/addiu` of data+0x12C for the base, the pointer compare and the
  strength-reduced loop end).
- Proved: any spelling that stops the two rank uses from being one CSE web
  (a different symbol for the compare) gives `previewMode` s5 at all ten
  sites and rematerialises the base and the loop end exactly as the target;
  what those spellings cannot produce is the target's bare folded constant
  at the compare (`sym + const` written by hand never folds; only uopt's own
  address arithmetic folds, e.g. `1B4 + 22` in the glyph loop, and a struct
  member base folds but then CSEs with the shifts pointer into s5). A
  diagnostic far-away read of the table also flips the order, so the web's
  priority is units-driven; the target references the table only at the
  three loop sites, so the original's form is still unknown.
- Also open: the advanced save-slot pointer is a separate web (target
  `slots` in s0, the blur-advanced slot in s1, `record` initialised from s1);
  ours reproduces that only by reusing an existing pointer local (92 words),
  because the pointer census is exact. Which of the eight pointer locals it
  is has not been identified; the committed form keeps `slots +=` (97).
- The two compiler temp homes (target 80 and 96, ours 84 and 88) follow the
  same cascade and were not chased separately.

Harness left in ignored `build/scratch-w60/`: `measure.py` (direct-cc
compile, masked positional and shift-tolerant scores, census, diff),
`sig.sh` (pre-switch colouring signature, spill and first-mismatch readout),
`firstmm.py`, `structdiff.py`, `sregs.py`, `regs_in.py`, `occupants.py`,
`bucket.py`, `rtrel.py`/`rtrel2.py` (runtime relocation identities from the
ROM tables), `mergetool.py`, `splitloop.py`, `sweep.py`.

#### 2026-09-09 second grind (lane/whale2-o060): 97 to 4 words

Measured with the same direct `tools/ido/cc` harness (`build/scratch-w602/
measure.py`, flags recovered from `gmake -n`, byte-identical `.text` to the
asm-processor object). Scores are relocation-masked positional words;
"identical rows" is the shift-tolerant count (Lever 48).

- Start: 97 words, 2647 identical rows, census delta 4, first mismatch
  `+0x9a4`. End: 4 words, 2703 identical rows (the other 86 rows differ only
  in relocation masking), census delta 0, size delta 0, first mismatch
  `+0x182c`. Every instruction of the 2789 is identical to the target; the
  four words are two `sw` and two `lw` of one spilled temp at `88(sp)` where
  the target homes it at `96(sp)`.

Identities proved (each reverts the residual when undone):

- Tier B: the rank lookup is an index, not a pointer. `spare` (the eighth
  scalar, previously unrecovered) holds `(progress[Data150] & masks[i]) >>
  shifts[i]`; the table read is `gOverlay60Data12C[spare - 1] * 256` and the
  draw test is `spare != 0`, with `gOverlay60Data12C` a distinct symbol at
  data `+0x12C`. uopt keeps `&table + spare` live across the two calls in
  s2, folds the `-1` into the `lb` displacement and rematerialises the bare
  folded `data+0x12C` constant at the compare, exactly the target's shape.
  This answers the first cascade question: there is no CSE web because the
  source never spells the table address twice. 97 to 16 words alone;
  `previewMode` takes s5 at all ten sites and `limit` and the 14C value
  return to v1.
- Tier B: the screen-mode panel's wide-adjust value is `row` (a0 with no
  copy). With `spare` taken by the rank index, `spare` there costs an `or`
  and a3. `minutes`/`seconds`/`hundredths`/`panel` also give a0 but cost 1
  word elsewhere; `i` costs 4.
- Tier B: the blur-advanced save block is a second pointer local (`slot`,
  in the ninth pointer's slot; `rank` no longer exists) computed as byte
  arithmetic on the named blur index: `i = levelGetBlurEffect(...)`; `slot =
  (SavesSlot *)((u8 *)slots + i * 32)`. This gives the target's `addu s1,s0,
  t7` operand order; every scaled pointer add (`slots + n`, `&slots[n]`,
  integer casts with an inline call result) puts the index first, as probes
  `build/scratch-w602/probe1.c`/`probe2.c` show. Reusing `objects` or `path`
  for it scores the same 5 words, so the type is the only evidence for the
  identity.
- Tier D: `record` is an explicit induction pointer beside `row`: `for (row
  = 0, record = slot->records; row < 4; record++, row++)`. Code identical;
  it moves the spilled name-table pointer's home from 88 to 92.

Mechanism of the remaining four words (proved by diagnostics, kept only as
evidence): uopt gives every "troubled" web (one that fails first-attempt
colouring) a frame temp slot in colouring order, top-down from the bottom
of the local block (probes `probe5.c`/`probe6.c`: the last-coloured webs sit
at the lowest addresses; adding an unused local moves every slot by 4).
Fifteen such webs exist here in both builds (frame tight at 0x198; a
sixteenth would show as 0x1A0). The spilled `gOverlay60Data1A4 + row*4`
pointer is the 12th in ours and the 11th in the target; the hoisted
`(f32)ticks` temp is the 15th in both. The order follows Chow priority:
adding three empty conditional blocks to the records loop (more units)
pushes the pointer down to 88 and 84; one extra reference to
`gOverlay60Data1A4[row]` in the loop pushes it up to exactly 96 (with
different code). So the target's name-table pointer web carries one more
reference, or one of the webs just above it carries one fewer, than any
spelling found here. Webs shown to sit below the pointer (an extra
reference to each moves the pointer down): the `gOverlay60Data1D0` row
pointer, the `gOverlay60Data1B4[0]` load, the records panel's
`previewMode` and `record`.

Eliminated (all code-identical, slot unmoved): every declaration order of
pointers and register-only scalars; dead extra definitions of
`previewMode` (eliminated before allocation); CSE-merged global re-reads at
every `previewMode` use; every pre/post-increment, `<`/`!=`/`<=`, `for`/
`while`/`do` form of the records, glyph, model, coordinate, enable and
character loops; statement line joins across the panels; a named `objects`
induction pointer for the model loop (moves the float temp instead); a
named `rank` pointer with the compare spelled through `&gOverlay60Data11C
[4]` or `&gOverlay60Data12B[1]` (folds, but the pointer form reopens the
s5 race: 790+ words); all 3292 pairs of the 82 individually neutral
rewrites. The decomp permuter's scratch scores this TU at base 4212 and does
not transfer. Deleting any of 319 statements outside the records and model
loops leaves the slot at 92, so the troubled set is local to those loops
and the function-wide address webs they displace.

Sharpest statement for the next worker (measured after the commit above):
a reference to `gOverlay60Data1A4[row]` placed anywhere in the records loop
BEFORE the name draw call, in a form that emits no code (an empty
`if (gOverlay60Data1A4[row] == NULL) { }`, a dead assignment of it to any
register-only local, `while (...) { break; }`), moves the pointer's home to
exactly the target's 96(sp) with the frame and every other instruction
unchanged -- so the target's name-table pointer web carries one more
counted reference than ours. The same reference also flips uopt's
loop-exit induction variable from the name pointer to the y-table pointer
(three-word delay-slot residual, score 5), because the exit test goes to
the last-created induction pointer and the early reference now creates the
name pointer's first. A reference after the draw never counts (CSE'd or
dead-eliminated before priorities), and creating the y-table pointer first
with a counted reference raises that web above the name pointer instead.
The missing piece is therefore a natural source form that references the
name before it is drawn without a matching load in the output, while the
y-table pointer is still met first: a construct folded after the priority
census. Candidates untested: a `switch` or `?:` on the name that folds; the
name passed to a compiled-out macro that still evaluates its argument in
this compiler; or an argument-evaluation order that meets the y table
before a real second use of the name. Harness in ignored
`build/scratch-w602/`: `measure.py` (prints frame, temp homes, positional
and shift-tolerant scores), `sweep2.py` (parallel single-rewrite sweep),
`pairs.py`, `variant.py`, and the `subs*.py` rewrite libraries with every
experiment above (`subs27`--`subs33` are the vacuous-reference series).

<!-- plateau-handoff:func_overlay_060_F0000334_18BA10C:end -->
