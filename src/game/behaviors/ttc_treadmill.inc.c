
/**
 * Behavior for bhvTTCTreadmill.
 * The first treadmill in processing order is the "master treadmill". It is
 * responsible for playing sounds and calling RNG to control all the other
 * treadmills.
 */

/**
 * Collision models for the different sized treadmills.
 */
static Collision const *sTTCTreadmillCollisionModels[] = {
    ttc_seg7_collision_070152B4,
    ttc_seg7_collision_070153E0,
};

static s16 sTTCTreadmillSpeeds[] = {
    /* TTC_SPEED_SLOW    */ 50,
    /* TTC_SPEED_FAST    */ 2000,
    /* TTC_SPEED_INSANE  */ 16000,
    /* TTC_SPEED_STOPPED */ 0,
};

extern s16 ttc_movtex_tris_big_surface_treadmill[];
extern s16 ttc_movtex_tris_small_surface_treadmill[];

/**
 * The treadmill that plays sounds and controls the others on random setting.
 */
struct Object *sMasterTreadmill;

void bhv_ttc_treadmill_init(void) {
    o->collisionData = segmented_to_virtual(sTTCTreadmillCollisionModels[o->oBehParams2ndByte & TREADMILL_BP_SIZE_MASK]);

    o->oTTCTreadmillBigSurface = segmented_to_virtual(ttc_movtex_tris_big_surface_treadmill);
    o->oTTCTreadmillSmallSurface = segmented_to_virtual(ttc_movtex_tris_small_surface_treadmill);

    *o->oTTCTreadmillBigSurface = *o->oTTCTreadmillSmallSurface = 200;

    sMasterTreadmill = NULL;
}

/**
 * Update function for bhvTTCTreadmill. It calls cur_obj_compute_vel_xz afterward.
 */
void bhv_ttc_treadmill_update(void) {
    if (sMasterTreadmill == o || sMasterTreadmill == NULL) {
        sMasterTreadmill = o;

        cur_obj_play_sound_1(SOUND_ENV_ELEVATOR2);

        if (gTTCSpeedSetting == TTC_SPEED_INSANE) {
            o->oTTCTreadmillSpeed = sTTCTreadmillSpeeds[gTTCSpeedSetting];
            *o->oTTCTreadmillBigSurface = *o->oTTCTreadmillSmallSurface = o->oTTCTreadmillSpeed;
        }
    }

    o->oForwardVel = 0.084f * *o->oTTCTreadmillBigSurface;
}
