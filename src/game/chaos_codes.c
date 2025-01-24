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
#include "engine/math_util.h"
#include "engine/surface_collision.h"
#include "include/object_constants.h"
#include "src/game/object_list_processor.h"


s32 nextGlobalCodeTimer = 150;
u32 gCurrentChaosID;
u8 gDisableChaos = TRUE; // Debug only
u8 gChaosOffOverride = FALSE; // Use this one for non debug overrides.
u8 gSpamAd;
s16 gSpamCursorX;
s16 gSpamCursorY;
float gCrimes = 0;
s32 gCrimeSpawnTimer;
ChaosCode *gCurrentChaosTable;

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
            spawn_object_relative(0x20, 0, 300, 0, gMarioState->marioObj, MODEL_BOBOMB_COP, bhvBobomb);
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
    if (gCurrentChaosTable[gCurrentChaosID].active == FALSE) {
        gCurrentChaosTable[gCurrentChaosID].active = TRUE;
    }
    gCurrentChaosTable[gCurrentChaosID].timer--;
    if (gCurrentChaosTable[gCurrentChaosID].timer <= 0) {
        gCurrentChaosTable[gCurrentChaosID].timer = 0;
        gCurrentChaosTable[gCurrentChaosID].active = FALSE;
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

void chaos_heave_ho_chaser(void) {
    struct Object *heaveho = spawn_object_relative(0, 0, 900, 0, gMarioState->marioObj, MODEL_HEAVE_HO, bhvHeaveHo);
    heaveho->oSuperHeaveHo = 1;
    heaveho->oHeaveHoLifeTimer = gChaosCodeTable[gCurrentChaosID].timer;
    gChaosCodeTable[gCurrentChaosID].timer = 0;
    gChaosCodeTable[gCurrentChaosID].active = FALSE;
}

void chaos_chain_chomp(void) {
    Vec3f pos;
    pos[0] = gMarioState->pos[0] + 400 * sins(gMarioState->faceAngle[1]);
    pos[1] = gMarioState->pos[1] + 100;
    pos[2] = gMarioState->pos[2] + 400 * coss(gMarioState->faceAngle[1]);
    struct Surface *floor;
    find_floor(pos[0], pos[1], pos[2], &floor);
    if (floor == NULL) {
        u8 attemptCounter = 0;
        while (floor == NULL && attemptCounter < 10) {
            pos[1] += 150;
            find_floor(pos[0], pos[1], pos[2], &floor);
            attemptCounter++;
        }
    }
    if (floor != NULL) {
        struct Object *chainChomp = spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_CHAIN_CHOMP, bhvChaosChainChomp);
        chainChomp->oPosX = pos[0];
        chainChomp->oPosY = pos[1];
        chainChomp->oPosZ = pos[2];
    }
    gChaosCodeTable[gCurrentChaosID].timer = 0;
    gChaosCodeTable[gCurrentChaosID].active = FALSE;
}

void chaos_thwomp(void) {
    struct Object *thwomp = spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_THWOMP, bhvThwomp);
    thwomp->oBehParams = 0x01300000;
    gChaosCodeTable[gCurrentChaosID].timer = 0;
    gChaosCodeTable[gCurrentChaosID].active = FALSE;
}

void chaos_yellow_block(void) {
    if (gMarioState->vel[1] > 0) {
        spawn_object_relative(EXCLAMATION_BOX_BP_COINS_1, 0, 200, 0, gMarioState->marioObj, MODEL_EXCLAMATION_BOX, bhvExclamationBox);

        s16 random = random_u16() % 4;
        switch (random) {
            case 0x00:
            default:
                play_sound(SOUND_NEW_FUNI1, gGlobalSoundSource);
                break;
            case 0x01:
                play_sound(SOUND_NEW_FUNI2, gGlobalSoundSource);
                break;
            case 0x02:
                play_sound(SOUND_NEW_FUNI3, gGlobalSoundSource);
                break;
            case 0x03:
                play_sound(SOUND_NEW_FUNI4, gGlobalSoundSource);
                break; 
        }

        gChaosCodeTable[gCurrentChaosID].timer = 0;
        gChaosCodeTable[gCurrentChaosID].active = FALSE;
    }
}

