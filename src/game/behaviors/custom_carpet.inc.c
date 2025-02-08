#include "src/engine/surface_collision.h"
#include "src/game/area.h"
#include "src/game/game_init.h"
#include "src/game/level_update.h"
#include "src/game/object_list_processor.h"
void bhv_custom_carpet_init(void) {
    // Use random_u16 to get a random number between -8000 and 8000
    o->oCustomCarpetGoalX = (random_u16() % 16000) - 8000;
    o->oCustomCarpetGoalZ = (random_u16() % 16000) - 8000;
    o->oCustomCarpetGoalY = random_u16() % ((u16)gMarioObject->oPosY + 10000);

    // If Mario is moving downwards, move initial position down based on Mario's yVel to avoid clipping
    if (gMarioState->vel[1] < 0) {
        o->oPosY -= gMarioState->vel[1] * -4;
    }

    struct Surface *floor;
    find_floor(o->oCustomCarpetGoalX, o->oCustomCarpetGoalY, o->oCustomCarpetGoalZ, &floor);

    while (floor == NULL) {

            o->oCustomCarpetGoalX = random_u16() % 8000;
            o->oCustomCarpetGoalZ = random_u16() % 8000;
            o->oCustomCarpetGoalY = random_u16() % ((u16)gMarioObject->oPosY + 10000);
            find_floor(o->oCustomCarpetGoalX, o->oCustomCarpetGoalY, o->oCustomCarpetGoalZ, &floor);
    }
}
#define CARPET_SPEED 50.f
void bhv_custom_carpet_loop(void) {
    f32 dx = o->oCustomCarpetGoalX - o->oPosX;
    f32 dz = o->oCustomCarpetGoalZ - o->oPosZ;
    f32 dy = o->oCustomCarpetGoalY - o->oPosY;

    f32 dist = sqrtf(dx * dx + dz * dz);
    f32 angle = atan2s(dz, dx);

    o->oPosX += CARPET_SPEED * sins(angle);
    o->oPosZ += CARPET_SPEED * coss(angle);
    o->oPosY += CARPET_SPEED * dy / dist;

    if (dist < 100.0f) {
        obj_mark_for_deletion(o);
    }
}