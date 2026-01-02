#include "keys.h"
#include "../config.h"

gboolean handle_keys_press(GtkWidget* widget,
                           GdkEventKey* event,
                           gpointer user_data)
{
  (void)user_data;
  VteTerminal* term = VTE_TERMINAL(widget);

  unsigned int i;
  for (i = 0;i < G_N_ELEMENTS(keys);i++) {
      if(event->keyval == keys[i].key &&
         (event->state & keys[i].mod) == keys[i].mod)
    {
      keys[i].func(term);
      return TRUE;
    }
  }
  return FALSE;
}

void copy_clipboard(VteTerminal* term)
{
  vte_terminal_copy_clipboard_format(term,VTE_FORMAT_TEXT);
}

void paste_clipboard(VteTerminal* term)
{
  vte_terminal_paste_clipboard(term);
}
