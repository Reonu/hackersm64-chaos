
/**
 * Behavior for bhvWigglerHead and bhvWigglerBody.
 * The bhvWigglerHead object controls the wiggler's behavior, and physically manifests
 * as the wiggler's head. The bhvWigglerBody objects represent the 3 tail body
 * parts, numbered 1 closest to the head, and 3 at the end of the tail.
 * Processing order is bhvWigglerHead, then bhvWigglerBody 1, 2, then 3.
 */

/**
 * Hitbox for wiggler's non-head body parts.
 */
static struct ObjectHitbox sWigglerBodyPartHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 99, // never decreases
    /* numLootCoins:      */ 0,
    /* radius:            */ 20,
    /* height:            */ 20,
    /* hurtboxRadius:     */ 20,
    /* hurtboxHeight:     */ 10,
};

/**
 * Hitbox for wiggler's head.
 */
static struct ObjectHitbox sWigglerHitbox = {
    /* interactType:      */ INTERACT_BOUNCE_TOP,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 3,
    /* health:            */ 4,
    /* numLootCoins:      */ 0,
    /* radius:            */ 60,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 30,
    /* hurtboxHeight:     */ 40,
};

/**
 * Attack handler for wiggler while in the walking action.
 */
static u8 sWigglerAttackHandlers[] = {
    /* ATTACK_PUNCH:                 */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_KICK_OR_TRIP:          */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_FROM_ABOVE:            */ ATTACK_HANDLER_SPECIAL_WIGGLER_JUMPED_ON,
    /* ATTACK_GROUND_POUND_OR_TWIRL: */ ATTACK_HANDLER_SPECIAL_WIGGLER_JUMPED_ON,
    /* ATTACK_FAST_ATTACK:           */ ATTACK_HANDLER_KNOCKBACK,
    /* ATTACK_FROM_BELOW:            */ ATTACK_HANDLER_KNOCKBACK,
};

/**
 * Target speed while walking when wiggler has health 1, 2, 3, and 4.
 */
static f32 sWigglerSpeeds[] = { 2.0f, 40.0f, 30.0f, 16.0f };

/**
 * Update function for bhvWigglerBody.
 * Set object position and angle based on wiggler segment data and avoid falling
 * through the floor.
 * Tangible if the wiggler is not in the shrinking action, but does nothing on
 * attack.
 */
void bhv_wiggler_body_part_update(void) {
    Vec3f d;
    struct ChainSegment *segment = &o->parentObj->oWigglerSegments[o->oBehParams2ndByte];

    cur_obj_scale(o->parentObj->header.gfx.scale[0]);

    o->oFaceAnglePitch = segment->angle[0];
    o->oFaceAngleYaw = segment->angle[1];

    // TODO: What is this for?
    f32 posOffset = -37.5f * o->header.gfx.scale[0];
    d[1] = posOffset * coss(o->oFaceAnglePitch) - posOffset;
    f32 dxz = posOffset * sins(o->oFaceAnglePitch);
    d[0] = dxz * sins(o->oFaceAngleYaw);
    d[2] = dxz * coss(o->oFaceAngleYaw);

    vec3f_sum(&o->oPosVec, segment->pos, d);

    if (o->oPosY < o->parentObj->oWigglerFallThroughFloorsHeight) {
        //! Since position is recomputed each frame, tilting the wiggler up
        //  while on the ground could cause the tail segments to clip through
        //  the floor
        o->oPosY -= 30.0f;
        cur_obj_update_floor_height();
        if (o->oFloorHeight > o->oPosY) { // TODO: Check ineq swap
            o->oPosY = o->oFloorHeight;
        }
    }

    segment->pos[1] = o->oPosY;

    // Inherit walking animation speed from wiggler
    cur_obj_init_animation_with_accel_and_sound(0, o->parentObj->oWigglerWalkAnimSpeed);
    if (o->parentObj->oWigglerWalkAnimSpeed == 0.0f) {
        cur_obj_reverse_animation();
    }

    if (o->parentObj->oAction == WIGGLER_ACT_SHRINK) {
        cur_obj_become_intangible();
    } else {
        obj_check_attacks(&sWigglerBodyPartHitbox, o->oAction);
    }
}

/**
 * Initialize the segment data and spawn the body part objects.
 */
