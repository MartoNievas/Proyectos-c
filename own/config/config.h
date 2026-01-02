#ifndef CONFIG_H
#define CONFIG_H

#include <gdk/gdk.h>
#include "../src/keys.h"

extern const double default_zoom;
extern const char *font;

extern const GdkRGBA bg;
extern const GdkRGBA fg;



extern const Key keys[];
extern unsigned int const keys_len;
#endif /* CONFIG_H */
