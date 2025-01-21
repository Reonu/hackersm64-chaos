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

#include "actors/common1.h"
#include "make_const_nonconst.h"
#include "levels/pss/header.h"

/* Fast64 begin persistent block [scripts] */
/* Fast64 end persistent block [scripts] */

const LevelScript level_pss_entry[] = {
	INIT_LEVEL(),
	LOAD_YAY0(0x07, _pss_segment_7SegmentRomStart, _pss_segment_7SegmentRomEnd), 
	LOAD_YAY0_TEXTURE(0x09, _mountain_yay0SegmentRomStart, _mountain_yay0SegmentRomEnd), 
	LOAD_YAY0(0x05, _group8_yay0SegmentRomStart, _group8_yay0SegmentRomEnd), 
	LOAD_RAW(0x0C, _group8_geoSegmentRomStart, _group8_geoSegmentRomEnd), 
	LOAD_YAY0(0x08, _common0_yay0SegmentRomStart, _common0_yay0SegmentRomEnd), 
	LOAD_RAW(0x0F, _common0_geoSegmentRomStart, _common0_geoSegmentRomEnd), 
	ALLOC_LEVEL_POOL(),
	MARIO(MODEL_MARIO, 0x00000001, bhvMario), 
	JUMP_LINK(script_func_global_1), 
	JUMP_LINK(script_func_global_9), 
	/* Fast64 begin persistent block [level commands] */
	/* Fast64 end persistent block [level commands] */

	AREA(1, pss_area_1),
		WARP_NODE(0xF1, LEVEL_CASTLE, 0x01, 0x23, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF0, LEVEL_CASTLE, 0x01, 0x26, WARP_NO_CHECKPOINT),
		WARP_NODE(0xF3, LEVEL_CASTLE, 0x01, 0x20, WARP_NO_CHECKPOINT),
		WARP_NODE(0x0A, LEVEL_PSS, 0x01, 0x0A, WARP_NO_CHECKPOINT),
		OBJECT(MODEL_1UP, 1847, -961, 3863, 0, -180, 0, 0x00000000, bhv1upSliding),
		OBJECT(MODEL_NONE, 5632, 6751, -5631, 0, -90, 0, 0x000A0000, bhvAirborneWarp),
		OBJECT(MODEL_BLUE_COIN_SWITCH, 5423, 6140, -6540, 0, 0, 0, 0x00000000, bhvBlueCoinSwitch),
		OBJECT(MODEL_NONE, 3000, 6180, -5640, 0, 90, 0, ((COIN_FORMATION_FLAG_NONE) << 16), bhvCoinFormation),
		OBJECT(MODEL_NONE, 1880, -1440, 60, 0, 0, 0, ((COIN_FORMATION_FLAG_NONE) << 16), bhvCoinFormation),
		OBJECT(MODEL_NONE, 1860, -1760, -1720, 0, 0, 0, ((COIN_FORMATION_FLAG_NONE) << 16), bhvCoinFormation),
		OBJECT(MODEL_NONE, 1860, -1200, 1680, 0, 0, 0, ((COIN_FORMATION_FLAG_NONE) << 16), bhvCoinFormation),
		OBJECT(MODEL_NONE, 260, 2580, 2280, 0, 90, 0, ((COIN_FORMATION_FLAG_NONE) << 16), bhvCoinFormation),
		OBJECT(MODEL_NONE, -880, 2880, 2280, 0, 90, 0, ((COIN_FORMATION_FLAG_NONE) << 16), bhvCoinFormation),
		OBJECT(MODEL_EXCLAMATION_BOX, -6385, 5277, 5770, 0, 0, 0, (EXCLAMATION_BOX_BP_STAR_1 << 16), bhvExclamationBox),
		OBJECT(MODEL_1UP, -6380, 4977, 5980, 0, 0, 0, (4 << 16), bhvHidden1up),
		OBJECT(MODEL_NONE, 1860, -1220, 1700, 0, 0, 0, 0x00000000, bhvHidden1upTrigger),
		OBJECT(MODEL_NONE, 1880, -1460, 80, 0, 0, 0, 0x00000000, bhvHidden1upTrigger),
		OBJECT(MODEL_NONE, 1860, -1820, -1680, 0, 0, 0, 0x00000000, bhvHidden1upTrigger),
		OBJECT(MODEL_NONE, -6380, 4927, 6320, 0, 0, 0, 0x00000000, bhvHidden1upTrigger),
		OBJECT(MODEL_BLUE_COIN, 780, 5613, -5600, 0, 0, 0, 0x00000000, bhvHiddenBlueCoin),
		OBJECT(MODEL_BLUE_COIN, 2360, 5979, -5600, 0, 0, 0, 0x00000000, bhvHiddenBlueCoin),
		OBJECT(MODEL_BLUE_COIN, -809, 5245, -5600, 0, 0, 0, 0x00000000, bhvHiddenBlueCoin),
		OBJECT(MODEL_BLUE_COIN, -2409, 4877, -5600, 0, 0, 0, 0x00000000, bhvHiddenBlueCoin),
		OBJECT(MODEL_BLUE_COIN, -4445, 4490, -5536, 0, 0, 0, 0x00000000, bhvHiddenBlueCoin),
		OBJECT(MODEL_BLUE_COIN, -6263, 4150, -4009, 0, 0, 0, 0x00000000, bhvHiddenBlueCoin),
		OBJECT(MODEL_WOODEN_SIGNPOST, 3580, 6140, -5180, 0, 90, 0, (DIALOG_149 << 16), bhvMessagePanel),
		OBJECT(MODEL_YELLOW_COIN, -3233, 4688, -5600, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -5636, 4331, -5054, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -5980, 3453, 945, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -2870, 2837, 2300, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, 3930, 274, 2425, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, 5174, 61, 3200, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, 5707, -128, 4565, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, 5194, -310, 5845, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, 3640, -543, 6450, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, 2550, -700, 6050, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, 1821, -921, 4616, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -20, -2542, -6304, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -1197, -2929, -4692, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -2565, -3268, -3525, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -4909, -3633, -4218, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -6290, -3937, -2545, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -6400, -4146, -590, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -6400, -4335, 1409, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, -6400, -4530, 3481, 0, 0, 0, 0x00000000, bhvOneCoin),
		OBJECT(MODEL_YELLOW_COIN, 1854, -2132, -4290, 0, 0, 0, 0x00000000, bhvOneCoin),
		TERRAIN(pss_area_1_collision),
		MACRO_OBJECTS(pss_area_1_macro_objs),
		SET_BACKGROUND_MUSIC(0x00, SEQ_LEVEL_SLIDE),
		TERRAIN_TYPE(TERRAIN_STONE),
		/* Fast64 begin persistent block [area commands] */
		/* Fast64 end persistent block [area commands] */
	END_AREA(),
	FREE_LEVEL_POOL(),
	MARIO_POS(1, 0, 0, 0, 0),
	CALL(0, lvl_init_or_update),
	CALL_LOOP(1, lvl_init_or_update),
	CLEAR_LEVEL(),
	SLEEP_BEFORE_EXIT(1),
	EXIT(),
};