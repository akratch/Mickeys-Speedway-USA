#include "overlays/overlay019.h"

/*
 * Classify an edge against a candidate edge, accepting either orientation.
 * PROVENANCE: Mickey-derived. The exact-donor ledger is negative. A retained
 * structural scan found JFG assembly-only
 * func_overlay_30_000012F0_1F4A3C0 at 0.443 and DKR public
 * src/object_models.c:func_80060C58, a five-argument edge classifier, at 0.227;
 * neither is an exact donor and no external C is adapted here.
 * The sole inbound is overlay19FindAdjacent+0xD8; this function is unexported.
 *
 * Matched 2026-09-10 by two edits, neither of which works alone:
 *
 * 1. The reversed-orientation block compares against `queryStartX` rather than
 *    re-reading `candidateEnd->y`/`->z`. The direct read is a separate common
 *    subexpression, and its web is barred from `queryStartX`'s register
 *    because the staged variable stays live into the trailing block; comparing
 *    through the staged name lets the load land in that register directly, the
 *    way the forward block already did.
 *
 * 2026-09-10, lane c6-close: the carrier is now *identified*, and closing it
 * opens a second, larger defect. The reversed-coordinate block reads
 * `candidateEnd->y` a second time in its first test while the staged local
 * carries the same value into the second; uopt common-subexpressions the two
 * reads into a fresh temporary web and copy-propagates the local away, so the
 * value at +0x138/+0x154 belongs to a web live only across those six rows and
 * first-strict-minimum hands it v1. Spelling that first test against the
 * staged local instead -- the shape the forward block already uses -- leaves
 * the value in `queryStartX`'s own web, which is live from +0x50 and therefore
 * interferes with the `candidateEndIndex` parameter in v1, so it colours t3.
 * That closes all six words: +0x138/+0x140/+0x148/+0x154/+0x15C/+0x164 become
 * byte-identical.
 *
 * The cost is 24 words in one substitution. Dropping those two CSE webs
 * renumbers the pool: `candidateStart` and the `candidateEndIndex` parameter
 * trade v0 for v1, and the two forward/tail `queryEnd->y` temporaries follow
 * whichever register `candidateStart` left free. Nothing reached that swap.
 * The named decision variable is the colour order between the `candidateStart`
 * pointer web and the `candidateEndIndex` parameter web, and it was measured
 * against 4,190 forms: the exhaustive 4,096-form lattice over the six staged
 * comparison pairs (each of test 1 and test 2 spelled as the local or as the
 * field), 82 declaration-order and `register`-qualifier forms, and 12
 * structural forms (entry-test operand and clause swaps, pointer-arithmetic
 * shapes, a self-assignment read-back, two `if (1)` region boundaries, three
 * alternative carrier variables, and a local copy of the stack parameter).
 * Every form that reaches the t3 carrier scores exactly 24; the global minimum
 * over the whole lattice is 6, which is this retained shape. The two defects
 * are exclusive under every spelling tried, so the next lever has to be one
 * that adds or removes an interfering web without touching the comparisons.
 *
 * The complete 119-configuration lattice is nonexact; thirteen O2/MIPS-II
 * rows tie V0. One instrumented uopt/ugen trace is fidelity-clean and confirms
 * separate zero-cost pool-carrier and temporary-lane choices. Direct DKR-style
 * fields regress to 108/120; staged y/z values regress to 98/120; reversing
 * the final equality checks stays 110/120 and only trades schedule sites for
 * register sites. No strict gain authorized a combination or generic batch.
 * Preserve the GLOBAL_ASM fallback; linked range/module/ROM equality proves
 * only that fallback. Resume only with a new natural pool-to-temporary or
 * line-association mechanism; do not repeat flags, these three forms, forced
 * colors, or the old broad batch.
 * 2. `candidateStart` is defined above the early-exit test rather than below
 *    it. Web identifiers follow first-definition order and the second
 *    allocator phase colours in that order, so hoisting the definition puts
 *    this web ahead of the stack-passed fifth argument's and it takes the
 *    first free colour. Written below the test, the argument is coloured
 *    first and the two swap registers across sixteen words.
 */
