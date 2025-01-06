#pragma once

void render_fb_effects(void);
void run_motion_blur(s32 goalAmount);
void set_fb_effect_type(u32 type);
void set_motion_blur(s32 goalAmount);
void set_fb_effect_col(u8 r, u8 g, u8 b);
s32 render_fbe_warning(void);
s32 script_check_fbe_warning(UNUSED s16 arg);

enum FBETypes {
    FBE_EFFECT_BRIGHTEN = (1 << 0),
    FBE_EFFECT_MULT = (1 << 1),
};

typedef struct FBEffects {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    u8 type; // FBETypes
} FBEffects;