#include "PR/ultratypes.h"

typedef struct Overlay40Color {
    u8 red;
    u8 green;
    u8 blue;
} Overlay40Color;

typedef struct Overlay40ColorSource {
    Overlay40Color *colors;
    s16 *groups;
} Overlay40ColorSource;

typedef struct Overlay40Vertex {
    u8 pad00[6];
    s8 red;
    s8 green;
    s8 blue;
    u8 pad09;
} Overlay40Vertex;

typedef struct Overlay40FadeRecord {
    Overlay40Vertex *vertices;
    u8 pad04[0x1C];
    s16 count;
    u8 pad22[0xC];
    u8 dirty;
    u8 pad2F;
    Overlay40ColorSource *source;
    u8 pad34[0xC];
} Overlay40FadeRecord;

typedef struct Overlay40FadeContext {
    u8 pad00[4];
    Overlay40FadeRecord *records;
    u8 pad08[0x12];
    s16 count;
} Overlay40FadeContext;

extern s16 gOverlay40BlendTimer;
extern s16 gOverlay40BlendCurrent;
extern s16 gOverlay40BlendTarget;
extern s16 gOverlay40BlendDuration;
extern s16 gOverlay40BlendOutput;

/* Plateau re-reviewed 2026-08-29 against retained-line-layout configured
 * full-TU and isolated C: both are 98/101 raw and relocation-normalized words,
 * first +0xC, with frame 0x8 and only +0xC/+0x10/+0x24 differing. Its ten
 * static relocations (five HI16/LO16 pairs) map to
 * runtime BSS D_800D6C4C(timer), D_800D6C52(current), D_800D6C50(target),
 * D_800D6C4E(duration), and D_800D6C54(output); the target/fallback objects
 * retain none of them statically. The former zero-word claim compared the
 * linked GLOBAL_ASM fallback, not this C candidate. Historical prose says a
 * linked promotion trial rejected C, but no candidate ELF/ROM survives.
 * Historical prose reports that an instrumented guide-19 oracle moved 16 rows
 * across 11 runs and worsened the residual to 13 words; chained,
 * comma-expression, and timer-separated copy formation reportedly widened it
 * to 16, 16, and 41 words. No attributable source/object pair for those probes
 * survives, so they are scheduling context rather than exhaustion proof. The
 * policy-complete ladder is: fresh HEAD V0 and a retained near-match oracle;
 * all 119 flags scored raw; one allocator
 * trace; independent output-origin and output/current/timer declaration-order
 * probes; combine or initialize only after strict gains. The hard cap is 123
 * deterministic builds plus one trace: 119 flags and four additional forms,
 * five total forms including V0. Mickey-only reconstruction; exact pinned DKR
 * v77/v80/JFG overlay-range scans are negative, but no near-match oracle report
 * survives. */
#ifdef NON_MATCHING
void overlay40FadeRecords(register s32 *enabled, Overlay40FadeContext *context,
                          s32 amount) {
    Overlay40FadeRecord *record;
    Overlay40ColorSource *source;
    Overlay40Color *color;
    Overlay40Vertex *vertex;
    s16 *group;
    s32 current;
    s32 output;
    s32 timer;
    s32 remaining;
    s32 groupRemaining;
    s32 vertexRemaining;

    current = gOverlay40BlendCurrent;
    output = current;
    timer = gOverlay40BlendTimer;
    if (timer != 0) {
        if (amount < timer) {
            gOverlay40BlendTimer = timer - amount;
            current += ((gOverlay40BlendTarget - output) *
                       gOverlay40BlendTimer) / gOverlay40BlendDuration;
        } else {
            gOverlay40BlendTimer = 0;
        }
        gOverlay40BlendOutput = current;
    }

    record = context->records;
    remaining = context->count;
    while (remaining--) {
        if (*enabled != 0) {
            source = record->source;
            if (source != 0) {
                group = source->groups;
                groupRemaining = (record->count + 0xF) >> 4;
                while (groupRemaining--) {
                    *group++ = 0;
                }

                color = source->colors;
                vertexRemaining = record->count;
                vertex = record->vertices;
                while (vertexRemaining--) {
                    vertex->red = (color->red * current) >> 8;
                    vertex->green = (color->green * current) >> 8;
                    vertex->blue = (color->blue * current) >> 8;
                    vertex++;
                    color++;
                }
                record->dirty = 0;
            }
        }
        enabled++;
        record++;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o040/overlay40FadeRecords/func_overlay_040_F0000690_1886F40.s")
#endif
