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
    GLOBAL_CHAOS_NO_SPEED_CAP,
    GLOBAL_CHAOS_SUPER_JUMPS,
    GLOBAL_CHAOS_HEAVE_HO_CHASER,
    GLOBAL_CHAOS_STRONG_PUNCH_KB,
    GLOBAL_CHAOS_AUTOMATIC_WALLKICKS,
    GLOBAL_CHAOS_CHAIN_CHOMP,
    GLOBAL_CHAOS_THWOMP,
    GLOBAL_CHAOS_YELLOW_BLOCK_ON_JUMP,
    GLOBAL_CHAOS_MIRROR_MARIO,
    GLOBAL_CHAOS_WEIRD_AUDIO,
    GLOBAL_CHAOS_SWAP_POSITIONS,
    GLOBAL_CHAOS_BILLBOARD_EVERYTHING,
    GLOBAL_CHAOS_FAST_ENEMIES,
    GLOBAL_CHAOS_BILERP,
    GLOBAL_CHAOS_RANDOM_CAP,
    GLOBAL_CHAOS_ENEMY_POV,
    GLOBAL_CHAOS_KOOPA_SHELL,
    GLOBAL_CHAOS_SQUISH_MARIO,
    GLOBAL_CHAOS_ORTHO_CAM,
    GLOBAL_CHAOS_NEXT_LONG_JUMP_GP,
    GLOBAL_CHAOS_RANDOM_JUMP,
    GLOBAL_CHAOS_MARIO_SOUNDS_RANDOM_BANK,
    GLOBAL_CHAOS_CHUCKYA_ON_OBJECT_DELETION,
    GLOBAL_CHAOS_FOV_BASED_ON_FVEL,
    GLOBAL_CHAOS_ALL_CEILINGS_HANGABLE,
    GLOBAL_CHAOS_SUDDEN_REONU_SPRING,
    GLOBAL_CHAOS_AMP,
    GLOBAL_CHAOS_RANDOM_SCALE,
    GLOBAL_CHAOS_OBJECTS_FLEE_MARIO
};

enum CCMChaosNames {
    CCM_CHAOS_VOLCANIC_ROCKS,
};

enum BoBChaosNames {
    BOB_CHAOS_WATER_BOMB_SPAM,
    BOB_CHAOS_KOOPA_STORM
};

enum LLLChaosNames {
    LLL_CHAOS_SUPER_BURNING,
};

enum SSLChaosNAmes {
    SSL_CHAOS_BLIZZARD,
    SSL_CHAOS_QUICKSAND_MAGNET,
    SSL_CHAOS_INSTA_SNOW
};

enum WDWChaosNames {
    WDW_CHAOS_WATER_LEVEL,
    WDW_CHAOS_HEAVE_HO_STRENGTH,
};

enum THIChaosNames {
    THI_CHAOS_VERTICAL_WIND,
};

enum TTCChaosNames {
    TTC_CHAOS_UPWARP,
    TTC_CHAOS_MEDUSA_HEADS,
    TTC_CHAOS_OBJECTS_CATCH_MARIO,
    TTC_CHAOS_INSANE_SPEED,
    TTC_CHAOS_STOPPED_SPEED
};

enum HMCChaosNames {
    HMC_RANDOM_BOULDER,
    HMC_BOULDERS_FACE_MARIO,
    HMC_RANDOM_DOOR_WARPS,
};

// A mark for exclusivity. Can only have one of thse effects active at a time.
enum CodeFlags {
    CODEFLAG_NONE,
    CODEFLAG_SCREEN,
    CODEFLAG_CAMERA,
    CODEFLAG_AUDIO,

    CODEFLAG_MINOR, // This isn't actually exclusivity, but will set the timer to 1/3 it's normal value
};

typedef struct ChaosCode {
    char *name;
    void (*func)();
    unsigned char probability;
    unsigned short timerLow;
    unsigned short timerHigh;
    unsigned int flags;

    short timer;
    char active;
} ChaosCode;

extern ChaosCode gChaosCodeTable[];
extern ChaosCode gBoBChaosTable[];
extern ChaosCode gTTCChaosTable[];
extern ChaosCode gSSLChaosTable[];
extern ChaosCode gWDWChaosTable[];
extern ChaosCode gLLLChaosTable[];
extern ChaosCode gSLChaosTable[];
extern ChaosCode gTHIChaosTable[];
extern ChaosCode gHMCChaosTable[];
extern u8 gDisableChaos;
extern float gCrimes;
extern u8 gChaosOffOverride;
extern u8 gSpamAd;
extern s16 gSpamCursorX;
extern s16 gSpamCursorY;
extern u8 gPovActive;
extern struct Object *gPovEnemy;
extern struct Object *sMirrorGhost;
extern s16 gPovPrevMode;
extern f32 gHeaveHoStrength;
extern s32 nextGlobalCodeTimer;

void global_chaos_code_handler(void);
void chaos_enable(ChaosCode *table, s32 codeID, s32 tableSize);
ChaosCode *chaos_level_table(s32 levelID, s32 *size);
void chaos_clear_level(ChaosCode *table, s32 size);
