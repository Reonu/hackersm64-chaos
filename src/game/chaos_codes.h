#pragma once
#include "include/types.h"

enum GlobalChaosNames {
    GLOBAL_CHAOS_ENTER_CANNON,
    GLOBAL_CHAOS_FALL_DAMAGE,
    GLOBAL_CHAOS_TRIPPING,
    GLOBAL_CHAOS_UPSIDE_DOWN_CAMERA,
    GLOBAL_CHAOS_NO_MODEL_IS_MARIO,
    GLOBAL_CHAOS_RETRO,
    GLOBAL_CHAOS_BLUR,
    GLOBAL_CHAOS_LOWFPS,
    GLOBAL_CHAOS_MARIO_KART,
    GLOBAL_CHAOS_PAY_TO_MOVE,
    GLOBAL_CHAOS_TANK_CONTROLS,
    GLOBAL_CHAOS_INVERT_CONTROLS,
    GLOBAL_CHAOS_DIM_LIGHTS,
    GLOBAL_CHAOS_LAW_METRE,
    GLOBAL_CHAOS_TINY_MARIO,
    GLOBAL_CHAOS_BILLBOARD_MARIO,
    GLOBAL_CHAOS_VERY_SLIPPERY,
    GLOBAL_CHAOS_ALL_QUICKSAND,
};

enum CCMChaosNames {
    CCM_CHAOS_VOLCANIC_ROCKS,
};

// A mark for exclusivity. Can only have one of thse effects active at a time.
enum CodeFlags {
    CODEFLAG_NONE,
    CODEFLAG_SCREEN,    
};

typedef struct ChaosCode {
    char *name;
    void (*func)();
    unsigned short timerLow;
    unsigned short timerHigh;
    unsigned int flags;

    short timer;
    char active;
} ChaosCode;

extern ChaosCode gChaosCodeTable[18];
extern u8 gDisableChaos;
extern float gCrimes;

#define CHAOSCOUNT (sizeof(gChaosCodeTable) / sizeof(ChaosCode))

void global_chaos_code_handler(void);
void chaos_enable(ChaosCode *table, s32 codeID, s32 tableSize);
