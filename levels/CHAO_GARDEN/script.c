#include <ultra64.h>
#include "sm64.h"
#include "behavior_data.h"
#include "model_ids.h"
#include "seq_ids.h"
#include "dialog_ids.h"
#include "segment_symbols.h"
#include "level_commands.h"

#include "game/level_update.h"

#include "levels/scripts.h"

#include "make_const_nonconst.h"
#include "levels/CHAO_GARDEN/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_CHAO_GARDEN_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x7, _CHAO_GARDEN_segment_7SegmentRomStart, _CHAO_GARDEN_segment_7SegmentRomEnd), 
	LOAD_YAY0(0xa, _water_skybox_yay0SegmentRomStart, _water_skybox_yay0SegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	LOAD_MODEL_FROM_GEO(MODEL_CREAM, cream_geo), 
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, CHAO_GARDEN_area_1),
		WARP_NODE(0x0A, LEVEL_BOB, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF1, LEVEL_CHAO_GARDEN, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_CHAO, -91, 1688, -2104, 0, 0, 0, (0x0A << 16), bhvChao),
		OBJECT(MODEL_CHAO, 958, 1780, -747, 0, 46, 0, (0x0A << 16), bhvChao),
		OBJECT(MODEL_CHAO, 2429, 4577, -685, 0, 27, 0, (0x0A << 16), bhvChao),
		OBJECT(MODEL_CHAO, 1278, 1687, -1794, 0, -68, 0, (0x0A << 16), bhvChao),
		OBJECT(MODEL_CREAM, 172, 1860, 484, 0, 0, 0, (0x0A << 16), bhvCream),
		MARIO_POS(0x01, 0, -373, 1689, -1762),
		OBJECT(MODEL_NONE, -337, 1689, -1765, 0, 0, 0, (0x0A << 16), bhvInstantActiveWarp),
		TERRAIN(CHAO_GARDEN_area_1_collision),
		MACRO_OBJECTS(CHAO_GARDEN_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_CHAOGARDEN),
		TERRAIN_TYPE(TERRAIN_GRASS),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(0x01, 0, -373, 1689, -1762),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};