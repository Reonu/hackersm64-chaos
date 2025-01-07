#include "src/game/mario.h"

void bhv_kart_controller_init(void) {
    cur_obj_scale(0.4f);
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
        break;
    }

    o->oPosX = gMarioState->pos[0];
    o->oPosY = gMarioState->pos[1];
    o->oPosZ = gMarioState->pos[2];
    o->oFaceAngleYaw = gMarioState->faceAngle[1];
}

