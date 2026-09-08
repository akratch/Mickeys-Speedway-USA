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

/* Tier B: calls and globals separated by overlay 57 runtime relocations.
 * Reserved data/BSS selectors remain separate from overlay-local storage. */
extern O57MiddleInput gO57MiddleFlags;
extern s16 gO57MiddleHorizontal, gO57MiddleVertical;
extern s32 gO57MiddleButtons, gO57MiddlePlayerCount;
extern s16 gO57MiddleCourseIds[], gO57MiddleCourseNames[];
extern char **gO57MiddleText;
extern void *gO57MiddleDisplayList, *gO57MiddleVertexList;
extern void *gO57MiddleGraphics[];
extern O57MiddleChoice gO57MiddleChoices[];
extern s32 gO57MiddleData31E4, gO57MiddleData31E8, gO57MiddleData31EC;
extern s32 gO57MiddleData31F0, gO57MiddleData31F4;
extern u8 gO57MiddleData3198, gO57MiddleData31AC;
extern s8 gO57MiddleData319C, gO57MiddleData31A4, gO57MiddleData31A8;
extern u16 gO57MiddleData31B4, gO57MiddleData31B8;
extern u8 gO57MiddleData3208, gO57MiddleData3214;
extern void *gO57MiddleItems[];
extern s32 gO57MiddleAlpha, gO57MiddlePreviousAlpha;
extern s32 gO57MiddleMode, gO57MiddleNextMode, gO57MiddleMoving;
extern s32 gO57MiddleFade, gO57MiddleFadeDelay;
extern s32 gO57MiddleSelection, gO57MiddlePreviousSelection;
extern s32 gO57MiddleState188, gO57MiddleState194;
extern s32 gO57MiddleUnlocked198, gO57MiddleUnlocked19C;
extern O57MiddleInfo gO57MiddleInfo;
extern s32 gO57MiddleStopList[], gO57MiddlePathIndices[], gO57MiddlePathList[];
extern O57MiddleOutput *gO57MiddleOutput;
extern s32 gO57MiddlePanelPosition, gO57MiddleTransition, gO57MiddleCanLeave;
extern s16 gO57MiddleColumns[], gO57MiddleColumnsEnd[];
extern u8 gO57MiddleEmptyFormat[], gO57MiddleResultFormat[];
extern u8 gO57MiddleCaption[], gO57MiddleBadge[];
extern char *gO57MiddleLabels[];
extern s16 gO57MiddleCharacterIds[];
extern u8 gO57MiddlePathId;

typedef struct O57MiddleRenderParameters {
    u8 pad00[0x10C];
    f32 position;
    f32 scale;
} O57MiddleRenderParameters;
extern O57MiddleRenderParameters gO57MiddleRenderParameters;

extern void func_80000F94();
extern void func_80005548();
extern void func_80022A50();
extern void func_80025444();
extern s32 func_80025D60();
extern void func_80028374();
extern void func_80028528();
extern void func_80028540();
extern void func_80028D24();
extern void func_800291B4();
extern s32 func_800291C4();
extern void func_8002F618();
extern void func_8002FB34();
extern void func_80039E34();
extern void func_8003A680();
extern s32 func_8003A700();
extern void func_800429A4();
extern void func_8004B0A4();
extern void func_8004B0B8();
extern void func_8004B0F8();
extern void func_80050688();
extern void func_80050704();
extern void func_overlay_045_F0000314_188C76C();
extern void func_overlay_056_F00000B8_18A2E30();
extern void func_overlay_057_F0001020_18A4C18();
extern void func_overlay_057_F00067DC_18AA3D4();
extern s32 func_overlay_068_F000146C_18C85CC();
extern s32 func_overlay_084_F0000C74_18D1154();
extern void func_overlay_084_F0001060_18D1540();
extern void func_overlay_084_F0001350_18D1830();
extern void func_overlay_084_F0001398_18D1878();