void wiggler_init_segments(void) {
    s32 i;
    struct Object *bodyPart;
    struct ChainSegment *segments = mem_pool_alloc(gObjectMemoryPool, WIGGLER_NUM_SEGMENTS * sizeof(struct ChainSegment));

    if (segments != NULL) {
        // Each segment represents the global position and orientation of each
        // object. Segment 0 represents the wiggler's head, and segment i>0
        // represents body part i.
        o->oWigglerSegments = segments;
        for (i = 0; i < WIGGLER_NUM_SEGMENTS; i++) {
            chain_segment_init(segments + i);

            vec3f_copy((segments + i)->pos, &o->oPosVec);

            (segments + i)->angle[0] = o->oFaceAnglePitch;
            (segments + i)->angle[1] = o->oFaceAngleYaw;
        }

        o->header.gfx.animInfo.animFrame = -1;

        // Spawn each body part
        for (i = 1; i < WIGGLER_NUM_SEGMENTS; i++) {
            bodyPart =
                spawn_object_relative(i, 0, 0, 0, o, MODEL_WIGGLER_BODY, bhvWigglerBody);
            if (bodyPart != NULL) {
                obj_init_animation_with_sound(bodyPart, wiggler_seg5_anims_0500C874, 0);
                bodyPart->header.gfx.animInfo.animFrame = (23 * i) % 26 - 1;
            }
        }

        o->oAction = WIGGLER_ACT_WALK;
        cur_obj_unhide();
    }

    o->oHealth = 4; // This fixes Wiggler reading UB on his first frame of his acceleration, as his health is not set.
}

/**
 * Update the tail to reflect changes in the head's yaw and pitch, and ensure
 * that the distance between parts is exactly the intended distance.
 * Since these positions are completely recomputed each frame, it is not possible
 * for a body part to get stuck on geometry and separate from the rest of the
 * body.
 */
 void wiggler_update_segments(void) {
    struct ChainSegment *prevBodyPart;
    struct ChainSegment *bodyPart;
    Vec3f d;
    s16 dpitch, dyaw;
    f32 dxz;
    s32 i;
    f32 segmentLength = (35.0f * o->header.gfx.scale[0]);

    for (i = 1; i < WIGGLER_NUM_SEGMENTS; i++) {
        prevBodyPart = &o->oWigglerSegments[i - 1];
        bodyPart = &o->oWigglerSegments[i];

        vec3_diff(d, bodyPart->pos, prevBodyPart->pos);

        // As the head turns, propagate this rotation backward if the difference
        // is more than 45 degrees
        dyaw = atan2s(-d[2], -d[0]) - prevBodyPart->angle[1];
        clamp_s16(&dyaw, -0x2000, 0x2000);
        bodyPart->angle[1] = prevBodyPart->angle[1] + dyaw;

        // As the head tilts, propagate the tilt backward
        dxz = sqrtf(sqr(d[0]) + sqr(d[2]));
        dpitch = atan2s(dxz, d[1]) - prevBodyPart->angle[0];
        clamp_s16(&dpitch, -0x2000, 0x2000);
        bodyPart->angle[0] = prevBodyPart->angle[0] + dpitch;

        // Set the body part's position relative to the previous body part's
        // position, using the current body part's angles. This means that the
        // head can rotate up to 45 degrees without the body moving
        bodyPart->pos[1] = segmentLength * sins(bodyPart->angle[0]) + prevBodyPart->pos[1];
        dxz = segmentLength * coss(bodyPart->angle[0]);
        bodyPart->pos[0] = prevBodyPart->pos[0] - dxz * sins(bodyPart->angle[1]);
        bodyPart->pos[2] = prevBodyPart->pos[2] - dxz * coss(bodyPart->angle[1]);
    }
}

/**
 * Show text if necessary. Then walk toward mario if not at full health, and
 * otherwise wander in random directions.
 * If attacked by mario, enter either the jumped on or knockback action.
 */
