#include "src/game/envfx_snow.h"

const GeoLayout spring_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(LAYER_OPAQUE, geo_set_spring_color),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, spring_spring_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
