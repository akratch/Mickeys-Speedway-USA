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
 * byte donor. A retained function-specific structural scan found no credible
 * near donor (best similarity 0.167).
 * Fresh current-alias V0 is exact-sized at 49/56 normalized words (seven
 * masked and eleven raw differences), frame 0x30. The target runtime surface
 * authenticates 18 records. The candidate emits 18 static records and 17/18
 * offset/type sites align because the four initial HI16 materializations are
 * shifted by one instruction. Static resolution proves only 1/18 candidate
 * identities (17 unresolved; linked-runtime alignment zero), so candidate
 * identity proof remains pending. Everything from +0x40 onward is
 * instruction-identical after relocation normalization.
 * The complete 119-configuration flag lattice tied V0. A fidelity-gated IDO
 * allocator trace found seven integer webs (v0 twice and s0..s4) and localized
 * the residue to the saved-register/address-materialization schedule. An
 * explicit command-type carrier compiled identically; an explicit D_EC pointer
 * lifetime regressed to 55 words and frame 0x28. The two permitted natural
 * probes are exhausted, so do not reopen without a new source-authentic
 * scheduling mechanism. The owned +0x498..+0x578 / ROM
 * 0x186FD70..0x186FE50 range has no padding or export and has five local inbound
 * JUMPs. */
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

/* PLATEAU-HANDOFF:overlay14ResetMode:start
 * symbol: overlay14ResetMode
 * score: 49/56 words
 * frame: 0x30
 * relocations: 18
 * first-mismatch: +0x14
 * summary: Fresh current-alias V0 has 7 masked/11 raw; flag lattice tied; trace proves schedule residue; candidate identity proof remains pending.
 * PLATEAU-HANDOFF:overlay14ResetMode:end
 */
