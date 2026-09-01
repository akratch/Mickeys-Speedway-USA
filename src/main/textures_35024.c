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
#ifdef NON_MATCHING
void func_80034434(s32 value) {
    D_8007BD80 = value & 0xFF;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_35024/func_80034434.s")
#endif
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
void func_800347A0(TextureHeader *tex) {
    s32 i;

    if (tex != NULL) {
        tex->numberOfInstances--;
        if (tex->numberOfInstances <= 0) {
            for (i = 0; i < D_800D2FE0; i++) {
                if (tex == D_800D2FD8[i].texture) {
                    mmFree(tex);
                    D_800D2FD8[i].id = -1;
                    D_800D2FD8[i].texture = (TextureHeader *)-1;
                    break;
                }
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_35024/func_800347A0.s")
#endif
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
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/textures_35024/func_8003484C.s")
#endif
