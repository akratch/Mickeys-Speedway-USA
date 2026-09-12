#include "PR/ultratypes.h"
#include "overlays/overlay_045.h"

typedef struct O57ResourceTable {
    void *entries[0x70];
} O57ResourceTable;

typedef struct O57SeedData {
    u8 pad00[0xC];
    s32 value0C;
    s32 value10;
    s32 value14;
    u8 pad18[4];
    s32 value1C;
    u8 pad20[0x1CC];
    s32 value1EC;
    s32 value1F0;
    s32 value1F4;
} O57SeedData;

typedef struct O57AnimObject {
    s16 angle;
    u8 pad02[6];
    f32 scale;
    f32 x;
    f32 y;
    f32 z;
} O57AnimObject;

typedef struct O57AnimPath {
    u8 pad00[8];
    O57AnimObject *object;
} O57AnimPath;

typedef struct O57SpawnPacket {
    s16 kind;
    u8 mode;
    u8 flags;
    s16 x;
    s16 y;
    s16 z;
    s16 angle;
    s32 state;
    f32 scale;
} O57SpawnPacket;

typedef struct O57FinalSpawnPacket {
    s16 kind;
    u8 mode;
    u8 flags;
    s16 x;
    s16 y;
    s16 z;
    u8 byte0A;
    u8 byte0B;
    s8 state;
} O57FinalSpawnPacket;

/* CORRECTED 2026-09-12, lane p11-mid.  This used to read "giving each its own
 * local reserves a second 20-byte home the target does not have".  The target
 * DOES have it: tools/frame_census.py reads two disjoint packet blocks in the
 * target's 24 stack homes, a 20-byte one at +0x54 and a 13-byte one at +0x3C,
 * against our single union at +0x78.  Splitting the union reproduces both with
 * their internal structure matching slot for slot and moves the size delta
 * from -20 to -12; it scores worse (282 -> 335) only because every home below
 * it shifts, and the frame excess that causes is the block-scoped register
 * locals below the packets, not the packets.  See the handoff block below. */
typedef union O57SpawnUnion {
    O57SpawnPacket initial;
    O57FinalSpawnPacket final;
} O57SpawnUnion;

typedef struct O57SpawnState {
    u8 pad00[8];
    s16 mode;
} O57SpawnState;

typedef struct O57Spawned {
    u8 pad00[0x3C];
    s32 field3C;
    u8 pad40[0x28];
    O57SpawnState **state;
} O57Spawned;

typedef struct O57ModeObject {
    s32 value;
} O57ModeObject;

typedef struct O57ModeResult {
    u8 pad00[0x16];
    u8 flags;
} O57ModeResult;

typedef struct O57Choice {
    u8 pad00[0x2A];
    s8 enabled;
    u8 pad2B[9];
} O57Choice;

typedef struct O57Pair {
    s16 first;
    s16 second;
} O57Pair;

extern void *gO57Current100Reloc;
extern s32 gO57Pending104Reloc;
extern void *gO57Previous108Reloc;
extern void *gO57Runtime1B8Reloc;
extern void *gO57ResidentCurrentReloc;
extern O57ResourceTable *gO57ResourceTableReloc;

