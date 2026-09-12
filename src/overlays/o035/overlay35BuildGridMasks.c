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

void func_overlay_035_F0000770_1882450(O35GridSource *source,
                                        O35GridBounds *bounds) {
    O35Vertex *vertices;
    s32 vertex;
    s32 mask;
    s32 end;
    s16 startVertex;
    s16 baseIndex;
    s32 groupIndex;
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
    s32 flags;
    u8 zMask;
    s16 start;
    s16 step;
    s16 value;
    s16 origin;
    s16 x;
    s16 y;
    s16 z;

    groupIndex = 0;
    if (source->groupCount > 0) {
        do {
            startVertex = *(s16 *)(source->groups + groupIndex * 0x10 + 8);
            baseIndex = *(s16 *)(source->groups + groupIndex * 0x10 + 6);
            end = *(s16 *)(source->groups + groupIndex * 0x10 + 0x18);
            flags = *(s32 *)(source->groups + groupIndex * 0x10 + 0xC);
            vertex = startVertex;
            if (vertex < end) {
                do {
                    if (((flags & 0x1080) != 0) && ((flags & 0x08010000) == 0)) {
                        *(u32 *)((u8 *)source->masks + vertex * 4) = 0;
                        source->zMasks[vertex] = 0;
                    } else {
                        maxX = -0x7FBC;
                        maxY = -0x7FBC;
                        maxZ = -0x7FBC;
                        minX = 0x7FBC;
                        minY = 0x7FBC;
                        minZ = 0x7FBC;
                        index = source->indices + vertex * 0x10;
                        i = 0;
                        vertices = source->vertices;
                        for (; i < 3; i++) {
                            v = &vertices[index[i + 1] + baseIndex];
                            x = v->x;
                            y = v->y;
                            z = v->z;
                            if (maxX < x) maxX = x;
                            if (x < minX) minX = x;
                            if (maxY < y) maxY = y;
                            if (y < minY) minY = y;
                            if (maxZ < z) maxZ = z;
                            if (z < minZ) minZ = z;
                        }

                        bit = 1;
                        mask = 0;
                        origin = bounds->x0;
                        step = ((bounds->x1 - origin) >> 4) + 1;
                        value = step + origin;
                        start = origin;
                        for (x = 0; x < 16; x++) {
                            if ((value >= minX) && (maxX >= start)) {
                                mask |= bit;
                            }
                            value += step; start += step;
                            bit *= 2;
                        }

                        origin = bounds->y0;
                        step = ((bounds->z1 - origin) >> 4) + 1;
                        value = step + origin;
                        start = origin;
                        for (x = 0; x < 16; x++) {
                            if ((value >= minZ) && (maxZ >= start)) {
                                mask |= bit;
                            }
                            value += step; start += step;
                            bit *= 2;
                        }
                        *(u32 *)((u8 *)source->masks + vertex * 4) = mask;

                        bit = 1;
                        zMask = 0;
                        step = ((bounds->y1 - bounds->z0) >> 3) + 1;
                        origin = bounds->z0;
                        value = step + origin;
                        start = origin;
                        for (x = 0; x < 8; x++) {
                            if ((value >= minY) && (maxY >= start)) {
                                zMask |= bit;
                            }
                            value += step; start += step;
                            bit *= 2;
                        }
                        source->zMasks[vertex] = zMask;
                    }
                    vertex++;
                } while (vertex < end);
            }
            groupIndex++;
        } while (groupIndex < source->groupCount);
    }
}
