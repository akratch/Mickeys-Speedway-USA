#include "PR/ultratypes.h"

typedef struct Overlay20RemoveOwner {
    u8 pad0[0x84];
    void *entry;
} Overlay20RemoveOwner;

extern void *gOverlay20Entries[];
extern void *gOverlay20ShiftEntries[];
extern s32 gOverlay20EntryCount;
extern u8 gOverlay20MarkerEnd;
extern u32 gOverlay20ActiveBits;

/* PROVENANCE: indexed search and list-compaction loops adapted from Diddy Kong
 * Racing's published src/weather.c::lensflare_override_remove. Mickey's owner
 * offset, arrays, marker cleanup, relocations, and target bytes remain
 * authoritative. */
/* Bounded plateau (2026-09-08): configured C is 51/53 words, frameless, with
 * exact 0xD4 ownership and all ten relocation tuples and identities. The
 * donor-style indexed compaction removes two pool webs and makes the 12-web
 * temporary lane exact. Capturing the decremented count in new_var removes
 * four residual words; the remaining two words are a pool-color tie where
 * the target uses v0 and IDO chooses a2.
 * Fidelity-clean proc-0 tracing identifies an invisible v0 web that conflicts
 * with the end web. A diagnostic split plus forced cursor color reaches the
 * target pool assignments but introduces a stack frame, so it is not a valid
 * promotion. The bounded forced-color permuter follow-up was flat at this
 * 51/53 result. Pointer-, index-, cursor-relative-, explicit-base-, register-,
 * byte-offset-, and association variants were exhausted. IDO's trailing 0xC
 * is section alignment, not target padding. */
/* 2026-09-09: re-measured unchanged at two words, 53 instructions, frameless,
 * and the single site is `addu v0,t0,t9` against `addu a2,t0,t9` -- the
 * compaction loop's destination base. The temp and shared lanes are exact 6/6
 * and 6/6; only pool slot 9 differs, so this is uopt colouring and the ugen
 * ring model that closed overlay7DispatchSelection and the o001 middle pair
 * does not reach it. Newly eliminated: giving the count its own local instead
 * of reusing the `owner` parameter (25 words), giving the marker pointer its
 * own local (11), the natural `for` search loop in place of the m2c
 * `do/while(1)` (44 words and one instruction fewer), and dropping the
 * `new_var` bound carrier (6). The parameter reuse is load-bearing, not an m2c
 * artefact to be cleaned up. Next lever is unchanged: the invisible v0 web that
 * interferes with the compaction base, which needs the instrumented uopt
 * capture rather than a source spelling.
 *
 * 2026-09-09, lane fin-misc: about 250 more candidates against the real object
 * on a 64 ms loop, every one of them 2/53 at the same two words. The colour
 * order is readable straight out of the object -- the target reuses the colour
 * `entry` had (v0) for the compaction bound while IDO reuses the one the
 * &gOverlay20EntryCount address had (a2), and both are dead at that point, so
 * a2 is simply the more recently freed of the two. Newly eliminated: all six
 * declaration orders and four extra-local shapes (declaration order does not
 * drive this, as the field guide says); a 63-cell colour-reserving dead-store
 * sweep -- `dead = 0`, `= i`, `= owner`, `= entry`, `= NULL`,
 * `= gOverlay20EntryCount`, `= gOverlay20ShiftEntries` inserted at each of the
 * nine statement positions -- none of which reserves a colour here; eight
 * compaction-loop spellings including a re-read bound (6 words), `!=` and
 * reversed conditions, unsigned bounds (14), a named end pointer (15), and
 * `++i` in the condition; hoisting the bound above the guard (6); five search
 * loop shapes and five marker loop shapes; use-side lock breaks on the bound,
 * on `i`, on the store index and on the entry comparison; and retyping the
 * bound carrier as `void *` / `void **` or folding it back onto `entry`
 * (22-29 words, because the comparison's type changes with it). The residual
 * is not reachable by any spelling of this function. */
#ifdef NON_MATCHING
void overlay20RemoveEntry(s32 owner) {
    void *entry;
    s32 new_var;
    s32 i;

    entry = ((Overlay20RemoveOwner *)owner)->entry;
    if (entry == NULL) {
        return;
    }
    owner = gOverlay20EntryCount;
    i = 0;
    if (owner > 0) {
        do {
            if (entry == gOverlay20Entries[i]) {
                break;
            }
            i++;
            if (i < owner) {
                continue;
            }
            break;
        } while (1);
    }
    if (i >= owner) {
        return;
    }
    gOverlay20EntryCount = owner - 1;
    if (i < gOverlay20EntryCount) {
        new_var = gOverlay20EntryCount;
        do {
            gOverlay20ShiftEntries[i] = gOverlay20ShiftEntries[i + 1];
            i++;
        } while (i < new_var);
    }

    owner = (s32)&gOverlay20MarkerEnd;
    i = 31;
    do {
        if (owner != 0) {
            gOverlay20ActiveBits &= ~(1U << i);
            return;
        }
        owner -= 0x24;
    } while (i--);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o020/overlay20RemoveEntry/func_overlay_020_F0001018_18775F0.s")
#endif

/* PLATEAU-HANDOFF:overlay20RemoveEntry:start
 * symbol: overlay20RemoveEntry
 * score: 51/53 words
 * frame: frameless
 * relocations: 10
 * first-mismatch: +0x6C
 * summary: The v0-a2 tie is a free-list choice between two dead colours; ~250 further candidates across declarations, dead stores, loop shapes and lock breaks are all 2/53, so no source spelling reaches it and the instrumented uopt oracle is the only lever left.
 * PLATEAU-HANDOFF:overlay20RemoveEntry:end
 */
