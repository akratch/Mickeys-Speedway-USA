#include "PR/ultratypes.h"
#include "overlays/overlay_045.h"

typedef struct Overlay57MenuSource {
    u8 pad00[0x28];
    s16 index;
    s8 active;
    u8 pad2B[9];
} Overlay57MenuSource;

typedef struct Overlay57MenuEntry {
    s32 type;
    s8 controller;
    u8 pad05[3];
    s32 value08;
    u8 pad0C[0x1C];
} Overlay57MenuEntry;

typedef struct Overlay57IndexLink {
    s32 index;
} Overlay57IndexLink;

extern s32 overlay84GetActive(void);
extern void overlay84ClearMode(void);
extern void overlay84ActivateCurrent(s32 kind);
extern void overlay84Mark(void);
extern void amSndPlay(s32 soundId, void *handle);
extern void animseqStartPath(s32 pathId);
extern void animseqStopPath(s32 pathId);
extern void joyCreateMap(s8 *activePlayers);
extern void mainSetMode(s32 mode);
extern void mainChangeCameras(s32 camera);
extern void mainChangeLevel(s32 level, s32 entrance, s32 vehicle,
                            s32 cutscene, s32 multiplayer, s32 arg5);

extern void func_overlay_057_F0001020_18A4C18(s32 updateRate);
extern void overlay57SetNodeValue(s32 id, s32 argument, s32 valueBits);

extern s32 gOverlay57State;
extern s32 gOverlay57DistanceState;
extern s32 gOverlay57Selection;
extern s32 gOverlay57PreviousSelection;
extern s32 gOverlay57LayoutValue;
extern s32 gOverlay57PreviousLayoutValue;
extern s32 gOverlay57LayoutBusy;
extern s32 gOverlay57Timer;
extern s32 gOverlay57ObjectReady;
extern u8 gOverlay57ObjectId;
extern Overlay45ResourceDescriptor *gOverlay57Layouts[];
extern Overlay57IndexLink gOverlay57StartLinks[];
extern Overlay57IndexLink gOverlay57StopLinks[];
extern s32 gOverlay57NodeArguments[];
extern s16 gOverlay57ControllerMap[];
extern Overlay57MenuEntry *gOverlay57MenuEntries;
extern s16 gOverlay57LevelBySelection[];
extern s16 gOverlay57EntranceByIndex[];
extern s16 gOverlay57EntranceIndex;
extern s16 gOverlay57DefaultEntrance;
extern s32 gOverlay57MenuCount;
extern s16 gOverlay57MenuSources[];
extern s32 gOverlay57MenuSourceIndex;
extern s16 gOverlay57MenuInputX;
extern s16 gOverlay57MenuInputY;
extern u32 gOverlay57MenuButtons;
extern s32 gOverlay57Camera;
extern u8 gOverlay57MirrorSelection;
extern u8 gOverlay57MirrorFlag;
extern u8 gOverlay57MenuByte0;
extern u8 gOverlay57MenuByte1;
extern u8 gOverlay57MenuByte2;
extern u8 gOverlay57MenuByte3;
extern u16 gOverlay57MenuHalf;
extern s16 gOverlay57MenuHalf2;
extern s8 gOverlay57MenuSourcesStart[];
extern s8 gOverlay57MenuSourcesEnd[];

/* Workbench: exact-size at 441 instructions and frame 0x60; 256 masked words.
 * `controller` is s8, not s32 -- as an s32 the two writes through it cost the
 * candidate 16 instructions of sign handling, and the size delta closes with
 * the narrower type alone. The menu-fill walk reuses `index` as its byte
 * offset rather than declaring one more local, which is the last 8 bytes of
 * frame.
 *
 * The temp-ring offset at +0x7C is now named.  It is two positions, and it is
 * not an onset at all: two LOOP-INVARIANT WEBS in the menu-fill loop -- the
 * menu-entry stride and the controller-map base -- are coloured t0 and t1 in
 * the candidate and s1 and s2 in the target.  A web holding a ring register
 * removes it from the temp ring for the WHOLE function, so the candidate's
 * ring runs two positions ahead of the target's from the first temp it takes
 * after the prologue.  That is why the offset is uniform and why its apparent
 * onset is simply the first temp-taking instruction.
 *
 * Measured: walking the entry pointer instead of indexing it frees both webs,
 * the first ring pick returns to the target's, and the score falls 256 -> 206
 * (delta -12, so not a candidate).  Freeing only the stride web returns the
 * first pick one position and scores 279.  So the 50 words are real and the
 * remaining question is an INDEXED form -- one that keeps the per-iteration
 * reload and multiply, and so the instruction count -- that still leaves both
 * webs off the ring.  The init loop's direction is inert (up, down do-while,
 * and down for-loop all score 256, 259, 256), and reading the controller map
 * twice instead of caching it costs 16 instructions. */
