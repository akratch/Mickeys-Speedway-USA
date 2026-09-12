<!-- plateau-handoff:packInit:start -->
### `packInit` plateau handoff

- source: `src/main/saves.c`
- score: 34/115 words
- frame: 0x60
- relocations: 20
- first mismatch: +0xA0
- summary: Generated bit nearly clears naming but loses eight bytes; carrier and success-store follow-ups retain the baseline.
#### 2026-09-12, lane p23-lastmile5: exhaustive colour evidence

The configured baseline is 115 words, 34 masked differences, delta zero,
frame 0x60, 20 relocations, first +0xA0. The older header's 81 is the agreeing
count. Aligned buckets: 81 exact, 25 naming, zero immediate, nine structural;
there are no surplus or missing words. No source was adopted.

Captured Ucode authenticates procedure 29 of 42. Stock/instrumented fidelity
passes text, data, rodata, symbols and relocations. The complete same-save-kind
landscape covers all 12 coloured webs: 69 compiled and accepted alternatives
at delta zero and one web with no alternative. The exhaustive winners list is
`p1:w19=c7` at 25 and `p1:w30=c7` at 27. The three-cell lattice reproduces both
singles and scores their accepted pair at 28, ten words worse than its additive
prediction. Their overlapping radii make them rivals. The measured diagnostic
lattice floor is 25, while stock remains 34.

Six source forms are retained privately under `build/p23/packInit/`. A named
PFS pointer assigned before or after the rumble pointer scores 37 at equal
instruction count, but grows the frame by eight bytes; its aligned buckets
are 78 exact, 24 naming, three immediate and ten structural. Removing the
second-loop rumble carrier and indexing the global directly remains 34.
Crossing those three shapes with a word-sized controller bound gives 37 in
all three cases. These final three cells neither improve the retained residual
nor establish a new identity, meeting the stall rule. No frame-growing form
was adopted for a smaller local register residual.

The remaining source question is the lifetime and materialization point of the
PFS array base relative to the controller bound, followed by the success-store
schedule. Introducing its local pointer does not move that base as required;
inlining the rumble pointer is commoned back into the existing address range.
Neither force winner can fix all structural rows on the current shape.

#### 2026-09-12, lane p24-recipe: generated controller bit is informative but short

The stock baseline remains 115 words, delta zero, frame 0x60, twenty
relocations, 34 raw/masked differences and first mismatch +0xA0. Its aligned
buckets are 81 exact, 25 naming, zero immediate and nine structural rows.
The existing authenticated 12-web landscape and rival-force floor 25 were
reused; no source change was adopted.

Deleting the controller-bit carrier and writing one shifted by controllerIndex
at its uses yields 113 words, delta minus eight, with 35 positional differences.
The aligned result is substantially different: 96 exact, one naming, one
immediate and fifteen paired structural rows, plus two target-only words.
The naming residual largely disappears, but the loop now computes the bit
from the index instead of preserving the target's byte-sized recurrence. This
is a retained structural diagnostic, not a matched or equal-geometry candidate.
Casting the generated bit to u8 restores 115 words but yields 57 masked and
41 paired residual rows, with two surplus/missing pairs; it does not restore
the target recurrence. A word-sized declared bit is eight bytes short at 73.

Deleting the bound carrier scores 76 at delta minus four. Replacing both
rumble-pointer uses with indexing scores 57 at equal geometry but changes the
first loop and frame displacements. Keeping a separate reset pointer while
indexing only the second loop is flat at 34. Reusing ret for the reset counter
gives 38 at delta zero, changing only four previously agreeing entry rows.
Preloading the success status through ret, both on separate lines and grouped
with its stores, is flat at 34; propagation restores the original schedule.
All sources, objects and aligned per-window deltas live privately under
build/p24/packInit. One mistyped field in the reset probe was corrected before
its successful compile; the failed compiler artifact is retained separately.

The final counter and two status-preload forms establish no better residual or
new target identity, meeting ADR 0018. The named source question is a bit
recurrence with the target's extra temporary draw and PFS-base materialization,
followed by interleaved success loads/stores. Deleting the bit explains much of
the naming symptom, but it also removes the byte-mask recurrence that the
shipped loop retains. Do not count its improved aligned diagnostic as a match.

<!-- plateau-handoff:packInit:end -->
