#include "src/game/envfx_snow.h"

const GeoLayout cat_normal_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_BILLBOARD_WITH_PARAMS(LAYER_OPAQUE, 0, 0, 0),
		GEO_OPEN_NODE(),
			GEO_ASM(LAYER_ETHEL, cat_clear_zbuffer),
			GEO_DISPLAY_LIST(LAYER_ETHEL, cat_normal_cat_normal_mesh_layer_4),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
