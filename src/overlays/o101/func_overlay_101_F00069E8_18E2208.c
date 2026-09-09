#include "PR/ultratypes.h"

typedef struct O101RootGroup {
    s32 childType;
    void *child;
    u8 mode;
    u8 pad09;
    s16 x;
    s16 width;
    s16 y;
    s16 height;
    s16 value12;
    s16 value14;
    u8 color16;
    u8 color17;
    void *data18;
} O101RootGroup;

typedef struct O101Node32_69E8 {
    s32 previousType;
    void *previous;
    s16 x;
    s16 y;
    f32 scale;
    s16 value10;
    u8 color12;
    u8 color13;
    f32 value14;
    s32 value18;
    void *handle;
} O101Node32_69E8;

typedef struct O101Node20_69E8 {
    s32 previousType;
    void *previous;
    s16 x;
    s16 y;
    f32 scale;
    void *handle;
} O101Node20_69E8;

typedef struct O101Node24_69E8 {
    s32 previousType;
    void *previous;
    s16 x;
    s16 y;
    u8 length;
    s8 opacity;
    u8 mode;
    u8 color0;
    u8 color1;
    u8 color2;
    u8 color3;
    u8 kind;
    void *text;
} O101Node24_69E8;

typedef struct O101Inputs69E8 {
    u8 pad000[0xC8];
    void *dataC8;
    u8 *textCC;
    u8 *textD0;
    void *dataD4;
    void *dataD8;
    u8 *textDC;
    u8 *textE0;
    void *dataE4;
    u8 *textE8;
    u8 *textEC;
    void *dataF0;
    u8 *textF4;
} O101Inputs69E8;

/* Tier B: runtime LOCAL relocation pairs and ROM-exact cleanup/draw
 * siblings identify the order records, order list, resource arrays and
 * their three counters. The text-node pool and input block are separate
 * objects; equal encoded addends never establish an alias. */
extern O101RootGroup gOverlay101OrderEntries[];
extern O101Inputs69E8 D_INPUT;
extern O101RootGroup D_1C;
extern O101RootGroup D_38;
extern O101RootGroup D_54;
extern O101RootGroup D_70;
extern O101RootGroup D_8C;
extern O101RootGroup D_A8;
extern u8 D_CC8[];
extern u8 D_1D2C[];
extern void *gOverlay101Order[];
extern s32 gOverlay101OrderCount;
extern s32 gOverlay101Resource20Count;
extern s32 gOverlay101Resource32Count;
extern s32 D_1D0;
extern O101Node20_69E8 gOverlay101Resources20[];
extern O101Node32_69E8 gOverlay101Resources32[];
extern O101Node24_69E8 D_540[];

/* Tier B: this function's runtime relocation records resolve ten sprite
 * loads to resident func_800355A0, four screen loads to func_80036DD0,
 * seven local calls to overlay101ByteLength, and the final call to
 * overlay101Reset. The matched local callees establish their prototypes. */
extern void *func_800355A0(s32 assetId, s32 flags);
extern s32 *func_80036DD0(s32 screenIndex);
extern s32 overlay101ByteLength(u8 *text);
extern void overlay101Reset(void *value);

/*
 * Mickey-local reconstruction from this overlay's extracted function and
 * the typed node builders immediately before and after it. The pinned DKR
 * v77/v80 and JFG overlay scans report no donor for overlay 101.
 * Configured full-TU residual measurement: 963/963 words,
 * 616 raw / 613 relocation-masked differences, first +0x88; frame 0x50.
 * The initial group line ties and selected expanded builders preserve the
 * reconstructed statement tokens while retaining the closer stock schedule.
 * The runtime table owns 110 relocations; the extracted assembly object
 * retains only 78 text relocations; its placeholders are not promotion proof.
 *
 * Residual shape, measured: 81% of the aligned differences are register
 * naming, not structure. Canonicalizing one saved-register pair and the
 * scratch-ring temp names leaves 296 of 616 differing words. Seven of the
 * nine saved registers already agree symbol-for-symbol; only the pool-index
 * web and the shared 0x78/screen-node-pointer web are exchanged, and the
 * scratch ring runs one slot behind from +0x88 onward because the pool count
 * this unit reads before each loader call is coloured to that saved register
 * instead of a scratch temporary, which forces its stack save into the entry
 * block a slot early.
 */
