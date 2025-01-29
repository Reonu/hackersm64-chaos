
/**
 * Behavior for TTC spinner.
 */

/**
 * Spinner speeds on each setting.
 */
static s16 sTTCSpinnerSpeeds[] = {
    /* TTC_SPEED_SLOW    */ 200,
    /* TTC_SPEED_FAST    */ 600,
    /* TTC_SPEED_INSANE  */ 40000,
    /* TTC_SPEED_STOPPED */ 0,
};

/**
 * Update function for bhvTTCSpinner.
 */
void bhv_ttc_spinner_update(void) {
    o->oAngleVelPitch = sTTCSpinnerSpeeds[gTTCSpeedSetting];

    o->oFaceAnglePitch += o->oAngleVelPitch;
}
