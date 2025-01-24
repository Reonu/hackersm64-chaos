#include "src/game/envfx_snow.h"

const GeoLayout medusa_head_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, medusa_head_Castlevania_Medusa_Head_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
