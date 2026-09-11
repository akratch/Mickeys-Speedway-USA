<!-- plateau-handoff:func_overlay_052_F000063C_189ACAC:start -->
### `func_overlay_052_F000063C_189ACAC` plateau handoff

- source: `src/overlays/o052/overlay52TailB.c`
- score: 1337 differing words
- frame: 0x118
- relocations: 283
- first mismatch: +0x130
- summary: Residual partitioned: 379 displaced, 899 aligned-differing, 60 unplaced. 818 of the 899 are register naming; one global bijection closes 309. Next: the single web-order slip behind the frame-home and temp-ring offsets.

The September 8 decompiler-assisted packet retains attempt 39 as ordinary,
guarded C. This is a compiled structural plateau, not an object or ROM match.
No symbol-table, atlas, scoreboard, or matching-credit change is claimed.
The source's GLOBAL_ASM fallback remains canonical.

**Owned extent and measured result**

- Identity: overlay 52, text offsets 0x63C through 0x2098, ROM offsets
  0x189ACAC through 0x189C708, exclusive ends. The owned executable extent
  is 6748 bytes, or 1687 words; none of the deficit is padding.
- Re-derived configured baseline: 6272 bytes, 1568 words, 476 bytes short;
  1672 raw and relocation-masked positional differing words, first +0x30,
  frame 0x118, and 208 candidate relocation records against 314 runtime
  target records. The inherited 314 figure described the target surface.
- Retained result: 6736 bytes, 1684 words, 12 bytes short. It recovers
  116 words, or 464 bytes, of the baseline size deficit. The diagnostic
  comparison has 1597 raw and 1595 relocation-masked differing words,
  first +0x130. Frame 0x118 agrees; 310 candidate relocation records all
  resolve, but only 27 sites agree in offset, type, and identity with the
  314-record runtime target. Relocation exactness fails.
- All 64 direct calls, covering 31 callees, agree in identity and order.
  This is stronger than a call multiset, and weaker than matching the call
  offsets or the linked executable bytes.
- The old scalar minimum is preserved at attempt 5: 1674 words, 1451
  positional differing words, frame 0x120. It has duplicate time divisions
  whose extra words cancel missing recurrence words. Attempt 40 improves
  the retained candidate's positional count by one word but substitutes
  duplicated stores for the target addition edges. Neither displaces the
  better-understood operation graph of attempt 39.

**Recovered evidence and source**

The raw extracted assembly collapses external calls and section-zero globals.
Decoding Mickey's runtime relocation tables recovers their real overlay,
section, and symbol identities before drafting C. The stock workbench target
contains only 162 relocation records, so it cannot prove this function's full
runtime relocation surface. A section-qualified assembly adapter was checked
against the owned ROM slice: 1687 words with zero differences and all 314
relocation tuples exact. That is an importer self-check, not a C match.

The full generated context did not parse in m2c because of unrelated header
types. A narrow context using this TU's authenticated fields and call names
produced a typed draft. Raw drafts, resolved assembly, and all comparison
artifacts remain ignored. No draft or instruction listing is tracked.
The actual asm-processor output was preprocessed with the configured compiler
and accepted by the context comparator when compared with itself. This audit
was completed after the initial reconstruction, not before the baseline;
it does not retroactively establish the required preparation order.

Recovered source includes the 16-byte texture entry layout, the racer's
accessed widths and field offsets, the menu object's float fields, and the
real descriptor/time-split interfaces. The template extent includes its
ninth, terminating entry. The remaining helper return and argument widths
were checked against Mickey callee evidence. Array indexing restores shared
time quotient/remainder computation and the target texture-copy loops.
Icon coordinates are sampled before the three count draws. The clock-hand
calculation now explicitly performs its 32-bit wrap before signed division.

