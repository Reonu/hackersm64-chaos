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
#include "include/seq_ids.h"
#include "src/game/interaction.h"
#include "sound_init.h"
#include "mario.h"
#include "game/spawn_sound.h"


s32 nextGlobalCodeTimer = 150;
u32 gCurrentChaosID;
u8 gDisableChaos = TRUE; // Debug only
u8 gChaosOffOverride = FALSE; // Use this one for non debug overrides.
u8 gSpamAd;
u8 gPovActive;
struct Object *gPovEnemy;
s16 gPovPrevMode = -1;
s16 gSpamCursorX;
s16 gSpamCursorY;
s16 gWaterLevelTarget;
float gCrimes = 0;
s32 gCrimeSpawnTimer;
f32 gHeaveHoStrength;
ChaosCode *gCurrentChaosTable;

extern OSViMode VI;

u8 buffer_code_until_grounded_out_of_water() {
    return !((gMarioState->action & ACT_GROUP_MASK) >= ACT_GROUP_AIRBORNE && 
    (gMarioState->action & ACT_GROUP_MASK) < (ACT_HOLD_JUMP & ACT_ID_MASK)) &&
    (gMarioState->action & ACT_GROUP_MASK) != ACT_GROUP_SUBMERGED;
}

void disable_current_code(void) {
    gCurrentChaosTable[gCurrentChaosID].timer = 0;
    gCurrentChaosTable[gCurrentChaosID].active = FALSE;
}

u8 current_code_update_timer(void) {
    gCurrentChaosTable[gCurrentChaosID].timer--;
    if (gCurrentChaosTable[gCurrentChaosID].timer <= 0) {
        disable_current_code();
        return TRUE;
    }
    return FALSE;
}

void chaos_cannon(void) {
    struct Object *cannon = spawn_object_relative(0, 0, 300, 0, gMarioState->marioObj, MODEL_NONE, bhvCannon);
    SET_BPARAM1(cannon->oBehParams, CHAOS_CODE_BPARAM);
    gChaosCodeTable[gCurrentChaosID].timer = 0;
    gChaosCodeTable[gCurrentChaosID].active = FALSE;
}

// Not final, just there to have a different func
void chaos_trip(void) {
    if ((gMarioState->action & ACT_FLAG_SWIMMING) == 0) {
        gMarioState->action = ACT_HARD_BACKWARD_GROUND_KB;
    }
    gChaosCodeTable[gCurrentChaosID].timer = 0;
    gChaosCodeTable[gCurrentChaosID].active = FALSE;
}


void chaos_spinflower(void) {
    if ((gMarioState->action & ACT_FLAG_SWIMMING) == 0) {
        play_sound(SOUND_MARIO_TWIRL_BOUNCE, gGlobalSoundSource);
        gMarioState->action = ACT_TWIRLING;
        gMarioState->vel[1] = 300.0f;
    }
    gChaosCodeTable[gCurrentChaosID].timer = 0;
    gChaosCodeTable[gCurrentChaosID].active = FALSE;
}

void chaos_retro(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
        change_vi(&VI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    }
    if (current_code_update_timer()) {
        change_vi(&VI, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

void chaos_blur(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
        set_fb_effect_type(FBE_EFFECT_MULT);
    }
    current_code_update_timer();
}

void chaos_upside_down_camera(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
    }
    sFOVState.fovFunc = CAM_FOV_SET_315;
    if (current_code_update_timer()) {
        sFOVState.fovFunc = CAM_FOV_DEFAULT;
    }
}

void chaos_mario_kart(void) {
    if (buffer_code_until_grounded_out_of_water()) {
        if (gMarioState->action != ACT_RIDING_KART) {
            spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_KART, bhvKartController);
        }
        disable_current_code();
    }
}

