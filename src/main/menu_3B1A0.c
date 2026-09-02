#include "PR/ultratypes.h"

extern u8 D_8007BF3C[];
extern s32 D_8007BF44;
extern void *func_80028F54(void);

typedef struct Menu3B1A0Record {
    u8 byte00;
    u8 byte01;
    u8 byte02;
    u8 byte03;
    u8 bytes04[3];
    u8 byte07;
    u8 bytes08[0x17];
    u8 byte1F;
    u8 bytes20[6];
    u16 half26;
} Menu3B1A0Record;

typedef struct Menu3B1A0ByteRecord {
    u8 bytes00[0x1F];
    u8 byte1F;
} Menu3B1A0ByteRecord;

typedef struct Menu3B1A0State {
    u8 bytes000[0x383];
    s8 count;
    u8 bytes384[0xD8];
    u8 flag45C;
} Menu3B1A0State;

typedef struct Menu3B1A0Object {
    u8 bytes00[0x64];
    Menu3B1A0State *state;
} Menu3B1A0Object;

typedef struct Menu3B1A0KeyGroup {
    s16 key00;
    s16 key02;
    s16 key04;
    s16 key06;
} Menu3B1A0KeyGroup;

extern s32 *D_8007C0B8;
extern u8 D_8007C0E8;
extern u8 D_8007C118;
extern s16 D_8007C11C[];
extern s16 D_8007C11E[];
extern s16 D_8007C120[];
extern s16 D_8007C122[];
extern u8 D_80082714[];

