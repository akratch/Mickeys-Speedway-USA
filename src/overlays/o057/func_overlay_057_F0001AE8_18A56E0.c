#include "PR/ultratypes.h"
#include "overlays/overlay_045.h"

extern s32 D_0;
extern void *D_8[];
extern void *D_2C[];
extern void *D_30[];
extern void *D_54[];
extern void *D_58[];
extern void *D_6C;
extern void *D_70;
extern void *D_74;
extern void *D_78;
extern void *D_80[];
extern void *D_E0[];
extern void *D_F8;
extern void *D_FC;
extern s32 D_100;
extern s32 D_104;
extern s32 D_108;
extern s32 D_10C;
extern s32 D_118;
extern s32 D_11C;
extern s32 D_128;
extern s32 D_130;
extern s32 D_134;
extern s32 D_138;
extern s32 D_144;
extern void *D_148;
extern s32 D_160;
extern s32 D_164;
extern s32 D_194;
extern s32 D_4F8;
extern s32 D_500;
extern s32 D_504;

extern void overlay57EaseAndLatch(s32 updateRate);
extern void overlay57SmoothAndCheckDistance(s32 updateRate);
extern void overlay57CheckDistance(s32 updateRate);
extern void overlay57UpdateTransition(s32 updateRate);
extern void overlay57UpdateSelection(s32 updateRate);
extern void overlay57UpdateModeState(s32 updateRate);
extern void overlay57BeginMode(s32 value);
extern void overlay57HandleModeInput(s32 updateRate);
extern void overlay57StartMode(s32 value);
extern void func_overlay_057_F0004460_18A8058(s32 updateRate);
extern void overlay57UpdateModeTrigger(s32 updateRate);
extern void overlay57InitializeMode(s32 value);
extern void func_overlay_057_F0004E18_18A8A10(s32 updateRate);
extern void func_overlay_057_F00060F8_18A9CF0(s32 updateRate);

extern void overlay57Draw32A0(s32 updateRate);
extern void overlay45ConfigureLayout(Overlay45ResourceDescriptor *descriptor,
                                      s32 x, s32 y, s32 flags);
extern void overlay45SetMode(Overlay45ResourceDescriptor *descriptor,
                             s32 mode);
extern void func_overlay_057_F0000000_18A3BF8();

/* Fresh workbench: structure-mismatch, 673 differing words, first mismatch
 * +0x00; 856 versus 883 instructions with the exact 0x38 frame. All 119 flag
 * rows are nonexact. A separate 24-entry bound plus sentinel loop improves the
 * prior 714-word result; later pointer-lifetime spellings are compiler-flat.
 * Promotion is rom-size (module -8 bytes, 689 in-range words); the runtime
 * relocation surface is target/candidate 359/331, 141 offset/type alignments,
 * 74 resolved candidate identities, and 38 aligned identities. */