The opening recurrence now advances the remainder and groups of four with
an explicit exit. A counted spelling of the already expanded draft had been
unrolled a second time; the explicit exit avoids that expansion. Its opening
call interval grows from 76 to 106 words against the target's 108, recovering
30 words in the intended region. Each positive update count still performs
exactly that many f32 subtraction/multiplication/addition steps and global
stores; nonpositive counts perform none. There is no extra final floating
operation, invented guard, or padding. This reconstruction does not prove
that the original source manually grouped the updates.

**Remaining deficit and stopping evidence**

Call-delimited extent differences are now confined to the opening recurrence
interval (two words short), item replacement (one short), item restoration
(two short), the main icon draw (one extra), and the final count draw
(one extra). Equal extent elsewhere is not byte identity.

The recurrence still has two ordinary backedges where the target uses likely
backedges with duplicated addition edges. Its FP carrier graph differs.
The initial paired halfword stores also form separate high-address loads,
where the target shares them. Other relocation-count differences concern
address formation for fields of the same authenticated entry arrays; there
are no unresolved callee or global identities. Remaining icon and item
intervals include different address/value lifetimes and spill placement.
No wrong arithmetic literal was found in the workbench constant bucket;
its aligned constant findings were stack-displacement differences.

Attempt 39 was the last accepted structural gain. After the single-word
positional alternative at attempt 40, five consecutive refinements left the
retained residual and the remaining causal explanation unchanged:

- 41: separate old-value and delta locals add copies, six words, and a
  larger frame; additional relocation sites are not the target sites.
- 42: one accumulator for delta and result retains the wrong latch and
  does not improve the residual.
- 43: a forward completion exit changes scheduling but leaves the same
  opening extent and relocation deficit.
- 44: a typed float cursor removes target loads/stores and grows the frame.
  Its lower positional count is a regression in the structural comparison.
- 45: counter initialization on the nonpositive path is byte-flat.

These refinements recover no new call/global identity and do not resolve a
new mismatch mechanism. Stop on that stall, not on the total attempt count.
Earlier named-step, simple loop-condition, countdown, and pointer-hoist forms
are preserved as exhausted inputs. The remaining recurrence issue is not a
missing update operation. Do not restart those spellings without changed
evidence. No flag lattice, permuter, instrumented compiler, invented volatile
access, global regrouping, or post-compile instruction editing was used.

The concrete next lever is a faithful comparison of the producing compiler
phases for the retained recurrence and a Mickey-proved sibling with the same
branch-latch shape, plus an address-context check for the paired fields.
First establish whether source def/use structure or compiler context owns
those decisions. This requires a separately authorized mechanism; it is not
permission to change shared flags, symbols, alignment, or data placement.
The workbench verdict remains structure-mismatch, routed through
structure-buckets: context parity and the spill-slot census, after the
completed constant and call/global identity audits.

**Artifacts, commits, and validation**

The packet base is cbd755d9b2e828b5f4c05a4efe81d70353d205cb.
Progress commits are 0f5059c5a702933b4a3c3faf6fdc1fdfa8b61d17,
c45403aa, and 0f914be9; the commit containing this handoff banks the grouped
recurrence and final interface corrections. Only the owned source and this
shard change in that final source commit.

Ignored evidence is under build/whale4/: baseline source/object and reports,
attempt snapshots through a045, the retained a039 source/object/diagnostic
ELF, per-attempt rationale and measurements, runtime identities, the target
adapter self-check, compiler-input self-comparison, and recurrence review.
The standalone link is a stored-addend diagnostic, not a synthetic-address
promotion or a canonical linked-ROM proof. Every meaningful alternative is
preserved, including the scalar minima.

Commands used include generate_ctx.sh, mips_to_c.sh with the typed local
context, tools/wb_compare.sh with the configured full-TU candidate,
decomp-workbench diagnose/compare/guide structure-buckets,
tools/candidate_context.py, and the ignored measure.py/call_buckets.py
adapters. tools/finalize_plateau.py generated the guarded plateau metadata
and this shard's metric header; the richer evidence is recorded here.

