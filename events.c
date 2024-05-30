#include <gtk/gtk.h>
#include "player.h"
#include "events.h"
#include "line.h"

static gboolean app_running = TRUE;
static guint timeout_id;

gboolean on_timeout(gpointer user_data) {
    GtkWidget *drawing_area = GTK_WIDGET(user_data);

    if (!GTK_IS_WIDGET(drawing_area) || !app_running) {
        return G_SOURCE_REMOVE;
    }

    // Update positions and trails
    update_positions_and_trails(width, height);

    // Queue redraw of the drawing area
    gtk_widget_queue_draw(drawing_area);

    // Continue calling this function
    return G_SOURCE_CONTINUE;
}

gboolean on_key_press(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
    gboolean ctrl_pressed = state & GDK_CONTROL_MASK;
    switch (keyval) {
        case GDK_KEY_Left:
            if (player_y == 0 || player_y == height || ctrl_pressed) { // Moving horizontally on the top or bottom border or Ctrl pressed
                player_x -= player_speed;
                if (player_x < 0) player_x = 0;
            }
            break;
        case GDK_KEY_Right:
            if (player_y == 0 || player_y == height || ctrl_pressed) { // Moving horizontally on the top or bottom border or Ctrl pressed
                player_x += player_speed;
                if (player_x > width) player_x = width;
            }
            break;
        case GDK_KEY_Up:
            if (player_x == 0 || player_x == width || ctrl_pressed) { // Moving vertically on the left or right border or Ctrl pressed
                player_y -= player_speed;
                if (player_y < 0) player_y = 0;
            }
            break;
        case GDK_KEY_Down:
            if (player_x == 0 || player_x == width || ctrl_pressed) { // Moving vertically on the left or right border or Ctrl pressed
                player_y += player_speed;
                if (player_y > height) player_y = height;
            }
            break;
    }

    gtk_widget_queue_draw(GTK_WIDGET(user_data)); // Redraw the area
    return TRUE;
}

