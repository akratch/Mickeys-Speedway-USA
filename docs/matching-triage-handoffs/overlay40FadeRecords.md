<!-- plateau-handoff:overlay40FadeRecords:start -->
### `overlay40FadeRecords` plateau handoff

- source: `src/overlays/o040/overlay40FadeRecords.c`
- score: 98/101 words
- frame: 0x8
- relocations: 10
- first mismatch: +0xC
- summary: CDX force proves the three-word load/copy residual is not colour-reachable; a different copy-web partition is required

The retained output-origin C remains exact-sized at 101 words, with the exact
`0x8` frame, all ten relocation identities, and 98/101 positional words. Its
three differences are the initial blend-current load, the following copy, and
one downstream use: the target loads the current value into `v1` and copies it
to `v0`, while the candidate loads it into `v0` and copies it to `v1`.

The instrumented IDO 5.3 CDX pass maps this function to optimizer procedure
zero. All 21 allocator decisions occur in p2, where they are visited in
ascending web order. Target and candidate have equal 27-slot pool lanes and
identical 34-slot temporary lanes, so the residual is not a population
difference.

Four force probes establish reachability. Forcing long-lived output web zero
from `v0` to `v1` regresses from three to thirteen differing words and rotates
two downstream webs. Adding the dependent web-four force reproduces that
object; attempts to assign web 29 alongside it are declined because the webs
interfere. The target's entry load therefore cannot belong to the candidate's
long-lived output web: a different copy-web partition is required upstream of
global colouring.

The natural current-origin spelling (`current` loaded first, then copied to
`output`) leaves the three entry sites byte-identical and regresses to 85/101;
cfe canonicalizes the copy chain before web formation. Preserve the retained
98/101 form. Reopen only with a new, source-authentic mechanism that changes
the initial copy partition without changing the instruction stream; do not
repeat flags, comma grouping, colour forces, declaration/scope variants, or
the current-origin spelling.
<!-- plateau-handoff:overlay40FadeRecords:end -->
