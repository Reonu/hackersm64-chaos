#include "src/game/mario.h"
#include "src/game/chaos_codes.h"

void bhv_kart_controller_init(void) {
    cur_obj_scale(0.3f);
}

void bhv_kart_controller_loop(void) {
    switch (o->oAction) {
        case 0:
            set_mario_action(gMarioState, ACT_RIDING_KART, 0);
            set_custom_mario_animation(gMarioState, 0);
            cur_obj_init_animation(0);
            o->oAction++;
        break;
        case 1:
            if (o->oTimer > 90) {
                o->oAction++;
            }
            

            o->oPosX = gMarioState->pos[0];
            o->oPosY = gMarioState->pos[1];
            o->oPosZ = gMarioState->pos[2];
            o->oFaceAngleYaw = gMarioState->faceAngle[1];
        break;
        case 2:
            if (gPlayer1Controller->stickMag < 2) {
                cur_obj_init_animation(1);
                set_custom_mario_animation(gMarioState, 1);
            }
            else if (gPlayer1Controller->rawStickX < 0) {
                if (gMarioState->forwardVel > 2) {
                    cur_obj_init_animation(2);
                    set_custom_mario_animation(gMarioState, 2);
                }
                else {
                    cur_obj_init_animation(3);
                    set_custom_mario_animation(gMarioState, 3);
                }
            }
            else {
                if (gMarioState->forwardVel > 2) {
                    cur_obj_init_animation(4);
                    set_custom_mario_animation(gMarioState, 4);
                }
                else {
                    cur_obj_init_animation(5);
                    set_custom_mario_animation(gMarioState, 5);
                }
            }

            

            o->oPosX = gMarioState->pos[0];
            o->oPosY = gMarioState->pos[1];
            o->oPosZ = gMarioState->pos[2];
            o->oFaceAngleYaw = gMarioState->faceAngle[1];

            if (gMarioState->action != ACT_RIDING_KART) {
                o->oAngleVelYaw = random_u16() % 512;
                o->oAngleVelPitch = random_u16() % 512;
                o->oAngleVelRoll = random_u16() % 512;
                o->oForwardVel = gMarioState->forwardVel;
                o->oMoveAngleYaw = o->oFaceAngleYaw;
                o->oGravity = -3.0f;
                o->oAction = 3;
            }
        break;
        case 3:
            cur_obj_move_standard(-78);
            cur_obj_update_floor_and_walls();
            o->oFaceAngleYaw += o->oAngleVelYaw;
            o->oFaceAngleRoll += o->oAngleVelRoll;
            o->oFaceAnglePitch += o->oAngleVelPitch;
            if (o->oMoveFlags & (OBJ_MOVE_HIT_WALL | OBJ_MOVE_LANDED)) {
                struct Object *explosion = spawn_object(o, MODEL_EXPLOSION, bhvExplosion);
                explosion->oGraphYOffset += 100.0f;
                obj_mark_for_deletion(o);
            }
        break;
    }
}

