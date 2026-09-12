#include "PR/ultratypes.h"

extern u16 joyGetPressed_o063Reloc(s32 arg0);
extern void o63TransitionReloc(void);
extern void o63SetModeReloc(s32 mode);
extern void o63SetColorReloc(s32 red, s32 green, s32 blue, s32 alpha, s32 intensity);
extern void o63DrawReloc(void *resource, s32 x, s32 y, void *state, s32 size);

extern u32 *gO63Sequence;
extern u8 gO63PendingStart[];
extern u32 gO63SequenceStart[];
extern void *gO63PendingState;
extern void *gO63ActiveState;
extern s32 gO63SequenceIndex;
extern s32 gO63Timer;
extern s32 gO63TransitionEnabled;
extern u32 gO63ToggleReloc;
extern u8 gO63GraphicReloc[];

/* Pinned DKR v77/v80 and JFG donor scans classify overlay 63 as none. */
void overlay63UpdateSequence(s32 delta) {
    s32 pressedButtons;

    pressedButtons = joyGetPressed_o063Reloc(0);
    if (pressedButtons != 0) {
        if (gO63Sequence == 0) {
            gO63PendingState = gO63PendingStart;
            gO63Sequence = gO63SequenceStart;
            gO63SequenceIndex = 1;
        } else if (pressedButtons == gO63Sequence[gO63SequenceIndex]) {
            gO63SequenceIndex++;
            if (gO63Sequence[gO63SequenceIndex] == 0) {
                gO63ToggleReloc = (gO63ToggleReloc ^ 1) & 1;
                gO63Sequence = 0;
                gO63Timer = 0xB4;
                gO63ActiveState = gO63PendingState;
            }
        } else {
            gO63Sequence = 0;
        }
    }

    if (gO63Timer > 0) {
        gO63Timer -= delta;
        if (gO63Timer <= 0 && gO63TransitionEnabled != 0) {
            o63TransitionReloc();
        }
        o63SetModeReloc(2);
        o63SetColorReloc(0, 0, 0, 0xFF, 0xFF);
        o63DrawReloc(gO63GraphicReloc, 0xA0, 0x3C, gO63ActiveState, 0xC);
        o63SetColorReloc(0, 0xFF, 0xFF, 0xFF, 0xFF);
        o63DrawReloc(gO63GraphicReloc, 0xA1, 0x3D, gO63ActiveState, 0xC);
    }
}
