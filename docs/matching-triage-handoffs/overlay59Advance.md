<!-- plateau-handoff:overlay59Advance:start -->
### `overlay59Advance` plateau handoff

- source: `src/overlays/o059/overlay59Advance.c`
- score: 243/262 words
- frame: 0x58
- relocations: 15
- first mismatch: +0x4
- summary: All 19 words are prologue schedule; body is word-identical. 4,680 order/line-grouping cells flat, constant-naming and global-inlining eliminated; phase replay reaches 10 but no reordering of the entry set reaches zero.

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
<!-- plateau-handoff:overlay59Advance:end -->
