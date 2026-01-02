#include "keys.h"
#include "../config/config.h"
static double zoom = 0.75;
gboolean handle_keys_press(GtkWidget* widget,
                           GdkEventKey* event,
                           gpointer user_data)
{
  (void)user_data;
  VteTerminal* term = VTE_TERMINAL(widget);

  unsigned int i;
  for (i = 0;i < keys_len;i++) {
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

void apply_zoom(VteTerminal *term)
{
    int rows = vte_terminal_get_row_count(term);
    int cols = vte_terminal_get_column_count(term);

    vte_terminal_set_font_scale(term, zoom);

    /* fuerza reflow */
    vte_terminal_set_size(term, cols, rows);
    
    gtk_widget_queue_resize(GTK_WIDGET(term));
    gtk_widget_queue_draw(GTK_WIDGET(term));
}

void zoom_in(VteTerminal *term)
{
    zoom *= ZOOM_FACTOR;
    if (zoom > MAX_ZOOM) zoom = MAX_ZOOM;
    apply_zoom(term);
}

void zoom_out(VteTerminal *term)
{
    zoom /= ZOOM_FACTOR;
    if (zoom < MIN_ZOOM) zoom = MIN_ZOOM;
    apply_zoom(term);
}

void reset_zoom(VteTerminal* term)
{
  zoom = DEF_ZOOM;
  vte_terminal_set_font_scale(term,zoom); 
}
