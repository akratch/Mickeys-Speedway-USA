<!-- plateau-handoff:func_overlay_079_F0000134_18CD0D4:start -->
### `func_overlay_079_F0000134_18CD0D4` plateau handoff

- source: `src/overlays/o079/func_overlay_079_F0000134_18CD0D4.c`
- score: 198/882 words
- frame: 0xB8
- relocations: 88
- first mismatch: +0x7C
- summary: Two source-order probes: inline dx regressed to 238; dz-before-dx was byte-identical. web346 and one folded FP draw remain.

Added to the flat list this lane, on the same base:

- all 18 adjacent swaps of the twenty declarations, scored 198 to 206, with
  `u04` and `u15` tying the incumbent at 198. None of them moves the differing
  spilltemp home: the frame census still reads +0x40 for us against +0x3C for
  the target on every one of them, so declaration ORDER is not what places it,
  which is a different axis from the "adding any local grows the frame" note
  already recorded and it is now covered too.
- ten pad-local probes, an unused `f32` and an unused `void *` each at five
  declaration positions, 245 and 246 with the frame growing 0xB8 to 0xC0 in
  every case. The local block really is quantized in eight-byte steps.

The instrument recipe, for the next lane: set IDO_DIR to the instrumented
toolchain, DKWB_UGEN_SCHED=1 and DKWB_UGEN_TRACE=1, and read the
`DKWB-FREELIST ALLOC_FP_RESULT` rows for this procedure; each is stamped with
its source line, so the draw index at any source statement is a direct
readout. `CDX_PROC=0 CDX_LOG=1 CDX_OUT=<path>` gives the colouring records for
the same procedure, where float webs print `reg=?` and colours 24 to 30.

## 2026-09-12 (lane `p12-o57`): the float bank priced from the records; nothing adopted

Unchanged at 198 masked of 882 words, size delta 0, byte-exact 703, register
naming 148, immediate only 3, really different 30, frame 0xB8 both sides. This
entry is a set of precise negatives and one priced decision; read it before
spending on the float axis again.

### The colour table, decoded on this procedure

`CDX_PROC=0 CDX_LOG=1 CDX_OUT=<path>` on the instrumented toolchain, identity
gate PASS against the configured object (`-Wab,-r4300_mul` included; without it
the gate is wrong on both sides). 77 decisions, one procedure.

Float colours are 24 through 30, and a `forbidden0` or `available0` bit for
colour c is bit 31-c, so colour 24 is bit 7 and colour 30 is bit 1. Colours 24
to 28 are the pool registers f0 f2 f12 f14 f16. **Colours 29 and 30 exist and
are real**, which is new: a web can be coloured with a register the five-entry
scratch ring would otherwise hold, and that shrinks the ring for the whole
procedure.

### Priced: the +0x368 colour is worth three words, and one interferer blocks it

The first naming difference, +0x368, is `dy` at the three-component distance
test. Our web 143 takes colour 28 (f16); the target takes colour 25 (f2).

Forcing web 143 onto colours 24 to 27 returns `forced=-2` every time -- they are
in its `forbidden0` mask (0xf0), genuine interference, which L101 says no ratio
or spelling reaches. **But the interferer is nameable and freeing it works.**
`CDX_DETAIL_WEB=143` lists web 346, assigned colour 25, among its interference
neighbours. Forcing web 346 to colour 26 makes web 143 take colour 25 without
being forced at all, and reproduces the target's instruction at +0x368
exactly. That is 198 -> 195.

So the decision has a name, a mechanism and a price: three words. Web 346's
save is 3.0 at nocs 1; web 143's is 2.5 at totalsave 5 over nocs 2, so 346 is
offered a colour first and takes f2. Raising web 143's save above 3.0, or
lowering 346's below 2.5, is the source-side requirement.

Eight spellings of the distance block were measured against that requirement --
`dy` declared first, last and split; three associativity and summation orders;
a named `distance` local; and a `+ (dy * 0.0f)` probe -- and none reaches 195.
Three are byte-identical to the base. The float analogue of L109's
discarded-expression probe does not count: `dy * 0.0f` is folded away before
uopt counts references.

### Refuted: colours 29 and 30 do not buy the ring phase

The fp ring phase at +0x494 -- we take f18, the target takes f4, one draw apart
in the five-cycle f4 f6 f8 f10 f18 -- would follow from the target colouring one
web with a register the ring holds. That is now testable, and it is refuted:
forcing each of the six webs whose `bestcolor` is 29 or 30 onto that colour is
accepted (`forced=29`, `forced=30`) and scores 713 to 877 against 198. The
ring does shrink; everything else collapses.

### Flat this lane

  - **`tools/blockclimb.py`, guarded, 745 compiles, two passes: 198 -> 196, and
    the move is SEMANTICALLY INVALID.** It moves `start.y = object->y +
    state->heightOffset;` to after `end.y = start.y + dy;`, which reads
    `start.y` before it is assigned. The tool's guard excludes statements
    containing calls, not data dependencies, and its own docstring requires
    every hunk to be read. Rejected. This is the first recorded case on this
    project of the guard passing a read-before-write.
  - all 36 permutations of the `start` and `end` component store groups: best
    197, and that form's aligned residual is 187 against the base's 181, so the
    one word is displacement rather than agreement.
  - ten byte-inert probes at the mode-0 dot product -- `forward.y` assigned
    from `forward.x`, three orders of the `forward` stores, two operand orders
    and a split of the dot product, a `dy` alias for `forward.z`, and negating
    the `dz` subtraction. Five are byte-identical to the base; the best is 198
    and the worst 574. This is L148 holding: the float no-op family draws no
    ring temp.
  - splitting `dz = state->targetZ - object->z` the way `dx` is already split
    is 205 with a better aligned residual (177 against 181) and does NOT move
    the ring phase; the load still lands in `dz`'s pool colour where the
    target's lands in a ring temporary.

### What is left

The ring phase is still one fp draw and it is still unexplained. What the
records now rule out is that it is a colour: no float web can be moved into or
out of the ring without a 500-word regression, and every pool colour above save
4.25 is already forbidden or split. What remains is an L149 folded draw -- an
operation whose result is drawn and whose instruction as1 then deletes -- and
the generator for it on the float side has not been found. `DKWB_UGEN_SCHED=1
DKWB_UGEN_TRACE=1` gives 168 `ALLOC_FP_RESULT` rows stamped by source line for
this procedure; the draw at line 412 is `f18` where the target's is `f4`, so
the target spends exactly one more before that line, in code that is
byte-identical to ours up to +0x490.

The 0x40 against 0x3C spilltemp home is unchanged and still open.

Validation: `gmake verify` printed
507341c0a40ca3e9a7cee969b396ee53facfb548 and `tools/gates.sh --staged` passed
all four gates. No source change was adopted.

<!-- plateau-handoff:func_overlay_079_F0000134_18CD0D4:end -->
