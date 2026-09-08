#include "game/charControl.h"
#include "game/menu.h"
#include "n_audio/mbi.h"
#include "overlays/offset_records.h"
#include "overlays/overlay_045.h"

#ifdef NON_MATCHING
/* Tier B: overlay 54's runtime records identify the resident calls and the
 * overlay 45/56 exports below. Their source declarations establish the ABI.
 * These externs retain overlay-specific linkage until the body is exact. */
extern void camStandardOrtho_o054Reloc(MenuCommand **dlist, Mtx **matrix);
extern void camSetNo_o054Reloc(s32 camera);
extern void camSetScissor_o054Reloc(MenuCommand **dlist);
extern ControlActor **func_80005750_o054Reloc(s32 *count);
extern u8 *levelGetLevel_o054Reloc(void);
extern s32 func_800290A0_o054Reloc(void);
extern s32 func_8003A7D0_o054Reloc(ControlActor *actor);
extern void viGetCurrentSize_o054Reloc(u32 *width, u32 *height);
extern s32 func_80036544_o054Reloc(void *resource, s32 *state,
    s32 animation, f32 *frame, s32 updateRate);
extern void func_8002F618_o054Reloc(MenuCommand **dlist,
    OverlayOffsetRecord *records, s32 x, s32 y, u8 red, u8 green,
    u8 blue, u8 alpha);
extern void func_8002FB34_o054Reloc(MenuCommand **dlist,
    OverlayOffsetRecord *records, f32 x, f32 y, f32 scaleX, f32 scaleY,
    s32 colour, s32 mode);
