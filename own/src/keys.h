#ifndef KEYS_H
#define KEYS_H

#include <gtk/gtk.h>
#include <vte/vte.h>

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

#endif /* KEYS_H */
