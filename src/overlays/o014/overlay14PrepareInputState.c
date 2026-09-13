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

/* Tier A source reconstruction: update and retest each timer directly.
 * No intervening call or volatile access can change the timer. */
void overlay14PrepareInputState(s32 step) {
    s32 first;
    s32 second;
    s32 vertical;

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
            gOverlay14Timer20 -= step;
            if (gOverlay14Timer20 <= 0) {
                gOverlay14Timer20 = 0xA;
                gOverlay14PulseC = 1;
            }
        } else {
            gOverlay14Timer20 = 0x14;
            gOverlay14PulseC = 1;
        }
    } else if (vertical < -0x1E) {
        if (gOverlay14Timer20 < 0) {
            gOverlay14Timer20 += step;
            if (gOverlay14Timer20 >= 0) {
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
        if (gOverlay14Timer1C > 0) {
            gOverlay14Timer1C -= step;
            if (gOverlay14Timer1C <= 0) {
                gOverlay14Timer1C = 0xA;
                gOverlay14Pulse18 = 1;
            }
        } else {
            gOverlay14Timer1C = 0x14;
            gOverlay14Pulse18 = 1;
        }
    } else if (second < -0x1E) {
        if (gOverlay14Timer1C < 0) {
            gOverlay14Timer1C += step;
            if (gOverlay14Timer1C <= 0) {
                gOverlay14Timer1C = -0xA;
                gOverlay14Pulse14 = 1;
            }
        } else {
            gOverlay14Timer1C = -0x14;
            gOverlay14Pulse14 = 1;
        }
    }
}
