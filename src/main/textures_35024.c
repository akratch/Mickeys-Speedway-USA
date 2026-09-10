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

typedef union TextureCacheWord {
    s32 id;
    TextureHeader *texture;
} TextureCacheWord;

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
/* PROVENANCE: control flow is adapted from Jet Force Gemini's public
 * src/textures.c::texFreeTexture. Mickey's raw two-word cache layout, callers,
 * and exact compiled bytes remain authoritative. */
void func_800347A0(TextureHeader *tex) {
    s32 i;

    if (tex != NULL) {
        tex->numberOfInstances--;
        if (tex->numberOfInstances <= 0) {
            for (i = 0; i < D_800D2FE0; i++) {
                if (tex == ((TextureCacheWord *)D_800D2FD8)[(i << 1) + 1]
                               .texture) {
                    mmFree(tex);
                    tex = (TextureHeader *) -1;
                    ((TextureCacheWord *)D_800D2FD8)[i << 1].texture = tex;
                    ((TextureCacheWord *)D_800D2FD8)[(i << 1) + 1].texture =
                        tex;
                    break;
                }
            }
        }
    }
}
/* Matched from a 5-word plateau by one edit: indexing the cache as the
 * two-word pair table the rest of this TU already uses. The residual was a
 * uniform +1 rotation of ugen's integer temp ring -- the target's shift and
 * field-load temps are t7/t8 where a hand-scaled byte offset gives t6/t7 --
 * and an index that is scaled twice (by TEXTURE_CACHE_PTR and again by the
 * union's own element size) buys exactly the one extra ring pop that rotation
 * needs, with no instruction. The hand-scaled `(s32)cache + (i << 3)` forms
 * that preceded this could not: every spelling that kept both shifts cost two
 * instructions and every spelling that folded them lost the pop.
 * func_800347A0 above indexes the same table the same way. */
s32 func_8003484C(void *texture) {
    s32 i;

    for (i = 0; i < D_800D2FE0; i++) {
        if (texture == ((TextureCacheWord *)D_800D2FD8)[TEXTURE_CACHE_PTR(i)].texture) {
            return ((TextureCacheWord *)D_800D2FD8)[TEXTURE_CACHE_ID(i)].id;
        }
    }
    return -1;
}
