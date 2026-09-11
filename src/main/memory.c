/*
 * Resident allocator -- ROM 0x2BCD0-0x2C8C0 (VRAM 0x8002B0D0).
 *
 * The linked pre-split object owns exactly 0xBF0 bytes. The last function ends
 * at ROM 0x2C8B4 and the final 0xC bytes are 16-byte alignment padding; the
 * next linked object begins at ROM 0x2C8C0.
 *
 * PROVENANCE: the candidate names recorded beside the still-unmatched stubs
 * were read from Jet Force Gemini's published src/memory.c and src/memory.h.
 * They are not adopted as Mickey symbols merely from source order. JFG is a
 * permitted public retail-derived decomp under docs/CLEANROOM.md.
 */

#include "game/memory.h"

extern u8 D_8007A274;
MemoryPool D_800D1C60[MEMORY_POOL_COUNT];
s32 D_800D1CA0;
s32 sMemoryPoolCountPadding;
void *D_800D1CA8[256];
u8 D_800D20A8[256];
s32 D_800D21A8;
s32 D_800D21AC;
s32 D_800D21B0;
s32 D_800D21B4;
#define D_800D1C64 (D_800D1C60[0].slots)
extern u8 D_800D8750[];

MemoryPoolSlot *func_8002B1A0(MemoryPoolSlot *slots, s32 poolSize, s32 numSlots);

/* PROVENANCE: adapted from JFG src/memory.c:mmInit. */
void mmInit(void) {
    D_800D1CA0 = -1;
    if (D_8007A274) {
        D_800D21B4 = 0x80600000;
    } else {
        D_800D21B4 = 0x80400000;
    }
    func_8002B1A0((MemoryPoolSlot *)D_800D8750,
                  D_800D21B4 - (s32)D_800D8750, 0x640);
    mmSetDelay(2);
    D_800D21A8 = 0;
}

/* PROVENANCE: adapted from JFG src/memory.c:mmExtended. */
u8 mmExtended(void) {
    return D_8007A274;
}

/* PROVENANCE: adapted from JFG src/memory.c:mmAllocRegion. */
void *func_8002B280(s32 size, u32 colourTag);

MemoryPoolSlot *func_8002B154(s32 poolDataSize, s32 numSlots) {
    s32 size;
    MemoryPoolSlot *slots;
    s32 pad;
    MemoryPoolSlot *newPool;

    size = poolDataSize + (numSlots * sizeof(MemoryPoolSlot));
    slots = (MemoryPoolSlot *)func_8002B280(size, 0x95);
    newPool = func_8002B1A0(slots, size, numSlots);
    return newPool;
}

/* PROVENANCE: adapted from JFG src/memory.c:mempool_init. */
MemoryPoolSlot *func_8002B1A0(MemoryPoolSlot *slots, s32 poolSize, s32 numSlots) {
    MemoryPoolSlot *firstSlot;
    s32 poolCount;
    s32 i;
    s32 firstSlotSize;

    poolCount = ++D_800D1CA0;
    firstSlotSize = poolSize - (numSlots * sizeof(MemoryPoolSlot));
    D_800D1C60[poolCount].maxNumSlots = numSlots;
    D_800D1C60[poolCount].curNumSlots = 0;
    D_800D1C60[poolCount].slots = slots;
    D_800D1C60[poolCount].size = poolSize;
    D_800D1C60[poolCount].freeSize = firstSlotSize;
    firstSlot = slots;
    for (i = 0; i < D_800D1C60[poolCount].maxNumSlots; i++) {
        firstSlot->index = i;
        firstSlot++;
    }
    firstSlot = &D_800D1C60[poolCount].slots[0];
    slots += numSlots;
    if ((s32)slots & 0xF) {
        firstSlot->data = (u8 *)(((s32)slots & ~0xF) + 0x10);
    } else {
        firstSlot->data = (u8 *)slots;
    }
    firstSlot->size = firstSlotSize;
    firstSlot->flags = MEMORY_SLOT_FREE;
    firstSlot->colourTagIndex = 0x95;
    firstSlot->prevIndex = -1;
    firstSlot->nextIndex = -1;
    D_800D1C60[poolCount].curNumSlots++;
    if (poolCount == MEMORY_POOL_MAIN) {
        D_800D21B0 = firstSlotSize;
    }
    return D_800D1C60[poolCount].slots;
}

extern s32 D_8007A270;
extern s32 D_8007A278;
extern s32 D_8007A27C;

