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


s32 nextGlobalCodeTimer = 150;
u32 gCurrentChaosID;
u8 gDisableChaos = TRUE; // Debug only
u8 gChaosOffOverride = FALSE; // Use this one for non debug overrides.
u8 gSpamAd;
s16 gSpamCursorX;
s16 gSpamCursorY;
float gCrimes = 0;
s32 gCrimeSpawnTimer;

extern OSViMode VI;

void chaos_cannon(void) {
    struct Object *cannon = spawn_object_relative(0, 0, 300, 0, gMarioState->marioObj, MODEL_NONE, bhvCannon);
    SET_BPARAM1(cannon->oBehParams, CHAOS_CODE_BPARAM);
    gChaosCodeTable[gCurrentChaosID].timer = 0;
    gChaosCodeTable[gCurrentChaosID].active = FALSE;
}

// Not final, just there to have a different func
void chaos_trip(void) {
    gMarioState->action = ACT_HARD_BACKWARD_GROUND_KB;
    gChaosCodeTable[gCurrentChaosID].timer = 0;
    gChaosCodeTable[gCurrentChaosID].active = FALSE;
}

void chaos_retro(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
        change_vi(&VI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    }
    gChaosCodeTable[gCurrentChaosID].timer--;
    if (gChaosCodeTable[gCurrentChaosID].timer <= 0) {
        change_vi(&VI, SCREEN_WIDTH, SCREEN_HEIGHT);
        gChaosCodeTable[gCurrentChaosID].timer = 0;
        gChaosCodeTable[gCurrentChaosID].active = FALSE;
    }
}

void chaos_blur(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
        set_fb_effect_type(FBE_EFFECT_MULT);
    }
    set_motion_blur(32);
    gChaosCodeTable[gCurrentChaosID].timer--;
    if (gChaosCodeTable[gCurrentChaosID].timer <= 0) {
        gChaosCodeTable[gCurrentChaosID].timer = 0;
        gChaosCodeTable[gCurrentChaosID].active = FALSE;
    }
}

void chaos_upside_down_camera(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
    }
    sFOVState.fovFunc = CAM_FOV_SET_315;
    gChaosCodeTable[gCurrentChaosID].timer--;
    if (gChaosCodeTable[gCurrentChaosID].timer <= 0) {
        sFOVState.fovFunc = CAM_FOV_DEFAULT;
        gChaosCodeTable[gCurrentChaosID].timer = 0;
        gChaosCodeTable[gCurrentChaosID].active = FALSE;
    }
}

void chaos_mario_kart(void) {
    if (gMarioState->action != ACT_RIDING_KART) {
        spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_KART, bhvKartController);
    }
    gChaosCodeTable[gCurrentChaosID].timer = 0;
    gChaosCodeTable[gCurrentChaosID].active = FALSE;
}

void chaos_pay_to_move(void) {
    if (gMarioState->numCoins >= 20) {
        print_text(SCREEN_CENTER_X - 150, 180, "PAY 20 COINS TO MOVE");
        if (gPlayer1Controller->buttonPressed & A_BUTTON) {
            gMarioState->numCoins -= 20;
            gHudDisplay.coins -= 20;
            play_sound(SOUND_GENERAL_COIN, gGlobalSoundSource);
            gChaosCodeTable[gCurrentChaosID].timer = 0;
            gChaosCodeTable[gCurrentChaosID].active = FALSE;
        }
    }
    else {
        gChaosCodeTable[gCurrentChaosID].timer = 0;
        gChaosCodeTable[gCurrentChaosID].active = FALSE;
    }
}

void chaos_lawmetre(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
        gCrimes = 0.0f;
        gCrimeSpawnTimer = 0;
    }
    
    s32 repeat = gCrimes / 100.0f;

    if (gCrimes > 100.0f) {
        gCrimeSpawnTimer--;
        if (gCrimeSpawnTimer <= 0) {
            struct Object *bombOmb = spawn_object_relative(0x20, 0, 300, 0, gMarioState->marioObj, MODEL_BOBOMB_COP, bhvBobomb);
            gCrimeSpawnTimer = 150 / repeat;
        }
    }
    gChaosCodeTable[gCurrentChaosID].timer--;
    if (gChaosCodeTable[gCurrentChaosID].timer <= 0) {
        gChaosCodeTable[gCurrentChaosID].timer = 0;
        gChaosCodeTable[gCurrentChaosID].active = FALSE;
    }
}

