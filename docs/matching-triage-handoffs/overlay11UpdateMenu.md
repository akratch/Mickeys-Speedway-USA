<!-- plateau-handoff:overlay11UpdateMenu:start -->
### `overlay11UpdateMenu` plateau handoff

- source: `src/overlays/o011/overlay11UpdateMenu.c`
- score: 299/301 words
- frame: 0x48
- relocations: 102
- first mismatch: +0x138
- summary: Reopened and re-diagnosed: the barrier is as1 memory disambiguation, not a debug line entry, and ugen emits it from ordinary C. The indexed spelling produces the target's spill order exactly; the live blocker is its two-cell frame cost.

Revalidated 2026-09-08 on assignment base
`419037148f512f2d38042f3a17ad4e2589db0768`. The retained C remains
`NON_MATCHING`, with the original assembly fallback.

- Ownership: Overlay 11 text `+0x1398..+0x184C`, ROM
  `0x1869BE0..0x186A094`; 1,204 executable bytes / 301 words, no owned padding.
  The candidate text section has 12 additional bytes of compiler alignment;
  they are outside this function and earn no credit.
- Configured baseline and metadata reproof: 285/301 raw positional words
  (16 differ, first `+0x14`), 299/301 relocation-masked positional words
  (2 differ, at `+0x138` and `+0x140`), zero size delta, frame `0x48` on both
  sides. The two differences exchange the index and handle spill stores
  across the same call. All register assignment lanes agree.
- Fresh function preflight authenticates 102/102 relocation offsets, types,
  identities, and addends against the overlay runtime table, with no
  unresolved identities. There is one local inbound at module `+0x930` and
  no exports. Synthetic names in the extracted target object are not the
  runtime relocation authority.
- Workbench comparison verdict: `allocation-mismatch`, routed guide
  `pool-position`. Its aligned diagnosis instead reports mixed constants and
  scheduling, `playbook=constant-audit`, and `lever: none-known`. Those early
  constant/identity warnings come from the extracted object's incomplete
  relocation representation: the runtime preflight resolves the full surface.
  The remaining two non-relocation words are the already-recorded spill pair;
  the generic pool-position route supplies no evidence of a new allocation
  defect. No constants, flags, identities, or allocation inputs changed.
- Similarity oracle: the best hit is `joys_handle_retrace` (Jaccard 0.0663);
  `menu_options_loop` scores 0.0603. These are weak structural hints, not
  donors. The local matched two-option and five-option menu siblings use the
  already-investigated pointer/index loop and supply no new spill dependency.

Stopping evidence: **zero new source attempts**. The last recorded strict
source gain was the natural declaration/lifetime repair in the earlier
identity pass. The latest source/handoff commit,
`c7ac01796dbcd314c79f2bf7a30c0e00e5d261c1`, explicitly records ten later
non-improving declaration-order and physical-line forms, plus a byte-inert as1
trace: both stores have the same call line, dependency, and zero aftercycles,
and the handle store ranks first. The retained notes also close the prior
flag lattice, explicit-handle form, loop/scope forms, and bounded batch.
The current baseline reproduces that exhausted state. ADR 0018's early
exhaustion clause applies; this run did not spend three attempts repeating
mechanisms whose inputs have not changed. Baselines, metadata migration, and
reproofs are not source attempts.

Next concrete lever: obtain source-authentic evidence for a changed
**pre-call spill dependency/emission order**, then pin a new reopen packet.
A paired ugen emit-provenance / as1 selection receipt should identify the
first differing spill-production decision while preserving the authenticated
callee ABI, stack homes, loop behavior, and relocation surface. Another
source-line, declaration-order, loop-form, or flag sweep without changed
inputs does not meet that bar. No new such source spelling is established here.

The structured source marker was moved from its legacy inline position to EOF
for `tools/finalize_plateau.py`. Recompilation proves the entire candidate
text section and relocation section unchanged from the archived baseline.
The C body and its fallback are retained; there is no new exact C credit.

