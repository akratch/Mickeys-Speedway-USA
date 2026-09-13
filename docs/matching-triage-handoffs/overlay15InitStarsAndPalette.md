<!-- plateau-handoff:overlay15InitStarsAndPalette:start -->
### `overlay15InitStarsAndPalette` plateau handoff

- source: `src/overlays/o015/overlay_015.c`
- score: 70/247 words
- frame: 0x40
- relocations: 14
- first mismatch: +0x4
- summary: Counter reuse and measured store moves reach 198 aligned exact words; 70 masked remain at exact size/frame, with adjacent scheduling controls stalled.

#### 2026-09-13, lane l1: counter reuse and measured bounds scheduling

Retained guarded source: 247 words, delta zero, seventy raw and masked
differences from plus 0x4. Alignment is 198 exact, thirteen naming, one
immediate and 32 paired structural rows, plus three candidate-only and three
target-only words. The frame and all thirteen stack-slot traffic records match
at 0x40. Candidate static relocations number fourteen against six in the
extracted target object; those different representations are not an exact
runtime relocation proof. Ownership remains overlay 15 text offsets 0x4C
through 0x428, ROM 0x18723E4 through 0x18727C0: 988 executable bytes. The
GLOBAL_ASM fallback remains active, and no new matched bytes are claimed.

The fresh procedure-2 baseline was 120 masked differences, 133 aligned exact
words, 73 naming, one immediate and 33 paired structural rows with seven gaps
on each side. Geometry and stack traffic already agreed. Its census was 114
draws and 310 emissions. Named Ucode mapping authenticates procedure 2 for
every retained source; complete stock/capture text fidelity passes throughout.
No instrumented or forced object is used as canonical compiler output.

Counter identity supplied the first gain. Reusing the completed star-loop
counter for the first palette index reached 94 masked differences. Reusing it
for the second index instead reached 65, with 185 aligned exact words and no
palette-tail naming residual. Total draws and their sequence stayed unchanged;
the definition-site emissions moved. This is allocator source identity, not a
claim that a hidden temporary draw disappeared. The 65-difference source and
object remain preserved as the best positional alternative.

Only after that source change, a fresh exhaustive landscape measured 171
single-force probes across 32 webs: 156 retained geometry and fifteen changed
size. Two address-carrier forces improved 65 to 63 individually, p1:w332=c5
and p1:w334=c3; neither reached zero. The disjoint positional footprints suggest
61 for the pair, but that combination was not compiled and is not a measured
result. The initial byte-count web cannot offer the target's occupied colour
in this shape. Existing landscapes were not repeated on unchanged source.

Reusing the other completed counter for the first palette index alone was
byte-inert. Splitting its zero initialization across the existing nonempty-loop
paths then reached 188 aligned exact words at delta plus four. The same-width
signed integer round trip of the count-global address restored delta zero and
reached 190 exact words, with six naming rows. It preserves pointer identity on
the configured 32-bit ABI; the cast is disclosed source shaping, not a new
storage identity. Generating the initial byte-count expression directly was
byte-inert. Moving the star-counter initialization into the positive-count
guard, alone or with that generated expression, worsened the result. Splitting
the initial multiply and retaining the byte count in the input parameter each
added a draw and disturbed the otherwise exact palette tail.

The draw census then located useful bounds-store moves. Advancing the y-range
store changed draw order and reached 194 aligned exact words. Advancing the
divisor store alone also reached 194, but their combination reached only 192.
Folding the x-max and y-range statements onto one source line was byte-inert.
Swapping the two existing entry counter definitions did not move any census
line and gave no gain. Separating the x-bound calculations from their stores
removed two draws but regressed to 171 exact words; reject it.

A bounded differential batch tested twelve dependency-safe adjacent statement
moves from the 194-word source. Each probe made one move, retained its census
comparison and aligned per-window delta, and preserved widths, accesses and
call order. Placing y-range after x-min reached 196 exact words; placing the
divisor store after z-max independently reached 196. Their measured combination
reached 198. The second move adds one paired structural row in one middle
window but removes two rows in the next window and removes a gap pair. This
local trade is retained explicitly; positional arithmetic alone would miss it.
The final source has 114 draws and 312 emissions. The palette tail remains
aligned exact, while the allocation-size carrier and bounds/entry scheduling
remain nonexact.

Stall evidence: the last improved shape is the paired store move, global
attempt 45. A second bounded batch checks nine previously unmeasured adjacent
neighbours of that shape and finds no improvement. The last three controls
(attempts 52 through 54) retain 198 aligned exact words and seventy masked
differences with unchanged geometry and stack traffic. They supply no new
carrier identity or residual reduction. Stop under ADR 0018 with this local
neighbourhood closed, not with a claim that all source schedules are exhausted.
A next pass needs a new trace-backed mechanism for the initial byte-count
interference or the remaining nonlocal store/entry scheduling; repeating these
adjacent moves or the unchanged colour landscape is not a new hypothesis.

Semantic review: the star loop retains its guard, calls, widths, increments
and global-count retest. Both paths enter the palette loop with the first
index zero; the reused second index is explicitly reset to one. The same four
palette formulas, index increments and 256-entry extent remain. The moved
bounds stores access distinct nonvolatile fields without intervening calls;
all field-read dependencies remain ordered. Source spelling and the concise
reuse comment are recompiled after selection. Meaningful alternatives are
preserved, not overwritten.

Evidence lives under ignored build/l1/overlay15InitStarsAndPalette, including
baseline, reuse-second-index, signed-count-address, paired-store-moves,
landscape65, adjacent-results.json, schedule-results.json and each saved
source/object/census/delta. Commands include configured full-TU compilation,
allocator_trace_receipt mapping/fidelity, draw_census profiling/comparison,
residual_map object comparison, web_footprint --every-colour on the changed
source, finalize_plateau, and tools/gates.sh. Stock output remains nonexact;
the normal fallback ROM is separately verified before committing.

<!-- plateau-handoff:overlay15InitStarsAndPalette:end -->
