#include "PR/ultratypes.h"

typedef struct Overlay14Command { s16 pad0; s16 type; s32 value; } Overlay14Command;

extern s32 D_EC;
extern s32 D_E4;
extern s32 D_FC;
extern s32 gOverlay14ValueF8;
extern s32 gOverlay14ValueD8;
extern s32 gOverlay14ValueDC;
extern s32 gOverlay14ValueE0;
extern Overlay14Command D_128[];
extern void overlay14ResetReleaseOwnerReloc(void);
extern s32 func_overlay_014_F0000578_186FE50(s32);

/* PROVENANCE: Mickey-derived; pinned DKR v77/v80 and JFG scans found no exact
 * byte donor. Retained pre-current-alias full-TU and isolated C have identical
 * text at 45/56 raw and 49/56 after relocation normalization, with frame 0x30.
 * Raw sites are
 * +0x14/+0x18/+0x1C/+0x20/+0x24/+0x28/+0x3C/+0x88/+0x90/+0x98/+0xA4;
 * normalizing four output LO16 addends leaves one seven-site prologue schedule:
 * four address materializations and the s3/ra saves plus s3 = 1. Everything
 * from +0x40 onward is instruction-identical. The shipped surface has 18
 * runtime records, not the extracted target object's incomplete ten. Current
 * source annotates +0x54 as overlay14ReleaseOwner (+0x1B54), but fresh compiled
 * pre/postprocess identity proof is pending; +0xB0 is a local JUMP to
 * overlay14MoveCommandCursor (+0x578). Run and retain the 119-row lattice on
 * identity-correct V0, trace once, and try at most three natural s3-lifetime
 * forms; cap at 122 stock builds plus the trace and batch only after a gain. */
#ifdef NON_MATCHING
void overlay14ResetMode(void) {
    Overlay14Command *command;
    do {
        if (D_EC <= 0) return;
        D_EC--;
        if (D_EC <= 0) return;
        overlay14ResetReleaseOwnerReloc();
        command = &D_128[D_EC - 1];
        D_E4 = command->type;
        if (command->type != 1) {
            gOverlay14ValueF8 = command->value;
            gOverlay14ValueD8 = 0;
            gOverlay14ValueDC = 0;
            gOverlay14ValueE0 = 0;
            return;
        }
        D_FC = command->value;
    } while (func_overlay_014_F0000578_186FE50(0) == 0);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o014/overlay14ResetMode/func_overlay_014_F0000498_186FD70.s")
#endif
