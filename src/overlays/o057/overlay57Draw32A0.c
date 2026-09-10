#include "PR/ultratypes.h"

typedef struct Overlay57Draw32A0Record {
    s32 word0;
    s32 word4;
    u32 packed8;
} Overlay57Draw32A0Record;

extern s32 gO57Draw32A0Available4CC;
extern s32 gO57Draw32A0Selection1A0;
extern s32 gO57Draw32A0State118;
extern s32 gO57Draw32A0Envelope124;
extern s32 gO57Draw32A0TableMode18C[];
extern u32 gO57Draw32A0WordTable4B8[];
extern Overlay57Draw32A0Record gO57Draw32A0Records3F8[];
extern Overlay57Draw32A0Record gO57Draw32A0Records458[];
extern Overlay57Draw32A0Record gO57Draw32A0Work34C[];
extern u32 gO57Draw32A0Masks3C8[];
extern u32 gO57Draw32A0Shifts3D8[];
extern u8 gO57Draw32A0Local08[];
extern u8 gO57Draw32A0Local18[];
extern u8 gO57Draw32A0Local28[];

extern s32 gO57Draw32A0GateReloc;
extern u8 gO57Draw32A0AnchorReloc[];
extern u8 gO57Draw32A0StripeReloc[];
extern u16 gO57Draw32A0HalfwordsReloc[];

extern s32 o57Draw32A0Call0Reloc(void);
extern s32 o57Draw32A0Call1Reloc(void);
extern s32 o57Draw32A0Call2Reloc(void);
extern void o57Draw32A0SetupReloc(void *anchor, s32 count,
                                  Overlay57Draw32A0Record *records, s32 mode);
extern void o57Draw32A0ModeReloc(s32 mode);
extern void o57Draw32A0ColorReloc(s32 red, s32 green, s32 blue, s32 alpha,
                                  s32 envelope);
extern void o57Draw32A0Stripe0Reloc(void *anchor, s32 width, s32 position,
                                    void *data, s32 count);
extern void o57Draw32A0Stripe1Reloc(void *anchor, s32 width, s32 position,
                                    void *data, s32 count);
extern void o57Draw32A0Stripe2Reloc(void *anchor, s32 width, s32 position,
                                    void *data, s32 count);
extern void o57Draw32A0Stripe3Reloc(void *anchor, s32 width, s32 position,
                                    void *data, s32 count);
extern void o57Draw32A0RenderReloc(void *anchor,
                                   Overlay57Draw32A0Record *record,
                                   f32 scale, f32 position, f32 xScale,
                                   f32 yScale, s32 color, s32 command);

/* Overlay 57 text +0x32A0..+0x35E0. */
/* Plateau: exact 0x340, 48 masked words, first at +0x4; frame and both
 * instruction counts are exact and the pool lane is exact 85/85.
 *
 * Three things got it here. Two declared pointer cursors over the mask and
 * shift tables were two frame cells the target does not reserve; indexing both
 * tables by the loop counter closes the frame at 0x70. The saturating arm
 * writes 0xFE to the global and reads it back rather than computing
 * `rising + 0xFD`: spelling the constant directly makes uopt hoist a second
 * materialisation of 254 into the guard's delay slot, but routing the value
 * through the global keeps one `li v1,254` feeding one `sw v1,0(s8)`, which is
 * what the target has.
 *
 * The residual is one uniform rotation of the block-local temp ring with a
 * single onset at row 64 (+0x100): rows 0..63 are byte-identical, and from
 * +0x100 every temp is one ring position behind the target. Declaration order
 * (all 43 single moves), both loop bounds, the arm order of the table-mode
 * `if`, and the operand order of the packed-word `or` do not move it. */
