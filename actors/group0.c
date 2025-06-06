#include <ultra64.h>
#include "sm64.h"
#include "surface_terrains.h"
#include "geo_commands.h"

#include "make_const_nonconst.h"

// Note: This bin does not use bin IDs, unlike the other segmented bins.
#include "mario/model.inc.c"

#include "bubble/model.inc.c"

#include "walk_smoke/model.inc.c"

#include "burn_smoke/model.inc.c"

#include "small_water_splash/model.inc.c"

#include "water_wave/model.inc.c"

#include "sparkle/model.inc.c"

#include "water_splash/model.inc.c"

#include "white_particle_small/model.inc.c"

#include "sparkle_animation/model.inc.c"

#ifdef S2DEX_TEXT_ENGINE
#include "src/s2d_engine/s2d_config.h"
#include FONT_C_FILE
#endif

#include "chao/model.inc.c"
#include "chao/anims/data.inc.c"
#include "chao/anims/table.inc.c"
#include "kart/model.inc.c"
#include "kart/anims/data.inc.c"
#include "kart/anims/table.inc.c"
#include "mario/anims/data.inc.c"
#include "mario/anims/table.inc.c"
#include "mario_billboard/model.inc.c"
#include "bobomb_cop/model.inc.c"


#include "heave_ho/model.inc.c"
#include "heave_ho/anims/data.inc.c"
#include "heave_ho/anims/table.inc.c"
#include "item_box/model.inc.c"
#include "item_box/anims/data.inc.c"
#include "item_box/anims/table.inc.c"

#include "chain_chomp/model.inc.c"
#include "chain_chomp/anims/data.inc.c"
#include "chain_chomp/anims/table.inc.c"

#include "chain_ball/model.inc.c"

#include "thwomp/model.inc.c"
#include "thwomp/collision.inc.c"
#include "shadow_mario/model.inc.c"
#include "spring/model.inc.c"

#include "skeeter/model.inc.c"
#include "skeeter/anims/data.inc.c"
#include "skeeter/anims/table.inc.c"

#include "koopa_shell/model.inc.c"
#include "cat_normal/model.inc.c"
#include "cat_loaf/model.inc.c"
#include "betah_mario_icon/model.inc.c"

#include "moneybag/model.inc.c"
#include "moneybag/anims/data.inc.c"
#include "moneybag/anims/table.inc.c"

#include "levels/rr/flying_carpet/model.inc.c"
#include "levels/rr/flying_carpet/collision.inc.c"

#include "king_bobomb/model.inc.c"
#include "king_bobomb/anims/data.inc.c"
#include "king_bobomb/anims/table.inc.c"

#include "bowser/model.inc.c"
#include "bowser/anims/data.inc.c"
#include "bowser/anims/table.inc.c"
#include "bowser/flames_data.inc.c"

#include "impact_ring/model.inc.c"

#include "bowser_flame/model.inc.c"
#include "impact_smoke/model.inc.c"
#include "bomb/model.inc.c"
#include "yellow_sphere/model.inc.c"
#include "armstrong_frame_1/model.inc.c"
#include "armstrong_frame_2/model.inc.c"
#include "armstrong_frame_3/model.inc.c"
#include "armstrong_idle/model.inc.c"
#include "mario_none/model.inc.c"