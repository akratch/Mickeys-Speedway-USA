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
    u8 value0;
    u8 pad01[3];
    u8 controller;
    u8 pad05[0x23];
} O57MiddleOutput;

typedef union O57MiddleInput {
    s32 word;
    u32 uword;
    s16 half;
    u8 bytes[4];
} O57MiddleInput;

typedef struct O57MiddleTextureNode {
    void *texture;
    void *alternate;
    u32 packedOffset;
    s16 x;
    s16 y;
} O57MiddleTextureNode;

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
extern char gO57MiddleEmptyFormat[], gO57MiddleResultFormat[];
extern O57MiddleTextureNode gO57MiddleCaption[], gO57MiddleBadge[];
extern char *gO57MiddleLabels[];
extern s16 gO57MiddleCharacterIds[];
extern u8 gO57MiddlePathId;

typedef struct O57MiddleRenderParameters {
    u8 pad00[0x10C];
    f32 position;
    f32 scale;
} O57MiddleRenderParameters;
extern O57MiddleRenderParameters gO57MiddleRenderParameters;

extern void func_80000F94(u16 soundId, void **handle);
extern void func_80005548(u8 count);
extern void func_80022A50(void **displayList, void **matrices);
extern void func_80025444(s8 *players);
extern s32 func_80025D60(s32 course);
extern void func_80028374(s32 level, s32 character, s32 animation, s32 mode, s32 arg4, s32 arg5);
extern void func_80028528(s32 group);
extern void func_80028540(s32 cameras);
extern void func_80028D24(s32 mode);
extern void func_800291B4(void);
extern O57MiddleRenderItem * func_800291C4(void);
extern void func_8002F618(void **displayList, O57MiddleTextureNode *nodes, s32 x, s32 y, u8 r, u8 g, u8 b, u8 a);
extern void func_8002FB34(void **displayList, O57MiddleTextureNode *nodes, f32 x, f32 y, f32 sx, f32 sy, s32 mode, s32 flags);
extern void func_80039E34(s32 spacing);
extern void func_8003A680(u8 character);
extern s32 func_8003A700(u8 character);
extern s32 func_800429A4(char *buffer, const char *format, ...);
extern void func_8004B0A4(s32 font);
extern void func_8004B0B8(s32 r, s32 g, s32 b, s32 a, s32 opacity);
extern void func_8004B0F8(void **displayList, s32 x, s32 y, char *text, s32 align);
extern void func_80050688(u8 path);
extern void func_80050704(u8 path);
extern void func_overlay_045_F0000314_188C76C(void *descriptor, s32 x, s32 y, s32 flags);
extern void func_overlay_056_F00000B8_18A2E30(s32 time, s32 *first, s32 *second, s32 *third);
extern void func_overlay_057_F0001020_18A4C18(s32 updateRate);
extern void func_overlay_057_F00067DC_18AA3D4(s32 id, s32 argument, f32 value);
extern s32 func_overlay_068_F000146C_18C85CC(s32 course);
extern s32 func_overlay_084_F0000C74_18D1154(void);
extern void func_overlay_084_F0001060_18D1540(s32 state);
extern void func_overlay_084_F0001350_18D1830(void);
extern void func_overlay_084_F0001398_18D1878(void);

/* Typed reconstruction candidate; control-flow reconstruction remains in progress. */
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
    s8 sourceState[4];
    u8 activePlayers[10];
    O57MiddleTextureNode textureNodes[2];
    char stackB0[2];
    char renderState[32];
    s32 stack80;
    s32 stack7C;
    s32 stack78;
    s32 stack64;
    s32 stack5C;
    O57MiddleRenderItem *renderItems;
    O57MiddleRenderItem *renderItem;
    O57MiddleChoice *choice;
    s16 *color;
    char *palette;
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
                        0x2F, (gO57MiddleSelection / 6) + 5, 0.012f);
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
                        0x2F, gO57MiddleSelection / 6, 0.012f);
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
                        0x2F, currentGroup, 0.012f);
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
                            0x2F, currentGroup + 5, 0.012f);
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
                    0x2F, (gO57MiddleSelection / 6) + 5, 0.012f);
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
                    0x2F, gO57MiddleSelection / 6, 0.012f);
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
                    0x2F, 0, -0.012f);
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
                        currentGroup, gO57MiddlePathIndices[currentGroup], 0.007f);
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
                        renderState, palette);
                } else {
                    valueB = func_8003A700(
                                 renderItem->value04) & 0xFF;
                    valueC = func_8003A700(
                                 renderItem->value05) & 0xFF;
                    func_800429A4(
                        renderState, gO57MiddleResultFormat, valueB, valueC,
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
                textureNodes[0].alternate = NULL;
                textureNodes[0].y = (s16)(row - 4);
                textureNodes[0].packedOffset = 0;
                textureNodes[1].texture = NULL;
                textureNodes[0].texture = gO57MiddleGraphics[valueA];
                textureNodes[0].x = (s16)stack5C;
                func_8002F618(
                    &gO57MiddleDisplayList, textureNodes, 0, 0,
                    0xFF, 0xFF, 0xFF, 0xFF);
                color = gO57MiddleColumns;
                active = (u8 *)renderState;
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
                        &gO57MiddleDisplayList, stack64, row,
                        gO57MiddleText[0xD4 / 4], 4);
                    func_8004B0B8(
                        0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
                }
                i++;
                renderItem++;
                row += 0x1B;
                stack5C += 0x1B;
            } while (i != 4);
            func_8002FB34(
                &gO57MiddleDisplayList, gO57MiddleCaption, (f32)(value + 0x2D),
                184.0f, 1.0f, 1.0f, -2, 3);
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
                    sourceState);
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
 * score: 1236 differing words
 * frame: 0x140
 * relocations: 373
 * first-mismatch: +0x2C
 * summary: Typed checkpoint: 1282/1208 words, 1239 raw differences, 373/379 relocations. Corrected byte/pointer output and renderer ABI. Next restore natural CFG.
 * PLATEAU-HANDOFF:func_overlay_057_F0004E18_18A8A10:end
 */
