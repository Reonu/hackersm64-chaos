#include "include/types.h"

enum GlobalChaosFlags {
    GLOBAL_CHAOS_FLAG_NONE = 0,
    GLOBAL_CHAOS_FLAG_ENTER_CANNON = (1 << 0),
    GLOBAL_CHAOS_FLAG_FALL_DAMAGE_THRESHOLD = (1 << 1),
    GLOBAL_CHAOS_FLAG_TRIPPING = (1 << 2),
    GLOBAL_CHAOS_FLAG_UPSIDE_DOWN_CAMERA = (1 << 3)
};

typedef struct ChaosCode {
    char *name;
    void (*func)();
} ChaosCode;

extern ChaosCode gChaosCodeTable[4];
extern u64 globalChaosFlags;
extern u8 gDisableChaos;

#define CHAOSCOUNT (sizeof(gChaosCodeTable) / sizeof(ChaosCode))

extern void global_chaos_code_handler();