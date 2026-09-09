<!-- plateau-handoff:overlay59Advance:start -->
### `overlay59Advance` plateau handoff

- source: `src/overlays/o059/overlay59Advance.c`
- score: 243/262 words
- frame: 0x58
- relocations: 15
- first mismatch: +0x4
- summary: One proved ugen ordering constraint: the 1.0f invariant must be emitted before the factor load. Phase replay reaches 0 on that order; save-block axis falsified.

#### 2026-09-09 lane `lm-bigsingles`: the residual is localised to the prologue

The 243/262 baseline reproduces on this lane's assignment base. All nineteen
differing words lie in the first 0x58 bytes; from that point to the end of the
function the candidate is word-identical to the target. This is a prologue
*schedule*, not an allocation or a body defect: the frame size, every saved
register's home, and every register lane already agree.

What differs is the interleaving of the register saves with the four entry
computations (the loop cursor's address, the invariant float load, and the two
hoisted float constants). The candidate lets the assembler start the invariant
load first, so the float register it targets is saved first; the target
materialises the constants first and defers the load to the last word of the
prologue.

Search performed and eliminated:

- 3,840 cells of statement order crossed with every physical-line grouping of
  the six entry assignments -- all identical to baseline.
- 840 cells joining a suffix of those assignments onto the loop-header line.
  Line merging does collapse the debug line entries to a single line (verified
  in the phase output), which rules out the line-region barrier as the cause
  here, and the score does not move.
- Naming the two float constants as locals: the frame grows and the score
  collapses, so they are hoisted invariants and not source locals.
- Reading the invariant through the global at its use sites instead of caching
  it in a local: the frame shrinks and the score collapses, so the local is
  real.
- Moving the invariant load, the state constants, or both inside the outer
  loop: flat or worse.

Phase replay narrows it further. Re-assembling the candidate's own phase output
is byte-faithful, so the last phase can be searched directly. Reordering the
nine entry instructions inside a single line region reaches 10 differing words
-- roughly half the residual -- but 6,000 sampled orders of those nine never
reach zero. So the target's prologue is *not* obtainable by reordering this
instruction set alone; the producing phase must also differ in the save block
or emit a slightly different set.

Next lever, in order: capture the save-block ordering as a search axis in the
phase replay (the nine-instruction search above held the saves fixed), and only
then look for the C shape that produces it. Do not re-run statement order, line
grouping, or constant-naming; all three are now excluded by measurement.

#### 2026-09-09 lane `w2-bigA`: the residual is one ugen ordering constraint

Phase replay (`ugen -l` + `acpp`/`as0`/`as1` under the compiler-path `as1`
flags) is byte-faithful on this unit, so the last phase was searched directly
rather than sampled. Three results, all measured:

1. **The whole residual is reachable, and the rest of the C is already exact.**
   In `ugen`'s output the entry block emits four values -- the entries base, the
   factor load, and the two hoisted float constants. Of the 24 orders of those
   four, every one that emits `li.s $f24, 1.0` **before**
   `l.s $f20, gOverlay59ApproachFactor` reassembles to **zero differing words**;
   every one that emits it after holds at 19. Nothing else in the order matters:
   the entries base and the `0.0f` constant may sit anywhere. The previous note
   that "no reordering of the entry set reaches zero" was measuring a
   nine-instruction window that held the wrong axis fixed, and is withdrawn.

2. **The save-block axis is falsified.** All six permutations of the three
   `s.d` saves, each with the FP group before and after the ten integer saves,
   move the score to 18 at best -- `as1` re-schedules the save block from the
   value definitions, so the order it arrives in is nearly free. Do not spend
   another pass there; it was the previous note's recommended next lever.

3. **The constraint is emission order, not the `.loc` barrier.** Deleting every
   `.loc` between the load and the constants leaves the score at 19; deleting
   all `.loc`s costs four more; an inserted decreasing `.loc` costs three. The
   barrier rule is real but is not what is holding this function.

What blocks it at C level: `ugen` emits a pre-loop statement's code before the
loop preheader's hoisted invariants, always. Measured flat at 19 across all 64
physical line groupings of the six entry statements (including every statement
merged onto the `do {` line, verified in the phase output to collapse to one
`.loc` region), both orders of the two loads, and the `*(&g)` and `(&g)[0]`
spellings. Moving `factor = gOverlay59ApproachFactor` inside the outer loop
does put the load after the constants -- but `uopt` then does not hoist it at
all, because the outer loop contains calls that may alias the global, and the
load stays in the loop (27 words). Declaring the global `const` does not change
that; IDO 5.3 does not use the qualifier for its loop-invariant test.

So the open question is narrow and specific: **what C makes IDO materialise the
`1.0f` invariant ahead of a pre-loop global load into a saved FP register?**
Candidates not yet tried: a source form in which the constant is not a hoisted
loop invariant at all, and a form in which the factor load is hoisted by `uopt`
out of a call-free region. Do not re-run statement order, line grouping,
declaration order, constant naming, global inlining, the save block, or flags.

Note for whoever resumes: this target object carries four unrelocated
`%hi`/`%lo` pairs -- splat wrote them as literal `(0xNN >> 16)` splits because
the halves straddle other instructions -- so any comparator must resolve
relocations before believing a difference here. None of the 19 words is one of
those; the `lui $at, (0x3F800000 >> 16)` at +0x8 is a float immediate and must
not be masked.

<!-- plateau-handoff:overlay59Advance:end -->
