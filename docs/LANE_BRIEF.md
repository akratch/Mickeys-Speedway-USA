# Standing lane brief

Read this once at the start of a lane. Your dispatch message carries only your
targets, their measured numbers, and whatever is specific to them; everything
that is true for every lane lives here so it is written once and stays correct.

This file is the authority. If a dispatch message contradicts it, the dispatch
message is stale — say so in your report.

## What you are doing

Mickey's Speedway USA, a byte-identical matching decompilation for the N64. The
ROM must rebuild to SHA1 `507341c0a40ca3e9a7cee969b396ee53facfb548`. Read
`CLAUDE.md` in your worktree before anything else; it carries the build rules,
the clean-room policy, and the overlay promotion sequence.

Work only in your own lane worktree. Never touch the integration worktree or
another lane. Do not merge, rebase, or push — the coordinator integrates.

## Measure the cause before choosing a lever

`tools/align_symbol.py <symbol>` aligns your candidate against the target on a
register-erased shape and splits the residual four ways. Run it first, and
again after every adoption:

| bucket | what it means | where the lever is |
|---|---|---|
| byte-exact | agreed | — |
| register naming | same instruction, different register | allocation |
| immediate only | same instruction and registers, different displacement or constant | the frame, or a wrong literal |
| really different | different instruction, or present on one side only | structure |

It also prints a **displacement tax** — how much of the positional count is
pure shift — and the offsets where instructions are surplus or missing. On a
size mismatch that last pair is the whole question.

`tools/register_census.py <symbol>` says *which* registers the two sides disagree on and whether they form a cycle. A closed cycle over the temp ring is one ring-phase fact with a single cause (L127, fixable from source at zero byte cost); scattered substitutions are per-web colour questions. Reading a cycle as N colour problems is how a lane spends a day on the wrong axis -- on one function 195 of 279 words were a single four-cycle.

  **Read the coherence line, not just the cycle.** A cycle in an incoherent mapping is not a ring phase: a transfer was applied on cycle presence alone and refused. The lead that transferred reads 88% of substitutions following their source's dominant target; the sibling that refused reads 51%, with one source register splitting three ways. Check the size delta too -- the lead was +4 with identical frames, the sibling -76 with frames 0x10 apart.

`tools/frame_census.py <symbol>` censuses both sides' stack slots and diffs
them: each side's ladder from the frame top down, the slots only one side uses,
and shared slots with different traffic. Run it on any residual with a frame or
displacement component -- three of six closures re-tested on 2026-09-11 were
home-set problems nobody had measured. It reads the objects, so it is symmetric;
`cc -g3` names which of *your* slots are declared locals rather than compiler
temps, which is the other half of L118.

`tools/score_symbol.py <symbol>` gives the positional masked count, agreeing
with the ranking by construction. **Work the masked number**, never the raw
one: the difference is relocation artefacts, already partitioned and stored.

**Concurrency.** `score_symbol.py`, `align_symbol.py` and `frame_census.py`
each swap `nm_ranking`'s work directory for private scratch, so any number of
them can run at once in one worktree — measured: three in parallel return the
same numbers as a serial run. **`nm_ranking.py` run directly does not**, because
a whole-queue pass is meant to own the tree. So do not start a `--refresh-stale`
or `--out` pass while a harness is measuring in the same worktree; that is the
one combination that collides.

## The call test — ask this before any allocator work

Every procedure that issues a call emits **p1 allocator records only**; every
leaf emits **p2 only**. 59 procedures classified, zero counterexamples.

- **Your function contains a call** ⇒ p1 only ⇒ the axis is the `save` ratio
  (L100). **Definition position, declaration order and statement order decide
  nothing.** L106 does not apply. A closure that swept them measured an axis
  the function does not have.
- **Your function is a leaf** ⇒ p2 only ⇒ the axis is ascending web number, and
  moving a defining statement is a lever (L106).

