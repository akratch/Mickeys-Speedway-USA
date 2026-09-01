/*
 * Resident animation/collision block -- ROM 0x50C00-0x58570
 * (VRAM 0x80050000-0x80057970).
 *
 * PROVENANCE -- names and structural comparisons in this file use Jet Force
 * Gemini's public decompilation, principally src/anim.c, src/hit.c, src/fmv.c,
 * and their declarations. JFG is a permitted published retail-derived decomp
 * under docs/CLEANROOM.md. Mickey's own ROM remains authoritative; the block
 * is kept under its existing 16-byte-aligned boundaries because no internal
 * whole-object boundary has yet been proved.
 *
 * Flags: -O2 -mips2 -32, inherited from the src/main/ build rule.
 */

#include "PR/ultratypes.h"
#include "game/anim.h"
#include "game/charControl.h"

void *func_8002B280();
AnimPathObject *func_8000590C(ControlSpawnPacket *packet, s32 mode);
void func_80005768(AnimPathObject *object);
void piRomLoadSection();
u8 *levelGetLevel(void);
void func_800511C4();
void func_80021504(f32 value, s32 arg1);
f32 sqrtf(f32 value);
extern void func_800031E8(void *handle);
HitCopyState **func_80005750(s32 *count);

/*
 * PROVENANCE: adapted from JFG's func_80076020_76C20. Mickey's globals and
 * final compiler output are independently established from Mickey's ROM.
 */
void func_80050000(s32 *stream) {
    D_800D6D54 = stream;
    D_800D6D58 = (u8 *) *stream;
    D_800D6D5C = 0x80;
}

/*
 * PROVENANCE: adapted from JFG's func_80076044_76C44. The bitstream globals
 * are Mickey's, and the compiled result is checked against Mickey's ROM.
 */
s32 func_80050024(u32 bitCount) {
    s32 value;

    value = 0;
    if (bitCount != 0) {
        bitCount = 1 << (bitCount + 0x1F);
        do {
            if (D_800D6D5C == 0) {
                D_800D6D58++;
                D_800D6D5C = 0x80;
            }
            if (*D_800D6D58 & D_800D6D5C) {
                value |= bitCount;
            }
            bitCount >>= 1;
            D_800D6D5C >>= 1;
        } while (bitCount != 0);
    }
    return value;
}

/*
 * PROVENANCE: adapted from JFG's func_800760C0_76CC0. Mickey's ROM fixes the
 * signed-extension expression and all generated instruction choices.
 */
s32 func_800500A4(u32 bitCount) {
    u32 signMask;
    s32 value;

    value = 0;
    if (bitCount != 0) {
        signMask = 0xFFFFFFFF << (bitCount - 1);
        bitCount = 1 << (bitCount - 1);
        do {
            if (D_800D6D5C == 0) {
                D_800D6D58++;
                D_800D6D5C = 0x80;
            }
            if (*D_800D6D58 & D_800D6D5C) {
                value |= bitCount;
            }
            bitCount >>= 1;
            D_800D6D5C >>= 1;
        } while (bitCount != 0);
        if (value & signMask) {
            value |= signMask;
        }
    }
    return value;
}

void func_8005013C(void) {
    if (D_800D6D5C != 0x80) {
        D_800D6D58++;
    }
    *D_800D6D54 = (s32) D_800D6D58;
}


/*
 * Clear the current animation-sequence cursors. The exact JFG donor assembly
 * corroborates the three-global shape; this C is reconstructed from Mickey.
 */
void func_8005017C(void) {
    if (D_8007D698 != NULL) {
        D_8007D698 = NULL;
        D_8007D69C = NULL;
        D_8007D6A0 = 0;
    }
}

s32 func_800501AC(AnimStreamEntry *entry) {
    return D_8007D6C0[(entry->command >> 8) & 0xFF];
}

/*
 * PROVENANCE: JFG asm/nonmatchings/anim/func_80076968.s corroborates the CFG;
 * this exact typed body is reconstructed from Mickey's target and m2c draft.
 */
s32 func_800501C8() {
    s32 step;
    s32 done;
    s32 entryCount;
    s32 total;
    AnimStreamEntry *cursor;

    cursor = D_8007D698;
    entryCount = 0;
    done = 0;
    total = 0;
    if (cursor != NULL) {
        do {
            if (((cursor->command >> 8) & 0xFF) == 0x7F) {
                done = 1;
            }
            step = func_800501AC(cursor);
            total += step;
            if (step != 0) {
                cursor = (AnimStreamEntry *)
                    ((u8 *) cursor + (step >> 1) * 2);
                entryCount++;
                if (entryCount >= 0x2001) {
                    goto end;
                }
            } else {
                total = 0;
end:
                done = 1;
            }
        } while (done == 0);
    }
    return total;
}

/* Exact JFG donor assembly corroborates this setup shape; C is Mickey-led. */
void func_8005027C(void) {
    s32 *base;
    s32 header;

    base = D_8007D68C;
    header = *base;
    D_8007D698 = (u8 *) base + (header & 0xFFFFFF);
    D_8007D69C = D_8007D698;
    D_8007D6A0 = func_800501C8(&D_8007D698);
}

void func_800502CC(u8 pathIndex) {
    AnimPath *path;

    path = D_800D6B00[pathIndex];
    if (path != NULL) {
        if (path->unk8 != NULL) {
            func_80006EA0(path->unk8);
        }
        mmFree(path);
    }
    D_800D6B00[pathIndex] = NULL;
}

/*
 * PROVENANCE: adapted from JFG's public
 * asm/nonmatchings/anim/animseqInitPath.s. Mickey's shorter character-table
 * selection, resident object layout, and final compiler output are
 * independently established from Mickey's ROM.
 */
void func_80050348(pathIndex)
u8 pathIndex;
{
    ControlSpawnPacket packet;
    AnimPathNode *node;
    AnimPathObject *object;
    AnimPath *path;
    s16 objectId;

    path = D_800D6B00[pathIndex];
    if (path != NULL) {
        node = path->nodes;
        if ((node != NULL) && (path->unk8 == NULL) && (path->unk2 != -1)) {
            packet.x = node->unkC;
            packet.y = (s16) node->unk10;
            packet.z = (s16) node->unk14;
            packet.mode = 0xA;
            objectId = path->unk2;
            if (objectId == 0x115) {
                packet.kind = D_8007D780[D_8007BF04 & 3][D_8007BF20 & 0xF];
            } else if (objectId == 0x11A) {
                packet.kind = D_8007D780[D_8007BF04 & 3][D_8007BF24 & 0xF];
            } else if (objectId == 0x119) {
                packet.kind = D_8007D780[D_8007BF04 & 3][D_8007BF28 & 0xF];
            } else {
                packet.kind = objectId;
            }
            object = func_8000590C(&packet, 1);
            path->unk8 = object;
            if (object != NULL) {
                object->unk3C = 0;
                object->unk6 |= 0x400;
                if (object->unk44 == 0x1D) {
                    func_80005768(path->unk8);
                }
                path->unk24 = 0xFF;
                path->unk25 = 0xFF;
                path->unk26 = 0;
                path->unk27 = 0;
                path->unk2A = 0;
                path->unk2C = 1.0f;
                path->unk30 = 0.0f;
                if (path->unk8->unk58 != NULL) {
                    path->unk8->unk58->unk132 = 0;
                }
            }
        }
        path->unk28 = 0x64;
        path->unk29 = 0;
    }
}
/* JFG's animseqResetPath assembly corroborates this Mickey-led reset. */
#pragma weak animResetTrap = TrapDanglingJump
extern s32 animResetTrap(AnimPath *, f32, s32, s32);
void func_8005055C(pathIndex)
u8 pathIndex;
{
    AnimPath *path;
    AnimPathObject *object;
    u8 flags;

    path = D_800D6B00[pathIndex];
    if (path != NULL) {
        flags = path->flags;
        if (!(flags & 8)) {
            object = path->unk8;
            path->unk10 = 1.0f;
            path->unk1 = path->unk0;
            path->unkC = path->unk4 / 16384.0f;
            path->unk1C = 0.0f;
            path->unk14 = path->unk6;
            path->unk15 = path->unk7;
            path->currentNode = path->nodes;
            path->flags = flags & 0x80;
            if (object != NULL) {
                object->unk6 |= 0x400;
                path->unk24 = 0xFF;
                path->unk25 = 0xFF;
                path->unk26 = 0;
                path->unk27 = 0;
                path->unk2A = 0;
                path->unk2C = 1.0f;
                path->unk30 = 0.0f;
                if (path->unk8->unk58 != NULL) {
                    path->unk8->unk58->unk132 = 0;
                }
                animResetTrap(path, 0.0f, 0, 0);
                if (*(s32 *) &object->soundHandle != 0) {
                    func_800031E8(*(s32 *) &object->soundHandle);
                    *(s32 *) &object->soundHandle = 0;
                }
            }
        }
    }
}
void animseqStartPath(u8 pathIndex) {
    AnimPath *path;
    AnimPathObject *object;

    path = D_800D6B00[pathIndex];
    if (path != NULL) {
        if (!(path->flags & 8) && !(path->flags & 1)) {
            object = path->unk8;
            path->flags |= 1;
            path->flags &= ~4;
            if (object != NULL) {
                object->unk6 &= ~0x400;
                if (path->unk8->unk58 != NULL) {
                    path->unk8->unk58->unk132 = 1;
                }
            }
        }
    }
}
void animseqStopPath(u8 pathIndex) {
    AnimPath *path;
    AnimPathObject *object;

    path = D_800D6B00[pathIndex];
    if (path != NULL) {
        if (!(path->flags & 8) && (path->flags & 5)) {
            object = path->unk8;
            path->flags &= ~5;
            if (object != NULL) {
                object->unk6 |= 0x400;
                if (path->unk8->unk58 != NULL) {
                    path->unk8->unk58->unk132 = 0;
                }
            }
        }
    }
}
u32 func_8005077C(u8 pathIndex) {
    AnimPath *path;
    u32 result;

    path = D_800D6B00[pathIndex];
    result = 1;
    if (path != NULL) {
        return (path->flags & 1) == 0;
    }
    return result;
}

void animseqHoldPath(u8 pathIndex) {
    AnimPath *path;

    path = D_800D6B00[pathIndex];
    if (path != NULL) {
        if (!(path->flags & 8)) {
            if (path->flags & 1) {
                path->flags &= ~1;
            } else if (path->unk8 != NULL) {
                path->unk8->unk6 &= ~0x400;
                if (path->unk8->unk58 != NULL) {
                    path->unk8->unk58->unk132 = 1;
                }
            }
            path->flags |= 4;
        }
    }
}
void animseqLockPath(u8 pathIndex) {
    AnimPath *path;

    path = D_800D6B00[pathIndex];
    if (path != NULL) {
        path->flags |= 8;
    }
}

void animseqUnLockPath(u8 pathIndex) {
    AnimPath *path;

    path = D_800D6B00[pathIndex];
    if (path != NULL) {
        path->flags &= ~8;
    }
}

AnimPath *func_800508B4(u8 pathIndex) {
    return D_800D6B00[pathIndex];
}

/*
 * PROVENANCE: adapted from JFG's public
 * asm/nonmatchings/anim/func_800772C4.s. Mickey's bit-reader calls, field
 * layout, constants, and final compiler output are independently established
 * from Mickey's ROM.
 * The unsigned scale is the literal 0.01f: 0x3C23D70A needs its low halfword,
 * so IDO materializes it with lwc1 from the TU's own literal pool, which
 * places it third in the invariant group (f26,f24,f22,f20) exactly as the
 * ROM orders it. An extern read (the old D_80083FA8 spelling) stays a
 * statement-position load and surfaces four schedule words early
 * (as1 -Wa,-R trace: the constant pairs chain on $at and carry the loop-head
 * line; only a pool literal joins that group).
 */
