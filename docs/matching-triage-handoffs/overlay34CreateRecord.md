<!-- plateau-handoff:overlay34CreateRecord:start -->
### `overlay34CreateRecord` plateau handoff

- source: `src/overlays/o034/overlay34CreateRecord.c`
- score: 8/125 words
- frame: 0x30
- relocations: 12
- first mismatch: +0x54
- summary: Three ranking edits took 24 to 8; what is left is one more nocs-merge, on the height web, plus the two words the copy device costs.

#### 2026-09-12, lane p9-mid: 24 -> 8 on the globalcolor records

The closure above asked for "direct new evidence for the missing producer/home
identity". That framing was wrong: the frame and every home have been exact for
some time, and the instrumented globalcolor records show the whole residual was
**rank order among webs that all get a colour**, not a missing home. Nothing here
needed a home, a donor, or a flag.

Aligner buckets before: 101 byte-exact, 22 register naming, 0 immediate-only, 2
really different, at 24 masked and size delta 0. After: 117 byte-exact, 6
register naming, 0 immediate-only, 2 really different, at 8 masked and delta 0.

The instrumented toolchain's `.text` for this TU is byte-identical to the tree's
object, and this TU holds one function, so the procedure ordinal is 0. Every
force below was verified accepted by reading the `forced` field, not by the
object changing.

Three edits, each priced by a force first and then reached from source:

- **The structural pair was L111, not a home.** Writing the cursor advance as
  `record = current + 1` instead of `record++` keeps the `current = record` copy
  as a real node. That gives as1 a third node in the loop's test block, so it
  fills the branch delay slot with the index compare where the candidate had a
  `nop`. Structural bucket 2 -> 0 at an unchanged 24.
- **An L109 or-with-zero probe on the index is worth 6 words.** `CDX_FORCE=p1:w13=c2,p1:w10=c1`
  scored 17 against 24 at delta 0. The two webs are the record cursor
  (totalsave 32, nocs 2, save 16.0) and the loop index (totalsave 31, nocs 2,
  save 15.5), and p1 ranks by `save`, so one unit of totalsave decides which
  takes `v0`. The probe raises the index web to totalsave 51 at unchanged nocs,
  save 25.5, and the probed object is **byte-identical to the forced object**.
  Seven natural spellings that might have added the same weight -- pre-increment
  in the condition, a compound loop condition, subscripting, counting down --
  are all flat at 24 or change the size.
- **Statement order merges a web's occurrences, and that is worth 9 more.**
  `CDX_FORCE=p1:w119=c9,p1:w124=c8` was worth 11 on its own. Those two are the
  literal 2 (totalsave 1.0, nocs 2, save 0.5) and the address of
  gOverlay34ActiveCount (totalsave 3.0, nocs 5, save 0.6). Moving
  `byte12 = 2` one statement earlier, ahead of `short16 = height`, takes the
  literal's nocs from 2 to 1 at unchanged totalsave, so its save becomes 1.0 and
  it outranks the address web. 17 -> 8.

**nocs is per block, not per occurrence.** The probe added a def and a use and
left nocs at 2; the statement move changed nocs without changing totalsave. That
is the lever the rest of this function needs and it is the one worth carrying to
siblings.

**What is left, with its price.** `CDX_FORCE=p1:w45=c3,p1:w51=c2` on the current
baseline scores **4**, so the four words at +0x88..+0xA4 are one more ranking:
the resource-pointer copy web (totalsave 3.0, nocs 1, save 3.0) is decided
before the height web (totalsave 4.0, nocs 2, save 2.0), and the shipped code
decides them the other way round. The height web needs the same nocs merge the
literal 2 got -- it needs nocs 1, which gives save 4.0. Not found yet: a
380-cell exhaustive single-statement-move hill climb over the twenty-statement
store block is at a local optimum of 8, and eleven width/height spellings,
declaration orders, line joins and probes are flat or worse.

The other two open words are the cost of the copy device: `record = current + 1`
makes the increment read `current`, where the shipped code reads `record`, so
`+0x54` and `+0x5C` disagree. Reverting to `record++` scores 9 and loses the
delay-slot fill. The wanted form keeps the copy live *and* increments from
`record`; five loop spellings for it, including a discarded read of `current`
and a self-assignment of `record`, are all flat at 9.

**One inherited claim is refuted.** The in-source note said the target reads
gOverlay34ActiveCount, gOverlay34Pointers and gOverlay34Count as offsets 0, 4
and 8 of one relocated symbol, "so the three were members of a single struct in
the original". The displacements are real and visible in the relocation table --
the shipped code reads the count at +8 and the pointer table at +4 of a base
whose low half is zero, and it carries six relocations against this candidate's
twelve. But writing them as one C struct is **eight bytes short**: IDO reuses
the single base register, while the shipped code materialises a fresh base for
each access. They are separate symbols at adjacent addresses, and splat names
only the first. Do not spend another pass on the struct.

#### Epoch 15 linked near-shape pass (2026-09-04)

- assignment base: `58fac8f8`. Fresh configured preflight reproduces the
  retained 125-word body, 30 relocation-masked and 32 raw positional
  differences, first mismatch `+0x0`, and frame `0x28` versus target `0x30`.
  All 12 candidate and target relocation records still agree by offset, type,
  effective identity, and addend.
- a fresh linked promotion trial classifies the retained C as `text-differs`,
  with 30 in-range words and zero outside the owned function. This is not a
  match; exact linked overlay and ROM evidence still comes from the assembly
  fallback.
- workbench diagnosis reports that the target colors one more integer pool
  web and owns one more eight-byte frame quantum. The one authorized
  structural reshape declared the semantically natural resource result local
  and used it for the candidate store, null test, and two dimension loads.
  IDO folds that local instead of allocating a home: the function becomes 124
  words in the same `0x28` frame, with 109 masked and 110 raw positional
  differences.
- the reshaped linked trial is `text-differs` with 109 in-range and 1,222
  outside words; only 5/12 relocation sites remain aligned. The original C is
  restored. This mechanism does not explain the target's extra web or frame
  home and is now exhausted; resume only with direct new evidence for the
  missing producer/home identity.

#### Whole-itable loop-current reproof (2026-08-31)

- assignment base: `d63836a7`; configured V0 reproduces 500 bytes / 125
  words, 95/125 relocation-aware positional words, 32 raw and 30 masked
  differences, first mismatch `+0x0`, frame `0x28` versus target `0x30`, and
  the previously authenticated 12 relocation identities.
- IDO 5.3 `uopt.c` was regenerated from static-recompiler revision
  `9c242adc` at the pinned `b0058f15...` source digest, instrumented with the
  shipped global-color profile and preserved whole-itable patch, and built in
  a lane-local toolchain copy. With tracing enabled, `.text`, `.data`,
  `.rodata`, relocations, and symbols are identical to stock output.
- the baseline ladder contains eight slots, including six automatic homes.
  It confirms that `record` and `current` already remain distinct frontend
  homes; the missing frame quantum is therefore not explained by the source
  having collapsed those declarations.
- the single authorized source form changed the active test from
  `record->active` to the just-assigned `current->active`. It is byte-identical
  in `.text`, retains 125 words and frame `0x28`, and leaves all comparison
  metrics unchanged. The compiler proves the two expressions equivalent
  before the missing allocation decision.
- no second ordering form was attempted because the first form produced no
  strict frame or word gain. Preserve the retained source and reopen only
  when a producer trace identifies the missing automatic home or delayed-bound
  carrier directly; do not repeat flags, loop spellings, or broad permutation.
<!-- plateau-handoff:overlay34CreateRecord:end -->
