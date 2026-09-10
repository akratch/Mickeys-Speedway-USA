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
