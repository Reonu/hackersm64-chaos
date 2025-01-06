#include "include/types.h"

enum GlobalChaosFlags {
    GLOBAL_CHAOS_FLAG_NONE = 0,
    GLOBAL_CHAOS_FLAG_ENTER_CANNON = (1 << 0),
    GLOBAL_CHAOS_FLAG_FALL_DAMAGE_THRESHOLD = (1 << 1),
    GLOBAL_CHAOS_FLAG_TRIPPING = (1 << 2),
    GLOBAL_CHAOS_FLAG_RETRO = (1 << 3),
};

#define NUM_GLOBAL_CHAOS_FLAGS  4

typedef struct ChaosCode {
    char *name;
    void (*func)();
    unsigned short timerLow;
    unsigned short timerHigh;
} ChaosCode;

extern ChaosCode gChaosCodeTable[4];
extern u64 globalChaosFlags;
extern u8 gDisableChaos;
extern u8 gRetroVision;

#define CHAOSCOUNT (sizeof(gChaosCodeTable) / sizeof(ChaosCode))

extern void global_chaos_code_handler();
void chaos_enable(s32 codeID);