#ifdef NON_MATCHING
void func_overlay_057_F0001AE8_18A56E0(s32 updateRate) {
    s32 mode;
    s32 count;
    s32 i;
    s32 transitionX;
    s32 transitionY;
    void **cursor;

    mode = D_118 - 1;
    D_130 = 0;
    switch (mode) {
        case 0:
            overlay57EaseAndLatch(updateRate);
            break;
        case 1:
            overlay57SmoothAndCheckDistance(updateRate);
            break;
        case 2:
            overlay57CheckDistance(updateRate);
            break;
        case 3:
            overlay57UpdateTransition(updateRate);
            break;
        case 4:
            overlay57UpdateSelection(updateRate);
            break;
        case 5:
            overlay57UpdateModeState(updateRate);
            break;
        case 6:
            overlay57BeginMode(updateRate);
            break;
        case 7:
            overlay57HandleModeInput(updateRate);
            break;
        case 8:
            overlay57StartMode(updateRate);
            break;
        case 9:
            func_overlay_057_F0004460_18A8058(updateRate);
            break;
        case 10:
            overlay57UpdateModeTrigger(updateRate);
            break;
        case 11:
            overlay57InitializeMode(updateRate);
            break;
        case 12:
            func_overlay_057_F0004E18_18A8A10(updateRate);
            break;
        case 13:
            func_overlay_057_F00060F8_18A9CF0(updateRate);
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        default:
            break;
    }

    overlay57Draw32A0(updateRate);

    if (D_11C == 1) {
        overlay45ConfigureLayout((Overlay45ResourceDescriptor *)D_0,
                                 0xA0, 0x20, 0x104);
        if (D_0 == 1) {
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_8[D_100],
                0xA0, 0xBE, 0x104);
        } else {
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_30[D_100],
                0xA0, 0xBE, 0x104);
        }

        cursor = D_8;
        do {
            overlay45SetMode((Overlay45ResourceDescriptor *)*cursor, 0);
            cursor++;
        } while (cursor < D_2C);
        cursor = D_30;
        do {
            overlay45SetMode((Overlay45ResourceDescriptor *)*cursor, 0);
            cursor++;
        } while (cursor < D_54);

        if (D_0 == 1) {
            overlay45SetMode((Overlay45ResourceDescriptor *)D_8[D_108], D_10C);
            overlay45SetMode((Overlay45ResourceDescriptor *)D_8[D_100], D_104);
        } else {
            overlay45SetMode((Overlay45ResourceDescriptor *)D_30[D_108], D_10C);
            overlay45SetMode((Overlay45ResourceDescriptor *)D_30[D_100], D_104);
        }

        cursor = D_80;
        do {
            overlay45SetMode((Overlay45ResourceDescriptor *)*cursor, 0);
            cursor++;
        } while (cursor < D_E0);
        cursor = D_E0;
        do {
            overlay45SetMode((Overlay45ResourceDescriptor *)*cursor, 0);
            cursor++;
        } while (cursor != (void **)D_F8);
    } else {
        overlay45ConfigureLayout((Overlay45ResourceDescriptor *)D_0,
                                 0xA0, -0x1E, 0x104);
        if (D_0 == 1) {
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_8[D_100],
                0xA0, 0x104, 0x104);
        } else {
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_30[D_100],
                0xA0, 0x104, 0x104);
        }
    }

    if ((D_11C == 2) && (D_118 == 10)) {
        if (D_194 == 1) {
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_80[D_160],
                0xA0, 0x20, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_FC,
                0xA0, -0x20, 0x104);
        } else {
            if (D_160 < 0x18) {
                overlay45ConfigureLayout(
                    (Overlay45ResourceDescriptor *)D_80[D_160],
                    0xA0, 0xBE, 0x104);
            }
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_FC,
                0xA0, -0x20, 0x104);
        }

        i = 0x18;
        for (count = 0; count != i; count++) {
            if ((count != D_108) && (count != D_100)) {
                overlay45SetMode(
                    (Overlay45ResourceDescriptor *)D_80[count], 0);
            }
        }
        overlay45SetMode((Overlay45ResourceDescriptor *)D_80[D_164], D_10C);
        overlay45SetMode((Overlay45ResourceDescriptor *)D_80[D_160], D_104);

        cursor = D_8;
        do {
            overlay45SetMode((Overlay45ResourceDescriptor *)*cursor, 0);
            cursor++;
        } while (cursor != D_2C);
    } else {
        if (D_160 < 0x18) {
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_E0[D_160],
                0xA0, 0x104, 0x104);
        }
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_FC,
            0xA0, -0x20, 0x104);
    }

    cursor = D_E0;
    if (D_11C == 6) {
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_E0[D_160],
            0xA0, 0xBE, 0x104);
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_FC,
            0xA0, 0x20, 0x104);
        for (i = 0; i < 6; i++) {
            if ((i != D_108) && (i != D_100)) {
                overlay45SetMode(
                    (Overlay45ResourceDescriptor *)cursor[i], 0);
            }
        }
        overlay45SetMode((Overlay45ResourceDescriptor *)cursor[D_164], D_10C);
        overlay45SetMode((Overlay45ResourceDescriptor *)cursor[D_160], D_104);

        cursor = D_8;
        do {
            overlay45SetMode((Overlay45ResourceDescriptor *)*cursor, 0);
            cursor++;
        } while (cursor != D_2C);
    } else if (D_160 < 6) {
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)cursor[D_160],
            0xA0, 0x104, 0x104);
    }

    if (D_11C == 3) {
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_F8, 0xA0, 0x1E, 0x104);
    } else {
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_F8, 0xA0, -0x1E, 0x104);
    }

    if (D_11C == 5) {
        switch ((u8)D_0) {
        case 0:
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_6C,
                0xA0, 0xBE, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_70,
                0xA0, 0x104, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_74,
                0xA0, 0x104, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_78,
                0xA0, 0x104, 0x104);
            break;
        case 1:
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_6C,
                0xA0, 0x104, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_70,
                0xA0, 0xBE, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_74,
                0xA0, 0x104, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_78,
                0xA0, 0x104, 0x104);
            break;
        case 2:
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_6C,
                0xA0, 0x104, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_70,
                0xA0, 0x104, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_74,
                0xA0, 0xBE, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_78,
                0xA0, 0x104, 0x104);
            break;
        case 3:
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_6C,
                0xA0, 0x104, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_70,
                0xA0, 0x104, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_74,
                0xA0, 0x104, 0x104);
            overlay45ConfigureLayout(
                (Overlay45ResourceDescriptor *)D_78,
                0xA0, 0xBE, 0x104);
            break;
        default:
            break;
        }
    } else if (D_4F8 < 5) {
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_58[D_4F8],
            0xA0, 0x20, 0x104);
    } else {
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_58[D_4F8],
            0xA0, -0x28, 0x104);
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_74,
            0xA0, 0x104, 0x104);
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_6C,
            0xA0, 0x104, 0x104);
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_70,
            0xA0, 0x104, 0x104);
        overlay45ConfigureLayout(
            (Overlay45ResourceDescriptor *)D_78,
            0xA0, 0x104, 0x104);
    }

    D_500 -= D_504 * updateRate;
    if (D_500 < 0) {
        D_500 = -D_500;
        D_504 = -D_504;
    } else if (D_500 >= 0x100) {
        D_500 = 0x1FE - D_500;
        D_504 = -D_504;
    }

    if ((D_11C == 0) || (D_11C == 3) || (D_11C == 4) ||
        (D_11C == 5) || ((D_11C == 2) && (D_194 == 1))) {
        D_128 -= updateRate << 2;
    } else {
        D_128 += updateRate << 2;
        if (D_128 > 0) {
            D_128 = 0;
        }
    }
    if (D_128 < -0x50) {
        D_128 = -0x50;
    }

    if (D_144 != 0) {
        for (i = 0; i < updateRate; i++) {
            D_134 += (0x3DE00 - D_134) >> 4;
            D_138 += (0x3E800 - D_138) >> 4;
        }
    } else {
        for (i = 0; i < updateRate; i++) {
            D_134 += (0x3FA00 - D_134) >> 6;
            D_138 += (0x3FF00 - D_138) >> 6;
        }
    }

    transitionX = (s16)(D_134 >> 8);
    transitionY = (s16)(D_138 >> 8);
    func_overlay_057_F0000000_18A3BF8(
        0, transitionX, transitionY, transitionX, 0, 0, 0, 0);
    if (D_130 != 0) {
        if (D_148 != NULL) {
            func_overlay_057_F0000000_18A3BF8(D_148);
        }
        func_overlay_057_F0000000_18A3BF8(0xF, &D_148);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o057/func_overlay_057_F0001AE8_18A56E0/func_overlay_057_F0001AE8_18A56E0.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_057_F0001AE8_18A56E0:start
 * symbol: func_overlay_057_F0001AE8_18A56E0
 * score: 673 differing words
 * frame: 0x38
 * relocations: 331
 * first-mismatch: +0x0
 * summary: Frame-exact 883/856; separate-bound sentinel loop improves 714 to 673. Promotion is module -8 bytes; relocs 331/359, 141 offset/type, 38 identities.
 * PLATEAU-HANDOFF:func_overlay_057_F0001AE8_18A56E0:end
 */
