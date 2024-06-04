#include <gtk/gtk.h>
#include "includes/drawing.h"
#include "includes/player.h"
#include "includes/qix_monster.h"
#include "includes/events.h"
#include "includes/globals.h"

static void on_window_destroy(GtkWidget *widget, gpointer user_data) {
  app_running = FALSE;
  if (timeout_id > 0) {
    g_source_remove(timeout_id);
    timeout_id = 0;
  }
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *drawing_area;
  GtkEventController *controller;

  // Initialize random positions and directions
  initialize_positions_and_directions(width, height);

  // Initialize player colors
  initialize_player_colors();

  // Create a new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "QIX Style Line Drawing with Trails");
  gtk_window_set_default_size(GTK_WINDOW(window), width, height);

  // Disable window resizing
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  // Create a drawing area widget
  drawing_area = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_area, width, height);
  gtk_window_set_child(GTK_WINDOW(window), drawing_area);

  // Connect the draw event to the callback function
  gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area), on_draw, NULL, NULL);

  // Add a timeout to update the line position regularly
  timeout_id = g_timeout_add_full(G_PRIORITY_DEFAULT, 16, on_timeout, drawing_area, NULL);

  // Connect the destroy signal to stop the timeout when the window is closed
  g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

  // Create and connect a key event controller to handle player movement
  controller = gtk_event_controller_key_new();
  g_signal_connect(controller, "key-pressed", G_CALLBACK(on_key_press), drawing_area);
  gtk_widget_add_controller(GTK_WIDGET(window), controller);

  // Present the window
  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  // Create a new GTK application
  app = gtk_application_new("com.augmentedjs.qixline", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  // Run the application
  status = g_application_run(G_APPLICATION(app), argc, argv);

  // Clean up
  g_object_unref(app);

  return status;
}
