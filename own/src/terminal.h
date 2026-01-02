#pragma once
#include <vte/vte.h>

VteTerminal *terminal_new(void);
void terminal_spawn(VteTerminal* term);
