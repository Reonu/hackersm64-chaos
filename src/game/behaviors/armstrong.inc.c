#include "game/game_init.h"
#include "game/level_update.h"
#include "game/interaction.h"
#include "game/object_helpers.h"
#include "game/spawn_sound.h"
#include "game/sound_init.h"
#include "engine/math_util.h"
#include "audio/external.h"
#include "engine/graph_node.h"

enum ArmstrongActions {
    ARMSTRONG_ACT_CHASE_MARIO,
    ARMSTRONG_ACT_IDLE,
};

void bhv_armstrong_init(void) {
    o->oArmstrongGoalX = gMarioState->pos[0];
    o->oArmstrongGoalY = gMarioState->pos[1];
    o->oArmstrongGoalZ = gMarioState->pos[2];
    if (gMarioState->action != ACT_END_PEACH_CUTSCENE) {
        seq_player_fade_to_target_volume(SEQ_PLAYER_LEVEL, 1, 0);
        cur_obj_play_sound_2(SOUND_NEW_2_HAS_TO_BE_THIS_WAY);
    }
}

void bhv_armstrong_chase_mario(void) {
    f32 dx = o->oArmstrongGoalX - o->oPosX;
    f32 dy = o->oArmstrongGoalY - o->oPosY;
    f32 dz = o->oArmstrongGoalZ - o->oPosZ;

    f32 dist = sqrtf(dx * dx + dz * dz);
    f32 angle = atan2s(dz, dx);

    o->oPosX += 100 * sins(angle);
    o->oPosZ += 100 * coss(angle);
    o->oPosY = approach_f32(o->oPosY, gMarioState->pos[1], 10.f, 10.f);

    if (dist <= 200.f || o->oDistanceToMario < 500.f) {
        if (o->oDistanceToMario < 500.f) {
            if (gMarioState->action == ACT_END_PEACH_CUTSCENE) {
                spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_EXPLOSION, bhvExplosion);
                obj_set_model(gMarioState->marioObj, MODEL_NONE);
            } else {
                gMarioState->action = ACT_REKT_BY_ARMSTRONG;
                gMarioState->marioObj->oFaceAngleYaw = o->oAngleToMario + DEGREES(180);
                gMarioState->marioObj->oMoveAngleYaw = o->oAngleToMario + DEGREES(180);
            }
        }
        o->oAction = ARMSTRONG_ACT_IDLE;
    }

    if (o->oTimer == 0) {
        cur_obj_play_sound_2(SOUND_NEW_ARMSTRONG_SCREAM);
    }
}

void bhv_armstrong_act_idle(void) {
    if (o->oTimer == 0) {
        cur_obj_play_sound_2(SOUND_NEW_ARMSTRONG_OMELETTES);
    }
    
    if (o->oTimer > 120) {
        if (gMarioState->action != ACT_END_PEACH_CUTSCENE) {
            seq_player_fade_to_normal_volume(SEQ_PLAYER_LEVEL, 1);
        }
        mark_obj_for_deletion(o);
    }
}

void bhv_armstrong_loop(void) {
    static u16 model = MODEL_ARMSTRONG_FRAME_1;
    switch (o->oAction) {
        case ARMSTRONG_ACT_CHASE_MARIO:
            bhv_armstrong_chase_mario();
            if (gGlobalTimer % 2) {
                model++;
            }
            if (model > MODEL_ARMSTRONG_FRAME_3) {
                model = MODEL_ARMSTRONG_FRAME_1;
            }
            cur_obj_set_model(model);
            break;
        case ARMSTRONG_ACT_IDLE:
            bhv_armstrong_act_idle();
            cur_obj_set_model(MODEL_ARMSTRONG_IDLE);
            break;
    }
}