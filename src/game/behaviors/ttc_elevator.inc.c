
/**
 * Behavior for bhvTTCElevator.
 */

/**
 * The speed on each setting.
 */
static s8 sTTCElevatorSpeeds[] = {
    /* TTC_SPEED_SLOW    */ 6,
    /* TTC_SPEED_FAST    */ 10,
    /* TTC_SPEED_INSANE  */ 40,
    /* TTC_SPEED_STOPPED */ 0,
};

/**
 * Init function for bhvTTCElevator.
 */
void bhv_ttc_elevator_init(void) {
    // If behParam is nonzero, then move 100 * behParam units. Otherwise default
    // to 500
    f32 peakOffset =
        ((o->oBehParams >> 16) & 0xFFFF) ? 100.0f * ((o->oBehParams >> 16) & 0xFFFF) : 500.0f;

    o->oTTCElevatorPeakY = o->oPosY + peakOffset;
}

/**
 * Update function for bhvTTCElevator.
 */
void bhv_ttc_elevator_update(void) {
    o->oVelY = sTTCElevatorSpeeds[gTTCSpeedSetting] * o->oTTCElevatorDir;

    // This is basically equivalent to posY += velY
    cur_obj_move_using_fvel_and_gravity();

    // Flip directions if the elevator tries to run away
    if (clamp_f32(&o->oPosY, o->oHomeY, o->oTTCElevatorPeakY)) {
        o->oTTCElevatorDir = -o->oTTCElevatorDir;
    }
}
