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

/* Typed reconstruction candidate; control-flow reconstruction remains in
 * progress. The frame is exact at 0x140: renderState is a 24-byte buffer, not
 * 32 -- 24 is the only size in 21..24 that closes the frame, and every other
 * homed object's size was already pinned by its fake name (stack5C, stack64,
 * stack78/7C/80, stackB0 are the target's own offsets). Declaration order
 * still places the homed block 16..40 bytes off those offsets.
 *
 * 494 -> 272 masked words by three source edits, two of which are a pair that
 * only works together:
 *
 *  1+2. The two path-list walks. The first was spelled with a named `index`
 *       carrying the loop value and a named `currentGroup` carrying the
 *       re-read; the target names NEITHER. Writing both walks as
 *       `while (*list != -1) { call((u8)*list); call(*list, map[*list], k); }`
 *       reproduces the target's loop instruction-for-instruction: one web for
 *       the list value, re-read after the call because the call may write it,
 *       and the `& 0xFF` mask folded into the argument as a single
 *       `andi a0,<value>,0xff` instead of `move`/`andi`/`move`. The `(u8)`
 *       cast and the `while` shape are ONE edit each and each alone moves the
 *       size (+8 and -8); together they are delta 0 and worth 36 words.
 *       Naming a re-read value is what costs the extra web -- this is the
 *       drop-a-declared-local lever the workbench names for a longer pool
 *       lane, applied to a loop body.
 *
 *  3.   `row = 0x51` was the first statement of the panel block, so as1 put
 *       `li s3,81` in the delay slot of the block's guard branch; the target
 *       leaves that slot a nop and materialises the constant 27 instructions
 *       later, inside the basic block that ends at the func_80022A50 call.
 *       Moving the assignment after `func_8004B0A4(0)` reproduces the target
 *       from `bnez at` through `swc1 $f8` exactly and is worth 186 words --
 *       far more than the one slot, because it also re-colours s3 and the
 *       whole run of calls that follow it.
 *
 *  4.   THE SURPLUS INSTRUCTION, closed 2026-09-11 (lane/p9-oneoff). The
 *       diagnosis above was right about the web and wrong about where it is
 *       anchored. It is not the two tail reads that hold
 *       `&gO57MiddleChoices` alive -- it is the choice loop's own BOUND.
 *       `do { ... } while (choice < &gO57MiddleChoices[4])` makes the array's
 *       end address a loop invariant, uopt hoists it into a callee-saved
 *       register, and from there it reaches the two
 *       `gO57MiddleCharacterIds[gO57MiddleChoices[0].tableIndex]` reads past
 *       the calls between them, which is why a region marker around the
 *       reads could never close it: the marker was on the wrong end of the
 *       web. Bounding the walk on the OTHER pointer the loop already steps,
 *       `while (source < &sourceState[4])`, names no address of the global at
 *       all. Both pointers advance in lockstep from the same do/while, so the
 *       trip count is identical and the semantics are unchanged.
 *
 *       Measured, with tools/align_symbol.py: size delta +4 -> 0 (1209 words
 *       -> 1208, exact), positional masked 272 -> 231, displacement tax
 *       59 -> 26, and the aligned split 1002/113/100 -> 1012/109/96. The first
 *       STRUCTURAL difference moves from +0x34 to +0xBC4 -- 755 words in
 *       which nothing but register naming now differs.
 *
 *       `!=` instead of `<` on either pointer overshoots to delta -4
 *       (269 and 266 words); an explicit counter in `rank` or `valueA`
 *       is +8 and +260; `for (choice = ...; choice < &gO57MiddleChoices[4];
 *       choice++, source++)` is +24. `<` on `sourceState` is the only
 *       spelling in the set that is exact-sized.
 *
 * What is left, measured: the size is now exact and the residual is
 * allocation. The first structural difference at +0xBC4 is a STACK HOME
 * (`sw a1,84(sp)` against the target's `sw a1,100(sp)`), and the whole homed
 * block is displaced: stack5C/stack64 sit at 80/84 against the target's
 * 92/100, stack78/7C/80 at 160/164/168 against 120/124/128, the
 * address-taken array block starts at 172 against 144, and activePlayers is
 * at 232 against 272. The frame total is exact at 0x140 on both sides, so
 * this is an ordering/padding question, not a size one.
 *
 * L112 was tested directly and does NOT apply here. Every unobservable
 * dimension was swept: sourceState[6/8/10], activePlayers[12/16],
 * renderState[32/40], stackB0[4/8], textureNodes[3]. Every enlargement that
 * moves anything costs exactly the same 13 words (231 -> 244) and none gains,
 * so no array count solves the displacement -- the target's gaps at
 * 132..143, 168..175 and 216..271 are not a bigger array.
 *
 * Re-tested 2026-09-11 and still rejected, now that the size is exact:
 * caching `choice->active` in a local is 527 words and delta -8; the
 * activePlayers countdown pointer walk (both the `*active-- = 1` and the
 * split `*active = 1; active--;` spellings) is 282 words and delta +4.
 *
 * Two target structures are read but NOT adoptable yet, both because they cost
 * more than they buy at this frame layout:
 *   - the choice loop caches `choice->active` in ONE load: target is
 *     `lb v1,42(v0); beqz v1; sb v1,0(a0)`, the candidate reloads because the
 *     store through `source` may alias the global. Caching it in a local
 *     reproduces the target's three instructions exactly and takes the size to
 *     delta -4, but costs 290 words: it consumes the callee-saved register the
 *     loop's `outputIndex` holds in the target. Measured identical at 562 for
 *     `choiceActive`, `rank`, `nextValue` and `activeCount` as the carrier, so
 *     the carrier is not the variable; it is the register pressure.
 *   - the activePlayers fill is a countdown pointer walk with a dead
 *     post-decrement copy (`li v1,9`, `move v0,v1`, `sb`, `addiu a0,a0,-1`,
 *     `bnez v1`, `addiu v1,v1,-1`), not the `for (i = 9; i >= 0; i--)` index
 *     form here. Adopting it alone costs 301 words and +8 bytes.
 * Both are the kind that need the stack-home fix first: the homed block is
 * still 40 bytes low (activePlayers sp+232 against the target's sp+272,
 * stack78/7C/80 sp+160/164/168 against sp+120/124/128), and the target's
 * layout has gaps -- 132..143, 168..175, 216..271 -- that no current
 * declaration accounts for. */
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
    s32 nextSelection;
    s32 panelX;
    s32 characterId;
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
    char renderState[24];
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

        if ((gO57MiddleFlags.uword << 13) >> 31) {
            limit = 0x14;
            state = 3;
        } else if (gO57MiddleUnlocked19C != 0) {
            limit = 0x13;
            state = 3;
        } else if (gO57MiddleUnlocked198 != 0) {
            limit = 0xF;
            state = 2;
        } else {
            state = 1;
            limit = 0xB;
        }

        if (gO57MiddleState194 == 0) {
            currentGroup = gO57MiddleSelection / 6;
            if ((gO57MiddleHorizontal < -16) && (gO57MiddleSelection > 0) && (gO57MiddleTransition == 0)) {
                gO57MiddleMoving = 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[gO57MiddleSelection], 0xA0, 0x104, 0x104);
                gO57MiddlePreviousSelection = gO57MiddleSelection;
                gO57MiddleSelection -= 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[gO57MiddleSelection], -0xA0, 0xBE, 4);
                gO57MiddlePreviousAlpha = gO57MiddleAlpha;
                gO57MiddleAlpha = 0xFF;
                if ((gO57MiddleSelection % 6) == 5) {
                    gO57MiddleFade = 0;
                    gO57MiddleFadeDelay = 0x5A;
                    func_overlay_057_F00067DC_18AA3D4(
                        0x2F, (gO57MiddleSelection / 6) + 5, 0.012f);
                }
            } else if ((gO57MiddleHorizontal >= 17) && (gO57MiddleSelection < limit) &&
                       (gO57MiddleTransition == 0)) {
                gO57MiddleMoving = 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[gO57MiddleSelection], 0xA0, 0x104, 0x104);
                gO57MiddlePreviousSelection = gO57MiddleSelection;
                gO57MiddleSelection += 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[gO57MiddleSelection], 0x1E0, 0xBE, 4);
                gO57MiddlePreviousAlpha = gO57MiddleAlpha;
                gO57MiddleAlpha = 0xFF;
                if ((gO57MiddleSelection % 6) == 0) {
                    gO57MiddleFade = 0;
                    gO57MiddleFadeDelay = 0x3C;
                    func_overlay_057_F00067DC_18AA3D4(
                        0x2F, gO57MiddleSelection / 6, 0.012f);
                }
            } else if ((gO57MiddleVertical < -16) && (gO57MiddleSelection < (limit - 2)) &&
                       (gO57MiddleTransition == 0)) {
                gO57MiddleMoving = 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[gO57MiddleSelection], 0xA0, 0x104, 0x104);
                gO57MiddlePreviousSelection = gO57MiddleSelection;
                previousGroup = gO57MiddleSelection / 6;
                gO57MiddleSelection += 3;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[gO57MiddleSelection], 0xA0, 0x104, 4);
                gO57MiddlePreviousAlpha = gO57MiddleAlpha;
                gO57MiddleAlpha = 0xFF;
                currentGroup = gO57MiddleSelection / 6;
                if (previousGroup != currentGroup) {
                    gO57MiddleFade = 0;
                    gO57MiddleFadeDelay = 0x3C;
                    func_overlay_057_F00067DC_18AA3D4(
                        0x2F, currentGroup, 0.012f);
                }
            } else if ((gO57MiddleVertical >= 17) && (gO57MiddleSelection >= 3) &&
                    (gO57MiddleTransition == 0)) {
                    gO57MiddleMoving = 1;
                    func_overlay_045_F0000314_188C76C(
                        gO57MiddleItems[gO57MiddleSelection], 0xA0, 0x104, 0x104);
                    gO57MiddlePreviousSelection = gO57MiddleSelection;
                    previousGroup = gO57MiddleSelection / 6;
                    gO57MiddleSelection -= 3;
                    func_overlay_045_F0000314_188C76C(
                        gO57MiddleItems[gO57MiddleSelection], 0xA0, 0x104, 4);
                    gO57MiddlePreviousAlpha = gO57MiddleAlpha;
                    gO57MiddleAlpha = 0xFF;
                    currentGroup = gO57MiddleSelection / 6;
                    if (previousGroup != currentGroup) {
                        gO57MiddleFade = 0;
                        gO57MiddleFadeDelay = 0x3C;
                        func_overlay_057_F00067DC_18AA3D4(
                            0x2F, currentGroup + 5, 0.012f);
                    }
                } else if ((gO57MiddleButtons & 0x2020) && (gO57MiddleSelection >= 6) &&
                           (gO57MiddleTransition == 0)) {
                gO57MiddleMoving = 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[gO57MiddleSelection], 0xA0, 0x104, 0x104);
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
                } else if ((gO57MiddleButtons & 0x10) && (currentGroup < state) &&
                           (gO57MiddleTransition == 0)) {
                gO57MiddleMoving = 1;
                func_overlay_045_F0000314_188C76C(
                    gO57MiddleItems[gO57MiddleSelection], 0xA0, 0x104, 0x104);
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
                if (gO57MiddleSelection > limit) {
                    gO57MiddleSelection = limit;
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
                while (*list != -1) {
                    func_80050688((u8)*list);
                    func_overlay_057_F00067DC_18AA3D4(
                        *list, gO57MiddlePathIndices[*list], 0.007f);
                    list++;
                }
                list = gO57MiddleStopList;
                while (*list != -1) {
                    func_80050704((u8)*list);
                    list++;
                }
            }

            for (i = 0; i < updateRate; i++) {
                gO57MiddlePanelPosition += (-0x1400 - gO57MiddlePanelPosition) >> 2;
            }
        } else {
            if (gO57MiddleState194 == 1) {
                for (i = 0; i < updateRate; i++) {
                    gO57MiddlePanelPosition += (-gO57MiddlePanelPosition) >> 2;
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
            renderItems = (O57MiddleRenderItem *)
                ((u8 *) func_800291C4() +
                 ((s32) func_80025D60(
                      gO57MiddleCourseIds[gO57MiddleSelection]) << 5));
            func_8004B0A4(0);
            row = 0x51;
            panelX = gO57MiddlePanelPosition >> 4;
            gO57MiddleRenderParameters.position = (f32) panelX;
            gO57MiddleRenderParameters.scale = 7.0f;
            func_80022A50(&gO57MiddleDisplayList, &gO57MiddleVertexList);
            func_80039E34(8);
            func_8004B0B8(
                gO57MiddleInfo.value08, gO57MiddleInfo.value09, gO57MiddleInfo.value0A,
                0xFF, 0xFF);
            valueA = panelX + 0xA0;
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
                        &gO57MiddleDisplayList, gO57MiddleBadge, panelX + 0x30,
                        0x26, 0xFF, 0xFF, 0xFF, 0xFF);
            }

            renderItem = renderItems;
            stack5C = panelX + 0x56;
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
                        &gO57MiddleDisplayList, panelX + 0x2E, row,
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
                        &gO57MiddleDisplayList, *color + panelX, row, stackB0, 0);
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
            } while (i != 4);
            func_8002FB34(
                &gO57MiddleDisplayList, gO57MiddleCaption, (f32)(panelX + 0x2D),
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
                for (i = 9; i >= 0; i--) {
                    activePlayers[i] = 1;
                }
                choice = gO57MiddleChoices;
                source = sourceState;
                outputIndex = 0;
                do {
                    *source = choice->active;
                    if (choice->active != 0) {
                        gO57MiddleOutput[outputIndex].controller =
                            gO57MiddleCharacterIds[choice->tableIndex];
                        activePlayers[gO57MiddleCharacterIds[choice->tableIndex]] = 0;
                        outputIndex++;
                    }
                    choice++;
                    source++;
                    /* Bound on sourceState, not on
                     * &gO57MiddleChoices[4]: naming the global's end
                     * address here makes it a loop invariant that uopt
                     * parks in a callee-saved register, and it then
                     * reaches the two gO57MiddleChoices[0] reads in the
                     * tail. Both pointers step together, so the trip
                     * count is the same. Worth the whole size delta:
                     * 1209 words -> 1208, 272 -> 231 masked. */
                } while (source < &sourceState[4]);
                i = 0;
                for (outputIndex = gO57MiddlePlayerCount; outputIndex < 6; outputIndex++) {
                    while (activePlayers[i] == 0) {
                        i++;
                    }
                    gO57MiddleOutput[outputIndex].controller = i;
                    i++;
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
 * score: 231/1208 words
 * frame: 0x140
 * relocations: 373
 * first-mismatch: +0x100
 * summary: 494 falls to 231 on four edits -- both path-list walks respelled as while loops over *list with no named index or re-read local and a (u8) cast rather than a mask, which is a delta-0 pair worth 36 words where each half alone moves the size, and row = 0x51 moved after func_8004B0A4 so the constant leaves the guard branch delay slot, worth 186; and the choice loop bounded on &sourceState[4] rather than &gO57MiddleChoices[4], which closes the surplus instruction -- the loop bound, not the tail reads, is what anchored the callee-saved address web -- taking the size delta to 0 and the first structural difference from +0x34 to +0xBC4; what is left is 109 register-naming and 96 structural words, led by a displaced homed block that no local array dimension moves.
 * PLATEAU-HANDOFF:func_overlay_057_F0004E18_18A8A10:end
 */
