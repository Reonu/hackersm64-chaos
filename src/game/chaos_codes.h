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
    GLOBAL_CHAOS_MARIO_SOUNDS_SCREAM,
    GLOBAL_CHAOS_RANDOMIZE_COIN_COLORS,
    GLOBAL_CHAOS_COIN_CUTSCENES,
    GLOBAL_CHAOS_ALL_JUMPS_TRIPLE,
    GLOBAL_CHAOS_DELETE_NEARBY_OBJECTS,
    GLOBAL_CHAOS_INVERT_DIVE_AND_KICK,
    GLOBAL_CHAOS_LIVE_MARIO_REACTION,
    GLOBAL_CHAOS_AD_SPAM,
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

extern ChaosCode gChaosCodeTable[25];
extern u8 gDisableChaos;
extern float gCrimes;
extern u8 gChaosOffOverride;
extern u8 gSpamAd;
extern s16 gSpamCursorX;
extern s16 gSpamCursorY;

#define CHAOSCOUNT (sizeof(gChaosCodeTable) / sizeof(ChaosCode))

void global_chaos_code_handler(void);
void chaos_enable(ChaosCode *table, s32 codeID, s32 tableSize);