void func_800508D4(s32 count, AnimPathNode *node, s32 stream,
                   AnimPathNode **nodeEnd, s32 *streamEnd) {
    f32 valueFloat;
    u32 value;
    s32 wideValues;

    if (count > 0) {
        f32 halfScale = 0.5f;
        f32 wideScale = 0.390625f;
        f32 unsignedScale = 0.01f;
        f32 signedScale = 0.125f;

        do {
            func_80050000(&stream);
            node->unk6 = func_80050024(8);
            wideValues = func_80050024(1);
            node->unk7 = func_80050024(1);
            node->unkC = func_800500A4(0x12) * signedScale;
            node->unk10 = func_800500A4(0x12) * signedScale;
            node->unk14 = func_800500A4(0x12) * signedScale;
            node->unk0 = func_80050024(0xC) * 0x10;
            node->unk2 = func_80050024(0xC) * 0x10;
            node->unk4 = func_80050024(0xC) * 0x10;
            node->unk8 = (u32) func_80050024(0xC) * unsignedScale;
            if (wideValues == 0) {
                node->unk8 *= wideScale;
            }
            node->unk18 = func_800500A4(0xC) * 4;
            node->unk1A = func_800500A4(0xC) * 4;
            node->unk1C = func_800500A4(0xC) * 4;
            value = func_80050024(0xC);
            valueFloat = value;
            node->unk20 = valueFloat * halfScale;
            node->unk1E = 0;
            func_8005013C();
            count--;
            node++;
        } while (count > 0);
    }
    if (nodeEnd != NULL) {
        *nodeEnd = node;
    }
    if (streamEnd != NULL) {
        *streamEnd = stream;
    }
}
/* JFG's animseqLinkNodes assembly corroborates this Mickey-led body. */
void func_80050AD4(u8 pathIndex) {
    AnimPath *path;
    s32 nodeIndex;

    path = D_800D6B00[pathIndex];
    if (path != NULL) {
        if (path->nodeCount >= 2) {
            nodeIndex = 0;
            if (path->nodeCount > 0) {
                do {
                    if (nodeIndex > 0) {
                        path->nodes[nodeIndex].previous =
                            &path->nodes[nodeIndex - 1];
                    }
                    if (nodeIndex < path->nodeCount - 1) {
                        path->nodes[nodeIndex].next =
                            &path->nodes[nodeIndex + 1];
                    }
                    nodeIndex++;
                } while (nodeIndex < path->nodeCount);
            }
        }
        if (path->flags & 0x80) {
            path->nodes[0].previous = &path->nodes[path->nodeCount - 1];
            path->nodes[path->nodeCount - 1].next = &path->nodes[0];
        } else {
            path->nodes[0].previous = &path->nodes[0];
            path->nodes[path->nodeCount - 1].next =
                &path->nodes[path->nodeCount - 1];
        }
    }
}
/*
 * PROVENANCE: adapted from JFG's src/anim.c animseqInit assembly. Mickey's
 * globals, allocator call, data boundaries, and compiler output are
 * independently established from Mickey's ROM.
 *
 * Fresh bounded loop/type/lifetime pass: structure-mismatch, 87/87 words,
 * 15 differing words, first +0x34. Direct comparisons retain IDO's bound
 * register; a remaining-byte comparison removes it but selects a subtraction
 * branch and regresses. Five later endpoint/base address schedules remain.
 */
