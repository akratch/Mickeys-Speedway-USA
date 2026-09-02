#include "ultra64.h"
#include "overlays/overlay_045.h"

extern char * volatile D_0[];
extern u32 D_8[];
extern s32 D_28[];
extern f32 D_2C;
extern u32 D_30[];
extern s32 D_40[];
extern s32 D_50[];
extern f32 D_54;
extern Overlay45ResourceDescriptor *D_58;
extern Overlay45ResourceDescriptor *D_5C;
extern Overlay45ResourceDescriptor *D_60;
extern Overlay45ResourceDescriptor *D_64;
extern Overlay45ResourceDescriptor *D_68;
extern Overlay45ResourceDescriptor *D_6C;
extern Overlay45ResourceDescriptor *D_70;
extern Overlay45ResourceDescriptor *D_74;
extern Overlay45ResourceDescriptor *D_78;
extern u32 D_7C;
extern u32 D_80[];
extern Overlay45ResourceDescriptor *D_84;
extern Overlay45ResourceDescriptor *D_88;
extern s32 D_94;
extern s32 D_98;
extern s32 D_9C;
extern s32 D_A4;
extern s32 D_A8;
extern s32 D_AC;
extern s32 D_B0;
extern u8 D_C0[];
extern u8 D_D0[];
extern s32 D_150[];
extern s32 D_15C[];
extern u8 D_5D0[];