extern Overlay45ResourceDescriptor *gO57Descriptor00Reloc;
extern Overlay45ResourceDescriptor *gO57Descriptor00PrepareReloc;
extern Overlay45ResourceDescriptor *gO57DescriptorF8Reloc;
extern Overlay45ResourceDescriptor *gO57DescriptorF8PrepareReloc;
extern Overlay45ResourceDescriptor *gO57Descriptor6CReloc;
extern Overlay45ResourceDescriptor *gO57Descriptor6CPrepareReloc;
extern Overlay45ResourceDescriptor *gO57Descriptor70Reloc;
extern Overlay45ResourceDescriptor *gO57Descriptor70PrepareReloc;
extern Overlay45ResourceDescriptor *gO57Descriptor74Reloc;
extern Overlay45ResourceDescriptor *gO57Descriptor74PrepareReloc;
extern Overlay45ResourceDescriptor *gO57Descriptor78Reloc;
extern Overlay45ResourceDescriptor *gO57Descriptor78PrepareReloc;
extern Overlay45ResourceDescriptor *gO57DescriptorFCReloc;
extern Overlay45ResourceDescriptor *gO57Descriptors08Reloc[9];
extern Overlay45ResourceDescriptor *gO57Descriptors2CEndReloc[];
extern Overlay45ResourceDescriptor *gO57Descriptors30Reloc[9];
extern Overlay45ResourceDescriptor *gO57Descriptors54EndReloc[];
extern Overlay45ResourceDescriptor *gO57Descriptors80Reloc[24];
extern Overlay45ResourceDescriptor *gO57DescriptorsE0EndReloc[];
extern Overlay45ResourceDescriptor *gO57DescriptorsE0Reloc[6];
extern Overlay45ResourceDescriptor *gO57DescriptorsF8EndReloc[];
extern Overlay45ResourceDescriptor *gO57Descriptors58Reloc[5];
extern Overlay45ResourceDescriptor *gO57Descriptors6CEndReloc[];
extern Overlay45ResourceDescriptor *gO57DescriptorList80Reloc;
extern s16 gO57DescriptorIndicesD8Reloc[9];
extern s16 gO57DescriptorIndicesECReloc[9];
extern s16 gO57DescriptorIndices100Reloc[24];
extern s16 gO57DescriptorIndices130Reloc[6];
extern s16 gO57DescriptorList130Reloc;

extern O57SeedData gO57SeedDataReloc;
extern s32 gO57Value1FCReloc;
extern s32 gO57Value21CReloc;
extern s32 gO57Value23CReloc;
extern s32 gO57Value25CReloc;
extern s32 gO57Value32CReloc;
extern s32 gO57Value34CReloc;
extern s32 gO57Value27CReloc;
extern s32 gO57Value28CReloc;
extern s32 gO57Values29CReloc[32];

extern s32 gO57Mode11CReloc;
extern s32 gO57Value128Reloc;
extern s32 gO57Value12CReloc;
extern s32 gO57Ids134Reloc[];
extern s32 gO57Value138Reloc;
extern s32 gO57Value13CReloc;
extern s32 gO57Active144Reloc;
extern s32 gO57Value148Reloc;
extern s32 gO57Value14CReloc;
extern s32 gO57Value160Reloc;
extern s32 gO57Value164Reloc;
extern s32 gO57Value50CReloc;
extern s32 gO57NodeValues17CReloc[];
extern s16 gO57ColourCycle1A8Reloc;

extern s32 gO57State118Reloc;
extern O57ModeObject gO57ModeObject180Reloc;
extern s32 gO57SpecialModeReloc;
extern u8 gO57SpecialByteReloc;
extern s32 gO57SpecialConditionReloc;
extern O57Choice gO57ChoicesReloc[4];
extern u8 gO57ChoiceMaskReloc;
extern u8 gO57ChoiceMaskReferenceReloc;
extern u8 gO57ChoiceSourceReloc;
extern s32 gO57ChoiceDirtyReloc;
extern u8 gO57ChoicePublishedReloc;

extern s32 gO57Value188Reloc;
extern s32 gO57Value18CReloc;
extern s32 gO57Value190Reloc;
extern u16 gO57ResidentFlags10Reloc;
extern s32 gO57Value198Reloc;
extern s32 gO57Value19CReloc;
extern f32 gO57Value110Reloc;
extern s32 gO57Value124Reloc;
extern O57Pair gO57SpawnPairs3E8Reloc[4];
extern O57Spawned *gO57Spawned150Reloc[4];

extern void *func_80028F54(void);
extern void func_8004B0A4(s32 font);
extern void fontColour(s32 red, s32 green, s32 blue, s32 alpha, s32 opacity);
extern void o57PrepareDescriptorReloc(Overlay45ResourceDescriptor *descriptor,
                                      s32 value);