void chaos_pay_to_move(void) {
    if (gMarioState->numCoins >= 20) {
        print_text(SCREEN_CENTER_X - 150, 180, "PAY 20 COINS TO MOVE");
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
        if (gPlayer1Controller->buttonPressed & A_BUTTON) {
            gMarioState->numCoins -= 20;
            gHudDisplay.coins -= 20;
            play_sound(SOUND_GENERAL_COIN, gGlobalSoundSource);
            disable_current_code();
        }
    }
    else {
        disable_current_code();
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
    current_code_update_timer();
}

void chaos_generic(void) {
    if (gCurrentChaosTable[gCurrentChaosID].active == FALSE) {
        gCurrentChaosTable[gCurrentChaosID].active = TRUE;
    }
    current_code_update_timer();
}

void chaos_wdw_heaveho(void) {
    if (gCurrentChaosTable[gCurrentChaosID].active == FALSE) {
        gCurrentChaosTable[gCurrentChaosID].active = TRUE;
        gHeaveHoStrength = absf(random_f32_around_zero(300.0f));
    }
    current_code_update_timer();
}

void chaos_enemypov(void) {
    if (gCurrentChaosTable[gCurrentChaosID].active == FALSE) {
        gCurrentChaosTable[gCurrentChaosID].active = TRUE;
        gPovEnemy = NULL; 
        gPovActive = FALSE;
    }
    if (gPovEnemy == NULL) {
        for (int i = 0; i < OBJECT_POOL_CAPACITY; i++) {
            BehaviorScript *beh = gObjectPool[i].behavior;
            if (beh == segmented_to_virtual(bhvGoomba) ||
                beh == segmented_to_virtual(bhvBobomb) ||
                beh == segmented_to_virtual(bhvChuckya) ||
                beh == segmented_to_virtual(bhvBobomb)) {
                if (gObjectPool[i].activeFlags & ACTIVE_FLAG_ACTIVE && dist_between_objects(&gObjectPool[i], gMarioState->marioObj) < 3000.0f) {
                    gPovEnemy = &gObjectPool[i];   
                    break; 
                }
            }
        }
    }

    if (gPovActive == FALSE) {
        if (gPovEnemy && gMarioState->marioObj) {
            float dist = dist_between_objects(gPovEnemy, gMarioState->marioObj);

            if (dist < 1500.0f) {
                gPovActive = TRUE;
            }
        }
    }

    if (gPovActive) {
        if (!(gPovEnemy->activeFlags & ACTIVE_FLAG_ACTIVE)) {
            gCurrentChaosTable[gCurrentChaosID].active = FALSE;
            gCurrentChaosTable[gCurrentChaosID].timer = 0;
            gPovActive = FALSE;
        }
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
    current_code_update_timer();
}

void chaos_ccm_spawn_penguins(void) {
    gCurrentChaosTable[gCurrentChaosID].active = TRUE;
    if (gCurrentChaosTable[gCurrentChaosID].timer % 20 == 0) {
        spawn_object_relative(0, 0, 300, 0, gMarioState->marioObj, MODEL_PENGUIN, bhvSmallPenguin);
    }
    current_code_update_timer();    
}

void chaos_randomize_coin_colors(void) {
    if (gChaosCodeTable[gCurrentChaosID].active == FALSE) {
        gChaosCodeTable[gCurrentChaosID].active = TRUE;
    }
    current_code_update_timer();
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
    disable_current_code();
}

void chaos_thwomp(void) {
    struct Object *thwomp = spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_THWOMP, bhvThwomp);
    thwomp->oBehParams = 0x01300000;
    if (gCurrLevelNum == LEVEL_WF) {
        obj_set_model(thwomp, MODEL_THWOMP_BETA);
    }
    disable_current_code();
}

void chaos_spring(void) {
    if (gMarioState->vel[1] < 0) {
        struct Object *spring = spawn_object_relative(0, 0, -100, 0, gMarioState->marioObj, MODEL_SPRING, bhvSpring);
        u16 color = random_u16() % 3;
        switch (color) {
            case 0:
                spring->oBehParams = 0x01000000;
                break;
            case 1:
            default:
                spring->oBehParams = 0x01010000;
                break;
            case 2:
                spring->oBehParams = 0x01020000;
                break;
        }
        disable_current_code();
    }
}

void chaos_amp(void) {
    struct Object *amp = spawn_object_relative(0, 30, 0, 0, gMarioState->marioObj, MODEL_AMP, bhvHomingAmp);
    amp->oBehParams = HOMING_AMP_BP_CHAOS;
    disable_current_code();
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

        disable_current_code();
    }
}

void chaos_ttc_upwarp(void) {
    int upwarpPos = gMarioState->pos[1];
    upwarpPos ^= 0b100000000000;
    gMarioState->pos[1] = upwarpPos;
    disable_current_code();
}

