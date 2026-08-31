#include "PR/ultratypes.h"

s32 gOverlay14InputFlag0;
s32 gOverlay14InputFlag4;
s32 gOverlay14InputFlag8;
s32 gOverlay14PulseC;
s32 gOverlay14Pulse10;
s32 gOverlay14Pulse14;
s32 gOverlay14Pulse18;
s32 gOverlay14Timer1C;
s32 gOverlay14Timer20;

extern u16 joyGetPressed(s32 player);
extern s8 joyGetStickX(s32 player);
extern s8 joyGetStickY(s32 player);

/* Plateau (batch 21): exact 0x20C and frame 0x20; 65 words remain, first +0xC.
 * The nine BSS definitions reproduce offsets 0x0..0x20. A diagnostic surface
 * synthesized from this candidate authenticates all 29 relocation identities;
 * the guarded fallback remains fail-closed at 3/29 until the C can be promoted.
 * Generic D-name spellings were flat and direct D definitions collided globally.
 * Batch 20 exhausted branch, pointer, axis, call, load, flag, and permuter forms;
 * allocator lifetime/UOPT tracing is the next untried code-generation lever. */
#ifdef NON_MATCHING
void overlay14PrepareInputState(s32 step) {
    s32 first;
    s32 second;
    s32 vertical;
    s32 verticalPositive;
    s32 verticalNegative;
    s32 secondPositive;
    s32 secondNegative;

    first = joyGetPressed(0);
    second = joyGetStickX(0);
    vertical = joyGetStickY(0);

    gOverlay14InputFlag0 = first & 0x8000;
    gOverlay14InputFlag4 = first & 0x4000;
    gOverlay14InputFlag8 = first & 0x1000;
    gOverlay14PulseC = 0;
    gOverlay14Pulse10 = 0;
    gOverlay14Pulse14 = 0;
    gOverlay14Pulse18 = 0;

    if ((vertical >= -0x1E) && (vertical < 0x1F)) {
        gOverlay14Timer20 = 0;
    } else if (vertical >= 0x1F) {
        if (gOverlay14Timer20 > 0) {
            verticalPositive = gOverlay14Timer20 - step;
            gOverlay14Timer20 = verticalPositive;
            if (verticalPositive <= 0) {
                gOverlay14Timer20 = 0xA;
                gOverlay14PulseC = 1;
            }
        } else {
            gOverlay14Timer20 = 0x14;
            gOverlay14PulseC = 1;
        }
    } else if (vertical < -0x1E) {
        if (gOverlay14Timer20 < 0) {
            verticalNegative = gOverlay14Timer20 + step;
            gOverlay14Timer20 = verticalNegative;
            if (verticalNegative >= 0) {
                gOverlay14Timer20 = -0xA;
                gOverlay14Pulse10 = 1;
            }
        } else {
            gOverlay14Timer20 = -0x14;
            gOverlay14Pulse10 = 1;
        }
    }

    if ((second >= -0x1D) && (second < -0x1E)) {
        gOverlay14Timer1C = 0;
        return;
    }
    if (second >= 0x1F) {
        secondPositive = gOverlay14Timer1C - step;
        if (gOverlay14Timer1C > 0) {
            gOverlay14Timer1C = secondPositive;
            if (secondPositive <= 0) {
                gOverlay14Timer1C = 0xA;
                gOverlay14Pulse18 = 1;
            }
        } else {
            gOverlay14Timer1C = 0x14;
            gOverlay14Pulse18 = 1;
        }
    } else if (second < -0x1E) {
        secondNegative = gOverlay14Timer1C + step;
        if (gOverlay14Timer1C < 0) {
            gOverlay14Timer1C = secondNegative;
            if (secondNegative <= 0) {
                gOverlay14Timer1C = -0xA;
                gOverlay14Pulse14 = 1;
            }
        } else {
            gOverlay14Timer1C = -0x14;
            gOverlay14Pulse14 = 1;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o014/overlay14PrepareInputState/func_overlay_014_F0000B5C_1870434.s")
#endif

/* PLATEAU-HANDOFF:overlay14PrepareInputState:start
 * symbol: overlay14PrepareInputState
 * score: 66/131 words
 * frame: 0x20
 * relocations: 29
 * first-mismatch: +0xC
 * summary: Nine owned BSS definitions enable a 29/29 diagnostic identity proof; 65 allocator words remain, with UOPT lifetime tracing as the next lever.
 * PLATEAU-HANDOFF:overlay14PrepareInputState:end
 */