s32 runlinkGetAddressInfo(u32 address, s32 *moduleId, s32 *moduleAddress, u32 **symbolName);
void *func_8002B3A8(MemoryPoolIndex poolIndex, s32 size, u32 colourTag);

/* PROVENANCE: adapted from JFG src/memory.c:mmAlloc. */
void *func_8002B280(s32 size, u32 colourTag) {
    struct {
        volatile s32 address;
        s32 moduleAddress;
        s32 moduleId;
        s32 pad;
    } stack;

    stack.address = 0x666;
    D_8007A270 = colourTag;
    if (D_8007A278 != -1) {
        colourTag = D_8007A278 | 0xFF000000;
    } else if (D_8007A27C != -1) {
        colourTag = D_8007A27C | 0xFE000000;
    } else {
        runlinkGetAddressInfo(stack.address - 8, &stack.moduleId, &stack.moduleAddress, NULL);
        colourTag = (stack.moduleId << 24) | stack.moduleAddress;
    }
    return func_8002B3A8(MEMORY_POOL_MAIN, size, colourTag);
}

/* PROVENANCE: adapted from JFG src/memory.c:mmAlloc2. */
void *func_8002B314(s32 size, u32 colourTag) {
    struct {
        volatile s32 address;
        s32 moduleAddress;
        s32 moduleId;
        s32 pad;
    } stack;

    stack.address = 0x666;
    D_8007A270 = colourTag;
    if (D_8007A278 != -1) {
        colourTag = D_8007A278 | 0xFF000000;
    } else if (D_8007A27C != -1) {
        colourTag = D_8007A27C | 0xFE000000;
    } else {
        runlinkGetAddressInfo(stack.address - 8, &stack.moduleId, &stack.moduleAddress, NULL);
        colourTag = (stack.moduleId << 24) | stack.moduleAddress;
    }
    return func_8002B3A8(MEMORY_POOL_MAIN, size, colourTag);
}

/* PROVENANCE: adapted from JFG src/memory.c:mempool_slot_find. */
void *func_8002B3A8(MemoryPoolIndex poolIndex, s32 size, u32 colourTag) {
    s32 slotSize;
    MemoryPoolSlot *slot;
    volatile s32 pad;
    MemoryPool *pool;
    MemoryPoolSlot *slots;
    s16 nextIndex;
    s32 currIndex;

    pool = &D_800D1C60[poolIndex];
    if (pool->maxNumSlots == pool->curNumSlots + 1) {
        return NULL;
    }
    currIndex = -1;
    if (size & 0xF) {
        size = (size & ~0xF) + 0x10;
    }
    slotSize = 0x7FFFFFFF;
    slots = pool->slots;
    nextIndex = 0;
    do {
        slot = (MemoryPoolSlot *)((u8 *)slots + (nextIndex << 4) + (nextIndex << 2));
        if (slot->flags == MEMORY_SLOT_FREE) {
            if (slot->size >= size && slot->size < slotSize) {
                slotSize = slot->size;
                currIndex = nextIndex;
            }
        }
        nextIndex = slot->nextIndex;
    } while (nextIndex != -1);

    if (currIndex != -1) {
        func_8002BB40(poolIndex, currIndex, size, TRUE, FALSE, colourTag);
        return (currIndex + slots)->data;
    }
    return NULL;
}

/* PROVENANCE: adapted from JFG src/memory.c:mmAllocR. */
void *func_8002B4C0(MemoryPoolSlot *slots, s32 size) {
    s32 i;

    for (i = D_800D1CA0; i != 0; i--) {
        if (slots == D_800D1C60[i].slots) {
            return func_8002B3A8(i, size, 0);
        }
    }
    return NULL;
}

/* PROVENANCE: adapted from JFG src/memory.c:mmAllocAtAddr. Mickey's globals,
 * pool/slot layouts, absent diagnostic calls, and linked bytes are authoritative. */
