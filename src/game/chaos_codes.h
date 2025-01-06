#include "include/types.h"

enum GlobalChaosFlags {
    GLOBAL_CHAOS_FLAG_NONE = 0,
    GLOBAL_CHAOS_FLAG_ENTER_CANNON = (1 << 0),
    GLOBAL_CHAOS_FLAG_FALL_DAMAGE_THRESHOLD = (1 << 1),
    GLOBAL_CHAOS_FLAG_TRIPPING = (1 << 2),
    GLOBAL_CHAOS_FLAG_UPSIDE_DOWN_CAMERA = (1 << 3),
    GLOBAL_CHAOS_FLAG_NO_MODEL_IS_MARIO = (1 << 4),
    GLOBAL_CHAOS_FLAG_RETRO = (1 << 5),
    GLOBAL_CHAOS_FLAG_BLUE = (1 << 6),
};

typedef struct ChaosCode {
    char *name;
    void (*func)();
    unsigned short timerLow;
    unsigned short timerHigh;
} ChaosCode;

extern ChaosCode gChaosCodeTable[7];
extern s32 gChaosCodeTimers[7];
extern u64 globalChaosFlags;
extern u8 gDisableChaos;
extern u8 gRetroVision;
extern u8 gBlurVision;

#define CHAOSCOUNT (sizeof(gChaosCodeTable) / sizeof(ChaosCode))

extern void global_chaos_code_handler();
void chaos_enable(s32 codeID);
