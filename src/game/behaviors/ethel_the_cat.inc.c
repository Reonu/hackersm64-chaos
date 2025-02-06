#include "game/spawn_sound.h"
#include "game/camera.h"
#include "game/print.h"

#define FRAME_FIRST_MEME 47
#define FRAME_SECOND_IDLE 93
#define FRAME_SECOND_MEME 129
#define FRAME_THIRD_IDLE 194
#define FRAME_THIRD_MEME 232
#define FRAME_FOURTH_IDLE 294
#define FRAME_FOURTH_MEME 303
#define FRAME_END 350

#define BASE_CAMERA_HEIGHT_IDLE gLakituState.curPos[1] - 103
#define BASE_CAMERA_HEIGHT_MEME gLakituState.curPos[1] - 103

#define SPINNING_SPEED_FAST DEGREES(30)
#define SPINNING_SPEED_NORMAL DEGREES(15)

#define BOB_SPEED_FAST 3000
#define BOB_SPEED_NORMAL 1500

enum EthelActions {
    ETHEL_ACT_IDLE,
    ETHEL_ACT_FIRST_MEME,
    ETHEL_ACT_IDLE2,
    ETHEL_ACT_SECOND_MEME,
    ETHEL_ACT_IDLE3,
    ETHEL_ACT_THIRD_MEME,
    ETHEL_ACT_IDLE4,
    ETHEL_ACT_FOURTH_MEME,
    ETHEL_ACT_END,
};

void bhv_ethel_the_cat_init(void) {
    cur_obj_play_sound_2(SOUND_NEW_OIIAOIIA);
}

void bhv_ethel_act_idle(void) {
    o->oFaceAngleYaw = gCamera->yaw;
    o->oPosY = BASE_CAMERA_HEIGHT_IDLE;
    if (o->oChaosTimer == FRAME_FIRST_MEME) {
        o->oAction = ETHEL_ACT_FIRST_MEME;
    }
}

void bhv_ethel_act_first_meme(void) {
    o->oPosY = BASE_CAMERA_HEIGHT_MEME + (sins(o->oBitfsPlatformTimer) * 100.f);
    o->oBitfsPlatformTimer += BOB_SPEED_FAST;
    o->oFaceAngleYaw += SPINNING_SPEED_FAST;
    if (o->oChaosTimer == FRAME_SECOND_IDLE) {
        o->oAction = ETHEL_ACT_IDLE2;
    }
}

void bhv_ethel_act_idle2(void) {
    bhv_ethel_act_idle();
    o->oPosY = BASE_CAMERA_HEIGHT_IDLE;
    if (o->oChaosTimer == FRAME_SECOND_MEME) {
        o->oAction = ETHEL_ACT_SECOND_MEME;
    }
}

void bhv_ethel_act_second_meme(void) { 
    o->oPosY = BASE_CAMERA_HEIGHT_MEME + (sins(o->oBitfsPlatformTimer) * 100.f);
    o->oBitfsPlatformTimer += BOB_SPEED_NORMAL;
    o->oFaceAngleYaw += SPINNING_SPEED_NORMAL;
    if (o->oChaosTimer == FRAME_THIRD_IDLE) {
        o->oAction = ETHEL_ACT_IDLE3;
    }
}

void bhv_ethel_act_idle3(void) {
    bhv_ethel_act_idle();
    if (o->oChaosTimer == FRAME_THIRD_MEME) {
        o->oAction = ETHEL_ACT_THIRD_MEME;
    }    
}

void bhv_ethel_act_third_meme(void) {
    o->oPosY = BASE_CAMERA_HEIGHT_MEME + (sins(o->oBitfsPlatformTimer) * 100.f);
    o->oBitfsPlatformTimer += BOB_SPEED_NORMAL;
    o->oFaceAngleYaw += SPINNING_SPEED_NORMAL;
    if (o->oChaosTimer == FRAME_FOURTH_IDLE) {
        o->oAction = ETHEL_ACT_IDLE4;
    }    
}

void bhv_ethel_act_idle4(void) {
    bhv_ethel_act_idle();
    if (o->oChaosTimer == FRAME_FOURTH_MEME) {
        o->oAction = ETHEL_ACT_FOURTH_MEME;
    }
}

void bhv_ethel_act_fourth_meme(void) {
    o->oPosY = BASE_CAMERA_HEIGHT_MEME + (sins(o->oBitfsPlatformTimer) * 100.f);
    o->oBitfsPlatformTimer += BOB_SPEED_FAST;
    o->oFaceAngleYaw += SPINNING_SPEED_FAST;
    if (o->oChaosTimer == FRAME_END) {
        o->oAction = ETHEL_ACT_END;
    }
}

void bhv_ethel_act_end(void) {
    bhv_ethel_act_idle();
    if (o->oChaosTimer >= FRAME_END + 60) {
        mark_obj_for_deletion(o);
    }
}

void bhv_ethel_the_cat_loop(void) {
    o->oPosX = gLakituState.curPos[0] - 650 * sins(gLakituState.yaw);
    o->oPosZ = gLakituState.curPos[2] - 650 * coss(gLakituState.yaw);


    //print_text_fmt_int(20, 20, "TIME %d", o->oTimer);

    switch (o->oAction) {
        case ETHEL_ACT_IDLE:
            cur_obj_set_model(MODEL_ETHEL_THE_CAT);
            bhv_ethel_act_idle();
            break;
        case ETHEL_ACT_FIRST_MEME:
            cur_obj_set_model(MODEL_ETHEL_THE_CAT_LOAF);
            bhv_ethel_act_first_meme();
            break;
        case ETHEL_ACT_IDLE2:
            cur_obj_set_model(MODEL_ETHEL_THE_CAT);
            bhv_ethel_act_idle2();
            break;
        case ETHEL_ACT_SECOND_MEME:
            cur_obj_set_model(MODEL_ETHEL_THE_CAT_LOAF);
            bhv_ethel_act_second_meme();
            break;
        case ETHEL_ACT_IDLE3:
            cur_obj_set_model(MODEL_ETHEL_THE_CAT);
            bhv_ethel_act_idle3();
            break;
        case ETHEL_ACT_THIRD_MEME:
            cur_obj_set_model(MODEL_ETHEL_THE_CAT_LOAF);
            bhv_ethel_act_third_meme();
            break;
        case ETHEL_ACT_IDLE4:
            cur_obj_set_model(MODEL_ETHEL_THE_CAT);
            bhv_ethel_act_idle4();
            break;
        case ETHEL_ACT_FOURTH_MEME:
            cur_obj_set_model(MODEL_ETHEL_THE_CAT_LOAF);
            bhv_ethel_act_fourth_meme();
            break;
        case ETHEL_ACT_END:
            cur_obj_set_model(MODEL_ETHEL_THE_CAT);
            bhv_ethel_act_end();
            break;
    }
}