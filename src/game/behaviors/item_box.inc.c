void bhv_item_box_init(void) {
   
}

void bhv_item_box_loop(void) {
    // Move up and down in a SLOW cycle:
    o->oPosY = o->oHomeY + (sins(o->oTimer * 0x400) * 10.0f);
}

