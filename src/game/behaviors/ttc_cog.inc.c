
/**
 * Behavior for bhvTTCCog. This includes both the hexagonal cogs and the
 * triangular prisms that rotate around the y axis.
 */

/**
 * Collision model for hexagon and triangle, respectively.
 */
static Collision const *sTTCCogCollisionModels[] = {
    ttc_seg7_collision_07015584,
    ttc_seg7_collision_07015650,
};

/**
 * The direction that the cog moves on non-random settings.
 */
static s8 sTTCCogDirections[] = { 1, -1 };

/**
 * The speed that the cog moves on the slow and fast settings, respectively.
 */
static s16 sTTCCogNormalSpeeds[] = { 200, 400, 4000};

/**
 * Init function for bhvTTCCog.
 */
void bhv_ttc_cog_init(void) {
    o->collisionData = segmented_to_virtual(
        sTTCCogCollisionModels[(o->oBehParams2ndByte & TTC_COG_BP_SHAPE_MASK) >> 1]);
    o->oTTCCogDir = sTTCCogDirections[o->oBehParams2ndByte & TTC_COG_BP_DIR_MASK];

    if (GET_BPARAM1(o->oBehParams) == 1) {
        obj_scale_xyz(o, 2.0f, 1.0f, 2.0f);
    }
}

/**
 * Update function for bhvTTCCog.
 */
void bhv_ttc_cog_update(void) {
    switch (gTTCSpeedSetting) {
        case TTC_SPEED_SLOW:
        case TTC_SPEED_FAST:
            o->oTTCCogSpeed = sTTCCogNormalSpeeds[gTTCSpeedSetting];
            break;

        case TTC_SPEED_INSANE:
            o->oTTCCogSpeed = sTTCCogNormalSpeeds[gTTCSpeedSetting];

        case TTC_SPEED_STOPPED:
            break;
    }

    if (GET_BPARAM1(o->oBehParams) == 1) {
        o->oTTCCogSpeed = 1200;
    }

    o->oAngleVelYaw = (s32)(o->oTTCCogSpeed * o->oTTCCogDir);
    o->oFaceAngleYaw += o->oAngleVelYaw;
}
