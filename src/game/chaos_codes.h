#include "include/types.h"

enum GlobalChaosFlags {
    GLOBAL_CHAOS_FLAG_NONE = 0,
    GLOBAL_CHAOS_FLAG_ENTER_CANNON = (1 << 0),
    GLOBAL_CHAOS_FLAG_FALL_DAMAGE_THRESHOLD = (1 << 1),
    GLOBAL_CHAOS_FLAG_TRIPPING = (1 << 2),
    GLOBAL_CHAOS_FLAG_UPSIDE_DOWN_CAMERA = (1 << 3),
    GLOBAL_CHAOS_FLAG_NO_MODEL_IS_MARIO = (1 << 4),
    GLOBAL_CHAOS_FLAG_RETRO = (1 << 5),
    GLOBAL_CHAOS_FLAG_BLUR = (1 << 6),
    GLOBAL_CHAOS_FLAG_LOWFPS = (1 << 6),
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
} ChaosCode;

extern ChaosCode gChaosCodeTable[10];
extern s32 gChaosCodeTimers[10];
extern u64 globalChaosFlags;
extern u8 gDisableChaos;
extern u8 gRetroVision;
extern u8 gBlurVision;
extern u8 gLowFPS;
extern u8 gTankControls;
extern u8 gFlipInputs;

#define CHAOSCOUNT (sizeof(gChaosCodeTable) / sizeof(ChaosCode))

extern void global_chaos_code_handler();
void chaos_enable(s32 codeID);