void chaos_ttc_medusa_heads(void) {
    if (gGlobalTimer % 45 == 0) {
        spawn_object_abs_with_rot(gMarioState->marioObj, 0, MODEL_MEDUSA_HEAD, bhvMedusaHead, 
            gMarioState->pos[0] + -1000*sins(gCamera->yaw + 0x4000),
            gMarioState->pos[1] + random_u16()%300,
            gMarioState->pos[2] + -1000*coss(gCamera->yaw + 0x4000),
            0,
            gCamera->yaw + 0x4000, 0);
    }

    current_code_update_timer();
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
    if (current_code_update_timer()) {
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

    disable_current_code();
}

//unused
void chaos_billboard(void) {
    int i;
    u8 shouldBillboard = 1;

    gCurrentChaosTable[gCurrentChaosID].timer--;
    if (gCurrentChaosTable[gCurrentChaosID].timer <= 0) {
        disable_current_code();
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

void chaos_random_cap(void) {
    u32 capFlag;
    const BehaviorScript *script;
    switch (random_u16()%3) {
        case 0:
            capFlag = MARIO_WING_CAP;
            script = bhvWingCap;
        break;
        case 1:
            capFlag = MARIO_METAL_CAP;
            script = bhvMetalCap;
        break;
        case 2:
            capFlag = MARIO_VANISH_CAP;
            script = bhvVanishCap;
        break;
    }

    
    u16 capMusic = 0;
    u16 capTime = 0;

    if (gMarioState->action != ACT_GETTING_BLOWN && capFlag != 0) {
        struct Object *obj = spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_MARIOS_METAL_CAP, script);
        gMarioState->interactObj = obj;
        obj->oInteractStatus = INT_STATUS_INTERACTED;

        gMarioState->flags &= ~MARIO_CAP_ON_HEAD & ~MARIO_CAP_IN_HAND;
        gMarioState->flags |= capFlag;

        switch (capFlag) {
            case MARIO_VANISH_CAP: capTime =  600; capMusic = SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP  ); break;
            case MARIO_METAL_CAP:  capTime =  600; capMusic = SEQUENCE_ARGS(4, SEQ_EVENT_METAL_CAP); break;
            case MARIO_WING_CAP:   capTime = 1800; capMusic = SEQUENCE_ARGS(4, SEQ_EVENT_POWERUP  ); break;
        }

        if (capTime > gMarioState->capTimer) {
            gMarioState->capTimer = capTime;
        }

        if ((gMarioState->action & ACT_FLAG_IDLE) || gMarioState->action == ACT_WALKING) {
            gMarioState->flags |= MARIO_CAP_IN_HAND;
            set_mario_action(gMarioState, ACT_PUTTING_ON_CAP, 0);
        } else {
            gMarioState->flags |= MARIO_CAP_ON_HEAD;
        }

        play_sound(SOUND_MENU_STAR_SOUND, gMarioState->marioObj->header.gfx.cameraToObject);
        play_sound(SOUND_MARIO_HERE_WE_GO, gMarioState->marioObj->header.gfx.cameraToObject);

        if (capMusic != 0) {
            play_cap_music(capMusic);
        }
    }

    disable_current_code();
}

u32 attack_object(struct Object *obj, s32 interaction);

void chaos_koopa_shell(void) {
    struct Object *obj =
        spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_KOOPA_SHELL, bhvKoopaShell);
    gMarioState->interactObj = obj;
    gMarioState->usedObj = obj;
    gMarioState->riddenObj = obj;

    attack_object(obj, INT_HIT_FROM_ABOVE);
    update_mario_sound_and_camera(gMarioState);
    play_shell_music();
    mario_drop_held_object(gMarioState);

    //! Puts Mario in ground action even when in air, making it easy to
    // escape air actions into crouch slide (shell cancel)
    set_mario_action(gMarioState, ACT_RIDING_SHELL_GROUND, 0);

    disable_current_code();
}

void chaos_squish_mario(void) {
    gMarioState->squishTimer = 15;
    current_code_update_timer();
}

void chaos_next_long_jump_gp(void) {
    gCurrentChaosTable[gCurrentChaosID].active = TRUE;
    if (gMarioState->action == ACT_GROUND_POUND_LAND) {
        disable_current_code();
    }
}

