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