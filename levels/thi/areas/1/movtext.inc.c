// 0x0700E2BC
static Movtex thi_movtex_area1_short_side_water_data[] = {
    MOV_TEX_INIT_LOAD(    1),
    MOV_TEX_ROT_SPEED(   6),
    MOV_TEX_ROT_SCALE(   6),
    MOV_TEX_4_BOX_TRIS(-4914, -4914),
    MOV_TEX_4_BOX_TRIS(-4914,  3748),
    MOV_TEX_4_BOX_TRIS(-3071,  3748),
    MOV_TEX_4_BOX_TRIS(-3071, -4914),
    MOV_TEX_ROT(     ROTATE_COUNTER_CLOCKWISE),
    MOV_TEX_ALPHA(    0x96),
    MOV_TEX_DEFINE(  TEXTURE_WATER),
    MOV_TEX_END(),
    0,
};

// 0x0700E2DC
static Movtex thi_movtex_area1_large_side_water_data[] = {
    MOV_TEX_INIT_LOAD(    1),
    MOV_TEX_ROT_SPEED(   6),
    MOV_TEX_ROT_SCALE(   6),
    MOV_TEX_4_BOX_TRIS(-2764,  2703),
    MOV_TEX_4_BOX_TRIS(-2764,  4915),
    MOV_TEX_4_BOX_TRIS( 4177,  4915),
    MOV_TEX_4_BOX_TRIS( 4177,  2703),
    MOV_TEX_ROT(     ROTATE_COUNTER_CLOCKWISE),
    MOV_TEX_ALPHA(    0x96),
    MOV_TEX_DEFINE(  TEXTURE_WATER),
    MOV_TEX_END(),
    0,
};

// 0x0700E2FC
static Movtex thi_movtex_area1_mountain_top_water_data[] = {
    MOV_TEX_INIT_LOAD(    1),
    MOV_TEX_ROT_SPEED(   6),
    MOV_TEX_ROT_SCALE(   6),
    MOV_TEX_4_BOX_TRIS(-614, -1533),
    MOV_TEX_4_BOX_TRIS(-614, -304),
    MOV_TEX_4_BOX_TRIS(614, -304),
    MOV_TEX_4_BOX_TRIS(614, -1533),
    MOV_TEX_ROT(     ROTATE_COUNTER_CLOCKWISE),
    MOV_TEX_ALPHA(    0x96),
    MOV_TEX_DEFINE(  TEXTURE_WATER),
    MOV_TEX_END(),
    0,
};

// 0x0700E31C
const struct MovtexQuadCollection thi_movtex_area1_water[] = {
    {0, thi_movtex_area1_short_side_water_data},
    {1, thi_movtex_area1_large_side_water_data},
    {2, thi_movtex_area1_mountain_top_water_data},
    {-1, NULL},
};