Evidence is retained under ignored `build/wb/o11-menu/` (baseline C/object,
comparison and preflight reports, raw and masked first-mismatch receipt) and
`build/wb/overlay11UpdateMenu.*` (current comparison, diagnosis, and preflight).
Commands run: `tools/wb_compare.sh --summary-json overlay11UpdateMenu`;
`decomp-workbench diagnose` on the target and configured full-TU candidate;
`decomp-workbench guide allocation-mismatch` and `guide constant-audit`;
`tools/function_preflight.py overlay11UpdateMenu --json`;
`tools/skeleton_scan.py similar --target 11:+0x1398 --top 5 --json`;
`tools/finalize_plateau.py` with the measured fields above. Validation receipts
are retained with the ignored evidence.

Validation: `gmake verify`, `gmake cleanroom`, and `gmake check-docs` pass;
the structured handoff audit reports no issues. The linked ELF's owned
1,204-byte range was extracted and byte-compared with those same baserom
offsets: exact **assembly fallback only**. The rebuilt US ROM SHA1 is
`507341c0a40ca3e9a7cee969b396ee53facfb548`. No matching status, symbol-table
line, atlas ownership, or scoreboard credit changed. This handoff and the
source metadata migration are the only tracked changes in the lane commit.

#### 2026-09-09 lane `lm-bigsingles`: the residual is fully explained and the
#### remaining word pair is not source-reachable

The 299/301 baseline reproduces exactly on this lane's assignment base. Twenty
further source forms (loop shape, pointer versus index spelling, ternary
inlining, increment order, declaration order and physical-line grouping,
use-site rewrites of the compared value, an added dead colour, a hoisted
dereference, a comma-joined statement, split call arguments) all held at 2.
Two forms regressed and are recorded as eliminated: a post-incremented
dereference in the call argument, and bounding the loop on the pointer instead
of the counter.

The mechanism is now known end to end, and it is not a colouring fact.

1. `ugen` already emits the pair in the target's order. Confirmed by reading
   the phase output directly rather than inferring it.
2. `as1` swaps them. Feeding `as1` the same listing with only that pair
   exchanged produces the owned 1,204 bytes with **zero** non-relocation word
   differences -- every remaining raw difference is an unresolved relocation
   field. That is a positive proof that the retained C is otherwise exact and
   that this pair is the entire residual.
3. The swap is triggered by the may-alias load that supplies the call's first
   argument sitting in front of the pair in the same line region. Substituting
   a load with a provably distinct stack displacement stops it; register
   identities and stack displacements were both swept and neither matters.
4. `ugen` orders caller-save spills strictly by ascending register number --
   73 of 73 comparable sites in the tree, including 38 where register order and
   home-offset order disagree. The order `as1` needs here is descending, so no
   C spelling can supply it.
5. The one input shape that survives is a scheduling barrier between the two
   stores, and only a `.loc` naming a greater line acts as one. `ugen` emits at
   most one `.loc` per statement and both spills belong to the call statement,
   so that barrier is unreachable from C. Line merging does not help: `ugen`
   still emits a same-line `.loc`, which is not a barrier.

Do not spend further attempts on declaration order, statement order, loop form,
line grouping, use-site rewrites or the flag lattice; each of those was
re-measured here and each is now excluded by mechanism rather than by search.
A reopen needs a *new input*: a compile mode in which the assembler's memory
disambiguation or its debug context differs (both were shown to flip the
decision), or evidence that the argument load in the target is not a may-alias
reference to that pointer. The general laws are recorded in
`docs/ido-learnings.md` under "Assembler scheduling and phase replay".

#### 2026-09-10 lane `c2-reopen`: the closure's mechanism is falsified; two more
#### barriers exist and one of them is emitted by `ugen` from ordinary C

The 299/301 baseline reproduces on this lane (raw 16, relocation-masked 2, zero
size delta, frame 0x48, first masked mismatch +0x138). The listing replay is
byte-exact here too: `cc -S`, then `as0` + `as1` under the compiler-path flags
without `-pic0`, reproduces the configured object with the same 16/2 score, so
every result below is measured against the same comparator the ranking uses.

