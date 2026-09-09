#include "overlays/overlay019.h"

/*
 * Classify an edge against a candidate edge, accepting either orientation.
 * PROVENANCE: Mickey-derived. The exact-donor ledger is negative. A retained
 * structural scan found JFG assembly-only
 * func_overlay_30_000012F0_1F4A3C0 at 0.443 and DKR public
 * src/object_models.c:func_80060C58, a five-argument edge classifier, at 0.227;
 * neither is an exact donor and no external C is adapted here.
 * Bounded full-TU reproof (2026-08-29): configured V0 is exact-sized at
 * 120 words / 0x1E0, frameless, and has no relocations or owned padding. It
 * matches 110/120 positional words, first +0x138. Six sites are one visible
 * carrier web at +0x138/+0x140/+0x148/+0x154/+0x15C/+0x164; two final y/z
 * load pairs account for +0x194/+0x198 and +0x1B0/+0x1B4. Candidate `.text`
 * SHA-256 is cbc6e4fc4f6b5b810bc239c6cef0ea183c1084cf4c2eafa35269ee3d04acbca5.
 * The sole inbound is overlay19FindAdjacent+0xD8; this function is unexported.
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
 */
#ifdef NON_MATCHING
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

    if (((queryStartIndex == candidateStartIndex) &&
         (queryEndIndex == candidateEndIndex)) ||
        ((queryStartIndex == candidateEndIndex) &&
         (queryEndIndex == candidateStartIndex))) {
        return 1;
    }

    candidateStart = &vertices[candidateStartIndex];
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
    if (queryStart->y < candidateEnd->y) {
        goto no_match;
    }
    if (queryStartX < queryStart->y) {
        goto no_match;
    }
    queryStartX = candidateEnd->z;
    if (queryStart->z < candidateEnd->z) {
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
    if (queryEnd->y < candidateStart->y) {
        goto no_match;
    }
    if (candidateStart->y < queryEnd->y) {
        goto no_match;
    }
    if (queryEnd->z < candidateStart->z) {
        goto no_match;
    }
    if (candidateStart->z < queryEnd->z) {
        goto no_match;
    }
    return 2;

no_match:
    return 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o019/overlay19ClassifyEdge/func_overlay_019_F0000D78_1875FD0.s")
#endif
