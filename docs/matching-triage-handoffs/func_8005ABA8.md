<!-- plateau-handoff:func_8005ABA8:start -->
### `func_8005ABA8` plateau handoff

- source: `src/main/models_5B300.c`
- score: 2 differing words
- frame: frameless
- relocations: 0
- first mismatch: +0x3C
- summary: 111/111 words, every register and branch target exact; the last two words are one as1 delay-slot decision at +0x3C.

#### Owned evidence and result, 2026-09-09

47 differing words to 2, at 111/111 words, frameless, zero relocations, with
every register name and every branch target now agreeing. Four source
artefacts closed 45 of them and none was an allocator question:

- `temp_f0_2` cached `instance->frameValue` for two tests that no store
  separates. The cache costs a `mov.s`; the target reads the field at both
  sites and lets uopt common the load, and the `nop` the target shows at that
  hazard slot is exactly what the copy was filling. Worth 27 words.
- The two blend stores were written sub-then-div. The target allocates the
  div's FP temporary first, so the source computes `blendStart` before
  `blendEnd`; both read only locals, so the order is free. Worth 4 words.
- The null test spelled through a cached `temp_a1` gave the loaded pointer a
  copy and exchanged `a1`/`a2` across both frame carriers and their two later
  uses. Testing `temp_v0->frame == NULL` directly lets the load keep `a1` and
  the surviving carrier take the copy into `a2`. Worth 7 words, and it is the
  opposite of the lever the previous handoff filed: the second declared
  pointer was the *cause* of the exchange once the size question was closed.
- `var_v1 = 1` written before the inner `if` of each arm rather than once
  after it moved no instruction at all, but made as1 duplicate the join's
  `move v0,v1` into two annulled delay slots the target leaves as `nop`.
  Worth 5 words, and it is the proof that this class is source-reachable.

`temp_f2_2` was an m2c-only second name; one carrier serves both `blendEnd`
reads. Removing it, and the now-dead `temp_a1`, is byte-inert.

Remaining: one as1 delay-slot decision at `+0x3C`. The target branches `beqz`
with a `nop` to a block whose first scheduled instruction is
`mul.s $f18,$f14,$f12`; as1 turns the same branch into `beqzl`, duplicates
that multiply into the annulled slot and retargets past it, which leaves the
original copy unreachable and both sides at 111 words. Twenty-eight further
shapes of the else-block head, the transition test, the declaration list,
the comparison spellings and the placement of the product are flat at 2.
Resume on the ugen text as1 consumes, not on the schedule: the `var_v1`
result above proves a statement move that changes no emitted instruction can
still flip this decision.

Tooling: the permuter's isolated scratch for this TU compiles the function at
112 words against the real object's 111, so its base score of 400 is a false
reading and nothing measured there transfers. `tools/permute.sh` on
`models_5B300.c` should not be trusted until that is fixed.

#### Second owned pass, 2026-09-09 (lane/fin-mod)

Still 2. What this pass bought is a proof of *where* the two words live and a
large set of retired levers, not a match.

- **Phase replay is exact for this TU.** `cc -S`'s ugen listing, re-assembled
  with `cfe -E` + `as0` + `as1` at the compiler-path flags (drop the driver's
  `-pic0`, keep `-O2`), reproduces the real object byte for byte. One candidate
  costs ~60 ms, so the last phase is a directly searchable space. Watch out:
  `cc -S -o <path>` ignores `-o` and drops the listing in the *current
  directory* -- move it into your scratch dir before anything else.
- **The residual is one assembler decision and nothing else.** Inserting a
  single location-counter directive (`.align n`, `.space 0`, or `.text`)
  immediately after the else-arm's label in the ugen listing suppresses the
  branch-likely conversion and yields a **byte-exact 111-word object**. Every
  other word of this function's C is therefore already the target's C.
- **The suppressor set is exactly those three directives.** Everything else
  ugen can emit inside a function body was tested at that position and at ten
  others and is inert: `.loc` (every line value, inserted, deleted and moved),
  `.livereg`, `.noalias`/`.alias`, `.mask`, `.frame`, `.file`, `.option`,
  `.verstamp`, an extra label, a padding instruction, a redundant jump. ugen
  emits `.align` and `.text` only at function *starts* (11 of each for this
  TU's 11 functions), so the suppressor is not reachable from C.
- **Physical line grouping is retired for this residual.** Grouping changes
  only `.loc` lines, and every `.loc` edit anywhere in the function is inert
  here. Do not spend another pass on it.
- **No cross-function state.** Dropping any other function from the listing
  leaves the decision unchanged, so the space is this function's own ugen text.
- **Single-line ugen-text moves do not reach it.** All 43,056 single-line moves
  (every line of the function to every other position in it) were scored. One
  variant reaches 1 -- moving the else arm's label deep into the block, which
  makes the target head a branch-class instruction and suppresses the
  duplication, leaving only the branch's own displacement wrong. That variant
  is not semantics-preserving, but it is the measurement that pins the rule
  below: **only a branch-class head, or a location-counter directive, makes the
  assembler decline.** Mapping every instruction of the function to the head of
  that block confirms it: 17 of 85 decline, and all 17 are branches.
- **Insertions the assembler removes are inert too.** A dead register-to-
  register copy vanishes from the object (the word count stays 111), so it was
  the last candidate for a phase-input difference that leaves no trace; 836
  such insertions, at every position in the function, all still convert. The
  same goes for self-copies, zero-adds, an encoded no-op and a redundant jump.
- **384 C spellings are flat.** Cross product of: the three integer tests
  plain vs `!= 0` vs `!= 0U`, the two float tests as `>=` vs negated `<`, the
  blend statement order, the disjunction spelling, `frame` carrier placement,
  `while` vs guard-plus-bottom-tested loop, the product in a local, and the
  explicit-add form. Assigning `frame` inside the else arm sinks the carrier
  copy below the test and costs 45 words: the carrier must be assigned before
  the test, as it already is.
- **How the conversion actually behaves**, measured on this function's nine
  other conditional branches: the assembler duplicates the first *scheduled*
  instruction of the branch-target block into the annulled slot and retargets
  past it. It declines only when that head is itself a branch, when the head
  has been hoisted out from under its own label by an earlier assembler
  transformation (retargeting such a branch to the next label restores the
  conversion), or when a location-counter directive stands at the head.
  Liveness of the duplicated destination on the fall-through path is *not* a
  factor (renaming it in either direction is inert), and neither is the head's
  opcode class -- integer branches duplicate float arithmetic here.
- One unexplained sensitivity worth a follow-up: replacing the byte load that
  defines the test's condition register with an ALU definition in the same
  block makes the assembler decline. Both sides of this target define that
  register with the same byte load, so it does not explain the residual, but
  it is the only input perturbation found that flips the decision without a
  directive.

Next worker: do not re-run the source-spelling or line-grouping space. The
open question is narrow -- what ugen-emittable input makes this one branch
keep its own delay slot -- and the phase-replay harness above is the tool for
it.

<!-- plateau-handoff:func_8005ABA8:end -->