#ifdef NON_MATCHING
void func_overlay_101_F00069E8_18E2208(void) {
    register s32 commonY;
    s32 index;
    s32 orderIndex;
    s32 length;
    void *handle;
    O101Node20_69E8 *node20;
    O101Node24_69E8 *node24;
    O101Node32_69E8 *node32;

#define INIT_GROUP(group, px, pwidth, py, pheight, value0, value1, input,      \
                   owner)                                                      \
    gOverlay101OrderEntries[1 + group].x = (px);                               \
    gOverlay101OrderEntries[1 + group].width = (pwidth);                       \
    gOverlay101OrderEntries[1 + group].y = (py);                               \
    gOverlay101OrderEntries[1 + group].height = (pheight);                     \
    gOverlay101OrderEntries[1 + group].value12 = (value0);                     \
    gOverlay101OrderEntries[1 + group].value14 = (value1);                     \
    gOverlay101OrderEntries[1 + group].mode = 0;                               \
    gOverlay101OrderEntries[1 + group].color16 = 0xFF;                         \
    gOverlay101OrderEntries[1 + group].color17 = 0xFF;                         \
    gOverlay101OrderEntries[1 + group].childType = 0;                          \
    gOverlay101OrderEntries[1 + group].child = 0;                              \
    gOverlay101OrderEntries[1 + group].data18 = D_INPUT.input;                 \
    orderIndex = gOverlay101OrderCount;                                        \
    gOverlay101Order[orderIndex] = &(owner);                                   \
    gOverlay101OrderCount = orderIndex + 1

/* y is assigned before x here: the target materializes the sprite's y
 * constant first and stores x first, and the stock scheduler reverses the
 * pair, so only this order reproduces both the constant order and the store
 * order over +0xBC..+0xE4. The screen and text builders measure worse this
 * way and keep the declared field order. */
#define ADD_NODE32(group, nodeX, nodeY, nodeScale, nodeValue, nodeColor,       \
                   imageId)                                                    \
    index = gOverlay101Resource32Count;                                        \
    node32 = &gOverlay101Resources32[index];                                   \
    node32->y = (nodeY);                                                       \
    node32->x = (nodeX);                                                       \
    node32->scale = (nodeScale);                                               \
    node32->value10 = (nodeValue);                                             \
    node32->color12 = (nodeColor);                                             \
    node32->color13 = 0;                                                       \
    node32->value14 = 0.0f;                                                    \
    node32->value18 = 0;                                                       \
    handle = func_800355A0((imageId), 0);                                      \
    index = gOverlay101Resource32Count;                                        \
    node32 = &gOverlay101Resources32[index];                                   \
    node32->previousType = gOverlay101OrderEntries[1 + group].childType;       \
    node32->previous = gOverlay101OrderEntries[1 + group].child;               \
    node32->handle = handle;                                                   \
    gOverlay101OrderEntries[1 + group].childType = 2;                          \
    gOverlay101OrderEntries[1 + group].child = node32;                         \
    gOverlay101Resource32Count = index + 1

#define ADD_NODE20(group, nodeX, nodeY, imageId)                               \
    index = gOverlay101Resource20Count;                                        \
    node20 = &gOverlay101Resources20[index];                                   \
    node20->x = (nodeX);                                                       \
    node20->y = (nodeY);                                                       \
    node20->scale = 1.0f;                                                      \
    handle = func_80036DD0(imageId);                                           \
    index = gOverlay101Resource20Count;                                        \
    node20 = &gOverlay101Resources20[index];                                   \
    node20->previousType = gOverlay101OrderEntries[1 + group].childType;       \
    node20->previous = gOverlay101OrderEntries[1 + group].child;               \
    node20->handle = handle;                                                   \
    gOverlay101OrderEntries[1 + group].childType = 1;                          \
    gOverlay101OrderEntries[1 + group].child = node20;                         \
    gOverlay101Resource20Count = index + 1