extern void o57PrepareDescriptorListReloc(void *list);
extern void animseqStartPath(u8 pathId);
extern O57AnimPath *func_800508B4(u8 pathId);
extern O57Spawned *func_8000590C(void *packet, s32 mode);
extern void initColourCycle(void *cycle, s32 count);
extern void joyResetMap(void);
extern s32 o57QueryModeReloc(void);
extern void overlay57SetNodeValue(s32 id, s32 argument, s32 valueBits);
extern void o57PublishChoicesReloc(void);
extern void func_8003A754(void);
extern void func_8005AD64(O57Spawned *spawned, s32 mode, s32 index,
                          s32 valueBits);

/* Workbench plateau: structure-mismatch, first divergence +0x8.
 *
 * 2026-09-12 (lane p12-o57): 282 -> 276 masked of 597 words, byte-exact
 * 410 -> 421, register naming 75, immediate 23 -> 17, really different
 * 98 -> 94, frame 0x90 -> 0x80 against the target's 0x78.  Two edits:
 *
 *  1. EVERY BLOCK-SCOPED `register` LOCAL IS MERGED TO FUNCTION SCOPE.  The
 *     seven inner blocks declared fourteen locals between them -- three
 *     separate `i` and two separate `spawned` -- and each reserves its own
 *     home (L99), so the block scoping was buying nothing and costing four
 *     slots.  Twelve function-scope locals is 0x90 -> 0x80 at an unchanged
 *     score, and it makes the ladder BELOW the packets exact: six slots on
 *     each side, +0x2C +0x28 +0x24 +0x20 +0x1C +0x10.
 *
 *  2. THE PACKET IS DECLARED SIXTH, not first.  A move-one climb over all
 *     thirteen declaration positions, 145 compiles, reaches 276 in one move
 *     and is then a fixed point.  What that move buys is the target's own
 *     packet offsets: the union lands at +0x54 .. +0x64 and the target's
 *     upper packet block is +0x54 .. +0x64, slot for slot, where before it
 *     sat 0x24 too high.
 *
 * WHAT IS LEFT, and it is now a COUNT rather than a placement.  The target
 * has 24 stack homes to this candidate's 17.  The seven it has and this does
 * not are its second packet block, +0x3C +0x3E +0x40 +0x42 +0x44 +0x46 +0x47
 * +0x48 -- the O57FinalSpawnPacket, which shares the union here.
 *
 * Splitting the union reproduces that block EXACTLY when `final` is declared
 * LAST of the thirteen: its block then lands on +0x3C +0x3E +0x40 +0x42 +0x46
 * +0x47 +0x48, the target's offsets with only the target's +0x44 store
 * unaccounted.  It is not adoptable yet because the frame goes 0x80 -> 0x90
 * and the score 276 -> 335.  The split is FLAT across placement: all eight
 * positions of `final` score 335, and a separate move-one climb over all
 * fourteen positions of both packets, 170 compiles, is a fixed point at 335.
 * So the split's cost is not a declaration-order artefact and no order sweep
 * will recover it.
 *
 * The free parameter is the LOCAL COUNT (L134).  With the split the frame is
 * 0x90 against 0x78: exactly six slots too many, and this candidate declares
 * twelve non-packet locals where the target's two gaps -- +0x4C .. +0x53 and
 * +0x6C .. +0x77 -- leave room for five or six.  Merging `value` into `id`
 * and `stride` into `i` alone, without the split, takes the frame to 0x78
 * EXACTLY (candidate 0x78, target 0x78) and costs 15 words, which is the
 * measurement that proves the count is the lever and that ten is two too few
 * once the union is one object rather than two.  The next edit is to find six
 * reuses among `current`, `descriptor`, `resourceIndex`, `descriptorEnd`,
 * `i`, `value`, `stride`, `spawned`, `path`, `result`, `entry` and `id` that
 * do not cost more than the split buys. */
