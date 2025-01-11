#include "engine/surface_collision.h"
#include "game/spawn_sound.h"
#include "game/game_init.h"
#include "game/level_update.h"

struct ObjectHitbox sRockHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 20,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 200,
    /* height:            */ 250,
    /* hurtboxRadius:     */ 140,
    /* hurtboxHeight:     */ 240,
};

enum volcano_actions {
    VOLCANO_ACT_THREATEN,
    VOLCANO_ACT_FALL_DOWN,
    VOLCANO_ACT_DISAPPEAR,
};

void bhv_rock_volcano_init(void) {
   o->oAction = VOLCANO_ACT_THREATEN;
   o->oFaceAngleYaw = random_u16();
   o->oFaceAnglePitch = random_u16();
   o->oFaceAngleRoll = random_u16();
}

void bhv_rock_volcano_threaten(void) {
    o->oPosY = gMarioState->pos[1] + 800;
    if (o->oTimer == 15) {
        o->oAction = VOLCANO_ACT_FALL_DOWN;
    }
}

void bhv_rock_volcano_fall_down(void) {
    f32 floorHeight = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
    o->oPosY -= 70.f;

    /*if (o->oTimer < 60) {
        o->oPosX = gMarioState->pos[0];
        o->oPosZ = gMarioState->pos[2];
    }*/

    if ((o->oPosY - floorHeight) < 5.f) {
        o->oAction = VOLCANO_ACT_DISAPPEAR;
    }
}

void bhv_rock_volcano_disappear(void) {
    if (o->oTimer == 0) {
        spawn_mist_particles();
        cur_obj_disable_rendering();
    } else if (o->oTimer >= 30) {
        obj_mark_for_deletion(o);
    }
    cur_obj_play_sound_2(SOUND_GENERAL_EXPLOSION6);
}

void bhv_rock_volcano_loop(void) {
    switch (o->oAction) {
        case VOLCANO_ACT_THREATEN:
            bhv_rock_volcano_threaten();
            break;
        case VOLCANO_ACT_FALL_DOWN:
            bhv_rock_volcano_fall_down();
            break;
        case VOLCANO_ACT_DISAPPEAR:
            bhv_rock_volcano_disappear();
            break;
    }
    o->oInteractStatus = INT_STATUS_NONE;
}

