#include "src/game/envfx_snow.h"

const GeoLayout ssl_pyramid_top_custom_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_CULLING_RADIUS(1000),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_OPAQUE, ssl_pyramid_top_custom_000_displaylist_mesh_layer_1),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
