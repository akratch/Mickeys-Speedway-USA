#include "PR/ultratypes.h"

typedef struct O57MiddleRenderItem {
    s32 type;
    u8 value04;
    u8 value05;
    u8 value06;
    u8 value07;
} O57MiddleRenderItem;

typedef struct O57MiddleInfo {
    u8 pad00[8];
    u8 value08;
    u8 value09;
    u8 value0A;
} O57MiddleInfo;

typedef struct O57MiddleChoice {
    u8 pad00[0x28];
    s16 tableIndex;
    s8 active;
    u8 pad2B[9];
} O57MiddleChoice;

typedef struct O57MiddleOutput {
    s32 value0;
    u8 controller;
    u8 pad05[0x23];
} O57MiddleOutput;

typedef union O57MiddleInput {
    s32 word;
    u32 uword;
    s16 half;
    u8 bytes[0x520];
} O57MiddleInput;

typedef union O57MiddleFloatBits {
    f32 value;
    s32 bits;
} O57MiddleFloatBits;

extern O57MiddleInput D_0;
extern void *D_8[];
extern void *D_80[];
extern s32 D_104;
extern f32 D_10C;
extern f32 D_110;
extern s32 D_11C;
extern s32 D_118;
extern s32 D_128;
extern s32 D_130;
extern s32 D_134;
extern s32 D_138;
extern s32 D_13C;
extern s32 D_140;
extern s32 D_144;
extern s32 D_148;
extern s32 D_160;
extern s32 D_164;
extern s32 D_188;
extern s32 D_194;
extern s32 D_198;
extern s32 D_19C;
extern O57MiddleInfo D_1A8;
extern s32 D_1A0[];
extern s32 D_17C[];
extern O57MiddleOutput D_1B8[];
extern s32 D_4FC;
extern s32 D_50C;
extern s16 D_528[];
extern s16 D_53E[];
extern u8 D_4C[];
extern u8 D_58[];
extern u8 D_25C[];
extern u8 D_32C[];
extern s16 D_36C[];
extern u8 D_D0[];
extern u8 D_183;

extern s32 func_overlay_057_F0000000_18A3BF8();
extern void func_overlay_057_F0001020_18A4C18(s32 updateRate);
extern void func_overlay_057_F00067DC_18AA3D4();

/* Fresh workbench: structure-mismatch, 1169 differing words, first mismatch
 * +0x34; 1217 versus 1208 instructions with the exact 0x140 frame. Naming
 * the distinct D_198 and D_19C globals improves the prior 1187-word result.
 * All 119 flag rows are nonexact; the 1160-word flag minimum is 21
 * instructions short. Promotion stops at schedule divergence for D_183 and
 * D_188, and the configured candidate exceeds its owner by 36 bytes, so the
 * runtime relocation comparison correctly refuses it. */
