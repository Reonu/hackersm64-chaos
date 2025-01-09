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
#include "src/audio/external.h"


u64 globalChaosFlags = GLOBAL_CHAOS_FLAG_NONE;
s32 nextGlobalCodeTimer = 150;
u32 gCurrentChaosID;
float gCrimes = 0;
s32 gCrimeSpawnTimer;
u8 gDisableChaos = TRUE;
u8 gRetroVision = FALSE;
u8 gBlurVision = FALSE;
u8 gLowFPS = FALSE;
u8 gTankControls = FALSE;
u8 gFlipInputs = FALSE;
u8 gDimLights = FALSE;
u8 gLawMetre = FALSE;
u8 gTinyMario = FALSE;
u8 gBillboardMario = FALSE;
u8 gCCMVolcanoRocks = FALSE;
u8 gVerySlippery = FALSE;

extern s32 gChaosCodeTimers[];
extern OSViMode VI;

void chaos_cannon(void) {
    struct Object *cannon = spawn_object_relative(0, 0, 300, 0, gMarioState->marioObj, MODEL_NONE, bhvCannon);
    SET_BPARAM1(cannon->oBehParams, CHAOS_CODE_BPARAM);
    globalChaosFlags &= ~(1 << gCurrentChaosID);
}

void chaos_fall_damage(void) {
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
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

void chaos_lowfps(void) {
    gLowFPS = TRUE;
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        gLowFPS = FALSE;
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_flipinput(void) {
    gFlipInputs = TRUE;
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        gFlipInputs = FALSE;
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_dimlights(void) {
    gDimLights = TRUE;
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        gDimLights = FALSE;
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_upside_down_camera(void) {
    sFOVState.fovFunc = CAM_FOV_SET_315;
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        sFOVState.fovFunc = CAM_FOV_DEFAULT;
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_no_model_is_mario(void) {
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_mario_kart(void) {
    if (gMarioState->action != ACT_RIDING_KART) {
        spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_KART, bhvKartController);
    }
    globalChaosFlags &= ~(1 << gCurrentChaosID);
}

void chaos_pay_to_move(void) {
    if (gMarioState->numCoins >= 20) {
        print_text(SCREEN_CENTER_X - 150, 180, "PAY 20 COINS TO MOVE");
        if (gPlayer1Controller->buttonPressed & A_BUTTON) {
            gMarioState->numCoins -= 20;
            gHudDisplay.coins -= 20;
            play_sound(SOUND_GENERAL_COIN, gGlobalSoundSource);
            globalChaosFlags &= ~(1 << gCurrentChaosID);
        }
    }
    else {
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_tank_controls(void) {
    gTankControls = TRUE;
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        gTankControls = FALSE;
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_lawmetre(void) {
    if (gLawMetre == FALSE) {
        gLawMetre = TRUE;
        gCrimes = 0.0f;
        gCrimeSpawnTimer = 0;
    }
    
    s32 repeat = gCrimes / 100.0f;

    if (gCrimes > 100.0f) {
        gCrimeSpawnTimer--;
        if (gCrimeSpawnTimer <= 0) {
            struct Object *bombOmb = spawn_object_relative(0x20, 0, 300, 0, gMarioState->marioObj, MODEL_BLACK_BOBOMB, bhvBobomb);
            gCrimeSpawnTimer = 150 / repeat;
        }
    }
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        gLawMetre = FALSE;
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_tinymario(void) {
    if (gTinyMario == FALSE) {
        gTinyMario = TRUE;
    }
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        gTinyMario = FALSE;
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_billboardmario(void) {
    if (gBillboardMario == FALSE) {
        gBillboardMario = TRUE;
    }
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        gBillboardMario = FALSE;
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_veryslippery(void) {
    if (gVerySlippery == FALSE) {
        gVerySlippery = TRUE;
    }
    gChaosCodeTimers[gCurrentChaosID]--;
    if (gChaosCodeTimers[gCurrentChaosID] <= 0) {
        gVerySlippery = FALSE;
        globalChaosFlags &= ~(1 << gCurrentChaosID);
    }
}

void chaos_ccm_rocks_from_volcano(void) {
    if (gCCMVolcanoRocks == FALSE) {
        gCCMVolcanoRocks = TRUE;
    }

    s16 x = (random_u16() % 500) - 250;
    // biased in favour of spawning the rocks in front of mario rather than behind
    s16 z = (random_u16() % 1100) - 250;

    // 9% chance
    if (random_u16() % 100 < 9) {
        spawn_object_relative(0, x, 1000, z + 250, gMarioState->marioObj, MODEL_CCM_ROCK_VOLCANO, bhvRockVolcano);
    }
}

ChaosCode gChaosCodeTable[] = {
    {"Cannon", chaos_cannon, 0, 0, 0},
    {"Fall Damage", chaos_fall_damage, 15, 30, 0},
    {"Trip", chaos_trip, 0, 0, 0},
    {"Upside Down Camera", chaos_upside_down_camera, 10, 20, 0},
    {"Model None Mario", chaos_no_model_is_mario, 10, 20, 0},
    {"Retro Vision", chaos_retro, 15, 30, CODEFLAG_SCREEN},
    {"Blur Vision", chaos_blur, 20, 30, CODEFLAG_SCREEN},
    {"Low FPS", chaos_lowfps, 15, 30, 0},
    {"Mario Kart", chaos_mario_kart, 0, 0, 0},
    {"Pay to Move", chaos_pay_to_move, 0, 0, 0},
    {"Tank Controls", chaos_tank_controls, 15, 30, 0},
    {"Invert Controls", chaos_flipinput, 20, 30, 0},
    {"Dim Lights", chaos_dimlights, 30, 60, 0},
    {"Law Metre", chaos_lawmetre, 60, 90, 0},
    {"Tiny Mario", chaos_tinymario, 20, 35, 0},
    {"Billboard Mario", chaos_billboardmario, 20, 35, 0},
    {"CCM Volcano Rocks", chaos_ccm_rocks_from_volcano, 20, 35, 0},
    {"Very Slippery", chaos_veryslippery, 30, 45, 0},
};

s32 gChaosCodeTimers[sizeof(gChaosCodeTable) / sizeof(ChaosCode)];

void chaos_enable(s32 codeID) {
    globalChaosFlags |= 1 << codeID;
    if (gChaosCodeTable[codeID].flags) {
        for (u32 i = 0; i < sizeof(gChaosCodeTable) / sizeof(ChaosCode); i++) {
            if (gChaosCodeTimers[i] && gChaosCodeTable[i].flags == gChaosCodeTable[codeID].flags) {
                gChaosCodeTimers[i] = 0;
                //globalChaosFlags &= ~(1 << i);
            }
        }
    }
    if (gChaosCodeTable[codeID].timerLow + gChaosCodeTable[codeID].timerHigh) {
        int rand = random_u16() % (gChaosCodeTable[codeID].timerHigh - gChaosCodeTable[codeID].timerLow);
        gChaosCodeTimers[codeID] = (gChaosCodeTable[codeID].timerLow + rand) * 30;
    }
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