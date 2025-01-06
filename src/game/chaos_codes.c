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
#include "src/game/camera.h"


u64 globalChaosFlags = GLOBAL_CHAOS_FLAG_NONE;
s32 nextGlobalCodeTimer = 150;
u8 gDisableChaos = TRUE;

void chaos_cannon(void) {
    struct Object *cannon = spawn_object_relative(0, 0, 300, 0, gMarioState->marioObj, MODEL_NONE, bhvCannon);
    SET_BPARAM1(cannon->oBehParams, CHAOS_CODE_BPARAM);
    globalChaosFlags &= ~GLOBAL_CHAOS_FLAG_ENTER_CANNON;
}

void chaos_fall_damage(void) {
    //stub until we figure out timers :)
}

// Not final, just there to have a different func
void chaos_trip(void) {
    gMarioState->action = ACT_HARD_BACKWARD_GROUND_KB;
    globalChaosFlags &= ~GLOBAL_CHAOS_FLAG_TRIPPING;
}

void chaos_upside_down_camera(void) {
    sFOVState.fovFunc = CAM_FOV_SET_315;
}

void chaos_no_model_is_mario(void) {
    //also stub until timer
}

ChaosCode gChaosCodeTable[] = {
    {"Cannon", chaos_cannon},
    {"Fall Damage", chaos_fall_damage},
    {"Trip", chaos_trip},
    {"Upside Down Camera", chaos_upside_down_camera},
    {"Model None Mario", chaos_no_model_is_mario}
};

void add_global_chaos_code(void) {
    u16 chosenCode;
    chosenCode = random_u16() % (sizeof(gChaosCodeTable) / sizeof(ChaosCode));
    globalChaosFlags |= 1 << chosenCode;
    append_puppyprint_log("Chaos effect added: %s", gChaosCodeTable[chosenCode].name);
}

void update_chaos_code_effects(void) {
    for (u32 i = 0; i < sizeof(gChaosCodeTable) / sizeof(ChaosCode); i++) {
        u64 codeFlag = (1 << i);
        if (globalChaosFlags & codeFlag) {
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