static void wiggler_act_walk(void) {

    cur_obj_scale(2.0f);

    o->oWigglerWalkAnimSpeed = 0.06f * o->oForwardVel;

    // Update text if necessary
    if (o->oWigglerTextStatus < WIGGLER_TEXT_STATUS_COMPLETED_DIALOG) {
        if (o->oWigglerTextStatus == WIGGLER_TEXT_STATUS_AWAIT_DIALOG) {
            seq_player_lower_volume(SEQ_PLAYER_LEVEL, 60, 40);
            o->oWigglerTextStatus = WIGGLER_TEXT_STATUS_SHOWING_DIALOG;
        }

        // If Mario is positioned below the wiggler, assume he entered through the
        // lower cave entrance, so don't display text.
        if (gMarioObject->oPosY < o->oPosY) {
            o->oWigglerTextStatus = WIGGLER_TEXT_STATUS_COMPLETED_DIALOG;
        }

        else if (cur_obj_update_dialog_with_cutscene(
            MARIO_DIALOG_LOOK_UP, DIALOG_FLAG_NONE, CUTSCENE_DIALOG, DIALOG_150)) {
            spawn_default_star(0.0f, 1228.0f, 0.0f);
            o->oWigglerTextStatus = WIGGLER_TEXT_STATUS_COMPLETED_DIALOG;
            }
    } else {
        obj_forward_vel_approach(sWigglerSpeeds[o->oHealth - 1], 1.0f);

        if (o->oWigglerWalkAwayFromWallTimer != 0) {
            o->oWigglerWalkAwayFromWallTimer--;
        } else {
            if (o->oDistanceToMario >= 25000.0f) {
                // If >1200 away from home, turn to home
                o->oWigglerTargetYaw = o->oAngleToMario;
            }

            if (obj_bounce_off_walls_edges_objects(&o->oWigglerTargetYaw)) {
                //! If the wiggler could self-intersect, or intersect a different
                //  non-mario object, this could potentially be used to force
                //  the wiggler to walk straight - past his usual radius
                o->oWigglerWalkAwayFromWallTimer = random_linear_offset(30, 30);
            } else {
                if (o->oHealth < 4) {
                    o->oWigglerTargetYaw = o->oAngleToMario;
                } else if (o->oWigglerTimeUntilRandomTurn != 0) {
                    o->oWigglerTimeUntilRandomTurn--;
                } else {
                    o->oWigglerTargetYaw = o->oMoveAngleYaw + 0x4000 * (s16) random_sign();
                    o->oWigglerTimeUntilRandomTurn = random_linear_offset(30, 50);
                }
            }
        }

        // If moving at high speeds, could overflow. But can't reach such speeds
        // in practice
        s16 yawTurnSpeed = (s16)(30.0f * o->oForwardVel);
        cur_obj_rotate_yaw_toward(o->oWigglerTargetYaw, yawTurnSpeed);
        obj_face_yaw_approach(o->oMoveAngleYaw, 2 * yawTurnSpeed);

        obj_face_pitch_approach(0, 0x320);
    }
}
/**
 * Squish and unsquish, then show text and enter either the walking or shrinking
 * action.
 */
static void wiggler_act_jumped_on(void) {
    // Text to show on first, second, and third attack.
    s32 attackText[3] = { DIALOG_152, DIALOG_168, DIALOG_151 };

    // Shrink until the squish speed becomes 0, then unisquish
    if (approach_f32_ptr(&o->oWigglerSquishSpeed, 0.0f, 0.05f)) {
        // Note that 4 is the default scale
        approach_f32_ptr(&o->header.gfx.scale[1], 4.0f, 0.2f);
    } else {
        o->header.gfx.scale[1] -= o->oWigglerSquishSpeed;
    }

    // Wait for a second after unsquishing, then show text and either shrink (if
    // defeated) or go back to walking
    if (o->header.gfx.scale[1] >= 4.0f) {
        if (o->oTimer > 30) {
            if (cur_obj_update_dialog_with_cutscene(MARIO_DIALOG_LOOK_UP, 
                DIALOG_FLAG_NONE, CUTSCENE_DIALOG, attackText[o->oHealth - 2])) {
                // Because we don't want the wiggler to disappear after being
                // defeated, we leave its health at 1
                if (--o->oHealth == 1) {
                    o->oAction = WIGGLER_ACT_SHRINK;
                    cur_obj_become_intangible();
                } else {
                    o->oAction = WIGGLER_ACT_WALK;
                    o->oMoveAngleYaw = o->oFaceAngleYaw;

                    if (o->oHealth == 2) {
                        cur_obj_play_sound_2(SOUND_OBJ_WIGGLER_JUMP);
                        o->oForwardVel = 10.0f;
                        o->oVelY = 70.0f;
                    }
                }
            }
        }
    } else {
        o->oTimer = 0;
    }

    obj_check_attacks(&sWigglerHitbox, o->oAction);
}

/**
 * Decelerate to a stop and then enter the walk action.
 */
