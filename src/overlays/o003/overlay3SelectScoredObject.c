#include "PR/ultratypes.h"
typedef struct Overlay3State { u8 pad000[1]; s8 selector; u8 pad002[0x3B4]; s16 scoreHigh; s16 scoreLow; u8 pad3BA[0x40]; s16 blocked; } Overlay3State;
typedef struct Overlay3Object { u8 pad00[0x0C]; f32 x; f32 y; f32 z; u8 pad18[0x4C]; Overlay3State *state; } Overlay3Object;
typedef struct Overlay3Search { u8 pad000[0x38D]; u8 cachedIndex; u16 timer; u8 pad390[0x18]; u8 weights[1]; } Overlay3Search;
extern Overlay3Object **overlay3GetSearchObjectsReloc(s32 *count);
extern s32 overlay3ContainsValueReloc(Overlay3Object *anchor, Overlay3Object *object);
extern s32 overlay3RandomRangeReloc(s32 low, s32 high);
extern f32 overlay3SqrtReloc(f32 value);
/*
 * Plateau, remeasured 2026-09-12: 21 relocation-masked words at the exact
 * 118-word extent and 0x80 frame, all of them in the entry and cached-path
 * block; every instruction from +0xA0 on is byte-exact. The residual is one
 * allocator shape, stated and priced in the handoff below: the ROM copies the
 * helper's return register into $a1 before the guard so $v0 is free for both
 * reloads of `count`, and neither a colour force nor a split force on this
 * candidate's single `objects` web reproduces that.
 */
#ifdef NON_MATCHING
Overlay3Object *overlay3SelectScoredObject(Overlay3Object *anchor, Overlay3Search *search, s32 elapsed) {
    s32 count; Overlay3Object **objects; Overlay3Object **cursor; Overlay3Object *result;
    Overlay3Object *object; Overlay3State *state; s32 index; s32 bestScore;
    s32 bestIndex; s32 score; f32 dx; f32 dz; u16 timer; u8 cachedIndex;
    objects = overlay3GetSearchObjectsReloc(&count);
    result = 0;
    if ((elapsed < search->timer) && ((cachedIndex = search->cachedIndex) != 0x7F)) {
        search->timer = search->timer - elapsed;
        result = objects[cachedIndex];
    } else {
        bestScore = -1000000;
        index = count - 1;
        if (count != 0) {
            cursor = &objects[index];
            do {
                object = *cursor;
                if ((object != anchor) && (object->state->blocked == 0) &&
                    (overlay3ContainsValueReloc(anchor, object) == 0)) {
                    score = overlay3RandomRangeReloc(0, 2000);
                    state = object->state;
                    score += (state->scoreHigh - state->scoreLow) * 1000;
                    dx = anchor->x - object->x;
                    dz = anchor->z - object->z;
                    score -= ((s32)overlay3SqrtReloc((dx * dx) + (dz * dz))) / 8;
                    score += search->weights[state->selector] << 8;
                    if (bestScore < score) {
                        bestScore = score;
                        bestIndex = index;
                        result = object;
                    }
                }
                cursor--;
            } while (index--);
        }
        if (result != 0) {
            search->cachedIndex = bestIndex;
            search->timer = overlay3RandomRangeReloc(3, 6) * 60;
        }
    }
    return result;
}

#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o003/overlay3SelectScoredObject/func_overlay_003_F00003B0_185A0E0.s")
#endif

/* PLATEAU-HANDOFF:overlay3SelectScoredObject:start
 * symbol: overlay3SelectScoredObject
 * score: 21/118 words
 * frame: 0x80
 * relocations: 5
 * first-mismatch: +0x38
 * summary: The 21 words are one block, the entry and cached path; everything from +0xA0 on is byte-exact. The ROM copies the helper's return register into $a1 before the guard, so the cached path keeps using the raw $v0 result while the loop path reads $a1, which frees $v0 for both reloads of count and lets the index come straight off it with addiu s3,v0,-1; this candidate leaves objects in $v0 for both paths, puts count in $a0 and pays an extra move s3,v1. Records, instrumented object cmp-identical to the configured one: objects is web 114, save 20, nocs 1, totalsave 20, bestcost 0, forbidden mask empty, so it takes c1 v0 unopposed; the two count reloads are webs 17 and 36 at save 1.5 and 1.0 and take c3 a0. Two forces, both recorded accepted, prove the ROM's shape is neither a colour nor a split of that one web: forcing w114 to c4 gives 78 at delta -4 because it removes the move s3,v1 without adding the copy, and forcing the split path gives 79 at delta +4. The ROM has BOTH the copy and count in v0, so uopt kept the call result as its own range and gave the loop use a caller-saved copy. A source-level second pointer local is not the way in: assigned after the call, at the top of the else, or used for the cached path instead, all cost 8 bytes of frame at 0x88 and score 30, and so does replacing the unused u16 timer with it. A two-step cursor and a pointer-plus-index cursor are byte-identical at 21. Decision variable: what makes uopt copy a helper return register into a caller-saved argument register while leaving an earlier-block use on the raw result. Confound to avoid: a 240-point declaration sweep read 27 to 28 everywhere, but it also re-packed the declarations one per line while the retained body packs several, so it measures L59 packing as well as order and is not a clean negative.
 * PLATEAU-HANDOFF:overlay3SelectScoredObject:end
 */
