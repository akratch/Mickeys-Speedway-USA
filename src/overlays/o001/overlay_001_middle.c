#include "overlays/overlay_001.h"

/* ---- overlay1FindNextAngle ---- */


typedef struct Overlay1AngleData {
    u8 pad00[0x39C];
    f32 angle;
} Overlay1AngleData;

typedef struct Overlay1AngleObject {
    u8 pad00[0x64];
    Overlay1AngleData *data;
} Overlay1AngleObject;

extern Overlay1AngleObject **overlay1GetAngleObjectsReloc(s32 *count);
extern f32 overlay1WrapOffset(f32 first, f32 second);
extern f32 gOverlay1NextAngleLimit;

/* DKR v77/v80 and JFG contain no exact donor for this angle-selection scan. */
/* Plateau evidence reviewed 2026-08-29: the surviving source-current isolated
 * candidate has the exact 0xC8/50-word boundary and 0x68 frame. Five raw
 * positions differ: the local-data LO16 addend at +0x38 and schedule swaps at
 * +0x3C/+0x40 and +0x6C/+0x70; masking the addend leaves four. The candidate
 * has all four required relocation sites, but the assembled target bakes the
 * local HI16/LO16 pair, so it is not a fully annotated promotion oracle. The
 * earlier zero-word claim compared fallback assembly. The full flag lattice
 * and eight source-faithful cursor/init/access/FP variants missed. Re-prove
 * unchanged full-TU V0 with a runtime-annotated target, then park absent a new
 * scheduling mechanism. */