extern u32 joyGetPressed_o054Reloc(s32 player);
extern void func_80034920_o054Reloc(MenuCommand **dlist);
extern void func_80034DE4_o054Reloc(s32 mode);
extern void func_80039E34_o054Reloc(s32 index);
extern s32 frontGetScreenMode_o054Reloc(void);
extern s32 mainGetMode_o054Reloc(void);
extern u8 *func_80028F54_o054Reloc(void);
extern void mainChangeCameras_o054Reloc(s32 cameras);
extern void func_800016EC_o054Reloc(u8 mode);
extern void func_8003A590_o054Reloc(void);
extern void func_80037414_o054Reloc(s32 kind, f32 duration, f32 delay,
    s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern void mainChangeLevel_o054Reloc(s32 level, s32 character,
    s32 animation, s32 mode, s32 arg4, s32 arg5);
extern void func_800005CC_o054Reloc(f32 fade, u8 volume);
extern void amSndPlay_o054Reloc(s32 sound, s32 *handle);
extern void overlay45ReleaseDescriptor_o054Reloc(Overlay45ResourceDescriptor *descriptor);
extern void overlay45SetMode_o054Reloc(Overlay45ResourceDescriptor *descriptor, s32 value);
extern void overlay56SplitTime_o054Reloc(s32 time, s32 *minutes,
    s32 *seconds, s32 *centiseconds);
extern void overlay54GetOffsets(s32 player, s32 kind, s32 *x, s32 *y);

/* The current-object layout is shared with src/main/menu.c. */
typedef struct O54MenuObject {
    s16 rotationX;
    s16 rotationY;
    s16 rotationZ;
    s16 index;
    f32 scale;
    f32 x;
    f32 y;
    f32 z;
    f32 frame;
    s8 flags[4];
} O54MenuObject;

extern MenuCommand *D_800D3140_o054Reloc;
extern Mtx *D_800D3144_o054Reloc;
extern void *D_800D31C8_o054Reloc[];
extern O54MenuObject D_800D3550_o054Reloc[];
extern u8 D_8007BEF4_o054Reloc;
extern s32 D_800C947C_o054Reloc;
extern s16 D_8007C180_o054Reloc[];
extern s32 D_8007C1B0_o054Reloc;
extern s32 o001_data_83E0_o054Reloc;

/* Tier B: LOCAL records distinguish the initialized templates and state
 * from the writable HUD copies in BSS. Names retain their section offsets. */
extern OverlayOffsetRecord o54Bss_0[];
extern OverlayOffsetRecord o54Bss_10[];
extern OverlayOffsetRecord o54Bss_90[];
extern OverlayOffsetRecord o54Bss_A0[];
extern OverlayOffsetRecord o54Bss_C0[];
extern OverlayOffsetRecord o54Bss_140[];
extern OverlayOffsetRecord o54Bss_1C0[];
extern OverlayOffsetRecord o54Bss_280[];
extern OverlayOffsetRecord o54Bss_340[];
extern OverlayOffsetRecord o54Bss_5C0[];
extern s16 o54Bss_640[];
extern s16 o54Bss_648[];
extern s8 o54Bss_654[];
extern f32 o54Bss_658;
extern f32 o54Bss_65C;
extern s16 o54Bss_660[];
extern Overlay45ResourceDescriptor *o54Bss_668;
extern s16 o54Bss_66C;
extern s16 o54Bss_66E;
extern s16 o54Data_78;
extern s16 o54Data_88;
extern OverlayOffsetRecord o54Data_CC[];
extern OverlayOffsetRecord o54Data_FC[];
extern OverlayOffsetRecord o54Data_12C[];
extern OverlayOffsetRecord o54Data_14C[];
extern s16 o54Data_18C[];
extern s16 o54Data_190[];
extern s16 o54Data_194[];
extern OverlayOffsetRecord o54Data_1E8[];
extern OverlayOffsetRecord o54Data_218[];
extern OverlayOffsetRecord o54Data_248[];
extern OverlayOffsetRecord o54Data_268[];
extern OverlayOffsetRecord o54Data_278[];
extern s32 o54Data_298[];
extern s32 o54Data_2A8;
extern s32 o54Data_2AC;
extern s8 o54Data_2B0;
extern s32 o54Data_2B4[];

/* These fields fall in gaps in the partial ControlPlayer header. Their
 * signedness and widths come from this function's own loads. */
#define O54_LAP(p) ((s8) (p)->pad37C[7])
#define O54_PLACE(p) ((p)->pad37C[9])
#define O54_TIME(p) (*(s32 *) ((p)->pad3FC + 4))
#define O54_TIME_DELTA(p) (*(s16 *) (p)->pad454)

void func_overlay_054_F00005AC_189F24C(s32 updateRate) {
    s32 playerIndex;
    s32 xOffset;
    s32 yOffset;
    s8 *level;
    ControlActor *actor;
    s32 actorCount;
    s32 screenY;
    s32 minutes;
    s32 seconds;
    s32 centiseconds;
    s32 hudY;
    f32 step;
    OverlayOffsetRecord icon[2];
    u32 width;
    u32 height;
    s32 i;
    s32 value;
    s32 x;
    s32 y;
    s32 visible;
    s32 side;
    s32 buttons;
    s32 texture;
    s32 alternate;
    s16 enterX;
    s16 hiddenX;
    s16 leaveX;
    s16 resetX;
    s16 deltaTime;
    ControlActor **actors;
    ControlPlayer *player;
    OverlayOffsetRecord *position;
    OverlayOffsetRecord *lap;
    OverlayOffsetRecord *lapCount;
    OverlayOffsetRecord *timer;
    OverlayOffsetRecord *src;
    OverlayOffsetRecord *dst;
    s32 *displayMode;
    s32 *alpha;
    s8 *item;

    camStandardOrtho_o054Reloc(&D_800D3140_o054Reloc, &D_800D3144_o054Reloc);
    if (o54Bss_668 != NULL) {
        o54Bss_66C += updateRate;
        if (o54Bss_66C >= 61) {
            if (o54Bss_66C >= 241) {
                o54Bss_66E -= updateRate * 4;
                if (o54Bss_66E < 0) {
                    overlay45ReleaseDescriptor_o054Reloc(o54Bss_668);
                    o54Bss_668 = NULL;
                } else {
                    overlay45SetMode_o054Reloc(o54Bss_668, o54Bss_66E);
                }
            } else {
                o54Bss_66E += updateRate * 4;
                if (o54Bss_66E >= 256) {
                    o54Bss_66E = 255;
                }
                overlay45SetMode_o054Reloc(o54Bss_668, o54Bss_66E);
            }
        }
    }
    actors = func_80005750_o054Reloc(&actorCount);
    if (D_800C947C_o054Reloc == 0) {
        for (i = 0; i < updateRate; i++) {
            step = (-11.0f - o54Bss_658) * 0.125f;
            o54Bss_658 += step;
        }
    }
    func_80036544_o054Reloc(D_800D31C8_o054Reloc[2], &o54Data_2A8,
        20, &D_800D3550_o054Reloc[2].frame, updateRate);
    func_80036544_o054Reloc(D_800D31C8_o054Reloc[40], &o54Data_2A8,
        20, &D_800D3550_o054Reloc[1].frame, updateRate);
    hudY = (s32) o54Bss_658;
    viGetCurrentSize_o054Reloc(&width, &height);
    o54Data_2B0++;
    o54Data_2B0 %= 10;

    playerIndex = 0;
    while (playerIndex < D_8007BEF4_o054Reloc) {
        actor = actors[playerIndex];
        if (actor == NULL) {
            return;
        }
        player = actor->player;
        position = &o54Bss_C0[playerIndex * 2];
        lap = &o54Bss_1C0[playerIndex * 3];
        lapCount = &o54Bss_280[playerIndex * 3];
        timer = &o54Bss_340[playerIndex * 10];
        displayMode = &o54Data_298[playerIndex];
        alpha = &o54Data_2B4[playerIndex];
        item = &o54Bss_654[playerIndex];
        camSetNo_o054Reloc(playerIndex);
        camSetScissor_o054Reloc(&D_800D3140_o054Reloc);
        value = O54_LAP(player) + 1;
        if (player->unk45C != 0) {
            value++;
        }
        if (value >= 4) {
            value = 3;
        }
        if (value <= 0) {
            value = 1;
        }
        lapCount[1].metadata = value << 16;
        lap[0].metadata = (player->unk192 / 10) << 16;
        lap[1].metadata = (player->unk192 % 10) << 16;
        lap[2].metadata = (s32) (o54Bss_65C * 65536.0f);
        overlay54GetOffsets(playerIndex, 1, &xOffset, &yOffset);
        if ((lap[0].metadata >> 16) == 1) {
            lap[0].x = o54Data_78 + xOffset + 1;
        } else {
            lap[0].x = o54Data_78 + xOffset;
        }
        if ((lap[1].metadata >> 16) == 1) {
            lap[1].x = o54Data_88 + xOffset - 1;
        } else {
            lap[1].x = o54Data_88 + xOffset;
        }
        if (player->unk3BA != 255) {
            position[0].metadata = player->unk3BA * 65536;
        } else {
            position[0].metadata = O54_PLACE(player) << 16;
        }
        overlay56SplitTime_o054Reloc(O54_TIME(player), &minutes, &seconds, &centiseconds);
        level = (s8 *) levelGetLevel_o054Reloc();
        if (D_800C947C_o054Reloc == 0 && level[0x86] != O54_LAP(player) &&
            func_800290A0_o054Reloc() == 0 &&
            func_8003A7D0_o054Reloc(actor) != O54_TIME(player)) {
            centiseconds = centiseconds - centiseconds % 10 + o54Data_2B0;
        }
        timer[0].metadata = (minutes / 10) * 65536;
        timer[1].metadata = (minutes % 10) * 65536;
        timer[3].metadata = (seconds / 10) * 65536;
        timer[4].metadata = (seconds % 10) * 65536;
        timer[6].metadata = (centiseconds / 10) * 65536;
        timer[7].metadata = (centiseconds % 10) * 65536;
        overlay54GetOffsets(playerIndex, 3, &xOffset, &yOffset);
        src = o54Data_CC;
        dst = timer;
        do {
            if ((dst->metadata >> 16) == 1) {
                if (src == o54Data_CC || src == o54Data_FC || src == o54Data_12C) {
                    dst->x = src->x + xOffset + 1;
                } else {
                    dst->x = src->x + xOffset - 1;
                }
            } else {
                dst->x = src->x + xOffset;
            }
            src++;
            dst++;
        } while ((u32) src < (u32) o54Data_14C);
        if (player->flags1A8 & 8) {
            func_8002F618_o054Reloc(&D_800D3140_o054Reloc,
                &o54Bss_140[playerIndex * 2], 0, hudY, 255, 255, 255, 255);
        } else {
            func_8002F618_o054Reloc(&D_800D3140_o054Reloc,
                position, 0, hudY, 255, 255, 255, 255);
        }
        if (joyGetPressed_o054Reloc(playerIndex) & 2) {
            (*displayMode)++;
            if (*displayMode >= 2) {
                *displayMode = 0;
            }
        }
        func_80034920_o054Reloc(&D_800D3140_o054Reloc);
        switch (*displayMode) {
        case 0:
            func_8002F618_o054Reloc(&D_800D3140_o054Reloc, lapCount, 0, hudY, 255, 255, 255, 255);
            func_8002F618_o054Reloc(&D_800D3140_o054Reloc, lap, 0, hudY, 255, 255, 255, 255);
            overlay54GetOffsets(playerIndex, 1, &xOffset, &yOffset);
            D_800D3550_o054Reloc[1].x = xOffset - 152;
            D_800D3550_o054Reloc[1].y = -yOffset - hudY + 108;
            func_80034DE4_o054Reloc(0);
            func_80039E34_o054Reloc(1);
            func_80034DE4_o054Reloc(1);
            break;
        case 1:
            func_8002F618_o054Reloc(&D_800D3140_o054Reloc, timer, 0, hudY, 255, 255, 255, 255);
            func_80034920_o054Reloc(&D_800D3140_o054Reloc);
            overlay54GetOffsets(playerIndex, 3, &xOffset, &yOffset);
            D_800D3550_o054Reloc[4].x = xOffset - 173;
            D_800D3550_o054Reloc[4].y = -yOffset - hudY + 116;
            D_800D3550_o054Reloc[4].rotationZ = (s32) ((u32) O54_TIME(player) * (u32) -65536) / 300;
            func_80039E34_o054Reloc(4);
            func_8002F618_o054Reloc(&D_800D3140_o054Reloc,
                &o54Bss_5C0[playerIndex * 2], 0, hudY, 255, 255, 255, 255);
            break;
        }
        if (player->unk19A != 255) {
            *alpha += updateRate * 16;
            if (*alpha >= 165) {
                *alpha = 164;
            }
        } else {
            *alpha -= updateRate * 8;
            if (*alpha < 0) {
                *alpha = 0;
            }
        }
        if (*alpha > 0) {
            if (player->unk19C != 0) {
                *item = 53;
            } else if (player->unk19A != 255) {
                *item = D_8007C180_o054Reloc[player->unk19A];
            }
            if (*item != -1) {
                if (frontGetScreenMode_o054Reloc() == 1) {
                    x = (playerIndex & 1) ? 276 : 25;
                    y = (playerIndex & 2) ? 209 : 89;
                } else {
                    x = (playerIndex & 1) ? 276 : 25;
                    y = (playerIndex & 2) ? 197 : 89;
                }
                if (*item == 53) {
                    x -= 7;
                    y -= 6;
                }
                icon[0].link = (s32) D_800D31C8_o054Reloc[*item];
                icon[0].value = 0;
                icon[0].metadata = 0;
                icon[0].x = 0;
                icon[0].y = 0;
                icon[1].link = 0;
                func_8002FB34_o054Reloc(&D_800D3140_o054Reloc,
                    icon, (f32) x, (f32) y, 0.66f, 0.66f, *alpha | ~255, 1);
                if (*item != 53 && player->unk19B >= 2) {
                    o54Data_278[0].metadata = player->unk19B << 16;
                    func_8002F618_o054Reloc(&D_800D3140_o054Reloc, o54Data_278,
                        x + 18, y + 18, 0, 0, 0, *alpha);
                    func_8002F618_o054Reloc(&D_800D3140_o054Reloc, o54Data_278,
                        x + 20, y + 20, 0, 0, 0, *alpha);
                    func_8002F618_o054Reloc(&D_800D3140_o054Reloc, o54Data_278,
                        x + 19, y + 19, 255, 255, 255, 255);
                }
            }
        } else {
            *item = -1;
        }
        side = playerIndex & 1;
        enterX = o54Data_18C[side];
        hiddenX = o54Data_190[side];
        leaveX = o54Data_194[side];
        if (player->unk388 != 0) {
            for (i = 0; i < updateRate; i++) {
                o54Bss_660[playerIndex] += (enterX - o54Bss_660[playerIndex]) >> 3;
            }
            visible = 1;
        } else if (hiddenX == o54Bss_660[playerIndex]) {
            visible = 0;
        } else {
            for (i = 0; i < updateRate; i++) {
                o54Bss_660[playerIndex] += (leaveX - o54Bss_660[playerIndex]) >> 3;
            }
            visible = 0;
            if ((leaveX >> 6) == (o54Bss_660[playerIndex] >> 6)) {
                o54Bss_660[playerIndex] = hiddenX;
            } else {
                visible = 1;
            }
        }
        if (visible) {
            o54Bss_A0[0].x = o54Bss_660[playerIndex] >> 4;
            if (frontGetScreenMode_o054Reloc() == 1) {
                o54Bss_A0[0].y = playerIndex < 2 ? 40 : 160;
            } else {
                o54Bss_A0[0].y = playerIndex < 2 ? 45 : 153;
            }
            func_8002F618_o054Reloc(&D_800D3140_o054Reloc, o54Bss_A0,
                0, 0, 255, 255, 255, 192);
        }
        x = side ? 2560 : 256;
        if (frontGetScreenMode_o054Reloc() == 1) {
            screenY = (height >> 1) * ((player->playerIndex >> 1) * 16);
        } else {
            screenY = playerIndex < 2 ? 192 : 1920;
        }
        if (O54_LAP(player) < level[0x86]) {
            if (player->unk456 >= updateRate) {
                player->unk456 -= updateRate;
                for (i = 0; i < updateRate; i++) {
                    o54Bss_640[player->playerIndex] +=
                        (x - o54Bss_640[player->playerIndex] + 816) >> 3;
                    o54Bss_648[player->playerIndex] +=
                        (screenY - o54Bss_648[player->playerIndex] + 560) >> 3;
                }
            } else {
                resetX = x - 1280;
                if (resetX != o54Bss_640[player->playerIndex]) {
                    if (player->unk456 != -1 && O54_TIME_DELTA(player) < 0) {
                        amSndPlay_o054Reloc(505, NULL);
                        player->unk456 = -1;
                    }
                    for (i = 0; i < updateRate; i++) {
                        o54Bss_640[player->playerIndex] +=
                            (width * 8 + x - o54Bss_640[player->playerIndex] + 816) >> 3;
                        o54Bss_648[player->playerIndex] +=
                            (screenY - o54Bss_648[player->playerIndex] - 400) >> 3;
                    }
                    if (x + width * 8 + 656 < (u32) o54Bss_640[player->playerIndex]) {
                        o54Bss_640[player->playerIndex] = resetX;
                        o54Bss_648[player->playerIndex] = screenY - 320;
                    }
                }
            }
            if (O54_TIME_DELTA(player) <= 0) {
                o54Bss_0[0].metadata = 12 << 16;
                texture = (s32) D_800D31C8_o054Reloc[20];
                alternate = (s32) D_800D31C8_o054Reloc[21];
                o54Bss_0[0].link = texture;
                o54Bss_0[0].value = alternate;
                deltaTime = -O54_TIME_DELTA(player);
                for (dst = o54Bss_10; dst != o54Bss_90; dst++) {
                    dst->link = texture;
                    dst->value = alternate;
                }
            } else {
                o54Bss_0[0].metadata = 13 << 16;
                alternate = (s32) D_800D31C8_o054Reloc[21];
                texture = (s32) D_800D31C8_o054Reloc[80];
                o54Bss_0[0].value = alternate;
                o54Bss_0[0].link = texture;
                deltaTime = O54_TIME_DELTA(player);
                for (dst = o54Bss_10; dst != o54Bss_90; dst++) {
                    dst->link = texture;
                    dst->value = alternate;
                }
            }
            overlay56SplitTime_o054Reloc(deltaTime, &minutes, &seconds, &centiseconds);
            o54Bss_0[1].metadata = (minutes / 10) * 65536;
            o54Bss_0[2].metadata = (minutes % 10) * 65536;
            o54Bss_0[4].metadata = (seconds / 10) * 65536;
            o54Bss_0[5].metadata = (seconds % 10) * 65536;
            o54Bss_0[7].metadata = (centiseconds / 10) * 65536;
            o54Bss_0[8].metadata = (centiseconds % 10) * 65536;
            src = o54Data_1E8;
            dst = o54Bss_10;
            do {
                if ((dst->metadata >> 16) == 1) {
                    if (src == o54Data_1E8 || src == o54Data_218 || src == o54Data_248) {
                        dst->x = src->x + 1;
                    } else {
                        dst->x = src->x - 1;
                    }
                } else {
                    dst->x = src->x;
                }
                src++;
                dst++;
            } while (src != o54Data_268);
            func_8002F618_o054Reloc(&D_800D3140_o054Reloc, o54Bss_0,
                o54Bss_640[player->playerIndex] >> 4,
                o54Bss_648[player->playerIndex] >> 4, 255, 255, 255, 255);
        }
        if (joyGetPressed_o054Reloc(playerIndex) & 1) {
            D_8007C1B0_o054Reloc ^= 1;
        }
        if (mainGetMode_o054Reloc() == 0) {
            buttons = 0;
            for (i = 0; i < D_8007BEF4_o054Reloc; i++) {
                buttons |= joyGetPressed_o054Reloc(i) & 0x9000;
            }
            switch (*func_80028F54_o054Reloc()) {
            case 3:
                if (o001_data_83E0_o054Reloc == 0 && buttons != 0 && o54Data_2AC == 0) {
                    mainChangeCameras_o054Reloc(1);
                    func_800016EC_o054Reloc(1);
                    func_8003A590_o054Reloc();
                    func_80037414_o054Reloc(2, 4.0f, -1.0f, 0, 0, 0, 0);
                    mainChangeLevel_o054Reloc(18, 0, 0, 7, 1, 1);
                    func_800005CC_o054Reloc(3.0f, 0);
                    o54Data_2AC = 1;
                }
                break;
            case 4:
                if (o001_data_83E0_o054Reloc == 0 && buttons != 0 && o54Data_2AC == 0) {
                    mainChangeCameras_o054Reloc(1);
                    func_800016EC_o054Reloc(1);
                    func_8003A590_o054Reloc();
                    func_80037414_o054Reloc(2, 4.0f, -1.0f, 0, 0, 0, 0);
                    mainChangeLevel_o054Reloc(18, 0, 0, 7, 1, 1);
                    func_800005CC_o054Reloc(3.0f, 0);
                    o54Data_2AC = 1;
                }
                break;
            }
        }
        playerIndex++;
    }
    camSetNo_o054Reloc(0);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o054/overlay54TailA/func_overlay_054_F00005AC_189F24C.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_054_F00005AC_189F24C:start
 * symbol: func_overlay_054_F00005AC_189F24C
 * score: 1400 differing words
 * frame: 0x158
 * relocations: 281
 * first-mismatch: +0x0
 * summary: Checkpoint; packet continues. Exact size and 59 ordered calls. Target frame 0x138; 71/269 exact relocation sites. Easing/copy regions offset.
 * PLATEAU-HANDOFF:func_overlay_054_F00005AC_189F24C:end
 */
