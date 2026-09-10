#include "PR/ultratypes.h"
typedef struct G { u32 w0, w1; } G;
typedef struct V { s16 x,y,z; u8 r,g,b,a; } V;
typedef struct Pair { V first, second; } Pair;
typedef struct Strip { s16 count; u8 buffer,pad03; void *material; u8 pad08[0x24]; Pair *buffers[2]; void *triangles; } Strip;
extern void overlay17PrepareStripReloc(G **, void *, s32, s32);
#define S(v,s,w) (((u32)(v)&((1U<<(w))-1U))<<(s))
#define PRIM(p) { G *m=(G *)(p); m->w0=0xFA000000; m->w1=0xFFFFFFFF; }
#define VTX(p,a,n) { G *m=(G *)(p); m->w0=S(4,24,8)|S(((n)<<3)|((u32)(a)&6),16,8)|S(((n)<<3)+((n)<<1)+8,0,16); m->w1=(u32)(a); }
#define STRIP(p,a,n,t) { G *m=(G *)(p); m->w0=S(5,24,8)|S((((n)-1)<<4)|(t),16,8)|S((n)<<4,0,16); m->w1=(u32)(a); }
#define SYNC(p) { G *m=(G *)(p); m->w1=0; m->w0=0xE7000000; }

/*
 * Plateau: spelling the final packet through the existing pair local gives
 * an exact-size 119-word candidate; the linked promotion trial agrees with the
 * object and carries no collateral.
 *
 * 2026-09-10, lane c6-close: the frame blocker is closed. The 0x38/0x40
 * difference was never packet-local lifetimes -- it is the declaration ORDER,
 * and moving `textured` ahead of `vertices` and `flush` recovers 0x38 with no
 * other home moving and 119 instructions unchanged: 16 -> 14 masked words.
 * Two earlier readings hid it. Removing a declaration left the frame at 0x40
 * because the reservation is `align8(4N)` over the surviving locals, and the
 * non-save block only steps from 32 to 24 bytes between N=7 and N=6, so one
 * removal cannot move it and two would have had to. And adding declarations is
 * inert here -- an unused local is eliminated before the frame is sized, so the
 * ladder cannot be probed with dummies the way it can on a function whose
 * locals all survive. Order, not count, is the lever on this body.
 *
 * All 40,320 declaration orders were measured: the floor is 14, and every
 * 14-scoring order carries the target's 0x38 frame with the `t5` spill home at
 * sp+0x28. All 6,720 legal orders of the eight prologue statements are
 * byte-flat at 14, so the residual is not statement order either.
 *
 * The residual is now one three-row block that sits three rows early
 * (`move t2,zero` / `move a1,v0` / the count test, which the target emits after
 * `move a3,zero`) plus seven register webs over `start`, `previous` and the
 * packet cursor: t3->a3, a0->v1, v1->t3, t3->t1, a1->a2, a0->v0, t1->t3. Every
 * temp-ring and shared-lane slot already agrees, 33/33 and 19/19; the pool lane
 * first diverges at slot 7 as an a0/a3 permutation.
 */
#ifdef NON_MATCHING
void overlay17DrawStrip(G **commands, Strip *strip) {
    Pair *pair;
    Pair *previous;
    Pair *start;
    s32 textured;
    s32 vertices;
    s32 flush;
    u32 remaining;
    u32 segment;
    if (strip == 0) return;
    segment=0x80000000U;
    textured = strip->material ? 1 : 0;
    textured |= (overlay17PrepareStripReloc(commands, strip->material, 0x1F, 0), 0);
    PRIM((*commands)++);
    vertices=strip->count;
    remaining=vertices;
    previous=0;
    pair=strip->buffers[strip->buffer];
    start=0; vertices=0; flush=0;
    if (remaining != 0) {
        remaining--;
        do {
            vertices += 2;
            if (remaining == 0) {
                flush=1;
                goto check_flush;
            }
            if (pair->first.a == 0) {
                flush=1;
                goto check_flush;
            }
            if (start == 0) {
                if (previous != 0) { start=previous; vertices=4; }
                else { start=pair; vertices=2; }
            }
            if (vertices >= 17) flush=1;
check_flush:
            if (flush) {
                flush=0;
                if (start != 0) {
                    VTX((*commands)++, (void *)((u32)start+segment), vertices);
                    STRIP((*commands)++, (void *)((u32)strip->triangles+segment), vertices-2,
                          textured);
                    start=0;
                }
                vertices=0;
            }
            previous=pair++;
        } while (remaining--);
    }
    pair = (Pair *)(*commands)++;
    ((G *)pair)->w1 = 0;
    ((G *)pair)->w0 = 0xE7000000;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o017/overlay17DrawStrip/func_overlay_017_F00008B4_187426C.s")
#endif

/* PLATEAU-HANDOFF:overlay17DrawStrip:start
 * symbol: overlay17DrawStrip
 * score: 105/119 words
 * frame: 0x38
 * relocations: 1
 * first-mismatch: +0x68
 * summary: The frame blocker is closed and was declaration ORDER, not packet-local lifetimes: moving textured ahead of vertices and flush recovers 0x38 with no other home moving, 16 to 14 masked words. All 40,320 declaration orders and all 6,720 prologue statement orders were measured; 14 is the floor of both. What remains is one three-row block emitted three rows early plus seven register webs over start, previous and the packet cursor.
 * PLATEAU-HANDOFF:overlay17DrawStrip:end
 */