Source gates and all 243 NON_MATCHING TU builds passed at each banked
checkpoint. gmake verify repeatedly fails in unchanged overlay94UpdateController
at the assigned base: undefined func_800254FC, func_8002565C, func_8002A878,
and func_8002A910, plus resident R_MIPS_26 overflows for func_8005ABA8,
func_8005AF14, func_80019AB8, and func_8002B040. There is no canonical linked
ELF available for an owned-range extraction. This remains outside this
lane's ownership. Closing gate receipts are retained alongside the packet;
no full-ROM success or newly matched bytes are claimed.


**Residual partition (2026-09-10 measurement pass)**

The retained candidate was re-measured against the extracted target under the
configured translation-unit flags. All 1687 words partition exactly, and the
partition contradicts the assumption that a function this far out is
structurally wrong:

- 349 words byte-identical in place.
- 379 words byte-identical to their aligned partner but in the wrong position.
- 699 aligned pairs differing in register fields only.
- 119 aligned pairs differing in register fields and immediate.
- 81 aligned pairs differing in immediate only.
- 60 target words with no aligned partner.

The first bucket plus the other five is 1687; the last five is the 1338 that
differ positionally. Alignment here means the opcode-and-shape correspondence,
computed by matching the two instruction streams on opcode, function code,
shift amount and format field with every register, immediate and branch
displacement removed.

*The operation mix is already right.* Counting every instruction class across
both sides, only seven classes disagree at all and the total instruction-count
disagreement is twelve out of 1687. High-half materialisations, calls,
comparisons, stores and floating multiplies agree exactly in count. This is not
a different program; it is the same program with different register names and a
small number of placement defects.

*All displacement comes from three call intervals out of sixty-four.* Splitting
both sides at their direct calls, sixty-one of the sixty-four intervals have
identical word counts. Interval 8, the opening float recurrence, is two words
short; intervals 36 and 39 are one word long each; the three cancel, which is
why the extent is exact. The two words in interval 8 are two floating additions
that accompany the target's use of the likely branch form on both recurrence
back edges, where the candidate uses the ordinary form and fills the delay from
the instruction before the branch. This is an assembler scheduling difference,
not a missing operation: both loop bodies hold the same six operations in the
same cyclic order and differ only in which one is rotated into the delay slot.
The extra word in interval 39 is a second reload of a spilled local that the
target reloads once, because the target's reload lands in a callee-saved
register that survives the intervening calls and the candidate's lands in a
caller-saved one. Repairing those three intervals is worth about 390 words:
under a piecewise constant shift the count drops from 1338 to 988, and under
free alignment to 948.

*The rest is register naming, and part of it is a uniform ring rotation.* Under
the opcode-and-shape alignment, 818 of the 899 differing pairs disagree in a
register field. Rotating the candidate's temporary ring by a fixed number of
steps recovers a large share of them. A three-segment model, no rotation to
about +0x540, one step from there to about +0x9C0, no rotation to about +0xF60,
two steps from there to the end, raises the identical count from 728 to 924.
The second segment alone is 91 words and the fourth is 68. Fitting one
arbitrary register substitution for the whole function instead raises it to
1037, and allowing the substitution to change every 128 words raises it to
1316, which bounds how much of the naming difference is relabelling rather than
different code.

The same slip shows in the frame. The frame size agrees and the number of homes
agrees, but the candidate's home block sits one four-byte cell below the
target's over a long span while one later home sits one cell above, so 54 of
the 67 disagreeing stack displacements are exactly four bytes. Home order is
web-creation order, so this is one temporary created one position out of turn,
and it is very likely the same decision as the one-step ring rotation.

*Relocation representation is not a factor.* The project ranking already
reports 1338 raw against 1337 relocation-masked. Resolving every overlay-local
high and low half on both sides to its numeric address independently changes
the aligned identical count from 728 to 735. Mickey's overlay text stores
addends and the extraction re-symbolises them, so the extracted target and the
unlinked candidate object are directly comparable. Twenty-four candidate sites
do carry a nonzero stored addend where the target stores the same address as a
symbol reference with none; ten of those are byte differences the source
spelling can remove by naming the object at that address instead of an offset
from an earlier one.

