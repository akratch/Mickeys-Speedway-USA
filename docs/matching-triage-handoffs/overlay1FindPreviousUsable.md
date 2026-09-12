<!-- plateau-handoff:overlay1FindPreviousUsable:start -->
### `overlay1FindPreviousUsable` plateau handoff

- source: `src/overlays/o001/overlay_001.c`
- score: 12 differing words
- frame: frameless
- relocations: 4
- first mismatch: +0x4
- summary: structure-mismatch; lever none-known. Raw/masked 14/12, masked first +0x14; attempts 9-13 stalled. Next: fidelity-pinned CFE copy/coalescing trace.


#### c2-o001: the residual is one copy-propagation decision, and it drives all twelve words

Re-measured at 12 masked words, 14 raw, frameless, exact extent. The whole
residual is a single mechanism with three visible consequences, so it is one
question and not twelve.

The target keeps two copies of the entry count that the candidate propagates
away. It copies the count into the loop counter and into the wrap bound, tests
the counter copy rather than the loaded value, decrements the copy in place,
and recomputes the wrap bound freshly from the loaded value at the wrap site,
leaving the wrap copy dead. The candidate propagates the counter copy back to
the loaded value, which makes the counter's pre-decrement and the wrap bound
the same expression, so uopt commons them into one loop-invariant temp,
shares it between the guard and the wrap site, and eliminates the wrap copy
entirely. Three words go to the copies, one to the shared temp at the wrap
site, one to the guard test, one to the branch form, and the rest is the
register renaming that follows.

The branch form is downstream of the same fact and is not a separate lever.
The target's range guard is a plain branch whose delay slot takes the first
instruction of the fall-through -- one of the copies -- and it branches to the
shared failure block. With the copies propagated away, the candidate's
fall-through begins with a branch, nothing can fill the slot from below, and
ugen emits a branch-likely with the failure block's first load duplicated into
the delay slot. Restore the copies and the branch form follows.

Twenty-nine source forms were measured this lane and every one is byte-flat at
12 or worse. Do not repeat: the wrap bound spelled from the count variable, an
inlined `count - 1` bound, and a pre-subtracted bound variable (17 each); the
counter decrement written as `remaining = count - 1` (19) and as `-= 1`,
`--remaining` and a trailing form (flat); the guard written as `> 0`, as a
bare truth test, and with the constant on the left (flat); the two copy
assignments in both orders (flat); the wrap bound assigned after the guard,
inside the loop, and split across two statements (flat or 17); the wrap site
written as a compound add, which is correct in this ring but loses an
instruction (34); unsigned and explicitly cast variants of both locals (flat);
and the whole family of reads straight from the import in place of the local,
which does emit the copies but leaves the loaded value dead at the wrap site
and cascades the pointer colours instead (17 to 38 across six forms).

The loop shape is not the lever either. Every top-tested rewrite -- `while
(remaining--)`, `while (remaining-- != 0)`, and the `for` form, with the wrap
bound spelled three ways -- loses an instruction and scores 34, which confirms
the retained guard plus do-while as the target's own shape.

Next lever: whatever makes uopt decline to propagate a copy of a loaded value
into a guard test that immediately follows it. That is the same question
`overlay1ResolvePathPoint` asks at one word, which makes it worth answering
once rather than twice.


#### c2-o001 diagnostic: the inert-probe family is completely inert here, which classifies the residual

The same ADR 0017 inert-read family that moves `overlay1AdvanceGauge` 18 words
and `overlay1ConsumeNearbyPending` 7 was run against this function: 220
placements over eleven slots and 20 expressions, spanning every local, both
imports and the loop-invariant differences. Not one reaches below 12, and the
byte-flat ones are flat rather than merely equal-scoring.

That is a classification, not just another negative. The family is a
pool/ring population dial, so its total inertness here says the residual is
not an allocation-population fact at all. It is the copy-propagation decision
the section above names, and it will not yield to anything that only changes
how many webs compete. The same test run on `overlay1ResolvePathPoint`, whose
one word is the same class, is equally inert over 140 placements.

#### 2026-09-11, lane `f9-small`: copy placement before the range guard, and the top-tested family, are flat

