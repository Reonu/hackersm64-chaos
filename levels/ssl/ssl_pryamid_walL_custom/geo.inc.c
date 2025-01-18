#include "src/game/envfx_snow.h"

const GeoLayout ssl_pryamid_walL_custom_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_CULLING_RADIUS(700),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_OPAQUE, ssl_pryamid_walL_custom_000_displaylist_mesh_layer_1),
			GEO_DISPLAY_LIST(LAYER_TRANSPARENT_DECAL, ssl_pryamid_walL_custom_000_displaylist_mesh_layer_6),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