extern void overlay61UpdateInput(s32 *, s32 *, s32 *, s32 *);
extern void overlay61ResetCounters(void);
extern void overlay61AddEntry(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 overlay61ReadCharacter(s32, s32, s32 *, s32 *, s32 *);
extern s32 overlay61RecordSize(void *);
extern void overlay61ChooseFileExtension(char *);
extern s32 overlay61WriteCharacter(s32 *, s32, s32);
extern void overlay61DrawList(s32);
extern void overlay45SetMode(Overlay45ResourceDescriptor *, s32);
extern s32 packDirectory(s32, s32, char **, char **, u32 *, u8 *);
extern s32 packFreeSpace(s32, u32 *, s32 *);
extern s32 packDeleteFile(s32, s32);
extern void amSndPlay(u16, void **);
extern void mainTitlePageInit(s32);
extern void func_800367A4(u8 *, s32 *, s32, f32 *, s32);
extern void func_8002F618(void **, void *, s32, s32, u8, u8, u8, u8);
extern s32 func_overlay_061_F0001648_18C0A10(s32 *, s32, s32 *);

/* promotion_trial: build-error, 0 words; schedule divergence at func_8002F618_o061Reloc/func_800367A4_o061Reloc (no linked text).
 * workbench: structure-mismatch, 476 differing words, first mismatch +0x0; 621 candidate instructions versus 637 target, frame 0x78 versus 0x70.
 * Structural gap: 16 instructions plus unresolved branch/relocation layout remain; not shape-exact. */
#ifdef NON_MATCHING
void func_overlay_061_F0000B84_18BFF4C(s32 arg0) {
    s32 x;
    s32 y;
    s32 confirm;
    s32 cancel;
    s32 marker0;
    s32 marker1;
    s32 i;
    s32 output0;
    s32 output1;
    s32 output2;
    s32 size;
    s32 result;
    u8 *entry;

    overlay61UpdateInput(&x, &y, &confirm, &cancel);
    marker0 = 0;
    marker1 = 0;

    overlay45SetMode(D_58, 0xFF);
    overlay45SetMode(D_5C, 0);
    overlay45SetMode(D_60, 0);
    overlay45SetMode(D_64, 0);
    overlay45SetMode(D_68, 0);
    overlay45SetMode(D_6C, 0);
    overlay45SetMode(D_70, 0);
    overlay45SetMode(D_74, 0);
    overlay45SetMode(D_78, 0);
    overlay45SetMode((Overlay45ResourceDescriptor *)D_7C, 0);
    overlay45SetMode((Overlay45ResourceDescriptor *)D_80[0], 0);
    overlay45SetMode(D_84, 0);
    overlay45SetMode(D_88, 0);

    switch (D_A4) {
        case 0:
            if ((D_A8 == 1) || (D_A8 == 8)) {
                overlay45SetMode(D_6C, 0xFF);
            } else if (D_A8 == 4) {
                overlay45SetMode(D_74, 0xFF);
            } else {
                overlay45SetMode(D_70, 0xFF);
            }
            overlay45SetMode((Overlay45ResourceDescriptor *)D_80[0], 0xFF);
            overlay45SetMode(D_84, 0xFF);
            marker0 = 0xFF;
            marker1 = 0xFF;
            if (confirm != 0) {
                D_A4 = D_AC;
                amSndPlay(0xC, (void **)0);
            } else if (cancel != 0) {
                D_A4 = D_B0;
                amSndPlay(0xD, (void **)0);
            }
            break;

        case 1:
            overlay45SetMode(D_68, 0xFF);
            marker0 = 0xFF;
            marker1 = 0xFF;
            if ((confirm != 0) || (cancel != 0)) {
                D_A4 = 2;
                amSndPlay(0xC, (void **)0);
            }
            break;

        case 2:
            result = packDirectory(0, 0x10, (char **)D_0, (char **)D_40,
                                    (u32 *)D_80, D_C0);
            if (result != 0) {
                D_A4 = 0;
                D_A8 = result;
                D_AC = 2;
                D_B0 = 7;
            } else {
                overlay61ResetCounters();
                entry = D_C0;
                for (i = 0; i < 0x10; i++) {
                    if ((*entry == 0) && (D_0[0] != 0)) {
                        result = overlay61ReadCharacter(
                            0, D_40[i], &output0, &output1, &output2);
                        if (result == 0) {
                            overlay61AddEntry(
                                4, i, (s32)D_0[i], D_40[i], D_80[i],
                                output0, output1, output2);
                        } else {
                            overlay61AddEntry(
                                6, i, (s32)D_0[i], D_40[i], D_80[i],
                                0, 0, 0);
                        }
                    } else if (*entry != 0xFF) {
                        overlay61AddEntry(
                            6, i, (s32)D_0[i], D_40[i], D_80[i],
                            0, 0, 0);
                    }
                    entry++;
                }

                if ((D_0[0] != 0) &&
                    (*(s16 *)((u8 *)D_0[0] + 6) != -1)) {
                    size = overlay61RecordSize(D_0[0]) + 4;
                    if ((size & 0xFF) != 0) {
                        size = (size - (size & 0xFF)) + 0x100;
                    }
                    overlay61AddEntry(
                        5, 0, (s32)D_150, 0, size,
                        ((s8 *)D_0[0])[4],
                        *(s16 *)((u8 *)D_0[0] + 6),
                        *(s16 *)((u8 *)D_0[0] + 8));
                }

                packFreeSpace(0, (u32 *)&D_94, (s32 *)0);
                overlay61AddEntry(
                    7, 0, (s32)D_15C, 0, D_94, 0, 0, 0);
                D_A4 = 3;
            }
            break;

        case 3:
            if (cancel != 0) {
                D_A4 = 7;
                amSndPlay(0xD, (void **)0);
            } else if (confirm != 0) {
                amSndPlay(0xC, (void **)0);
                D_A4 = *(s16 *)(D_D0 + (D_9C << 6) + 0xD0);
            } else if (y > 0) {
                D_9C--;
                if (D_9C < 0) {
                    D_9C = D_98 - 1;
                }
                amSndPlay(0xF, (void **)0);
            } else if (y < 0) {
                D_9C++;
                if (D_9C >= D_98) {
                    D_9C = 0;
                }
                amSndPlay(0xF, (void **)0);
            }
            break;

        case 4:
            overlay45SetMode(D_5C, 0xFF);
            overlay45SetMode(D_78, 0xFF);
            overlay45SetMode((Overlay45ResourceDescriptor *)D_7C, 0xFF);
            marker0 = 0xFF;
            marker1 = 0xFF;
            if (cancel != 0) {
                D_A4 = 6;
                amSndPlay(0xD, (void **)0);
            } else if (confirm != 0) {
                result = func_overlay_061_F0001648_18C0A10(
                    (s32 *)D_0[0], 0,
                    (s32 *)(D_D0 + (D_9C << 6) + 0x38));
                if (result != 0) {
                    D_A4 = 0;
                    D_A8 = result;
                    D_AC = 4;
                    D_B0 = 2;
                } else {
                    D_A4 = 1;
                }
            }
            break;

        case 5:
            overlay45SetMode(D_60, 0xFF);
            overlay45SetMode(D_78, 0xFF);
            overlay45SetMode((Overlay45ResourceDescriptor *)D_7C, 0xFF);
            marker0 = 0xFF;
            marker1 = 0xFF;
            if (cancel != 0) {
                D_A4 = 2;
            } else if (confirm != 0) {
                overlay61ChooseFileExtension((char *)D_5D0);
                result = overlay61WriteCharacter(
                    (s32 *)D_0[0], 0, (s32)D_5D0);
                if (result != 0) {
                    D_A4 = 0;
                    D_A8 = result;
                    D_AC = 5;
                    D_B0 = 2;
                } else {
                    D_A4 = 1;
                }
            }
            break;

        case 6:
            overlay45SetMode(D_64, 0xFF);
            overlay45SetMode(D_78, 0xFF);
            overlay45SetMode((Overlay45ResourceDescriptor *)D_7C, 0xFF);
            marker0 = 0xFF;
            marker1 = 0xFF;
            if (cancel != 0) {
                D_A4 = 2;
            } else if (confirm != 0) {
                amSndPlay(0xC, (void **)0);
                result = packDeleteFile(
                    0, *(u8 *)(D_D0 + (D_9C << 6) + 0xD2));
                if (result != 0) {
                    D_A4 = 0;
                    D_A8 = result;
                    D_AC = 6;
                    D_B0 = 2;
                } else {
                    D_A4 = 1;
                }
            }
            break;

        case 7:
            mainTitlePageInit(0);
            D_A4 = 8;
            break;

        case 8:
            break;
    }

    if (D_98 > 0) {
        overlay61DrawList((s32)D_0[0]);
    }
    func_800367A4((u8 *)D_8[0], D_28, 2, &D_2C, arg0);
    D_8[2] = (s32)(D_2C * 65536.0f);
    func_800367A4((u8 *)D_30[0], D_50, 2, &D_54, arg0);
    if (marker0 != 0) {
        func_8002F618((void **)0, (void *)D_8, 0x24, 0xCC,
                      0xFF, 0xFF, 0xFF, marker0);
    }
    if (marker1 != 0) {
        func_8002F618((void **)0, (void *)D_30, 0x11C, 0xCC,
                      0xFF, 0xFF, 0xFF, marker1);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o061/overlay_061_tail/func_overlay_061_F0000B84_18BFF4C.s")
#endif
