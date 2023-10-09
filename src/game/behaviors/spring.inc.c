void bhv_spring_init(void) {
    switch (BPARAM2) {
        case 0x00:
            o->oFriction = 40.0f;
            o->oPrimRGB = 0x00FF00;
            break;
        case 0x01:
            o->oFriction = 80.0f;
            o->oPrimRGB = 0xFFFF00;
            break;
        case 0x02:
            o->oFriction = 150.0f;
            o->oPrimRGB = 0xFF0000;
            break;
    }
}

void bhv_spring_loop(void) {
    if (o->oAction == 1) {
        if (o->oTimer == 1){
                gMarioState->action = ACT_DOUBLE_JUMP;
            }
        if (o->oTimer < 4) {
            o->header.gfx.scale[1] -= 0.125f;
        }
        else if (o->oTimer < 8) {
            o->header.gfx.scale[1] += 0.125f;
        }
        else {
            o->header.gfx.scale[1] = 1;
            o->oAction = 0;
        }
    }
}