void chaos_ttc_upwarp(void) {
    int upwarpPos = gMarioState->pos[1];
    upwarpPos ^= 0b100000000000;
    gMarioState->pos[1] = upwarpPos;
    gTTCChaosTable[gCurrentChaosID].timer = 0;
    gTTCChaosTable[gCurrentChaosID].active = FALSE;
}

struct Object *sMirrorGhost;

void chaos_mirrorghost(void) {
    if (gCurrentChaosTable[gCurrentChaosID].active == FALSE) {
        gCurrentChaosTable[gCurrentChaosID].active = TRUE;
    }
    if (sMirrorGhost == NULL) {
        sMirrorGhost = spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_SHADOW_MARIO, bhvMovementGhost);
        play_sound(SOUND_MARIO_HELLO, gGlobalSoundSource);
    }
    gCurrentChaosTable[gCurrentChaosID].timer--;
    if (gCurrentChaosTable[gCurrentChaosID].timer <= 0) {
        gCurrentChaosTable[gCurrentChaosID].timer = 0;
        gCurrentChaosTable[gCurrentChaosID].active = FALSE;
        if (sMirrorGhost) {
            obj_mark_for_deletion(sMirrorGhost);
        }
        sMirrorGhost = NULL;
    }
}

// i stole this from stackexchange
void shuffle_positions(f32 *array, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
          size_t j = i + random_u16() / (U16_MAX / (n - i) + 1);
          f32 t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

void chaos_swap_positions(void) {
    f32 objectPositions[OBJECT_POOL_CAPACITY][3];
    int i;
    u16 objCount = 0;
    for (i = 0; i < OBJECT_POOL_CAPACITY; i++) {
        struct Object *curObj = &gObjectPool[i];
        if (curObj == NULL) {
            break;
        }
        objectPositions[i][0] = curObj->oPosX;
        objectPositions[i][1] = curObj->oPosY;
        objectPositions[i][2] = curObj->oPosZ;
        objCount++;
    }

    shuffle_positions(*objectPositions, objCount);

    
    for (i = 0; i < OBJECT_POOL_CAPACITY; i++) {
        struct Object *curObj = &gObjectPool[i];
        if (curObj == NULL) {
            break;
        }
        curObj->oPosX = objectPositions[i][0];
        curObj->oPosY = objectPositions[i][1];
        curObj->oPosZ = objectPositions[i][2];
    }

    gMarioState->pos[0] = gMarioState->marioObj->oPosX;
    gMarioState->pos[1] = gMarioState->marioObj->oPosY;
    gMarioState->pos[2] = gMarioState->marioObj->oPosZ;

    gChaosCodeTable[gCurrentChaosID].timer = 0;
    gChaosCodeTable[gCurrentChaosID].active = FALSE;
}

void chaos_billboard(void) {
    int i;
    u8 shouldBillboard = 1;

    gCurrentChaosTable[gCurrentChaosID].timer--;
    if (gCurrentChaosTable[gCurrentChaosID].timer <= 0) {
        gCurrentChaosTable[gCurrentChaosID].timer = 0;
        gCurrentChaosTable[gCurrentChaosID].active = FALSE;
        shouldBillboard = 0;
    }

    for (i = 0; i < OBJECT_POOL_CAPACITY; i++) {
        struct Object *curObj = &gObjectPool[i];
        if (curObj == NULL) {
            break;
        }
        if (shouldBillboard) {
            curObj->header.gfx.node.flags |= GRAPH_RENDER_BILLBOARD;
        }
        else {
            curObj->header.gfx.node.flags &= ~GRAPH_RENDER_BILLBOARD;
        }
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
    {"No Speed Cap", chaos_generic, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Super Jumps", chaos_generic, 10, 20, 0,   /*ignore these*/ 0, 0},
    {"Heave Ho Chaser", chaos_heave_ho_chaser, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Strong Punch KB", chaos_generic, 30, 60, 0,   /*ignore these*/ 0, 0},
    {"Automatic Wallkicks", chaos_generic, 30, 60, 0,   /*ignore these*/ 0, 0},
    {"Chain Chomp", chaos_chain_chomp, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Thwomp", chaos_thwomp, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Yellow Block on Jump", chaos_yellow_block, 30, 60, 0,   /*ignore these*/ 0, 0},
    {"Mirror Ghost", chaos_mirrorghost, 30, 60, 0,   /*ignore these*/ 0, 0},
    {"Weird Audio", chaos_generic, 30, 45, 0,   /*ignore these*/ 0, 0},
    {"Swap Positions", chaos_swap_positions, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Billboard Everything", chaos_generic, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Fast Goombas", chaos_generic, 30, 60, 0,  /*ignore these*/ 0, 0},
    {"Bilerp", chaos_generic, 30, 60, 0,  /*ignore these*/ 0, 0},
};

ChaosCode gCCMChaosTable[] = {
    {"CCM Volcano Rocks", chaos_ccm_rocks_from_volcano, 20, 35, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gBoBChaosTable[] = {
    {"BoB Water Bombs", chaos_generic, 20, 35, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gTTCChaosTable[] = {
    {"TTC Upwarp", chaos_ttc_upwarp, 20, 35, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gSSLChaosTable[] = {
    {"SSL Blizzard", chaos_generic, 30, 60, 0,   /*ignore these*/ 0, 0},
    {"SSL Quicksand Magnet", chaos_generic, 30, 60, 0,   /*ignore these*/ 0, 0},
};

void chaos_enable(ChaosCode *table, s32 codeID, s32 tableSize) {
    append_puppyprint_log("Chaos effect added: %s\n", table[codeID].name);
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
}

void add_global_chaos_code(ChaosCode *table, s32 tableSize) {
    u16 chosenCode = random_u16() % tableSize;
    chaos_enable(table, chosenCode, tableSize);
}

ChaosCode *chaos_level_table(s32 levelID, s32 *size) {
    switch (levelID) {
    case LEVEL_CCM:
        *size = sizeof(gCCMChaosTable) / sizeof(ChaosCode);
        return gCCMChaosTable;
    case LEVEL_BOB:
        *size = sizeof(gBoBChaosTable) / sizeof(ChaosCode);
        return gBoBChaosTable;
    case LEVEL_TTC:
        *size = sizeof(gTTCChaosTable) / sizeof(ChaosCode);
        return gTTCChaosTable;
    case LEVEL_SSL:
        if (gCurrAreaIndex == 1) {
            *size = sizeof(gSSLChaosTable) / sizeof(ChaosCode);
            return gSSLChaosTable;
        }
        // fallthrough
    default:
        *size = sizeof(gChaosCodeTable) / sizeof(ChaosCode);
        return gChaosCodeTable;
    }
}

void update_chaos_code_effects(void) {
    s32 size;
    gCurrentChaosTable = chaos_level_table(gCurrLevelNum, &size);

    if (gCurrentChaosTable != gChaosCodeTable) {
        
        for (s32 i = 0; i < size; i++) {
            if (gCurrentChaosTable[i].timer) {
                gCurrentChaosID = i;
                (gCurrentChaosTable[i].func)();
            }
        }
    }
    gCurrentChaosTable = gChaosCodeTable;
    for (u32 i = 0; i < sizeof(gChaosCodeTable) / sizeof(ChaosCode); i++) {
        if (gChaosCodeTable[i].timer) {
            gCurrentChaosID = i;
            (gChaosCodeTable[i].func)();
        }
    }
}

void chaos_clear_level(ChaosCode *table, s32 size) {
    for (int i = 0; i < size; i++) {
        table[i].timer = 0;
        table[i].active = FALSE;
    }
}

void global_chaos_code_handler(void) {
    update_chaos_code_effects();

    
    if (gDisableChaos || gChaosOffOverride || gCurrLevelNum == LEVEL_CHAO_GARDEN) {
        return;
    }

    nextGlobalCodeTimer--;
    if (nextGlobalCodeTimer <= 0) {
        s32 size;
        int rand = random_u16() % 100;
        if (rand >= 66) {
            gCurrentChaosTable = chaos_level_table(gCurrLevelNum, &size);
        } else {
            gCurrentChaosTable = gChaosCodeTable;
            size = sizeof(gChaosCodeTable) / sizeof(ChaosCode);
        }
        add_global_chaos_code(gCurrentChaosTable, size);
        nextGlobalCodeTimer = 150 + (random_u16() % 600);
    }
}