**What the prior closure got right.** `ugen` does emit the pair in the target's
order, `as1` does invert it, and the inversion is the entire residual.

**What it got wrong.** Its stopping claim was that the only input shape that
stops the inversion is a debug line entry between the two stores, which `ugen`
cannot emit inside one statement's spill group, and that therefore no C form
can reach it. Three separate byte-inert perturbations of the phase input take
this function to **zero** relocation-masked differences, not one:

1. a `.noalias` fact naming the argument register against `$sp`, placed
   anywhere from the loop preheader through the point between the two stores;
2. a `.loc` naming a greater line between the two stores (the recorded one);
3. `.set volatile` bracketing the two stores.

Naming any other register in the `.noalias` is inert; so is placing it after
both stores, and so is opening it before the load but closing it with `.alias`
before the second store -- the fact has to still hold *at* the second store.
`.set volatile` around the **first** store alone is enough; around the second
alone is not. `.livereg` is inert here in every form tried: moved ahead of the
pair, deleted outright, and with two different masks. That locates the decision
precisely: `as1` is choosing which spill store to sink into the call's delay
slot, and it decides by whether it can prove the argument load's base register
disjoint from the stack. It is not an emission-index, source-line or liveness
decision.

**`ugen` does emit that fact from ordinary C, but not for a walking pointer.**
It emits `.noalias <reg>,$sp` for a reference to a *named* static object --
including for `uopt`'s own induction pointer over a named array, where it is
re-asserted every iteration and closed with `.alias` after the loop, so it
survives the pointer's own spill and reload -- and also wherever a static
address is materialised and dereferenced adjacently, which is why this
function's own volatile input pointer gets one. It never emits it for a
user-declared pointer that the source assigns from an array name and then
increments. Confirmed on a six-point standalone probe: the array-name and
`p[i - 1]` spellings produce the fact, `*p`, `p[0]`, a pointer-bound `for`, and
a `const`-qualified pointer do not.

**The array-index spelling reaches the fact and is nevertheless excluded.** It
does reproduce the target's spill order and argument-load form exactly. Its
cost is compiler temps, and the `cc -g3` `.mdebug` local table measures the
frame directly: 24 bytes of outgoing argument area plus the return-address save
is 28, this function's eleven declared locals occupy 44 more, and 28 + 44 is
exactly the target's 0x48 -- so **the target's frame has zero temp cells, and
the candidate's declared block already matches the target's homes name for
name.** Every indexed spelling adds temps instead: 12 bytes with the pointer
declaration dropped (frame 0x50) and 16 bytes with it kept (frame 0x58),
whether the index is written on the array name, on the pointer, as pointer
arithmetic, with the pointer left walking or removed. Dropping an unrelated
declaration does not pay for them either -- the pointer form gives up exactly
eight bytes for that same removal, so the measurement is not a rounding
artefact.

So the open question is not "which spelling" but **a zero-temp producer of the
disambiguation fact for a walking pointer**, and the candidate's declared-local
census is already proof that the rest of the frame is right.

Newly eliminated here, all flat at two words with frame 0x48: a `volatile`
pointee on the walking pointer, which does emit `.set volatile` around the
argument load and is byte-inert -- so the volatile barrier does not reach this
decision even though bracketing the *stores* does; the same through a cast at
the use site; an initialiser-in-declaration form; `register`; `handle[0]` for
the dereference; a hoisted dereference into its own statement, which puts a
`.loc` in front of the pair rather than between it; a `for` loop; a top-tested
`while`; and swapping the two preheader assignments, which regresses to four.
Bounding the loop on the pointer instead of the counter regresses to 215.

Do not re-run declaration order, statement order, loop form, line grouping,
the flag lattice, or any array-index spelling. The general laws are recorded in
`docs/ido-learnings.md` under "Assembler scheduling and phase replay".

<!-- plateau-handoff:overlay11UpdateMenu:end -->
