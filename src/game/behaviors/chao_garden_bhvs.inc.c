struct ObjectHitbox sChaoHitbox = {
    /* interactType:      */ INTERACT_GRABBABLE,
    /* downOffset:        */ 20,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 150,
    /* height:            */ 250,
    /* hurtboxRadius:     */ 140,
    /* hurtboxHeight:     */ 240,
};

enum chao_actions {
    CHAO_ACT_IDLE,
    CHAO_ACT_SIT_DOWN,
    CHAO_ACT_SIT_IDLE,
    CHAO_ACT_SIT_GET_UP,
    CHAO_ACT_LAY_DOWN,
    CHAO_ACT_CRAWING,
    CHAO_ACT_CRAWLING_GET_UP,
    CHAO_ACT_IDLE_LOOK_RIGHT,
    CHAO_ACT_GRABBED,
};

enum chao_anims {
    CHAO_ANIM_IDLE,
    CHAO_ANIM_IDLE_LOOK_RIGHT,
    CHAO_ANIM_SIT,
    CHAO_ANIM_SIT_IDLE,
    CHAO_ANIM_GET_UP_FROM_SITTING,
    CHAO_ANIM_LAY_DOWN,
    CHAO_ANIM_CRAWL,
    CHAO_ANIM_GET_UP_FROM_CRAWL,
};

void bhv_chao_act_idle(void) {
    o->oAnimationIndex = CHAO_ANIM_IDLE;
    o->oHeldState = HELD_FREE;
    if ((o->oTimer != 0) && cur_obj_check_if_at_animation_end()) {
        // 50% chance to do something else
        if (random_u16() % 2 == 0) {
            if (random_u16() % 2 == 0) {
                o->oAction = CHAO_ACT_SIT_DOWN;
            } else {
                o->oAction = CHAO_ACT_LAY_DOWN;
            }
        }
    }
}

void bhv_chao_act_sit_down(void) {
    o->oAnimationIndex = CHAO_ANIM_SIT;

    if ((o->oTimer != 0) && cur_obj_check_if_at_animation_end()) {
        o->oAction = CHAO_ACT_SIT_IDLE;
    }
}

void bhv_chao_act_sit_idle(void) {
    o->oAnimationIndex = CHAO_ANIM_SIT_IDLE;

    if ((o->oTimer != 0) && cur_obj_check_if_at_animation_end()) {
        // 50% chance to do something else
        if (random_u16() % 2 == 0) {
            o->oAction = CHAO_ACT_SIT_GET_UP;
        }
    }
}

void bhv_chao_act_sit_get_up(void) {
    o->oAnimationIndex = CHAO_ANIM_GET_UP_FROM_SITTING;

    if ((o->oTimer != 0) && cur_obj_check_if_at_animation_end()) {
        o->oAction = CHAO_ACT_IDLE;
    }
}

void bhv_chao_act_lay_down(void) {
    o->oAnimationIndex = CHAO_ANIM_LAY_DOWN;

    // Decide a random direction, rotate towards it
    if (o->oTimer < 15) {
        if (o->oTimer == 0) {
            if (cur_obj_lateral_dist_to_home() > 900.0f) {
                o->oMoveAngleYaw = o->oAngleToHome;
            } else {
                o->oMoveAngleYaw = random_u16();
            }
        }
        o->oFaceAngleYaw = (approach_s16_symmetric(o->oFaceAngleYaw, o->oMoveAngleYaw, 0x800));
    }


    if ((o->oTimer != 0) && cur_obj_check_if_at_animation_end()) {
        o->oAction = CHAO_ACT_CRAWING;
    }
}

void bhv_chao_act_crawing(void) {
    o->oAnimationIndex = CHAO_ANIM_CRAWL;
    // Move towards the direction it's facing
    o->oForwardVel = 5.0f;
    // If 2000 units away from home, rotate towards home
    if (cur_obj_lateral_dist_to_home() > 1000.0f || (o->oPosX > 1400)) {
        // Compute the angle to home manually
        o->oMoveAngleYaw = o->oAngleToHome;
    }
    o->oFaceAngleYaw = (approach_s16_symmetric(o->oFaceAngleYaw, o->oMoveAngleYaw, 0x800));

    if ((o->oTimer != 0) && cur_obj_check_if_at_animation_end()) {
        // 10% chance to get up
        if (random_u16() % 10 == 0) {
            o->oAction = CHAO_ACT_CRAWLING_GET_UP;
        }
    }
}

void bhv_chao_act_crawling_get_up(void) {
    o->oAnimationIndex = CHAO_ANIM_GET_UP_FROM_CRAWL;

    if ((o->oTimer != 0) && cur_obj_check_if_at_animation_end()) {
        o->oAction = CHAO_ACT_IDLE;
    }
}

void bhv_chao_act_grabbed(void) {
    o->oAnimationIndex = CHAO_ANIM_IDLE;
    if (o->oTimer != 0 && o->oHeldState == HELD_FREE) {
        o->oAction = CHAO_ACT_IDLE;
        return;
    }
    o->oHeldState = HELD_HELD;
}

void bhv_chao_loop(void) {
    o->oDrawingDistance = 32000.0f;
    cur_obj_init_animation(o->oAnimationIndex);
    if (o->oHeldState == HELD_FREE) {
        cur_obj_become_tangible();
        cur_obj_enable_rendering();
    } else if (o->oHeldState == HELD_HELD) {
        o->oAction = CHAO_ACT_GRABBED;
        cur_obj_disable_rendering();
        cur_obj_become_intangible();
    }
    if(o->oHeldState == HELD_DROPPED) {
        o->oHeldState = HELD_FREE;
    }
        switch (o->oAction) {
            case CHAO_ACT_IDLE:
                bhv_chao_act_idle();
                o->oForwardVel = 0.f;
                break;
            case CHAO_ACT_SIT_DOWN:
                bhv_chao_act_sit_down();
                o->oForwardVel = 0.f;
                break;
            case CHAO_ACT_SIT_IDLE:
                bhv_chao_act_sit_idle();
                o->oForwardVel = 0.f;
                break;
            case CHAO_ACT_SIT_GET_UP:
                bhv_chao_act_sit_get_up();
                o->oForwardVel = 0.f;
                break;
            case CHAO_ACT_LAY_DOWN:
                bhv_chao_act_lay_down();
                o->oForwardVel = 0.f;
                break;
            case CHAO_ACT_CRAWING:
                bhv_chao_act_crawing();
                break;
            case CHAO_ACT_CRAWLING_GET_UP:
                bhv_chao_act_crawling_get_up();
                o->oForwardVel = 0.f;
                break;
            case CHAO_ACT_GRABBED:
                bhv_chao_act_grabbed();
                o->oForwardVel = 0.f;
                break;

        }

    obj_set_hitbox(o, &sChaoHitbox);
    o->oInteractStatus = INT_STATUS_NONE;
    o->oInteractType = INTERACT_GRABBABLE;
    o->oInteractionSubtype = INT_SUBTYPE_HOLDABLE_NPC;

    if (o->oPosY < 1400) {
        o->oPosX = o->oHomeX;
        o->oPosY = o->oHomeY;
        o->oPosZ = o->oHomeZ;
    }
    cur_obj_move_standard(-78);
    cur_obj_update_floor_and_walls();
    o->oDrawingDistance = 32000.0f;
    o->oAngleToHome = cur_obj_angle_to_home();
}