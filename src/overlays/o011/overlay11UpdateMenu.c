#include "PR/ultratypes.h"

typedef struct O11Status {
    u8 value0;
    u8 value1;
    u8 value2;
    u8 mode;
} O11Status;

typedef struct O11ObjectSub {
    u8 pad0;
    s8 value1;
} O11ObjectSub;

typedef struct O11Object {
    u8 pad00[0x64];
    O11ObjectSub *sub64;
} O11Object;

extern s8 D_0[];
extern u8 D_menuBase[];
extern s16 D_1B8;
extern s32 D_1BC;
extern s32 D_1C4;
extern void *D_1CC[3];
extern s32 D_204;

extern s8 D_cfgA;
extern s8 D_cfgB;
extern s8 D_cfgC;
extern u16 D_flags;
extern s32 D_count;
extern s16 D_table[][4];
extern s32 D_paramA;
extern s32 D_paramB;
extern s32 D_paramC;
extern s32 D_paramD;
extern s32 D_modeFlag;

extern O11Status *func_80028F54(void);
extern void func_overlay_011_F0001058_18698A0(s32 arg0);
extern void func_overlay_011_F0001130_1869978(s32 arg0);
extern void func_overlay_011_F0002948_186B190(void);
extern void func_overlay_011_F0002BF4_186B43C(void);
extern void func_80000F94(s32 soundId, void *handle);
extern u32 func_8002554C(s32 controller);
extern O11Object *func_80005820(s32 controller);
extern void func_80028374(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                         s32 arg5);
extern void func_80028528(s32 arg0);
extern void func_8003A754(void);
extern void func_800290AC(s32 arg0);
extern void func_800291D8(s32 arg0);
extern void func_800006BC(f32 arg0, s32 arg1);
extern void func_overlay_045_F0001BF4_188E04C(void *handle, s32 value);
extern void overlay66Select(s32 selection);

/* Pinned exact-overlay DKR v77/v80 and JFG scans found no exact donor. */
/*
 * The 2026-08-29 bounded reproof exhausted 122 stock builds plus one
 * codegen-faithful allocator trace. Clean V0 emitted 301 instructions with a
 * 0x30 frame and 23 relocation-masked positional differences. All 119 flag
 * configurations were nonexact (53 compiled; seven tied V0; 66 failed). The
 * trace found identical uopt pool and ugen temporary lanes. An explicit
 * current-handle form regressed to 30 differences. Keeping the three loop
 * locals in their natural lexical block is the sole strict gain: 301
 * instructions, exact 0x48 frame, 33 raw target-object differences and 19
 * after masking linker-filled fields (workbench norm=4, regs=2), first +0x1C.
 * The historical +0x138/+0x140 v0/v1 pair remains, but the removed source's
 * 299/301 runtime-normalized score does not transfer to this clean body.
 *
 * A 2026-09-01 identity pass names the Overlay 66 +0 call as overlay66Select,
 * closing all 102 relocation offsets, types, identities, and addends. Moving
 * the existing index and semantic locals to their natural declaration homes,
 * while narrowing action to the input block, restores every target stack home
 * and improves the candidate to 299/301 words. Only the two spill stores at
 * +0x138/+0x140 remain reversed around the call. Four additional loop/scope
 * forms and one five-minute gain-gated batch were nonexact. The exact linked
 * range remains fallback assembly; do not revive dead frame arrays, the prior
 * flag lattice, or the exhausted loop forms.
 */
#ifdef NON_MATCHING
/* PLATEAU-HANDOFF:overlay11UpdateMenu:start
 * symbol: overlay11UpdateMenu
 * score: 299/301 relocation-masked positional words
 * frame: 0x48
 * relocations: 102
 * first-mismatch: +0x138
 * summary: exact extent/frame and 102/102 relocation identities; two spill stores remain reversed around one call
 * PLATEAU-HANDOFF:overlay11UpdateMenu:end
 */
void overlay11UpdateMenu(s32 updateRate) {
    s32 index;
    s8 direction;
    s32 selection;
    O11Object *object;
    O11ObjectSub *sub;
    O11Status *status;
    s32 finish;
    volatile s32 *menuInput;

    status = func_80028F54();
    direction = D_0[D_1C4];
    if (direction < -32) {
        if (((status->mode >= 2) && (D_1BC < 3)) ||
            ((status->mode == 1) && (D_1BC < 2))) {
            D_1BC++;
            func_80000F94(0x32C, 0);
            direction = D_0[D_1C4];
        } else {
            func_80000F94(0x32D, 0);
            direction = D_0[D_1C4];
        }
    }
    if (direction >= 33) {
        if (D_1BC >= 2) {
            D_1BC--;
            func_80000F94(0x32C, 0);
        } else {
            func_80000F94(0x32D, 0);
        }
    }

    {
        void **handle;
        s16 value;

        handle = D_1CC;
        index = 1;
        do {
            value = (index == D_1BC) ? D_1B8 : 0;
            func_overlay_045_F0001BF4_188E04C(*handle, value);
            index++;
            handle++;
        } while (index != 4);
    }

    menuInput = (s32 *)(D_menuBase + 0x1C4);
    if ((func_8002554C(D_1C4) & 0x8000) || *menuInput != 0) {
        s32 action;

        finish = 0;
        selection = D_1BC;
        switch (selection) {
        case 1:
            overlay66Select(0);
            func_800290AC(0);
            func_800291D8(0x1E);
            func_800006BC(0.5f, 0x7F);
            func_overlay_011_F0002BF4_186B43C();
            D_204 = 1;
            break;
        case 2:
            action = *menuInput;
            if (action == 0) {
                func_overlay_011_F0001058_18698A0(4);
            } else if (action == -1) {
                func_overlay_011_F0001130_1869978(4);
            } else if (action == 1) {
                D_cfgA = 6;
                D_cfgB = 5;
                D_cfgC = 0;
                object = func_80005820(D_1C4);
                sub = object->sub64;
                if ((status->value1 != 0) && !(D_flags & 0x100)) {
                    status->mode--;
                }
                if (status->mode == 0) {
                    finish = 1;
                } else {
                    if (D_count > 0) {
                        D_paramA = D_table[status->value2][status->value1];
                        D_paramB = sub->value1;
                        D_paramC = 5;
                        D_paramD = 0;
                        func_80028374(0x12, 0, 0, 0xF, 1, 0);
                        func_80028528(1);
                    } else {
                        func_80028374(D_table[status->value2][status->value1],
                                     sub->value1, 0, 5, 1, 0);
                    }
                    D_204 = 1;
                }
            }
            break;
        case 3:
            action = *menuInput;
            if (action == 0) {
                func_overlay_011_F0001058_18698A0(4);
            } else if (action == -1) {
                func_overlay_011_F0001130_1869978(4);
            } else if (action == 1) {
                if (D_modeFlag == 1) {
                    func_8003A754();
                }
                func_overlay_011_F0002948_186B190();
                finish = 1;
            }
            break;
        }

        if (finish != 0) {
            if (D_count > 0) {
                object = func_80005820(D_1C4);
                sub = object->sub64;
                D_paramA = 0xC;
                D_paramB = sub->value1;
                D_paramC = 0xC;
                D_paramD = 1;
                func_80028374(0x12, 0, 0, 0xF, 1, 0);
                func_80028528(1);
            } else {
                func_80028374(0xC, 0, 0, 0xC, 1, 0);
            }
            D_204 = 1;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o011/overlay11UpdateMenu/func_overlay_011_F0001398_1869BE0.s")
#endif
