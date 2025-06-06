// thi_top.inc.c

static struct SpawnParticlesInfo sThiTopPuffs = {
    /* behParam:        */ 0,
    /* count:           */ 30,
    /* model:           */ MODEL_WHITE_PARTICLE_SMALL,
    /* offsetY:         */ 0,
    /* forwardVelBase:  */ 40,
    /* forwardVelRange: */ 0,
    /* velYBase:        */ 20,
    /* velYRange:       */ 40,
    /* gravity:         */ 252,
    /* dragStrength:    */ 30,
    /* sizeBase:        */ 20.0f,
    /* sizeRange:       */ 0.0f,
};

void bhv_thi_huge_island_top_loop(void) {
    if (gTHIWaterDrained & 1) {
        if (o->oTimer == 0) {
            gEnvironmentRegions[18] = 1800;
        }
        cur_obj_hide();
    } else {
        load_object_collision_model();
    }
}

void bhv_thi_tiny_island_top_loop(void) {
    if (!(gTHIWaterDrained & 1)) {
        if (o->oAction == 0) {
            if (o->oDistanceToMario < 500.0f && gMarioStates[0].action == ACT_GROUND_POUND_LAND) {
                o->oAction++;
                cur_obj_spawn_particles(&sThiTopPuffs);
                spawn_triangle_break_particles(20, MODEL_DIRT_ANIMATION, 0.3f, 3);
                cur_obj_play_sound_2(SOUND_GENERAL_ACTIVATE_CAP_SWITCH);
                cur_obj_hide();
            }
        } else {
            if (o->oTimer < 50) {
                gEnvironmentRegions[18]--;
                cur_obj_play_sound_1(SOUND_ENV_WATER_DRAIN);
            } else {
                gTHIWaterDrained |= 1;
                play_puzzle_jingle();
                o->oAction++;
            }
        }
    } else {
        if (o->oTimer == 0) {
            gEnvironmentRegions[18] = 1800;
        }
        cur_obj_hide();
    }
}
