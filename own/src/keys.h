#ifndef KEYS_H
#define KEYS_H

#include <gtk/gtk.h>
#include <vte/vte.h>
#define ZOOM_FACTOR 1.1
#define MIN_ZOOM 0.5
#define MAX_ZOOM 3
#define DEF_ZOOM 0.75

typedef struct {
    guint key;
    GdkModifierType mod;
    void (*func)(VteTerminal *);
} Key;

/* handler de eventos */
gboolean handle_keys_press(GtkWidget *widget,
                           GdkEventKey *event,
                           gpointer user_data);

/* acciones */
void copy_clipboard(VteTerminal *term);
void paste_clipboard(VteTerminal *term);
void zoom_in(VteTerminal* term);
void zoom_out(VteTerminal* term);
void reset_zoom(VteTerminal* term);
void apply_zoom(VteTerminal* term);
#endif /* KEYS_H */
