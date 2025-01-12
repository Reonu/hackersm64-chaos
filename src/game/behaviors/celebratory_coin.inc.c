void bhv_celebratory_coin_init(void) {
    o->oPosX = gMarioState->pos[0];
    o->oPosY = gMarioState->pos[1] + 170;
    o->oPosZ = gMarioState->pos[2];
}

void bhv_celebratory_coin_loop(void) {
    o->oPosX = gMarioState->pos[0];
    o->oPosY = gMarioState->pos[1] + 170;
    o->oPosZ = gMarioState->pos[2];    

    o->oAnimState++;

    if (gMarioState->action != ACT_GET_COIN) {
        mark_obj_for_deletion(o);
    }
}