void chaos_generic(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
    }
    gChaosCodeTable[gCurrentChaosID].timer--;
    if (gChaosCodeTable[gCurrentChaosID].timer <= 0) {
        gChaosCodeTable[gCurrentChaosID].timer = 0;
        gChaosCodeTable[gCurrentChaosID].active = FALSE;
    }
}

void chaos_ccm_rocks_from_volcano(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
    }

    s16 x = (random_u16() % 500) - 250;
    // biased in favour of spawning the rocks in front of mario rather than behind
    s16 z = (random_u16() % 1100) - 250;

    // 9% chance
    if (random_u16() % 100 < 9) {
        spawn_object_relative(0, x, 1000, z + 250, gMarioState->marioObj, MODEL_CCM_ROCK_VOLCANO, bhvRockVolcano);
    }
    gChaosCodeTable[gCurrentChaosID].timer--;
    if (gChaosCodeTable[gCurrentChaosID].timer <= 0) {
        gChaosCodeTable[gCurrentChaosID].timer = 0;
        gChaosCodeTable[gCurrentChaosID].active = FALSE;
    }
}

void chaos_randomize_coin_colors(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
    }
    
    gChaosCodeTable[gCurrentChaosID].timer--;
    if (gChaosCodeTable[gCurrentChaosID].timer <= 0) {
        gChaosCodeTable[gCurrentChaosID].timer = 0;
        gChaosCodeTable[gCurrentChaosID].active = FALSE;
    }
}

void chaos_ad(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
        gSpamAd = random_u16() % 5;
        gSpamCursorX = 48;
        gSpamCursorY = 48;
    }
}

void chaos_generic_bob(void) {
    if (gBoBChaosTable[gCurrentChaosID].active == FALSE) {
        gBoBChaosTable[gCurrentChaosID].active = TRUE;
    }
    gBoBChaosTable[gCurrentChaosID].timer--;
    if (gBoBChaosTable[gCurrentChaosID].timer <= 0) {
        gBoBChaosTable[gCurrentChaosID].timer = 0;
        gBoBChaosTable[gCurrentChaosID].active = FALSE;
    }
}

void chaos_ttc_upwarp(void) {
    int upwarpPos = gMarioState->pos[1];
    upwarpPos ^= 0b100000000000;
    gMarioState->pos[1] = upwarpPos;
    gTTCChaosTable[gCurrentChaosID].timer = 0;
    gTTCChaosTable[gCurrentChaosID].active = FALSE;
}

void chaos_generic_ttc(void) {
    if (gTTCChaosTable[gCurrentChaosID].active == FALSE) {
        gTTCChaosTable[gCurrentChaosID].active = TRUE;
    }
    gTTCChaosTable[gCurrentChaosID].timer--;
    if (gTTCChaosTable[gCurrentChaosID].timer <= 0) {
        gTTCChaosTable[gCurrentChaosID].timer = 0;
        gTTCChaosTable[gCurrentChaosID].active = FALSE;
    }
}

