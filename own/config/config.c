#include "config.h"

#define MOD_TERM GDK_CONTROL_MASK

const double default_zoom = 0.75;
const char *font = "JetBrainsMono Nerd Font 20";

const GdkRGBA bg = {0.1, 0.1, 0.1, 1.0};
const GdkRGBA fg = {0.0, 0.0, 0.0, 0.0};

const Key keys[] = {
    { GDK_KEY_C,     MOD_TERM | GDK_SHIFT_MASK, copy_clipboard },
    { GDK_KEY_V,     MOD_TERM | GDK_SHIFT_MASK, paste_clipboard },
    { GDK_KEY_plus,  MOD_TERM,                 zoom_in },
    { GDK_KEY_minus, MOD_TERM,                 zoom_out },
    { GDK_KEY_0,     MOD_TERM,                 reset_zoom },
};

const unsigned int keys_len = sizeof(keys) / sizeof(keys[0]);

