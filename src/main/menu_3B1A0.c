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
    u8 bytes384[0x80];
    /* 0x404: `count` (+1 when flag45C is set) s32 entries, bounded above by
     * flag45C at 0x45C -- 0x16 of them.  Tier D, structural: the extent is
     * fixed by the two named neighbours, the element type and base by
     * func_8003A7D0's own accumulation loop. */
    s32 entries[0x16];
    u8 flag45C;
} Menu3B1A0State;

typedef struct Menu3B1A0Object {
    u8 bytes00[0x64];
    Menu3B1A0State *state;
} Menu3B1A0Object;

typedef struct Menu3B1A0KeyGroup {
    s16 key[4];
} Menu3B1A0KeyGroup;

extern s32 *D_8007C0B8;
extern u8 D_8007C0E8;
extern u8 D_8007C118;
extern s16 D_8007C11C[];
extern s16 D_8007C11E[];
extern s16 D_8007C120[];
extern s16 D_8007C122[];
extern u8 D_80082714[];

/* Workbench verdict: structure-mismatch, 9 differing words; target/candidate
 * 56 words. First mismatch +0x0; the residual is one register pair, the
 * cursor and the end pointer, which the target colours a1/v0 and this
 * candidate v0/a1, plus the %lo materialization order that follows from it.
 *
 * Two levers closed 19 -> 9. The fourth key read was a named local in the m2c
 * draft, which gave it a uopt pool colour and reversed the compare's operands
 * (L67/L87); spelling it as the dereference again puts it back on the ugen
 * temp ring. And the loop exit is `end == cursor`, not `cursor == end`.
 *
 * The `while (1)` with an interior exit is load-bearing, not a draft artifact:
 * this TU compiles with the default unroller (see the Makefile note on
 * menu_3B1A0.c), and every bottom- or top-tested form -- `do`/`while`, `for`,
 * `while (end != cursor)` -- is unrolled to 154 words against the target's 56.
 * Only the interior-exit form is opaque to the unroller.
 *
 * Ruled out at this residual, each measured over a full lattice rather than
 * sampled: declaration order (all 6), initialization order (all 6), physical
 * line grouping of the initializers (all 4 compositions x all 6 orders),
 * `register` on the cursor, s16* versus u8* for the end pointer, declaration
 * initializers, an (s32) compare, deriving the end as cursor + 0x30, and
 * swapping the cursor/offset increment order. All 192 + 7 flat at 9 or worse.
 *
 * No donor counterpart: JFG's src/menu.c has no function of this shape. */
#ifdef NON_MATCHING
void *func_8003A5A0(s32 arg0) {
    u8 *cursor;
    u8 *end;
    s32 offset;

    cursor = &D_8007C0E8;
    end = &D_8007C118;
    offset = 0;
    while (1) {
        if (arg0 == *(s16 *) (cursor + 0)) {
            return (void *) D_8007C0B8[
                *(s16 *) ((u8 *) D_8007C11C + offset)];
        }
        if (arg0 == *(s16 *) (cursor + 2)) {
            return (void *) D_8007C0B8[
                *(s16 *) ((u8 *) D_8007C11E + offset)];
        }
        if (arg0 == *(s16 *) (cursor + 4)) {
            return (void *) D_8007C0B8[
                *(s16 *) ((u8 *) D_8007C120 + offset)];
        }
        if (arg0 == *(s16 *) (cursor + 6)) {
            return (void *) D_8007C0B8[
                *(s16 *) ((u8 *) D_8007C122 + offset)];
        }
        cursor += 8;
        offset += 8;
        if (end == cursor) {
            return D_80082714;
        }
    }
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
/* PROVENANCE: no donor counterpart -- JFG's src/menu.c has no function of
 * this shape, so this is ordinary matching against Mickey's own bytes.
 * The target's 4x-unrolled accumulation loop is the compiler's, not the
 * source's: this TU's -Wo,-loopunroll,0 was removed (it was byte-inert for
 * every function the TU still compiles) and the plain `for` then unrolls to
 * the target's exact 44 words.  The guard and the final test read
 * `state->count` again rather than the local: IDO CSEs all three reads onto
 * one carrier and copies it into the second, which is what puts the -1
 * compare on the original and the increment on the copy (lever 45). */
s32 func_8003A7D0(Menu3B1A0Object *arg0) {
    Menu3B1A0State *state;
    s32 total;
    s32 limit;
    s32 i;

    state = arg0->state;
    total = 0;
    limit = state->count;
    if ((state->flag45C != 0) && (state->count != -1)) {
        limit += 1;
    }
    for (i = 0; i < limit; i++) {
        total += state->entries[i];
    }
    if (state->count < 3) {
        total += 0x2BF20;
    }
    return total;
}

/* PLATEAU-HANDOFF:func_8003A5A0:start
 * symbol: func_8003A5A0
 * score: 9 differing words
 * frame: frameless
 * relocations: 22
 * first-mismatch: +0x0
 * summary: Cursor/end register pair and the %lo materialization order that follows it; every declaration, initialization, line-grouping and loop-shape spelling measured flat at 9.
 * PLATEAU-HANDOFF:func_8003A5A0:end
 */

/* PLATEAU-HANDOFF:func_8003A754:start
 * symbol: func_8003A754
 * score: 12/31 words
 * frame: 0x18
 * relocations: 1
 * first-mismatch: +0x10
 * summary: Trace isolates ugen line-order; next lever is authentic call-result C that schedules t0 before a3 without #line or inert scaffolding. The -Wo,-loopunroll,0 removal from this TU is byte-inert here as well: 36 loop shapes (4 head orders x 5 inner forms x 2 outer forms) all measure flat at 12, so the reopen this flag change would otherwise justify is already spent.
 * PLATEAU-HANDOFF:func_8003A754:end
 */