/*
 * 2 differing words at delta 0: every instruction and every register now agrees
 * with the ROM, and the only residual is one compiler temp's displacement --
 * the slot-pointer save around the first split call sits at +0x38 here and at
 * +0x3C in the ROM.
 *
 * That residual is a frame-geometry identity, not an allocator one. Homes
 * descend from frame-4 in declaration order, one word each, and the first
 * compiler temp sits directly below the lowest home; the frame itself is
 * round8(48 + 4*locals + temp_area). The ROM's temp at +0x3C therefore fixes
 * six declared locals with a temp area of 13..16 bytes, whereas every form that
 * produces the two live ranges above needs a seventh symbol and so gets seven
 * homes with an 8-byte temp area. slotIndex, slot, slots, moduleId,
 * moduleAddress and callerAddress are all forced (dropping `slot` also drops
 * the slot-pointer copy the ROM has), so the seventh symbol has nowhere to go.
 * Reopening this needs a construct that either splits the value with no named
 * local or raises the temp area by eight bytes at zero instruction cost.
 * Measured and reaching neither: every lvalue spelling and cast of the value,
 * nested-block and inner-scope declarations, register/volatile/static storage
 * classes, uopt region openers and discarded-expression probes, self-assignment
 * and cast-round-trip no-ops at two insertion points, and 64-bit or array
 * locals. A wider local only adds local words, which moves the temp the wrong
 * way; note the ROM's temp at +0x3C is not eight-byte aligned, so forcing
 * alignment is the wrong direction too.
 *
 * Reuse instead of adding a symbol is the other door, and it is shut for a
 * reason worth writing down. Only three of the six locals are dead across the
 * contested region -- moduleId, moduleAddress and callerAddress are last
 * touched in the prologue, while slotIndex, slot and slots all span it -- and
 * all three are memory class, the first two because their address is taken and
 * the third because it is volatile. Carrying the value in one of them does cost
 * no home, and the frame duly drops, but it costs eight extra instructions for
 * the module pair and twenty for the volatile.
 *
 * Also worth recording because it is the premise everything above rests on: in
 * this compilation an unused local is NOT eliminated before the frame is sized.
 * An unused `s32` and an unused pointer produce identical frames, and deleting
 * one moves the whole ladder, so the former `s32 pad` was occupying a home and
 * `data` replaced it rather than adding an eighth. The local count, the frame
 * and these two words are all unchanged from the pre-edit baseline.
 *
 * The likeliest place for the missing evidence is the `callerAddress`
 * stand-in, which is the one part of this body known not to be the ROM's own
 * construct; four rewritings of it were measured and none turns its slot into a
 * compiler temp, which is what would free the sixth home.
 */
