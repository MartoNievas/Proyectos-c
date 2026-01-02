#ifndef CONFIG_H
#define CONFIG_H

#include <gdk/gdk.h>
#include "src/keys.h"

#define MOD_TERM GDK_CONTROL_MASK

static char *font = "JetBrainsMono Nerd Font 11";

static Key keys[] = {
    { GDK_KEY_C, MOD_TERM | GDK_SHIFT_MASK, copy_clipboard },
    { GDK_KEY_V, MOD_TERM | GDK_SHIFT_MASK, paste_clipboard },
};

#endif /* CONFIG_H */