This costs one look at the body and it retires or opens a whole family of
levers. Two lanes were sent after L106 on p1-only functions before this was
known; both spent a sweep finding out.

## Treat a closure as a claim, not a fact

A plateau handoff saying "N forms measured, all flat" is evidence about the
axis its author varied. The measured base rate here is poor: of sixteen
closures re-tested, **ten fell**. One recorded 4,190 flat forms — an exhaustive
4,096-cell lattice plus 82 declaration-order forms — and the function then
matched on a single hoisted definition. Another was recorded as a 562-word
structural residual and matched on a compiler flag.

So read each closure adversarially and ask:

1. **Does it name a decision variable?** If it says "no source form reaches it"
   without naming one, it is not a closure. The closures that survive re-testing
   name one; the ones that fall do not.
2. **What did it vary, and therefore hold fixed?** Declaration order and
   definition position are different axes. A spelling lattice holds block
   membership constant by construction. A force sweep says nothing about
   statement placement.
3. **Was it written before the law that reaches it?** `tools/reopen_candidates.py`
   lists closures that predate a law touching their territory.

Read the in-source `/* PLATEAU-HANDOFF:<symbol>:start … */` block and
`docs/matching-triage-handoffs/<symbol>.md` for every target before touching it.
Re-running a lattice another lane already exhausted is this project's single
largest measured waste.

## The laws

Full statements and receipts are in `docs/compiler-laws/ido-5.3.md` in the
workbench at `~/Desktop/dev/n64-decomp-workbench`. Skim the index; do not read
it end to end. The ones that carry most of the weight:

- **L100** — a web's save is `totalsave/nocs`, and a symbol boundary moves both
  terms at zero width. Merging a per-arm declaration to function scope raises a
  save; naming what was one shared cfe temp splits it; a reference inside a
  loop weighs ×10. Ties keep the incumbent, scanning in ascending web number.
  One function went 21 → 5 on this where **each edit alone was a 31-word
  regression** — do not discard a direction because one half regresses.
- **L101** — a web whose span reaches a call result is not *offered* v0; it is
  absent from the `p1cost` list rather than forbidden in it. A declined force
  has three kinds: nothing in the record ⇒ split the web; a `forbidden` mask ⇒
  genuine interference no ratio or spelling reaches; already forbidden at
  decision time ⇒ the force never applied and **the object returns
  byte-identical, so the experiment proved nothing**. Check a force was
  *accepted* before reading an unchanged object.
- **L114, corrected 2026-09-11** — globalcolor's colour table reaches further
  down the caller-saved registers than this brief used to say, and **the
  boundary is per procedure**. One procedure decodes as c1 `v0` … c9 `t2`, c10
  `t3`, c11 `t4`, c12 `t5`, c13 unnamed, c14 `s0`, c15 `s1`, with two webs
  carrying `color=10 reg=t3` outright — so `t3`–`t6` are priced candidates a
  force can reach there, and only `t7`–`t9` and the float ring sit outside.
  **Decode the table from the `p1color`/`p1cost` records on your own procedure**
  rather than assuming a boundary. Splitting a naming residual by register bank
  is still the right first move; just do not treat a `t`-register row as
  unreachable until the records say so. Roughly a sixth of a naming residual is
  colour, and that figure is a **floor** — a force sweep never offers a colour
  another web has forbidden, so free the interferer before concluding a lever is
  out of reach.
- **L115** — a live range is formed per *symbol*, and interference is a
  block-set intersection. Reusing a local that is already live elsewhere imports
  its interference at zero width and no instruction; adding a fresh one does
  not. Which existing local you pick is the whole decision.
- **L112** — an unobservable array length is a free parameter the frame identity
  solves for: `frame = round8(fixed + block + temps)`, `block = base +
  element_size × count`. Put the target's frame in, read the count out. Check
  every dimension the ROM cannot see before concluding the frame has no cell to
  give.
