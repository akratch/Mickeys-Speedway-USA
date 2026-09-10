#include "PR/ultratypes.h"

typedef struct Overlay57ModeObject {
    s32 value;
} Overlay57ModeObject;

typedef struct Overlay57ModeResult {
    u8 pad00[0x16];
    u8 flags16;
} Overlay57ModeResult;

extern s32 gOverlay57Countdown;
extern s32 gOverlay57Timer;
extern s32 gOverlay57ModeFlag;
extern s32 gOverlay57SetupValues[6];
extern Overlay57ModeObject gOverlay57Object;
extern u8 gOverlay57ObjectId;
extern s32 gOverlay57SetupStatus;
extern s32 gOverlay57SetupDelay;
extern s32 gOverlay57TriggerLatched;
extern s32 gOverlay57ObjectStatus;

u8 gOverlay57BssPad000[0x114];
s32 gOverlay57Countdown;
s32 gOverlay57Timer;
u8 gOverlay57BssPad11C[0x20];
s32 gOverlay57SetupStatus;
s32 gOverlay57SetupDelay;
s32 gOverlay57ModeFlag;
u8 gOverlay57BssPad148[0x20];
s32 gOverlay57SetupValues[6];
Overlay57ModeObject gOverlay57Object;
s32 gOverlay57BssPad184;
s32 gOverlay57TriggerLatched;
s32 gOverlay57BssPad18C[2];
s32 gOverlay57ObjectStatus;
u8 gOverlay57BssPad198[0x20];

/* Physical a0 retains updateRate at this site; consumption is not proven. */
extern void *overlay57TailQueryModeReloc(void);
extern s32 overlay57TailQueryChoiceReloc(s32 mode);
extern void overlay57TailPrepareObjectReloc(u8 id);
extern void overlay57TailStartObjectReloc(u8 id);
extern Overlay57ModeResult *overlay57TailFindObjectReloc(u8 id);
extern void overlay57SetNodeValue(s32 id, s32 argument, f32 value);
extern void overlay57AdvanceReloc(s32 updateRate);

/* Exact under the canonical -O2 -mips2 flags: 0 masked words of 94.
 *
 * The setup-array block is one six-iteration constant loop. uopt unrolls it
 * by four, so it peels 6 % 4 = 2 iterations -- those fold to the literal
 * stores of 0 and 0x30 -- and emits a single unrolled body whose induction
 * variable stays register-resident at 2 rather than being propagated. A
 * straight-line reconstruction of the six stores cannot reach that shape.
 * The body is written on the `for` line because as1 breaks scheduling ties
 * on the source line, and a braced body puts the unrolled copies on a
 * different line than the peeled ones.
 *
 * framePad is declared before `trigger` because the declaration order fixes
 * which frame cells the two spill homes take; declared after, the frame
 * closes at 0x20 instead of the target's 0x28. */
#ifdef NON_MATCHING
void overlay57UpdateModeTrigger(s32 updateRate) {
    s32 framePad[2];
    s32 trigger;

    gOverlay57ModeFlag = 1;

    if (gOverlay57Countdown > 0) {
        gOverlay57Countdown = gOverlay57Countdown - updateRate;
        if (gOverlay57Countdown <= 0) {
            trigger = 1;
        }
    }

    gOverlay57SetupStatus = 0;
    if (overlay57TailQueryModeReloc() == 0) {
        s32 index;
        s32 i;
        Overlay57ModeResult *result;

        index = 2;

        for (i = 0; i < 6; i++) gOverlay57SetupValues[i] = i * 0x30;

        trigger = 1;
        if (overlay57TailQueryChoiceReloc(index) == 5) {
            gOverlay57Timer = 10;
        }

        gOverlay57ObjectStatus = 0;
        gOverlay57Object.value = 80;
        overlay57TailPrepareObjectReloc(((u8 *)&gOverlay57Object)[3]);
        overlay57TailStartObjectReloc(((u8 *)&gOverlay57Object)[3]);
        result = overlay57TailFindObjectReloc(((u8 *)&gOverlay57Object)[3]);
        if (result != 0) {
            result->flags16 |= 2;
        }

        if (gOverlay57TriggerLatched != 0) {
            gOverlay57SetupDelay = 0;
        } else {
            gOverlay57SetupDelay = 60;
        }
    }

    /* Retail reads this deliberately uninitialized value when neither path
     * above assigns it. Initializing it changes the physical function. */
    if ((trigger != 0) && (gOverlay57TriggerLatched == 0)) {
        overlay57SetNodeValue(0x2F, 0, 0.012f);
        gOverlay57TriggerLatched = 1;
    }

    overlay57AdvanceReloc(updateRate);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o057/overlay57UpdateModeTrigger/func_overlay_057_F0004C18_18A8810.s")
#endif

/* PLATEAU-HANDOFF:overlay57UpdateModeTrigger:start
 * symbol: overlay57UpdateModeTrigger
 * score: 0/94 words
 * frame: 0x28
 * relocations: 38
 * first-mismatch: none
 * summary: Exact: uopt peels two iterations of the six-trip constant loop and leaves the unrolled body's induction variable register-resident; the one-line loop statement fixes as1 tie-breaking and a leading pad array restores the 0x28 frame.
 * PLATEAU-HANDOFF:overlay57UpdateModeTrigger:end
 */