void chaos_random_jump(void) {
    if (!((gMarioState->action & ACT_GROUP_MASK) >= ACT_GROUP_AIRBORNE && (gMarioState->action & ACT_GROUP_MASK) < (ACT_HOLD_JUMP & ACT_ID_MASK))) {
        gMarioState->action = ACT_SIDE_FLIP;
        gMarioState->vel[1] = 48.0f;
        disable_current_code();
    }
}

void chaos_ethel(void) {
    static u8 started = 0;
    Vec3f pos;
    pos[0] = gCamera->pos[0] - 1000 * sins(gCamera->yaw);
    pos[1] = gCamera->pos[1];
    pos[2] = gCamera->pos[2] - 1000 * coss(gCamera->yaw);
    struct Object *ethel;
    if (!started) {
        ethel = spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_ETHEL_THE_CAT, bhvEthelTheCat);
        started = 1;
        ethel->oPosX = pos[0];
        ethel->oPosY = pos[1];
        ethel->oPosZ = pos[2];
        ethel->oFaceAngleYaw = 0;
        ethel->oFaceAngleRoll = 0;
        ethel->oFaceAnglePitch = 0;
    } else {
        if (gChaosCodeTable[GLOBAL_CHAOS_ETHEL_THE_CAT].timer == 1) {
            started = 0;
            disable_current_code();
            return;
        }
    }
    current_code_update_timer();
}

void chaos_dab(void) {
    drop_and_set_mario_action(gMarioState, ACT_DAB, 0);
    disable_current_code();
}

void chaos_wdw_water(void) {
    if (gCurrentChaosTable[gCurrentChaosID].active == FALSE) {
        gCurrentChaosTable[gCurrentChaosID].active = TRUE;
        s32 min;
        s32 max;
        if (gCurrAreaIndex == 1) {
            min = 40;
            max = 2700;
        } else {
            min = 0;
            max = 2635;
        }
        gWaterLevelTarget = random_u16() % (max - min);
        if (gCurrAreaIndex == 2) {
            gWaterLevelTarget -= max;
        } else {
            gWaterLevelTarget -= min;
        }
        append_puppyprint_log("water level changing to %d\n", gWaterLevelTarget);
    }
    if (gEnvironmentLevels[0] != gWaterLevelTarget) {
        gEnvironmentLevels[0] = approach_s32(gEnvironmentLevels[0], gWaterLevelTarget, 10, 10);
        cur_obj_play_sound_1(SOUND_ENV_WATER_DRAIN);
    } else {
        disable_current_code();
    }
    if (gEnvironmentRegions == NULL) {
        disable_current_code();
        return;
    }

}

void chaos_lll_super_burning(void) {
    gMarioState->marioObj->oMarioBurnTimer = 100;
    if (buffer_code_until_grounded_out_of_water()) {
        if (gMarioState->action != ACT_BURNING_GROUND && gMarioState->action != ACT_BURNING_JUMP && gMarioState->action != ACT_BURNING_FALL) {
            gMarioState->action = ACT_BURNING_GROUND;
        }
    }
    gCurrentChaosTable[gCurrentChaosID].active = TRUE;
    current_code_update_timer();
}

void chaos_ttc_super_speed(void) {
    gCurrentChaosTable[gCurrentChaosID].active = TRUE;
    gTTCSpeedSetting = TTC_SPEED_INSANE;
    if (current_code_update_timer()) {
        gTTCSpeedSetting = TTC_SPEED_FAST;
    };
}

void chaos_ttc_stopped_speed(void) {
    gCurrentChaosTable[gCurrentChaosID].active = TRUE;
    gTTCSpeedSetting = TTC_SPEED_STOPPED;
    if (current_code_update_timer()) {
        gTTCSpeedSetting = TTC_SPEED_FAST;
    };
}

void chaos_bob_koopa_storm(void) {
    gCurrentChaosTable[gCurrentChaosID].active = TRUE;
    if (gCurrentChaosTable[gCurrentChaosID].timer % 20 == 0) {
        spawn_object_relative(0, 0, 300, 0, gMarioState->marioObj, MODEL_KOOPA_WITHOUT_SHELL, bhvKoopa);
    }
    current_code_update_timer();
}