Assigning `remaining = count` and `wrapCount = count` before `if (index <
count)` (both orders, wrap site from either variable, guard on any of the
three names) scores 25; after the guard as retained, 12. Top-tested
`while (remaining--)`, `while (remaining-- != 0)` and `for` all lose one
instruction (uopt shares `count - 1` between the guard decrement and the wrap
site) and score 34, so the guarded do/while is the target's shape. The
target's dead `move $a2,$v1` is the same "propagated but not deleted" copy
seen on `func_80020D8C` (post-decrement temp) and closed on `func_8003A754`
(a copy uopt's EQ_INEQ saw before propagation). Decision variable: which pass
propagates `remaining = count` into the guard test -- cfe within the block, or
uopt after DCE. A block boundary between the copy and its guard that emits
nothing is the lever to look for.


#### p8-o001: the region lever and the leaf position lever are both flat

This is a leaf, and its records confirm it: proc 12 emits p2 decisions only,
ten coloured in ascending web number taking the lowest free colour, exactly as
L141 says. So the ratio is inert here by construction and the lever is web
numbering, which is moved by position.

The previous record closed on "a block boundary between the copy and its guard
that emits nothing is the lever to look for". It is not. Nine L97 region forms
were measured and every one is byte-flat at 12: the two copies inside
`do { } while (0)` and inside `if (1) { }`, each copy separately wrapped, an
empty region placed between the copies and the guard in both spellings, two
empty regions, the guard itself opened into a region, and the copies swapped
inside a region.

Position is equally flat. All twelve declaration orders permitted by L143 --
six of the `s32` run by two of the pointer run -- plus six orders with the
pointer run first and six with `flags` first are byte-flat at 12.

A form the earlier records did not try is also eliminated: `if (remaining--)`
and `if (remaining-- != 0)`, which reproduce the target's exact guard shape of
`beqz` on the counter copy with the decrement in the delay slot, lose one
instruction (delta -4) and score 34, with or without the wrap bound spelled from
`count`, and with the copies in either order. So the target's `beqz a1` plus
`addiu a1,a1,-1` pair is *not* a post-decrement guard.

A twelve-cell lattice over where each of the two copies and the wrap bound reads
from -- the `count` local or the `gOverlay1EntryCount` import, crossed with three
wrap spellings -- confirms the retained form is the floor: 12 for the retained
row, 17 where the wrap reads `count`, 27 where `remaining` reads the import and
the wrap reads `wrapCount`, and 38 for every row where the counter copy reads
the import, half of them a word short.

The naming half of the residual is now measured rather than described. The
candidate runs exactly **two** ring positions ahead of the target -- its first
scratch temp is `t1` where the target's is `t0`, and it draws one extra for the
commoned `count - 1`. One of the two is the shared temp the structural residual
creates, so it is downstream of the copy question and not a separate lever; the
other is a phase offset present before any of it.

#### 2026-09-12, lane `p12-tight`: the CSE half of the residual is REACHED, and the blocker is now one instruction

Baseline reproduces: 160 bytes, 12 relocation-masked words (14 raw), size delta
0, frameless, first mismatch +0x4. Aligner: 28 byte-exact, 6 register naming, 0
immediate only, 6 really different. `register_census` reads one coherent
mapping over one window at 100 per cent, three source registers, no cycle. No
edit adopted.

**Read from the object rather than described.** The two sides differ in exactly
one region, the preamble between the range guard and the loop. The target loads
the count, tests the index against it, and then in the guard's delay slot and
the instruction after it makes TWO copies of the count; it tests the first copy
against zero, decrements that copy in place in the next delay slot, and at the
wrap site recomputes the bound from the loaded value. The candidate tests the
loaded value directly, computes the bound once into a scratch temporary in the
guard's delay slot, copies that into the counter, and reuses the same
temporary at the wrap site. Because its fall-through begins with a branch the
candidate also needs a branch-likely where the target uses a plain branch. All
six structural words and the three naming ones are that one region.

**L109's successor is L151, and it works here.** The commoning that the earlier
records name -- the counter's pre-decrement and the wrap bound become one
loop-invariant temporary -- is broken by giving the two literal ones DIFFERENT
TYPES. Writing the wrap bound with an `unsigned` one while the pre-loop
decrement keeps a plain one (or the reverse) stops the common subexpression
entirely: the wrap site then recomputes the bound from the loaded value at the
target's own offset, and the structural bucket falls from 6 to 3. That is the
first time any pass has produced the target's wrap site.

**What it costs, exactly.** Every form that breaks the commoning loses ONE
instruction, delta -4, and scores 34. The missing instruction is the SECOND
count copy -- the wrap-count copy the target emits and never reads. With the
commoning broken, the candidate folds the counter copy and its decrement into a
single instruction off the loaded value, so the fall-through has nothing to put
in the guard's delay slot and the copy pair never materialises.

So the reopen condition is now one instruction rather than an open question:
**a source form in which the counter's copy of the count is materialised BEFORE
it is decremented, and the wrap-count copy is emitted although it is dead**,
while the two literal ones stay differently typed so the wrap site recomputes.
The target's own object shows both copies surviving copy propagation, so this
is a real compiler state, not an impossibility.

**Negatives added this pass, all built rather than sampled.**

- 57 cells over three pre-loop decrement spellings, six wrap-bound spellings
  (plain, unsigned, long, complement-of-negative-zero, cast-through-unsigned,
  read from the count local) and three loop-tail test spellings. Only the type
  split moves anything, and it moves it to -4.
- 32 cells over the source of every count read: the guard, the counter copy,
  the wrap copy and the wrap bound each taken from the loaded local or from the
  import directly. The incumbent is the unique floor at 12; the next best is 17.
- 144 cells over four copy-chain shapes (counter from count, counter from count
  with the wrap copied from the counter, wrap from count with the counter
  copied from the wrap, both from count) crossed with three guard subjects,
  three decrement spellings and four bound spellings. Floor 12; a copy chain is
  inert, so uopt propagates through two levels as readily as one.
- 40 cells over the post-decrement guard family -- guard written as a bare
  post-decrement, as a post-decrement compared with zero, and with the pre-loop
  decrement removed -- crossed with five bound spellings and two loop-tail
  spellings. EVERY one of them loses the same single instruction. That is worth
  stating plainly: the target's guard shape and the target's wrap site are each
  individually reachable, and each one alone costs the same word.
- 96 cells over the declared types of the count, counter and wrap locals
  (signed and unsigned, 32-bit and 16-bit) crossed with three bound spellings.
  The 32-bit signed and unsigned rows are byte-identical, so a declared type is
  not a web identity here; only a LITERAL's type is.
<!-- plateau-handoff:overlay1FindPreviousUsable:end -->