#ifdef NON_MATCHING
void func_overlay_057_F00060F8_18A9CF0(s32 updateRate) {
    Overlay57MenuSource *source;
    Overlay57IndexLink *link;
    s8 playerOrder[4];
    u8 activePlayers[10];
    s32 selection;
    s32 index;
    s32 count;
    s8 controller;

    if (overlay84GetActive() != 0) {
        return;
    }

    gOverlay57State = 6;
    if (gOverlay57MenuInputX < -16 && gOverlay57Selection > 0 &&
        gOverlay57DistanceState == 0) {
        gOverlay57LayoutBusy = 1;
        overlay45ConfigureLayout(gOverlay57Layouts[gOverlay57Selection], 0xA0, 0x104,
                                 0x104);
        gOverlay57PreviousSelection = gOverlay57Selection;
        gOverlay57Selection--;
        overlay45ConfigureLayout(gOverlay57Layouts[gOverlay57Selection], -0xA0, 0xBE,
                                 4);
        gOverlay57PreviousLayoutValue = gOverlay57LayoutValue;
        gOverlay57LayoutValue = 0xFF;
    } else if (gOverlay57MenuInputX >= 17 && gOverlay57Selection < 3 &&
               gOverlay57DistanceState == 0) {
        gOverlay57LayoutBusy = 1;
        overlay45ConfigureLayout(gOverlay57Layouts[gOverlay57Selection], 0xA0, 0x104,
                                 0x104);
        gOverlay57PreviousSelection = gOverlay57Selection;
        gOverlay57Selection++;
        overlay45ConfigureLayout(gOverlay57Layouts[gOverlay57Selection], 0x1E0, 0xBE,
                                 4);
        gOverlay57PreviousLayoutValue = gOverlay57LayoutValue;
        gOverlay57LayoutValue = 0xFF;
    } else if (gOverlay57MenuInputY < -16 && gOverlay57Selection < 2 &&
               gOverlay57DistanceState == 0) {
        gOverlay57LayoutBusy = 1;
        overlay45ConfigureLayout(gOverlay57Layouts[gOverlay57Selection], 0xA0, 0x104,
                                 0x104);
        gOverlay57PreviousSelection = gOverlay57Selection;
        gOverlay57Selection += 2;
        overlay45ConfigureLayout(gOverlay57Layouts[gOverlay57Selection], 0xA0, 0x104,
                                 0x104);
        gOverlay57PreviousLayoutValue = gOverlay57LayoutValue;
        gOverlay57LayoutValue = 0xFF;
    } else if (gOverlay57MenuInputY >= 17 && gOverlay57Selection >= 2 &&
               gOverlay57DistanceState == 0) {
        gOverlay57LayoutBusy = 1;
        overlay45ConfigureLayout(gOverlay57Layouts[gOverlay57Selection], 0xA0, 0x104,
                                 0x104);
        gOverlay57PreviousSelection = gOverlay57Selection;
        gOverlay57Selection -= 2;
        overlay45ConfigureLayout(gOverlay57Layouts[gOverlay57Selection], 0xA0, 0x104,
                                 4);
        gOverlay57PreviousLayoutValue = gOverlay57LayoutValue;
        gOverlay57LayoutValue = 0xFF;
    }

    if ((gOverlay57MenuButtons & 0x4000) && gOverlay57DistanceState == 0 &&
        gOverlay57ObjectReady != 0) {
        amSndPlay(13, 0);
        overlay84ClearMode();
        animseqStopPath(gOverlay57ObjectId);
        gOverlay57Timer = 7;
        gOverlay57State = 0;
        overlay84ActivateCurrent(1);

        link = gOverlay57StartLinks;
        if (link->index != -1) {
            index = link->index;
            do {
                animseqStartPath(index & 0xFF);
                overlay57SetNodeValue(index, gOverlay57NodeArguments[index],
                                      0x3BE56042);
                index = (++link)->index;
            } while (index != -1);
        }
        link = gOverlay57StopLinks;
        if (link->index != -1) {
            index = link->index;
            do {
                animseqStopPath(index & 0xFF);
                index = (++link)->index;
            } while (index != -1);
        }
    }

    func_overlay_057_F0001020_18A4C18(updateRate);
    if ((gOverlay57MenuButtons & 0x9000) && gOverlay57DistanceState == 0) {
        amSndPlay(12, 0);

        for (index = 0; index < 10; index++) {
            activePlayers[index] = 1;
        }

        source = (Overlay57MenuSource *)gOverlay57MenuSourcesStart;
        index = 0;
        count = 0;
        while ((s8 *)source < gOverlay57MenuSourcesEnd) {
            playerOrder[index++] = source->active;
            if (source->active != 0) {
                controller = gOverlay57ControllerMap[source->index];
                gOverlay57MenuEntries[count++].controller = controller;
                activePlayers[controller] = 0;
            }
            source++;
        }

        controller = 0;
        index = gOverlay57MenuCount * sizeof(Overlay57MenuEntry);
        while (index < 6 * (s32)sizeof(Overlay57MenuEntry)) {
            while (activePlayers[controller] == 0) {
                controller++;
            }
            ((Overlay57MenuEntry *)((u8 *)gOverlay57MenuEntries + index))->controller = controller++;
            index += sizeof(Overlay57MenuEntry);
        }

        joyCreateMap(playerOrder);
        mainSetMode(0);
        mainChangeCameras(gOverlay57Camera);
        gOverlay57MenuEntries[0].type = 5;
        selection = gOverlay57Selection;
        if (selection == 1) {
            gOverlay57MenuByte0 = 4;
            gOverlay57MenuByte1 = 3;
        } else if (gOverlay57MirrorFlag != 0) {
            gOverlay57MenuByte0 = 4;
            gOverlay57MenuByte1 = 4 - selection;
        } else {
            gOverlay57MenuByte0 = selection;
            gOverlay57MenuByte1 = 0;
        }
        gOverlay57MenuByte2 = selection >= 2;
        gOverlay57MenuByte3 = 2;
        gOverlay57MenuEntries[0].value08 = 0;
        gOverlay57MenuEntries[1].value08 = 0;
        gOverlay57MenuEntries[2].value08 = 0;
        gOverlay57MenuEntries[3].value08 = 0;
        gOverlay57MenuEntries[4].value08 = 0;
        gOverlay57MenuEntries[5].value08 = 0;
        gOverlay57MenuHalf = 0x3FC;
        gOverlay57MirrorSelection = 0;
        mainChangeLevel(gOverlay57LevelBySelection[gOverlay57Selection],
                        gOverlay57EntranceByIndex[gOverlay57EntranceIndex],
                        0, 5, 1, 0);
        gOverlay57DistanceState = 1;
        overlay84Mark();
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/overlays/o057/func_overlay_057_F00060F8_18A9CF0/func_overlay_057_F00060F8_18A9CF0.s")
#endif

/* PLATEAU-HANDOFF:func_overlay_057_F00060F8_18A9CF0:start
 * symbol: func_overlay_057_F00060F8_18A9CF0
 * score: 256/441 words
 * frame: 0x60
 * relocations: 175
 * first-mismatch: +0x14
 * summary: The +2 temp-ring offset is two loop-invariant webs in the menu-fill loop, the menu-entry stride and the controller-map base, coloured t0 and t1 here and s1 and s2 in the target, which removes both from the ring for the whole function; walking the entry pointer frees them and closes 50 of the 256 words but costs 12 instructions, so what is wanted is an indexed form that keeps the reload and multiply while leaving both webs off the ring.
 * PLATEAU-HANDOFF:func_overlay_057_F00060F8_18A9CF0:end
 */
