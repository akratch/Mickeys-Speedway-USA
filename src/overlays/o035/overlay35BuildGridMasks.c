#include "ultra64.h"

typedef struct O35Vertex {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[4];
} O35Vertex;

typedef struct O35GridBounds {
    s16 x0;
    s16 z0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 z1;
} O35GridBounds;

typedef struct O35GridSource {
    O35Vertex *vertices;
    u8 *indices;
    u8 pad8[4];
    u8 *groups;
    u32 *masks;
    u8 *zMasks;
    u8 pad18[0xC];
    s16 groupCount;
} O35GridSource;

/* 244 instructions, frame 0x80 and an empty relocation surface are exact; 59 masked words
 * fell to 54 on 2026-09-12 (lane p7-ovl3) by two independent levers, each measured alone:
 *
 *   - Declaring `end` before `vertex` in the inner block is 59 -> 57.  [L99]: cfe homes a
 *     memory-class local in declaration order, and this one transposition moves two stack
 *     displacements onto the target's (aligned immediate-only 12 -> 10, byte-exact 197 -> 199).
 *     Declaration order is NOT free here: of 1,500 random permutations of the three blocks only
 *     42 keep the 0x80 frame at all, the rest pad out to 0x88 or 0x90, and a permutation that
 *     scores better while growing the frame is not closer.  Permuting only WITHIN a same-type
 *     run preserves the padding and therefore the frame; 600 such points were measured and
 *     every one holds 0x80, with the aligned residual flat at byte-exact 199 -- so this axis
 *     is at its floor, not merely sampled.
 *   - Pairing the four loop-tail statements two per physical line (`value += step; start += step;`
 *     then `bit *= 2; x++;`) is 59 -> 56 with the aligned buckets UNCHANGED: it buys displacement
 *     tax (9 -> 6), not agreement.  [L59]/[L132].  All 24 statement orders x 8 line groupings were
 *     measured; folding all four onto ONE line scores 51 but costs 6 byte-exact rows and adds 3
 *     structural ones, which is the trap in reading the positional number alone.
 *
 * The 54 that remain are 27 register naming (s1/s2, a2/a3 and v1/a0 transpositions), 10
 * immediate-only (two stack homes still on the wrong slot) and 11 structural.  The structural
 * eleven are three copies of one fact, at +0x214, +0x298 and +0x334: the target sign-extends the
 * s16 loop counter immediately after the two `sll`s and tests it three words earlier than we do,
 * so `sra`/`slti` land ahead of the `value`/`start` extensions.  Measured flat against that:
 * every statement order and line grouping above, `} while (++x < 16)`, `x != 16`, s32 counters
 * (all three explode the frame), and both operand orders of the `&&`. */
