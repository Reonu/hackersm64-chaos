#include "src/game/envfx_snow.h"

const GeoLayout bbh_wooden_platform_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, bbh_wooden_platform_Cube_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
