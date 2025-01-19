#include "src/game/envfx_snow.h"

const GeoLayout item_box_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, NULL),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, item_box_a_cube_mesh_layer_5),
			GEO_ANIMATED_PART(LAYER_TRANSPARENT, 0, 0, 0, item_box_z_question_mark_mesh_layer_5),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
