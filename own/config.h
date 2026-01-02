#ifndef CONFIG_H
#define CONFIG_H

#include <gdk/gdk.h>
#include "src/keys.h"

#define MOD_TERM GDK_CONTROL_MASK
static const double default_zoom = 0.75;
static char *font = "JetBrainsMono Nerd Font 20";

static const GdkRGBA bg = {1.0,1.0,1.0,1.0};
static const GdkRGBA fg = {0.0,0.0,0.0,0.0};



static Key keys[] = {
    { GDK_KEY_C, MOD_TERM | GDK_SHIFT_MASK, copy_clipboard },
    { GDK_KEY_V, MOD_TERM | GDK_SHIFT_MASK, paste_clipboard },
    { GDK_KEY_plus, MOD_TERM, zoom_in},
    { GDK_KEY_minus,MOD_TERM,zoom_out},
    { GDK_KEY_0, MOD_TERM,reset_zoom},
};

#endif /* CONFIG_H */
