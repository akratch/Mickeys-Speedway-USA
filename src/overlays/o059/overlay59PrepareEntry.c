#include "PR/ultratypes.h"

/*
 * PROVENANCE: Mickey-derived from this overlay's owned assembly and runtime
 * relocation tables; pinned DKR/JFG scans found no donor.
 *
 * Plateau (re-audited 2026-08-29): retained pre-HEAD configured full-TU and
 * configured-recipe isolated C agree at 0xF8 bytes/62 words with frame 0x28.
 * Ten raw sites remain at +0x10/+0x54/+0x5C/+0x68/+0x88/
 * +0x8C/+0x90/+0x98/+0xB8/+0xC0; normalizing the table LO16 leaves nine
 * descriptor/call-argument carrier web. The LOCAL table pair resolves through
 * base +0xA20 plus +0x5A4 to module +0xFC4. The other four records are local
 * JUMPs to overlay59Release at +0x48/+0xD0 and resident ORT 135 calls to
 * func_80034448 at +0x64/+0x94. The synthetic target
 * object collapses all four zero-field calls to one placeholder and cannot
 * prove their identities. No linked candidate-C proof survives.
 * Historical natural-form, flag, trace, and permutation outcomes have no
 * surviving attributable result set and do not prove exhaustion. Current alias
 * spelling is uncompiled. Retain exactly 119 flags including V0, trace the
 * descriptor web, try one distinct input/result carrier, then only after strict
 * gain one trace-selected scope/line form; cap 121 stock builds plus trace.
 */

typedef struct Overlay59Descriptor {
    u32 first;
    u32 second;
    u8 pad08[8];
} Overlay59Descriptor;

typedef struct Overlay59Entry {
    u8 pad00[0x20];
    Overlay59Descriptor *owner;
    u32 handles[8];
} Overlay59Entry;

typedef struct Overlay59DescriptorGroup {
    Overlay59Descriptor **descriptors;
    u32 unused;
} Overlay59DescriptorGroup;

extern Overlay59DescriptorGroup gOverlay59DescriptorTables[];
extern void overlay59PrepareReleaseReloc(Overlay59Entry *entry);
extern u32 overlay59PrepareAcquireReloc(u32 value);

#ifdef NON_MATCHING
s32 overlay59PrepareEntry(Overlay59Entry *entry, s32 tableIndex, s32 itemIndex) {
    Overlay59Descriptor *descriptor;
    u32 handle;
    s32 count;
    s32 result;

    descriptor = gOverlay59DescriptorTables[tableIndex].descriptors[itemIndex];
    result = 1;
    if (descriptor != entry->owner) {
        overlay59PrepareReleaseReloc(entry);
        entry->owner = descriptor;
        count = 0;
        if ((handle = descriptor->first) != 0) {
            do {
                if ((handle = overlay59PrepareAcquireReloc(handle)) == 0) {
                    result = 0;
                } else {
                    entry->handles[count] = handle;
                    count++;
                }

                if ((handle = descriptor->second) != 0) {
                    if ((handle = overlay59PrepareAcquireReloc(handle)) == 0) {
                        result = 0;
                    } else {
                        entry->handles[count] = handle;
                        count++;
                    }
                }
                descriptor++;
            } while ((handle = descriptor->first) != 0);
        }

        if (result == 0) {
            overlay59PrepareReleaseReloc(entry);
        }
    }
    return result;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o059/overlay59PrepareEntry/func_overlay_059_F0000070_18B87C0.s")
#endif