- **L99** — stack homes descend from the frame top in declaration order, and
  only a memory-class local gets one. The offset is a linear readout, so sweep
  and solve rather than guess. Frame *size* is a count, not an order. An unused
  `s32` is eliminated before the frame is sized; an unused `f32` or pointer is
  not. `align8(4N)` hides a one-slot change.
- **L110** — uopt never merges an address constant across a basic-block
  boundary, and no read count produces a stack temporary.
- **L105** — uopt forwards a call's return register into every use in the call's
  own block; move the assignment to a later block and it reaches the
  callee-saved copy instead.
- **L113** — a loop whose index dies at strength reduction emits no preheader
  `move`. If the target's preheader has none and yours does, give the loop an
  index of its own.
- **L109** — a discarded-expression probe's weight is its loop depth, so a save
  ratio is settable from source at zero instruction cost. Narrow: uopt counts
  after copy propagation, a probe reading an already-read global is CSE'd away,
  and only OR-with-zero, AND-with-minus-one and XOR-with-zero reliably count.
  Verify a probe moves `totalsave` in the records before building on it.
- **L107** — a uopt region boundary blocks address reassociation through a
  pointer's own definition, costing 8 bytes of temp, payable by shrinking a
  local aggregate. **L97** — `if (1) { }` or `do { } while (0)` opens a region;
  a bare `{ }` does not. **L111** — as1 fills a delay slot when the block holds
  a third node. **L59** — as1's tie-break reads physical source line numbers,
  last key LIFO; folding two statements onto one line retires a tie while
  swapping them only moves it. **L103** — the float constant pool is keyed on
  the constant's *spelling*, not its value. **L44** — carrier identity is the
  lever, not its presence.

## Instruments

- `cc -S` — ugen's allocation before as1 schedules. Scratch cwd; `-o` is
  ignored and it writes `<base>.s` beside the input.
- `cc -Wa,-R` — as1's scheduler trace, byte-inert.
- `cc -g3` plus the `.mdebug` reader — exact frame home census.
**Scoring a forced object.** `score_symbol.py`, `align_symbol.py`, `frame_census.py` and `register_census.py` all recompile the TU with the *configured* command before measuring, which overwrites whatever object a `CDX_FORCE`d or instrumented run produced — so they report the **unforced** build. A lane read the same score for twelve different forces before noticing. They now warn when force or instrumented-compiler environment is set; score a forced object against its target directly instead.

- `uopt -Wo,-zdbug:2` — writes `./uoptlist` with the colouring decision records.
  **Never commit it.**
- The instrumented toolchain at `~/Desktop/dev/ido-instrumented` (`CDX_LOG`,
  `CDX_PROC`, `CDX_DETAIL_WEB`, `CDX_FORCE`). **Confirm its `.text` is
  byte-identical to the tree's object before trusting any reading.**
- Direct `cc` equals the asm-processor build for a candidate with no
  `GLOBAL_ASM` pragma, at roughly 130 candidates/sec, which makes a small
  lattice exhaustive rather than sampled.

## Traps that have each cost a lane real time

1. **Never read target words from splat's `.s` annotation** — that is the ROM's
   *linked* word. Assemble the listing and read the object.
2. **Check size delta before word count.** A stale object reads as a plausible
   coherent residual.
3. **A forced-colour receipt is numbered against its own compilation.**
   Re-derive any web number yourself and cite what you derived it from.
4. **`wb_compare.sh --summary-json` refuses whenever a linked
   `build/mickey.us.elf` exists** and blames your candidate. It is lying. Score
   before a full build.
5. **A lower positional count is not automatically closer once the schedule
   moves.** One candidate scored 37 against 39 while emitting five loads where
   the target emits three and filling both multiply-hazard `nop` slots the
   target leaves open. Count instructions too.
6. **The fast direct-compile loop needs the TU's own per-file flags.** Three TUs
   here carry overrides; a missing `-Wab,-r4300_mul` changes multiply expansion
   so the symbol scores as a *size* mismatch against a target it matches in
   shape. Check your harness's base score equals `score_symbol.py` once per TU.