#ifdef NON_MATCHING
void overlay57Draw32A0(s32 updateRate) {
    Overlay57Draw32A0Record *records;
    Overlay57Draw32A0Record *cursor;
    s32 envelope;
    s32 rowCount[1];
    s32 position;
    s32 i;
    s32 rising;

    if (gO57Draw32A0Available4CC == 0) {
        return;
    }

    rising = 0;
    if ((gO57Draw32A0GateReloc == 1) &&
        (o57Draw32A0Call0Reloc() < 5) &&
        (o57Draw32A0Call1Reloc() != 0)) {
        gO57Draw32A0Selection1A0 = o57Draw32A0Call2Reloc();
        if (gO57Draw32A0State118 == 1) {
            rising = 1;
        }
    }

    if (rising != 0) {
        envelope = (gO57Draw32A0Envelope124 += updateRate * 8);
        if (envelope >= 0xFF) {
            gO57Draw32A0Envelope124 = 0xFE;
            envelope = gO57Draw32A0Envelope124;
        }
    } else {
        envelope = gO57Draw32A0Envelope124 - updateRate * 32;
        gO57Draw32A0Envelope124 = envelope;
        if (envelope < 0) {
            gO57Draw32A0Envelope124 = 0;
            return;
        }
    }

    if (gO57Draw32A0TableMode18C[0] != 0) {
        rowCount[0] = 4;
        records = gO57Draw32A0Records3F8;
        position = 0x3E;
    } else {
        rowCount[0] = 3;
        records = gO57Draw32A0Records458;
        position = 0x4D;
    }

    rising = gO57Draw32A0WordTable4B8[gO57Draw32A0Selection1A0] |
             ((envelope * 5) >> 3);
    i = 1;
    cursor = &records[1];
    do {
        i++;
        cursor->packed8 = rising;
        cursor++;
    } while (i < 8);

    o57Draw32A0SetupReloc(gO57Draw32A0AnchorReloc, 8, records, 0);
    o57Draw32A0ModeReloc(2);
    o57Draw32A0ColorReloc(0xFF, 0xFF, 0xFF, 0xFF,
                          gO57Draw32A0Envelope124);
    o57Draw32A0Stripe0Reloc(gO57Draw32A0AnchorReloc, 0xF8, position,
                            gO57Draw32A0StripeReloc, 4);
    position += 0x1E;
    o57Draw32A0Stripe1Reloc(gO57Draw32A0AnchorReloc, 0xF8, position,
                            gO57Draw32A0Local08, 4);
    position += 0x1E;
    o57Draw32A0Stripe2Reloc(gO57Draw32A0AnchorReloc, 0xF8, position,
                            gO57Draw32A0Local18, 4);
    position += 0x1E;
    if (gO57Draw32A0TableMode18C[0] != 0) {
        o57Draw32A0Stripe3Reloc(gO57Draw32A0AnchorReloc, 0xF8, position,
                                gO57Draw32A0Local28, 4);
    }

    i = 0;
    position -= 0x48;
    if (rowCount[0] > 0) {
        do {
            gO57Draw32A0Work34C[0].packed8 =
                ((gO57Draw32A0HalfwordsReloc[4 + gO57Draw32A0Selection1A0] &
                  gO57Draw32A0Masks3C8[i]) >> gO57Draw32A0Shifts3D8[i]) << 16;
            o57Draw32A0RenderReloc(gO57Draw32A0AnchorReloc,
                                   gO57Draw32A0Work34C, 248.0f,
                                   (f32)position, 1.0f, 1.0f,
                                   gO57Draw32A0Envelope124 | -256, 0x2003);
            i++;
            position += 0x1E;
        } while (i != rowCount[0]);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o057/overlay57Draw32A0/func_overlay_057_F00032A0_18A6E98.s")
#endif

/* PLATEAU-HANDOFF:overlay57Draw32A0:start
 * symbol: overlay57Draw32A0
 * score: 48/208 words
 * frame: 0x70
 * relocations: 53
 * first-mismatch: +0x4
 * summary: Frame, instruction count and the pool lane are exact; 48 words remain as one uniform block-local temp-ring rotation whose onset is row 64 with rows 0 to 63 byte-identical.
 * PLATEAU-HANDOFF:overlay57Draw32A0:end
 */