/* Workbench verdict: structure-mismatch, 19 differing words; target/candidate 56 words. */
/* First mismatch: +0x0; two opcode and five relocation-site differences remain. */
/* Structural gap: lookup/end pointer carrier allocation and relocation web. */
#ifdef NON_MATCHING
void *func_8003A5A0(s32 arg0) {
    register u8 *var_a1;
    s16 temp_t1;
    s32 var_v1;

    var_a1 = &D_8007C0E8;
    var_v1 = 0;
loop_lookup:
    if (arg0 == *(s16 *) (var_a1 + 0)) {
        return (void *) D_8007C0B8[
            *(s16 *) ((u8 *) D_8007C11C + var_v1)];
    }
    if (arg0 == *(s16 *) (var_a1 + 2)) {
        return (void *) D_8007C0B8[
            *(s16 *) ((u8 *) D_8007C11E + var_v1)];
    }
    if (arg0 == *(s16 *) (var_a1 + 4)) {
        return (void *) D_8007C0B8[
            *(s16 *) ((u8 *) D_8007C120 + var_v1)];
    }
    temp_t1 = *(s16 *) (var_a1 + 6);
    var_a1 += 8;
    if (arg0 == temp_t1) {
        return (void *) D_8007C0B8[
            *(s16 *) ((u8 *) D_8007C122 + var_v1)];
    }
    var_v1 += 8;
    if (var_a1 == &D_8007C118) {
        return D_80082714;
    }
    goto loop_lookup;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/menu_3B1A0/func_8003A5A0.s")
#endif

void func_8003A680(s32 arg0) {
    if (D_8007BF44 < 8) {
        D_8007BF3C[D_8007BF44] = arg0;
        D_8007BF44 += 1;
    }
}
s32 func_8003A6B0(u8 arg0) {
    s32 temp_t6 = arg0;
    s32 temp_v1 = temp_t6;

    if (temp_t6 == 0x21) {
        goto return_1B;
    }
    if (temp_t6 == 0x2A) {
        goto return_1C;
    }
    if (temp_t6 != 0x3F) {
        goto return_default;
    }
    return 0x1A;

return_1B:
    return 0x1B;
return_1C:
    return 0x1C;
return_default:
    return (temp_v1 - 0x41) & 0xFF;
}
s32 func_8003A700(u8 arg0) {
    s32 temp_t6 = arg0;
    s32 temp_v1 = temp_t6;

    switch (temp_t6) {
    case 0x1A:
        return 0x3F;
    case 0x1B:
        return 0x21;
    case 0x1C:
        return 0x2A;
    default:
        return (temp_v1 + 0x41) & 0xFF;
    }
}
/* Workbench verdict: structure-mismatch, 12 differing words; target/candidate 31 words. */
/* First mismatch: +0x10; both frames are 0x18, with scalar inner-loop carriers differing. */
/* Structural gap: inner-loop counter/base carrier order remains unresolved. */
#ifdef NON_MATCHING
void func_8003A754(void) {
    s32 var_t0;
    s32 var_v1;
    u8 *var_a0;
    Menu3B1A0Record *var_a1;
    Menu3B1A0Record *var_a2;
    Menu3B1A0Record *var_a3;

    var_a3 = (Menu3B1A0Record *) func_80028F54();
    var_t0 = 0;
    var_a1 = var_a3;
    var_a2 = var_a3;
loop_outer:
    var_a2->half26 = 0;
    var_a2->byte07 = 0;
    var_v1 = 0;
    var_a0 = (u8 *) var_a3;
loop_inner:
    var_v1 += 1;
    var_a0 += 1;
    var_a0[0x1F] = 0;
    if (var_v1 < 6) {
        goto loop_inner;
    }
    var_t0 += 0x28;
    var_a3 += 1;
    var_a1 += 1;
    var_a2 += 1;
    var_a1->byte01 = 0;
    var_a1->byte02 = 0;
    var_a1->byte03 = 0;
    var_a1->byte00 = 0;
    if (var_t0 != 0x190) {
        goto loop_outer;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/menu_3B1A0/func_8003A754.s")
#endif
/* Workbench verdict: structure-mismatch, 36 differing words; target 43/candidate 41 words. */
/* First mismatch: +0x10; the candidate omits two initial pointer-materialization words. */
/* Structural gap: count-carrier allocation and first-loop address formation. */
#ifdef NON_MATCHING
s32 func_8003A7D0(Menu3B1A0Object *arg0) {
    s32 temp_t1;
    s32 temp_t5;
    s32 temp_t8;
    s32 temp_v1;
    s32 var_a0;
    s32 var_v1;
    s32 var_a1;
    s32 temp_a2;
    Menu3B1A0State *temp_v0;
    s32 *var_a3;
    s32 *var_a3_2;

    temp_v0 = arg0->state;
    var_v1 = 0;
    temp_a2 = temp_v0->count;
    var_a1 = temp_a2;
    if ((temp_v0->flag45C != 0) && (temp_a2 != -1)) {
        var_a1 += 1;
    }
    var_a0 = 0;
    if (var_a1 > 0) {
        temp_t1 = var_a1 & 3;
        if (temp_t1 != 0) {
            var_a3 = (s32 *) temp_v0 + var_a0;
loop_small:
            temp_t8 = var_a3[0x101];
            var_a0 += 1;
            var_a3 += 1;
            var_v1 += temp_t8;
            if (temp_t1 != var_a0) {
                goto loop_small;
            }
            if (var_a0 == var_a1) {
                goto done;
            }
        }
        var_a3_2 = (s32 *) temp_v0 + var_a0;
loop_large:
        temp_t5 = var_a3_2[0x104];
        var_a0 += 4;
        temp_v1 = var_v1 + var_a3_2[0x101] + var_a3_2[0x102] +
                  var_a3_2[0x103];
        var_a3_2 += 4;
        var_v1 = temp_v1 + temp_t5;
        if (var_a0 != var_a1) {
            goto loop_large;
        }
    }
done:
    if (temp_a2 < 3) {
        var_v1 += 0x2BF20;
    }
    return var_v1;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/menu_3B1A0/func_8003A7D0.s")
#endif

/* PLATEAU-HANDOFF:func_8003A5A0:start
 * symbol: func_8003A5A0
 * score: 19 differing words
 * frame: unknown
 * relocations: 22
 * first-mismatch: +0x0
 * summary: Lookup carrier allocation and absolute relocation web remain unresolved.
 * PLATEAU-HANDOFF:func_8003A5A0:end
 */

/* PLATEAU-HANDOFF:func_8003A754:start
 * symbol: func_8003A754
 * score: 12 differing words
 * frame: 0x18
 * relocations: 1
 * first-mismatch: +0x10
 * summary: Scalar inner-loop carrier order remains structurally different.
 * PLATEAU-HANDOFF:func_8003A754:end
 */

/* PLATEAU-HANDOFF:func_8003A7D0:start
 * symbol: func_8003A7D0
 * score: 37 differing words
 * frame: frameless
 * relocations: 0
 * first-mismatch: +0x10
 * summary: Count-carrier allocation and first-loop address formation remain unresolved.
 * PLATEAU-HANDOFF:func_8003A7D0:end
 */
