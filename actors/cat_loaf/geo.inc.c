#include "src/game/envfx_snow.h"
const GeoLayout cat_loaf_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(LAYER_ETHEL, cat_clear_zbuffer),
		GEO_DISPLAY_LIST(LAYER_ETHEL, cat_loaf_cat_loaf_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
