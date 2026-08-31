/*
 * Character and camera control -- ROM 0x1C790-0x20020
 * (VRAM 0x8001BB90-0x8001F420).
 *
 * The yaml boundaries were originally splat's aligned file-boundary
 * candidates. The content now supports the TU assignment independently: the
 * first routines follow JFG's camera-control cluster, exact skeleton anchors
 * identify func_8001C2D4 and controlSetPlayerSetup inside the block, the tail
 * is the same player-setup set/get/clear sequence, and the next yaml block
 * starts with a tier-A JFG models.c function. See docs/modules.md section 3.4.
 *
 * PROVENANCE -- Jet Force Gemini's public decomp src/charControl.c,
 * src/charControl.h, built charControl.c object, public symbol map, and
 * asm/nonmatchings/charControl filenames were consulted to identify the
 * translation unit and obtain comparison leads. Names not already supported
 * by tier-A evidence remain comments in docs/modules.md and are not adopted
 * here. Any future body adapted from JFG must carry its own PROVENANCE note
 * before that body and must be proved against Mickey's bytes.
 *
 * Flags: -O2 -mips2 -32 -Wab,-r4300_mul, measured on func_8001F09C.
 */

#include "PR/ultratypes.h"
#include "game/charControl.h"

extern u8 D_80079BF8;
extern u16 D_80079A0C[];
extern u16 D_80079A20[][4];
extern ControlGravityVector D_800799EC;
extern ControlGravityVector D_800799FC;
extern u16 D_8007BF1C;
extern f32 D_8008187C;
extern f32 D_80081880;
extern f32 D_80081884;
extern f32 D_80081888;
extern f32 D_8008188C;
extern f32 D_80081890;
extern f32 D_80081894;
extern f32 D_80081898;
extern s32 D_80079BCC;
extern s32 D_8007C1A0;
extern f32 D_80079BD4[];
extern CameraTrackedObject *D_800CB308[];
extern CameraOverrideSlot D_800CB368[];
extern CameraOverride D_800CB380[];
extern s16 D_800CB470;
extern s16 D_800CB472;
extern s16 D_800CB474;
extern s16 D_800CB476;
extern ControlCollisionState D_800CB2C0;
extern ControlCameraState *D_800CB300;
extern u8 D_8007BF10;
extern f32 D_80081840;
extern f32 D_80081844;
extern f32 D_80081848;
extern f32 D_8008184C;
extern f32 D_800CB304;

typedef struct CharControlEffectDefinition {
    u8 kind;
    u8 index;
    s16 angle;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    u8 arg14;
    u8 arg15;
    u8 arg16;
    u8 arg17;
} CharControlEffectDefinition;

typedef struct CharControlEffectList {
    s32 count;
    CharControlEffectDefinition *entries;
} CharControlEffectList;

typedef struct CharControlParticleDefinition {
    u8 kind;
    u8 index;
    s8 angle;
    s8 angleLow;
    s16 arg4;
    s16 arg6;
    s16 arg8;
    s16 argA;
    s16 argC;
    s16 argE;
} CharControlParticleDefinition;

typedef struct CharControlParticleList {
    s32 count;
    CharControlParticleDefinition *entries;
} CharControlParticleList;

typedef struct CharControlIndex {
    u16 offset;
    u16 value;
} CharControlIndex;

typedef struct CharControlCharacterData {
    u8 pad00[0x1C];
    s16 *positions;
    u8 pad20[0x2D - 0x20];
    u8 count;
    u8 pad2E[2];
    CharControlIndex *indexTable;
} CharControlCharacterData;

typedef struct CharControlParticleSlot {
    u8 kind;
    u8 active;
    u8 index;
    s8 model;
    u8 unk4;
    u8 pad05;
    s16 unk6;
    void *handle;
} CharControlParticleSlot;

typedef struct CharControlLevelDescription {
    u8 pad00[0x0A];
    u8 characterLow;
    u8 characterHigh;
    s8 nextLevel;
    u8 pad0D[0x16 - 0x0D];
    s8 animGroup;
    u8 pad17[0x1A - 0x17];
    s8 camera;
} CharControlLevelDescription;

typedef struct CharControlLevelRequest {
    u8 pad00[0x3C];
    CharControlLevelDescription *description;
} CharControlLevelRequest;

typedef struct CharControlSpawnSetup {
    s16 kind;
    s16 arg02;
    s16 arg04;
    s16 arg06;
    s16 arg08;
    s8 arg0A;
    s8 arg0B;
    void *owner;
} CharControlSpawnSetup;

extern CharControlEffectList D_8007980C[];
extern CharControlParticleList D_8007987C[];
extern CharControlParticleList D_800798DC[];
extern CharControlParticleList D_8007992C[];
extern CharControlParticleList D_8007996C[];
extern CharControlParticleList D_800799AC[];
extern u8 D_8007BEF8;
extern u8 D_8007BEFC;
extern u8 D_8007BF04;
extern f32 D_80081864;
extern f32 D_80081868;
extern f32 D_8008186C;
extern f32 D_80081870;
extern f32 D_80081874;
extern f32 D_80081878;
extern f32 D_800CB2D8;

typedef struct ControlCollisionNormal {
    f32 x;
    f32 y;
    f32 z;
} ControlCollisionNormal;
extern f32 D_800CB2C4;
extern f32 D_800CB2C8;
extern f32 D_800CB2CC;
extern ControlCollisionNormal D_800CB2D0;
extern f32 D_800CB2D4;
extern f32 D_800CB2DC;
extern f32 D_800CB2E0;
extern f32 D_800CB2E4;
extern s32 D_800CB2F8;
extern u8 D_800CB2FC;
extern u8 D_800CB2FD;

void pointListRPY(s32 count, s16 *rotation, f32 *input, f32 *output);
void func_8001EFFC(ControlTransform *transform, ControlPlayer *player, f32 *output);
f32 func_8002A8BC(s32 angle);
f32 func_8002A8C0(s32 angle);
s16 Arctanf(f32 x, f32 y);
f32 sqrtf(f32 value);
void mathOneFloatRPY(ControlTransform *transform, f32 *output);
s32 mathRnd(s32 minimum, s32 maximum);
ControlSpawned *func_8000590C(ControlSpawnPacket *packet, s32 mode);
void func_800031E8(void *handle);
void func_80002FE0(s32 id, f32 x, f32 y, f32 z, s32 priority, void **handle);
void func_8001D690(ControlActor *actor, ControlPlayer *player);
void func_80006EA0(void *handle);
s32 func_8000FAE0(f32 x, f32 y, f32 z);
void func_8001C4C0(ControlActor *actor, ControlPlayerInitState *state, s32 mode);
s32 TrapDanglingJump();
void mainChangeLevel(s32 nextLevel, s32 nextCharacter, s32 nextAnimGroup,
                     s32 nextCamera, s32 fadeOut, s32 flags);
s32 mainGetNextCharacter(void);
void mainSetAnimGroup(s32 group);
u8 frontGetMode(void);
void func_800214AC(void);
void func_8001F09C(ControlPlayer *player, s32 updateRate);
void func_800031C0(void *soundHandle, f32 x, f32 y, f32 z);
void func_8001BBB4(ControlActor *actor, ControlPlayer *player, f32 arg2);
void func_8001C114(s32 slotIndex, f32 x, f32 y, f32 z);
void *func_80053420(s32 index, void *target);
void func_80024ED8();
s32 func_8003A550(void);
s32 func_8000FBD8(s32 segmentIndex, f32 x, f32 y, f32 z);
void func_800475E8(void *cone, s16 angle);
void func_800479D4(void *cone, s16 angle, f32 x, f32 y, s32 length);
void partUpdateTriggers(void *object, s32 updateRate);
void changeLightIntensity(void *light, u8 intensity);
s32 func_8002A204(s16 angle);
void camSetNo();
ControlCameraState *camGetPtr(void);
s32 camGetMode(void);
ControlCameraState *camGetListPtr(void);
ControlTrackState *trackGetTrack(void);
ControlLevelState *levelGetLevel(void);
s32 mainGetNumberOfCameras(void);
s32 func_800299E8(s32 minimum, s32 maximum);
ControlActor **func_8000572C(s32 *start, s32 *end);
s32 func_8005776C(f32 x, f32 y, f32 z, f32 radius, s32 mode, ControlActor **hitActor);
void func_800282C8(void);
void func_8005AD64(void *instance, s32 frame, s32 arg2, f32 value);
void *func_80046EC4(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4,
                    f32 arg5, f32 arg6, f32 arg7, s32 arg8, s32 arg9,
                    s32 argA);
f32 Powerf(f32 value, s32 exponent);
void trackMakePolylist(s32 count, ControlVector3 *start,
                       ControlVector3 *end, f32 *radius, s32 arg4, s32 arg5);
s32 func_80010654(ControlVector3 *start, ControlVector3 *end,
                  ControlVector3 *result, f32 *maximum);
s32 func_80010900(ControlVector3 *start, ControlVector3 *end, f32 radius,
                  s32 actor, void *callback);
void func_8001EC44(s32 arg0, ControlVector3 *arg1, ControlVector3 *arg2,
                   f32 arg3, ControlCollisionPlane *arg4);
