#include "PR/ultratypes.h"

/* DKR v77/v80 and JFG searches found no exact donor for this resource path. */

typedef struct Overlay68KindPair {
    s8 kind;
    s8 amount;
} Overlay68KindPair;

typedef struct Overlay68Probe {
    s32 assetOffsets[4];
    s32 assetSizes[4];
    s16 values[4];
} Overlay68Probe;

typedef struct Overlay68EntryHeader {
    s32 active;
    u8 field4;
    u8 timer;
    s16 index;
    s16 width;
    s16 generation;
    void *payload;
} Overlay68EntryHeader;

typedef struct Overlay68ResidentEntry {
    s32 thresholdNumerator;
    u8 pad04[0x1C];
} Overlay68ResidentEntry;

extern const Overlay68KindPair gOverlay68KindMap[];
extern Overlay68EntryHeader *gOverlay68SecondaryEntry;

extern s32 overlay68PayloadLimit(void);
extern void *overlay68AllocReloc(s32 size, s32 tag);
extern s32 overlay68RomLoadSectionReloc(u32 assetIndex, u32 address,
                                       s32 assetOffset, s32 size);
extern Overlay68ResidentEntry *overlay68GetResidentEntriesReloc(void);
extern s32 overlay68GetBlurEffectReloc(s32 kind);
extern void overlay68ReleaseReloc(void *resource);

/*
 * Retained pre-cleanup configured full-TU and isolated C are diagnostic
 * 111/122 literal and 113/122 runtime-normalized words, frame 0x40, first
 * substantive +0x58, with all 19 runtime record sites. The former duplicate
 * map identity, mappingStart, loopMapping, valueCursor, false pointer fields,
 * and split aliases were allocation/reload aids and are removed. Clean,
 * identity-correct V0 is uncompiled. Retain 119 flags and one allocator trace,
 * then try sentinel order and one trace-selected stack-home form, combining
 * only strict gains. Hard cap: 122 stock builds plus one trace; annotated batch
 * only after a policy-clean gain. ORT 1163's sole inbound is
 * func_80004FE0+0x4C8.
 */
#ifdef NON_MATCHING
void overlay68RebuildSecondaryEntry(s32 kind) {
    s32 amount;
    const Overlay68KindPair *mapping;
    Overlay68EntryHeader *entry;
    Overlay68Probe *probe;
    Overlay68ResidentEntry *entries;
    s32 currentKind;
    s32 threshold;
    s32 index;

    gOverlay68SecondaryEntry = 0;
    amount = -1;
    mapping = gOverlay68KindMap;

    if (mapping->kind != -1) {
        currentKind = mapping->kind;
        do {
            if (kind == currentKind) {
                amount = mapping->amount;
                break;
            }
            mapping++;
            currentKind = mapping->kind;
        } while (-1 != currentKind);
    }

    if (amount != -1) {
        entry = overlay68AllocReloc(overlay68PayloadLimit(), 0x85);
        if (entry != 0) {
            probe = overlay68AllocReloc(sizeof(*probe), 0x85);
            if (probe != 0) {
                overlay68RomLoadSectionReloc(0x3F, (u32)probe,
                                             amount * (s32)sizeof(*probe),
                                             sizeof(*probe));
                entries = overlay68GetResidentEntriesReloc();
                index = overlay68GetBlurEffectReloc(kind);
                threshold = entries[index].thresholdNumerator / 5;
                if (threshold == 0) {
                    threshold = 0x7080;
                }

                index = 0;
                if ((probe->assetSizes[0] != 0) &&
                    (threshold < probe->values[0])) {
                    do {
                        index++;
                    } while ((index < 4) &&
                             (probe->assetSizes[index] != 0) &&
                             (threshold < probe->values[index]));
                }

                if (index >= 4) {
                    index = 3;
                } else if (probe->assetSizes[index] == 0) {
                    index--;
                }

                if (index >= 0) {
                    overlay68RomLoadSectionReloc(0x40, (u32)entry,
                                                 probe->assetOffsets[index],
                                                 probe->assetSizes[index]);
                    entry->payload = entry + 1;
                    gOverlay68SecondaryEntry = entry;
                }
                overlay68ReleaseReloc(probe);
            }
            if (gOverlay68SecondaryEntry == 0) {
                overlay68ReleaseReloc(entry);
            }
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o068/overlay68RebuildSecondaryEntry/func_overlay_068_F0001250_18C83B0.s")
#endif
