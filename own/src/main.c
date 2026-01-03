#include <gtk/gtk.h>
#include <gtk/gtk.h>
#include "terminal.h"
#include "keys.h"
int main(int argc, char* argv[])
{
  gtk_init(&argc,&argv);

  // Window
  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "own");
  gtk_window_set_default_size(GTK_WINDOW(window),800,600);

  g_signal_connect(
    window,
    "destroy",
    G_CALLBACK(gtk_main_quit),
    NULL
  );

  //Terminal 
  VteTerminal* term = terminal_new();
  terminal_spawn(term);
  terminal_set_font(term);
  //terminal_set_colors(term);
  //keybindings
  
   g_signal_connect(
    term,
    "key_press_event",
    G_CALLBACK(handle_keys_press),
    NULL
  );

  //Layout
  
  gtk_container_add(
    GTK_CONTAINER(window),
    GTK_WIDGET(term)
  );


  gtk_widget_show_all(window);


  //Main loop
  gtk_main();
  return 0;
}
