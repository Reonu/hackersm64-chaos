void bhv_gear_walls_init(void) {
    o->oF4 = 0;
}

void bhv_gear_walls_loop(void) {

    if (o->oTimer % 3 == 0) {
        o->oF4 += 1;
        if (o->oF4 >= 3) {
            o->oF4 = 0;
        }
    }

    cur_obj_set_model(MODEL_TTC_GEAR_WALLS_1 + o->oF4);
}

