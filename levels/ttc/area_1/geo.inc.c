#include "src/game/envfx_snow.h"

const GeoLayout ttc_area_1_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_ASM(0, geo_movtex_pause_control),
		GEO_ASM(MOVTEX_TREADMILL_BIG, geo_movtex_update_horizontal),
		GEO_ASM(MOVTEX_TREADMILL_SMALL, geo_movtex_update_horizontal),
		GEO_ROTATION_NODE_WITH_DL(LAYER_OPAQUE, 90, 0, 0, ttc_dl_18_GeoRoot_ttc_1_ttc_seg7_dl_0700AD38_mesh_layer_1),
		GEO_ROTATION_NODE_WITH_DL(LAYER_TRANSPARENT, 90, 0, 0, ttc_dl_19_GeoRoot_ttc_1_ttc_seg7_dl_0700B1D8_mesh_layer_5),
		GEO_ROTATION_NODE_WITH_DL(LAYER_ALPHA, 90, 0, 0, ttc_dl_20_GeoRoot_ttc_1_ttc_seg7_dl_0700E878_mesh_layer_4),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_OPAQUE, ttc_dl_20_GeoRoot_ttc_1_ttc_seg7_dl_0700E878_mesh_layer_1),
		GEO_CLOSE_NODE(),
		GEO_ASM(ENVFX_MODE_NONE, geo_envfx_main),
	GEO_CLOSE_NODE(),
	GEO_RETURN(),
};
const GeoLayout ttc_area_1[] = {
	GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
	GEO_OPEN_NODE(),
		GEO_ZBUFFER(0),
		GEO_OPEN_NODE(),
			GEO_NODE_ORTHO(100.0000),
			GEO_OPEN_NODE(),
				GEO_BACKGROUND_COLOR(0x0001),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_ZBUFFER(1),
		GEO_OPEN_NODE(),
			GEO_CAMERA_FRUSTUM_WITH_FUNC(45.0000, 100, 30000, geo_camera_fov),
			GEO_OPEN_NODE(),
				GEO_CAMERA(CAMERA_MODE_OUTWARD_RADIAL, 0, 0, 0, 0, -100, 0, geo_camera_main),
				GEO_OPEN_NODE(),
					GEO_BRANCH(1, ttc_area_1_geo),
					GEO_RENDER_OBJ(),
					GEO_ASM(ENVFX_MODE_NONE, geo_envfx_main),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
