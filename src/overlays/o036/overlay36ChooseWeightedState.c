#include "PR/ultratypes.h"

typedef struct Overlay36Inner {
    s8 selector;
    u8 pad001[0x19A];
    u8 countdown;
    u8 pad19C[0x1E9];
    u8 strength;
} Overlay36Inner;

typedef struct Overlay36Object {
    u8 pad000[0x64];
    Overlay36Inner *inner;
} Overlay36Object;

typedef struct Overlay36Choice {
    u8 pad0[3];
    u8 value;
} Overlay36Choice;

typedef struct Overlay36Node {
    u8 pad0[4];
    s16 value4;
    s16 flags6;
} Overlay36Node;

typedef struct Overlay36AltTable {
    u8 pad0[0xA8];
    u8 values[1];
} Overlay36AltTable;

typedef struct Overlay36ScaleDivisorData {
    u8 pad0[0x28];
    f32 value;
} Overlay36ScaleDivisorData;

typedef struct Overlay36ScaleMultiplierData {
    u8 pad0[0x2C];
    f32 value;
} Overlay36ScaleMultiplierData;

typedef struct Overlay36StrengthMultiplierData {
    u8 pad0[0x30];
    f32 value;
} Overlay36StrengthMultiplierData;

typedef struct Overlay36NodeDataA {
    u8 pad0[0x150];
    Overlay36Node *node;
} Overlay36NodeDataA;

typedef struct Overlay36NodeDataB {
    u8 pad0[0x14C];
    Overlay36Node *node;
} Overlay36NodeDataB;

extern s32 gOverlay36Mode;
extern u8 gOverlay36AdjustEnabled;
extern u16 gOverlay36EnabledMask;
extern u8 gOverlay36Weights[14][10];
extern Overlay36AltTable gOverlay36AltTable;
extern Overlay36ScaleDivisorData gOverlay36ScaleDivisor;
extern Overlay36ScaleMultiplierData gOverlay36ScaleMultiplier;
extern Overlay36StrengthMultiplierData gOverlay36StrengthMultiplier;
extern Overlay36NodeDataA gOverlay36NodeA;
extern Overlay36NodeDataB gOverlay36NodeB;

extern u32 overlay36ChooseReloc();
extern f32 overlay36MeasureReloc(Overlay36Object *object, s32 mode);
extern Overlay36Choice *overlay36GetChoiceReloc(s8 selector, s32 arg1);
extern void func_overlay_036_F0000914_1883DCC(Overlay36Object *object,
                                              s32 arg1, s32 state,
                                              s32 enabled);

/* Fresh configured V0 remains exact-sized at 170 words with the exact 0x38
 * frame, 99/170 positional words, and first mismatch +0x3C. The 33-record
 * candidate and target relocation surfaces retain only 18 offset/type
 * alignments and one resolved identity. A later exact independent-FP-term
 * rotation and explicit-dereference precedent justified three bounded forms:
 * rotating the initial value terms was byte-flat, explicit (*inner).strength
 * emitted 169 words and regressed to 138 residuals, and combining both was
 * byte-flat. Preserve V0 pending new mode-branch staging or FP-pool evidence;
 * GLOBAL_ASM stays canonical. */
#ifdef NON_MATCHING
void func_overlay_036_F0000A60_1883F18(Overlay36Object *object, s32 arg1,
                                       volatile s32 arg2,
                                       volatile s32 arg3) {
    Overlay36Inner *inner;
    s32 total;
    s32 state;
    s32 position;
    s32 i;
    f32 value;
    f32 blend;

    inner = object->inner;
    total = 0;
    if (inner->countdown == 0) {
        if (gOverlay36Mode == 3) {
            state = gOverlay36AltTable.values[overlay36ChooseReloc(0, 5)];
        } else {
            value = ((overlay36MeasureReloc(object, 5) /
                      gOverlay36ScaleDivisor.value) *
                         gOverlay36ScaleMultiplier.value) +
                    ((f32)inner->strength * gOverlay36StrengthMultiplier.value);

            if (gOverlay36AdjustEnabled != 0) {
                Overlay36Choice *choice;

                choice = overlay36GetChoiceReloc(inner->selector, 0);
                if (choice->value >= 0x21) {
                    blend = 32.0f;
                } else {
                    blend = (f32)choice->value;
                }
                blend *= 0.015625f;
                value = ((1.0f - blend) * value) + (blend * 10.0f);
            }

            state = -1;
            position = (s32)value;
            if (position >= 10) {
                position = 9;
            }

            i = 13;
            do {
                if (gOverlay36EnabledMask & (1 << i)) {
                    state = i;
                    total += gOverlay36Weights[i][position];
                }
            } while (i--);

            if (total >= 2) {
                state = overlay36ChooseReloc(1, total, state, position);
                i = 13;
                do {
                    if (gOverlay36EnabledMask & (1 << i)) {
                        state -= gOverlay36Weights[i][position];
                        if (state <= 0) {
                            state = i;
                            break;
                        }
                    }
                } while (i--);
            }

            if (state == -1) {
                state = 9;
            }
        }
        func_overlay_036_F0000914_1883DCC(object, arg1, state, 1);
    }

    if (gOverlay36Mode == 3) {
        gOverlay36NodeA.node->value4 = 0xF0;
    } else {
        gOverlay36NodeA.node->value4 = 0x3C;
    }
    gOverlay36NodeB.node->flags6 |= 0x400;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o036/overlay36ChooseWeightedState/func_overlay_036_F0000A60_1883F18.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_036_F0000A60_1883F18:start
 * symbol: func_overlay_036_F0000A60_1883F18
 * score: 99/170 words
 * frame: 0x38
 * relocations: 33
 * first-mismatch: +0x3C
 * summary: New FP-term rotation and its explicit-deref combination are flat; explicit deref alone regresses to 169 words/138 residuals; 18/33 sites align.
 * PLATEAU-HANDOFF:func_overlay_036_F0000A60_1883F18:end
 */