#ifdef NON_MATCHING
void *func_8002B524(s32 size, u8 *address, u32 colourTag) {
    s32 slotIndex;
    MemoryPoolSlot *slot;
    MemoryPoolSlot *slots;
    s32 moduleId;
    s32 moduleAddress;
    volatile s32 callerAddress = 0x666;
    /* `data` carries slot->data for the two range tests and the split-call
     * offset, while the equality test and the direct return keep reading
     * slot->data. That is what splits the one value into the two live ranges
     * the ROM allocates: the range-test range takes a0 (the copy into v0 then
     * fills the delay slot of the first range branch, turning a branch-likely
     * into a plain branch), which in turn pushes the slot-pointer copy to a1.
     * As one symbol the range reaches the split call's argument setup, so a0
     * is genuinely forbidden to it and no save ratio can reach a0. Declared
     * last, in place of the former `s32 pad`, so the frame stays 0x58. */
    u8 *data;

    D_8007A270 = colourTag;
    if (D_8007A278 != -1) {
        colourTag = D_8007A278 | 0xFF000000;
    } else if (D_8007A27C != -1) {
        colourTag = D_8007A27C | 0xFE000000;
    } else {
        runlinkGetAddressInfo(callerAddress - 8, &moduleId, &moduleAddress, NULL);
        colourTag = (moduleId << 24) | moduleAddress;
    }

    if (D_800D1C60[MEMORY_POOL_MAIN].curNumSlots + 1 ==
        D_800D1C60[MEMORY_POOL_MAIN].maxNumSlots) {
        return NULL;
    }
    if (size & 0xF) {
        size = (size & ~0xF) + 0x10;
    }

    slots = D_800D1C60[MEMORY_POOL_MAIN].slots;
    for (slotIndex = 0; slotIndex != -1; slotIndex = slot->nextIndex) {
        slot = (MemoryPoolSlot *)((u8 *)slots + (slotIndex << 4) + (slotIndex << 2));
        if (slot->flags == MEMORY_SLOT_FREE) {
            data = slot->data;
            if (address >= data && address + size <= data + slot->size) {
                if (address == slot->data) {
                    func_8002BB40(MEMORY_POOL_MAIN, slotIndex, size, TRUE, FALSE, colourTag);
                    return slot->data;
                }
                slotIndex = func_8002BB40(MEMORY_POOL_MAIN, slotIndex,
                                          address - data, FALSE, TRUE, colourTag);
                func_8002BB40(MEMORY_POOL_MAIN, slotIndex, size, TRUE, FALSE, colourTag);
                return *(u8 **)((u8 *)slots + (slotIndex << 4) + (slotIndex << 2));
            }
        }
    }

    return NULL;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/memory/func_8002B524.s")
#endif

/* PROVENANCE: adapted from JFG src/memory.c:mmSetDelay. */
void mmSetDelay(s32 state) {
    D_800D21AC = state;
}

/* PROVENANCE: adapted from JFG src/memory.c:mmFlushFreeStack. */
void func_8002B8A8(u8 *address);

void func_8002B700(void) {
    while (D_800D21A8 > 0) {
        func_8002B8A8(D_800D1CA8[--D_800D21A8]);
    }
}

/* PROVENANCE: adapted from JFG src/memory.c:mmFree. */
void func_8002B93C(void *dataAddress);

void mmFree(void *data) {
    volatile s32 callerAddress = 0x666;

    if (D_800D21AC == 0) {
        func_8002B8A8(data);
    } else {
        func_8002B93C(data);
    }
}

/*
 * PROVENANCE: adapted from JFG src/memory.c:mmFreeTick. Mickey's low-memory
 * link-slot release and absent diagnostic print are authoritative differences.
 */
void ReleaseUnusedLinkSlots(void);

/*
 * The second low-memory tier is JFG's: `mmFreeTick` re-reads FreeRAM under a
 * 0xC000 guard and calls into a module Mickey never links, so the guarded
 * block is empty here. The read is not decoration -- referencing D_800D21B0
 * twice is what makes IDO materialize its address into a callee-saved register
 * and load through it (`lui`/`addiu`/`lw 0(reg)`), where a single reference
 * folds the %lo into the load and loses one instruction. Recorded in
 * docs/cleanup-queue.md; seek a natural spelling with the same 63 words.
 *
 * The counter reset is on its own line, not in a `for` header, so `as1`
 * schedules it ahead of the loop preheader's hoisted D_800D20A8 address
 * instead of into the guard's delay slot (docs/ido-learnings.md, the source
 * line stamped on each emitted record).
 */
void func_8002B7AC(void) {
    s32 i;

    if (D_800D21B0 < 0x14000) {
        ReleaseUnusedLinkSlots();
        if (D_800D21B0 < 0xC000) {
        }
    }
    i = 0;
    while (i < D_800D21A8) {
        D_800D20A8[i]--;
        if (D_800D20A8[i] == 0) {
            func_8002B8A8(D_800D1CA8[i]);
            D_800D1CA8[i] = D_800D1CA8[D_800D21A8 - 1];
            D_800D20A8[i] = D_800D20A8[D_800D21A8 - 1];
            D_800D21A8--;
        } else {
            i++;
        }
    }
}

/* PROVENANCE: adapted from JFG src/memory.c:mempool_free_addr. */
s32 func_8002B978(u8 *address);
void func_8002B9D0(MemoryPoolIndex poolIndex, s32 slotIndex);

void func_8002B8A8(u8 *address) {
    s16 slotIndex;
    s32 poolIndex;
    MemoryPoolSlot *slots;
    MemoryPoolSlot *slot;

    poolIndex = func_8002B978(address);
    slots = *(MemoryPoolSlot **)((u8 *)&D_800D1C64 + (poolIndex << 4));
    for (slotIndex = 0; slotIndex != -1; slotIndex = slot->nextIndex) {
        slot = (MemoryPoolSlot *)((u8 *)slots + (slotIndex << 4) + (slotIndex << 2));
        if (address == slot->data) {
            if (slot->flags == MEMORY_SLOT_USED || slot->flags == MEMORY_SLOT_SAFEGUARD) {
                func_8002B9D0(poolIndex, slotIndex);
            }
            break;
        }
    }
}

/* PROVENANCE: adapted from JFG src/memory.c:mempool_free_queue. */
void func_8002B93C(void *dataAddress) {
    D_800D1CA8[D_800D21A8] = dataAddress;
    D_800D20A8[D_800D21A8] = D_800D21AC;
    D_800D21A8++;
}

/* PROVENANCE: adapted from JFG src/memory.c:mempool_get_pool. */
s32 func_8002B978(u8 *address) {
    s32 i;
    MemoryPool *pool;

    for (i = D_800D1CA0; i > 0; i--) {
        pool = &D_800D1C60[i];
        if ((u8 *)pool->slots >= address) {
            continue;
        }
        if (address < pool->size + (u8 *)pool->slots) {
            break;
        }
    }
    return i;
}

/* PROVENANCE: adapted from JFG src/memory.c:mempool_slot_clear. */
void func_8002B9D0(MemoryPoolIndex poolIndex, s32 slotIndex) {
    s16 nextIndex;
    s16 prevIndex;
    s16 tempNextIndex;
    MemoryPoolSlot *slots;
    MemoryPoolSlot *slot;
    MemoryPoolSlot *nextSlot;
    MemoryPoolSlot *prevSlot;

    slots = D_800D1C60[poolIndex].slots;
    slot = (MemoryPoolSlot *)((u8 *)slots + (slotIndex << 4) + (slotIndex << 2));
    nextIndex = slot->nextIndex;
    prevIndex = slot->prevIndex;
    nextSlot = (MemoryPoolSlot *)((u8 *)slots + (nextIndex << 4) + (nextIndex << 2));
    prevSlot = (MemoryPoolSlot *)((u8 *)slots + (prevIndex << 4) + (prevIndex << 2));
    slot->flags = MEMORY_SLOT_FREE;
    if (poolIndex == MEMORY_POOL_MAIN) {
        D_800D21B0 += slot->size;
    }
    D_800D1C60[poolIndex].freeSize += slot->size;
    if (nextIndex != -1 && nextSlot->flags == MEMORY_SLOT_FREE) {
        slot->size += nextSlot->size;
        tempNextIndex = nextSlot->nextIndex;
        slot->nextIndex = tempNextIndex;
        if (tempNextIndex != -1) {
            ((MemoryPoolSlot *)((u8 *)slots + (tempNextIndex << 4) +
                                (tempNextIndex << 2)))->prevIndex = slotIndex;
        }
        D_800D1C60[poolIndex].curNumSlots--;
        slots[D_800D1C60[poolIndex].curNumSlots].index = nextIndex;
    }
    if (prevIndex != -1 && prevSlot->flags == MEMORY_SLOT_FREE) {
        prevSlot->size += slot->size;
        tempNextIndex = slot->nextIndex;
        prevSlot->nextIndex = tempNextIndex;
        if (tempNextIndex != -1) {
            ((MemoryPoolSlot *)((u8 *)slots + (tempNextIndex << 4) +
                                (tempNextIndex << 2)))->prevIndex = prevIndex;
        }
        D_800D1C60[poolIndex].curNumSlots--;
        slots[D_800D1C60[poolIndex].curNumSlots].index = slotIndex;
    }
}

/* PROVENANCE: adapted from JFG src/memory.c:mmGetSlotPtr. */
MemoryPoolSlot *mmGetSlotPtr(MemoryPoolIndex poolIndex) {
    return *(MemoryPoolSlot **) ((u8 *) &D_800D1C64 + (poolIndex * sizeof(MemoryPool)));
}

/* PROVENANCE: adapted from JFG src/memory.c:mmGetDelay. */
s32 mmGetDelay(void) {
    return D_800D21AC;
}

/*
 * PROVENANCE: adapted from JFG src/memory.c:mempool_slot_assign. Mickey's
 * pool accounting, byte-sized slot fields, globals, and bytes are authoritative.
 * Canonical -O2/-mips2 C is exact for all 72 frameless words and all eight
 * relocation tuples. Reusing dead incoming/local carriers preserves the
 * allocator's slot-count and remainder-link webs without artificial code.
 */
s32 func_8002BB40(MemoryPoolIndex poolIndex, s32 slotIndex, s32 size,
                   s32 slotIsTaken, s32 newSlotIsTaken, u32 colourTag) {
    MemoryPool *pool;
    MemoryPoolSlot *slots;
    MemoryPoolSlot *slot;
    s32 index;
    s32 slotSize;

    if (slotIsTaken == TRUE) {
        if (poolIndex == MEMORY_POOL_MAIN) {
            D_800D21B0 -= size;
        }
        pool = (MemoryPool *)((u8 *)D_800D1C60 + (poolIndex << 4));
        pool->freeSize -= size;
    }

    pool = (MemoryPool *)((u8 *)D_800D1C60 + (poolIndex << 4));
    slots = pool->slots;
    slot = (MemoryPoolSlot *)((u8 *)slots + (slotIndex << 4) + (slotIndex << 2));
    slot->flags = slotIsTaken;
    slot->colourTagIndex = D_8007A270;
    slotSize = slot->size;
    slot->size = size;
    slot->colourTag = colourTag;
    if (size < slotSize) {
        slotIsTaken = pool->curNumSlots;
        index = ((MemoryPoolSlot *)((slotIsTaken * sizeof(MemoryPoolSlot)) +
                                    (u8 *)slots))->index;
        pool->curNumSlots = slotIsTaken + 1;
        ((MemoryPoolSlot *)((u8 *)slots + (index << 4) + (index << 2)))->data =
            slot->data + size;
        ((MemoryPoolSlot *)((u8 *)slots + (index << 4) + (index << 2)))->size =
            slotSize - size;
        ((MemoryPoolSlot *)((u8 *)slots + (index << 4) + (index << 2)))->flags =
            newSlotIsTaken;
        ((MemoryPoolSlot *)((u8 *)slots + (index << 4) +
                            (index << 2)))->colourTagIndex = D_8007A270;
        {
            slotSize = slot->nextIndex;

            ((MemoryPoolSlot *)((u8 *)slots + (index << 4) + (index << 2)))->prevIndex =
                slotIndex;
            ((MemoryPoolSlot *)((u8 *)slots + (index << 4) + (index << 2)))->nextIndex =
                slotSize;
            slot->nextIndex = index;
            if (slotSize != -1) {
                ((MemoryPoolSlot *)((u8 *)slots + (slotSize << 4) +
                                    (slotSize << 2)))->prevIndex = index;
            }
        }
        return index;
    }
    return slotIndex;
}

/* PROVENANCE: adapted from JFG src/memory.c:mmAlign16. */
u8 *align16(u8 *address) {
    s32 remainder = (s32) address & 0xF;

    if (remainder > 0) {
        address = (u8 *) (((s32) address - remainder) + 16);
    }
    return address;
}

/* PROVENANCE: derived from JFG src/memory.c's mmAlign16/mmAlign4 family. */
u8 *align8(u8 *address) {
    s32 remainder = (s32) address & 7;

    if (remainder > 0) {
        address = (u8 *) (((s32) address - remainder) + 8);
    }
    return address;
}

/* PROVENANCE: adapted from JFG src/memory.c:mmAlign4. */
u8 *align4(u8 *address) {
    s32 remainder = (s32) address & 3;

    if (remainder > 0) {
        address = (u8 *) (((s32) address - remainder) + 4);
    }
    return address;
}

/* PLATEAU-HANDOFF:func_8002B524:start
 * symbol: func_8002B524
 * score: 2/116 words
 * frame: 0x58
 * relocations: 12
 * first-mismatch: +0x138
 * summary: 14 -> 2 (2026-09-11, lane w3-low), and the recorded 'no source change adopted' is refuted. The 10 naming and 2 structural words were ONE decision and the structure was downstream of the colouring: the slot pointer's field was a single web where the ROM has two ranges, an argument register for the range tests plus a copy for the split-call offset. Caching the field at exactly three of its five uses, with the carrier declared last, closes all twelve; neighbouring partitions regress to 49-56. What is left is 2 immediate-only words, and 2026-09-11 lane p6-tight restates them as a local COUNT rather than a frame identity. Every declared local consumes a four-byte cell assigned from the frame top downward in declaration order, regardless of storage class, of whether it is ever referenced, and of block scope; the compiler temporary area sits strictly below the last cell. Four measured layouts fit that rule and no other, so the ROM declares exactly SIX locals and the seventh cell this candidate spends on the carrier is the whole residual. That also confirms and widens the falsification of L99's clause that an unused s32 is eliminated before the frame is sized: a register-class local is not eliminated either. Newly flat at two words: the carrier declared register in three integer spellings, retyped u32 and s32 with casts at the use sites, and declared inside the search loop or the free-slot test rather than at function scope, so block scope does not overlay cells here. Folding the two module outputs into a two-element array is 15 words; dropping the carrier and re-reading the field at all five sites is 24. The open question is exact: two live ranges for the field without a seventh declared local, which on this compiler means two expression webs from two spellings of one load that do not common with each other.
 * PLATEAU-HANDOFF:func_8002B524:end
 */