#ifdef NON_MATCHING
void func_overlay_057_F0004E18_18A8A10(s32 updateRate) {
    s32 i;
    s32 index;
    s32 limit;
    s16 input;
    s32 remainder;
    s32 previousGroup;
    s32 currentGroup;
    s32 oldValue;
    s32 value;
    s32 nextValue;
    s32 cursorValue;
    s32 outputIndex;
    s32 state;
    s32 valueA;
    s32 valueB;
    s32 valueC;
    O57MiddleFloatBits captionBits;
    s8 sourceState[0x28];
    u8 activePlayers[10];
    s32 stackC8;
    s16 stackC6;
    s16 stackC4;
    s32 stackC0;
    s32 stackBC;
    s32 stackB8;
    u8 stackB0[2];
    u8 renderState;
    s32 stack80;
    s32 stack7C;
    s32 stack78;
    s32 stack64;
    s32 stack5C;
    O57MiddleRenderItem *renderItems;
    O57MiddleRenderItem *renderItem;
    O57MiddleChoice *choice;
    s16 *color;
    u8 *palette;
    u8 *active;
    s8 *source;
    s8 *activeInit;
    O57MiddleOutput *output;
    s32 *list;
    s32 activeCount;
    s8 rank;
    s8 choiceActive;
    s32 row;

    if (func_overlay_057_F0000000_18A3BF8() == 0) {
        if (D_11C != 7) {
            D_11C = 2;
        }

        limit = 0x14;
        if ((D_0.uword << 13) >> 31) {
            state = 3;
        } else {
            limit = 0x13;
            if (D_19C != 0) {
                state = 3;
            } else {
                state = 1;
                limit = 0xB;
                if (D_198 != 0) {
                    limit = 0xF;
                    state = 2;
                }
            }
        }

        if (D_194 == 0) {
            index = D_160;
            input = D_0.half;
            remainder = index / 6;
            if ((input < -16) && (index > 0) && (D_50C == 0)) {
                D_130 = 1;
                func_overlay_057_F0000000_18A3BF8(
                    D_80[index], 0xA0, 0x104, 0x104);
                D_164 = D_160;
                value = D_160 - 1;
                D_160 = value;
                func_overlay_057_F0000000_18A3BF8(
                    D_80[value], -0xA0, 0xBE, 4);
                *(s32 *) (void *)&D_10C = D_104;
                D_104 = 0xFF;
                if ((D_160 % 6) == 5) {
                    D_13C = 0;
                    D_140 = 0x5A;
                    func_overlay_057_F00067DC_18AA3D4(
                        0x2F, (D_160 / 6) + 5, 0x3C449BA6);
                }
            } else if ((input >= 17) && (index < limit) &&
                       (D_50C == 0)) {
                D_130 = 1;
                func_overlay_057_F0000000_18A3BF8(
                    D_80[index], 0xA0, 0x104, 0x104);
                D_164 = D_160;
                value = D_160 + 1;
                D_160 = value;
                func_overlay_057_F0000000_18A3BF8(
                    D_80[value], 0x1E0, 0xBE, 4);
                *(s32 *) (void *)&D_10C = D_104;
                D_104 = 0xFF;
                if ((D_160 % 6) == 0) {
                    D_13C = 0;
                    D_140 = 0x3C;
                    func_overlay_057_F00067DC_18AA3D4(
                        0x2F, D_160 / 6, 0x3C449BA6);
                }
            } else if ((input < -16) && (index < (limit - 2)) &&
                       (D_50C == 0)) {
                D_130 = 1;
                func_overlay_057_F0000000_18A3BF8(
                    D_80[index], 0xA0, 0x104, 0x104);
                D_164 = D_160;
                value = D_160 + 3;
                previousGroup = D_160 / 6;
                D_160 = value;
                func_overlay_057_F0000000_18A3BF8(
                    D_80[value], 0xA0, 0x104, 4);
                *(s32 *) (void *)&D_10C = D_104;
                D_104 = 0xFF;
                currentGroup = D_160 / 6;
                if (previousGroup != currentGroup) {
                    D_13C = 0;
                    D_140 = 0x3C;
                    func_overlay_057_F00067DC_18AA3D4(
                        0x2F, currentGroup, 0x3C449BA6, currentGroup);
                }
            } else {
                if ((input >= 17) && (index >= 3) &&
                    (D_50C == 0)) {
                    D_130 = 1;
                    func_overlay_057_F0000000_18A3BF8(
                        D_80[index], 0xA0, 0x104, 0x104);
                    D_164 = D_160;
                    value = D_160 - 3;
                    previousGroup = D_160 / 6;
                    D_160 = value;
                    func_overlay_057_F0000000_18A3BF8(
                        D_80[value], 0xA0, 0x104, 4);
                    *(s32 *) (void *)&D_10C = D_104;
                    D_104 = 0xFF;
                    currentGroup = D_160 / 6;
                    if (previousGroup != currentGroup) {
                        D_13C = 0;
                        D_140 = 0x3C;
                        func_overlay_057_F00067DC_18AA3D4(
                            0x2F, currentGroup + 5, 0x3C449BA6,
                            currentGroup);
                    }
                } else if ((D_0.word & 0x2020) && (index >= 6) &&
                           (D_50C == 0)) {
                D_130 = 1;
                func_overlay_057_F0000000_18A3BF8(
                    D_80[index], 0xA0, 0x104, 0x104);
                oldValue = D_160;
                D_164 = oldValue;
                D_160 = oldValue - 6;
                func_overlay_057_F0000000_18A3BF8(
                    D_80[D_160], -0xA0, 0xBE, 4);
                *(s32 *) (void *)&D_10C = D_104;
                D_104 = 0xFF;
                D_13C = 0;
                D_140 = 0x5A;
                func_overlay_057_F00067DC_18AA3D4(
                    0x2F, (D_160 / 6) + 5, 0x3C449BA6);
                } else if ((D_0.word & 0x10) && (remainder < state) &&
                           (D_50C == 0)) {
                D_130 = 1;
                func_overlay_057_F0000000_18A3BF8(
                    D_80[index], 0xA0, 0x104, 0x104);
                oldValue = D_160;
                D_164 = oldValue;
                D_160 = oldValue + 6;
                func_overlay_057_F0000000_18A3BF8(
                    D_80[D_160], 0x1E0, 0xBE, 4);
                *(s32 *) (void *)&D_10C = D_104;
                D_104 = 0xFF;
                D_13C = 0;
                D_140 = 0x5A;
                func_overlay_057_F00067DC_18AA3D4(
                    0x2F, D_160 / 6, 0x3C449BA6);
                if (D_160 > state) {
                    D_160 = state;
                }
                }
            }

            if ((D_0.word & 0x4000) && (D_50C == 0) &&
                (((s32 *) (void *)&D_0)[0x4CC / 4] != 0)) {
                func_overlay_057_F0000000_18A3BF8(0xD, 0);
                func_overlay_057_F0000000_18A3BF8();
                func_overlay_057_F0000000_18A3BF8(D_183);
                D_118 = 7;
                func_overlay_057_F00067DC_18AA3D4(
                    0x2F, 0, 0xBC449BA6);
                D_188 = 0;
                D_11C = 0;
                func_overlay_057_F0000000_18A3BF8(1);

                list = &D_134;
                index = *list;
                if (index != -1) {
                    do {
                    func_overlay_057_F0000000_18A3BF8(index & 0xFF);
                    currentGroup = *list;
                    func_overlay_057_F00067DC_18AA3D4(
                        currentGroup, D_17C[currentGroup], 0x3BE56042);
                    index = list[1];
                    list++;
                    } while (index != -1);
                }
                list = D_1A0;
                index = *list;
                if (index != -1) {
                    do {
                        func_overlay_057_F0000000_18A3BF8(index & 0xFF);
                        index = list[1];
                        list++;
                    } while (index != -1);
                }
            }

            i = 0;
            remainder = updateRate & 3;
            if (updateRate > 0) {
                if (remainder != 0) {
                    do {
                        i++;
                        D_4FC += (-0x1400 - D_4FC) >> 2;
                    } while (i != remainder);
                }
                while (i < updateRate) {
                    i += 4;
                    value = D_4FC + ((-0x1400 - D_4FC) >> 2);
                    nextValue = value + ((-0x1400 - value) >> 2);
                    cursorValue = nextValue + ((-0x1400 - nextValue) >> 2);
                    D_4FC = value;
                    D_4FC = nextValue;
                    D_4FC = cursorValue;
                    D_4FC = cursorValue +
                            ((-0x1400 - cursorValue) >> 2);
                }
            }
        } else if (D_194 == 1) {
            i = 0;
            remainder = updateRate & 3;
            if (updateRate > 0) {
                if (remainder != 0) {
                    do {
                        i++;
                        D_4FC += (-D_4FC) >> 2;
                    } while (i != remainder);
                }
                while (i < updateRate) {
                    i += 4;
                    value = D_4FC + ((-D_4FC) >> 2);
                    nextValue = value + ((-value) >> 2);
                    cursorValue = nextValue + ((-nextValue) >> 2);
                    D_4FC = value;
                    D_4FC = nextValue;
                    D_4FC = cursorValue;
                    D_4FC = cursorValue + ((-cursorValue) >> 2);
                }
            }
            if ((D_0.word & 0x4000) && (D_50C == 0)) {
                func_overlay_057_F0000000_18A3BF8(0xD, 0);
                D_194 = 0;
                D_11C = 2;
            }
        }

        func_overlay_057_F0001020_18A4C18(updateRate);
        if ((D_4FC >> 4) >= -0x135) {
            row = 0x51;
            renderItems = (O57MiddleRenderItem *)
                ((u8 *) func_overlay_057_F0000000_18A3BF8() +
                 ((s32) func_overlay_057_F0000000_18A3BF8(
                      ((s16 *) (void *)&D_0)[D_160]) << 5));
            func_overlay_057_F0000000_18A3BF8(0);
            value = D_4FC >> 4;
            D_10C = (f32) value;
            D_110 = 7.0f;
            func_overlay_057_F0000000_18A3BF8(0);
            func_overlay_057_F0000000_18A3BF8(8);
            func_overlay_057_F0000000_18A3BF8(
                D_1A8.value08, D_1A8.value09, D_1A8.value0A,
                0xFF, 0xFF);
            valueA = value + 0xA0;
            stack64 = valueA;
            func_overlay_057_F0000000_18A3BF8(
                0, valueA, 0x23,
                ((s32 *) (void *)&D_0)[0x510 / 4 + D_160], 4);
            func_overlay_057_F0000000_18A3BF8(0xFF, 0x80, 0, 0xFF, 0xFF);
            func_overlay_057_F0000000_18A3BF8(
                0, valueA, 0x37,
                ((s32 *) (void *)&D_0)[0xD0 / 4], 4);
            func_overlay_057_F0000000_18A3BF8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
            if (func_overlay_057_F0000000_18A3BF8(
                    ((s16 *) (void *)&D_0)[D_160]) != 0) {
                    func_overlay_057_F0000000_18A3BF8(
                        0, D_32C, value + 0x30,
                        0x26, 0xFF, 0xFF, 0xFF, 0xFF);
            }

            renderItem = renderItems;
            stack5C = value + 0x56;
            i = 0;
            do {
                func_overlay_057_F0000000_18A3BF8(
                    renderItem->type, &stack80, &stack7C, &stack78);
                palette = D_4C;
                if (renderItem->type == 0) {
                    valueA = 0x4A;
                    func_overlay_057_F0000000_18A3BF8(
                        &renderState, palette);
                } else {
                    valueB = func_overlay_057_F0000000_18A3BF8(
                                 renderItem->value04, palette) & 0xFF;
                    valueC = func_overlay_057_F0000000_18A3BF8(
                                 renderItem->value05) & 0xFF;
                    func_overlay_057_F0000000_18A3BF8(
                        &renderState, D_58, valueB, valueC,
                        func_overlay_057_F0000000_18A3BF8(
                            renderItem->value06),
                        stack80, stack7C, stack78);
                    valueA = renderItem->value07 + 0x51;
                }
                if (i < 3) {
                    func_overlay_057_F0000000_18A3BF8(
                        0, value + 0x2E, row,
                        ((s32 *) (void *)&D_0)[0x510 / 4 + i], 0);
                }
                stackBC = 0;
                stackC6 = (s16) (row - 4);
                stackC0 = 0;
                stackC8 = 0;
                stackB8 = ((s32 *) (void *)&D_0)[valueA];
                stackC4 = (s16) stack5C;
                func_overlay_057_F0000000_18A3BF8(
                    0, &stackB8, 0, 0,
                    0xFF, 0xFF, 0xFF, 0xFF);
                color = D_528;
                active = &renderState;
                do {
                    stackB0[1] = 0;
                    stackB0[0] = *active;
                    func_overlay_057_F0000000_18A3BF8(
                        0, *color + value, row, stackB0, 0);
                    color++;
                    active++;
                } while (color != D_53E);
                if (i == 2) {
                    row += 0x1B;
                    func_overlay_057_F0000000_18A3BF8(
                        0xFF, 0x80, 0, 0xFF, 0xFF);
                    func_overlay_057_F0000000_18A3BF8(
                        0, (u8) stack64, row,
                        ((s32 *) (void *)&D_0)[0xD4 / 4], 4);
                    func_overlay_057_F0000000_18A3BF8(
                        0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                i++;
                renderItem++;
                row += 0x1B;
                stack5C += 0x1B;
            } while (i != 4);
            captionBits.value = (f32) (value + 0x2D);
            func_overlay_057_F0000000_18A3BF8(
                0, D_25C, captionBits.bits,
                0x43380000, 0x3F800000, 0x3F800000, -2, 3);
        }

        if (D_140 > 0) {
            D_140 -= updateRate;
        } else {
            D_13C += updateRate * 4;
            if (D_13C >= 0x100) {
                D_13C = 0xFF;
            }
        }

        if ((D_0.word & 0x9000) && (D_50C == 0)) {
            func_overlay_057_F0000000_18A3BF8(0xC, 0);
            if (D_0.word >= 2 || D_194 == 1) {
                activeInit = (s8 *) &activePlayers[9];
                i = 9;
                do {
                    *activeInit = 1;
                    activeInit--;
                    i--;
                } while (i >= 0);
                choice = (O57MiddleChoice *) (void *)&D_0;
                source = &sourceState[0];
                outputIndex = 0;
                while ((u8 *) choice < D_D0) {
                    choiceActive = choice->active;
                    *source++ = choiceActive;
                    if (choiceActive != 0) {
                        value = D_36C[choice->tableIndex];
                        D_1B8[outputIndex].controller = (u8) value;
                        activePlayers[value] = 0;
                        outputIndex++;
                    }
                    choice = (O57MiddleChoice *) ((u8 *) choice + 0x34);
                }
                activeCount = D_0.word;
                rank = 0;
                active = activePlayers;
                if (activeCount < 6) {
                    output = &D_1B8[activeCount];
                    do {
                        if (*active == 0) {
                            do {
                                rank++;
                                active++;
                            } while (*active == 0);
                        }
                        active++;
                        output->controller = (u8) rank;
                        output++;
                        rank++;
                    } while (output < &D_1B8[6]);
                }
                func_overlay_057_F0000000_18A3BF8(
                    sourceState, 0x28, activePlayers, D_D0);
                func_overlay_057_F0000000_18A3BF8(0);
                func_overlay_057_F0000000_18A3BF8(D_0.word);
                ((u8 *) (void *)&D_0)[1] = 0;
                if (D_0.word == 1) {
                    D_1B8[0].value0 = 1;
                    if ((((u16 *) (void *)&D_0)[0] & 1) != 0) {
                        ((u8 *) (void *)&D_0)[1] =
                            ((u8 *) (void *)&D_0)[1] & 0xFE;
                        func_overlay_057_F0000000_18A3BF8();
                        func_overlay_057_F0000000_18A3BF8(0x14);
                    }
                } else {
                    D_1B8[0].value0 = 4;
                }
                state = D_0.word;
                if (((state == 2) || (state == 3)) &&
                    ((u8) D_0.word != 0)) {
                    ((u8 *) (void *)&D_0)[0] = (u8) (4 - state);
                    ((u8 *) (void *)&D_0)[1] = 4;
                } else {
                    ((u8 *) (void *)&D_0)[0] = 0;
                    ((u8 *) (void *)&D_0)[1] = (u8) state;
                }
                ((u8 *) (void *)&D_0)[0] = (u8) ((state < 2) ^ 1);
                ((u8 *) (void *)&D_0)[1] = 2;
                ((u16 *) (void *)&D_0)[0] = ((u16 *) (void *)&D_0)[0];
                ((u8 *) (void *)&D_0)[1] = 0;
                if (D_0.word > 0) {
                    func_overlay_057_F0000000_18A3BF8(
                        ((s16 *) (void *)&D_0)[D_160],
                        D_36C[((s16 *) (void *)&D_0)[0x28 / 2]],
                        0, 5, 1, 0);
                    func_overlay_057_F0000000_18A3BF8(0x12, 0, 0, 0xF, 1, 0);
                    func_overlay_057_F0000000_18A3BF8(1);
                } else {
                    func_overlay_057_F0000000_18A3BF8(
                        ((s16 *) (void *)&D_0)[D_160],
                        D_36C[((s16 *) (void *)&D_0)[0x28 / 2]],
                        0, 5, 1, 0);
                }
                D_50C = 1;
                func_overlay_057_F0000000_18A3BF8();
                if (((u8) D_0.word == 0) ||
                    (((u8) D_0.word != 0) && (((u8 *) (void *)&D_0)[1] != 0))) {
                    func_overlay_057_F0000000_18A3BF8(D_0.word);
                    if (((u8 *) (void *)&D_0)[1] != 0) {
                        ((u8 *) (void *)&D_0)[1] = 0;
                    }
                }
            } else {
                D_194 = 1;
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o057/func_overlay_057_F0004E18_18A8A10/func_overlay_057_F0004E18_18A8A10.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_057_F0004E18_18A8A10:start
 * symbol: func_overlay_057_F0004E18_18A8A10
 * score: 1169 differing words
 * frame: 0x140
 * relocations: 304
 * first-mismatch: +0x34
 * summary: Frame-exact 1217/1208; distinct D_198/D_19C globals improve 1187 to 1169. Candidate is +36 bytes; promotion stops at D_183/D_188 schedule divergence.
 * PLATEAU-HANDOFF:func_overlay_057_F0004E18_18A8A10:end
 */