#ifdef NON_MATCHING
void func_overlay_035_F0000770_1882450(O35GridSource *source,
                                        O35GridBounds *bounds) {
    s32 groupIndex;
    s32 groupOffset;

    groupIndex = 0;
    if (source->groupCount > 0) {
        groupOffset = 0;
        do {
            u8 *group;
            s32 end;
            s32 flags;
            s32 vertex;
            s16 startVertex;
            s16 baseIndex;

            group = source->groups + groupOffset;
            startVertex = *(s16 *)(group + 8);
            baseIndex = *(s16 *)(group + 6);
            end = *(s16 *)(group + 0x18);
            flags = *(s32 *)(group + 0xC);
            vertex = startVertex;
            if (vertex < end) {

                do {
                    if (((flags & 0x1080) != 0) && ((flags & 0x08010000) == 0)) {
                        *(u32 *)((u8 *)source->masks + vertex * 4) = 0;
                        source->zMasks[vertex] = 0;
                    } else {
                        O35Vertex *v;
                        u8 *index;
                        s16 maxX;
                        s16 maxY;
                        s16 maxZ;
                        s16 minX;
                        s16 minY;
                        s16 minZ;
                        s32 i;
                        s32 bit;
                        s32 mask;
                        u8 zMask;
                        s16 start;
                        s16 step;
                        s16 value;
                        s16 origin;
                        s16 x;
                        s16 y;
                        s16 z;

                        maxX = -0x7FBC;
                        maxY = -0x7FBC;
                        maxZ = -0x7FBC;
                        minX = 0x7FBC;
                        minY = 0x7FBC;
                        minZ = 0x7FBC;
                        index = source->indices + vertex * 0x10;
                        i = 0;
                        do {
                            v = &source->vertices[index[1] + baseIndex];
                            x = v->x;
                            y = v->y;
                            z = v->z;
                            if (maxX < x) maxX = x;
                            if (x < minX) minX = x;
                            if (maxY < y) maxY = y;
                            if (y < minY) minY = y;
                            if (maxZ < z) maxZ = z;
                            if (z < minZ) minZ = z;
                            i++;
                            index++;
                        } while (i != 3);

                        bit = 1;
                        mask = 0;
                        origin = bounds->x0;
                        step = ((bounds->x1 - origin) >> 4) + 1;
                        value = step + origin;
                        start = origin;
                        x = 0;
                        do {
                            if ((value >= minX) && (maxX >= start)) {
                                mask |= bit;
                            }
                            value += step; start += step;
                            bit *= 2; x++;
                        } while (x < 16);

                        origin = bounds->y0;
                        step = ((bounds->z1 - origin) >> 4) + 1;
                        value = step + origin;
                        start = origin;
                        x = 0;
                        do {
                            if ((value >= minZ) && (maxZ >= start)) {
                                mask |= bit;
                            }
                            value += step; start += step;
                            bit *= 2; x++;
                        } while (x < 16);
                        *(u32 *)((u8 *)source->masks + vertex * 4) = mask;

                        bit = 1;
                        zMask = 0;
                        step = ((bounds->y1 - bounds->z0) >> 3) + 1;
                        origin = bounds->z0;
                        value = step + origin;
                        start = origin;
                        x = 0;
                        do {
                            if ((value >= minY) && (maxY >= start)) {
                                zMask |= bit;
                            }
                            value += step; start += step;
                            bit *= 2; x++;
                        } while (x < 8);
                        source->zMasks[vertex] = zMask;
                    }
                    vertex++;
                } while (vertex < end);
            }
            groupIndex++;
            groupOffset += 0x10;
        } while (groupIndex < source->groupCount);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o035/overlay35BuildGridMasks/func_overlay_035_F0000770_1882450.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_035_F0000770_1882450:start
 * symbol: func_overlay_035_F0000770_1882450
 * score: 54 differing words
 * frame: 0x80
 * relocations: 0
 * first-mismatch: +0x2C
 * summary: 59 fell to 54 on two separable levers. Declaring `end` before `vertex` is 59 to 57 and moves two stack homes onto the target's (immediate-only 12 to 10, byte-exact 197 to 199), which is [L99] read as a linear order. Pairing the four loop-tail statements two per physical line is 59 to 56 with the aligned buckets unchanged, so it buys displacement tax and not agreement ([L59]). Declaration order is bounded rather than sampled: permuting within a same-type run preserves cfe's padding and therefore the 0x80 frame, 600 such points are all 0x80 and all flat at byte-exact 199, while 1,458 of 1,500 unconstrained permutations grow the frame to 0x88 or 0x90 and several of those score better while being further away. The 54 left are 27 naming, 10 immediate-only and 11 structural, and the eleven are three copies of one fact at +0x214, +0x298 and +0x334: the target sign-extends the s16 loop counter right after the two shifts and tests it three words earlier. Flat against that fact: all 24 tail statement orders by 8 line groupings, `while (++x < 16)`, `x != 16`, s32 counters and both operand orders of the guard. 2026-09-12 lane p8-close reproduced 54 at delta 0 and sharpened the eleven structural words: both sides emit the SAME eleven instructions in the loop tail, so this is a schedule permutation and not a missing or surplus operation. The target orders the counter's widen-and-narrow pair, then the running value's, then the running start, then the guard; we interleave the start update between the two pairs and put the guard one slot later. No edit was made.
 * PLATEAU-HANDOFF:func_overlay_035_F0000770_1882450:end
 */