Falsified during this pass, each by a paired build under the configured flags:
equality-comparison operand order, canonicalised to byte-identical output;
commutative operand order on the recurrence multiply, byte-identical; an
explicit non-compound assignment for the accumulator, byte-identical; giving
the remainder or the counter its own local, which changes the frame and worsens
the residual, so the current local census is the right one; and the top-tested
counted loop forms, which unroll and add twenty-eight words.

One diagnostic result is worth recording even though its spelling is not
adoptable. Replacing both recurrence exit tests with an exclusive-or against
zero leaves every instruction in the recurrence unchanged and still moves 764
words of the candidate downstream of +0x300, dropping the positional count from
1338 to 1220 and removing the two-step ring rotation in the tail. That proves
the tail's rotation is set by a web count upstream rather than by anything in
the tail, and that one more web consumed in the recurrence is enough to correct
it. It is not a proposed source form.

The next lever is therefore the web-order slip, approached from the frame-home
census rather than from the recurrence's operation graph, which the earlier
packet exhausted. `gmake verify` passes in this worktree at this commit, so the
link failure recorded in the previous section no longer reproduces.


#### Phase census, the two-region partition, and the allocator's real share (2026-09-11)

**Census first: this translation unit compiles to a single uopt procedure, and
its 237 globalcolor decisions are all phase one -- 228 integer, 9 floating,
zero phase two.** 104 are coloured and 133 split. So the phase-two
web-numbering axis is not live here and no amount of moving a defining
statement is a colour lever for this function. The rule is general: over the
61 procedures of this unit, `overlay_001_tail.c` and `overlay_008.c`, every
procedure that issues a call emitted phase-one records only and every leaf
emitted phase-two records only, 59 classified with no counterexample
(`docs/ido-learnings.md`).

Aligned against the target on a register-erased shape
(`tools/align_symbol.py`): 1687 words against 1687, positional 1337,
displacement tax 306, and the residual splits 672 byte-exact / 739
register-naming / 292 really-different. The extent agrees but not locally --
16 candidate words in 14 spans and 16 target words in 14 spans, which is why
the extent cancels.

**The residual is two independent problems with different owners, and the
earlier packet's segment model is one of them.**

*Region 8, the opening float recurrence, +0x130 to +0x2E0.* 113 words, 19
byte-exact, 54 naming, 40 really-different. Every one of the 54 naming rows
is a floating-register difference; there is essentially no integer naming
before +0x400 (nine field substitutions in the whole prefix). This region is
the only place in the function where float allocation is in question at all --
the aligner finds float differences in just 4 of the 65 call-delimited
regions.

*The integer body from +0x400 on.* 622 of the 739 naming rows name a register
`globalcolor` never assigns in this procedure. The trace is explicit about
which registers it does assign: `v0`, `v1`, `a0`-`a3`, `t0`, `t1`, `t2` and
`s0`-`s8`, 104 colourings in all of which four are floating, and **never
`t3` through `t9`**. Those seven
are ugen expression temporaries, and the residual is a piecewise-constant
permutation of them:

  - +0x0 to +0x400: nine integer field substitutions in the whole prefix;
    clean.
  - +0x400 to +0x800: the seven-cycle `t3`->`t4`->`t5`->`t6`->`t7`->`t8`->
    `t9`->`t3`, a one-step rotation, carrying 93 of the window's 107
    substitutions across 78 naming rows.
  - +0x800 to +0xC00: a transition; both permutations are present and neither
    dominates.
  - +0x1000 to the end: a five-cycle `t3`->`t5`->`t7`->`t9`->`t6`->`t3` with
    `t4` and `t8` transposed, carrying 497 of the window's substitutions
    across 376 naming rows.

  Over the whole function that second permutation alone accounts for 586 field
  substitutions, which is why an arbitrary single relabelling recovered so much
  of the residual for the earlier packet and a rotation of a single ring did
  not: it is a five-cycle and a transposition, not a rotation.

