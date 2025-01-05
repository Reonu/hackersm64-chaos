#include "include/types.h"
#include "src/game/chaos_codes.h"
#include "src/game/game_init.h"
#include "src/game/level_update.h"
#include "include/model_ids.h"
#include "include/behavior_data.h"
#include "include/object_fields.h"
#include "include/object_constants.h"


u64 globalChaosFlags = GLOBAL_CHAOS_FLAG_NONE;
s32 nextGlobalCodeTimer = 150;

void add_global_chaos_code(void) {
    u16 chosenCode;
    u8 searchingForCode = TRUE;
    while (searchingForCode) {
        chosenCode = random_u16() % NUM_GLOBAL_CHAOS_FLAGS;
        if (!(globalChaosFlags & (1 << chosenCode))) {
            globalChaosFlags |= GLOBAL_CHAOS_FLAG_ENTER_CANNON;//(1 << chosenCode);
            searchingForCode = FALSE;
        }
    }


}

void trigger_chaos_code(u64 codeFlag) {
    switch (codeFlag) {
        case GLOBAL_CHAOS_FLAG_ENTER_CANNON:;
            struct Object *cannon = spawn_object_relative(0, 0, 300, 0, gMarioState->marioObj, MODEL_NONE, bhvCannon);
            SET_BPARAM1(cannon->oBehParams, CHAOS_CODE_BPARAM);
            globalChaosFlags &= ~GLOBAL_CHAOS_FLAG_ENTER_CANNON;
        break;
    }
}

void update_chaos_code_effects(void) {
    for (int i = 0; i < NUM_GLOBAL_CHAOS_FLAGS; i++) {
        u64 codeFlag = (1 << i);
        if (globalChaosFlags & codeFlag) {
            trigger_chaos_code(codeFlag);
        }
    }
}

void global_chaos_code_handler(void) {

    update_chaos_code_effects();

    nextGlobalCodeTimer--;
    if (nextGlobalCodeTimer <= 0) {
        add_global_chaos_code();
        nextGlobalCodeTimer = 150 + (random_u16() % 600);
    }
}