enum chao_actions {
    CHAO_ACT_IDLE,
    CHAO_ACT_SIT_DOWN,
    CHAO_ACT_SIT_IDLE,
    CHAO_ACT_SIT_GET_UP,
    CHAO_ACT_LAY_DOWN,
    CHAO_ACT_CRAWING,
    CHAO_ACT_CRAWLING_GET_UP,
    CHAO_ACT_IDLE_LOOK_RIGHT,
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

    if ((o->oTimer != 0) && cur_obj_check_if_at_animation_end()) {
        o->oAction = CHAO_ACT_CRAWING;
    }
}

void bhv_chao_act_crawing(void) {
    o->oAnimationIndex = CHAO_ANIM_CRAWL;

    if ((o->oTimer != 0) && cur_obj_check_if_at_animation_end()) {
        // 50% chance to do something else
        if (random_u16() % 2 == 0) {
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

void bhv_chao_loop(void) {
    cur_obj_init_animation(o->oAnimationIndex);
    switch (o->oAction) {
        case CHAO_ACT_IDLE:
            bhv_chao_act_idle();
            break;
        case CHAO_ACT_SIT_DOWN:
            bhv_chao_act_sit_down();
            break;
        case CHAO_ACT_SIT_IDLE:
            bhv_chao_act_sit_idle();
            break;
        case CHAO_ACT_SIT_GET_UP:
            bhv_chao_act_sit_get_up();
            break;
        case CHAO_ACT_LAY_DOWN:
            bhv_chao_act_lay_down();
            break;
        case CHAO_ACT_CRAWING:
            bhv_chao_act_crawing();
            break;
        case CHAO_ACT_CRAWLING_GET_UP:
            bhv_chao_act_crawling_get_up();
            break;
    }
    
}