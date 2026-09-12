#include "overlays/overlay019.h"

/* Guarded candidate: 36 differing words at exact size, frame 0x80.
 * Naming the fifth argument and removing redundant signed narrowing improve
 * allocation, but a moved load and temporary-register differences remain. */
#ifdef NON_MATCHING
void overlay19BuildAdjacency(
    O19Context *context,
    O19Group *group,
    O19Output *output) {
    struct A30Frame {
        s32 outputOffset;
        s32 spanOffset;
        s32 suppressed;
        u8 pad0C[0x18];
        s32 itemEnd;
        u8 pad28[0x0C];
        s32 spanIndex;
    } frame;
    s32 itemIndex;
    s32 edgeIndex;
    s32 nextEdgeIndex;
    s32 edgeOffset;
    s32 adjacentItem;
    s32 edgeLimit;
    u16 invalid;
    s16 vertexBase;
    s16 spanCount;
    s16 itemStart;
    O19Span *span;

    frame.spanIndex = 0;
    spanCount = group->spanCount;
    if (spanCount > 0) {
        frame.spanOffset = 0;
        edgeLimit = 6;
        invalid = 0xFFFF;
        do {
            span = (O19Span *)((u8 *)group->spans + frame.spanOffset);
            frame.itemEnd = (span + 1)->itemStart +
                (itemStart = span->itemStart,
                 vertexBase = span->vertexBase,
                 0);
            itemIndex = itemStart;
            adjacentItem = span->flags;
            if (itemStart < frame.itemEnd) {
                adjacentItem &= 0x1080;
                frame.suppressed = adjacentItem & 0x7FFF; frame.outputOffset = itemIndex << 3;
                do {
                    if (frame.suppressed != 0) {
                        *(u16 *)((u8 *)output->records + frame.outputOffset) = invalid;
                        edgeIndex = 0;
                        do {
                            O19AdjacencyRecord *invalidRecords;

                            invalidRecords = output->records;
                            *(u16 *)&invalidRecords[itemIndex]
                                .edgeNeighbor[edgeIndex] = invalid;
                            edgeIndex++;
                        } while (edgeIndex < 3);
                    } else {
                        *(s16 *)((u8 *)output->records + frame.outputOffset) =
                            (s16)itemIndex;
                        edgeIndex = 0;
                        do {
                            nextEdgeIndex = edgeIndex + 1;
                            if (nextEdgeIndex >= 3) {
                                nextEdgeIndex = 0;
                            }
                            edgeOffset = group->points[itemIndex].selectors[nextEdgeIndex] + vertexBase;
                            adjacentItem = overlay19FindAdjacent(
                                context,
                                group,
                                itemIndex,
                                group->points[itemIndex].selectors[edgeIndex] +
                                    vertexBase,
                                edgeOffset);
                            if (adjacentItem == -1) {
                                *(u16 *)&output->records[itemIndex]
                                    .edgeNeighbor[edgeIndex] = 0xFFFE;
                            } else {
                                output->records[itemIndex]
                                    .edgeNeighbor[edgeIndex] =
                                    (s16)adjacentItem;
                            }
                            edgeIndex++;
                        } while (edgeIndex != 3);
                    }
                    itemIndex++;
                    frame.outputOffset += 8;
                } while (itemIndex < frame.itemEnd);
            }
            frame.spanOffset += sizeof(O19Span);
            frame.spanIndex++;
            spanCount = group->spanCount;
        } while (frame.spanIndex < spanCount);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o019/overlay19BuildAdjacency/func_overlay_019_F0000A30_1875C88.s")
#endif

/* PLATEAU-HANDOFF:overlay19BuildAdjacency:start
 * symbol: overlay19BuildAdjacency
 * score: 36/123 words
 * frame: 0x80
 * relocations: 1
 * first-mismatch: +0x7C
 * summary: Named fifth argument and redundant-narrowing removal improve 41 to 36. No colour winners remain; ring draws and one moved-load gap need a new source identity.
 * PLATEAU-HANDOFF:overlay19BuildAdjacency:end
 */
