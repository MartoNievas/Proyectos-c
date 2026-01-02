#include "terminal.h"
#include "../config/config.h"
VteTerminal* terminal_new(void)
{
  VteTerminal* term = VTE_TERMINAL(vte_terminal_new());
  vte_terminal_set_enable_legacy_osc777(term,TRUE);
  
  return term;
}

void terminal_spawn(VteTerminal* term)
{
  const char* shell = g_getenv("SHELL");
  if (!shell) shell = "/bin/bash";
  
  char* argv[] = {(char*) shell, NULL};

  vte_terminal_spawn_async(
    term,
    VTE_PTY_DEFAULT,
    NULL,
    argv,
    NULL,
    G_SPAWN_DEFAULT,
    NULL,NULL,NULL,
    -1,
    NULL,NULL,NULL
  );
  
  vte_terminal_set_audible_bell(term, FALSE);
    
}

void terminal_set_font(VteTerminal* term) 
{
  /* Seteo de fuente */
  PangoFontDescription* font_desc;
  font_desc = pango_font_description_from_string(font);

  vte_terminal_set_font(term,font_desc);

  pango_font_description_free(font_desc);

  /* Seteto de escala*/

  vte_terminal_set_font_scale(term, 0.75);
  vte_terminal_set_cell_width_scale(term, 1.0);
  vte_terminal_set_cell_height_scale(term, 1.0);
}

void terminal_set_colors(VteTerminal* term)
{
    vte_terminal_set_colors(
    term,
    &bg,
    &fg,
    NULL,
    0
  );
}
