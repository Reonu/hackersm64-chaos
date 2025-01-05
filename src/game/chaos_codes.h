#include "include/types.h"

enum GlobalChaosFlags {
    GLOBAL_CHAOS_FLAG_NONE = 0,
    GLOBAL_CHAOS_FLAG_ENTER_CANNON = (1 << 0),
    GLOBAL_CHAOS_FLAG_FALL_DAMAGE_THRESHOLD = (1 << 1),
    GLOBAL_CHAOS_FLAG_TRIPPING = (1 << 2)
};

#define NUM_GLOBAL_CHAOS_FLAGS  4

extern void global_chaos_code_handler();