#ifdef NON_MATCHING
void func_overlay_057_F0000000_18A3BF8(void) {
    u8 choiceMask;
    register void *current;
    register Overlay45ResourceDescriptor **descriptor;
    register s16 *resourceIndex;
    register Overlay45ResourceDescriptor **descriptorEnd;
    register s32 i;
    O57SpawnUnion packet;
    register s32 value;
    s32 stride;
    register O57Spawned *spawned;
    register O57AnimPath *path;
    register O57ModeResult *result;
    register s32 *entry;
    register s32 id;

    current = gO57ResidentCurrentReloc;
    gO57Current100Reloc = current;
    gO57Pending104Reloc = 0;
    choiceMask = 0;
    gO57Previous108Reloc = current;
    gO57Runtime1B8Reloc = func_80028F54();
    func_8004B0A4(3);
    fontColour(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);

    {

    gO57Descriptor00Reloc = overlay45CreateDescriptor(
        gO57ResourceTableReloc->entries[0x40], 0xA0, -0x28, 4);
    gO57DescriptorF8Reloc = overlay45CreateDescriptor(
        gO57ResourceTableReloc->entries[0x3C], 0xA0, -0x28, 4);
    gO57Descriptor6CReloc = overlay45CreateDescriptor(
        gO57ResourceTableReloc->entries[0x41], 0xA0, 0x104, 4);
    gO57Descriptor70Reloc = overlay45CreateDescriptor(
        gO57ResourceTableReloc->entries[0x42], 0xA0, 0x104, 4);
    gO57Descriptor74Reloc = overlay45CreateDescriptor(
        gO57ResourceTableReloc->entries[0x43], 0xA0, 0x104, 4);
    gO57Descriptor78Reloc = overlay45CreateDescriptor(
        gO57ResourceTableReloc->entries[0x44], 0xA0, 0x104, 4);
    gO57DescriptorFCReloc = overlay45CreateDescriptor(
        gO57ResourceTableReloc->entries[0x6D], 0xA0, -0x20, 4);

    resourceIndex = gO57DescriptorIndicesD8Reloc;
    descriptor = gO57Descriptors08Reloc;
    descriptorEnd = gO57Descriptors2CEndReloc;
    do {
        *descriptor = overlay45CreateDescriptor(
            gO57ResourceTableReloc->entries[*resourceIndex], 0xA0, 0xBE,
            0x204);
        overlay45SetMode(*descriptor, 0);
        descriptor++;
        resourceIndex++;
    } while (descriptor < descriptorEnd);

    resourceIndex = gO57DescriptorIndicesECReloc;
    descriptor = gO57Descriptors30Reloc;
    descriptorEnd = gO57Descriptors54EndReloc;
    do {
        *descriptor = overlay45CreateDescriptor(
            gO57ResourceTableReloc->entries[*resourceIndex], 0xA0, 0xBE,
            0x204);
        overlay45SetMode(*descriptor, 0);
        descriptor++;
        resourceIndex++;
    } while (descriptor < descriptorEnd);

    resourceIndex = gO57DescriptorIndices100Reloc;
    descriptor = gO57Descriptors80Reloc;
    descriptorEnd = gO57DescriptorsE0EndReloc;
    do {
        *descriptor = overlay45CreateDescriptor(
            gO57ResourceTableReloc->entries[*resourceIndex], 0xA0, 0x104, 4);
        overlay45SetMode(*descriptor, 0);
        descriptor++;
        resourceIndex++;
    } while (descriptor < descriptorEnd);

    resourceIndex = gO57DescriptorIndices130Reloc;
    descriptor = gO57DescriptorsE0Reloc;
    descriptorEnd = gO57DescriptorsF8EndReloc;
    do {
        *descriptor = overlay45CreateDescriptor(
            gO57ResourceTableReloc->entries[*resourceIndex], 0xA0, 0x104, 4);
        overlay45SetMode(*descriptor, 0);
        descriptor++;
        resourceIndex++;
    } while (descriptor < descriptorEnd);

    descriptor = gO57Descriptors58Reloc;
    descriptorEnd = gO57Descriptors6CEndReloc;
    i = 0;
    do {
        *descriptor = overlay45CreateDescriptor(
            gO57ResourceTableReloc->entries[0x36 + i], 0xA0, -0x28, 4);
        descriptor++;
        i++;
    } while (descriptor < descriptorEnd);

    o57PrepareDescriptorReloc(gO57Descriptor00PrepareReloc, 0xFF);
    o57PrepareDescriptorReloc(gO57DescriptorF8PrepareReloc, 0xFF);
    o57PrepareDescriptorReloc(gO57Descriptor6CPrepareReloc, 0xFF);
    o57PrepareDescriptorReloc(gO57Descriptor70PrepareReloc, 0xFF);
    o57PrepareDescriptorReloc(gO57Descriptor74PrepareReloc, 0xFF);
    o57PrepareDescriptorReloc(gO57Descriptor78PrepareReloc, 0xFF);
    o57PrepareDescriptorListReloc(&gO57DescriptorList80Reloc);
    o57PrepareDescriptorListReloc(&gO57DescriptorList130Reloc);
    }

    {

    gO57Value1FCReloc = gO57SeedDataReloc.value0C;
    value = gO57SeedDataReloc.value10;
    gO57Value21CReloc = value;
    gO57Value23CReloc = gO57SeedDataReloc.value14;
    gO57Value25CReloc = value;
    gO57Value32CReloc = gO57SeedDataReloc.value1EC;
    gO57Value34CReloc = gO57SeedDataReloc.value1F4;
    value = gO57SeedDataReloc.value1C;
    gO57Value27CReloc = value;
    gO57Value28CReloc = value;
    for (stride = 0; stride < 32; stride += 4) {
        gO57Values29CReloc[stride] = value;
    }
    }

    gO57Mode11CReloc = 0;
    gO57Value128Reloc = -0x50;
    gO57Value12CReloc = 0;
    gO57Ids134Reloc[0] = 0x40000;
    gO57Value138Reloc = 0x41800;
    gO57Value14CReloc = 0;
    gO57Value160Reloc = 0;
    gO57Value164Reloc = 0;
    gO57Value50CReloc = 0;
    gO57Value148Reloc = 0;
    gO57Value13CReloc = 0;

    animseqStartPath(0x3C);
    {

    path = func_800508B4(0x3C);
    if (path->object != 0) {
        packet.initial.mode = 0x14;
        packet.initial.flags = 0;
        packet.initial.x = path->object->x;
        packet.initial.y = path->object->y;
        packet.initial.z = path->object->z;
        packet.initial.angle = path->object->angle;
        packet.initial.kind = 0x35;
        packet.initial.state = 0;
        packet.initial.scale = path->object->scale;
        spawned = func_8000590C(&packet.initial, 1);
        if (spawned != 0) {
            spawned->field3C = 0;
        }
        packet.initial.kind = 0x38;
        packet.initial.state = 1;
        spawned = func_8000590C(&packet.initial, 1);
        if (spawned != 0) {
            spawned->field3C = 0;
        }
    }
    }

    initColourCycle(&gO57ColourCycle1A8Reloc, 0xA);
    gO57Active144Reloc = 0;
    joyResetMap();
    {

    switch (o57QueryModeReloc()) {
    case 4:
        gO57State118Reloc = 0xB;
        gO57ModeObject180Reloc.value = 0x2E;
        animseqStartPath(((u8 *)&gO57ModeObject180Reloc)[3]);
        result = (O57ModeResult *)func_800508B4(
            ((u8 *)&gO57ModeObject180Reloc)[3]);
        if (result != 0) {
            result->flags |= 2;
        }
        gO57SpecialModeReloc = 0;
        gO57SpecialByteReloc = 1;
        break;
    case 10:
        gO57State118Reloc = 7;
        entry = gO57Ids134Reloc;
        if (*entry != -1) {
            id = *entry;
            do {
                animseqStartPath(id & 0xFF);
                id = *entry;
                overlay57SetNodeValue(id, gO57NodeValues17CReloc[id],
                                      0x3BE56042);
                id = entry[1];
                entry++;
            } while (id != -1);
        }
        break;
    case 12:
        gO57State118Reloc = 1;
        entry = gO57Ids134Reloc;
        if (*entry != -1) {
            id = *entry;
            do {
                animseqStartPath(id & 0xFF);
                id = *entry;
                overlay57SetNodeValue(id, gO57NodeValues17CReloc[id],
                                      0x3BE56042);
                id = entry[1];
                entry++;
            } while (id != -1);
        }
        gO57Mode11CReloc = 1;
        break;
    case 17:
        gO57State118Reloc = 0xA;
        gO57ModeObject180Reloc.value = 0x50;
        animseqStartPath(((u8 *)&gO57ModeObject180Reloc)[3]);
        result = (O57ModeResult *)func_800508B4(
            ((u8 *)&gO57ModeObject180Reloc)[3]);
        if (result != 0) {
            result->flags |= 2;
        }
        gO57SpecialModeReloc = 5;
        break;
    case 18:
        gO57State118Reloc = 0x14;
        gO57ModeObject180Reloc.value = 0x54;
        animseqStartPath(((u8 *)&gO57ModeObject180Reloc)[3]);
        result = (O57ModeResult *)func_800508B4(
            ((u8 *)&gO57ModeObject180Reloc)[3]);
        if (result != 0) {
            result->flags |= 2;
        }
        if (gO57SpecialConditionReloc == 1) {
            gO57SpecialModeReloc = 7;
        } else {
            gO57SpecialModeReloc = 6;
        }
        break;
    }
    }

    {

    i = 0;
    do {
        if (gO57ChoicesReloc[i].enabled != 0) {
            choiceMask = (choiceMask | (1 << i)) & 0xFF;
        }
        i++;
    } while (i < 4);
    }
    if ((gO57ChoiceMaskReloc != choiceMask) ||
        (gO57ChoiceMaskReferenceReloc != gO57ChoiceSourceReloc)) {
        o57PublishChoicesReloc();
        gO57ChoiceDirtyReloc = 0;
        gO57ChoiceMaskReloc = choiceMask;
        gO57ChoicePublishedReloc = 1;
        gO57ChoiceMaskReferenceReloc = gO57ChoiceSourceReloc;
    }

    gO57Value188Reloc = 0;
    gO57Value18CReloc =
        ((((gO57ResidentFlags10Reloc & 0x1C0) >> 6) < 3) ^ 1);
    if (gO57Value18CReloc != 0) {
        gO57Value190Reloc = 3;
    } else {
        gO57Value190Reloc = 2;
    }
    func_8003A754();
    gO57Value198Reloc = 0;
    gO57Value19CReloc = 0;
    gO57Value110Reloc = -140.0f;
    gO57Value124Reloc = 0;

    {

    i = 0;
    do {
        packet.final.kind = 0x138;
        packet.final.mode = 0xE;
        packet.final.byte0A = 0;
        packet.final.state = 0;
        packet.final.byte0B = 0x80;
        packet.final.x = gO57SpawnPairs3E8Reloc[i].first;
        packet.final.y = gO57SpawnPairs3E8Reloc[i].second;
        spawned = func_8000590C(&packet.final, 0);
        gO57Spawned150Reloc[i] = spawned;
        (*spawned->state)->mode = 2;
        func_8005AD64(spawned, 0, 0, 0);
        i++;
    } while (i != 4);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o057/func_overlay_057_F0000000_18A3BF8/func_overlay_057_F0000000_18A3BF8.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_057_F0000000_18A3BF8:start
 * symbol: func_overlay_057_F0000000_18A3BF8
 * score: 276/597 words
 * frame: 0x80
 * relocations: 246
 * first-mismatch: +0x8
 * summary: 282 to 276: every block-scoped register local merged to function scope and the packet declared sixth; the frame excess is now a local count.
 * PLATEAU-HANDOFF:func_overlay_057_F0000000_18A3BF8:end
 */
