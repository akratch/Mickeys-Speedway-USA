#include "PR/ultratypes.h"

typedef struct Overlay71Command {
    u32 w0;
    u32 w1;
} Overlay71Command;

typedef struct Overlay71DrawState {
    u8 pad00[0xCC];
    u8 active;
    u8 vertexBank;
    u16 flags;
} Overlay71DrawState;

typedef struct Overlay71DrawObject {
    u8 pad00[0x64];
    Overlay71DrawState *state;
    s32 *resourceIndex;
} Overlay71DrawObject;

extern void *gOverlay71InitialResourceReloc;
extern u8 D_80000008[];
extern u8 D_80000028[];
extern u8 D_800000D8[];

extern void func_80032BF0(void *resource, s32 mode, s32 flags);
extern void func_8002409C(Overlay71Command **commands, s32 context,
                         Overlay71DrawObject *object, f32 scale, f32 extra);
extern void func_80034554(Overlay71Command **commands, s32 resource, s32 mode,
                         s32 flags);
extern void func_800241BC(Overlay71Command **commands);

/* DKR v77/v80 and JFG contain no exact donor for this renderer. */
/*
 * Workbench: exact 182 words and -48 frame; 11 masked words, first +0x94.
 *
 * Lane c6-band-b (2026-09-10): 33 -> 11 by ONE whitespace edit.  Every
 * two-word display-list command shipped its `w1` store before its `w0` store
 * and this candidate emitted them the other way round -- ten sites, twenty
 * words.  The two long-expression commands (the vertex-bank pair) already
 * agreed.  Writing `w1` first in the source REGRESSES to 41: as1 orders two
 * ready stores by physical source line (L59, lexicographic minimum on
 * `node->lineno`), so an inversion just moves the same tie.  Folding the pair
 * onto ONE physical line makes the two line numbers equal, drops the tie
 * through to ready-list position, and that is the shipped order at all ten
 * sites at once.  Token stream unchanged after whitespace normalisation.
 *
 * The residue is one colouring fact: the shipped code reads `state->flags`
 * into `a2` at all four tests, this candidate into `a1` (the first two) and
 * `a0` (the last two).  Instrumented globalcolor says it is fully
 * colour-reachable -- forcing the two load webs to c5 gives a byte-identical
 * object, 0 differing words -- but the allocator takes the first available
 * colour at strictly-minimum cost, and reaching a2 needs interfering webs
 * already parked on a0 AND a1.  The `u16 flags` local supplies the a0 one
 * (its zero-extend is peepholed away, so the web is invisible in the object,
 * and dropping the local costs a word); no source form found so far supplies
 * an a1 one.  Flat at 11 or worse: the 16 single-local and 60 two-local
 * carrier placements over the four tests, the local's type and qualifier
 * lattice (`register` is inert at -O2, `volatile` changes the frame), six
 * uopt region boundaries (L97), `!= 0` and unsigned-literal test spellings,
 * hoisting `*object->resourceIndex`, and reloading `state` inside the block.
 * Also inert, all twenty of them byte-identical to this file: wrapping each
 * command word's constant in a nested assignment to a named local, the
 * `w0 = (x = 0xE7000000)` idiom a permuter run proposed. The pointer form of
 * that idiom is a real web on overlay101DrawTransformed because the pointer
 * escapes; a constant one is propagated away here.
 *
 * That permuter run is also why `skills/tools/permuter.md` now warns that the
 * importer reformats the TU: it undoes the fold above before generating a
 * single candidate, reported this function's unfolded score of 420 as its
 * base, and spent its whole budget improving that.
 */
#ifdef NON_MATCHING
void func_overlay_071_F0000870_18CA390(Overlay71Command **commands,
                                       s32 context,
                                       Overlay71DrawObject *object) {
    Overlay71DrawState *state;
    Overlay71Command *command;
    u16 flags;
    func_80032BF0(gOverlay71InitialResourceReloc, 2, 2);
    state = object->state;
    if (state->active != 0) {
        func_8002409C(commands, context, object, 1.0f, 0.0f);

        command = *commands;
        *commands = command + 1;
        command->w0 = 0xE7000000; command->w1 = 0;
        command = *commands;
        *commands = command + 1;
        command->w0 = 0xFB000000; command->w1 = 0xFFFFFFFF;

        flags = state->flags;
        if (flags & 1) {
            func_80034554(commands, 0, 0x17, 0);
            command = *commands;
            *commands = command + 1;
            command->w0 = 0xFA000000; command->w1 = 0xFFFFFFD0;
            command = *commands;
            *commands = command + 1;
            command->w0 =
                ((((((((u32)state + state->vertexBank * 0x50) +
                         0x80000000) & 6) | 0x20) & 0xFF) << 16) |
                 0x04000000) | 0x30;
            command->w1 =
                (u32)state + state->vertexBank * 0x50 + 0x80000000;
            command = *commands;
            *commands = command + 1;
            command->w0 = 0x05100020; command->w1 = (u32)D_80000008;
            command = *commands;
            *commands = command + 1;
            command->w0 = 0xE7000000; command->w1 = 0;
        }

        if (state->flags & 6) {
            func_80034554(commands, *object->resourceIndex, 0x17, 0);
            command = *commands;
            *commands = command + 1;
            command->w0 = 0xFA000000; command->w1 = 0xFFFFFFFF;
            command = *commands;
            *commands = command + 1;
            command->w0 =
                ((((((((u32)state + state->vertexBank * 0x50) +
                         0x80000000) & 6) | 0x40) & 0xFF) << 16) |
                 0x04000000) | 0x58;
            command->w1 =
                (u32)state + state->vertexBank * 0x50 + 0x80000000;
            if (state->flags & 4) {
                command = *commands;
                *commands = command + 1;
                command->w0 = 0x05710080; command->w1 = (u32)D_800000D8;
            }
            if (state->flags & 2) {
                command = *commands;
                *commands = command + 1;
                command->w0 = 0x05710080; command->w1 = (u32)D_80000028;
            }
            command = *commands;
            *commands = command + 1;
            command->w0 = 0xE7000000; command->w1 = 0;
        }
        command = *commands;
        *commands = command + 1;
        command->w0 = 0xFA000000; command->w1 = 0xFFFFFFFF;
        func_800241BC(commands);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o071/func_overlay_071_F0000870_18CA390/func_overlay_071_F0000870_18CA390.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_071_F0000870_18CA390:start
 * symbol: func_overlay_071_F0000870_18CA390
 * score: 171/182 words
 * frame: 0x30
 * relocations: 13
 * first-mismatch: +0x94
 * summary: Folding each command's w0/w1 pair onto one physical line makes their as1 line-number tie fall through to ready-list position and reproduces the shipped store order at all ten simple command sites, 33 -> 11 words at unchanged size and frame. The residue is one colour: the four state->flags reads want a2 and take a1/a0. Instrumented globalcolor proves it reachable (forcing both load webs to c5 gives 0 words); no source form yet parks an interfering web on a1.
 * PLATEAU-HANDOFF:func_overlay_071_F0000870_18CA390:end
 */