s32 overlay19ClassifyEdge(
    O19Vertex *vertices,
    s32 queryStartIndex,
    s32 queryEndIndex,
    s32 candidateStartIndex,
    s32 candidateEndIndex) {
    O19Vertex *queryStart;
    O19Vertex *queryEnd;
    O19Vertex *candidateStart;
    O19Vertex *candidateEnd;
    s32 queryStartX;
    s32 queryEndX;
    s32 candidateStartX;
    s32 candidateEndX;
    s32 candidateStartXMinusOne;

    candidateStart = &vertices[candidateStartIndex];
    if (((queryStartIndex == candidateStartIndex) &&
         (queryEndIndex == candidateEndIndex)) ||
        ((queryStartIndex == candidateEndIndex) &&
         (queryEndIndex == candidateStartIndex))) {
        return 1;
    }

    queryStart = &vertices[queryStartIndex];
    candidateStartX = candidateStart->x;
    candidateStartXMinusOne = candidateStartX - 1;
    queryStartX = queryStart->x;

    if (!(candidateStartXMinusOne < queryStartX) ||
        (candidateStartX < queryStartX)) {
        goto check_reversed_coordinates;
    }
    if (candidateStart->y >= (queryStart->y + 1)) {
        goto check_reversed_coordinates;
    }
    if (queryStart->y > candidateStart->y) {
        goto check_reversed_coordinates;
    }
    if (candidateStart->z >= (queryStart->z + 1)) {
        goto check_reversed_coordinates;
    }
    if (queryStart->z > candidateStart->z) {
        goto check_reversed_coordinates;
    }

    queryEnd = &vertices[queryEndIndex];
    candidateEnd = &vertices[candidateEndIndex];
    candidateEndX = candidateEnd->x;
    queryEndX = queryEnd->x;
    if (queryEndX < candidateEndX) {
        goto no_match;
    }
    if (candidateEndX < queryEndX) {
        goto no_match;
    }
    queryStartX = candidateEnd->y;
    if (queryEnd->y < queryStartX) {
        goto no_match;
    }
    if (queryStartX < queryEnd->y) {
        goto no_match;
    }
    queryStartX = candidateEnd->z;
    if (queryEnd->z < queryStartX) {
        goto no_match;
    }
    if (queryStartX < queryEnd->z) {
        goto no_match;
    }
    return 2;

check_reversed_coordinates:
    candidateEnd = &vertices[candidateEndIndex];
    candidateEndX = candidateEnd->x;
    if (queryStartX < candidateEndX) {
        goto no_match;
    }
    if (candidateEndX < queryStartX) {
        goto no_match;
    }
    queryStartX = candidateEnd->y;
    if (queryStart->y < queryStartX) {
        goto no_match;
    }
    if (queryStartX < queryStart->y) {
        goto no_match;
    }
    queryStartX = candidateEnd->z;
    if (queryStart->z < queryStartX) {
        goto no_match;
    }
    if (queryStartX < queryStart->z) {
        goto no_match;
    }

    queryEnd = &vertices[queryEndIndex];
    queryEndX = queryEnd->x;
    if (!(candidateStartXMinusOne < queryEndX) ||
        (candidateStartX < queryEndX)) {
        goto no_match;
    }
    queryStartX = candidateStart->y;
    if (queryEnd->y < candidateStart->y) {
        goto no_match;
    }
    if (queryStartX < queryEnd->y) {
        goto no_match;
    }
    queryStartX = candidateStart->z;
    if (queryEnd->z < candidateStart->z) {
        goto no_match;
    }
    if (queryStartX < queryEnd->z) {
        goto no_match;
    }
    return 2;

no_match:
    return 0;
}