static void wiggler_act_knockback(void) {
    if (o->oVelY > 0.0f) {
        o->oFaceAnglePitch -= o->oVelY * 30.0f;
    } else {
        obj_face_pitch_approach(0, 0x190);
    }

    if (obj_forward_vel_approach(0.0f, 1.0f) && o->oFaceAnglePitch == 0) {
        o->oAction = WIGGLER_ACT_WALK;
        o->oMoveAngleYaw = o->oFaceAngleYaw;
    }

    obj_check_attacks(&sWigglerHitbox, o->oAction);
}

/**
 * Shrink, then spawn the star and enter the fall through floor action.
 */
static void wiggler_act_shrink(void) {
    if (o->oTimer >= 20) {
        if (o->oTimer == 20) {
            cur_obj_play_sound_2(SOUND_OBJ_ENEMY_DEFEAT_SHRINK);
        }

        // 4 is the default scale, so shrink to 1/4 of regular size
        if (approach_f32_ptr(&o->header.gfx.scale[0], 1.0f, 0.1f)) {
            spawn_default_star(0.0f, 2048.0f, 0.0f);
            o->oAction = WIGGLER_ACT_FALL_THROUGH_FLOOR;
        }

        cur_obj_scale(o->header.gfx.scale[0]);
    }
}

/**
 * Fall through floors until y < 1700, then enter the walking action.
 */
static void wiggler_act_fall_through_floor(void) {
    if (o->oTimer == 60) {
        stop_background_music(SEQUENCE_ARGS(4, SEQ_EVENT_BOSS));
        o->oWigglerFallThroughFloorsHeight = 1700.0f;
    } else if (o->oTimer > 60) {
        if (o->oPosY < o->oWigglerFallThroughFloorsHeight) {
            o->oAction = WIGGLER_ACT_WALK;
        } else {
            o->oFaceAnglePitch = obj_get_pitch_from_vel();
        }

        cur_obj_move_using_fvel_and_gravity();
    }
}

/**
 * Attack handler for when wiggler is jumped or ground pounded on.
 * Stop and enter the jumped on action.
 */
void wiggler_jumped_on_attack_handler(void) {
    cur_obj_play_sound_2(SOUND_OBJ_WIGGLER_ATTACKED);
    o->oAction = WIGGLER_ACT_JUMPED_ON;
    o->oForwardVel = o->oVelY = 0.0f;
    o->oWigglerSquishSpeed = 0.4f;
}

/**
 * Update function for bhvWigglerHead.
 */
void bhv_wiggler_update(void) {
    // PARTIAL_UPDATE

    if (o->oAction == WIGGLER_ACT_UNINITIALIZED) {
        wiggler_init_segments();
    } else {
        if (o->oAction == WIGGLER_ACT_FALL_THROUGH_FLOOR) {
            wiggler_act_fall_through_floor();
        } else {
            treat_far_home_as_mario(1200.0f);

            // Walking animation and sound
            cur_obj_init_animation_with_accel_and_sound(0, o->oWigglerWalkAnimSpeed);
            if (o->oWigglerWalkAnimSpeed != 0.0f) {
                cur_obj_play_sound_at_anim_range(0, 13,
                              o->oHealth >= 4 ? SOUND_OBJ_WIGGLER_LOW_PITCH : SOUND_OBJ_WIGGLER_HIGH_PITCH);
            } else {
                cur_obj_reverse_animation();
            }

            cur_obj_update_floor_and_walls();
            switch (o->oAction) {
                case WIGGLER_ACT_WALK:
                    wiggler_act_walk();
                    break;
                case WIGGLER_ACT_KNOCKBACK:
                    wiggler_act_knockback();
                    break;
                case WIGGLER_ACT_JUMPED_ON:
                    wiggler_act_jumped_on();
                    break;
                case WIGGLER_ACT_SHRINK:
                    wiggler_act_shrink();
                    break;
                case WIGGLER_ACT_FALL_THROUGH_FLOOR:
                    wiggler_act_fall_through_floor();
                    break;
            }

            cur_obj_move_standard(-78);
        }

        // Update segment 0 with data from the wiggler object
        vec3f_copy(o->oWigglerSegments[0].pos, &o->oPosVec);
        o->oWigglerSegments[0].angle[0] = o->oFaceAnglePitch;
        o->oWigglerSegments[0].angle[1] = o->oFaceAngleYaw;

        // Update the rest of the segments to follow segment 0
        wiggler_update_segments();
    }
}
