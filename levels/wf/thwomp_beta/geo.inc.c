#include "src/game/envfx_snow.h"

const GeoLayout thwomp_beta_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_SHADOW(1, 180, 300),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_OPAQUE, thwomp_beta_2_thwomp_geo_display_list_thwomp_seg5_dl_0500B750_mesh_layer_1),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