u8 levelGetType(void);
u8 *func_80028F54(void);
u32 joyGetButtons(s32 playerIndex);
u32 joyGetPressed(s32 playerIndex);
u32 joyGetReleased(s32 playerIndex);
u32 joyGetStickX(s32 playerIndex);
u32 joyGetAbsX(s32 playerIndex);
u32 joyGetStickY(s32 playerIndex);
u32 joyGetAbsY(s32 playerIndex);
void func_800291D8(s32 arg0);
s32 func_800291FC(void);
void rumbleStart(s32 playerIndex, s32 strength, f32 duration);

f32 func_8001BB90(s32 cameraIndex) {
    return D_800CB380[cameraIndex].blend;
}
/* Bounded plateau: 150/150 instructions, 38 differing words, first mismatch +0x0. */
/* The 0x30 frame is eight non-save bytes short of the 0x38 target; all 18 relocation offsets/types align. */
/* All 119 flags and ten coherent forms are nonexact; resume with a natural eight-byte local-layout mechanism. */
/* PROVENANCE: JFG's corresponding character-control routine supplied the control-flow role;
 * all field offsets, calls, and the body below are reconstructed from Mickey. */
#ifdef NON_MATCHING
void func_8001BBB4(ControlActor *actor, ControlPlayer *player, f32 arg2) {
    ControlTrackState *track;
    ControlLevelState *level;
    void *cameraSource;
    s32 surfaceIndex;
    s32 surfaceValid;
    s32 i;
    s32 mask;
    s16 angle;
    u8 *surface;
    if (player->playerIndex < mainGetNumberOfCameras()) {
        func_8001C114(player->playerIndex, actor->x, actor->y, actor->z);
        cameraSource = func_80053420(0, D_800CB300);
        if (cameraSource == NULL) {
            if ((player->flags1A8 & 1) && (func_8003A550() == 0)) {
                TrapDanglingJump(actor, player, D_800CB300, (s32) arg2);
            } else if (player->controlKeys & 4) {
                func_80024ED8(actor, player, D_800CB300);
            } else if (D_8007BF10 != 0) {
                TrapDanglingJump(D_800CB300, actor, *(s32 *) &arg2);
            } else {
                TrapDanglingJump(D_800CB300, actor, *(s32 *) &arg2);
            }
        }
        track = trackGetTrack();
        if (track != NULL) {
            if (D_800CB300->y < ((f32) track->unk24 - 100.0f)) {
                D_800CB300->y += (((f32) track->unk24 - 100.0f) - D_800CB300->y) * D_80081840;
            }
        }
        level = levelGetLevel();
        surfaceIndex = func_8000FAE0(D_800CB300->x, D_800CB300->y, D_800CB300->z);
        if (surfaceIndex != -1) {
            surfaceValid = 1;
            i = 7;
            surface = (u8 *) level + 0xE;
            do {
                angle = *(s16 *) (surface + 0x112);
                surface -= 2;
                if (surfaceIndex == angle) {
                    surfaceValid = func_8000FBD8(surfaceIndex, D_800CB300->x,
                                                 D_800CB300->y, D_800CB300->z);
                    break;
                } else {
                    i--;
                    if (i == 0) {
                        break;
                    }
                }
            } while (1);
            if (surfaceValid != 0) {
                D_800CB300->unk3E = (s16) surfaceIndex;
            }
        }
        if (*(void **)((u8 *) actor + 0x50) != NULL && level->unk0E3 == 0) {
            *(f32 *) ((u8 *) *(void **)((u8 *) actor + 0x50) + 0x10) = D_800CB300->x - actor->x;
            *(f32 *) ((u8 *) *(void **)((u8 *) actor + 0x50) + 0x14) = D_800CB300->y - actor->y;
            *(f32 *) ((u8 *) *(void **)((u8 *) actor + 0x50) + 0x18) = D_800CB300->z - actor->z;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/charControl/func_8001BBB4.s")
#endif
/* PROVENANCE: JFG's corresponding character-control routine supplied the control-flow role; fields and body are reconstructed from Mickey. */
void func_8001BE0C(ControlActor *actor, ControlPlayer *player) {
    s32 i;

    D_800CB300 = camGetPtr();
    D_800CB300->unk4 = 0;
    D_800CB300->unk2 = 0;
    D_800CB300->unk0 = 0;
    D_800CB300->unk24 = 600.0f;
    D_800CB300->unk28 = 150.0f;
    D_800CB300->unk3D = 0;
    D_800CB300->unk44 = 0xFF;
    D_800CB300->unk45 = 0xFF;
    D_800CB300->unk46 = 0xFF;
    D_800CB300->unk47 = 0xFF;
    D_800CB300->unk40 = 0.0f;
    D_800CB300->y = actor->y + 80.0f;
    D_800CB300->unk18 = D_800CB300->x;
    D_800CB300->unk1C = D_800CB300->y;
    D_800CB300->unk20 = D_800CB300->z;
    D_800CB300->unk49 = 1;
    if (camGetMode() >= 2) {
        D_800CB300->unk24 = 400.0f;
    }
    if ((player->playerIndex >= 0) && (player->playerIndex < 4)) {
        D_800CB368[player->playerIndex].object = 0;
        D_800CB368[player->playerIndex].unk08 = 0.0f;
        D_800CB368[player->playerIndex].unk0C = 0.0f;
        D_800CB368[player->playerIndex].unk10 = 1.0f;
        D_800CB368[player->playerIndex].unk14 = 1.0f;
        D_800CB368[player->playerIndex].unk18 = 0.0f;
        D_800CB368[player->playerIndex].unk1C = 0.0f;
        D_800CB368[player->playerIndex].unk20 = 0.0f;
        D_800CB368[player->playerIndex].unk24 = 1.0f;
        D_800CB368[player->playerIndex].unk28 = 0.0f;
    }
    player->unk16F = 0;
    i = 0;
    do {
        func_8001BBB4(actor, player, 1.0f);
        i++;
    } while (i != 8);
    player->unk16F = 1;
}
void func_8001C054(CameraTrackedObject *value) {
    if (D_80079BCC < 24) {
        D_800CB308[D_80079BCC] = value;
        D_80079BCC++;
    }
}
void func_8001C088(CameraTrackedObject *value) {
    s32 index;
    s32 foundIndex;

    index = 0;
    foundIndex = -1;
    if (D_80079BCC > 0) {
        do {
            if (D_800CB308[index] == value) {
                foundIndex = index;
            }
            index++;
        } while (index < D_80079BCC);
    }
    if (foundIndex != -1) {
        for (index = foundIndex; index < D_80079BCC - 1; index++) {
            D_800CB308[index] = D_800CB308[index + 1];
        }
        D_80079BCC--;
    }
}
void func_8001C114(s32 slotIndex, f32 x, f32 y, f32 z) {
    CameraOverrideSlot *slot;
    CameraTrackedObject *object;
    CameraTrackedObject *searchObject;
    CameraBounds *bounds;
    f32 deltaX;
    f32 deltaZ;
    f32 trackedRadius;
    f32 radius;

    if (slotIndex >= 0 && slotIndex < 4) {
        slot = &D_800CB368[slotIndex];
        object = slot->object;
        if (object != 0) {
            bounds = slot->bounds;
            if (bounds != 0) {
                trackedRadius = bounds->trackedRadius;
                deltaX = object->x - x;
                deltaZ = object->z - z;
                trackedRadius *= trackedRadius;
                if (trackedRadius <
                    ((deltaX * deltaX) + (deltaZ * deltaZ))) {
                    slot->object = 0;
                    object = 0;
                } else if ((bounds->flags & 0x8000) &&
                           ((y < bounds->trackedUpper) || (bounds->trackedLower < y))) {
                    slot->object = 0;
                    object = 0;
                }
            }
        }
        if (object == 0) {
            if (D_80079BCC > 0) {
                CameraTrackedObject **current;
                s32 index;

                index = 0, current = D_800CB308;
                do {
                    searchObject = *current;
                    bounds = searchObject->bounds;
                    radius = bounds->radius;
                    deltaX = searchObject->x - x;
                    deltaZ = searchObject->z - z;
                    radius *= radius;
                    if (((deltaX * deltaX) + (deltaZ * deltaZ)) < radius) {
                        slot->object = searchObject;
                        slot->bounds = bounds;
                        if ((bounds->flags & 0x4000) &&
                            ((y < bounds->upper) || (bounds->lower < y))) {
                            slot->object = 0;
                        }
                    }
                    index++;
                    current++;
                } while (index < D_80079BCC);
            }
        }
    }
}
void func_8001C2C4(void) {
}
void func_8001C2CC(void) {
}
void func_8001C2D4(u8 *start, u8 *end) {
    u8 *current = start;

    if (start < end) {
        do {
            *current++ = 0;
        } while (current != end);
    }
}
/*
 * PROVENANCE -- JFG's charControl symbols and assembly supplied the
 * controlPlayerReInit name/role. This Mickey-specific save, clear, initialize,
 * and restore body is independently reconstructed from Mickey's code.
 */
void controlPlayerReInit(ControlActor *actor, f32 x, f32 y, f32 z, s16 arg4, s16 arg5, s16 arg6) {
    ControlPlayer *player;
    s32 saved192;
    s32 saved1A8;
    s32 saved3BA;
    s32 saved45C;
    s32 saved45D;
    ControlPlayerInitState stateStorage;
    ControlPlayerInitState *state;

    player = actor->player;
    state = &stateStorage;
    state->playerIndex = player->playerIndex;
    state->unk11 = player->unk1;
    state->arg4 = arg4;
    state->arg5 = arg5;
    state->arg6 = arg6;
    saved192 = player->unk192;
    saved1A8 = player->flags1A8;
    saved3BA = player->unk3BA;
    saved45C = player->unk45C;
    saved45D = player->unk45D;
    actor->x = x;
    actor->y = y;
    actor->flags &= ~0x400;
    actor->z = z;
    actor->velocityX = 0.0f;
    actor->velocityY = 0.0f;
    actor->velocityZ = 0.0f;
    actor->positionTag = func_8000FAE0(actor->x, actor->y, actor->z);
    actor->alpha = 0xFF;
    actor->unk80 = 0;
    if (player->unkD0 != 0) {
        func_80006EA0(player->unkD0);
    }
    if (player->unkD4 != 0) {
        func_80006EA0(player->unkD4);
    }
    if (player->unkD8 != 0) {
        func_80006EA0(player->unkD8);
    }
    func_8001C2D4((u8 *) player, (u8 *) player + 0xA4);
    func_8001C2D4((u8 *) player + 0xC8, (u8 *) player + 0x134);
    func_8001C2D4((u8 *) player + 0x144, (u8 *) player + 0x19A);
    func_8001C2D4((u8 *) player + 0x1A4, (u8 *) player + 0x34C);
    func_8001C2D4((u8 *) player + 0x3E4, (u8 *) player + 0x400);
    func_8001C4C0(actor, state, 0);
    player->unk11C[0] = -2.0f;
    player->unk11C[1] = -2.0f;
    player->unk11C[2] = -2.0f;
    player->unk11C[3] = -2.0f;
    player->unk192 = saved192;
    player->flags1A8 = saved1A8;
    player->unk3BA = saved3BA;
    player->unk45C = saved45C;
    player->unk45D = saved45D;
}
/* Workbench verdict: structure-mismatch, 390 differing words, first mismatch +0x0. */
/* Candidate shape: 401 instructions/frame -0xC8 versus target 403/-0xA8; relocations remain positionally different. */
/* Remaining structural gap: callee-saved loop state/stack homes and fixed-count state initialization; not shape-exact. */
/* PROVENANCE: JFG's corresponding character-control initialization role supplied the control-flow lead; fields and body are reconstructed from Mickey. */
#ifdef NON_MATCHING
void func_8001C4C0(ControlActor *actor, ControlPlayerInitState *state, s32 mode) {
    ControlPlayer *player;
    CharControlEffectList *effectList;
    CharControlEffectDefinition *effect;
    CharControlParticleList *particleList;
    CharControlParticleDefinition *particle;
    CharControlCharacterData *characterData;
    CharControlParticleSlot *slot;
    ControlSpawnPacket packet;
    void *savedActor;
    f32 *output;
    s16 *position;
    void **effectOwner;
    void *stateCursor;
    s32 effectIndex;
    s32 effectCount;
    s32 effectSlot;
    s32 pointIndex;
    s32 particleCount;
    register s32 remaining;
    register s32 particleSlotCount;
    s32 packetIndex;
    s8 playerIndex;

    player = actor->player;
    player->unk1B8 = 0x2000;
    player->playerIndex = *((u8 *) state + 0x10);
    player->unk10 = 0.0f;
    player->unk1 = *((u8 *) state + 0x11);
    actor->rotationX = state->arg4;
    actor->rotationY = state->arg5;
    actor->rotationZ = state->arg6;
    player->unkF0 = actor->rotationX;
    player->unkF2 = actor->rotationY;
    player->unkFE = 0;
    player->unkF4 = actor->rotationZ;
    player->unkDC = (s16) (0x8000 - actor->rotationX);
    func_8005AD64(actor, 0, -1, 0.0f);
    player->unk50 = 1.0f;
    player->unk54 = 1.0f;

    output = &player->unk2C0[0];
    if (D_8007BF10 != 0) {
        player->unk2BC = 4;
        player->unk2B8 = (ControlGravityVector *) &D_800799AC;
    } else {
        player->unk2BC = 4;
        if (D_8007BF1C & 8) {
            player->unk2B8 = (ControlGravityVector *) &D_8007996C;
        } else {
            player->unk2B8 = (ControlGravityVector *) &D_8007992C;
        }
    }
    player->unk33C = 0;
    player->unk340 = 0;
    pointIndex = 0;
    if (player->unk2BC > 0) {
        effectSlot = 0;
        do {
            effectSlot++;
            output += 3;
            output[-3] = *((f32 *) ((u8 *) player->unk2B8 + pointIndex));
            output[-2] = *((f32 *) ((u8 *) player->unk2B8 + pointIndex + 4));
            output[-1] = *((f32 *) ((u8 *) player->unk2B8 + pointIndex + 8));
            pointIndex += 0x10;
        } while (effectSlot < player->unk2BC);
    }
    func_8001EFFC(actor, player, &player->unk2F0);

    effectIndex = (s32) player->unk1;
    if ((player->unk1 < 0) || (player->unk1 >= 10)) {
        effectIndex = 0;
    }
    if (player->playerIndex < (D_8007BEF8 - D_8007BEFC)) {
        effectList = &D_8007980C[effectIndex];
        effect = effectList->entries;
        if (effect != 0) {
            effectCount = effectList->count;
            effectSlot = 0;
            effectOwner = (void **) player;
            if (effectCount > 0) {
                do {
                    if (effectOwner[0x134 / 4] == 0) {
                        effectOwner[0x134 / 4] =
                            (void *) TrapDanglingJump(
                                actor, effect->kind, effect->angle, effect->index,
                                effect->x, effect->y, effect->z, effect->w,
                                effect->arg14, effect->arg15, effect->arg16,
                                effect->arg17);
                    }
                    effectSlot++;
                    effectOwner++;
                    effect++;
                } while (effectSlot != effectCount);
            }
        }
    }

    characterData = (CharControlCharacterData *)
        *(*(actor->unk68 + actor->unk3A));
    particleCount = effectIndex * 8;
    particleSlotCount = 0;
    if (levelGetType() == 3) {
        particleList = (CharControlParticleList *)
            ((u8 *) D_8007987C + particleCount);
    } else if (D_8007BF04 != 0) {
        particleList = (CharControlParticleList *)
            ((u8 *) D_800798DC + particleCount);
    } else {
        particleList = (CharControlParticleList *)
            ((u8 *) D_8007987C + particleCount);
    }
    particleCount = particleList->count;
    particle = particleList->entries;
    slot = (CharControlParticleSlot *) player->particles;
    remaining = particleCount - 1;
    if (particleCount != 0) {
        do {
            if (slot->handle == 0) {
                if (particle->index < characterData->count) {
                    slot->kind = particle->kind;
                    slot->index = particle->index;
                    position = (s16 *) ((u8 *) characterData->positions +
                        (characterData->indexTable[particle->index].offset * 10));
                    slot->model = (s8)
                        characterData->indexTable[particle->index].value;
                    slot->handle = func_80046EC4(
                        position[0], position[1], position[2],
                        (s16) (particle->angle << 8),
                        (s16) (particle->angleLow << 8),
                        (f32) particle->arg4, (f32) particle->arg6,
                        (f32) particle->arg8, particle->argA, particle->argC,
                        particle->argE);
                }
            }
            slot->active = 0;
            slot->unk4 = 0;
            slot->unk6 = 0;
            particle++;
            slot++;
            particleSlotCount++;
            remaining--;
        } while (remaining != 0);
    }
    if (particleSlotCount < 4) {
        do {
            particleSlotCount++;
            slot->handle = 0;
            slot++;
        } while (particleSlotCount < 4);
    }

    playerIndex = player->playerIndex;
    player->unk38 = actor->x;
    player->unk3C = actor->y;
    player->unk40 = actor->z;
    player->unk44 = actor->x;
    player->unk48 = actor->y;
    player->unk190 = 0xFF;
    player->unk18D = 0;
    player->unk338 = 0;
    player->unk348 = 0;
    player->unk456 = 0;
    player->unk387 = 0xFF;
    player->unk388 = 0;
    player->unk183 = 0x80;
    player->unk184 = 0;
    player->unk185 = 0;
    player->unk186 = 0;
    player->unk187 = 0;
    player->unk188 = 0.0f;
    player->unk4C = actor->z;
    if (playerIndex != -1) {
        camSetNo(playerIndex, 0, &D_800CB300);
        func_8001BE0C(actor, player);
    }
    player->unk16C = 0;
    player->unk2 = 0;
    player->unk3 = 0;
    player->unk16D = 0;
    if (player->actions == 0) {
        player->unk19A = 0xFF;
        player->unk19B = 0;
        player->unk19C = 0;
        player->actions = 0;
    }
    player->unk1A4 = 0;
    player->unk1A5 = 0;
    player->unk1A6 = 0;
    player->unk172 = 0;
    player->unk173 = 0;
    stateCursor = (u8 *) player;
    player->unk174 = 1.0f;
    player->unk178 = 2.0f;
    player->unk17C = 1.0f;
    particleSlotCount = 0;
    do {
        packetIndex = particleSlotCount * 8;
        particleSlotCount++;
        stateCursor = (u8 *) stateCursor + 1;
        *((u8 *) stateCursor + 0x12B) = 0;
        *((u8 *) stateCursor + 0x12F) = (u8) packetIndex;
    } while (particleSlotCount < 4);
    player->unk3EC = 0.0f;
    player->unk3F0 = D_80081844;
    player->unk43C = actor->rotationX;
    player->unk43E = actor->rotationY;
    player->unk440 = actor->rotationZ;
    player->unk444 = actor->unk8;
    player->unk448 = actor->x;
    player->unk44C = actor->y;
    player->unk3BA = 0xFF;
    player->unk450 = actor->z;
    if (levelGetType() == 3) {
        packetIndex = 0;
        if (mode != 0) {
            savedActor = actor;
            packet.kind = 0x124;
            packet.x = 0;
            packet.y = 0;
            packet.z = 0;
            do {
                packet.unkA = packetIndex;
                func_8000590C(&packet, 1);
                packetIndex++;
            } while (packetIndex != 3);
        }
    }
    if ((D_8007BF1C & 2) && (*func_80028F54() != 1)) {
        player->unk192 = 0xA;
        return;
    }
    player->unk192 = 0;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/charControl/func_8001C4C0.s")
#endif
void func_8001CB0C(ControlTransform *transform, ControlPlayer *player) {
    player->unk2BC = 1;
    if (D_8007BF1C & 8) {
        player->unk2B8 = &D_800799FC;
    } else {
        player->unk2B8 = &D_800799EC;
    }
    player->unk33C = 0;
    player->unk340 = 0;
    player->unk2C0[0] = player->unk2B8->x;
    player->unk2C0[1] = player->unk2B8->y;
    player->unk2C0[2] = player->unk2B8->z;
    func_8001EFFC(transform, player, &player->unk2C0[12]);
}
/* PROVENANCE: JFG's public charControl.c identifies the corresponding
 * controlSquashCheckPost-adjacent character-control routine, but publishes
 * assembly only; this body is reconstructed from Mickey's fields, calls,
 * and branch conditions. */
#ifdef NON_MATCHING
/* Workbench verdict: structure-mismatch, 138 differing words, first mismatch +0x0. */
/* Candidate shape: exact-sized at 455 instructions, frame -0xB8 versus target -0x80. */
/* Remaining gap: 0x38-byte frame/home excess and the resulting FP register-web cascade. */
void func_8001CB84(ControlActor *actor, s32 updateRate) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f14;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 var_f12;
    s16 particleState;
    s16 bounceState;
    s16 temp_v0_5;
    s16 temp_v0_6;
    s16 temp_v0_8;
    s16 temp_v0_9;
    s8 temp_a0;
    s8 temp_v0_2;
    s8 temp_v0_7;
    u8 highCharacter;
    s32 character;
    u8 temp_v0_12;
    s32 temp_v1_3;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a0_4;
    ControlPlayer *player;
    ControlParticleEffect *effect;
    ControlParticleState *state;
    CharControlLevelDescription *sp70;
    CharControlSpawnSetup packetD0;
    CharControlSpawnSetup packetD8;
    s32 sp44;
    s32 sp3C;
    f32 sp7C;
    f32 sp38;

    player = actor->player;
    player->unk3 = player->unk2;
    if (player->unkC8 != NULL) {
        sp70 = ((CharControlLevelRequest *) player->unkC8)->description;
        highCharacter = sp70->characterHigh;
        character = sp70->characterLow;
        if (highCharacter != 0xFF) {
            character |= highCharacter << 8;
        }
        sp44 = character;
        sp3C = mainGetNextCharacter();
        mainChangeLevel(sp44, sp3C,
                        sp70->nextLevel, frontGetMode(),
                        sp70->camera, 0);
        temp_a0 = sp70->animGroup;
        if (temp_a0 != -1) {
            mainSetAnimGroup(temp_a0);
        }
        func_80006EA0(player->unkC8);
        player->unkC8 = NULL;
    }
    if (player->unk1A6 != 0) {
        if (player->unk1A5 == 0) {
            player->unk1A5 = 1;
            if (player->unk1A4 == 1) {
                camSetNo(player->playerIndex);
                func_800214AC();
            }
        }
        player->unk1A6 = (s16) (player->unk1A6 - updateRate);
        if (player->unk1A6 <= 0) {
            player->unk1A6 = 0;
            if (player->unk1A4 == 1) {
                camSetNo(player->playerIndex);
                func_800214AC();
            }
        }
    }
    temp_v0_2 = player->unk18D;
    if (temp_v0_2 != 0) {
        player->unk18D = (s8) (temp_v0_2 - updateRate);
        if (player->unk18D <= 0) {
            effect = player->unk338;
            player->unk18D = 0;
            player->unk54 = 1.0f;
            if ((effect != NULL) && (effect->unk44 == 0x52)) {
                state = effect->state;
                *(s16 *) state &= 0xFFFD;
                effect = player->unk338;
                effect->unk20 = (f32) effect->state->unk18;
                player->unk338 = NULL;
            }
        }
    }
    if (player->unk50 != player->unk54) {
        func_8001F09C(player, updateRate);
    }
    actor->unk48->unk54 = 0.0f;
    particleState = player->unk158;
    if (particleState != 0) {
        if (particleState & 0x8000) {
            temp_a0_2 = (s32) player->unkB4;
            if (temp_a0_2 != 0) {
                func_800031E8((void *) temp_a0_2);
            }
            func_80002FE0(0x21, actor->x, actor->y, actor->z, 4,
                          (void **) &player->unkB4);
            player->unk158 = (s16) (player->unk158 & 0x7FFF);
        }
        temp_f2 = player->unk150;
        temp_f0 = (f32) updateRate;
        player->unk154 = player->unk154 +
                         ((temp_f2 * temp_f0) -
                          (0.5f * D_800CB304 * temp_f0 * temp_f0));
        temp_f14 = player->unk154;
        player->unk150 = temp_f2 - (D_800CB304 * temp_f0);
        if (temp_f14 < 0.0f) {
            player->unk154 = -temp_f14;
            player->unk158 = (s16) (player->unk158 - 1);
            player->unk150 = -player->unk150 * 0.5f;
        }
        bounceState = player->unk158;
        if ((bounceState >= 2) ||
            ((bounceState == 1) && (player->unk150 > 0.0f))) {
            player->unk160 = (s16) (player->unk160 + player->unk15A * updateRate);
            player->unk164 = (s16) (player->unk164 + player->unk15E * updateRate);
            player->unk162 = (s16) (player->unk162 + player->unk15C * updateRate);
        } else if (bounceState == 1) {
            temp_f0_2 = Powerf(D_80081848, updateRate);
            sp7C = temp_f0_2;
            player->unk160 = dAngle(player->unk160, 0, temp_f0_2);
            player->unk164 = dAngle(player->unk164, 0, sp7C);
            player->unk162 = dAngle(player->unk162, 0, sp7C);
        } else if (bounceState == 0) {
            player->unk160 = 0;
            player->unk164 = 0;
            player->unk162 = 0;
            player->unk154 = 0.0f;
            player->unk150 = 0.0f;
        }
        sp38 = func_8002A8BC(player->unk162);
        temp_f2_2 = func_8002A8BC(player->unk164) * sp38;
        if (temp_f2_2 < 0.0f) {
            var_f12 = 0.0f;
        } else {
            var_f12 = temp_f2_2 * temp_f2_2;
        }
        player->unk14C = 30.0f - (30.0f * var_f12);
        actor->unk48->unk54 = player->unk154;
        player->unk185 = 0;
        player->unk188 = 0.0f;
    }
    if (D_8007BF10 != 0) {
        if (player->unk191 == 0) {
            TrapDanglingJump(actor, updateRate);
        }
        TrapDanglingJump(actor, updateRate);
    } else if (player->flags1A8 & 1) {
        TrapDanglingJump(actor, player, updateRate);
        if (player->unk191 == 0) {
            TrapDanglingJump(actor, updateRate);
        }
    } else {
        if (player->unk191 == 0) {
            temp_v0_7 = player->unk1;
            if ((temp_v0_7 == 0) || (temp_v0_7 == 1) ||
                (temp_v0_7 == 2) || (temp_v0_7 == 3)) {
                TrapDanglingJump(actor, updateRate);
            } else {
                TrapDanglingJump(actor, updateRate);
            }
        }
        TrapDanglingJump(actor, updateRate);
    }
    temp_v0_8 = player->unk168;
    if ((temp_v0_8 != 0) && (player->unk3FA == 0)) {
        player->unk168 = (s16) (temp_v0_8 - updateRate);
        if (player->unk168 < 0) {
            player->unk168 = 0;
        }
        if (player->unk168 & 8) {
            player->unk190 = 0x40;
        } else {
            player->unk190 = 0xFF;
        }
    }
    temp_v0_9 = player->unk16A;
    if (temp_v0_9 != 0) {
        player->unk16A = (s16) (temp_v0_9 - updateRate);
        if (player->unk16A <= 0) {
            player->unk16A = 0;
        } else if (player->unkD0 == NULL) {
            packetD0.kind = 0xB8;
            packetD0.arg04 = 0;
            packetD0.arg06 = 0xE;
            packetD0.arg08 = 7;
            packetD0.arg0A = 0x14;
            packetD0.arg0B = 0x14;
            packetD0.owner = actor;
            player->unkD0 = func_8000590C(
                (ControlSpawnPacket *) &packetD0, 1);
            if (player->unkD0 != NULL) {
                ((ControlSpawned *) player->unkD0)->unk3C = 0;
            }
        }
    }
    if (!(player->flags1A8 & 1)) {
        if ((player->unkD8 == NULL) &&
            (TrapDanglingJump((void *) player->playerIndex) != 0)) {
            packetD8.kind = 0x14C;
            packetD8.arg02 = 0x10;
            packetD8.arg0B = 0;
            packetD8.arg04 = 0;
            packetD8.arg06 = 0xE;
            packetD8.arg08 = 7;
            packetD8.owner = actor;
            player->unkD8 = func_8000590C(
                (ControlSpawnPacket *) &packetD8, 1);
            if (player->unkD8 != NULL) {
                ((ControlSpawned *) player->unkD8)->unk3C = 0;
            }
        } else if ((player->unkD8 != NULL) &&
                   (TrapDanglingJump((void *) player->playerIndex) == 0)) {
            func_80006EA0(player->unkD8);
        }
    }
    controlDisableJoypad(player, 0);
    if (player->unk3FA != 0) {
        temp_v0_12 = player->unk190;
        if ((s32) temp_v0_12 > 0) {
            temp_v1_3 = temp_v0_12 - (updateRate * 4);
            if (temp_v1_3 <= 0) {
                temp_a0_3 = (s32) player->unkAC;
                if (temp_a0_3 != 0) {
                    func_800031E8((void *) temp_a0_3);
                    actor->unk80 = 0;
                }
                player->unk191 = 1;
                player->unk190 = 0;
                actor->x = player->unk44;
                actor->y = player->unk48 + D_8008184C;
                actor->z = player->unk4C;
            } else {
                player->unk190 = (u8) temp_v1_3;
            }
        }
    }
    temp_a0_4 = (s32) player->unkA4;
    if (temp_a0_4 != 0) {
        func_800031C0((void *) temp_a0_4, actor->x, actor->y, actor->z);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/charControl/func_8001CB84.s")
#endif
/*
 * Workbench: structure-mismatch, best 96/95 instructions, 3 masked words, first +0xE0.
 * Levers tried: commutative base add (best), volatile/old-style/comma/result forms, and flags/context.
 * Remains: candidate CSE hoists the global base address before the camera-count call; target keeps it split.
 */
#ifdef NON_MATCHING
void func_8001D2A0(ControlActor *actor, s32 arg1) {
    ControlPlayer *player;
    s32 cameraIndex;

    player = actor->player;
    player->unk43C = actor->rotationX;
    player->unk43E = actor->rotationY;
    player->unk440 = actor->rotationZ;
    player->unk444 = actor->unk8;
    player->unk448 = actor->x;
    player->unk44C = actor->y;
    player->unk450 = actor->z;
    if (player->unk158 != 0) {
        player->unk43C += player->unk160;
        player->unk43E += player->unk164;
        player->unk440 += player->unk162;
        player->unk44C += player->unk154 + player->unk14C;
    }
    if (!(player->flags1A8 & 1)) {
        TrapDanglingJump(actor, player, arg1);
    }
    if (player->unkD4 != 0) {
        TrapDanglingJump(player->unkD4, arg1);
    }
    D_800CB300 = camGetListPtr();
    cameraIndex = mainGetNumberOfCameras() - 1;
    if (player->playerIndex < cameraIndex) {
        cameraIndex = player->playerIndex;
    }
    D_800CB300 = (ControlCameraState *)
        ((u8 *) D_800CB300 + (cameraIndex * sizeof(ControlCameraState)));
    camSetNo(player->playerIndex, cameraIndex, &D_800CB300);
    if ((player->unk190 != 0) || (player->unk3FA == 0)) {
        func_8001BBB4(actor, player, (f32) arg1);
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/charControl/func_8001D2A0.s")
#endif
void func_8001D41C(ControlActor *actor, ControlPlayer *player, s32 updateRate) {
    ControlPlayerActions *actions;
    ControlPlayerAction action;
    s32 effectIndex;

    if (player->unk19C > 0) {
        player->unk19C -= updateRate;
        if (player->unk19C <= 0) {
            player->unk19C = 0;
            if ((D_8007C1A0 == 1) && !(player->flags1A8 & 1)) {
                effectIndex = player->unk19A;
                if ((effectIndex >= 2) && (effectIndex < 10)) {
                    TrapDanglingJump(effectIndex + 30);
                }
            }
        }
    }

    actions = player->actions;
    if ((actions == 0) || (player->unk19C != 0) ||
        (TrapDanglingJump(player->unkD4) != 0)) {
        if (player->controlDkeys & 0x2000) {
            if ((player->unk1 >= 0) && (player->unk1 < 10)) {
                if (D_8007BF1C & 4) {
                    if (player->unkA4 != 0) {
                        func_800031E8(player->unkA4);
                    }
                    func_80002FE0(
                        D_80079A20[player->unk1][func_800299E8(0, 3)],
                        actor->x, actor->y, actor->z, 4, &player->unkA4);
                    return;
                }
                if (player->unkA8 != 0) {
                    func_800031E8(player->unkA8);
                }
                func_80002FE0(D_80079A0C[player->unk1], actor->x, actor->y,
                              actor->z, 4, &player->unkA8);
            }
        }
    } else if (player->controlDkeys & 0x2000) {
        action = actions->positive;
        if ((action != 0) && (player->controlYjoy >= 65)) {
            action(actor);
            return;
        }
        action = actions->negative;
        if ((action != 0) && (player->controlYjoy < -64)) {
            action(actor);
            return;
        }
        action = actions->fallback;
        if (action != 0) {
            action(actor);
        }
    }
}
void controlFrozen(ControlActor *actor, ControlPlayer *player) {
    if (func_800291FC() == 1) {
        func_800291D8(10);
    }
    if (joyGetPressed(player->playerIndex) & 0xF00F) {
        func_8001D690(actor, player);
    }
}
/*
 * PROVENANCE -- JFG's charControl symbols and assembly supplied the
 * controlRestartPlayer name/role. This Mickey-specific respawn-point search
 * and reinitialization body is independently reconstructed from Mickey's code.
 */
void func_8001D690(ControlActor *actor, ControlPlayer *player) {
    s32 start;
    s32 end;
    ControlActor **objects;
    ControlActor *current;
    s32 maxIndex;
    s32 playerCount;
    f32 radius;
    s32 mode;
    s32 candidateIndex;
    ControlActor *hitActor;
    ControlActor *candidates[8];
    s32 count;
    s32 hit;

    playerCount = func_800291FC();
    if (playerCount >= 2) {
        objects = func_8000572C(&start, &end);
        count = 0;
        radius = 32.0f;
        mode = 0;
        if (start < end) {
            do {
                current = objects[start++];
                if (current->kind == 5) {
                    hit = func_8005776C(current->x, current->y, current->z,
                                       radius, mode, &hitActor);
                    if ((hit == 0) || ((hit == 1) && (actor == hitActor))) {
                        candidates[count++] = current;
                    }
                }
            } while (start < end);
        }
        if (count == 0) {
            current = actor;
        } else if (count == 1) {
            current = candidates[0];
        } else {
            maxIndex = count - 1;
            candidateIndex = mathRnd(0, maxIndex);
            current = candidates[candidateIndex];
        }
        controlPlayerReInit(actor, current->x, current->y, current->z,
                            current->rotationX, current->rotationY,
                            current->rotationZ);
    } else {
        func_800282C8();
    }
}
/* PROVENANCE -- adapted from JFG's src/charControl.c dAngle. */
s16 dAngle(s16 arg0, s16 arg1, f32 arg2) {
    s32 temp_t1;
    s32 var_v1;

    var_v1 = (arg1 - arg0) & 0xFFFF;
    temp_t1 = (arg0 - arg1) & 0xFFFF;
    if (temp_t1 < var_v1) {
        var_v1 = -temp_t1;
    }
    return (s16) (arg0 + (s32) ((f32) var_v1 * arg2));
}
/* PLATEAU-HANDOFF
 * Fresh configured full-TU C is exact-sized at 36 words/0x90, frameless,
 * with 7/36 positional words equal and first mismatch +0x4; no relocations.
 * Thirteen bounded natural builds plus four fidelity-clean trace diagnostics
 * found no strict gain. Proc 21 shows the second-base FP web occupying the
 * target constant color, cascading the register map and final load/multiply
 * schedule. Preserve V0 and fallback; resume only with a source-authentic
 * pool-to-temp/web-formation mechanism.
 */
/* PROVENANCE -- adapted from JFG's charControl controlMakeV implementation. */
#ifdef NON_MATCHING
f32 func_8001D880(f32 arg0, f32 arg1, f32 *table, f32 divisor) {
    f32 base;
    f32 value;
    s32 index;
    f32 *entry;

    arg1 *= 10.0f;
    index = (s32) arg1;
    entry = table + index;
    base = entry[0];
    value = ((entry[1] - base) * (arg1 - (f32) index)) + base;
    arg0 *= 10.0f;
    index = (s32) arg0;
    entry = table + index;
    base = entry[0];
    return (value - (base + ((entry[1] - base) * (arg0 - (f32) index)))) / divisor;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/charControl/func_8001D880.s")
#endif
/* PROVENANCE -- adapted from JFG's src/charControl.c controlFSUvels. */
void controlFSUvels(s16 *rotation, ControlPlayer *player) {
    s16 sp18[3];

    sp18[0] = rotation[0];
    sp18[1] = rotation[1];
    sp18[2] = 0;
    pointListRPY(3, sp18, D_80079BD4, player->unk14);
}
typedef struct ControlFlameSlot {
    u8 state;
    u8 mode;
    u8 pad02[2];
    u8 intensity;
    u8 pad05;
    s16 phase;
    void *particle;
} ControlFlameSlot;

typedef struct ControlFlameParticle {
    u8 pad00[0x18];
    f32 value18;
    f32 value1C;
    s16 value20;
    s16 value22;
    s16 value24;
} ControlFlameParticle;

/* Workbench verdict: structure-mismatch, 176 differing words, first mismatch +0x0. */
/* Candidate: 221/220 instructions with a -0x70 frame versus target -0x60; three instruction/stack and state-machine residuals remain. */
/* Shape status: four-slot state machine, signed phase/intensity arithmetic, and cone calls are reconstructed, but it is not shape-exact. */
/* PROVENANCE: JFG's public controlUpdateJetFlames role and Mickey's m2c/assembly establish
 * the state-machine order; no external body is copied into this reconstruction. */
#ifdef NON_MATCHING
void func_8001D960(ControlActor *actor, ControlPlayer *player, s32 arg2, s32 arg3,
                   s32 arg4) {
    s32 sp5C;
    f32 var_f20;
    f32 var_f22;
    s16 temp_v1;
    s32 var_s3;
    s32 var_s6;
    s32 *temp_v0;
    s32 temp_a0;
    s32 temp_s2;
    s32 temp_t7;
    s32 temp_t9;
    s32 var_s5;
    s32 var_v0;
    s32 var_s0;
    u8 temp_v0_2;
    ControlFlameSlot *var_s1;
    void *temp_s7;

    var_v0 = 0;
    var_s5 = 1;
    var_s1 = (ControlFlameSlot *) ((u8 *) player + 0x34C);
    do {
        temp_s7 = var_s1->particle;
        sp5C = var_v0;
        if (temp_s7 != NULL) {
            temp_v1 = *(s16 *) ((u8 *) temp_s7 + 0x24);
            var_s0 = var_s1->intensity;
            var_s3 = var_s1->phase;
            var_f20 = *(f32 *) ((u8 *) temp_s7 + 0x18);
            var_f22 = *(f32 *) ((u8 *) temp_s7 + 0x1C);
            var_s6 = temp_v1;
            if (var_s1->state == 2) {
                s32 updateRate;

                updateRate = arg4;
                temp_t7 = updateRate << 5;
                if (var_s1->mode == 0) {
                    var_s0 -= temp_t7;
                    if (var_s0 < 0) {
                        var_s0 = 0;
                    }
                    var_s3 = actor->rotationX;
                    if (player->unk186 & var_s5) {
                        var_s1->mode = 2;
                    }
                } else {
                    var_s0 += temp_t7;
                    if (var_s0 >= 0x100) {
                        var_s0 = 0xFF;
                    }
                    var_s3 = actor->rotationX;
                    if (!(player->unk186 & var_s5)) {
                        var_s1->mode = 0;
                    }
                }
                var_f20 *= (f32) var_s0 / 255.0f;
                var_f22 *= (f32) var_s0 / 255.0f;
                temp_v0 = actor->unk70;
                if (temp_v0 != NULL) {
                    temp_a0 = *temp_v0;
                    if (temp_a0 != 0) {
                        changeLightIntensity((void *) temp_a0, var_s0);
                    }
                }
            } else {
                temp_v0_2 = var_s1->mode;
                switch (temp_v0_2) {
                case 0:
                    var_s0 = 0;
                    if (player->unk186 & var_s5) {
                        temp_s2 = actor->unk80;
                        actor->unk80 = arg3;
                        partUpdateTriggers(actor, 2);
                        actor->unk80 = temp_s2;
                        var_s1->mode = 1;
                    }
                    break;
                case 1: {
                    s32 updateRate;

                    updateRate = arg4;
                    var_s0 += updateRate << 5;
                    var_s6 = (s32) (temp_v1 * var_s0) >> 7;
                    if (var_s0 >= 0x100) {
                        var_s0 = 0xFF;
                        if (player->unk186 & var_s5) {
                            var_s1->mode = 2;
                        } else {
                            var_s1->mode = 3;
                        }
                    }
                    break;
                }
                case 2: {
                    s32 updateRate;

                    updateRate = arg4;
                    var_s0 += updateRate * 0x10;
                    if (var_s0 >= 0x100) {
                        var_s0 = 0xFF;
                    }
                    var_s3 += updateRate << 0xC;
                    temp_t9 = func_8002A204((s16) (var_s3 << 8)) + 0x18000;
                    actor->unk80 |= arg2;
                    var_s6 = (s32) (temp_t9 * ((s32) (var_s6 * var_s0) >> 8)) >> 0x10;
                    if (!(player->unk186 & var_s5)) {
                        var_s1->mode = 3;
                    }
                    break;
                }
                case 3: {
                    s32 updateRate;

                    updateRate = arg4;
                    var_s0 -= updateRate * 8;
                    if (var_s0 <= 0) {
                        var_s0 = 0;
                        var_s1->mode = 0;
                    } else {
                        var_s3 += updateRate << 0xC;
                        var_s6 = (s32) ((func_8002A204((s16) (var_s3 << 8)) + 0x18000) *
                                       ((s32) (var_s6 * var_s0) >> 8)) >> 0x10;
                        if (player->unk186 & var_s5) {
                            var_s1->mode = 2;
                        }
                    }
                    break;
                }
                }
            }
            var_s1->intensity = var_s0;
            var_s1->phase = var_s3;
            if (var_s0 != 0) {
                func_800475E8(temp_s7, var_s1->phase);
                func_800479D4(temp_s7, var_s6, var_f20, var_f22, var_s1->intensity);
            }
        }
        var_v0 = sp5C + 1;
        var_s5 *= 2;
        var_s1++;
    } while (var_v0 != 4);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/charControl/func_8001D960.s")
#endif
void func_8001DCD0(s16 rotation, ControlVector3 *vector, s16 *pitch, s16 *yaw) {
    f32 cosine;
    f32 pitchX;
    s32 angle;
    f32 y;
    f32 transformedX;

    angle = -rotation;
    cosine = func_8002A8C0(angle);
    transformedX = func_8002A8BC(angle);
    y = vector->y;
    pitchX = (vector->z * cosine) + (vector->x * transformedX);
    transformedX = (vector->z * transformedX) - (vector->x * cosine);
    *pitch = Arctanf(-pitchX, y);
    *yaw = Arctanf(transformedX, y);
}
s16 dAngle(s16 arg0, s16 arg1, f32 arg2);
void func_8001DCD0(s16 rotation, ControlVector3 *vector, s16 *pitch, s16 *yaw);
#pragma GLOBAL_ASM("asm/nonmatchings/main/charControl/func_8001DD70.s")
/* PROVENANCE: JFG's public charControl.c identifies the corresponding
 * controlSquashCheckPrior routine, but publishes assembly only; this body is
 * reconstructed from Mickey's fields, calls, branch conditions, and stores. */
#ifdef NON_MATCHING
/* Workbench verdict: structure-mismatch, 413 differing words, first mismatch +0x0. */
/* Candidate shape: 398 instructions/frame -0xF8 versus target 416/-0xD0; relocation roles are present but positionally different. */
/* Remaining structural gap: state-clear loop and local/call stack homes; not shape-exact. */
s32 func_8001E5C4(ControlActor *actor, ControlPlayer *player, f32 updateRate) {
    s16 sp40;
    s16 sp3E;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;
    s32 sp44;
    f32 spC4;
    f32 spC0;
    f32 spBC;
    f32 spB0;
    f32 spAC;
    f32 spA8;
    f32 spA4;
    f32 spA0;
    f32 sp9C;
    f32 sp98;
    f32 sp94;
    f32 sp90;
    f32 sp8C;
    f32 sp80;
    f32 sp88;
    f32 sp84;
    f32 sp7C;
    f32 sp78;
    f32 sp74;
    f32 sp70;
    f32 sp6C;
    f32 sp68;
    f32 sp64;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    s32 sp2C;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f18;
    f32 temp_f2;
    f32 var_f14;
    f32 var_f8;
    s16 temp_v0_3;
    s16 temp_v0_4;
    u8 temp_v0_2;
    s32 *var_v1;
    s32 var_a2;
    u32 temp_v0;
    u32 temp_t0;
    u32 temp_t3;
    var_v1 = (s32 *) &D_800CB2C0;
    var_a2 = 0xF;
    do {
        *var_v1 = 0;
        var_v1++;
        var_a2--;
    } while (var_a2 != 0);
    pointListRPY(player->unk2BC, (s16 *) actor, player->unk2C0, &spB0);
    spBC = spB0 + actor->x;
    spC0 = ((f32 *) &spB0)[1] + actor->y;
    spC4 = ((f32 *) &spB0)[2] + actor->z;
    sp70 = player->unk2B8->w;
    sp2C = (s32) &player->unk2F0;
    trackMakePolylist(1, (ControlVector3 *) sp2C,
                      (ControlVector3 *) &spBC, &sp70,
                      player->unk33C, 1);
    temp_v0 = (u32) func_80010900(
        (ControlVector3 *) sp2C, (ControlVector3 *) &spBC, sp70,
        (s32) actor, (void *) func_8001EC44);
    temp_t0 = temp_v0 >> 0x1E;
    temp_t3 = temp_v0 & 1;
    actor->x = spBC - spB0;
    actor->y = spC0 - ((f32 *) &spB0)[1];
    actor->z = spC4 - ((f32 *) &spB0)[2];
    sp44 = 0;
    if (temp_t0 != 0) {
        actor->x = player->unk38;
        actor->y = player->unk3C;
        actor->z = player->unk40;
        sp44 = 2;
    } else {
        player->unk349 = 0;
        player->unk34A = 0;
        player->unk34B = 0;
        player->unk18E = 0;
        player->unk334 = 0;
        player->unk344 = 0;
        if (temp_t3 != 0) {
            if (D_800CB2FD & 0x12) {
                player->unk349 = 1;
                if ((D_800CB2FD & 0x10) &&
                    (D_800CB2C0.hitObject != 0)) {
                    player->unk334 = D_800CB2C0.hitObject;
                }
            }
            if (D_800CB2FD & 0x48) {
                player->unk34B = (u8) (player->unk34B | 1);
            }
            if (D_800CB2FD & 0x24) {
                sp74 = 0.0f;
                sp78 = 0.0f;
                sp7C = -1.0f;
                sp3E = 0;
                sp40 = 0;
                sp3C = actor->rotationX;
                mathOneFloatRPY((ControlTransform *) &sp3C, &sp74);
                temp_f0 = sqrtf((D_800CB2D8 * D_800CB2D8) +
                                (D_800CB2D0.x * D_800CB2D0.x));
                temp_f16 = D_800CB2D0.x / temp_f0;
                sp64 = temp_f0;
                temp_f18 = D_800CB2D8 / temp_f0;
                player->unk90 = (temp_f18 * sp74) -
                                (sp7C * temp_f16);
                var_f14 = (sp7C * temp_f18) +
                          (sp74 * temp_f16);
                player->unk8C = var_f14;
                if (var_f14 < 0.0f) {
                    var_f14 = -var_f14;
                }
                temp_f0_2 = actor->velocityX;
                temp_f2 = actor->velocityZ;
                sp68 = temp_f18;
                sp6C = temp_f16;
                sp50 = var_f14;
                temp_f0_3 = sqrtf((temp_f0_2 * temp_f0_2) +
                                  (temp_f2 * temp_f2));
                if (temp_f0_3 > 0.0f) {
                    sp58 = temp_f0_2 / temp_f0_3;
                    sp54 = temp_f2 / temp_f0_3;
                }
                temp_v0_2 = player->unk198;
                temp_f16_2 = (sp6C * sp58) + (sp68 * sp54);
                if ((temp_v0_2 == 0) && (temp_f0_3 > 8.0f) &&
                    ((temp_f16_2 < D_80081864) ||
                     (D_80081868 < temp_f16_2))) {
                    temp_f12 = 2.0f * -temp_f16_2;
                    player->unk78 = 0.0f;
                    player->unk74 = (temp_f12 * sp6C) + sp58;
                    player->unk7C = (temp_f12 * sp68) + sp54;
                    temp_f14 = ((D_8008186C * sp50) + 0.5f) *
                               temp_f0_3;
                    player->unk80 = temp_f14;
                    player->unk84 = temp_f14;
                    player->unk181 = 1;
                    player->unk4 = (f32) (player->unk4 * 0.5f);
                    player->unk88 = D_80081870;
                    player->unk8 = (f32) (player->unk8 * 0.5f);
                } else {
                    var_f8 = (f32) temp_v0_2;
                    if (var_f8 < 240.0f) {
                        player->unk198 = (u8) (temp_v0_2 +
                                                (s32) updateRate);
                    } else {
                        player->unk198 = 0;
                        player->unk166 = 1;
                    }
                    sp44 = 1;
                }
                player->unk34A = (u8) (player->unk34A | 1);
            }
        }
        player->unk320 = D_800CB2FC;
        player->unk324 = D_800CB2F8;
        player->unk344 = (s32) (player->unk344 | D_800CB2F8);
    }
    spA4 = 0.0f;
    spA8 = 0.0f;
    spAC = 0.0f;
    if (player->unk16C != 1) {
        sp98 = 0.0f;
        sp9C = -50.0f;
        spA0 = 0.0f;
        mathOneFloatRPY((ControlTransform *) actor, &sp98);
        sp8C = sp98 + spBC;
        sp90 = sp9C + spC0;
        sp64 = 1.0f;
        sp94 = spA0 + spC4;
        if (func_80010654((ControlVector3 *) &spBC,
                          (ControlVector3 *) &sp8C,
                          (ControlVector3 *) &sp80, &sp64) != 0) {
            spA4 += sp80;
            spA8 += sp84;
            spAC += sp88;
        }
    }
    if (player->unk173 == 0) {
        sp2C = (s32) updateRate;
        func_8001DCD0(actor->rotationX, (ControlVector3 *) &spA4,
                      &sp3A, &sp38);
        actor->rotationZ = dAngle(
            actor->rotationZ, sp3A,
            1.0f - Powerf(D_80081874, sp2C));
        actor->rotationY = dAngle(
            actor->rotationY, sp38,
            1.0f - Powerf(D_80081878, sp2C));
    }
    temp_v0_3 = actor->rotationZ;
    if (temp_v0_3 >= 0x3001) {
        actor->rotationZ = 0x3000;
    } else if (temp_v0_3 < -0x3000) {
        actor->rotationZ = -0x3000;
    }
    temp_v0_4 = actor->rotationY;
    if (temp_v0_4 >= 0x3001) {
        actor->rotationY = 0x3000;
    } else if (temp_v0_4 < -0x3000) {
        actor->rotationY = -0x3000;
    }
    if (player->unk349 != 0) {
        actor->velocityY = (actor->y - player->unk3C) / updateRate;
    }
    if (player->unk34A == 0) {
        player->unk198 = 0;
    }
    player->unk2F0 = spBC;
    player->unk2F4 = spC0;
    player->unk2F8 = spC4;
    return sp44;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/charControl/func_8001E5C4.s")
#endif
/* PROVENANCE -- JFG's public charControl.c identifies the corresponding
 * controlSquashCheckPrior routine, but publishes assembly only; this body is
 * reconstructed from Mickey's fields, calls, branch conditions, and stores. */
#ifdef NON_MATCHING
/* Workbench verdict: structure-mismatch; 231 differing words, first mismatch +0x0. */
/* Target 238 instructions/frame -160; candidate 241 instructions/frame -224. */
/* Scalar relocation identities are repaired; common-prefix sinking and FP allocation remain. */
void func_8001EC44(s32 arg0, ControlVector3 *arg1, ControlVector3 *arg2,
                   f32 arg3, ControlCollisionPlane *arg4) {
    f32 sp94;
    f32 sp8C;
    f32 sp70;
    f32 sp6C;
    f32 sp5C;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    f32 normalX;
    f32 normalY;
    f32 normalZ;
    f32 planeDistance;
    f32 pointX;
    f32 pointY;
    f32 pointZ;
    f32 crossX;
    f32 crossY;
    f32 crossZ;
    f32 value;
    f32 distance;
    f32 delta;
    f32 vectorZ;
    f32 vectorX;
    f32 crossBase;
    f32 crossMiddle;
    f32 crossOther;

    planeDistance = arg4->distance;
    normalX = arg4->x;
    normalY = arg4->y;
    normalZ = arg4->z;
    sp70 = planeDistance;
    pointZ = arg1->z;
    sp58 = pointZ * normalZ;
    pointX = arg1->x;
    sp50 = normalX * pointX;
    pointY = arg1->y;
    value = sp58 + (sp50 + (normalY * pointY)) + planeDistance;
    if ((D_8008187C <= normalY) || (arg4->flags & 0x10000000)) {
        vectorZ = arg2->z;
        vectorX = arg2->x;
        crossBase = vectorZ * normalY;
        crossMiddle = (normalZ * vectorX) -
                      (vectorZ * normalX);
        crossOther = -(vectorX * normalY);

        crossX = (crossMiddle * normalZ) -
                 (crossOther * normalY);
        crossY = (crossOther * normalX) -
                 (crossBase * normalZ);
        crossZ = (crossBase * normalY) -
                 (crossMiddle * normalX);
        sp54 = crossY;
        sp4C = crossZ;
        distance = (crossX * crossX) + (crossY * crossY) +
                   (crossZ * crossZ);
        if (D_80081880 < distance) {
            sp5C = crossX;
            distance = sqrtf(distance);
            delta = arg3 - arg4->unk1C;
            arg1->x = arg4->unk10 + (delta * (crossX / distance));
            arg1->y = arg4->unk14 + (delta * (crossY / distance));
            arg1->z = arg4->unk18 + (delta * (crossZ / distance));
        } else {
            arg1->y = (-(sp58 + sp50 + sp70) / normalY) + D_80081884;
        }
        D_800CB2C4 = normalX;
        D_800CB2C8 = normalY;
        D_800CB2CC = normalZ;
        D_800CB2FD |= 2;
    } else if (normalY <= D_80081888) {
        delta = D_8008188C - value;
        arg1->x = pointX + (delta * normalX);
        arg1->y = pointY + (delta * normalY);
        arg1->z = pointZ + (delta * normalZ);
        D_800CB2DC = normalX;
        D_800CB2E0 = normalY;
        D_800CB2E4 = normalZ;
        D_800CB2FD |= 8;
    } else {
        f32 planeX;
        f32 planeZ;

        planeX = normalX;
        planeZ = normalZ;
        sp54 = pointX;
        delta = D_80081890 - value;
        sp4C = pointY;
        sp8C = delta;
        crossX = sp54 + (delta * normalX);
        crossY = sp4C + (delta * normalY);
        crossZ = pointZ + (delta * normalZ);
        normalX = sp54 - crossX;
        normalZ = pointZ - crossZ;
        sp94 = sp4C - crossY;
        value = func_8002A8BC(Arctanf(sp94,
                                      sqrtf((normalX * normalX) +
                                            (normalZ * normalZ))));
        if (value != 0.0f) {
            delta = sp8C / value;
            sp6C = delta;
            distance = sqrtf((planeX * planeX) +
                             (planeZ * planeZ));
            arg1->x += delta * (planeX / distance);
            arg1->z += delta * (planeZ / distance);
        } else {
            arg1->x = crossX;
            arg1->y = crossY;
            arg1->z = crossZ;
        }
        D_800CB2D0.x = planeX;
        D_800CB2D4 = arg4->y;
        D_800CB2D8 = planeZ;
        D_800CB2FD |= 4;
    }
    D_800CB2F8 = arg4->flags;
    D_800CB2FC = arg4->kind;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/main/charControl/func_8001EC44.s")
#endif
void func_8001EFFC(ControlTransform *transform, ControlPlayer *player, f32 *output) {
    f32 *current;
    s32 index;

    pointListRPY(player->unk2BC, (s16 *) transform, player->unk2C0, output);
    current = output;
    index = 0;
    if (player->unk2BC > 0) {
        do {
            current[0] += transform->x;
            current[1] += transform->y;
            current[2] += transform->z;
            current += 3;
            index++;
        } while (index < player->unk2BC);
    }
}
void func_8001F09C(ControlPlayer *player, s32 updateRate) {
    f32 rate;

    rate = (f32) updateRate;
    if (player->unk50 < player->unk54) {
        player->unk50 += ((player->unk54 - player->unk50) * 0.125f * rate) + D_80081894;
        if (player->unk54 <= player->unk50) {
            player->unk50 = player->unk54;
        }
    } else {
        player->unk50 += ((player->unk54 - player->unk50) * 0.125f * rate) - D_80081898;
        if (player->unk50 <= player->unk54) {
            player->unk50 = player->unk54;
        }
    }
}
void func_8001F14C(ControlTransform *transform, ControlCeilingContext *context) {
    register ControlSpawned *spawned;
    ControlSpawnPacket packet;
    f32 offset[3];
    f32 x;
    f32 y;
    f32 z;

    offset[0] = 0.0f;
    offset[1] = 0.0f;
    offset[2] = 10.0f;
    mathOneFloatRPY(transform, offset);
    x = offset[0] + transform->x;
    y = context->height;
    z = offset[2] + transform->z;
    packet.kind = 0x157;
    packet.mode = 0xC;
    packet.flags = 0;
    packet.x = (s16) x;
    packet.y = (s16) y;
    packet.z = (s16) z;
    packet.unkA = mathRnd(-0x7FFF, 0x7FFF);
    spawned = func_8000590C(&packet, 1);
    if (spawned != 0) {
        spawned->unk3C = 0;
    }
    if (context->handle != 0) {
        func_800031E8(context->handle);
    }
    func_80002FE0(0x329, x, y, z, 4, &context->handle);
}
/*
 * PROVENANCE -- JFG's src/charControl.c supplied the controlDisableJoypad
 * name/role. Mickey's two-argument field store independently determines this
 * per-player body and differs from JFG's one-argument global implementation.
 */
void controlDisableJoypad(ControlPlayer *player, s32 disabled) {
    player->joypadDisabled = disabled;
}
/* PROVENANCE -- adapted from JFG's src/charControl.c controlReadJoypad. */
void controlReadJoypad(ControlPlayer *player, s32 playerIndex) {
    if ((playerIndex >= 0) && (playerIndex < 4) && (player->joypadDisabled == 0)) {
        player->controlXjoy = joyGetStickX(playerIndex);
        player->controlAbsXjoy = joyGetAbsX(playerIndex);
        player->controlYjoy = joyGetStickY(playerIndex);
        player->controlAbsYjoy = joyGetAbsY(playerIndex);
        player->controlKeys = joyGetButtons(playerIndex);
        player->controlDkeys = joyGetPressed(playerIndex);
        player->controlReleasedKeys = joyGetReleased(playerIndex);
    } else {
        player->controlXjoy = 0;
        player->controlAbsXjoy = 0;
        player->controlYjoy = 0;
        player->controlAbsYjoy = 0;
        player->controlKeys = 0;
        player->controlDkeys = 0;
        player->controlReleasedKeys = 0;
    }
}
/*
 * PROVENANCE -- JFG's charControl symbols supplied the controlSetRumble
 * name/role. Mickey's smaller wrapper independently determines this body.
 */
void controlSetRumble(ControlPlayer *player, s32 strength, f32 duration) {
    if ((player->unk191 == 0) && !(player->flags1A8 & 1)) {
        rumbleStart(player->playerIndex, strength, duration);
    }
}
void func_8001F364(void) {
}
void controlSetPlayerSetup(s16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    D_800CB470 = arg0;
    D_800CB472 = arg1;
    D_800CB474 = arg2;
    D_800CB476 = arg3;
    D_80079BF8 = 1;
}
/*
 * PROVENANCE -- JFG's charControl symbols supplied the controlGetPlayerSetup
 * name/role. This body is reconstructed from Mickey's setup-state accesses.
 */
s32 controlGetPlayerSetup(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3) {
    if (D_80079BF8 != 0) {
        *arg0 = D_800CB470;
        *arg1 = D_800CB472;
        *arg2 = D_800CB474;
        *arg3 = D_800CB476;
        D_80079BF8 = 0;
        return 1;
    }
    return 0;
}

/* PROVENANCE -- adapted from JFG's src/charControl.c controlClearPlayerSetup. */
void controlClearPlayerSetup(void) {
    D_80079BF8 = 0;
}

/* PLATEAU-HANDOFF:func_8001D960:start
 * symbol: func_8001D960
 * score: 176 differing words
 * frame: 0x70
 * relocations: 6
 * first-mismatch: +0x0
 * summary: one word long; fifth-argument promotion rotates saved-register roles and adds 0x10 non-save frame
 * PLATEAU-HANDOFF:func_8001D960:end
 */

/* PLATEAU-HANDOFF:func_8001EC44:start
 * symbol: func_8001EC44
 * score: 231 differing words
 * frame: 0xE0
 * relocations: 45
 * first-mismatch: +0x0
 * summary: Target 238w, frame 0xA0, 47 relocs; scalar identities and X/Z normalization are correct, but common-prefix sinking and FP/integer allocation remain.
 * PLATEAU-HANDOFF:func_8001EC44:end
 */

/* PLATEAU-HANDOFF:func_8001CB84:start
 * symbol: func_8001CB84
 * score: 317/455 words
 * frame: 0xB8
 * relocations: 41
 * first-mismatch: +0x0
 * summary: Exact geometry and relocation offset/type surface; reduce the 0x38 frame/home excess before addressing the FP register-web cascade.
 * PLATEAU-HANDOFF:func_8001CB84:end
 */
