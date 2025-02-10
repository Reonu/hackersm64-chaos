#include "src/game/envfx_snow.h"

const GeoLayout mario_billboard_geo[] = {
   GEO_SHADOW(SHADOW_CIRCLE_PLAYER, 0xB4, 100),
	GEO_OPEN_NODE(),
		GEO_BILLBOARD_WITH_PARAMS_AND_DL(LAYER_ALPHA, 0, 0, 0, mario_billboard_mario_billboard_mesh_layer_4),
		GEO_TRANSLATE_NODE(LAYER_ALPHA, 0, 200, 0),
		GEO_OPEN_NODE(),
			GEO_HELD_OBJECT(0x00, 0, 200, 0, geo_switch_mario_hand_grab_pos),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
