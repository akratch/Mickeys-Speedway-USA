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
    u8 format;
    u8 flags;
    u8 pad04[0x1C];
} TextureHeader;

typedef struct TempTextureHeader {
    TextureHeader header;
    u32 uncompressedSize;
    u8 pad24[4];
} TempTextureHeader;

extern s32 D_800D2FE0;
extern s32 *D_800D2FD8;
extern s32 D_800D2FE8[2];
extern s32 *D_800D2FD0[2];
extern s32 D_800D2FF0;
extern TempTextureHeader *D_800D301C;
extern s32 D_8007A278;
extern u8 *D_8007BDA4;
extern s32 D_8007BD84;
extern void *func_8002B314(s32 size, u32 colourTag);
extern void *func_8002B524(s32 size, u8 *address, u32 colourTag);
extern void mmFree(void *ptr);
extern s32 piRomLoadSection(u32 assetIndex, u32 address, s32 assetOffset,
                            s32 size);
extern s32 byteswap32(u8 *address);
extern void func_8004D7E0(void *compressed, void *output);
extern void *align16(void *address);
extern void func_80035E88(TextureHeader *texture, void *displayList);

#define TEXTURE_CACHE_ID(index) (((index) << 1) + 0)
#define TEXTURE_CACHE_PTR(index) (((index) << 1) + 1)
void func_80034424(u8 enabled) {
    D_8007BD94 = enabled;
}
void func_80034434(s32 value)
{
  s32 *new_var;
  D_8007BD80 = (*(new_var = &value)) & 0xFF;
}
#ifdef NON_MATCHING
/* PROVENANCE: control flow is adapted from the public Diddy Kong Racing
 * src/textures_sprites.c::load_texture and compared with Jet Force Gemini's
 * public texLoadTexture object. Mickey's globals, layouts, helpers, and
 * compiled bytes remain authoritative. The retained configured candidate has
 * the exact 0x50 frame and reconstructs the complete cache, asset, allocation,
 * decompression, and overflow paths. It is 215 instructions against 214,
 * differs at 118 positional words (normalized distance 58), and first differs
 * at +0x58. All 53 relocation identities resolve and 37 align exactly. Ten
 * bounded structure, type, declaration, and expression forms leave a one-word
 * aligned-address copy plus an upstream ugen temporary-ring rotation. */
TextureHeader *func_80034448(s32 textureId) {
    s32 i;
    TextureHeader *texture;
    s32 assetIndex;
    s32 assetOffset;
    s32 assetSize;
    s32 assetSection;
    s32 slotIndex;
    s32 tableType;
    s32 uncompressedSize;
    s32 loadSize;

    textureId &= 0xFFFF;
    assetIndex = textureId;
    assetSection = 2;
    tableType = 0;
    if (textureId & 0x8000) {
        tableType = 1;
        assetIndex = textureId & 0x7FFF;
        assetSection = 0;
    }
    if (assetIndex >= D_800D2FE8[tableType] || assetIndex < 0) {
        textureId = 0;
    }

    for (i = 0; i < D_800D2FE0; i++) {
        if (textureId == D_800D2FD8[TEXTURE_CACHE_ID(i)]) {
            texture = (TextureHeader *)D_800D2FD8[TEXTURE_CACHE_PTR(i)];
            texture->numberOfInstances++;
            return texture;
        }
    }

    assetOffset = D_800D2FD0[tableType][assetIndex];
    assetSize = D_800D2FD0[tableType][assetIndex + 1] - assetOffset;
    D_8007A278 = textureId;
    piRomLoadSection(assetSection, (u32)D_800D301C, assetOffset, 0x28);

    if (!(D_800D301C->header.flags & 1)) {
        if (D_8007BDA4 != NULL) {
            texture = func_8002B524(assetSize + 0x90, D_8007BDA4,
                                    D_8007BD84);
        } else {
            texture = func_8002B314(assetSize + 0x90, D_8007BD84);
        }
        D_8007A278 = -1;
        if (texture == NULL) {
            return NULL;
        }
        piRomLoadSection(assetSection, (u32)texture, assetOffset, assetSize);
    } else {
        uncompressedSize = byteswap32((u8 *)&D_800D301C->uncompressedSize);
        loadSize = uncompressedSize;
        if (loadSize < assetSize) {
            loadSize = assetSize;
        } else {
            loadSize = uncompressedSize;
        }
        loadSize += 0x20;
        if (D_8007BDA4 != NULL) {
            texture = func_8002B524(loadSize + 0x90, D_8007BDA4,
                                    D_8007BD84);
        } else {
            texture = func_8002B314(loadSize + 0x90, D_8007BD84);
        }
        D_8007A278 = -1;
        if (texture == NULL) {
            return NULL;
        }
        piRomLoadSection(
            assetSection,
            (((s32)texture + loadSize) - assetSize) -
                ((((s32)texture + loadSize) - assetSize) % 16),
            assetOffset, assetSize);
        func_8004D7E0(
            (void *)(((((s32)texture + loadSize) - assetSize) -
                      ((((s32)texture + loadSize) - assetSize) % 16)) +
                     0x20),
            texture);
        assetSize = uncompressedSize;
    }

    slotIndex = -1;
    for (i = 0; i < D_800D2FE0; i++) {
        if (D_800D2FD8[TEXTURE_CACHE_ID(i)] == -1) {
            slotIndex = i;
        }
    }
    if (slotIndex == -1) {
        slotIndex = D_800D2FE0;
        D_800D2FE0++;
    }
    D_800D2FD8[TEXTURE_CACHE_ID(slotIndex)] = textureId;
    D_800D2FD8[TEXTURE_CACHE_PTR(slotIndex)] = (s32)texture;
    func_80035E88(texture, align16((void *)((s32)texture + assetSize)));
    if (D_800D2FF0 >= 0x280) {
        return NULL;
    }
    if (D_800D2FE0 >= 0x2BD) {
        return NULL;
    }
    return texture;
}
/* PLATEAU-HANDOFF:func_80034448:start
 * symbol: func_80034448
 * score: 118 differing words; normalized distance 58
 * frame: 0x50 (target 0x50)
 * relocations: 53
 * first-mismatch: +0x58
 * summary: DKR/JFG loader structure restores the complete semantic path and exact frame; one aligned-address copy and an upstream temp-ring rotation leave 215 candidate versus 214 target instructions, with 37 relocation sites exact.
 * PLATEAU-HANDOFF:func_80034448:end
 */
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
