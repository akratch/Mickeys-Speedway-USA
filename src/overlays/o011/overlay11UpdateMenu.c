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
 *
 * A 2026-09-04 scheduler pass exhausted ten additional declaration-order and
 * physical-line variants without a gain. The trace-on object is byte-identical
 * to the normal candidate; as1 assigns both spills to the call line with the
 * same dependency and zero aftercycles, then ranks the handle spill first.
 * Source-line scheduling and declaration order are exhausted for this pair.
 *
 * A 2026-09-10 scoped reproof identifies the argument-load base as $3 (v1).
 * The baseline already emits .noalias for $3 against $sp, but only after the
 * handle loop, when that register holds menuInput. No fact covers the handle
 * load and spill pair. Indexing D_menuBase changes neither bytes nor alias
 * scope: it remains 299/301 masked words, frame 0x48, with 102 relocations.
 * A faithful assembly replay scores zero masked differences with the $3 fact
 * at the pair; naming $2 or closing the fact before the second store leaves
 * two. These are diagnostics, not matching C. The zero-temp indexed menuInput
 * form disproves the prior universal temp-cost claim, not the scope blocker.
 * docs/matching-triage-handoffs/overlay11UpdateMenu.md.
 *
 * A 2026-09-10 second pass (lane c4-o11) closes the frame objection and moves
 * the residual. The scheduler trace shows why the pair inverts: without the
 * disambiguation fact the argument load has BOTH spill stores as dependent
 * successors, so scheduling the load releases them together, they enter the
 * ready list in reverse emission order, and the later one wins the tie. With
 * the fact they have no predecessor and keep emission order, which is the
 * target. A second sufficient input exists at the ugen level -- emitting the
 * two spill stores in the opposite order reaches an object identical to the
 * fact-bearing control -- but a scan of 391 compiled units finds 136 of 136
 * caller-save spill groups emitted in ascending physical register number, and
 * the target's own bytes put the index in v0 and the handle in v1, so ugen
 * cannot emit the handle store first. That route is closed.
 *
 * The recorded temp-cell exclusion is false for this function. The frame is
 * align8(28 + declared block + 4 * pooled temporaries) and the declared block
 * is a census the source controls. Cutting it from 44 to 28 bytes -- drop the
 * object local, fold selection and action into index, inline the value
 * ternary, keep one pointer local -- lands the strength-reduced pointer on
 * pool cell 1 at 36(sp), index at 68(sp), status at 48(sp), finish at 44(sp),
 * frame 0x48, 215 instructions, with the fact covering the pair. The
 * configured object then carries the target's store order at +0x138 and its
 * delay slot at +0x140: that pair is fixed.
 *
 * The residual moves to +0x10C/+0x110, the loop preheader, and the new barrier
 * is a different mechanism. uopt appends a strength-reduced induction
 * pointer's preheader initialisation after every user preheader statement,
 * while the target orders the pointer's address materialisation before the
 * index initialiser -- an order only a user assignment produces, and a user
 * assignment means a walking pointer, which never emits the fact. Five loop
 * shapes and three initialiser placements all keep the induction init last;
 * six constant-index forms (including a pointer-to-array element and a
 * one-element array member) all fold to a plain dereference and emit no fact;
 * six pointer-arithmetic spellings of the indexed load all fold to the same
 * base-plus-four with a minus-four displacement, and only routing the offset
 * through a pointer variable restores the zero displacement. So the two halves
 * are complementary: the fact costs the preheader, and the preheader costs the
 * fact. Both land at 299/301 with zero size delta, so the retained body is
 * unchanged. Do not re-run the spelling lattice; the open lever is a form that
 * emits the fact without a strength-reduced induction pointer.
 */
#ifdef NON_MATCHING
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

/* PLATEAU-HANDOFF:overlay11UpdateMenu:start
 * symbol: overlay11UpdateMenu
 * score: 299/301 words
 * frame: 0x48
 * relocations: 102
 * first-mismatch: +0x138
 * summary: The indexed form reaches frame 0x48 with every target stack home and fixes the +0x138 spill pair; the residual moves to the preheader because uopt emits an induction pointer's initialiser after every user statement.
 * PLATEAU-HANDOFF:overlay11UpdateMenu:end
 */