/* Identity-recovery candidate. ABI and CFG reconstruction remain in progress. */
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

    if (func_overlay_084_F0000C74_18D1154() == 0) {
        if (gO57MiddleMode != 7) {
            gO57MiddleMode = 2;
        }

        limit = 0x14;
        if ((gO57MiddleFlags.uword << 13) >> 31) {
            state = 3;
        } else {
            limit = 0x13;
            if (gO57MiddleUnlocked19C != 0) {
                state = 3;
            } else {
                state = 1;
                limit = 0xB;
                if (gO57MiddleUnlocked198 != 0) {
                    limit = 0xF;
                    state = 2;
                }
            }
        }

        if (gO57MiddleState194 == 0) {
            index = gO57MiddleSelection;
            input = gO57MiddleHorizontal;
            remainder = index / 6;
            if ((input < -16) && (index > 0) && (gO57MiddleTransition == 0)) {
                gO57MiddleMoving = 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[index], 0xA0, 0x104, 0x104);
                gO57MiddlePreviousSelection = gO57MiddleSelection;
                value = gO57MiddleSelection - 1;
                gO57MiddleSelection = value;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[value], -0xA0, 0xBE, 4);
                gO57MiddlePreviousAlpha = gO57MiddleAlpha;
                gO57MiddleAlpha = 0xFF;
                if ((gO57MiddleSelection % 6) == 5) {
                    gO57MiddleFade = 0;
                    gO57MiddleFadeDelay = 0x5A;
                    func_overlay_057_F00067DC_18AA3D4(
                        0x2F, (gO57MiddleSelection / 6) + 5, 0x3C449BA6);
                }
            } else if ((input >= 17) && (index < limit) &&
                       (gO57MiddleTransition == 0)) {
                gO57MiddleMoving = 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[index], 0xA0, 0x104, 0x104);
                gO57MiddlePreviousSelection = gO57MiddleSelection;
                value = gO57MiddleSelection + 1;
                gO57MiddleSelection = value;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[value], 0x1E0, 0xBE, 4);
                gO57MiddlePreviousAlpha = gO57MiddleAlpha;
                gO57MiddleAlpha = 0xFF;
                if ((gO57MiddleSelection % 6) == 0) {
                    gO57MiddleFade = 0;
                    gO57MiddleFadeDelay = 0x3C;
                    func_overlay_057_F00067DC_18AA3D4(
                        0x2F, gO57MiddleSelection / 6, 0x3C449BA6);
                }
            } else if ((gO57MiddleVertical < -16) && (index < (limit - 2)) &&
                       (gO57MiddleTransition == 0)) {
                gO57MiddleMoving = 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[index], 0xA0, 0x104, 0x104);
                gO57MiddlePreviousSelection = gO57MiddleSelection;
                value = gO57MiddleSelection + 3;
                previousGroup = gO57MiddleSelection / 6;
                gO57MiddleSelection = value;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[value], 0xA0, 0x104, 4);
                gO57MiddlePreviousAlpha = gO57MiddleAlpha;
                gO57MiddleAlpha = 0xFF;
                currentGroup = gO57MiddleSelection / 6;
                if (previousGroup != currentGroup) {
                    gO57MiddleFade = 0;
                    gO57MiddleFadeDelay = 0x3C;
                    func_overlay_057_F00067DC_18AA3D4(
                        0x2F, currentGroup, 0x3C449BA6, currentGroup);
                }
            } else {
                if ((gO57MiddleVertical >= 17) && (index >= 3) &&
                    (gO57MiddleTransition == 0)) {
                    gO57MiddleMoving = 1;
                    func_overlay_045_F0000314_188C76C(
                        gO57MiddleItems[index], 0xA0, 0x104, 0x104);
                    gO57MiddlePreviousSelection = gO57MiddleSelection;
                    value = gO57MiddleSelection - 3;
                    previousGroup = gO57MiddleSelection / 6;
                    gO57MiddleSelection = value;
                    func_overlay_045_F0000314_188C76C(
                        gO57MiddleItems[value], 0xA0, 0x104, 4);
                    gO57MiddlePreviousAlpha = gO57MiddleAlpha;
                    gO57MiddleAlpha = 0xFF;
                    currentGroup = gO57MiddleSelection / 6;
                    if (previousGroup != currentGroup) {
                        gO57MiddleFade = 0;
                        gO57MiddleFadeDelay = 0x3C;
                        func_overlay_057_F00067DC_18AA3D4(
                            0x2F, currentGroup + 5, 0x3C449BA6,
                            currentGroup);
                    }
                } else if ((gO57MiddleButtons & 0x2020) && (index >= 6) &&
                           (gO57MiddleTransition == 0)) {
                gO57MiddleMoving = 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[index], 0xA0, 0x104, 0x104);
                oldValue = gO57MiddleSelection;
                gO57MiddlePreviousSelection = oldValue;
                gO57MiddleSelection = oldValue - 6;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[gO57MiddleSelection], -0xA0, 0xBE, 4);
                gO57MiddlePreviousAlpha = gO57MiddleAlpha;
                gO57MiddleAlpha = 0xFF;
                gO57MiddleFade = 0;
                gO57MiddleFadeDelay = 0x5A;
                func_overlay_057_F00067DC_18AA3D4(
                    0x2F, (gO57MiddleSelection / 6) + 5, 0x3C449BA6);
                } else if ((gO57MiddleButtons & 0x10) && (remainder < state) &&
                           (gO57MiddleTransition == 0)) {
                gO57MiddleMoving = 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[index], 0xA0, 0x104, 0x104);
                oldValue = gO57MiddleSelection;
                gO57MiddlePreviousSelection = oldValue;
                gO57MiddleSelection = oldValue + 6;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[gO57MiddleSelection], 0x1E0, 0xBE, 4);
                gO57MiddlePreviousAlpha = gO57MiddleAlpha;
                gO57MiddleAlpha = 0xFF;
                gO57MiddleFade = 0;
                gO57MiddleFadeDelay = 0x5A;
                func_overlay_057_F00067DC_18AA3D4(
                    0x2F, gO57MiddleSelection / 6, 0x3C449BA6);
                if (gO57MiddleSelection > state) {
                    gO57MiddleSelection = state;
                }
                }
            }

            if ((gO57MiddleButtons & 0x4000) && (gO57MiddleTransition == 0) &&
                (gO57MiddleCanLeave != 0)) {
                func_80000F94(0xD, 0);
                func_overlay_084_F0001350_18D1830();
                func_80050704(gO57MiddlePathId);
                gO57MiddleNextMode = 7;
                func_overlay_057_F00067DC_18AA3D4(
                    0x2F, 0, 0xBC449BA6);
                gO57MiddleState188 = 0;
                gO57MiddleMode = 0;
                func_overlay_084_F0001060_18D1540(1);

                list = gO57MiddlePathList;
                index = *list;
                if (index != -1) {
                    do {
                    func_80050688(index & 0xFF);
                    currentGroup = *list;
                    func_overlay_057_F00067DC_18AA3D4(
                        currentGroup, gO57MiddlePathIndices[currentGroup], 0x3BE56042);
                    index = list[1];
                    list++;
                    } while (index != -1);
                }
                list = gO57MiddleStopList;
                index = *list;
                if (index != -1) {
                    do {
                        func_80050704(index & 0xFF);
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
                        gO57MiddlePanelPosition += (-0x1400 - gO57MiddlePanelPosition) >> 2;
                    } while (i != remainder);
                }
                while (i < updateRate) {
                    i += 4;
                    value = gO57MiddlePanelPosition + ((-0x1400 - gO57MiddlePanelPosition) >> 2);
                    nextValue = value + ((-0x1400 - value) >> 2);
                    cursorValue = nextValue + ((-0x1400 - nextValue) >> 2);
                    gO57MiddlePanelPosition = value;
                    gO57MiddlePanelPosition = nextValue;
                    gO57MiddlePanelPosition = cursorValue;
                    gO57MiddlePanelPosition = cursorValue +
                            ((-0x1400 - cursorValue) >> 2);
                }
            }
        } else if (gO57MiddleState194 == 1) {
            i = 0;
            remainder = updateRate & 3;
            if (updateRate > 0) {
                if (remainder != 0) {
                    do {
                        i++;
                        gO57MiddlePanelPosition += (-gO57MiddlePanelPosition) >> 2;
                    } while (i != remainder);
                }
                while (i < updateRate) {
                    i += 4;
                    value = gO57MiddlePanelPosition + ((-gO57MiddlePanelPosition) >> 2);
                    nextValue = value + ((-value) >> 2);
                    cursorValue = nextValue + ((-nextValue) >> 2);
                    gO57MiddlePanelPosition = value;
                    gO57MiddlePanelPosition = nextValue;
                    gO57MiddlePanelPosition = cursorValue;
                    gO57MiddlePanelPosition = cursorValue + ((-cursorValue) >> 2);
                }
            }
            if ((gO57MiddleButtons & 0x4000) && (gO57MiddleTransition == 0)) {
                func_80000F94(0xD, 0);
                gO57MiddleState194 = 0;
                gO57MiddleMode = 2;
            }
        }

        func_overlay_057_F0001020_18A4C18(updateRate);
        if ((gO57MiddlePanelPosition >> 4) >= -0x135) {
            row = 0x51;
            renderItems = (O57MiddleRenderItem *)
                ((u8 *) func_800291C4() +
                 ((s32) func_80025D60(
                      gO57MiddleCourseIds[gO57MiddleSelection]) << 5));
            func_8004B0A4(0);
            value = gO57MiddlePanelPosition >> 4;
            gO57MiddleRenderParameters.position = (f32) value;
            gO57MiddleRenderParameters.scale = 7.0f;
            func_80022A50(&gO57MiddleDisplayList, &gO57MiddleVertexList);
            func_80039E34(8);
            func_8004B0B8(
                gO57MiddleInfo.value08, gO57MiddleInfo.value09, gO57MiddleInfo.value0A,
                0xFF, 0xFF);
            valueA = value + 0xA0;
            stack64 = valueA;
            func_8004B0F8(
                &gO57MiddleDisplayList, valueA, 0x23,
                gO57MiddleText[gO57MiddleCourseNames[gO57MiddleSelection]], 4);
            func_8004B0B8(0xFF, 0x80, 0, 0xFF, 0xFF);
            func_8004B0F8(
                &gO57MiddleDisplayList, valueA, 0x37,
                gO57MiddleText[0xD0 / 4], 4);
            func_8004B0B8(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
            if (func_overlay_068_F000146C_18C85CC(
                    gO57MiddleCourseIds[gO57MiddleSelection]) != 0) {
                    func_8002F618(
                        &gO57MiddleDisplayList, gO57MiddleBadge, value + 0x30,
                        0x26, 0xFF, 0xFF, 0xFF, 0xFF);
            }

            renderItem = renderItems;
            stack5C = value + 0x56;
            i = 0;
            do {
                func_overlay_056_F00000B8_18A2E30(
                    renderItem->type, &stack80, &stack7C, &stack78);
                palette = gO57MiddleEmptyFormat;
                if (renderItem->type == 0) {
                    valueA = 0x4A;
                    func_800429A4(
                        &renderState, palette);
                } else {
                    valueB = func_8003A700(
                                 renderItem->value04, palette) & 0xFF;
                    valueC = func_8003A700(
                                 renderItem->value05) & 0xFF;
                    func_800429A4(
                        &renderState, gO57MiddleResultFormat, valueB, valueC,
                        func_8003A700(
                            renderItem->value06),
                        stack80, stack7C, stack78);
                    valueA = renderItem->value07 + 0x51;
                }
                if (i < 3) {
                    func_8004B0F8(
                        &gO57MiddleDisplayList, value + 0x2E, row,
                        gO57MiddleLabels[i], 0);
                }
                stackBC = 0;
                stackC6 = (s16) (row - 4);
                stackC0 = 0;
                stackC8 = 0;
                stackB8 = gO57MiddleGraphics[valueA];
                stackC4 = (s16) stack5C;
                func_8002F618(
                    &gO57MiddleDisplayList, &stackB8, 0, 0,
                    0xFF, 0xFF, 0xFF, 0xFF);
                color = gO57MiddleColumns;
                active = &renderState;
                do {
                    stackB0[1] = 0;
                    stackB0[0] = *active;
                    func_8004B0F8(
                        &gO57MiddleDisplayList, *color + value, row, stackB0, 0);
                    color++;
                    active++;
                } while (color != gO57MiddleColumnsEnd);
                if (i == 2) {
                    row += 0x1B;
                    func_8004B0B8(
                        0xFF, 0x80, 0, 0xFF, 0xFF);
                    func_8004B0F8(
                        &gO57MiddleDisplayList, (u8) stack64, row,
                        gO57MiddleText[0xD4 / 4], 4);
                    func_8004B0B8(
                        0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                i++;
                renderItem++;
                row += 0x1B;
                stack5C += 0x1B;
            } while (i != 4);
            captionBits.value = (f32) (value + 0x2D);
            func_8002FB34(
                &gO57MiddleDisplayList, gO57MiddleCaption, captionBits.bits,
                0x43380000, 0x3F800000, 0x3F800000, -2, 3);
        }

        if (gO57MiddleFadeDelay > 0) {
            gO57MiddleFadeDelay -= updateRate;
        } else {
            gO57MiddleFade += updateRate * 4;
            if (gO57MiddleFade >= 0x100) {
                gO57MiddleFade = 0xFF;
            }
        }

        if ((gO57MiddleButtons & 0x9000) && (gO57MiddleTransition == 0)) {
            func_80000F94(0xC, 0);
            if (gO57MiddlePlayerCount >= 2 || gO57MiddleState194 == 1) {
                activeInit = (s8 *) &activePlayers[9];
                i = 9;
                do {
                    *activeInit = 1;
                    activeInit--;
                    i--;
                } while (i >= 0);
                choice = gO57MiddleChoices;
                source = &sourceState[0];
                outputIndex = 0;
                while ((u8 *) choice < (u8 *)&gO57MiddleFlags) {
                    choiceActive = choice->active;
                    *source++ = choiceActive;
                    if (choiceActive != 0) {
                        value = gO57MiddleCharacterIds[choice->tableIndex];
                        gO57MiddleOutput[outputIndex].controller = (u8) value;
                        activePlayers[value] = 0;
                        outputIndex++;
                    }
                    choice = (O57MiddleChoice *) ((u8 *) choice + 0x34);
                }
                activeCount = gO57MiddlePlayerCount;
                rank = 0;
                active = activePlayers;
                if (activeCount < 6) {
                    output = &gO57MiddleOutput[activeCount];
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
                    } while (output < &gO57MiddleOutput[6]);
                }
                func_80025444(
                    sourceState, 0x28, activePlayers, &gO57MiddleFlags);
                func_80028D24(0);
                func_80028540(gO57MiddlePlayerCount);
                gO57MiddleData31E4 = 0;
                if (gO57MiddlePlayerCount == 1) {
                    gO57MiddleOutput[0].value0 = 1;
                    if ((gO57MiddleFlags.half & 1) != 0) {
                        gO57MiddleFlags.bytes[1] &= 0xFE;
                        func_800291B4();
                        func_8003A680(0x14);
                    }
                } else {
                    gO57MiddleOutput[0].value0 = 4;
                }
                state = gO57MiddlePlayerCount;
                if (((state == 2) || (state == 3)) &&
                    (gO57MiddleData3214 != 0)) {
                    gO57MiddleData319C = (s8)(4 - state);
                    gO57MiddleData3198 = 4;
                } else {
                    gO57MiddleData319C = 0;
                    gO57MiddleData3198 = (u8)state;
                }
                gO57MiddleData31AC = (u8)((state < 2) ^ 1);
                gO57MiddleData31A4 = 2;
                gO57MiddleData31B8 = gO57MiddleData31B4;
                gO57MiddleData31A8 = 0;
                if (gO57MiddleData31E4 > 0) {
                    gO57MiddleData31E8 = gO57MiddleCourseIds[gO57MiddleSelection];
                    gO57MiddleData31EC = gO57MiddleCharacterIds[gO57MiddleChoices[0].tableIndex];
                    gO57MiddleData31F0 = 5;
                    gO57MiddleData31F4 = 0;
                    func_80028374(0x12, 0, 0, 0xF, 1, 0);
                    func_80028528(1);
                } else {
                    func_80028374(
                        gO57MiddleCourseIds[gO57MiddleSelection],
                        gO57MiddleCharacterIds[gO57MiddleChoices[0].tableIndex],
                        0, 5, 1, 0);
                }
                gO57MiddleTransition = 1;
                func_overlay_084_F0001398_18D1878();
                if ((gO57MiddleData31AC == 0) ||
                    ((gO57MiddleData31AC != 0) && (gO57MiddleData3208 != 0))) {
                    func_80005548(gO57MiddleData3198);
                    if (gO57MiddleData3208 != 0) {
                        gO57MiddleData3208 = 0;
                    }
                }
            } else {
                gO57MiddleState194 = 1;
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o057/func_overlay_057_F0004E18_18A8A10/func_overlay_057_F0004E18_18A8A10.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_057_F0004E18_18A8A10:start
 * symbol: func_overlay_057_F0004E18_18A8A10
 * score: 1240 differing words
 * frame: 0x140
 * relocations: 375
 * first-mismatch: +0x2C
 * summary: Identity checkpoint: 1276/1208 words, 1242 raw differences, 375/379 relocations. Recovered 65 call identities. ABI and CFG reconstruction continues.
 * PLATEAU-HANDOFF:func_overlay_057_F0004E18_18A8A10:end
 */
