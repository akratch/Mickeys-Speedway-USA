#include "PR/ultratypes.h"

/*
 * PROVENANCE: this texture-TU sequence was compared with Jet Force Gemini's
 * public src/textures.c. The two state setters are JFG-shaped starting
 * points; Mickey's globals and final byte/relocation results decide adoption.
 */

extern u8 D_8007BD94;
extern s32 D_8007BD80;

typedef struct TextureCacheEntry {
    s32 id;
    void *texture;
} TextureCacheEntry;

typedef struct TextureHeader {
    u16 numberOfInstances;
    u8 pad02[0x1E];
} TextureHeader;

extern s32 D_800D2FE0;
extern TextureCacheEntry *D_800D2FD8;
extern void mmFree(void *ptr);
void func_80034424(u8 enabled) {
    D_8007BD94 = enabled;
}
void func_80034434(s32 value)
{
  s32 *new_var;
  D_8007BD80 = (*(new_var = &value)) & 0xFF;
}
#ifdef NON_MATCHING
/* Frame-exact source scaffold; the loader's cache and decompression path remains
 * on the canonical fallback until its full ABI is recovered. */
void *func_80034448(s32 textureId) {
    volatile u8 frame_pad[0x40];
    TextureCacheEntry *entry;
    s32 i;

    frame_pad[0] = (u8)textureId;
    if (D_800D2FE0 > 0) {
        for (i = 0; i < D_800D2FE0; i++) {
            entry = &D_800D2FD8[i];
            if (entry->id == textureId) {
                return entry->texture;
            }
        }
    }
    return NULL;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_35024/func_80034448.s")
#endif
#ifdef NON_MATCHING
/* PROVENANCE: control-flow and the two-word cache index are adapted from Jet
 * Force Gemini's public src/textures.c::texFreeTexture; Mickey's raw cache
 * layout, callers, and compiled bytes remain authoritative. Configured C now
 * matches 41/43 words with the exact instruction, register, opcode, and
 * seven-relocation shape. Only the symmetric frame adjustment differs: the
 * candidate is 0x30 while the target is 0x28. Ten local ablations and
 * scope/width/register
 * variants either preserve that frame or destroy the exact topology. Candidate
 * SHA-1 is a988205d2a34; preserve this form pending producer evidence for the
 * unused automatic home that rounds the frame upward. */
void func_800347A0(TextureHeader *tex) {
    s32 i;
    s32 tableIndex;
    s32 textureId;

    if (tex != NULL) {
        tex->numberOfInstances--;
        if (tex->numberOfInstances <= 0) {
            for (i = 0; i < D_800D2FE0; i++) {
                tableIndex = i << 1;
                if (tex == ((TextureHeader **)D_800D2FD8)[tableIndex + 1]) {
                    textureId = -1;
                    mmFree(tex);
                    ((s32 *)D_800D2FD8)[tableIndex] = textureId;
                    ((TextureHeader **)D_800D2FD8)[tableIndex + 1] =
                        (TextureHeader *)textureId;
                    break;
                }
            }
        }
    }
}
/* PLATEAU-HANDOFF:func_800347A0:start
 * symbol: func_800347A0
 * score: 41/43 words
 * frame: 0x30 (target 0x28)
 * relocations: 7
 * first-mismatch: +0x0
 * summary: JFG-shaped raw cache indexing fixes 25 words and every instruction/register/relocation; only the two symmetric frame-adjust immediates remain.
 * PLATEAU-HANDOFF:func_800347A0:end
 */
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_35024/func_800347A0.s")
#endif
/* Bounded full-TU reproof (2026-09-04): configured C is exact-sized and
 * frameless at 11/21 words, first +0x8, with four resolved static relocation
 * identities; three retain the target offset/type. The target keeps the
 * texture argument in a0 and computes each cache entry in a2, while V0 copies
 * the argument to a2 and uses a0 for the entry. Ten natural loop, declaration,
 * parameter-type, and pointer-lifetime forms were tested. Direct indexing
 * shrank to 19 instructions, explicit early exit grew to 23, a named cache
 * base worsened allocation, and the remaining forms were byte-identical to V0
 * (SHA-1 b7a49009e2a8). Preserve the fallback pending a new natural
 * parameter-versus-entry allocation mechanism. */
#ifdef NON_MATCHING
s32 func_8003484C(void *texture) {
    s32 i = 0;

    if (D_800D2FE0 > 0) {
        do {
            TextureCacheEntry *entry = &((TextureCacheEntry *)D_800D2FD8)[i];
            i++;
            if (texture == entry->texture) {
                return entry->id;
            }
        } while (i < D_800D2FE0);
    }
    return -1;
}
/* PLATEAU-HANDOFF:func_8003484C:start
 * symbol: func_8003484C
 * score: 11/21 words
 * frame: frameless
 * relocations: 4
 * first-mismatch: +0x8
 * summary: Ten natural forms leave the original exact-sized loop best; its texture argument and computed-entry register roles remain reversed.
 * PLATEAU-HANDOFF:func_8003484C:end
 */
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_35024/func_8003484C.s")
#endif
