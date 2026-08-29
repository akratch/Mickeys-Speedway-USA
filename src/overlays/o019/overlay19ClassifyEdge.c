#include "overlays/overlay019.h"

/*
 * Classify an edge against a candidate edge, accepting either orientation.
 * PROVENANCE: Mickey-derived. The exact-donor ledger is negative. A retained
 * structural scan found JFG assembly-only
 * func_overlay_30_000012F0_1F4A3C0 at 0.443 and DKR public
 * src/object_models.c:func_80060C58, a five-argument edge classifier, at 0.227;
 * neither is an exact donor and no external C is adapted here.
 * Reproof lead (evidence reviewed 2026-08-29): the generated isolated scalar
 * repeatedly records the unchanged body at exact size, 110/120 positional
 * words, first +0x138, category register-only. No isolated object/report is
 * retained. Historical configured full-TU prose reports the same score,
 * frameless/zero-relocation shape, and a carrier web at +0x138/+0x140/+0x148/
 * +0x154/+0x15C/+0x164 with final y/z load pairs reversed at +0x194/+0x198 and
 * +0x1B0/+0x1B4, but no configured object/hash/site map survives. Configured
 * score, frame, relocations, and the complete ten-site list therefore require
 * reproof.
 * The sole inbound is overlay19FindAdjacent+0xD8; this function is unexported.
 *
 * Historical prose attributes the gain from 106/120 to promoted-s16
 * comparisons and reports several flat source/trace/permuter probes, but their
 * attributable objects do not survive and do not prove exhaustion. Ordinary
 * object, linked-function, module, and ROM identity prove the GLOBAL_ASM
 * fallback only; no linked C candidate survives. Compile configured V0 after
 * the retained oracle and invalidate the detailed plateau if 110/120,
 * framelessness, or zero records fails to reproduce. Otherwise retain exactly
 * 119 configurations including V0, trace once, try one natural carrier-
 * lifetime/DKR-faithful lexical form and one independent y/z line-association
 * form, then combine only if both strictly improve. Hard stop at 122
 * deterministic builds plus one trace; no generic batch or forced-color
 * promotion.
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
