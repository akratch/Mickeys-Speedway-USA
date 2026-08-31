#include "PR/ultratypes.h"

typedef struct O98Object {
    u8 pad00[0x8E];
    u8 accepted;
} O98Object;

typedef struct O98Entry {
    O98Object *object;
    f32 value;
} O98Entry;

extern u8 *overlay98AcquireContextReloc(void);
extern s32 overlay98CheckObject(O98Object *, u8 *, f32 *);
extern s32 gOverlay98AcceptedCount;
extern O98Entry gOverlay98AcceptedEntries[0x50];

/* PROVENANCE: candidate source reproduced from the project's public mirror at
 * f56d08c746f891f76c4b7bab8e3a2a4332894634. All retained measurements and
 * the ABI correction above were independently derived from Mickey's own
 * source, object, relocation tables, and retail bytes. */
/* Natural C requires prohibited frame/home and schedule instruction edits to
 * reproduce retail. Keep it as a matching candidate; assembly is canonical. */
#ifdef NON_MATCHING
void overlay98CollectAccepted(s32 count, O98Object **objects) {
    f32 value;
    u8 *context;
    s32 index;

    context = overlay98AcquireContextReloc();
    gOverlay98AcceptedCount = 0;
    index = count - 1;
    if (index >= 0) {
        do {
            O98Object *object = objects[index];

            index--;
            if (overlay98CheckObject(object, context, &value) != 0) {
                O98Entry *entry;
                s32 next;

                object->accepted = 1;
                entry = &gOverlay98AcceptedEntries[gOverlay98AcceptedCount];
                next = gOverlay98AcceptedCount + 1;
                entry->object = object;
                gOverlay98AcceptedCount = next;
                entry->value = value;
                if (next >= 0x50) {
                    index = -1;
                }
            } else {
                object->accepted = 0;
            }
        } while (index >= 0);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o098/overlay98CollectAccepted/func_overlay_098_F0000144_18D8B04.s")
#endif

/* PLATEAU-HANDOFF:overlay98CollectAccepted:start
 * symbol: overlay98CollectAccepted
 * score: 48/60 words
 * frame: 0x58
 * relocations: 6
 * first-mismatch: +0x0
 * summary: Target frame 0x50; lexical initializer probe exhausted: entry-only/next-only 61w, both 60w/16 residual, all frame 0x58. Six runtime identities hold.
 * PLATEAU-HANDOFF:overlay98CollectAccepted:end
 */