#ifdef NON_MATCHING
void func_80050BF4(void) {
    s32 emptyIndex;
    s32 offset;
    u8 *cursor;

    D_800D6B04 = piRomLoad(0x3D);
    D_800D6B00 = func_8002B280(0x400, 0x81);
    offset = 0;
    do {
        *(s32 *) ((u8 *) D_800D6B00 + offset) = 0;
        offset += 4;
    } while (offset < 0x400);

    cursor = (u8 *) D_800D6B08;
    do {
        cursor += 4;
        *(void **) (cursor - 4) = NULL;
    } while ((u32) cursor < (u32) D_800D6B18);

    cursor = (u8 *) D_800D6B18;
    do {
        cursor += 4;
        *(void **) (cursor - 4) = NULL;
    } while ((u32) cursor < (u32) D_800D6B58);

    cursor = (u8 *) D_800D6B58;
    do {
        cursor += 0x14;
        cursor[-0x14] = 0xFF;
        *(s32 *) (cursor - 0x10) = 0;
        *(s32 *) (cursor - 8) = 0;
    } while ((u32) cursor < (u32) D_800D6BF8);

    emptyIndex = -1;
    cursor = D_800D6BF8;
    do {
        cursor += 8;
        *(s8 *) (cursor - 8) = emptyIndex;
    } while ((u32) cursor < (u32) D_800D6C38);

    D_8007D6B0 = 0;
    cursor = (u8 *) D_800D6C58;
    do {
        cursor += 0x40;
        *(s32 *) (cursor - 0x40) = 0;
        *(s32 *) (cursor - 0x30) = 0;
        *(s32 *) (cursor - 0x20) = 0;
        *(s32 *) (cursor - 0x10) = 0;
    } while (cursor != (u8 *) D_800D6D18);

    D_800D6C3E = 0;
    D_800D6C44 = 0;
    D_800D6C48 = 0;
    D_800D6C52 = 0xFF;
    D_800D6C54 = D_800D6C52;
    D_800D6C4C = 0;
    func_800534C0();
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_80050BF4.s")
#endif
void func_80050D50(void) {
    void **entry = D_800D6B18, **end = D_800D6B58;
    do {
        if (*entry != NULL) {
            amSndStop(*entry);
            *entry = NULL;
        }
        entry++;
    } while (entry != end);
}

void animseqFreeLevelData(void) {
    if (D_8007D680 != NULL) {
        mmFree(D_8007D680);
        D_8007D680 = NULL;
        D_8007D688 = -1;
        func_80050E9C();
    }
}

/*
 * PROVENANCE: adapted from JFG's public animseqLoadLevelData assembly.
 * Mickey's third allocator argument and two-word local layout establish the
 * source-offset home independently against Mickey's ROM.
 */
void func_80050DF0(s32 levelId) {
    struct {
        s32 unused;
        s32 source;
    } locals;
    s32 *bounds;

    if (levelId != -1 && levelId != D_8007D688) {
        animseqFreeLevelData();
        bounds = (s32 *) D_800D6B04 + levelId;
        locals.source = bounds[0];
        D_8007D684 = bounds[1] - locals.source;
        if (D_8007D684 > 0) {
            D_8007D680 =
                func_8002B280(D_8007D684, 0x81, locals.source);
            if (D_8007D680 != NULL) {
                piRomLoadSection(0x3E, D_8007D680, locals.source,
                                 D_8007D684);
                D_8007D688 = levelId;
            }
        }
    }
}
/*
 * PROVENANCE: adapted from JFG's animseqFreeGroup assembly. Mickey's data
 * boundaries, calls, scheduling, and final compiler output remain authoritative.
 * Fresh bounded pass: statement-line grouping fixes all four end/base address
 * schedules and all 41 relocation identities, leaving an exact-sized 84/90
 * allocation plateau. The remaining path-loop comparison spends one temporary
 * that shifts the two later reset temporaries; an algebraic increment probe
 * removes that whole register ladder but adds one instruction. GLOBAL_ASM
 * remains canonical.
 */
#ifdef NON_MATCHING
void func_80050E9C(void) {
    s32 emptyIndex;
    u8 *cursor;
    AnimScrollReset *scroll;
    AnimLockonReset *lockon;
    AnimLightReset *light;
    s32 pathIndex;

    if (D_8007D68C != NULL) {
        if (D_8007D694 != NULL) {
            mmFree(D_8007D68C);
            D_8007D694 = NULL;
        }
        emptyIndex = -1;
        D_8007D68C = NULL;
        D_8007D690 = emptyIndex;
        func_8005017C();

        pathIndex = 0;
        do {
            func_800502CC((u8) pathIndex);
            pathIndex++;
        } while ((pathIndex < 0x100) != 0);

        cursor = (u8 *) D_800D6B08; do {
            cursor += 4;
            *(void **) (cursor - 4) = NULL;
        } while ((u32) cursor < (u32) D_800D6B18);

        cursor = (u8 *) D_800D6B58; do {
            cursor += 0x14;
            cursor[-0x14] = 0xFF;
            *(s32 *) (cursor - 0x10) = 0;
            *(s32 *) (cursor - 8) = 0;
        } while ((u32) cursor < (u32) D_800D6BF8);

        cursor = D_800D6BF8; do {
            cursor += 8;
            *(s8 *) (cursor - 8) = emptyIndex;
        } while ((u32) cursor < (u32) D_800D6C38);

        D_8007D6B0 = 0;
        cursor = (u8 *) D_800D6C58; do {
            cursor += 0x40;
            *(s32 *) (cursor - 0x40) = 0;
            *(s32 *) (cursor - 0x30) = 0;
            *(s32 *) (cursor - 0x20) = 0;
            *(s32 *) (cursor - 0x10) = 0;
        } while (cursor != (u8 *) D_800D6D18);

        D_800D6C3E = 0;
        D_800D6C44 = 0;
        D_800D6C48 = 0;
        D_800D6C52 = 0xFF;
        D_800D6C54 = D_800D6C52;
        D_800D6C4C = 0;
        func_80050D50();
        func_800534C0();
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_80050E9C.s")
#endif

/*
 * PROVENANCE: adapted from JFG's public animseqSetupGroup assembly. Mickey's
 * directory layout, level-header field, globals, and calls are authoritative.
 */
void func_80051004(s32 groupId) {
    AnimGroupDirectoryEntry *entry;
    AnimLevelHeader *level;
    u8 *base;
    u32 packed;
    s32 foundId;
    s32 offset;
    s32 found;

    if ((groupId >= 0) && (groupId < 0x100) &&
        (groupId != D_8007D690)) {
        func_80050E9C();
        base = D_8007D680;
        entry = (AnimGroupDirectoryEntry *) base;
        do {
            packed = entry->packed;
            entry++;
            foundId = packed >> 24;
        } while ((groupId != foundId) && (foundId != 0xFF));
        found = (foundId == groupId);
        if (!found) {
            goto done;
        }
        offset = packed & 0xFFFFFF;
        if (offset == 0xFFFFFF) {
            goto done;
        }
        D_8007D690 = foundId;
        D_8007D68C = (s32 *) (base + offset);
        func_800511C4();
        animseqInitGroup();
        animseqResetGroup();
        level = (AnimLevelHeader *) levelGetLevel();
        D_8007D6B4 = level->sequenceRate;
        D_8007D6B8 = 0.0f;
        D_8007D6BC = 0;
        D_8007D6A4 = 1;
    }
done:
    return;
}
/* Exact JFG donor assembly corroborates the loop; C is Mickey-led. */
void animseqInitGroup(void) {
    s32 pathIndex;

    pathIndex = 0;
    do {
        func_80050348(pathIndex & 0xFF);
        pathIndex++;
    } while (pathIndex != 0x100);
}

/* JFG corroborates the reset-path loop; the remaining control flow is Mickey-led. */
void animseqResetGroup(void) {
    s32 pathIndex;

    if (D_8007D68C != NULL) {
        pathIndex = 0;
        do {
            func_8005055C(pathIndex & 0xFF);
            pathIndex++;
        } while (pathIndex != 0x100);
        D_8007D69C = D_8007D698;
        D_8007D6A8 = 0;
        D_8007D6AC = 0.0f;
        if (TrapDanglingJump(osRomBase) == 0) {
            D_8007D69C = NULL;
        }
        func_80050D50();
        func_800534C0();
    }
}

typedef struct AnimGroupPathHeader {
    u8 nodeCount;
    u8 unk1;
    s16 unk2;
    s16 unk4;
    u8 unk6;
    u8 flags;
    u8 nodeData[1];
} AnimGroupPathHeader;

void func_800508D4();

/*
 * PROVENANCE: adapted from JFG's func_80077468_78068 assembly. Mickey's
 * resident globals, packed fields, call identities, and output are checked
 * independently against Mickey's ROM.
 */
void func_800511C4(void) {
    u32 *entryCursor;
    s32 remaining;
    u32 headerWord;
    u32 entryWord;
    s32 pathIndex;
    AnimGroupPathHeader *source;
    AnimPath *path;
    s32 highBit;
    void func_8005055C(u8 pathIndex);

    entryCursor = (u32 *) D_8007D68C;
    headerWord = *entryCursor++;
    remaining = (headerWord >> 24) & 0xFF;
    func_8005027C();
    if (remaining > 0) {
        highBit = 0x80;
        do {
            entryWord = *entryCursor++;
            source = (AnimGroupPathHeader *)
                ((u8 *) D_8007D68C + (entryWord & 0xFFFFFF));
            path = func_8002B280((source->nodeCount * sizeof(AnimPathNode)) +
                                 sizeof(AnimPath),
                                 0x81);
            pathIndex = (entryWord >> 24) & 0xFF;
            D_800D6B00[pathIndex] = path;
            path = D_800D6B00[pathIndex];
            if (path != NULL) {
                if (source->nodeCount > 0) {
                    path->nodes = (AnimPathNode *)((u8 *)path +
                                                   sizeof(AnimPath));
                } else {
                    path->nodes = NULL;
                }
                path->unk2 = source->unk2;
                path->unk8 = NULL;
                path->unk0 = source->unk1;
                path->unk4 = source->unk4;
                path->unk6 = source->unk6;
                path->unk7 = source->flags;
                path->flags = 0;
                if (path->unk7 & highBit) {
                    path->flags = highBit;
                    path->unk7 &= 0x7F;
                }
                path->nodeCount = source->nodeCount;
                func_8005055C(pathIndex);
                func_800508D4(path->nodeCount, path->nodes, source->nodeData,
                              0, 0);
                func_80050AD4(pathIndex);
            }
            remaining--;
        } while (remaining > 0);
    }
}

extern s32 osTvType;
void func_800030B4(void *soundHandle, u8 pitch);
void func_800031C0(void *soundHandle, f32 x, f32 y, f32 z);
void func_800517E0(void);
#pragma weak animUpdateTrap = TrapDanglingJump
extern void animUpdateTrap(AnimPath *path, f32 delta, s32 updateRate,
                           s32 originalRate);

/*
 * PROVENANCE: adapted from JFG's public
 * asm/nonmatchings/anim/animseqUpdate.s. Mickey's command layout, resident
 * globals, sound-object offset, and final compiler output are independently
 * established from Mickey's ROM.
 *
 * Workbench wave 7: structure-mismatch after ten coherent attempts,
 * 287/287 instructions, frame -72 vs target -64, and 192 raw words from
 * +0x0. Reordering the interpolation update retained the headline score but
 * reduced aligned structural differences from 44 to 38 and gaps from 38 to
 * 32. The target retains playback-state and television-mode addresses in
 * saved registers; explicit source carriers regress geometry or alignment.
 * Prior flag, clock/type/lifetime, and canonical-permuter levers remain
 * exhausted; retain NON_MATCHING. */
#ifdef NON_MATCHING
void func_80051364(s32 updateRate) {
    AnimStreamEntry *command;
    AnimCameraSource **camera;
    AnimPath *path;
    AnimPathObject *object;
    void *soundHandle;
    s32 originalRate;
    s32 offset;
    s32 adjustedRate;
    f32 timeScale;
    f32 speed;

    if ((D_8007D68C != NULL) && (D_8007D6A4 != 0)) {
        if (osTvType == 0) {
            timeScale = D_80083FAC;
        } else {
            timeScale = D_80083FB0;
        }
        originalRate = updateRate;
        if (D_8007D6B0 > 0) {
            TrapDanglingJump(updateRate);
        }
        command = D_8007D69C;
        if ((command != NULL) && (D_8007D6A4 == 1)) {
            if ((command->command >> 8) == 0x7B) {
                if (((f32) command->duration / 100.0f) <
                    ((f32) (D_8007D6A8 + updateRate) * timeScale)) {
                    if (osTvType == 0) {
                        adjustedRate = command->duration >> 1;
                    } else {
                        adjustedRate = (command->duration * 6) / 10;
                    }
                    updateRate = adjustedRate - D_8007D6A8;
                    D_8007D69C = command + 1;
                    D_8007D6A4 = (s8) command->command;
                    if ((s8) command->command == 0) {
                        originalRate = updateRate;
                    }
                }
            }
        }
        if (updateRate > 0) {
            camera = D_800D6B08;
            do {
                *camera++ = NULL;
            } while (camera < (AnimCameraSource **) D_800D6B18);
            if (D_8007D6BC != 0) {
                if (updateRate < D_8007D6BC) {
                    D_8007D6B4 += D_8007D6B8 * (f32) updateRate;
                    D_8007D6BC -= updateRate;
                } else {
                    D_8007D6B4 += D_8007D6B8 * (f32) D_8007D6BC;
                    D_8007D6BC = 0;
                }
            }
            D_8007D6A8 += updateRate;
            D_8007D6AC = (f32) D_8007D6A8 * timeScale;
            offset = 0;
            do {
                path = *(AnimPath **) ((u8 *) D_800D6B00 + offset);
                if ((path != NULL) && (path->flags & 5)) {
                    animUpdateTrap(path, (f32) updateRate * timeScale,
                                   updateRate, originalRate);
                }
                offset += 4;
            } while (offset < 0x400);
            if (D_8007D6A4 == 1) {
                func_800517E0();
            }
            TrapDanglingJump(updateRate); /* runtime: overlay 41 +0x000 */
            TrapDanglingJump(updateRate); /* runtime: overlay 41 +0x124 */
            TrapDanglingJump(updateRate); /* runtime: overlay 41 +0x1B00 */
            offset = 0;
            do {
                path = *(AnimPath **) ((u8 *) D_800D6B00 + offset);
                if (path != NULL) {
                    object = path->unk8;
                    if ((object != NULL) && (object->soundHandle != NULL)) {
                        soundHandle = object->soundHandle;
                        func_800031C0(soundHandle, object->x, object->y,
                                      object->z);
                        if ((path->unk28 != 0x64) || (path->unk29 != 0)) {
                            speed = sqrtf((object->velocityX *
                                           object->velocityX) +
                                          (object->velocityY *
                                           object->velocityY) +
                                          (object->velocityZ *
                                           object->velocityZ));
                            func_800030B4(
                                object->soundHandle,
                                (u32) ((f32) path->unk28 +
                                       ((f32) path->unk29 * speed)) & 0xFF);
                        }
                    }
                }
                offset += 4;
            } while (offset != 0x400);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_80051364.s")
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_800517E0.s")
/*
 * PROVENANCE: JFG's public src/anim.c supplies the ordered animseqCamera
 * comparison; the body and local layouts are reconstructed from Mickey.
 */
AnimCameraSource *func_80053420(s32 index, AnimCameraTarget *target) {
    AnimCameraSource *source;

    source = D_800D6B08[index];
    if (source != NULL) {
        target->unkC = source->unkC;
        target->unk10 = source->unk10;
        target->unk14 = source->unk14;
        target->unk3E = source->unk2E;
        target->unk0 = 0x8000 - source->unk0;
        target->unk2 = -source->unk2;
        target->unk4 = source->unk4;
        func_80021504(D_8007D6B4, 0);
    }
    return source;
}
/* JFG's ordered anim.c tail and this store establish the tier-D Play name. */
void animseqPlay(void) {
    D_8007D6A4 = 1;
}

void func_800534C0(s32 i) {
    AnimPauseSlot *slot;

    /* The incoming scratch value is replaced before its first use. */
    slot = D_800D6D18;
    i = 4;
    do {
        slot->unkB = 0;
        slot->unk0 = 0;
        slot++;
    } while (i--);
}

void func_800534EC(s32 arg0) {
    AnimPauseSlot *slot;
    s32 i;

    slot = D_800D6D18;
    i = 4;
    do {
        if (slot->unkB > 0) {
            TrapDanglingJump(arg0, slot);
        }
        slot++;
    } while (i--);
}

void func_80002FE0(s32 id, f32 x, f32 y, f32 z, s32 priority,
                   void **handle);
u8 *func_80028F54(void);
void rumbleStart(s32 playerIndex, s32 strength, f32 duration);
void mathOneFloatRPY(ControlTransform *transform, f32 *output);

/*
 * PROVENANCE: JFG's public asm/nonmatchings/hit/hitInitObjectHit.s supplies
 * the structural comparison. Mickey's ABI, field offsets, branches, and
 * generated code remain independently established from Mickey's ROM.
 *
 * Workbench: exact; 198 instructions, -0x78 frame, zero differing words.
 * The local offset vector precedes the model locals, and the live hit pointer
 * follows the model declaration, reproducing the target stack homes.
 */
void func_80053550(HitInitSource *source, s32 kind, s32 mode, s16 rotationX,
                   s16 rotationY, s16 rotationZ, f32 radius, f32 height,
                   f32 arg8, f32 arg9, s32 collisionType, u16 flags) {
    f32 offset[3];
    HitInitModel *model;
    HitInitRecord *hit;
    HitInitHeader *header;
    HitInitDescriptor *descriptor;
    HitInitEntry *entry;
    f32 *floatPosition;
    s16 *vertex;
    s32 entryCount;
    s32 remaining;
    f32 extent;

    hit = source->hit;
    if (hit != NULL) {
        hit->rotationX = rotationX;
        hit->rotationY = rotationY;
        hit->rotationZ = rotationZ;
        hit->collisionType = collisionType;
        hit->kind = kind;
        hit->mode = mode;
        hit->position.x = source->position.x;
        hit->position.y = source->position.y;
        hit->position.z = source->position.z;
        if ((rotationX | rotationY | rotationZ) != 0) {
            hit->localOffset.x = rotationX * source->scale;
            hit->localOffset.y = rotationY * source->scale;
            hit->localOffset.z = rotationZ * source->scale;
            offset[0] = hit->localOffset.x;
            offset[1] = hit->localOffset.y;
            offset[2] = hit->localOffset.z;
            mathOneFloatRPY((ControlTransform *) source, offset);
            hit->position.x += offset[0];
            hit->position.y += offset[1];
            hit->position.z += offset[2];
        }
        hit->basePosition.x = hit->position.x;
        hit->basePosition.y = hit->position.y;
        hit->basePosition.z = hit->position.z;
        hit->radius = source->scale * radius;
        hit->height = source->scale * height;
        if (hit->mode == 0) {
            extent = hit->radius + 5.0f;
            hit->minX = hit->basePosition.x - extent;
            hit->maxX = hit->basePosition.x + extent;
            hit->minZ = hit->basePosition.z - extent;
            hit->maxZ = hit->basePosition.z + extent;
            extent = hit->height + 5.0f;
            hit->minY = hit->basePosition.y - extent;
            hit->maxY = hit->basePosition.y + extent;
        }
        hit->unk68 = arg8;
        hit->unk6C = arg9;
        hit->flags |= flags;
        entryCount = hit->entryCount;
        if (entryCount != 0) {
            entry = hit->entries;
            remaining = entryCount;
            model = *source->model;
            header = model->header;
            floatPosition = model->floatPositions;
            descriptor = header->descriptors;
            do {
                if (header->useFloatPositions != 0) {
                    entry->position.x = floatPosition[0];
                    floatPosition += 3;
                    entry->position.y = floatPosition[-2];
                    entry->position.z = floatPosition[-1];
                } else {
                    vertex = (s16 *)((u8 *)model->vertices +
                                     (descriptor->vertexIndex * 10));
                    offset[0] = vertex[0];
                    offset[1] = vertex[1];
                    offset[2] = vertex[2];
                    mathOneFloatRPY((ControlTransform *) source, offset);
                    entry->position.x = offset[0] + source->position.x;
                    entry->position.y = offset[1] + source->position.y;
                    entry->position.z = offset[2] + source->position.z;
                }
                remaining--;
                entry++;
                extent = descriptor->scale * source->scale;
                descriptor++;
                entry[-1].scaleX = extent;
                entry[-1].scaleY = extent;
            } while (remaining > 0);
        }
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_80053868.s")
typedef struct AnimCollisionShape {
    u8 pad0[6];
    u16 flags;
    u8 shape;
    u8 pad9[0xF];
    AnimVec3f position;
    AnimVec3f edge;
    AnimVec3f vector;
    u8 pad3C[0x1C];
    f32 radius;
    f32 height;
} AnimCollisionShape;

typedef struct AnimCollisionResult {
    s32 object;
    s32 value;
    f32 fraction;
    AnimVec3f normal;
} AnimCollisionResult;

s32 func_80012234(AnimVec3f *point, AnimVec3f *direction,
                  AnimVec3f *origin, AnimVec3f *planeDirection,
                  f32 radius, f32 *minimum, f32 *maximum);
s32 func_80012574(AnimVec3f *origin, AnimVec3f *direction,
                  AnimVec3f *center, f32 radius, f32 *minimum,
                  f32 *maximum);
s32 func_800131AC(AnimVec3f *origin, AnimVec3f *direction,
                  AnimVec3f *minimum, AnimVec3f *maximum,
                  f32 *near, f32 *far);
extern f32 D_8008420C;

#ifdef NON_MATCHING
/* PROVENANCE: JFG's public hit/collision code supplies the capsule and
 * endpoint-overlap role; Mickey's collision-shape offsets are authoritative. */
/* Workbench verdict: structure-mismatch, 365 differing words; first mismatch is at +0x0. */
/* Target is 370 instructions/frame -216; candidate is 371 instructions/frame -392. */
/* Remaining gap is frame/allocation: all three sqrtf identities resolve, but none has the target offset. */
s32 func_80054B3C(s32 arg0, AnimCollisionShape *arg1,
                  s32 arg2, AnimCollisionShape *arg3,
                  AnimCollisionResult *arg4) {
    f32 spBC;
    f32 spB8;
    f32 spB4;
    s32 sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp88;
    f32 sp84;
    f32 sp80;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp58;
    f32 sp54;
    f32 sp4C;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    AnimCollisionShape *first;
    AnimVec3f *temp_a0;
    AnimVec3f *temp_a0_2;
    AnimVec3f *temp_a1;
    AnimVec3f *temp_v0;
    AnimVec3f *temp_v0_2;
    AnimVec3f *temp_v1;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 temp_f0_7;
    f32 temp_f0_8;
    f32 temp_f10;
    f32 temp_f10_2;
    f32 temp_f10_3;
    f32 temp_f10_4;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f12_4;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f18_2;
    f32 temp_f20;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f2_5;
    f32 temp_f2_6;
    f32 temp_f2_7;
    f32 temp_f2_8;
    f32 temp_f4;
    f32 temp_f4_2;
    f32 temp_f4_3;
    f32 temp_f6;
    f32 temp_f6_2;
    f32 temp_f6_3;
    f32 temp_f6_4;
    f32 temp_f6_5;
    f32 temp_f8;
    f32 temp_f8_2;
    f32 temp_f8_3;
    f32 temp_f8_4;
    f32 temp_f8_5;
    f32 var_f12;
    f32 var_f12_2;
    s32 var_t0;

    temp_f20 = arg1->radius + arg3->radius;
    var_t0 = 0;
    first = arg1;
    temp_v0 = &first->position;
    temp_f20 *= temp_f20;
    temp_a0 = &arg3->position;
    if ((first->flags & 2) || (arg3->flags & 2)) {
        temp_f0 = temp_a0->x - temp_v0->x;
        temp_f2 = temp_a0->y - temp_v0->y;
        temp_f12 = temp_a0->z - temp_v0->z;
        if (((temp_f0 * temp_f0) + (temp_f2 * temp_f2) +
             (temp_f12 * temp_f12)) <= temp_f20) {
            arg4->object = arg0;
            arg4->value = arg2;
            arg4->fraction = 0.0f;
            return 1;
        }
    }
    temp_v1 = &first->vector;
    temp_f2_2 = temp_v1->x;
    temp_a1 = &arg3->vector;
    temp_f16 = temp_a1->x;
    temp_f12_2 = temp_v1->y;
    temp_f18 = temp_a1->y;
    temp_f0_2 = temp_v1->z;
    temp_f14 = temp_a1->z;
    temp_a0_2 = &arg3->position;
    temp_v0_2 = &first->position;
    temp_f8 = (temp_f14 * temp_f14) +
              ((temp_f0_2 * temp_f0_2) -
               (2.0f * temp_f0_2 * temp_f14)) +
              (((temp_f2_2 * temp_f2_2) -
                (2.0f * temp_f2_2 * temp_f16)) +
               (temp_f16 * temp_f16) +
               (((temp_f12_2 * temp_f12_2) -
                 (2.0f * temp_f12_2 * temp_f18)) +
                (temp_f18 * temp_f18)));
    spBC = temp_f8;
    sp94 = temp_a0_2->z;
    temp_f6 = 2.0f * sp94;
    sp54 = temp_f6;
    temp_f10 = temp_v0_2->z;
    sp28 = temp_f8;
    sp2C = sp94;
    sp90 = temp_f10;
    temp_f8_2 = 2.0f * temp_f10;
    sp8C = temp_f8_2;
    temp_f4 = temp_v0_2->x;
    sp30 = temp_f6;
    sp34 = temp_f10;
    sp88 = temp_f4;
    temp_f6_2 = 2.0f * temp_f4;
    sp84 = temp_f6_2;
    temp_f10_2 = temp_a0_2->x;
    sp38 = temp_f8_2;
    sp3C = temp_f4;
    sp80 = temp_f10_2;
    temp_f8_3 = 2.0f * temp_f10_2;
    sp4C = temp_f8_3;
    temp_f4_2 = temp_v0_2->y;
    sp40 = temp_f6_2;
    sp7C = temp_f4_2;
    temp_f6_3 = 2.0f * temp_f4_2;
    sp78 = temp_f6_3;
    temp_f6_4 = temp_a0_2->y;
    sp44 = temp_f10_2;
    sp74 = temp_f6_4;
    temp_f6_5 = temp_f6_4 + sp74;
    temp_f10_3 = sp38;
    sp38 = temp_f8_3;
    sp58 = temp_f6_5;
    temp_f8_4 = sp30;
    sp30 = temp_f4_2;
    temp_f4_3 = sp40;
    sp40 = temp_f10_3;
    temp_f10_4 = sp38;
    sp38 = (temp_f14 * temp_f8_4) +
           (((temp_f10_3 * temp_f0_2) - (temp_f10_3 * temp_f14)) -
            (temp_f8_4 * temp_f0_2));
    spB8 = sp38 +
           ((((temp_f4_3 * temp_f2_2) - (temp_f4_3 * temp_f16)) -
             (temp_f10_4 * temp_f2_2)) +
            (temp_f10_4 * temp_f16) +
            ((((temp_f6_3 * temp_f12_2) - (temp_f6_3 * temp_f18)) -
              (temp_f6_5 * temp_f12_2)) +
             (temp_f6_5 * temp_f18)));
    temp_f18_2 = (sp2C * sp2C) +
                 ((sp34 * sp34) - (sp40 * sp2C)) +
                 (((sp3C * sp3C) - (temp_f4_3 * sp44)) +
                  (sp44 * sp44) +
                  (((sp30 * sp30) - (sp78 * sp74)) + (sp74 * sp74)));
    if (sp28 != 0.0f) {
        temp_f2_3 = 4.0f * sp28;
        temp_f0_3 = temp_f2_3 * (temp_f18_2 - temp_f20);
        temp_f14_2 = spB8 * spB8;
        if (temp_f0_3 < temp_f14_2) {
            sp98 = 0;
            sp90 = temp_f2_3;
            sp94 = temp_f14_2;
            spB4 = temp_f18_2;
            temp_f0_4 = sqrtf(temp_f14_2 - temp_f0_3);
            var_t0 = 0;
            temp_f8_5 = -spB8;
            sp88 = 2.0f * spBC;
            temp_f2_4 = (temp_f8_5 - temp_f0_4) / sp88;
            sp8C = temp_f8_5;
            if ((temp_f2_4 >= 0.0f) && (temp_f2_4 <= 1.0f)) {
                temp_f0_5 = sp90 *
                            (temp_f18_2 - (temp_f20 + 83.0f));
                if (temp_f0_5 < temp_f14_2) {
                    temp_f2_5 = (sp8C - sqrtf(temp_f14_2 - temp_f0_5)) /
                                sp88;
                    var_t0 = 1;
                    var_f12 = temp_f2_5;
                    if (temp_f2_5 > 1.0f) {
                        var_f12 = 1.0f;
                    } else if (temp_f2_5 < 0.0f) {
                        var_f12 = 0.0f;
                    }
                    arg4->object = arg0;
                    arg4->value = arg2;
                    arg4->fraction = var_f12;
                }
            } else {
                temp_f0_6 = arg3->edge.x - first->edge.x;
                temp_f2_6 = arg3->edge.y - first->edge.y;
                temp_f12_3 = arg3->edge.z - first->edge.z;
                if (((temp_f0_6 * temp_f0_6) +
                     (temp_f2_6 * temp_f2_6) +
                     (temp_f12_3 * temp_f12_3)) <= temp_f20) {
                    temp_f0_7 = sp90 *
                                (temp_f18_2 - (temp_f20 + 83.0f));
                    if (temp_f0_7 < temp_f14_2) {
                        temp_f2_7 =
                            (sp8C - sqrtf(temp_f14_2 - temp_f0_7)) / sp88;
                        var_t0 = 1;
                        var_f12_2 = temp_f2_7;
                        if (temp_f2_7 > 1.0f) {
                            var_f12_2 = 1.0f;
                        } else if (temp_f2_7 < 0.0f) {
                            var_f12_2 = 0.0f;
                        }
                        arg4->object = arg0;
                        arg4->value = arg2;
                        arg4->fraction = var_f12_2;
                    }
                }
            }
        }
    }
    if (var_t0 == 0) {
        temp_f0_8 = arg3->edge.x - first->edge.x;
        temp_f2_8 = arg3->edge.y - first->edge.y;
        temp_f12_4 = arg3->edge.z - first->edge.z;
        if (((temp_f0_8 * temp_f0_8) + (temp_f2_8 * temp_f2_8) +
             (temp_f12_4 * temp_f12_4)) <= temp_f20) {
            arg4->object = arg0;
            arg4->value = arg2;
            var_t0 = 1;
            arg4->fraction = 0.0f;
        }
    }
    return var_t0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_80054B3C.s")
#endif
typedef struct HitResolveMass {
    u8 pad0[4];
    f32 mass;
} HitResolveMass;

typedef struct HitResolveVehicle {
    s8 playerIndex;
    u8 pad1[3];
    f32 rotatedZ;
    f32 rotatedX;
    u8 padC[0x88];
    AnimVec3f velocity;
    u8 padA0[0x18];
    void *soundHandle;
    u8 padBC[0x18];
    void *collisionData;
    u8 padD8[0x18];
    s16 rotationY;
    u8 padF2[0xC];
    s16 rotationX;
    u8 pad100[0x6A];
    s16 collisionMode;
    u8 pad16C[0x3C];
    u16 flags;
    u8 pad1AA[0x209];
    u8 collisionTimer;
    u8 pad3B4[2];
    s16 collisionCountA;
    s16 collisionCountB;
} HitResolveVehicle;

extern f32 D_800841F0;
extern u32 func_80001620(s32 soundId);
extern void func_8000309C(void *handle, u8 volume);

typedef struct HitResolveRotation {
    s16 x;
    s16 y;
    s16 z;
} HitResolveRotation;

extern void mathOneFloatYPR(HitResolveRotation *rotation, AnimVec3f *vector);

/*
 * Bare-pragma reconstruction from Mickey's collision response assembly.
 * The public JFG hit.c family supplies role context only; Mickey fixes every
 * field offset, call identity and arithmetic association below.
 */
#ifdef NON_MATCHING
void func_80055104(HitCopyState *first, HitCopyState *second, f32 scale) {
    HitCopySource *firstSource;
    HitCopySource *secondSource;
    HitResolveVehicle *firstVehicle;
    HitResolveVehicle *secondVehicle;
    HitResolveMass *mass;
    void *firstCollision;
    void *secondCollision;
    HitResolveRotation rotation;
    AnimVec3f rotated;
    AnimVec3f direction;
    AnimVec3f effectPosition;
    f32 firstMass;
    f32 secondMass;
    f32 distance;
    f32 relativeVelocity;
    f32 impulse;
    f32 firstScale;
    f32 secondScale;
    f32 halfDistance;
    f32 soundScale;
    f32 volume;
    f32 oldX;
    f32 oldY;
    f32 oldZ;

    firstVehicle = (HitResolveVehicle *) first->target;
    firstSource = first->source;
    mass = (HitResolveMass *) TrapDanglingJump(firstVehicle);
    firstMass = mass->mass;
    secondSource = second->source;
    secondVehicle = (HitResolveVehicle *) second->target;
    mass = (HitResolveMass *) TrapDanglingJump(secondVehicle);
    secondMass = mass->mass;
    direction.x = secondSource->current.x - firstSource->current.x;
    direction.y = secondSource->current.y - firstSource->current.y;
    direction.z = secondSource->current.z - firstSource->current.z;
    distance = sqrtf((direction.x * direction.x) +
                     (direction.y * direction.y) +
                     (direction.z * direction.z));
    direction.x /= distance;
    direction.y /= distance;
    direction.z /= distance;
    relativeVelocity =
        ((firstVehicle->velocity.x - secondVehicle->velocity.x) * direction.x) +
        ((firstVehicle->velocity.y - secondVehicle->velocity.y) * direction.y) +
        ((firstVehicle->velocity.z - secondVehicle->velocity.z) * direction.z);
    impulse = (D_800841F0 * relativeVelocity) /
              ((1.0f / firstMass) + (1.0f / secondMass));
    firstScale = impulse / firstMass;
    firstVehicle->velocity.x += firstScale * direction.x;
    firstVehicle->velocity.y += firstScale * direction.y;
    firstVehicle->velocity.z += firstScale * direction.z;
    rotation.x = -(firstVehicle->rotationY + firstVehicle->rotationX);
    rotation.y = -*(s16 *) ((u8 *) first + 2);
    rotation.z = -*(s16 *) ((u8 *) first + 4);
    rotated = firstVehicle->velocity;
    mathOneFloatYPR(&rotation, &rotated);
    firstVehicle->rotatedZ = rotated.z;
    firstVehicle->rotatedX = rotated.x;
    secondScale = impulse / secondMass;
    secondVehicle->velocity.x -= secondScale * direction.x;
    secondVehicle->velocity.y -= secondScale * direction.y;
    secondVehicle->velocity.z -= secondScale * direction.z;
    rotation.x = -(secondVehicle->rotationY + secondVehicle->rotationX);
    rotation.y = -*(s16 *) ((u8 *) second + 2);
    rotation.z = -*(s16 *) ((u8 *) second + 4);
    rotated = secondVehicle->velocity;
    mathOneFloatYPR(&rotation, &rotated);
    secondVehicle->rotatedZ = rotated.z;
    secondVehicle->rotatedX = rotated.x;
    oldY = first->position.y - firstSource->previous.y;
    oldX = first->position.x - firstSource->previous.x;
    oldZ = first->position.z - firstSource->previous.z;
    firstSource->previous.x =
        (firstVehicle->velocity.x * scale) + firstSource->current.x;
    firstSource->previous.y =
        (firstVehicle->velocity.y * scale) + firstSource->current.y;
    firstSource->previous.z =
        (firstVehicle->velocity.z * scale) + firstSource->current.z;
    first->position.x = firstSource->previous.x + oldX;
    first->position.y = firstSource->previous.y + oldY;
    first->position.z = firstSource->previous.z + oldZ;
    oldY = second->position.y - secondSource->previous.y;
    oldX = second->position.x - secondSource->previous.x;
    oldZ = second->position.z - secondSource->previous.z;
    secondSource->previous.x =
        (secondVehicle->velocity.x * scale) + secondSource->current.x;
    secondSource->previous.y =
        (secondVehicle->velocity.y * scale) + secondSource->current.y;
    secondSource->previous.z =
        (secondVehicle->velocity.z * scale) + secondSource->current.z;
    second->position.x = secondSource->previous.x + oldX;
    second->position.y = secondSource->previous.y + oldY;
    second->position.z = secondSource->previous.z + oldZ;

    firstCollision = (void *) TrapDanglingJump(firstVehicle->collisionData);
    secondCollision = (void *) TrapDanglingJump(secondVehicle->collisionData);
    if (((firstVehicle->collisionMode != 0) ||
         ((firstCollision == NULL) && (secondCollision != NULL))) &&
        (TrapDanglingJump(second, secondVehicle) != 0)) {
        firstVehicle->collisionCountA++;
        secondVehicle->collisionCountB++;
        if (*func_80028F54() == 5) {
            TrapDanglingJump(second);
        }
        TrapDanglingJump(first, second);
    }
    if (((secondVehicle->collisionMode != 0) ||
         ((secondCollision == NULL) && (firstCollision != NULL))) &&
        (TrapDanglingJump(first, firstVehicle) != 0)) {
        firstVehicle->collisionCountB++;
        secondVehicle->collisionCountA++;
        if (*func_80028F54() == 5) {
            TrapDanglingJump(first);
        }
        TrapDanglingJump(second, first);
    }

    firstVehicle->collisionTimer = 0x64;
    secondVehicle->collisionTimer = 0x64;
    if (relativeVelocity > 4.0f) {
        halfDistance = distance * 0.5f;
        effectPosition.x =
            (direction.x * halfDistance) + firstSource->current.x;
        effectPosition.y =
            (direction.y * halfDistance) + firstSource->current.y;
        effectPosition.z =
            (direction.z * halfDistance) + firstSource->current.z;
        soundScale = (f32) func_80001620(7);
        volume = (relativeVelocity / 20.0f) * soundScale;
        if (soundScale < volume) {
            volume = soundScale;
        }
        if (firstVehicle->soundHandle != NULL) {
            func_800031E8(firstVehicle->soundHandle);
        }
        func_80002FE0(7, effectPosition.x, effectPosition.y,
                      effectPosition.z, 4,
                      &firstVehicle->soundHandle);
        func_8000309C(firstVehicle->soundHandle, (u8) volume);
        if (!(firstVehicle->flags & 1)) {
            rumbleStart(firstVehicle->playerIndex, 0x32, 0.4f);
        }
        if (!(secondVehicle->flags & 1)) {
            rumbleStart(secondVehicle->playerIndex, 0x32, 0.4f);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_80055104.s")
#endif
/* Mickey-local collision response reconstructed from its resident ABI. */
void func_800557F8(HitCopyState *first, HitCopyState *second, f32 unused) {
    s32 priority;
    HitCollisionLink *secondTarget;
    HitCollisionVehicle *firstVehicle;
    HitCopySource *source;
    s32 soundHandle;
    s32 timer;

    secondTarget = (HitCollisionLink *) second->target;
    priority = 4;
    firstVehicle = (HitCollisionVehicle *) first->target;
    source = second->source;
    if ((firstVehicle->unk16A == 0) && (firstVehicle->unk168 == 0)) {
        TrapDanglingJump(first, firstVehicle);
        /* Inert aid preserving IDO's target v0 allocation; tracked in
         * docs/cleanup-queue.md. */
        if (1) {
        }
        timer = 0x258;
        firstVehicle->unk158 = -0x7FFD;
        firstVehicle->unk15A = timer;
        firstVehicle->unk15C = timer;
        firstVehicle->unk150 = 10.0f;
        TrapDanglingJump(secondTarget->state, first);
        /* Inert aid preserving IDO's target v0 allocation; tracked in
         * docs/cleanup-queue.md. */
        if (1) {
        }
        ((HitCollisionVehicle *) secondTarget->state->target)->unk3B6++;
        firstVehicle->unk3B8++;
        if (*func_80028F54() == 5) {
            TrapDanglingJump(first);
        }
        soundHandle = firstVehicle->soundHandle;
        firstVehicle->unk185 = 0;
        firstVehicle->unk188 = 0.0f;
        if (soundHandle != 0) {
            func_800031E8(soundHandle);
        }
        if (!(firstVehicle->flags1A8 & 1)) {
            rumbleStart(firstVehicle->playerIndex, 0x46, 0.75f);
        }
    } else {
        func_80002FE0(0x26E, source->current.x, source->current.y,
                      source->current.z, priority, NULL);
    }
    second->position.x = source->current.x;
    second->position.y = source->current.y;
    second->position.z = source->current.z;
    source->previous.x = source->current.x;
    source->previous.y = source->current.y;
    source->previous.z = source->current.z;
    TrapDanglingJump(second, 1);
}
/* Declaration order is load-bearing: uopt homes declared locals at descending
 * stack offsets in declaration order, and only firstVehicle (sp+0x48, the
 * call-crossing spill) and firstSource (sp+0x3C) surface in the object, so
 * firstVehicle must be the second declaration and firstSource the fifth
 * (uoptlist frame-ladder-guided; the volatile pad holds the 0x40 slot). */
void func_80055970(HitCopyState *first, HitCopyState *second, f32 unused) {
    HitCollisionNormalLink *secondTarget;
    HitCollisionVehicle *firstVehicle;
    HitCopySource *secondSource;
    volatile s32 stackPad;
    HitCopySource *firstSource;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    f32 distance;

    secondTarget = (HitCollisionNormalLink *) second->target;
    secondSource = second->source;
    firstSource = first->source;
    firstVehicle = (HitCollisionVehicle *) first->target;
    if (TrapDanglingJump(first, firstVehicle) != 0) {
        TrapDanglingJump(secondTarget->state, first, firstVehicle);
        /* Inert allocation aid retained by exact C; tracked in
         * docs/cleanup-queue.md. */
        if (1) {
        }
        ((HitCollisionVehicle *) secondTarget->state->target)->unk3B6++;
        firstVehicle->unk3B8++;
        if (*func_80028F54() == 5) {
            TrapDanglingJump(first);
        }
    } else {
        func_80002FE0(0x26E, secondSource->current.x,
                      secondSource->current.y, secondSource->current.z,
                      4, NULL);
    }
    deltaX = second->position.x - secondSource->previous.x;
    deltaY = second->position.y - secondSource->previous.y;
    deltaZ = second->position.z - secondSource->previous.z;
    secondSource->previous.x = secondSource->current.x;
    secondSource->previous.y = secondSource->current.y;
    secondSource->previous.z = secondSource->current.z;
    second->position.x = secondSource->previous.x + deltaX;
    second->position.y = secondSource->previous.y + deltaY;
    second->position.z = secondSource->previous.z + deltaZ;
    deltaX = secondSource->current.x - firstSource->current.x;
    deltaY = secondSource->current.y - firstSource->current.y;
    deltaZ = secondSource->current.z - firstSource->current.z;
    distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) +
                     (deltaZ * deltaZ));

    secondTarget->unk14 = deltaX / distance;
    secondTarget->unk18 = deltaY / distance;
    secondTarget->unk1C = deltaZ / distance;
    TrapDanglingJump(second, 0xE);
}

/* The first collision callback returns no value. Its typed weak alias removes
 * the generic trap placeholder's phantom s32 return web; the build restores
 * the measured TrapDanglingJump relocation identity without changing bytes. */
#pragma weak hitCopyFirstTrap = TrapDanglingJump
extern void hitCopyFirstTrap(HitCopyState *state, HitCollisionVehicle *vehicle);
void func_80055B24(HitCopyState *first, HitCopyState *second, f32 unused) {
    HitCollisionNormalLink *secondTarget;
    HitCopySource *secondSource;
    HitCollisionVehicle *firstVehicle;
    HitCopySource *firstSource;
    s32 soundHandle;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    f32 distance;

    secondTarget = (HitCollisionNormalLink *) second->target;
    secondSource = second->source;
    firstSource = first->source;
    firstVehicle = (HitCollisionVehicle *) first->target;
    if ((firstVehicle->unk16A == 0) && (firstVehicle->unk168 == 0)) {
        hitCopyFirstTrap(first, firstVehicle);
        {
            s32 timer;

            timer = 0x258;
            firstVehicle->unk158 = -0x7FFD;
            firstVehicle->unk15A = timer;
            firstVehicle->unk15C = timer;
        }
        firstVehicle->unk150 = 10.0f;
        TrapDanglingJump(secondTarget->state, first);
        /* Inert allocation aid retained by exact C; tracked in
         * docs/cleanup-queue.md. */
        if (1) {
        }
        ((HitCollisionVehicle *) secondTarget->state->target)->unk3B6++;
        firstVehicle->unk3B8++;
        if (*func_80028F54() == 5) {
            TrapDanglingJump(first);
        }
        soundHandle = firstVehicle->soundHandle;
        firstVehicle->unk185 = 0;
        firstVehicle->unk188 = 0.0f;
        if (soundHandle != 0) {
            func_800031E8(soundHandle);
        }
        if (!(firstVehicle->flags1A8 & 1)) {
            rumbleStart(firstVehicle->playerIndex, 0x46, 0.75f);
        }
    } else {
        func_80002FE0(0x26E, secondSource->current.x,
                      secondSource->current.y, secondSource->current.z,
                      4, NULL);
    }
    second->position.x = secondSource->current.x;
    second->position.y = secondSource->current.y;
    second->position.z = secondSource->current.z;
    secondSource->previous.x = secondSource->current.x;
    secondSource->previous.y = secondSource->current.y;
    secondSource->previous.z = secondSource->current.z;

    deltaX = secondSource->current.x - firstSource->current.x;
    deltaY = secondSource->current.y - firstSource->current.y;
    deltaZ = secondSource->current.z - firstSource->current.z;
    distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) +
                     (deltaZ * deltaZ));

    secondTarget->unk1C = deltaX / distance;
    secondTarget->unk20 = deltaY / distance;
    secondTarget->unk24 = deltaZ / distance;
    TrapDanglingJump(second, 6);
}
void func_80055D08(HitCopyState *first, HitCopyState *second, f32 unused) {
    HitCopySource *firstSource;
    HitCopySource *secondSource;
    HitCopyTarget *target;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    f32 distance;

    firstSource = first->source;
    first->position.x = firstSource->current.x;
    first->position.y = firstSource->current.y;
    first->position.z = firstSource->current.z;
    firstSource->previous.x = firstSource->current.x;
    firstSource->previous.y = firstSource->current.y;
    firstSource->previous.z = firstSource->current.z;

    secondSource = second->source;
    deltaX = second->position.x - secondSource->previous.x;
    deltaY = second->position.y - secondSource->previous.y;
    deltaZ = second->position.z - secondSource->previous.z;
    secondSource->previous.x = secondSource->current.x;
    secondSource->previous.y = secondSource->current.y;
    secondSource->previous.z = secondSource->current.z;
    second->position.x = secondSource->previous.x + deltaX;
    second->position.y = secondSource->previous.y + deltaY;
    second->position.z = secondSource->previous.z + deltaZ;

    deltaX = secondSource->current.x - firstSource->current.x;
    deltaY = secondSource->current.y - firstSource->current.y;
    deltaZ = secondSource->current.z - firstSource->current.z;
    distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) +
                     (deltaZ * deltaZ));

    target = second->target;
    target->unk14 = deltaX / distance;
    target->unk18 = deltaY / distance;
    target->unk1C = deltaZ / distance;
    TrapDanglingJump(first, 1, second);
    TrapDanglingJump(second, 0x12);
}
void func_80055E50(HitCopyState *first, HitCopyState *second, f32 unused) {
    HitCopySource *firstSource;
    HitCopySource *secondSource;
    HitCopyTarget *target;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    f32 distance;

    firstSource = first->source;
    first->position.x = firstSource->current.x;
    first->position.y = firstSource->current.y;
    first->position.z = firstSource->current.z;
    firstSource->previous.x = firstSource->current.x;
    firstSource->previous.y = firstSource->current.y;
    firstSource->previous.z = firstSource->current.z;

    secondSource = second->source;
    second->position.x = secondSource->current.x;
    second->position.y = secondSource->current.y;
    second->position.z = secondSource->current.z;
    secondSource->previous.x = secondSource->current.x;
    secondSource->previous.y = secondSource->current.y;
    secondSource->previous.z = secondSource->current.z;

    deltaX = secondSource->current.x - firstSource->current.x;
    deltaY = secondSource->current.y - firstSource->current.y;
    deltaZ = secondSource->current.z - firstSource->current.z;
    distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) +
                     (deltaZ * deltaZ));

    target = second->target;
    target->unk1C = deltaX / distance;
    target->unk20 = deltaY / distance;
    target->unk24 = deltaZ / distance;
    TrapDanglingJump(first, 1, second);
    TrapDanglingJump(second, 0xA);
}

/* IDO's local allocation follows declaration order here; retain secondZ before
 * secondY so the volatile value keeps its exact stack home. */
void func_80055F64(HitCopyState *first, HitCopyState *second, f32 unused) {
    HitCopySource *firstSource;
    HitCopySource *secondSource;
    HitCopyTarget *secondTarget;
    HitCopyTarget *firstTarget;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    f32 distance;
    f32 firstX;
    f32 secondX;
    volatile f32 secondZ;
    f32 secondY;

    firstSource = first->source;
    firstX = firstSource->current.x;
    deltaX = first->position.x - firstSource->previous.x;
    deltaY = first->position.y - firstSource->previous.y;
    deltaZ = first->position.z - firstSource->previous.z;
    firstSource->previous.x = firstX;
    firstSource->previous.y = firstSource->current.y;
    firstSource->previous.z = firstSource->current.z;
    first->position.x = firstX + deltaX;
    first->position.y = firstSource->previous.y + deltaY;
    first->position.z = firstSource->previous.z + deltaZ;

    secondSource = second->source;
    second->position.x = secondSource->current.x;
    second->position.y = secondSource->current.y;
    second->position.z = secondSource->current.z;
    secondX = *(volatile f32 *)&secondSource->current.x;
    secondY = *(volatile f32 *)&secondSource->current.y;
    secondSource->previous.x = secondX;
    secondSource->previous.y = secondY;
    secondZ = *(volatile f32 *)&secondSource->current.z;
    secondSource->previous.z = secondZ;

    deltaX = secondX - firstSource->current.x;
    deltaY = secondY - firstSource->current.y;
    deltaZ = secondZ - firstSource->current.z;
    distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) +
                     (deltaZ * deltaZ));

    secondTarget = second->target;
    secondTarget->unk1C = deltaX / distance;
    secondTarget->unk20 = deltaY / distance;
    secondTarget->unk24 = deltaZ / distance;
    firstTarget = first->target;
    firstTarget->unk14 = -secondTarget->unk1C;
    firstTarget->unk18 = -secondTarget->unk20;
    firstTarget->unk1C = -secondTarget->unk24;
    TrapDanglingJump(first, 0x12, second);
    TrapDanglingJump(second, 6);
}
void func_800560D0(HitCopyState *first, HitCopyState *second, f32 unused) {
    HitCopyTarget *firstTarget;
    HitCopyTarget *secondTarget;
    HitCopySource *firstSource;
    HitCopySource *secondSource;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    f32 distance;

    firstSource = first->source;
    deltaX = first->position.x - firstSource->previous.x;
    deltaY = first->position.y - firstSource->previous.y;
    deltaZ = first->position.z - firstSource->previous.z;
    firstTarget = first->target;
    secondTarget = second->target;
    secondSource = second->source;
    firstSource->previous.x = firstSource->current.x;
    firstSource->previous.y = firstSource->current.y;
    firstSource->previous.z = firstSource->current.z;
    first->position.x = firstSource->previous.x + deltaX;
    first->position.y = firstSource->previous.y + deltaY;
    first->position.z = firstSource->previous.z + deltaZ;

    deltaX = second->position.x - secondSource->previous.x;
    deltaY = second->position.y - secondSource->previous.y;
    deltaZ = second->position.z - secondSource->previous.z;
    secondSource->previous.x = secondSource->current.x;
    secondSource->previous.y = secondSource->current.y;
    secondSource->previous.z = secondSource->current.z;
    second->position.x = secondSource->previous.x + deltaX;
    second->position.y = secondSource->previous.y + deltaY;
    second->position.z = secondSource->previous.z + deltaZ;

    deltaX = secondSource->current.x - firstSource->current.x;
    deltaY = secondSource->current.y - firstSource->current.y;
    deltaZ = secondSource->current.z - firstSource->current.z;
    distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) +
                     (deltaZ * deltaZ));

    firstTarget->unk14 = deltaX / distance;
    firstTarget->unk18 = deltaY / distance;
    firstTarget->unk1C = deltaZ / distance;
    secondTarget->unk14 = -firstTarget->unk14;
    secondTarget->unk18 = -firstTarget->unk18;
    secondTarget->unk1C = -firstTarget->unk1C;
    TrapDanglingJump(first, 6);
    TrapDanglingJump(second, 0x12);
}
void func_80056274(HitCopyState *first, HitCopyState *second, f32 unused) {
    HitCopyTarget *firstTarget;
    HitCopyTarget *secondTarget;
    HitCopySource *firstSource;
    HitCopySource *secondSource;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    f32 distance;

    firstSource = first->source;
    secondTarget = second->target;
    secondSource = second->source;
    firstTarget = first->target;
    first->position.x = firstSource->current.x;
    first->position.y = firstSource->current.y;
    first->position.z = firstSource->current.z;
    firstSource->previous.x = firstSource->current.x;
    firstSource->previous.y = firstSource->current.y;
    firstSource->previous.z = firstSource->current.z;

    second->position.x = secondSource->current.x;
    second->position.y = secondSource->current.y;
    second->position.z = secondSource->current.z;
    secondSource->previous.x = secondSource->current.x;
    secondSource->previous.y = secondSource->current.y;
    secondSource->previous.z = secondSource->current.z;

    deltaX = secondSource->current.x - firstSource->current.x;
    deltaY = secondSource->current.y - firstSource->current.y;
    deltaZ = secondSource->current.z - firstSource->current.z;
    distance = sqrtf((deltaX * deltaX) + (deltaY * deltaY) +
                     (deltaZ * deltaZ));

    firstTarget->unk1C = deltaX / distance;
    firstTarget->unk20 = deltaY / distance;
    firstTarget->unk24 = deltaZ / distance;
    secondTarget->unk1C = -firstTarget->unk1C;
    secondTarget->unk20 = -firstTarget->unk20;
    secondTarget->unk24 = -firstTarget->unk24;
    TrapDanglingJump(first, 6, firstTarget);
    TrapDanglingJump(second, 0xA);
}
#ifdef NON_MATCHING
/*
 * PROVENANCE: JFG's public assembly-only hitVectorCheck establishes the
 * collision role and broad case ordering. This typed body is reconstructed
 * from Mickey's target, its m2c dataflow, and Mickey's neighboring collision
 * helpers; Mickey's bytes remain authoritative.
 */
