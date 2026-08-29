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

/* Workbench: allocation-mismatch, exact 73-word/0x30-frame shape, 26
 * register-only words from +0x54; temp phase +1. Temp-FIFO structure,
 * lifetime, width, and scope probes left the ring topology unchanged.
 * Runtime evidence fixes the +0x14/+0x28 HI16/LO16 pair as D_800D6B58
 * and the +0xD0 call as resident func_8000D16C. The historical filter
 * spec records diagnostics only and is not in production POSTPROCESS.
 * Mickey-only reconstruction: pinned DKR/JFG scans are negative; JFG's
 * animseqUpdateTextureScrollers is a role-only comparison, not a donor.
 * Reproduce once with configured flags, then park pending a new allocator
 * mechanism; assembly fallback stays canonical. */
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
                record->remaining -= step;
                record->x += record->dx * step;
                record->y += record->dy * step;
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
