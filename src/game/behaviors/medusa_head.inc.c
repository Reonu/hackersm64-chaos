struct ObjectHitbox sMedusaHeadHitbox = {
    /* interactType:      */ INTERACT_DAMAGE,
    /* downOffset:        */ 50,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 1,
    /* numLootCoins:      */ 1,
    /* radius:            */ 170,
    /* height:            */ 100,
    /* hurtboxRadius:     */ 160,
    /* hurtboxHeight:     */ 90,
};

void bhv_medusa_head_init(void) {

}

void bhv_medusa_head_loop(void) {
    o->oForwardVel = 20.0f;
    o->oPosY = o->oHomeY + 200*sins(o->oTimer *  0x222);
    cur_obj_move_xz_using_fvel_and_yaw();

    obj_set_hitbox(o, &sMedusaHeadHitbox);
    o->oInteractStatus = 0;

    if ((o->oTimer / 20)%2 == 0 ) {
        o->oFaceAngleRoll = 0x1500;
    }
    else {
        o->oFaceAngleRoll = -0x1500;
    }
}

