#include "PR/ultratypes.h"

typedef struct Overlay43RotationInput {
    s16 pad00;
    s16 angle;
} Overlay43RotationInput;

typedef struct Overlay43MotionOutput {
    f32 unk00;
    u8 pad04[0x0C];
    f32 unk10;
    f32 unk14;
    f32 unk18;
    u8 pad1C[0x0C];
    f32 unk28;
    u8 pad2C[0x18];
    s32 owner;
} Overlay43MotionOutput;

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

extern u8 D_0[];
#define D_24 (*(f32 *)(D_0 + 0x24))
extern void func_80029FE4(Overlay43RotationInput *input, Vec3f *direction);
extern void func_8002A82C(Overlay43MotionOutput *output);

/* The configured candidate retains the exact 55-word/0x38-frame geometry and
 * nine differing words; +0x5C is call metadata and the first code-bit mismatch
 * is +0x74. Moving the D_24 carrier declaration behind the saved components
 * in a 2026-09-04 structural pass shifts otherwise-exact homes and regresses
 * to 19 words. Preserve the original FP pool order and assembly fallback.
 *
 * 2026-09-10, lane o7-mid: the nine words are two facts, and the recorded
 * next lever ("flag and context parity review") is now closed by measurement.
 *
 * Seven words are ONE FP colour transposition. f0 and f2 are not ring temps
 * -- the ring is f4/f6/f8/f10 and the withdrawn pair are uopt colours (L13) --
 * so these are two coloured webs. The target puts sp20, the shared divisor,
 * in f0 and D_24 in f2; we do the exact opposite, at +0x74, +0x88, +0x98,
 * +0xA0, +0xAC, +0xBC and +0xC0. Every other register, every stack home
 * (sp24 at sp+0x24, sp20 at sp+0x20, sp1C at sp+0x1C) and the whole
 * instruction multiset already agree.
 *
 * The other two words are one scheduling pair: the target emits the D_24 load
 * at +0x90 and the `output` reload at +0x94, we emit them the other way.
 *
 * Falsified here, all leaving the f0/f2 assignment untouched: ALL 240 tail
 * statement orders that keep `temp_f2 = D_24` ahead of its two uses,
 * including every order that founds the division before the D_24 read (best
 * is the base at 9; nothing below it); all five orders of the three
 * direction-component saves; four declaration placements of temp_f2 relative
 * to `direction` and the three saved components (these move stack homes only
 * -- 15 and 19 words -- and never the colour); dropping the temp for two
 * direct D_24 reads and splitting it into two temps (+8 bytes each, uopt does
 * not CSE the reads); `-sp24 / sp20` versus `-(sp24 / sp20)`; and four
 * physical-line foldings of the tail (L59), which are flat at 9.
 *
 * And the full 119-mode flag lattice on this TU -- which contains only this
 * function, so a per-TU flag would have been adoptable -- floors at exactly 9
 * differing words with first mismatch +0x74. Thirteen O2/MIPS-II rows tie the
 * canonical `-O2 -mips2 -32`; every other row is worse or changes the size.
 * Flags are not the explanation and should not be swept again.
 *
 * Next lever: the f0/f2 order is a p1/p2 visit-order decision between two
 * webs that are structurally identical on both sides. Source form does not
 * reach it; a CDX force on the divisor web would say whether f0 is even on
 * its candidate list (L101's silent-decline shape) before more spelling. */
#ifdef NON_MATCHING
void func_overlay_043_F00010A8_188B078(Overlay43RotationInput *input,
                                      s32 owner,
                                      Overlay43MotionOutput *output) {
    f32 temp_f2;
    Vec3f direction;
    f32 sp24;
    f32 sp20;
    f32 sp1C;

    if (input->angle < 0) {
        input->angle = 0;
    }
    input->angle -= 0x4000;
    input->angle >>= 1;
    input->angle += 0x4000;

    direction.x = 0.0f;
    direction.y = 0.0f;
    direction.z = -1.0f;
    func_80029FE4(input, &direction);
    sp24 = direction.x;
    sp1C = direction.z;
    sp20 = direction.y;
    output->owner = owner;
    func_8002A82C(output);

    temp_f2 = D_24;
    output->unk00 = temp_f2;
    output->unk10 = -(sp24 / sp20);
    output->unk14 = 0.0f;
    output->unk18 = -(sp1C / sp20);
    output->unk28 = temp_f2;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o043/overlay43ComputeMotion/func_overlay_043_F00010A8_188B078.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_043_F00010A8_188B078:start
 * symbol: func_overlay_043_F00010A8_188B078
 * score: 46/55 words
 * frame: 0x38
 * relocations: 4
 * first-mismatch: +0x74
 * summary: Seven words are one f0/f2 colour transposition and two are one schedule pair; the 119-mode flag lattice and 240 tail orders are now measured out.
 * PLATEAU-HANDOFF:func_overlay_043_F00010A8_188B078:end
 */
