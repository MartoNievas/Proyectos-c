#pragma once
#include <vte/vte.h>

VteTerminal *terminal_new(void);
void terminal_spawn(VteTerminal* term);
void terminal_set_font(VteTerminal* term);
void terminal_set_colors(VteTerminal* term);
