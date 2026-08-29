#include "PR/ultratypes.h"

typedef struct Overlay41StepRecord {
    u8 id;
    u8 residual;
    s16 remaining;
    s32 x;
    s32 dx;
    s32 y;
    s32 dy;
} Overlay41StepRecord;

extern Overlay41StepRecord gOverlay41StepRecords[8];
extern void overlay41EmitStep(s32 id, s32 x, s32 y);

/* Exact-candidate replay: moving the remaining decrement after both position
 * accumulations is the sole semantic source change from the retained 47/73
 * baseline. No candidate object survives in this worktree, so the guard and
 * assembly fallback remain canonical pending fresh 73-word/0x30-frame proof,
 * the D_800D6B58 pair at +0x14/+0x28, resident func_8000D16C at +0xD0,
 * linked module, and full-bin equality. Pinned DKR/JFG scans are negative. */
#ifdef NON_MATCHING
void func_overlay_041_F0000000_1887338(s32 amount) {
    Overlay41StepRecord *record;
    s32 i;
    s32 x;
    s32 y;
    s32 step;

    record = gOverlay41StepRecords;
    i = 7;
    do {
        if (record->id != 0xFF) {
            step = record->remaining;
            if (step != 0) {
                if (amount < step) {
                    step = amount;
                }
                record->x += record->dx * step;
                record->y += record->dy * step;
                record->remaining -= step;
            }
            x = ((record->x * amount) >> 8) + (record->residual & 0xF);
            y = ((record->y * amount) >> 8) + (record->residual >> 4);
            overlay41EmitStep(record->id, x >> 3, y >> 3);
            record->residual = x & 7;
            record->residual |= (y & 7) << 4;
        }
        record++;
    } while (i--);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o041/overlay41AdvanceStepRecords/func_overlay_041_F0000000_1887338.s")
#endif
