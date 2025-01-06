#include "include/types.h"
#include "src/game/chaos_codes.h"
#include "src/game/game_init.h"
#include "src/game/level_update.h"
#include "include/model_ids.h"
#include "include/behavior_data.h"
#include "include/object_fields.h"
#include "include/object_constants.h"
#include "puppyprint.h"
#include "sm64.h"
#include "object_helpers.h"
#include "engine/math_util.h"
#include "game/main.h"
#include "src/game/camera.h"
#include "fb_effects.h"
#include "print.h"


u64 globalChaosFlags = GLOBAL_CHAOS_FLAG_NONE;
s32 nextGlobalCodeTimer = 150;
u32 gCurrentChaosID;
u8 gDisableChaos = TRUE;
u8 gRetroVision = FALSE;
u8 gBlurVision = FALSE;

extern s32 gChaosCodeTimers[];
extern OSViMode VI;

void chaos_cannon(void) {
    struct Object *cannon = spawn_object_relative(0, 0, 300, 0, gMarioState->marioObj, MODEL_NONE, bhvCannon);
    SET_BPARAM1(cannon->oBehParams, CHAOS_CODE_BPARAM);
    globalChaosFlags &= ~(1 << gCurrentChaosID);
}

void chaos_fall_damage(void) {
    //stub until we figure out timers :)
}

// Not final, just there to have a different func
void chaos_trip(void) {
    gMarioState->action = ACT_HARD_BACKWARD_GROUND_KB;
    globalChaosFlags &= ~(1 << gCurrentChaosID);
}

void chaos_retro(void) {
    if (gRetroVision == FALSE) {
        change_vi(&VI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        gRetroVision = TRUE;
    }
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        gRetroVision = FALSE;
        change_vi(&VI, SCREEN_WIDTH, SCREEN_HEIGHT);
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_blur(void) {
    if (gBlurVision == FALSE) {
        set_fb_effect_type(FBE_EFFECT_MULT);
        gBlurVision = TRUE;
    }
    set_motion_blur(32);
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        gBlurVision = FALSE;
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_upside_down_camera(void) {
    sFOVState.fovFunc = CAM_FOV_SET_315;
}

void chaos_no_model_is_mario(void) {
    //also stub until timer
}

ChaosCode gChaosCodeTable[] = {
    {"Cannon", chaos_cannon, 0, 0},
    {"Fall Damage", chaos_fall_damage, 0, 0},
    {"Trip", chaos_trip, 0, 0},
    {"Upside Down Camera", chaos_upside_down_camera, 0, 0},
    {"Model None Mario", chaos_no_model_is_mario, 0, 0},
    {"Retro Vision", chaos_retro, 15, 30},
    {"Blur Vision", chaos_blur, 15, 30},
};

s32 gChaosCodeTimers[sizeof(gChaosCodeTable) / sizeof(ChaosCode)];

void chaos_enable(s32 codeID) {
    globalChaosFlags |= 1 << codeID;
    int rand = random_u16() % (gChaosCodeTable[codeID].timerHigh - gChaosCodeTable[codeID].timerLow);
    gChaosCodeTimers[codeID] = (gChaosCodeTable[codeID].timerLow + rand) * 30;
    append_puppyprint_log("Chaos effect added: %s", gChaosCodeTable[codeID].name);
}

void add_global_chaos_code(void) {
    u16 chosenCode = random_u16() % (sizeof(gChaosCodeTable) / sizeof(ChaosCode));
    chaos_enable(chosenCode);
}

void update_chaos_code_effects(void) {
    for (u32 i = 0; i < sizeof(gChaosCodeTable) / sizeof(ChaosCode); i++) {
        u64 codeFlag = (1 << i);
        if (globalChaosFlags & codeFlag) {
            gCurrentChaosID = i;
            (gChaosCodeTable[i].func)();
        }
    }
}

void global_chaos_code_handler(void) {


    update_chaos_code_effects();

    if (gDisableChaos) {
        return;
    }

    nextGlobalCodeTimer--;
    if (nextGlobalCodeTimer <= 0) {
        add_global_chaos_code();
        nextGlobalCodeTimer = 150 + (random_u16() % 600);
    }
}