// spindrift.inc.c
#include "game/chaos_codes.h"
struct ObjectHitbox sSpindriftHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 2,
    /* health:            */ 1,
    /* numLootCoins:      */ 3,
    /* radius:            */ 90,
    /* height:            */ 80,
    /* hurtboxRadius:     */ 80,
    /* hurtboxHeight:     */ 70,
};

void bhv_spindrift_loop(void) {
    o->activeFlags |= ACTIVE_FLAG_IGNORE_ENV_BOXES;

    if (cur_obj_set_hitbox_and_die_if_attacked(&sSpindriftHitbox, SOUND_OBJ_DYING_ENEMY1, 0)) {
        cur_obj_change_action(1);
    }
    cur_obj_update_floor_and_walls();

    switch (o->oAction) {
        case 0:
            if (gChaosCodeTable[GLOBAL_CHAOS_FAST_ENEMIES].active) {
                approach_f32_symmetric_bool(&o->oForwardVel, 40.0f, 10.0f);
            } else {
                approach_f32_symmetric_bool(&o->oForwardVel, 4.0f, 1.0f);
            }
            
            if (cur_obj_lateral_dist_from_mario_to_home() > 1000.0f) {
                o->oAngleToMario = cur_obj_angle_to_home();
            } else if (o->oDistanceToMario > 300.0f) {
                o->oAngleToMario = obj_angle_to_object(o, gMarioObject);
            }
            if (gChaosCodeTable[GLOBAL_CHAOS_FAST_ENEMIES].active) {
                cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x1200);
            } else {
                cur_obj_rotate_yaw_toward(o->oAngleToMario, 0x400);
            }
            break;

        case 1:
            o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
            o->oForwardVel = -10.0f;
            if (o->oTimer > 20) {
                o->oAction = 0;
                o->oInteractStatus = INT_STATUS_NONE;
                o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
            }
            break;
    }

    cur_obj_move_standard(-60);
}