s32 func_800563B4(s32 object, AnimCollisionShape *first, s32 value,
                  AnimCollisionShape *second, AnimCollisionResult *result) {
    AnimVec3f axis;
    AnimVec3f direction;
    AnimVec3f point;
    AnimVec3f endpoint;
    AnimVec3f minimum;
    AnimVec3f maximum;
    f32 length;
    f32 radius;
    f32 near;
    f32 far;
    f32 normalX;
    f32 normalY;
    f32 normalZ;
    f32 projection;
    s32 face;
    s32 status;

    direction.x = first->vector.x;
    direction.y = first->vector.y;
    radius = first->radius + second->radius;
    direction.z = first->vector.z;
    status = 0;
    length = (direction.z * direction.z) +
             ((direction.x * direction.x) +
              (direction.y * direction.y));

    if (second->shape == 0) {
        if (length > 0.0f) {
            length = sqrtf(length);
            direction.x /= length;
            direction.y /= length;
            direction.z /= length;
        }
        if (func_80012574(&first->position, &direction, &second->edge,
                          radius, &near, &far) != 0) {
            if ((near >= 0.0f) && (near <= length)) {
                status = 1;
                point.x = (direction.x * near) + first->position.x;
                point.y = (direction.y * near) + first->position.y;
                point.z = (direction.z * near) + first->position.z;
                normalX = (point.x - second->edge.x) / radius;
                normalY = (point.y - second->edge.y) / radius;
                near /= length;
                normalZ = (point.z - second->edge.z) / radius;
            } else if ((first->flags & 2) && (near < 0.0f) && (far > 0.0f)) {
                status = 2;
            }
        }
    } else if (second->shape == 1) {
        if (length > 0.0f) {
            length = sqrtf(length);
            direction.x /= length;
            direction.y /= length;
            direction.z /= length;
        }
        axis.x = 0.0f;
        axis.z = 0.0f;
        axis.y = 1.0f;
        if (func_80012234(&first->position, &direction, &second->position,
                          &axis, radius, &near, &far) != 0) {
            if ((near >= 0.0f) && (near <= length)) {
                point.x = (direction.x * near) + first->position.x;
                point.y = (direction.y * near) + first->position.y;
                point.z = (direction.z * near) + first->position.z;
                projection =
                    (((point.x - second->position.x) * axis.x) +
                     ((point.y - second->position.y) * axis.y) +
                     ((point.z - second->position.z) * axis.z)) /
                    ((axis.z * axis.z) +
                     ((axis.x * axis.x) + (axis.y * axis.y)));
                if ((-second->height <= projection) &&
                    (projection <= second->height)) {
                    status = 1;
                    normalX =
                        (point.x - ((axis.x * projection) +
                                    second->position.x)) / radius;
                    normalY =
                        (point.y - ((axis.y * projection) +
                                    second->position.y)) / radius;
                    normalZ =
                        (point.z - ((axis.z * projection) +
                                    second->position.z)) / radius;
                    near /= length;
                }
            } else if ((first->flags & 2) && (near < 0.0f) && (far > 0.0f)) {
                status = 2;
            }
        }
        if (status == 0) {
            endpoint.x = second->position.x - (axis.x * second->height);
            endpoint.y = second->position.y - (axis.y * second->height);
            endpoint.z = second->position.z - (axis.z * second->height);
            if (func_80012574(&first->position, &direction, &endpoint,
                              radius, &near, &far) != 0) {
                if ((near >= 0.0f) && (near <= length)) {
                    status = 1;
                    point.x = (direction.x * near) + first->position.x;
                    point.y = (direction.y * near) + first->position.y;
                    point.z = (direction.z * near) + first->position.z;
                    normalX = (point.x - endpoint.x) / radius;
                    normalY = (point.y - endpoint.y) / radius;
                    normalZ = (point.z - endpoint.z) / radius;
                    near /= length;
                } else if ((first->flags & 2) && (near < 0.0f) &&
                           (far > 0.0f)) {
                    status = 2;
                }
            }
        }
        if (status == 0) {
            endpoint.x = (axis.x * second->height) + second->position.x;
            endpoint.y = (axis.y * second->height) + second->position.y;
            endpoint.z = (axis.z * second->height) + second->position.z;
            if (func_80012574(&first->position, &direction, &endpoint,
                              radius, &near, &far) != 0) {
                if ((near >= 0.0f) && (near <= length)) {
                    status = 1;
                    point.x = (direction.x * near) + first->position.x;
                    point.y = (direction.y * near) + first->position.y;
                    point.z = (direction.z * near) + first->position.z;
                    normalX = (point.x - endpoint.x) / radius;
                    normalY = (point.y - endpoint.y) / radius;
                    normalZ = (point.z - endpoint.z) / radius;
                    near /= length;
                } else if ((first->flags & 2) && (near < 0.0f) &&
                           (far > 0.0f)) {
                    status = 2;
                }
            }
        }
    } else if (second->shape == 2) {
        minimum.x = (second->edge.x - second->radius) - first->radius;
        minimum.y = (second->edge.y - second->height) - first->height;
        minimum.z = (second->edge.z - second->radius) - first->radius;
        maximum.x = second->edge.x + second->radius + first->radius;
        maximum.y = second->edge.y + second->height + first->height;
        maximum.z = second->edge.z + second->radius + first->radius;
        face = func_800131AC(&first->position, &direction, &minimum, &maximum,
                             &near, &far);
        if ((face != 0) && (near >= 0.0f) && (near <= 1.0f)) {
            switch (face) {
                case 1:
                    normalY = 0.0f;
                    normalZ = 0.0f;
                    status = 1;
                    normalX = -1.0f;
                    break;
                case 2:
                    normalX = 1.0f;
                    normalY = 0.0f;
                    normalZ = 0.0f;
                    status = 1;
                    break;
                case 3:
                    normalX = 0.0f;
                    normalZ = 0.0f;
                    status = 1;
                    normalY = -1.0f;
                    break;
                case 4:
                    normalX = 0.0f;
                    normalY = 1.0f;
                    normalZ = 0.0f;
                    status = 1;
                    break;
                case 5:
                    normalX = 0.0f;
                    normalY = 0.0f;
                    status = 1;
                    normalZ = -1.0f;
                    break;
                case 6:
                    normalX = 0.0f;
                    normalY = 0.0f;
                    normalZ = 1.0f;
                default:
                    status = 1;
                    break;
            }
        } else if (first->flags & 2) {
            if ((minimum.x <= first->position.x) &&
                (first->position.x <= maximum.x) &&
                (minimum.y <= first->position.y) &&
                (first->position.y <= maximum.y) &&
                (minimum.z <= first->position.z) &&
                (first->position.z <= maximum.z)) {
                status = 2;
            }
        }
    }

    if (status == 1) {
        result->object = object;
        result->value = value;
        near -= D_8008420C;
        if (near < 0.0f) {
            near = 0.0f;
        }
        result->fraction = near;
        result->normal.x = normalX;
        result->normal.y = normalY;
        result->normal.z = normalZ;
    }
    return status;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_800563B4.s")
#endif

f32 func_8002A8BC(s16 angle);
f32 func_8002A8C0(s16 angle);

/*
 * Plateau (2026-08-26, p5): workbench mixed, 229/226 instructions and 214 raw
 * differing words from +0x0; frames are 0x70/0x80, with 233 structural and 54
 * register rows. Prior flag, lifetime, spill, volatile, state-layout, and
 * permuter probes were exhausted; the compound-assignment operand-order probe
 * grew the candidate to 227 instructions and a 0x88 frame, then was reverted.
 * No frame-recovery lever with source evidence remains; retain NON_MATCHING.
 */
#ifdef NON_MATCHING
void func_80056DD8(HitCopyState *first, HitCopyState *second,
                   AnimVec3f *normal, f32 timeStep) {
    HitCopyTarget *target;
    HitCopySource *firstSource;
    HitCopySource *secondSource;
    f32 velocityX;
    f32 velocityY;
    f32 velocityZ;
    f32 magnitude;
    f32 impulse;
    f32 correction;
    f32 cosine;
    f32 sine;
    f32 offsetX;
    f32 offsetY;
    f32 offsetZ;
    f32 dot;
    f32 previousX;
    f32 previousY;
    f32 previousZ;
    f32 displacement;
    volatile f32 retained;

    target = first->target;
    velocityX = target->velocity.x;
    velocityY = target->velocity.y;
    velocityZ = target->velocity.z;
    firstSource = first->source;
    secondSource = second->source;
    if (((velocityZ * velocityZ) +
         ((velocityX * velocityX) + (velocityY * velocityY))) > 25.0f) {
        f32 mass;

        mass = ((HitCopyTarget *) TrapDanglingJump(target))->unk4;
        velocityX = target->velocity.x;
        velocityY = target->velocity.y;
        velocityZ = target->velocity.z;
        impulse = ((secondSource->unk6C + 1.0f) *
                   ((normal->z * velocityZ) +
                    ((velocityX * normal->x) +
                     (velocityY * normal->y)))) / (1.0f / mass);
        correction = impulse / mass;
        retained = impulse;
        target->velocity.x = velocityX - (correction * normal->x);
        target->velocity.y = velocityY - (correction * normal->y);
        target->velocity.z = velocityZ - (correction * normal->z);
        magnitude = sqrtf((target->velocity.z * target->velocity.z) +
                          ((target->velocity.x * target->velocity.x) +
                           (target->velocity.y * target->velocity.y)));
        target->magnitude80 = magnitude;
        target->magnitude84 = magnitude;
        target->direction.x = target->velocity.x / magnitude;
        target->direction.y = target->velocity.y / magnitude;
        target->direction.z = target->velocity.z / magnitude;
        target->unk181 = 1;
        target->unk4 = 0.0f;
        target->unk8 = 0.0f;
        target->unk88 = D_80084210;
        firstSource->unk63 = 1;
        secondSource->unk63 = 1;
        secondSource->unk64 = magnitude;
        cosine = -func_8002A8C0(*(s16 *) first);
        sine = -func_8002A8BC(*(s16 *) first);
        target->unk90 = (normal->z * cosine) - (normal->x * sine);
        target->unk8C = (normal->z * sine) + (cosine * normal->x);
        offsetY = first->position.y - firstSource->previous.y;
        offsetX = first->position.x - firstSource->previous.x;
        offsetZ = first->position.z - firstSource->previous.z;
        firstSource->previous.x =
            (target->velocity.x * timeStep) + firstSource->current.x;
        firstSource->previous.y =
            (target->velocity.y * timeStep) + firstSource->current.y;
        firstSource->previous.z =
            (target->velocity.z * timeStep) + firstSource->current.z;
        first->position.x = firstSource->previous.x + offsetX;
        first->position.y = firstSource->previous.y + offsetY;
        first->position.z = firstSource->previous.z + offsetZ;
    } else {
        dot = (normal->z * firstSource->current.z) +
              ((firstSource->current.x * normal->x) +
               (firstSource->current.y * normal->y));
        retained = -dot;
        previousZ = firstSource->previous.z;
        previousY = firstSource->previous.y;
        previousX = firstSource->previous.x;
        displacement = D_80084214 -
                       (((normal->z * previousZ) +
                         ((normal->x * previousX) +
                          (normal->y * previousY))) - dot);
        offsetY = first->position.y - previousY;
        offsetZ = first->position.z - previousZ;
        offsetX = first->position.x - previousX;
        firstSource->previous.x = previousX + (displacement * normal->x);
        firstSource->previous.y = previousY + (displacement * normal->y);
        firstSource->previous.z = previousZ + (displacement * normal->z);
        first->position.x = firstSource->previous.x + offsetX;
        first->position.y = firstSource->previous.y + offsetY;
        first->position.z = firstSource->previous.z + offsetZ;
        firstSource->unk62 = 1;
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_80056DD8.s")
#endif
#ifdef NON_MATCHING
/* Type pass: the shared collision overlays are neutral. Plateau (near-miss p5): workbench mixed(constant:2, structural:2, register:15), 18 words at 80 instructions/frame -0x28.
 * Levers: scalar/vector aliases, stack-spill/aggregate forms, expression spelling, and flag lattice; all regressed or inert.
 * Remains: FP pool/temp phase and two constant sites; assembly fallback stays canonical. */
void func_8005716C(HitCopyState *state, void *unused, AnimVec3f *normal,
                   f32 timeStep) {
    HitCopyTarget *target;
    HitCopySource *source;
    f32 velocityX;
    f32 velocityY;
    f32 normalXProduct;
    f32 velocityZ;
    f32 doubled;
    f32 highPad0;
    f32 highPad1;
    volatile f32 bounce;
    f32 lowPad;

    target = state->target;
    velocityX = state->velocity.x / target->unk4;
    velocityY = state->velocity.y / target->unk4;
    velocityZ = state->velocity.z / target->unk4;
    source = state->source;
    if (!target->unk0) {
        target->unk0 = 1;
    }
    target->unk4 *= D_80084218;

    doubled = -((normal->z * velocityZ) +
                ((velocityX * normal->x) + (velocityY * normal->y)));
    bounce = doubled;
    doubled = bounce;
    doubled += doubled;
    normalXProduct = normal->x * doubled;
    bounce = doubled;
    state->velocity.x = (normalXProduct + velocityX) * target->unk4;
    state->velocity.y = ((normal->y * bounce) + velocityY) * target->unk4;
    state->velocity.z = ((normal->z * bounce) + velocityZ) * target->unk4;

    state->position.x = source->current.x;
    state->position.y = source->current.y;
    state->position.z = source->current.z;
    source->previous.x = source->current.x + (state->velocity.x * timeStep);
    source->previous.y = source->current.y + (state->velocity.y * timeStep);
    source->previous.z = source->current.z + (state->velocity.z * timeStep);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_8005716C.s")
#endif
void func_800572AC(HitCopyState *state, void *unused, AnimVec3f *position,
                   f32 unusedFloat) {
    f32 currentX;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    HitCopySource *source;
    HitCopyTarget *target;

    source = state->source;
    currentX = source->current.x;
    target = state->target;
    deltaX = state->position.x - source->previous.x;
    deltaY = state->position.y - source->previous.y;
    deltaZ = state->position.z - source->previous.z;
    source->previous.x = currentX;
    source->previous.y = source->current.y;
    source->previous.z = source->current.z;
    state->position.x = currentX + deltaX;
    state->position.y = source->previous.y + deltaY;
    state->position.z = source->previous.z + deltaZ;
    target->unk14 = position->x;
    target->unk18 = position->y;
    target->unk1C = position->z;
    TrapDanglingJump(state, 0x16);
}
void func_80057350(HitCopyState *state, void *unused, AnimVec3f *position,
                   f32 unusedFloat) {
    HitCopySource *source;
    HitCopyTarget *target;

    source = state->source;
    target = state->target;
    state->position.x = source->current.x;
    state->position.y = source->current.y;
    state->position.z = source->current.z;
    source->previous.x = source->current.x;
    source->previous.y = source->current.y;
    source->previous.z = source->current.z;
    target->unk1C = position->x;
    target->unk20 = position->y;
    target->unk24 = position->z;
    TrapDanglingJump(state, 0xE);
}

/*
 * Mickey-led overlap response reconstruction; the nearest external skeleton
 * is only 0.085 similar and supplies no usable donor body.
 */
/* Workbench p7: structure/size mismatch, 233/231 instructions/frame -136, 203 raw words from +0x2C.
 * Context is clean; prior pointer-base, radius-tree, scaled-zero, and flag probes leave target’s folded AABB initialization as the first structural split.
 * FP pool/temp and tail integer webs remain; retain NON_MATCHING. */
#ifdef NON_MATCHING
void func_800573C8(HitOverlapState *state, HitOverlapVolume *other,
                   HitOverlapState *trigger, HitOverlapVolume *volume) {
    volatile f32 stackPad;
    f32 secondMin[3];
    f32 secondMax[3];
    f32 firstMin[3];
    f32 firstMax[3];
    f32 combinedRadius;
    f32 deltaX;
    f32 deltaY;
    f32 deltaZ;
    s32 index;
    s32 intersects;
    HitOverlapVehicle *vehicle;

    intersects = 0;
    if (volume->shape == 0) {
        combinedRadius = other->radius + volume->radius;
        deltaX = other->position.x;
        deltaX = volume->position.x - deltaX;
        deltaY = volume->position.y - other->position.y;
        deltaZ = volume->position.z - other->position.z;
        if (((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ)) <
            (combinedRadius * combinedRadius)) {
            intersects = 1;
        }
    } else if (volume->shape == 1) {
        combinedRadius = other->radius + volume->radius;
        deltaX = volume->position.x - other->position.x;
        deltaY = (volume->position.y - volume->height) - other->position.y;
        deltaZ = volume->position.z - other->position.z;
        if (((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ)) <
            (combinedRadius * combinedRadius)) {
            intersects = 1;
        }
    } else if (volume->shape == 2) {
        index = 0;
        intersects = 1;
        firstMin[0] = other->position.x - other->radius;
        firstMin[1] = other->position.y - other->height;
        firstMin[2] = other->position.z - other->radius;
        firstMax[0] = other->position.x + other->radius;
        firstMax[1] = other->position.y + other->height;
        firstMax[2] = other->position.z + other->radius;
        secondMin[0] = volume->position.x - volume->radius;
        secondMin[1] = volume->position.y - volume->height;
        secondMin[2] = volume->position.z - volume->radius;
        secondMax[0] = volume->position.x + volume->radius;
        secondMax[1] = volume->position.y + volume->height;
        secondMax[2] = volume->position.z + volume->radius;
        do {
            if ((firstMin[index] < secondMin[index]) &&
                (firstMax[index] < secondMin[index])) {
                goto no_intersection;
            }
            if ((secondMax[index] < firstMin[index]) &&
                (secondMax[index] < firstMax[index])) {
no_intersection:
                intersects = 0;
            }
            index++;
        } while ((index < 3) && (intersects != 0));
    }
    if (intersects != 0) {
        if (state->kind44 == 1) {
            vehicle = state->vehicle;
            if (vehicle->unk16A != 0) {
                vehicle->unk16A = 0;
            }
            if (volume->position.y < volume->unk1C) {
                vehicle->overlap54 =
                    ((volume->position.y - volume->height) -
                     state->position.y) /
                    ((other->position.y - state->position.y) +
                     other->height);
                if (vehicle->overlap54 < 0.0f) {
                    vehicle->overlap54 = 0.0f;
                }
            }
            if (trigger->kind44 == 0x52) {
                if (vehicle->target338 == NULL) {
                    vehicle->target338 = trigger;
                    vehicle->timer18D = 0x1E;
                }
            } else {
                vehicle->timer18D = 0x1E;
            }
            volume->active = 1;
        } else if (state->kind44 == 0x40) {
            TrapDanglingJump(state, 1);
        } else if (state->kind44 == 0x39) {
            TrapDanglingJump(state, 5);
        } else if (state->kind44 == 0x3A) {
            TrapDanglingJump(state, 5);
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_800573C8.s")
#endif
#ifdef NON_MATCHING
/*
 * PROVENANCE: adapted from JFG's src/hit.c hitPlayer assembly. Mickey's ROM
 * establishes the entity cutoff, resident structures, and final code here.
 */
/* Type pass: the shared collision-state overlay is neutral. Plateau: workbench mixed structural/register, 105/105 instructions and 51 words, first divergence +0x24.
 * Levers tried: prior radius/delay-slot, squared-radius lifetime, statement-order/tie, sort-cursor, flag, and context probes.
 * Remaining: IDO's radius/call schedule and register web stay displaced; the target/candidate sqrtf relocation identities also differ. */
s32 func_8005776C(f32 x, f32 y, f32 z, f32 radius, s32 useXZ,
                  HitCopyState **nearby) {
    f32 distances[8];
    HitCopyState **players;
    HitCopyState *player;
    HitCopyState **nearbyEntry;
    f32 *distance;
    f32 *lastDistance;
    f32 deltaY;
    f32 distanceSquared;
    f32 currentDistance;
    s32 playerCount;
    s32 found;
    s32 remaining;
    s32 nearbyOffset;

    found = 0;
    radius *= radius;
    players = func_80005750(&playerCount);
    if (playerCount > 0) {
        do {
            player = *players++;
            if ((*(s8 *) player->target >= 0) &&
                (*(s8 *) player->target < 6)) {
                f32 deltaX;
                f32 deltaZ;

                deltaX = player->position.x - x;
                deltaZ = player->position.z - z;
                distanceSquared = (deltaX * deltaX) + (deltaZ * deltaZ);
                if (useXZ == 0) {
                    deltaY = player->position.y - y;
                    distanceSquared += deltaY * deltaY;
                }
                if (distanceSquared < radius) {
                    distances[found] = sqrtf(distanceSquared);
                    nearby[found] = player;
                    found++;
                }
            }
            playerCount--;
        } while (playerCount > 0);

        remaining = found - 1;
        if (remaining > 0) {
            do {
                distance = distances;
                lastDistance = &distance[remaining];
                nearbyOffset = 0;
                do {
                    currentDistance = distance[0];
                    nearbyEntry = (HitCopyState **)
                        ((u8 *) nearby + nearbyOffset);
                    if (distance[1] < currentDistance) {
                        player = nearbyEntry[0];
                        distance[0] = distance[1];
                        nearbyEntry[0] = nearbyEntry[1];
                        distance[1] = currentDistance;
                        nearbyEntry[1] = player;
                    }
                    distance++;
                    nearbyOffset += sizeof(*nearby);
                } while (distance < lastDistance);
                remaining--;
            } while (remaining != 0);
        }
    }
    return found;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/anim/func_8005776C.s")
#endif
/*
 * PROVENANCE: adapted from JFG's src/fmvInit.c. Mickey's ROM establishes the
 * resource ID, globals, structure layout, and final compiler output here.
 */
void fmvInit(void) {
    FmvPlayer *player;
    s32 i;

    D_800D76D0[0] = piRomLoad(0x41);
    player = D_800D76D8;

    i = 2;
    while (i--) {
        player->unk0 = -1;
        player->unk14 = 0;
        player->unk18 = 0;
        player->unk1C = 0;
        player->unk20 = 0;
        player++;
    }
}

/* PLATEAU-HANDOFF:func_80051364:start
 * symbol: func_80051364
 * score: 95/287 words
 * frame: 0x48
 * relocations: 51
 * first-mismatch: +0x0
 * summary: Target is frame 0x40 with 47 relocations. Next isolate one source-authentic playback-state carrier lifetime without expanding the frame.
 * PLATEAU-HANDOFF:func_80051364:end
 */

/* PLATEAU-HANDOFF:func_80050E9C:start
 * symbol: func_80050E9C
 * score: 84/90 words
 * frame: 0x20
 * relocations: 41
 * first-mismatch: +0x64
 * summary: Authenticated UGEN markers map the target uniquely to ordinal 24 and isolate one condition-line temp birth; folded preincrement stays 84/90 and is neutral.
 * PLATEAU-HANDOFF:func_80050E9C:end
 */

/* PLATEAU-HANDOFF:func_80050BF4:start
 * symbol: func_80050BF4
 * score: 72/87 words
 * frame: 0x18
 * relocations: 41
 * first-mismatch: +0x34
 * summary: Ten natural forms leave the clear-loop bound web; fallback-static resolves 41/41 identities with 31 exact, and five endpoint/base LO16 pairs remain reversed.
 * PLATEAU-HANDOFF:func_80050BF4:end
 */

/* PLATEAU-HANDOFF:func_80054B3C:start
 * symbol: func_80054B3C
 * score: 365 differing words
 * frame: 0x188
 * relocations: 3
 * first-mismatch: +0x0
 * summary: typed m2c-faithful candidate is one instruction long; frame allocation remains 0x188 versus 0xD8 and all three sqrtf offsets are non-exact
 * PLATEAU-HANDOFF:func_80054B3C:end
 */

/* PLATEAU-HANDOFF:func_80056DD8:start
 * symbol: func_80056DD8
 * score: 214 differing words
 * frame: 0x80
 * relocations: 8
 * first-mismatch: +0x0
 * summary: Target is 229 words/frame 0x70; candidate is 226/frame 0x80; 1/8 relocation identities aligns; reopen only with a source-authentic frame lever.
 * PLATEAU-HANDOFF:func_80056DD8:end
 */

/* PLATEAU-HANDOFF:func_8005776C:start
 * symbol: func_8005776C
 * score: 54/105 words
 * frame: 0xC0
 * relocations: 2
 * first-mismatch: +0x24
 * summary: Fidelity trace proves -Wab,-r4300_mul fixes the first 28 words and sqrtf site, but the sort cursor then hoists a stack base and leaves 106 words.
 * PLATEAU-HANDOFF:func_8005776C:end
 */

/* PLATEAU-HANDOFF:func_80055104:start
 * symbol: func_80055104
 * score: 425 differing words
 * frame: 0xD0
 * relocations: 23
 * first-mismatch: +0x0
 * summary: Target 445 words and frame 0xB8; relocation identities agree, but stack lifetimes drift. Add resident size metadata to unlock donor and flag tools.
 * PLATEAU-HANDOFF:func_80055104:end
 */

/* PLATEAU-HANDOFF:func_8005716C:start
 * symbol: func_8005716C
 * score: 18 differing words
 * frame: -0x28
 * relocations: 2
 * first-mismatch: +0x54
 * summary: Add unique resident owner metadata to unlock flags; retained 18-word form is diagnostic only because its volatile/padding scaffolds are not promotable.
 * PLATEAU-HANDOFF:func_8005716C:end
 */

/* PLATEAU-HANDOFF:func_800563B4:start
 * symbol: func_800563B4
 * score: 12/649 words
 * frame: 0xD8
 * relocations: 11
 * first-mismatch: +0x1C
 * summary: Candidate is 40 words short with 637 differences and the exact frame; both sides have 11 relocations, but only one site/identity aligns and two candidate identities are unresolved.
 * PLATEAU-HANDOFF:func_800563B4:end
 */
