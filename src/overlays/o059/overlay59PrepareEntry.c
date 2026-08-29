#include "PR/ultratypes.h"

/*
 * PROVENANCE: Mickey-derived from this overlay's owned assembly and runtime
 * relocation tables; pinned DKR/JFG scans found no donor.
 *
 * Reproof lead (re-audited 2026-08-29): the historical pre-split-carrier
 * same-body configured full-TU and isolated C were measured at 52/62 raw and
 * 53/62 normalized words, 0xF8 bytes/frame 0x28, first raw +0x10 and
 * substantive +0x54. No attributable candidate object, comparison report, or
 * linked-C proof survives, so the current source's score, frame, extent, and
 * emitted relocations are unknown.
 * Ten raw sites remain at +0x10/+0x54/+0x5C/+0x68/+0x88/
 * +0x8C/+0x90/+0x98/+0xB8/+0xC0; normalizing the table LO16 leaves nine
 * descriptor/call-argument carrier web. The owned range is overlay
 * +0x70..+0x168 (ROM 0x18B87C0..0x18B88B8), with no target padding and the next
 * function beginning at +0x168; the separate +0xA1C..+0xA20 padding is not
 * owned. The LOCAL table pair resolves through
 * base +0xA20 plus +0x5A4 to module +0xFC4. The other four records are local
 * JUMPs to overlay59Release at +0x48/+0xD0 and resident ORT 135 calls to
 * func_80034448 at +0x64/+0x94. The synthetic target
 * object collapses all four zero-field calls to one placeholder and cannot
 * prove their identities. No linked candidate-C proof survives.
 * The s32 three-argument ABI is called only from four sites in
 * overlay59Advance at module +0x418/+0x590/+0x614/+0x66C; it is not exported
 * and has no resident or cross-overlay inbound. Historical natural-form, flag,
 * trace, and permutation outcomes have no surviving attributable result set
 * and do not prove exhaustion. The retained function-specific structural
 * oracle is negative (best similarity 0.106, with no credible donor). The
 * current policy-clean source splits descriptor input from acquisition result;
 * this alias-correct V1 is uncompiled. Retain 119 configurations including V0,
 * trace the web, then try one scope/line form only after strict gain; hard stop
 * at 121 deterministic builds plus one trace, with no generic batch.
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
    u32 descriptorValue;
    u32 handle;
    s32 count;
    s32 result;

    descriptor = gOverlay59DescriptorTables[tableIndex].descriptors[itemIndex];
    result = 1;
    if (descriptor != entry->owner) {
        overlay59PrepareReleaseReloc(entry);
        entry->owner = descriptor;
        count = 0;
        descriptorValue = descriptor->first;
        if (descriptorValue != 0) {
            do {
                handle = overlay59PrepareAcquireReloc(descriptorValue);
                if (handle == 0) {
                    result = 0;
                } else {
                    entry->handles[count] = handle;
                    count++;
                }

                descriptorValue = descriptor->second;
                if (descriptorValue != 0) {
                    handle = overlay59PrepareAcquireReloc(descriptorValue);
                    if (handle == 0) {
                        result = 0;
                    } else {
                        entry->handles[count] = handle;
                        count++;
                    }
                }
                descriptor++;
                descriptorValue = descriptor->first;
            } while (descriptorValue != 0);
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