void chaos_bob_nuke_omb(void) {
    if (gCurrentChaosTable[gCurrentChaosID].active == TRUE) {
        gMarioState->action = ACT_DEATH_ON_STOMACH;
        set_mario_animation(gMarioState, MARIO_ANIM_DYING_ON_STOMACH);
        gMarioState->marioObj->header.gfx.animInfo.animFrame = 40;
        if (current_code_update_timer()) {
            gMarioState->marioObj->header.gfx.animInfo.animFrame = 36;
        }

        if (gCurrentChaosTable[gCurrentChaosID].timer == 221) {
            
            play_sound(SOUND_NEW_NUKE_EXPLOSION, gGlobalSoundSource);
        }
    }
    else {
        gCurrentChaosTable[gCurrentChaosID].timer = 222;
    }
    
}

void chaos_sl_swap_mario_xz(void) {
    f32 marioX = gMarioState->pos[0];
    gMarioState->pos[0] = gMarioState->pos[2];
    gMarioState->pos[2] = marioX;
    disable_current_code();
}

void chaos_sl_pharaoh_curse(void) {

    if (buffer_code_until_grounded_out_of_water()) {
        if (gCurrentChaosTable[gCurrentChaosID].timer > 900) {
            gCurrentChaosTable[gCurrentChaosID].timer = 900;
        }
        if (gCurrentChaosTable[gCurrentChaosID].timer > 870) {
            set_mario_action(gMarioState, ACT_WAITING_FOR_DIALOG, 0);
        }
        if (gCurrentChaosTable[gCurrentChaosID].timer == 870) {
            gCurrentChaosTable[gCurrentChaosID].active = TRUE;
            gMarioState->quicksandDepth = 120;
            set_mario_action(gMarioState, ACT_IDLE, 0);
        }  
        current_code_update_timer();

        if (gMarioState->pos[1] > gMarioState->floorHeight + 30) {
            disable_current_code();
        }
    }
    else if (gCurrentChaosTable[gCurrentChaosID].timer <= 870) {
        current_code_update_timer();
        if (gMarioState->pos[1] > gMarioState->floorHeight + 30) {
            disable_current_code();
        }
    }
}

void chaos_ssl_insta_snow(void) {
    if (buffer_code_until_grounded_out_of_water()) {
        play_sound(SOUND_MARIO_OOOF2, gMarioState->marioObj->header.gfx.cameraToObject);
        gMarioState->particleFlags |= PARTICLE_MIST_CIRCLE;
        drop_and_set_mario_action(gMarioState, ACT_HEAD_STUCK_IN_GROUND + random_u16()%3, 0);

        disable_current_code();
    }
}

void chaos_hmc_boulder(void) {
    Vec3f pos;
    pos[0] = gMarioState->pos[0] + 2000 * sins(gMarioState->faceAngle[1]);
    pos[1] = gMarioState->pos[1] + 100;
    pos[2] = gMarioState->pos[2] + 2000 * coss(gMarioState->faceAngle[1]);
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
        struct Object *boulder = spawn_object_relative(0, 0, 0, 0, gMarioState->marioObj, MODEL_HMC_ROLLING_ROCK, bhvBigBoulder);
        boulder->oPosX = pos[0];
        boulder->oPosY = pos[1];
        boulder->oPosZ = pos[2];
        boulder->oMoveAngleYaw += DEGREES(180);
        boulder->oBehParams = HMC_BOULDER_BP_CHAOS;
    }    
    disable_current_code();
}

void chaos_thi_lakitu_throw_lakitu(void) {
    struct Object *lakitu = spawn_object_relative(0, 0, 1000, 0, gMarioState->marioObj, MODEL_ENEMY_LAKITU, bhvEnemyLakitu);
    lakitu->oEnemyLakituThrowsLakitus = 1;
    disable_current_code();
}

void chaos_thi_area_change(void) {
    if (gCurrAreaIndex < 3) {
        change_area((gCurrAreaIndex)%2 + 1);
    }
    disable_current_code();
}

void chaos_thi_bowling_balls(void) {
    if (gGlobalTimer % 30 == 0) {
        spawn_object_relative(0, random_u16()%2000 - 1000, 500, random_u16()%2000 - 1000, gMarioState->marioObj, MODEL_BOWLING_BALL, bhvBowlingBall);
    }
    current_code_update_timer();
}