#ifdef NON_MATCHING
Overlay1AngleObject *overlay1FindNextAngle(f32 angle) {
    s32 count;
    Overlay1AngleObject **objects;
    Overlay1AngleObject **cursor;
    Overlay1AngleObject *object;
    Overlay1AngleObject *best;
    f32 difference;
    f32 bestDifference;
    s32 remaining;
    s32 loopValue;
    objects = overlay1GetAngleObjectsReloc(&count);
    bestDifference = gOverlay1NextAngleLimit;
    best = (Overlay1AngleObject *)(count - count);
    if (count != 0) {
        remaining = count - 1;
        cursor = (Overlay1AngleObject **)((u8 *)objects + (remaining << 2));
        do {
            object = *cursor;
            objects = (Overlay1AngleObject **)object->data;
            difference = overlay1WrapOffset(
                angle, ((Overlay1AngleData *)objects)->angle);
            if ((difference > 0.0f) && (difference < bestDifference)) {
                bestDifference = difference;
                best = object;
            }
            loopValue = remaining--;
            cursor = (Overlay1AngleObject **)((u8 *)cursor - 4);
        } while (loopValue);
    }
    return best;
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_middle/func_overlay_001_F0002744_184EB24.s")
#endif

/* ---- overlay1FindPreviousAngle ---- */


typedef Overlay1AngleData Overlay1PreviousAngleData;
typedef Overlay1AngleObject Overlay1PreviousAngleObject;
extern f32 overlay1WrapOffset(f32 first, f32 second);
extern f32 gOverlay1PreviousAngleLimit;

/* DKR v77/v80 and JFG contain no exact donor for this angle-selection scan. */
/* Plateau (evidence reviewed 2026-08-29): configured full-TU records and the
 * surviving isolated ranking agree on the exact 0xC8/50-word boundary and
 * -0x68 frame. Five raw positions differ first at +0x38: statement schedules
 * are swapped at +0x3C/+0x40 and +0x6C/+0x70, while +0x38 is the unresolved
 * local-data addend. The candidate has four relocations; the assembled target
 * bakes the HI16/LO16 addend, but the runtime table confirms all four roles.
 * Six directed statement/lifetime probes were neutral, and a five-minute,
 * relocation-annotated MIPS2 permuter pass reached cost 120 but no zero. Its
 * only changes were neutral/worse or moved the cursor decrement inside the
 * new-best branch. The configured full-TU candidate object no longer survives:
 * re-prove unchanged V0, then park absent a new scheduling mechanism. */
#ifdef NON_MATCHING
Overlay1PreviousAngleObject *overlay1FindPreviousAngle(f32 angle) {
    s32 count;
    Overlay1PreviousAngleObject **objects;
    Overlay1PreviousAngleObject **cursor;
    Overlay1PreviousAngleObject *object;
    Overlay1PreviousAngleObject *best;
    f32 difference;
    f32 bestDifference;
    s32 remaining;
    s32 loopValue;

    objects = overlay1GetAngleObjectsReloc(&count);
    bestDifference = gOverlay1PreviousAngleLimit;
    best = (Overlay1PreviousAngleObject *)(count - count);
    if (count != 0) {
        remaining = count - 1;
        cursor = (Overlay1PreviousAngleObject **)((u8 *)objects +
                                                  (remaining << 2));
        do {
            object = *cursor;
            objects = (Overlay1PreviousAngleObject **)object->data;
            difference = overlay1WrapOffset(
                ((Overlay1PreviousAngleData *)objects)->angle, angle);
            if ((difference > 0.0f) && (difference < bestDifference)) {
                bestDifference = difference;
                best = object;
            }
            loopValue = remaining--;
            cursor = (Overlay1PreviousAngleObject **)((u8 *)cursor - 4);
        } while (loopValue);
    }
    return best;
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_middle/func_overlay_001_F000280C_184EBEC.s")
#endif

/* ---- overlay1RefreshMode ---- */


typedef struct Overlay1ModeObject {
    u8 pad00[0x38C];
    u8 mode;
} Overlay1ModeObject;

extern void *gOverlay1ModeSource;
extern Overlay1ModeObject *gOverlay1ModeObject;
extern s32 overlay1ReadModeReloc(void *source);

/* DKR v77/v80 and JFG have no exact donor for this mode refresh wrapper. */
void overlay1RefreshMode(s32 arg0, s32 arg1, s32 arg2) {
    if (overlay1ReadModeReloc(gOverlay1ModeSource) >= 3) {
        gOverlay1ModeObject->mode = 2;
    } else {
        gOverlay1ModeObject->mode = 1;
    }
    overlay1ReadModeReloc(gOverlay1ModeSource);
}

/* ---- overlay1CallGlobal ---- */


/* Pinned DKR v77/v80 and JFG object scans found no exact donor. */
extern void overlay1GlobalCallReloc();
extern void *gOverlay1SubmitArg4;

void overlay1CallGlobal(s32 unused0, s32 unused1, s32 unused2) {
    overlay1GlobalCallReloc(gOverlay1SubmitArg4);
}

/* ---- overlay1AdvanceObjectGauges ---- */


typedef struct O1GaugeState {
    u8 pad000[0x384]; s8 level; u8 pad385[0x7B]; s32 value; s32 levelValues[1];
} O1GaugeState;
typedef struct O1GaugeObject { u8 pad00[0x64]; O1GaugeState *state; } O1GaugeObject;
typedef struct O1GaugeOwner { u8 pad00[0x86]; s8 levelLimit; } O1GaugeOwner;
extern s32 D_0;
extern s32 overlay1GetGaugeObjectsRaw(s32 *count);
extern s32 overlay1GetGaugeLimit(O1GaugeObject *object);

void overlay1AdvanceObjectGauges(O1GaugeOwner *owner, s32 amount) {
    O1GaugeObject **objects;
    O1GaugeObject *object;
    O1GaugeState *state;
    s32 count;
    s32 index;
    s32 delta;
    s32 limit;
    s32 loopValue;

    objects = (O1GaugeObject **)overlay1GetGaugeObjectsRaw(&count);
    if (count != 0) {
        index = count - 1; objects += index; do { object = *objects;
            state = object->state;
            if ((D_0 == 0) && (state->level < owner->levelLimit)) {
                delta = amount * 5;
                state->value += delta;
                limit = overlay1GetGaugeLimit(object);
                if (limit < state->value) state->value = limit;
                state->levelValues[state->level] += delta;
                if (state->levelValues[state->level] >= 180001) {
                    state->levelValues[state->level] = 180000;
                }
            }
            loopValue = index;
            objects--;
            index--;
        } while (loopValue != 0);
    }
}

/* ---- overlay1AdvanceGauge ---- */


typedef struct O1AdvanceGaugeState { u8 pad000[0x3FA]; s16 disabled; u8 pad3FC[4]; s32 value; } O1AdvanceGaugeState;
typedef struct O1AdvanceGaugeObject { u8 pad00[0x64]; O1AdvanceGaugeState *state; } O1AdvanceGaugeObject;
extern s32 D_0;
extern O1AdvanceGaugeObject **overlay1GetGaugeObjects(s32 *count);

/* Plateau (batch 14): exact 0xA8 size; 26 words differ first at +0x14.
 * Count lands at stack +0x38 versus target +0x30, cascading register colors;
 * declaration, scalar/array, s64, and aggregate-slot variants regressed. */
#ifdef NON_MATCHING
void overlay1AdvanceGauge(s32 amount) {
    volatile s32 private;
    s32 count;
    s32 index;
    s32 loopValue;
    O1AdvanceGaugeObject **objects;
    O1AdvanceGaugeObject *object;
    O1AdvanceGaugeState *state;

    objects = overlay1GetGaugeObjects(&count);
    if (count != 0) {
        index = count - 1;
        objects += index;
        do {
            object = *objects--;
            state = object->state;
            if ((D_0 == 0) && (state->disabled == 0)) {
                state->value += amount * 5;
                if (state->value >= 540001) state->value = 540000;
            }
            loopValue = index;
            index--;
        } while (loopValue != 0);
    }
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o001/overlay_001_middle/func_overlay_001_F0002AA4_184EE84.s")
#endif

/* PLATEAU-HANDOFF:overlay1FindNextAngle:start
 * symbol: overlay1FindNextAngle
 * score: 2 differing words
 * frame: 0x68
 * relocations: 4
 * first-mismatch: +0x3C
 * summary: Explicit post-decrement carrier closes the loop-tail order pair; two branch-delay order words remain and the candidate is not shape-exact.
 * PLATEAU-HANDOFF:overlay1FindNextAngle:end
 */

/* PLATEAU-HANDOFF:overlay1FindPreviousAngle:start
 * symbol: overlay1FindPreviousAngle
 * score: 2 differing words
 * frame: 0x68
 * relocations: 4
 * first-mismatch: +0x3C
 * summary: Explicit post-decrement carrier closes the loop-tail order pair; two branch-delay order words remain and the candidate is not shape-exact.
 * PLATEAU-HANDOFF:overlay1FindPreviousAngle:end
 */

/* PLATEAU-HANDOFF:overlay1AdvanceGauge:start
 * symbol: overlay1AdvanceGauge
 * score: 26 differing words
 * frame: 0x40
 * relocations: 3
 * first-mismatch: +0x14
 * summary: Fresh exact-size V0: 26 diffs and exact 0x40 frame; candidate 3 vs target 1 reloc, with D_0 HI/LO runtime identity ambiguous.
 * PLATEAU-HANDOFF:overlay1AdvanceGauge:end
 */