#define ADD_TEXT(group, input, textX, textY)                                   \
    index = D_1D0;                                                             \
    node24 = &D_540[index];                                                    \
    node24->x = (textX);                                                       \
    node24->y = (textY);                                                       \
    length = overlay101ByteLength(D_INPUT.input);                              \
    index = D_1D0;                                                             \
    node24 = &D_540[index];                                                    \
    node24->length = (u8)length;                                               \
    node24->opacity =                                                          \
        (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1);                    \
    node24->mode = 2;                                                          \
    node24->color0 = 0xFF;                                                     \
    node24->color1 = 0xFF;                                                     \
    node24->color2 = 0xFF;                                                     \
    node24->color3 = 0xFF;                                                     \
    node24->kind = 4;                                                          \
    node24->text = D_INPUT.input;                                              \
    node24->previousType = gOverlay101OrderEntries[1 + group].childType;       \
    node24->previous = gOverlay101OrderEntries[1 + group].child;               \
    gOverlay101OrderEntries[1 + group].childType = 3;                          \
    gOverlay101OrderEntries[1 + group].child = node24;                         \
    D_1D0 = index + 1

    gOverlay101OrderEntries[1 + 0].mode = 4; \
    gOverlay101OrderEntries[1 + 0].value12 = 0x140; \
    gOverlay101OrderEntries[1 + 0].value14 = 0xF0; \
    gOverlay101OrderEntries[1 + 0].data18 = &D_CC8; \
    gOverlay101OrderEntries[1 + 0].color16 = 0xFF; \
    gOverlay101OrderEntries[1 + 0].color17 = 0xFF; \
    gOverlay101OrderEntries[1 + 0].x = 0; \
    gOverlay101OrderEntries[1 + 0].width = 0; \
    gOverlay101OrderEntries[1 + 0].y = 0; \
    gOverlay101OrderEntries[1 + 0].height = 0; \
    gOverlay101OrderEntries[1 + 0].childType = 0; \
    gOverlay101OrderEntries[1 + 0].child = 0; \
    orderIndex = gOverlay101OrderCount; \
    gOverlay101Order[orderIndex] = &D_1C; \
    gOverlay101OrderCount = orderIndex + 1;

    ADD_NODE32(0, 0xF2, 0x14E, 1.0f, 0, 0xFF, 0x91);
    commonY = 0x78;
    ADD_NODE32(0, 0xA0, commonY, 5.0f, 0xE38, 0, 0x96);
    ADD_NODE32(0, 0xA0, commonY, 5.0f, 0x71C, 0, 0x96);
    ADD_NODE32(0, 0xA0, commonY, 5.0f, 0, 0, 0x96);
    ADD_NODE32(0, 0xA0, commonY, 5.0f, 0xE38, 0, 0x95);
    ADD_NODE32(0, 0xA0, commonY, 5.0f, 0x71C, 0, 0x95);
    ADD_NODE32(0, 0xA0, commonY, 5.0f, 0, 0, 0x95);
    ADD_NODE32(0, 0xA0, commonY, 5.0f, 0xE38, 0, 0x94);
    ADD_NODE32(0, 0xA0, commonY, 5.0f, 0x71C, 0, 0x94);
    ADD_NODE32(0, 0xA0, commonY, 5.0f, 0, 0, 0x94);

    INIT_GROUP(1, 0x20, 0x18, 0x30, 0x10, 0xE0, 0xC0, dataC8, D_38);
    index = gOverlay101Resource20Count;
    node20 = &gOverlay101Resources20[index];
    node20->x = (0xC);
    node20->y = (0xE);
    node20->scale = 1.0f;
    handle = func_80036DD0(0xD);
    index = gOverlay101Resource20Count;
    node20 = &gOverlay101Resources20[index];
    node20->previousType = gOverlay101OrderEntries[1 + 1].childType;
    node20->previous = gOverlay101OrderEntries[1 + 1].child;
    node20->handle = handle;
    gOverlay101OrderEntries[1 + 1].childType = 1;
    gOverlay101OrderEntries[1 + 1].child = node20;
    gOverlay101Resource20Count = index + 1;
    index = D_1D0;
    node24 = &D_540[index];
    node24->x = (0x70);
    node24->y = (0xA8);
    length = overlay101ByteLength(D_INPUT.textCC);
    index = D_1D0;
    node24 = &D_540[index];
    node24->length = (u8)length;
    node24->opacity =
        (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1);
    node24->mode = 2;
    node24->color0 = 0xFF;
    node24->color1 = 0xFF;
    node24->color2 = 0xFF;
    node24->color3 = 0xFF;
    node24->kind = 4;
    node24->text = D_INPUT.textCC;
    node24->previousType = gOverlay101OrderEntries[1 + 1].childType;
    node24->previous = gOverlay101OrderEntries[1 + 1].child;
    gOverlay101OrderEntries[1 + 1].childType = 3;
    gOverlay101OrderEntries[1 + 1].child = node24;
    D_1D0 = index + 1;
    ADD_TEXT(1, textD0, 0x70, 0xB2);

    INIT_GROUP(2, 0x20, 0x40, 0x62, 0x18, 0x7C, 0x84, dataD4, D_54);
    index = gOverlay101Resource20Count;
    node20 = &gOverlay101Resources20[index];
    node20->x = (0x16);
    node20->y = (0x1E);
    node20->scale = 1.0f;
    handle = func_80036DD0(0x10);
    index = gOverlay101Resource20Count;
    node20 = &gOverlay101Resources20[index];
    node20->previousType = gOverlay101OrderEntries[1 + 2].childType;
    node20->previous = gOverlay101OrderEntries[1 + 2].child;
    node20->handle = handle;
    gOverlay101OrderEntries[1 + 2].childType = 1;
    gOverlay101OrderEntries[1 + 2].child = node20;
    gOverlay101Resource20Count = index + 1;

    INIT_GROUP(3, 0x20, 0x68, 0x40, 0xA0, 0xC0, 0x26, dataD8, D_70);
    index = D_1D0;
    node24 = &D_540[index];
    node24->x = (0x60);
    node24->y = (0xE);
    length = overlay101ByteLength(D_INPUT.textDC);
    index = D_1D0;
    node24 = &D_540[index];
    node24->length = (u8)length;
    node24->opacity =
        (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1);
    node24->mode = 2;
    node24->color0 = 0xFF;
    node24->color1 = 0xFF;
    node24->color2 = 0xFF;
    node24->color3 = 0xFF;
    node24->kind = 4;
    node24->text = D_INPUT.textDC;
    node24->previousType = gOverlay101OrderEntries[1 + 3].childType;
    node24->previous = gOverlay101OrderEntries[1 + 3].child;
    gOverlay101OrderEntries[1 + 3].childType = 3;
    gOverlay101OrderEntries[1 + 3].child = node24;
    D_1D0 = index + 1;
    ADD_TEXT(3, textE0, 0x60, 0x18);

    INIT_GROUP(4, 0x20, 0x90, 0x2F, 0x10, 0xE2, 0xC0, dataE4, D_8C);
    index = gOverlay101Resource20Count;
    node20 = &gOverlay101Resources20[index];
    node20->x = (0xD);
    node20->y = (0xE);
    node20->scale = 1.0f;
    handle = func_80036DD0(0x14);
    index = gOverlay101Resource20Count;
    node20 = &gOverlay101Resources20[index];
    node20->previousType = gOverlay101OrderEntries[1 + 4].childType;
    node20->previous = gOverlay101OrderEntries[1 + 4].child;
    node20->handle = handle;
    gOverlay101OrderEntries[1 + 4].childType = 1;
    gOverlay101OrderEntries[1 + 4].child = node20;
    gOverlay101Resource20Count = index + 1;
    index = D_1D0;
    node24 = &D_540[index];
    node24->x = (0x71);
    node24->y = (0xA8);
    length = overlay101ByteLength(D_INPUT.textE8);
    index = D_1D0;
    node24 = &D_540[index];
    node24->length = (u8)length;
    node24->opacity =
        (s8)(s32)((f32)(u32)(length & 0xFF) * (f32)(s32)1);
    node24->mode = 2;
    node24->color0 = 0xFF;
    node24->color1 = 0xFF;
    node24->color2 = 0xFF;
    node24->color3 = 0xFF;
    node24->kind = 4;
    node24->text = D_INPUT.textE8;
    node24->previousType = gOverlay101OrderEntries[1 + 4].childType;
    node24->previous = gOverlay101OrderEntries[1 + 4].child;
    gOverlay101OrderEntries[1 + 4].childType = 3;
    gOverlay101OrderEntries[1 + 4].child = node24;
    D_1D0 = index + 1;
    ADD_TEXT(4, textEC, 0x71, 0xB2);

    INIT_GROUP(5, 0x90, -0x50, 0x30, 0x30, 0xE0, 0x8C, dataF0, D_A8);
    index = gOverlay101Resource20Count;
    node20 = &gOverlay101Resources20[index];
    node20->x = (0xC);
    node20->y = (0x12);
    node20->scale = 1.0f;
    handle = func_80036DD0(0xC);
    index = gOverlay101Resource20Count;
    node20 = &gOverlay101Resources20[index];
    node20->previousType = gOverlay101OrderEntries[1 + 5].childType;
    node20->previous = gOverlay101OrderEntries[1 + 5].child;
    node20->handle = handle;
    gOverlay101OrderEntries[1 + 5].childType = 1;
    gOverlay101OrderEntries[1 + 5].child = node20;
    gOverlay101Resource20Count = index + 1;
    ADD_TEXT(5, textF4, 0x70, 0x7B);

    overlay101Reset(&D_1D2C);

#undef ADD_TEXT
#undef ADD_NODE20
#undef ADD_NODE32
#undef INIT_GROUP
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o101/func_overlay_101_F00069E8_18E2208/func_overlay_101_F00069E8_18E2208.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_101_F00069E8_18E2208:start
 * symbol: func_overlay_101_F00069E8_18E2208
 * score: 613/963 words
 * frame: 0x50
 * relocations: 110
 * first-mismatch: +0x88
 * summary: 616 raw/613 masked; 81% register naming, one saved-register pair exchanged, scratch ring one slot behind from +0x88.
 * PLATEAU-HANDOFF:func_overlay_101_F00069E8_18E2208:end
 */