ChaosCode gChaosCodeTable[] = {
    {"Cannon", chaos_cannon, 100, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Fall Damage", chaos_generic, 100, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Trip", chaos_trip, 100, 0, 0, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
    {"Upside Down Camera", chaos_upside_down_camera, 100, 10, 20, CODEFLAG_CAMERA,   /*ignore these*/ 0, 0},
    {"Model None Mario", chaos_generic, 100, 10, 20, 0,   /*ignore these*/ 0, 0},
    {"Retro Vision", chaos_retro, 50, 15, 30, CODEFLAG_SCREEN,   /*ignore these*/ 0, 0},
    {"Blur Vision", chaos_blur, 30, 20, 30, CODEFLAG_SCREEN,   /*ignore these*/ 0, 0},
    {"Mario Kart", chaos_mario_kart, 100, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Pay to Move", chaos_pay_to_move, 100, 0, 0, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
    {"Tank Controls", chaos_generic, 100, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Invert Controls", chaos_generic, 100, 20, 30, 0,   /*ignore these*/ 0, 0},
    {"Dim Lights", chaos_generic, 30, 30, 60, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
    {"Law Metre", chaos_lawmetre, 100, 60, 90, 0,   /*ignore these*/ 0, 0},
    {"Tiny Mario", chaos_generic, 100, 20, 35, 0,   /*ignore these*/ 0, 0},
    {"Billboard Mario", chaos_generic, 100, 20, 35, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
    {"Very Slippery", chaos_generic, 100, 30, 45, 0,   /*ignore these*/ 0, 0},
    {"All Quicksand", chaos_generic, 80, 25, 40, 0,   /*ignore these*/ 0, 0},
    {"Mario Sounds Scream", chaos_generic, 100, 15, 30, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
    {"Randomize Coin Colors", chaos_generic, 100, 30, 45, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
    {"Coin Cutscenes", chaos_generic, 100, 30, 45, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
    {"All jumps are triple", chaos_generic, 100, 15, 45, 0,   /*ignore these*/ 0, 0},
    {"Delete Nearby Objects", chaos_generic, 100, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Invert Dive and Kick", chaos_generic, 100, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Live Mario Reaction", chaos_generic, 100, 30, 45, 0,   /*ignore these*/ 0, 0},
    {"Ad Spam", chaos_ad, 100, 0, 0, CODEFLAG_SCREEN,   /*ignore these*/ 0, 0},
    {"No Speed Cap", chaos_generic, 100, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Super Jumps", chaos_generic, 100, 10, 20, 0,   /*ignore these*/ 0, 0},
    {"Heave Ho Chaser", chaos_heave_ho_chaser, 100, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Strong Punch KB", chaos_generic, 100, 45, 90, 0,   /*ignore these*/ 0, 0},
    {"Automatic Wallkicks", chaos_generic, 100, 30, 60, 0,   /*ignore these*/ 0, 0},
    {"Chain Chomp", chaos_chain_chomp, 100, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Thwomp", chaos_thwomp, 100, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Yellow Block on Jump", chaos_yellow_block, 100, 30, 60, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
    {"Mirror Ghost", chaos_mirrorghost, 100, 30, 60, 0,   /*ignore these*/ 0, 0},
    {"Weird Audio", chaos_generic, 100, 30, 45, CODEFLAG_AUDIO,   /*ignore these*/ 0, 0},
    {"Swap Positions", chaos_swap_positions, 100, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"Billboard Everything", chaos_generic, 100, 15, 30, 0,   /*ignore these*/ 0, 0},
    {"Fast Enemies", chaos_generic, 100, 30, 60, 0,  /*ignore these*/ 0, 0},
    {"Bilerp", chaos_generic, 100, 30, 60, CODEFLAG_MINOR,  /*ignore these*/ 0, 0},
    {"Random Cap", chaos_random_cap, 100, 0, 0, CODEFLAG_MINOR,  /*ignore these*/ 0, 0},
    {"Enemy PoV", chaos_enemypov, 100, 0, 0, 0,  /*ignore these*/ 0, 0},
    {"Koopa Shell", chaos_koopa_shell, 100, 0, 0, 0,  /*ignore these*/ 0, 0},
    {"Squish Mario", chaos_squish_mario, 100, 4, 8, 0,  /*ignore these*/ 0, 0},
    {"Ortho Cam", chaos_generic, 100, 30, 60, 0,  /*ignore these*/ 0, 0},
    {"Random Spin", chaos_spinflower, 100, 0, 0, CODEFLAG_MINOR,  /*ignore these*/ 0, 0},
    {"Long Jump GP", chaos_next_long_jump_gp, 100, 1, 2, 0,  /*ignore these*/ 0, 0},
    {"Random Jump", chaos_random_jump, 100, 1, 2, CODEFLAG_MINOR,  /*ignore these*/ 0, 0},
    {"Mario Random Bank", chaos_generic, 100, 30, 60, CODEFLAG_AUDIO,  /*ignore these*/ 0, 0},
    {"Chuckya on Object Deletion", chaos_generic, 100, 30, 60, 0,  /*ignore these*/ 0, 0},
    {"FoV based on fvel", chaos_generic, 100, 30, 60, CODEFLAG_CAMERA,  /*ignore these*/ 0, 0},
    {"All Ceilings Hangable", chaos_generic, 100, 60, 120, CODEFLAG_MINOR,  /*ignore these*/ 0, 0},
    {"Sudden Reonu Spring", chaos_spring, 100, 1, 2, CODEFLAG_MINOR,  /*ignore these*/ 0, 0},
    {"Amp", chaos_amp, 100, 1, 2, CODEFLAG_MINOR,  /*ignore these*/ 0, 0},
    {"Coins Remove Coins", chaos_generic, 100, 20, 30, 0,  /*ignore these*/ 0, 0},
    {"Random object scale", chaos_generic, 100, 30, 60, 0,  /*ignore these*/ 0, 0},
    {"Objects flee Mario", chaos_generic, 100, 30, 60, 0,  /*ignore these*/ 0, 0},
    {"Ethel the Cat", chaos_ethel, 100, 14, 15, 0,  /*ignore these*/ 0, 0}, // the code lasts for the duration of time the cat takes to despawn, do not change
    {"Mario Gravitation", chaos_generic, 60, 20, 30, 0,  /*ignore these*/ 0, 0},
    {"Hurricane", chaos_generic, 30, 15, 20, 0,  /*ignore these*/ 0, 0},
    {"Mario Dabs", chaos_dab, 100, 1, 2, CODEFLAG_MINOR,  /*ignore these*/ 0, 0},
};

ChaosCode gCCMChaosTable[] = {
    {"CCM Volcano Rocks", chaos_ccm_rocks_from_volcano, 100, 20, 35, 0,   /*ignore these*/ 0, 0},
    {"CCM Spawn Penguins", chaos_ccm_spawn_penguins, 100, 5, 7, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gWFChaosTable[] = {
    {"WF Betah Invasion", chaos_generic, 100, 30, 60, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gBoBChaosTable[] = {
    {"BoB Water Bombs", chaos_generic, 100, 20, 35, 0,   /*ignore these*/ 0, 0},
    {"BoB Koopa Storm", chaos_bob_koopa_storm, 100, 5, 10, 0,   /*ignore these*/ 0, 0},
    {"BoB Nuke Omb", chaos_bob_nuke_omb, 100, 5, 10, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gTTCChaosTable[] = {
    {"TTC Upwarp", chaos_ttc_upwarp, 100, 20, 35, 0,   /*ignore these*/ 0, 0},
    {"TTC Medusa Heads", chaos_ttc_medusa_heads, 100, 30, 45, 0,   /*ignore these*/ 0, 0},
    {"TTC Objects Catch Mario", chaos_generic, 100, 20, 45, 0,   /*ignore these*/ 0, 0},
    {"TTC Super Speed", chaos_ttc_super_speed, 100, 10, 20, 0,   /*ignore these*/ 0, 0},
    {"TTC Stopped Speed", chaos_ttc_stopped_speed, 100, 20, 45, 0,   /*ignore these*/ 0, 0},

};

ChaosCode gLLLChaosTable[] = {
    {"LLL Super Burning", chaos_lll_super_burning, 20, 10, 15, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gSSLChaosTable[] = {
    {"SSL Blizzard", chaos_generic, 100, 30, 60, 0,   /*ignore these*/ 0, 0},
    {"SSL Quicksand Magnet", chaos_generic, 100, 30, 60, 0,   /*ignore these*/ 0, 0},
    {"SSL Insta Snow", chaos_ssl_insta_snow, 100, 30, 60, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
};

ChaosCode gSLChaosTable[] = {
    {"SL Swap Mario XZ", chaos_sl_swap_mario_xz, 50, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"SL Pharaoh's Curse", chaos_sl_pharaoh_curse, 100, 30, 31, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gWDWChaosTable[] = {
    {"Random Water Level", chaos_wdw_water, 100, 0, 0, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
    {"Heave Ho Strength", chaos_wdw_heaveho, 100, 20, 30, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gTHIChaosTable[] = {
    {"THI Vertical Wind", chaos_generic, 60, 5, 10, 0,   /*ignore these*/ 0, 0},
    {"THI Lakitu Throw Lakitu", chaos_thi_lakitu_throw_lakitu, 100, 5, 10, 0,   /*ignore these*/ 0, 0},
    {"THI Area Change", chaos_thi_area_change, 100, 0, 0, 0,   /*ignore these*/ 0, 0},
    {"THI Bowling Balls", chaos_thi_bowling_balls, 100, 10, 20, 0,   /*ignore these*/ 0, 0},
};

ChaosCode gHMCChaosTable[] = {
    {"HMC Random Boulder", chaos_hmc_boulder, 100, 0, 0, CODEFLAG_MINOR,   /*ignore these*/ 0, 0},
    {"HMC Boulders Face Mario", chaos_generic, 100, 15, 45, 0,   /*ignore these*/ 0, 0},
    {"HMC Random Door Warps", chaos_generic, 100, 40, 70, 0,   /*ignore these*/ 0, 0},
};

void chaos_enable(ChaosCode *table, s32 codeID, s32 tableSize) {
    append_puppyprint_log("Chaos effect added: %s\n", table[codeID].name);
    if (table[codeID].flags && table[codeID].flags != CODEFLAG_MINOR) {
        for (s32 i = 0; i < tableSize; i++) {
            if (table[i].timer && table[i].flags == table[codeID].flags) {
                table[i].timer = 1;
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

u16 gPrevChosenCode;

void add_global_chaos_code(ChaosCode *table, s32 tableSize) {
    gPrevChosenCode = random_u16() % tableSize;
    chaos_enable(table, gPrevChosenCode, tableSize);
}

ChaosCode *chaos_level_table(s32 levelID, s32 *size) {
    switch (levelID) {
    case LEVEL_CCM:
        *size = sizeof(gCCMChaosTable) / sizeof(ChaosCode);
        return gCCMChaosTable;
    case LEVEL_BOB:
        *size = sizeof(gBoBChaosTable) / sizeof(ChaosCode);
        return gBoBChaosTable;
    case LEVEL_WF:
        *size = sizeof(gWFChaosTable) / sizeof(ChaosCode);
        return gWFChaosTable;        
    case LEVEL_LLL:
        *size = sizeof(gLLLChaosTable) / sizeof(ChaosCode);
        return gLLLChaosTable;
    case LEVEL_SL:
        *size = sizeof(gSLChaosTable) / sizeof(ChaosCode);
        return gSLChaosTable;
    case LEVEL_TTC:
        *size = sizeof(gTTCChaosTable) / sizeof(ChaosCode);
        return gTTCChaosTable;
    case LEVEL_WDW:
        *size = sizeof(gWDWChaosTable) / sizeof(ChaosCode);
        return gWDWChaosTable;
    case LEVEL_THI:
        *size = sizeof(gTHIChaosTable) / sizeof(ChaosCode);
        return gTHIChaosTable;
    case LEVEL_HMC:
        *size = sizeof(gHMCChaosTable) / sizeof(ChaosCode);
        return gHMCChaosTable;        
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
        int rand;
        
        tryAgain:
        rand = random_u16() % 100;
        if (rand >= 66) {
            gCurrentChaosTable = chaos_level_table(gCurrLevelNum, &size);
        } else {
            gCurrentChaosTable = gChaosCodeTable;
            size = sizeof(gChaosCodeTable) / sizeof(ChaosCode);
        }
        if ((random_u16() % 100) <= gCurrentChaosTable->probability) {
            add_global_chaos_code(gCurrentChaosTable, size);
        } else {
            goto tryAgain;
        }
        if (gCurrentChaosTable[gPrevChosenCode].flags == CODEFLAG_MINOR) {
            nextGlobalCodeTimer = (random_u16() % 200);
        } else {
            nextGlobalCodeTimer = 150 + (random_u16() % 600);
        }
    }
}