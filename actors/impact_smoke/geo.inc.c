// 0x0D000BB8 / 0BFC
#include "actors/group0.h"
const GeoLayout bowser_impact_smoke_geo[] = {
   GEO_NODE_START(),
   GEO_OPEN_NODE(),
      GEO_ASM(GEO_TRANSPARENCY_MODE_NORMAL, geo_update_layer_transparency),
      GEO_SWITCH_CASE(5, geo_switch_anim_state),
      GEO_OPEN_NODE(),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, impact_smoke_seg6_dl_06062B38),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, impact_smoke_seg6_dl_06062BD8),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, impact_smoke_seg6_dl_06062C78),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, impact_smoke_seg6_dl_06062D18),
         GEO_DISPLAY_LIST(LAYER_TRANSPARENT, impact_smoke_seg6_dl_06062BD8),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
