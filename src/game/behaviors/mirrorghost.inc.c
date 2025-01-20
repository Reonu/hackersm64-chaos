
enum MirrorGhostActs {
    MIRRORGHOST_WAIT,
    MIRRORGHOST_FOLLOW,
};

static struct ObjectHitbox sMirrorMarioHitbox = {
    /* interactType:      */ INTERACT_MR_BLIZZARD,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 99,
    /* numLootCoins:      */ 0,
    /* radius:            */ 60,
    /* height:            */ 160,
    /* hurtboxRadius:     */ 90,
    /* hurtboxHeight:     */ 160,
};

void bhv_mirrorghost_init(void) {
    o->oTimer = 0;
    o->oAction = MIRRORGHOST_WAIT;
}

extern const struct Animation *const mario_anims[];
struct DmaHandlerList *gMirrorAnimList;

s16 set_custom_mario_animation_mirror(s32 targetAnimID) {
    if (o->header.gfx.animInfo.animID != targetAnimID) {
        struct Animation **animPtrAddr = (struct Animation **) &mario_anims[targetAnimID];
        struct Animation **animSegmented = segmented_to_virtual(animPtrAddr);
        struct Animation *targetAnim = segmented_to_virtual(*animSegmented);

        o->header.gfx.animInfo.animID = targetAnimID;
        o->header.gfx.animInfo.curAnim = targetAnim;
        o->header.gfx.animInfo.animAccel = 0;
        o->header.gfx.animInfo.animYTrans = 0xBD;

        if (targetAnim->flags & ANIM_FLAG_NO_ACCEL) {
            o->header.gfx.animInfo.animFrame = targetAnim->startFrame;
        } else {
            if (targetAnim->flags & ANIM_FLAG_FORWARD) {
                o->header.gfx.animInfo.animFrame = targetAnim->startFrame + 1;
            } else {
                o->header.gfx.animInfo.animFrame = targetAnim->startFrame - 1;
            }
        }
    }

    return o->header.gfx.animInfo.animFrame;
}

s16 set_mario_anim_with_accel_mirror(s32 targetAnimID, s32 accel) {
    struct Animation *targetAnim = gMirrorAnimList->bufTarget;

    if (load_patchable_table(gMirrorAnimList, targetAnimID)) {
        targetAnim->values = (void *) VIRTUAL_TO_PHYSICAL((u8 *) targetAnim + (uintptr_t) targetAnim->values);
        targetAnim->index = (void *) VIRTUAL_TO_PHYSICAL((u8 *) targetAnim + (uintptr_t) targetAnim->index);
    }

    if (o->header.gfx.animInfo.animID != targetAnimID) {
        o->header.gfx.animInfo.animID = targetAnimID;
        o->header.gfx.animInfo.curAnim = targetAnim;
        o->header.gfx.animInfo.animYTrans = 0xBD;

        if (targetAnim->flags & ANIM_FLAG_NO_ACCEL) {
            o->header.gfx.animInfo.animFrameAccelAssist = (targetAnim->startFrame << 0x10);
        } else {
            if (targetAnim->flags & ANIM_FLAG_FORWARD) {
                o->header.gfx.animInfo.animFrameAccelAssist = (targetAnim->startFrame << 0x10) + accel;
            } else {
                o->header.gfx.animInfo.animFrameAccelAssist = (targetAnim->startFrame << 0x10) - accel;
            }
        }

        o->header.gfx.animInfo.animFrame = (o->header.gfx.animInfo.animFrameAccelAssist >> 0x10);
    }

    o->header.gfx.animInfo.animAccel = accel;

    return o->header.gfx.animInfo.animFrame;
}


#include "string.h"

void bhv_mirrorghost_loop(void) {
    if (o->oAction == MIRRORGHOST_WAIT) {
        o->oTimer++;
        if (o->oTimer >= 45) {
            gMirrorAnimList = &gMirrorAnimsBuf;
            o->oAction = MIRRORGHOST_FOLLOW;
            o->oTimer = (gMarioState->shadowTime + 1) % 45;
            obj_set_hitbox(o, &sMirrorMarioHitbox);
            set_mario_anim_with_accel_mirror(MARIO_ANIM_IDLE_HEAD_CENTER, 0);
        }
    } else {
        o->oInteractStatus = INT_STATUS_NONE;
        struct MarioState *m = gMarioState;
        s32 p = (m->shadowTime + 1) % 45;
        o->oPosX = m->shadowPos[p][0];
        o->oPosY = m->shadowPos[p][1];
        o->oPosZ = m->shadowPos[p][2];
        o->oFaceAngleYaw = m->shadowAngle[p];
        s32 accel = m->shadowAnimFrame[p];
        if (accel == 0) {
            accel = 0x10000;
        }
        set_mario_anim_with_accel_mirror(m->shadowAnim[p], accel);
    }
}