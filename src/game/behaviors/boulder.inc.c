// boulder.inc.c
#include "game/chaos_codes.h"
void bhv_big_boulder_init(void) {
    vec3f_copy(&o->oHomeVec, &o->oPosVec);

    o->oGravity = 8.0f;
    o->oFriction = 0.999f;
    o->oBuoyancy = 2.0f;
}

void boulder_act_1(void) {
    s16 collisionFlags = object_step_without_floor_orient();

    if ((collisionFlags & OBJ_COL_FLAGS_LANDED) == OBJ_COL_FLAG_GROUNDED && o->oVelY > 10.0f) {
        cur_obj_play_sound_2(SOUND_GENERAL_GRINDEL_ROLL);
        spawn_mist_particles();
    }

    if (o->oForwardVel > 70.0f) {
        o->oForwardVel = 70.0f;
    }

    if (o->oPosY < -1000.0f) {
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }
}

void bhv_big_boulder_loop(void) {
    cur_obj_scale(1.5f);

    o->oGraphYOffset = 270.0f;
    if (gHMCChaosTable[HMC_BOULDERS_FACE_MARIO].active) {
        o->oMoveAngleYaw = o->oAngleToMario;
        o->oFaceAngleYaw = o->oAngleToMario;
    }
    switch (o->oAction) {
        case 0:
            o->oForwardVel = 40.0f;
            o->oAction = 1;
            break;

        case 1:
            boulder_act_1();
            adjust_rolling_face_pitch(1.5f);
            cur_obj_play_sound_1(SOUND_ENV_BOWLING_BALL_ROLL);
            break;
    }

    set_rolling_sphere_hitbox();
    if (o->oBehParams == HMC_BOULDER_BP_CHAOS && o->oChaosTimer > 120) {
        obj_mark_for_deletion(o);
    }


    //safeguard
    if (o->oChaosTimer > 300) {
        obj_mark_for_deletion(o);
    }
}

void bhv_big_boulder_generator_loop(void) {
    struct Object *boulderObj;

    if (o->oTimer >= 256) {
        o->oTimer = 0;
    }

#ifdef ENABLE_VANILLA_LEVEL_SPECIFIC_CHECKS
    if (!current_mario_room_check(4)
        || is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 1500)) {
#else 
    if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 1500)) {
#endif
        return;
    }

    if (is_point_within_radius_of_mario(o->oPosX, o->oPosY, o->oPosZ, 6000)) {
        if (!(o->oTimer & 0x3F)) {
            boulderObj = spawn_object(o, MODEL_HMC_ROLLING_ROCK, bhvBigBoulder);
            boulderObj->oMoveAngleYaw = random_float() * 4096.0f;
        }
    } else {
        if (!(o->oTimer & 0x7F)) {
            boulderObj = spawn_object(o, MODEL_HMC_ROLLING_ROCK, bhvBigBoulder);
            boulderObj->oMoveAngleYaw = random_float() * 4096.0f;
        }
    }
}
