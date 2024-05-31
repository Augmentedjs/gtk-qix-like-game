#include "includes/player.h"
#include "includes/events.h"
#include "includes/line.h"
#include "includes/globals.h"
#include "includes/lines.h"

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
    static gboolean was_ctrl_pressed = FALSE;

    double new_x = player_x;
    double new_y = player_y;

    switch (keyval) {
        case GDK_KEY_Left:
            if (player_y == 1 || player_y == height - 2 || ctrl_pressed) {
                new_x -= player_speed;
                if (new_x < 1) new_x = 1;
            }
            break;
        case GDK_KEY_Right:
            if (player_y == 1 || player_y == height - 2 || ctrl_pressed) {
                new_x += player_speed;
                if (new_x > width - 2) new_x = width - 2;
            }
            break;
        case GDK_KEY_Up:
            if (player_x == 1 || player_x == width - 2 || ctrl_pressed) {
                new_y -= player_speed;
                if (new_y < 1) new_y = 1;
            }
            break;
        case GDK_KEY_Down:
            if (player_x == 1 || player_x == width - 2 || ctrl_pressed) {
                new_y += player_speed;
                if (new_y > height - 2) new_y = height - 2;
            }
            break;
    }

    if (ctrl_pressed && !(new_x <= 1 || new_x >= width - 2 || new_y <= 1 || new_y >= height - 2)) {
        update_player_position(new_x, new_y);
    } else {
        player_x = new_x;
        player_y = new_y;
        last_player_x = new_x;
        last_player_y = new_y;
    }

    if (!ctrl_pressed && was_ctrl_pressed) {
        // Control key was released, set drawing_complete flag
        drawing_complete = TRUE;
    }

    was_ctrl_pressed = ctrl_pressed;

    gtk_widget_queue_draw(GTK_WIDGET(user_data)); // Redraw the area
    return TRUE;
}