7. **A harness that re-prints the source destroys physical-line levers.** Same
   check catches it.
8. **A carrier sweep ranked by score will offer a semantically wrong candidate
   that scores better.** One offered a loop's index carried by the x cursor
   inside that same loop, five words better than the correct answer; it would
   have compiled and verified against nothing. Require the candidate name to be
   absent from the region's text, then read the winner against the whole
   enclosing scope.
9. **A same-module call can still be a `SYMBOL` relocation record.** A function
   can score 0 masked words and still fail `verify` by exactly two, and
   `reloc_surface.py` says nothing. See `CLAUDE.md`.
10. **A per-file flag override is a falsifiable claim.** If the target holds an
    instruction the compiler cannot emit at that level — a branch-likely, or a
    rounding-mode float conversion — the override is refuted whatever it scored.
    `tools/check_isa_overrides.py` enforces this.
11. **Link failure on arrival:** run `gmake overlay-syms`, then build. If
    `git status` shows `overlay_undefined_syms.us.txt` turning a
    `<sym>_oNNReloc` back into a bare `<sym>`, that is lost-rename build drift —
    regenerate and rebuild, never commit that diff.
12. **Give yourself a private scratch subdirectory.** The session scratchpad is
    shared and two lanes have overwritten each other's files.

## Rules, non-negotiable

- NEVER pass `--no-verify`. NEVER lower a check threshold. NEVER edit
  `config/lane-reopen-authorizations.us.json` — repinning is a coordinator
  action; if a stale pin blocks you, say so and stop.
- Nothing ROM-derived is ever committed: no assembly, no instruction text, no
  hexdumps, no machine-word arrays, no base64 of ROM bytes. `uoptlist` and
  compiler scratch are never committed.
- No absolute workstation paths in tracked files.
- A handoff shard is a strict grammar: the metric header must be source, score,
  frame, relocations, first mismatch, then an optional summary, each on its own
  line, and `|` is forbidden anywhere in the block — write measurements as prose
  or indented lists, never a markdown table.
- Before EVERY commit: `gmake verify` (must print the expected SHA1),
  `gmake cleanroom`, `gmake check-docs`. **Read the exit status, not the last
  lines of output** — filtering a gate until it looks clean is how a red gate
  gets committed.
- On a resident match: remove that function's `#ifdef NON_MATCHING` guard,
  `gmake extract`, rebuild, `gmake verify`, `gmake scoreboard`, and commit that
  function alone with a short comment at the point of use saying which edits got
  it there. On an overlay match follow the promotion sequence in `CLAUDE.md`,
  then `check-overlay-syms` and `promotion-proof`.
- Do not refresh `docs/nm-ranking.md` unless `check-docs` fails without it; the
  coordinator regenerates it.

## When your lane is merged

The coordinator removes integrated lane worktrees with
`tools/reclaim_worktrees.py --apply`, which keeps every branch and only deletes
checkouts that are not running, have nothing uncommitted, and carry no commit
missing from the integration branch. Each lane's `build/` tree is hundreds of
megabytes and the fleet does not shrink on its own -- one sweep reclaimed 31 GB
across 106 worktrees.

So **commit everything you want kept.** A worktree that reports clean is a
worktree whose findings live only in your report, and a report is not the tree.

## Reporting

Pair every claim with its measurement. Give the aligner's four buckets before
and after anything you adopt, not just the positional count. State explicitly
whether `gmake verify` ran on each claimed match.

For anything you do not close, leave an updated handoff naming the decision
variable you reached, the record that blocks it, and the axes your lattice
actually covered. A precise negative is a real result — several have been worth
more than a match, because the next lane inherits whichever you wrote down.

Where a prior closure turns out to be wrong, say so with the measurement that
breaks it. That is the most valuable thing you can report.