ChaosCode gChaosCodeTable[] = {
    {"Cannon", chaos_cannon, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Fall Damage", chaos_generic, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Trip", chaos_trip, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Upside Down Camera", chaos_upside_down_camera, 10, 20, 0,   /*ignore these*/ 0, 0},
    {"Model None Mario", chaos_generic, 10, 20, 0,   /*ignore these*/ 0, 0},
    {"Retro Vision", chaos_retro, 15, 30, CODEFLAG_SCREEN,   /*ignore these*/ 0, 0},
    {"Blur Vision", chaos_blur, 20, 30, CODEFLAG_SCREEN,   /*ignore these*/ 0, 0},
    {"Mario Kart", chaos_mario_kart, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Pay to Move", chaos_pay_to_move, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Tank Controls", chaos_generic, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Invert Controls", chaos_generic, 20, 30, 0,   /*ignore these*/ 0, 0},
    {"Dim Lights", chaos_generic, 30, 60, 0,   /*ignore these*/ 0, 0},
    {"Law Metre", chaos_lawmetre, 60, 90, 0,   /*ignore these*/ 0, 0},
    {"Tiny Mario", chaos_generic, 20, 35, 0,   /*ignore these*/ 0, 0},
    {"Billboard Mario", chaos_generic, 20, 35, 0,   /*ignore these*/ 0, 0},
    {"Very Slippery", chaos_generic, 30, 45, 0,   /*ignore these*/ 0, 0},
    {"All Quicksand", chaos_generic, 30, 45, 0,   /*ignore these*/ 0, 0},
    {"Mario Sounds Scream", chaos_generic, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Randomize Coin Colors", chaos_generic, 30, 45, 0,   /*ignore these*/ 0, 0},
    {"Coin Cutscenes", chaos_generic, 30, 45, 0,   /*ignore these*/ 0, 0},
    {"All jumps are triple", chaos_generic, 15, 45, 0,   /*ignore these*/ 0, 0},
    {"Delete Nearby Objects", chaos_generic, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Invert Dive and Kick", chaos_generic, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Live Mario Reaction", chaos_generic, 30, 45, 0,   /*ignore these*/ 0, 0},
    {"Ad Spam", chaos_ad, 0, 0, CODEFLAG_SCREEN,   /*ignore these*/ 0, 0},
};

ChaosCode gCCMChaosTable[] = {
    {"CCM Volcano Rocks", chaos_ccm_rocks_from_volcano, 20, 35, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gBoBChaosTable[] = {
    {"BoB Water Bombs", chaos_generic_bob, 20, 35, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gTTCChaosTable[] = {
    {"TTC Upwarp", chaos_ttc_upwarp, 20, 35, 0,   /*ignore these*/ 0, 0},
};

void chaos_enable(ChaosCode *table, s32 codeID, s32 tableSize) {
    if (table[codeID].flags) {
        for (s32 i = 0; i < tableSize; i++) {
            if (table[i].timer && table[i].flags == table[codeID].flags) {
                table[i].timer = 0;
            }
        }
    }
    if (table[codeID].timerLow + table[codeID].timerHigh) {
        int rand = random_u16() % (table[codeID].timerHigh - table[codeID].timerLow);
        table[codeID].timer = (table[codeID].timerLow + rand) * 30;
    } else {
        table[codeID].timer = 1;
    }
    append_puppyprint_log("Chaos effect added: %s", table[codeID].name);
}

void add_global_chaos_code(ChaosCode *table, s32 tableSize) {
    u16 chosenCode = random_u16() % tableSize;
    chaos_enable(table, chosenCode, tableSize);
}

ChaosCode *chaos_level_table(s32 *size) {
    switch (gCurrLevelNum) {
    case LEVEL_CCM:
        *size = sizeof(gCCMChaosTable) / sizeof(ChaosCode);
        return gCCMChaosTable;
    case LEVEL_BOB:
        *size = sizeof(gBoBChaosTable) / sizeof(ChaosCode);
        return gBoBChaosTable;
    case LEVEL_TTC:
        *size = sizeof(gTTCChaosTable) / sizeof(ChaosCode);
        return gTTCChaosTable;
    default:
        *size = sizeof(gChaosCodeTable) / sizeof(ChaosCode);
        return gChaosCodeTable;
    }
}

void update_chaos_code_effects(void) {
    s32 size;
    ChaosCode *table = chaos_level_table(&size);

    if (table != gChaosCodeTable) {
        
        for (s32 i = 0; i < size; i++) {
            if (table[i].timer) {
                gCurrentChaosID = i;
                (table[i].func)();
            }
        }
    }
    for (u32 i = 0; i < sizeof(gChaosCodeTable) / sizeof(ChaosCode); i++) {
        if (gChaosCodeTable[i].timer) {
            gCurrentChaosID = i;
            (gChaosCodeTable[i].func)();
        }
    }
}

void global_chaos_code_handler(void) {
    update_chaos_code_effects();

    
    if (gDisableChaos || gChaosOffOverride || gCurrLevelNum == LEVEL_CHAO_GARDEN) {
        return;
    }

    nextGlobalCodeTimer--;
    if (nextGlobalCodeTimer <= 0) {
        ChaosCode *table;
        s32 size;
        int rand = random_u16() % 100;
        if (rand >= 66) {
            table = chaos_level_table(&size);
        } else {
            table = gChaosCodeTable;
            size = sizeof(gChaosCodeTable) / sizeof(ChaosCode);
        }
        add_global_chaos_code(table, size);
        nextGlobalCodeTimer = 150 + (random_u16() % 600);
    }
}