Only 48 of the 739 naming rows are purely among the registers globalcolor
assigned, and 19 of the 69 rows with a float difference have both sides inside
the class-2 pool (c24=`f0`, c25=`f2`, c26=`f12`, c27=`f14`, c28=`f16`,
c29=`f18`; decode in `docs/ido-learnings.md`). So only about 67 of the 739
naming rows are a colour globalcolor actually picked -- though that is a
statement about what *kind* of decision each row is, not a bound on what the
allocator can reach, because the ring's phase is itself downstream of how many
pool colours are consumed (the force ceiling below closes 156). The earlier
packet's ring-rotation reading is confirmed and its owner is now named:
it is ugen's temp ring, not a colour, and the ring's phase is set by how many
temps are consumed upstream -- which is why that packet's exclusive-or
diagnostic in the recurrence moved 764 words in the tail.

**Where the naming actually is.** Four consecutive call intervals hold more
than half of it: region 40 at +0xE9C (162 words, 47 exact, 101 naming), region
41 at +0x1120 (143 words, 45 exact, 98 naming, and *zero* really-different),
region 42 at +0x135C (200 words, 61 exact, 116 naming) and region 43 at
+0x166C (112 words, 39 exact, 68 naming). 617 words, 383 naming rows, 42
really-different. Region 41 is the cleanest target in the function: 98 words
wrong, all of them a register name, nothing structural at all.

**The really-different bucket is mostly not different code.** 71 of the 292
rows are the same instruction at a different `sp` displacement, 49 at a
different non-`sp` displacement, 57 the same mnemonic with another immediate,
83 a genuinely different opcode, and 32 present on one side alone.

**The frame is one home swapped in creation order.** Both frames are 280
bytes. The candidate's home block at +0x5C through +0x70 should be +0x60
through +0x74 and its home at +0x9C should be at +0x98: the shipped frame
leaves +0x5C empty and uses +0x74, the candidate uses +0x5C and leaves +0x74
empty, and the two swap back at the later home. 36 of the 71 `sp` rows are
exactly +4 as a consequence. That is the earlier packet's "one temporary
created one position out of turn", now read off the home set directly rather
than inferred from a displacement histogram.

**How much of this is the allocator's to give.** Greedy force ceiling,
sweeping all 213 webs against every colour and the split path each round and
keeping the best (4,764 compiles per round):

  - baseline 672 byte-exact / 739 naming / 292 different, positional 1337;
  - `p1:w877=s` -> 806 / 605 / 292, positional 1210. This is a single force
    worth 134 byte-exact words. Web 877 is one of eight webs uopt derives from
    its symbol 102, a local with 54 references; its first decision already
    splits, and it is the *second*, `save=30.0`/`nocs=1` piece that the
    candidate colours and the target does not;
  - `+p1:w882=c7` -> 821 / 590 / 292, positional 1195;
  - `+p1:w462=c6` -> 828 / 583 / 292, positional 1189.

Three rounds recover 156 byte-exact words, 15% of the 1031 wrong ones, and the
increments are 134, 15 and 7, so the series has effectively converged. The
same sweep on the other two whales converged at 16% and 17%, so a sixth is the
number to budget. **The really-different bucket does not move by a single word
under any force**, and the integer ring permutation survives all three rounds:
what the forces buy is bought inside the naming bucket, and not enough of it to
change the verdict.

**Recommended order for the next attempt.** Region 8 first -- it is upstream of
the ring phase, it is the only float region, and the earlier packet already
showed that consuming one more web there corrects the tail rotation. Then the
one swapped home. Region 41 is the cheapest confirmation that the ring moved,
because it has no structural content to confound the reading. Do not open a
declaration-position or statement-position lattice: the census says that axis
is dead for this function.

Nothing above is a source change, a match claim or a credit claim. The
`GLOBAL_ASM` fallback remains canonical and `gmake verify` passes at this
commit.
<!-- plateau-handoff:func_overlay_052_F000063C_189ACAC:end -->
