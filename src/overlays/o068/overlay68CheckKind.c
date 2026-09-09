#include "PR/ultratypes.h"

/* DKR v77/v80 and JFG searches found no exact donor for this probe check. */

typedef struct Overlay68KindPair {
    s8 kind;
    s8 amount;
} Overlay68KindPair;

typedef struct Overlay68Probe {
    u8 pad00[0x10];
    void *slots[4];
    s16 values[4];
} Overlay68Probe;

typedef struct Overlay68ResidentEntry {
    s32 thresholdNumerator;
    u8 pad04[0x1C];
} Overlay68ResidentEntry;

/* Both aliases bind to the same overlay-local kind/amount table. */
extern const Overlay68KindPair gOverlay68KindMapInitial[];
extern volatile const s8 gOverlay68KindMapLoop;

/* The runtime relocation stream preserves these five distinct call roles. */
extern void *func_8002B280(s32 size, u32 colourTag);
extern s32 piRomLoadSection(u32 assetIndex, u32 address,
                                    s32 assetOffset, s32 size);
extern u8 *func_800291C4(void);
extern s32 levelGetBlurEffect(s32 kind);
extern void mmFree(void *probe);

/*
 * Bounded plateau (2026-08-30): a declaration-slot census reproduces the
 * exact 0x48-byte frame and all four call-crossing stack homes, improving the
 * configured candidate from 18 to 10 differing words. A fidelity-gated UOPT
 * trace (2026-08-31) authenticates 13 integer allocation decisions (11 color,
 * two split). Moving the zero-cursor lifetime across the division or calls,
 * and splitting the pointer carrier into two statements, all reproduce V0
 * byte-for-byte. A single hash-bound split-provenance capture (2026-09-01)
 * identifies split webs 42 and 0, but the producer exposes no source-semantic
 * or virtual/final stack-home fields, so it cannot select an admissible C
 * change. The 2026-09-06 binding repair authenticates five resident callee
 * ABIs from ROM destinations and matched C. All nine static/runtime identities
 * and offsets/types now align through the existing mixed per-TU alias chain.
 * Raw instructions remain unchanged: 320 bytes, frame 0x48 and ten normalized
 * word differences, first +0x50. The 2026-09-04 byte-offset cursor reshape was
 * byte-identical; that negative result does not prove source unreachability.
 * Earlier flag/batch history remains; this binding repair adds no match credit.
 *
 * 2026-09-10, lane nm-ovlsmall: the ten words separate cleanly into one `bne`
 * operand order at +0x50 and a nine-word cluster around the value cursor, and
 * the cluster now has a named cause. The target forms the cursor base as
 * `sll t4,zero,0x1; addu v1,a1,t4` -- it shifts the ZERO REGISTER, so uopt
 * knew the index was zero and propagated `$zero` into it, yet the address
 * arithmetic was never folded away. Every provably-zero carrier folds instead
 * to `move v1,a1`: `cursorIndex = 0` at the use, hoisted to the top of the
 * function, hoisted above the allocation call, taken from `result` (which is
 * zero there), from `index = 0`, from `amount - amount`, written as
 * `&((s16 *)probe)[...]`, or multiplied by one. A carrier uopt cannot fold
 * does survive as a shift-and-add: substituting the (semantically wrong,
 * diagnostic-only) live `index` drops the residual from ten words to five and
 * reduces the remaining cluster to a two-register temp-ring shift. So the open
 * question is precisely what supplies an index that is zero at run time and
 * unknown at compile time -- not whether the shift-and-add shape is reachable.
 * Also byte-flat: writing either `!= -1` test with the literal first, which
 * leaves the `bne` operand order unchanged.
 */
#ifdef NON_MATCHING
s32 overlay68CheckKind(s32 kind) {
    s32 amount;
    const Overlay68KindPair *mapping;
    volatile const s8 *loopMapping;
    s32 currentKind;
    s32 result;
    s32 threshold;
    Overlay68Probe *probe;
    s32 value;
    Overlay68ResidentEntry *entries;
    s32 index;
    s16 *valueCursor;
    s32 cursorIndex;

    result = 0;
    amount = -1;
    mapping = gOverlay68KindMapInitial;

    if (mapping->kind != -1) {
        loopMapping = &gOverlay68KindMapLoop;
        currentKind = *loopMapping;
        do {
            if (kind == currentKind) {
                amount = mapping->amount;
                break;
            }
            mapping++;
            currentKind = mapping->kind;
        } while (currentKind != -1);
    }

    if (amount != -1) {
        probe = func_8002B280(sizeof(*probe), 0x85);
        if (probe != NULL) {
            piRomLoadSection(0x3F, (u32)probe,
                                    amount * (s32)sizeof(*probe),
                                    sizeof(*probe));
            entries = (Overlay68ResidentEntry *)func_800291C4();
            index = levelGetBlurEffect(kind);
            threshold = entries[index].thresholdNumerator / 5;
            cursorIndex = 0;
            valueCursor = (s16 *)probe + cursorIndex;
            if (threshold == 0) {
                threshold = 0x7080;
            }

            value = 0x8CA0;
            index = 0;
            if (probe->slots[0] != NULL) {
                do {
                    index++;
                    value = valueCursor[0x10];
                    valueCursor++;
                } while ((index < 4) && (probe->slots[index] != NULL));
            }

            if (threshold < value) {
                result = 1;
            }
        }
        mmFree(probe);
    }

    return result;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o068/overlay68CheckKind/func_overlay_068_F000146C_18C85CC.s")
#endif

/* PLATEAU-HANDOFF:overlay68CheckKind:start
 * symbol: overlay68CheckKind
 * score: 70/80 words
 * frame: 0x48
 * relocations: 9
 * first-mismatch: +0x50
 * summary: Workbench verdict structure-mismatch, lever none-known; five fresh source spellings were byte-flat. Next capture a stock-fidelity UGEN ring/line-order trace.
 * PLATEAU-HANDOFF:overlay68CheckKind:end
 */
