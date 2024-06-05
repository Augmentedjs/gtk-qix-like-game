#include "includes/events.h"
#include "includes/globals.h"
#include "includes/lines.h"
#include "includes/player.h"
#include "includes/qix_monster.h"

gboolean on_timeout(gpointer user_data) {
  GtkWidget *drawing_area = GTK_WIDGET(user_data);

  if (!GTK_IS_WIDGET(drawing_area) || !app_running) {
    return G_SOURCE_REMOVE;
  }

  // Update positions and trails
  update_positions_and_trails(width, height);
  update_bouncing_line_position();

  // Queue redraw of the drawing area
  gtk_widget_queue_draw(drawing_area);

  // Continue calling this function
  return G_SOURCE_CONTINUE;
}

gboolean on_key_press(GtkEventControllerKey *controller, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
  gboolean shift_pressed = state & GDK_SHIFT_MASK;
  static gboolean was_shift_pressed = FALSE;

  double new_x = player_x;
  double new_y = player_y;
  int dx = 0, dy = 0;

  // save original point for possible storage
  const double original_player_x = player_x;
  const double original_player_y = player_y;

  switch (keyval) {
  case GDK_KEY_Left:
    if (player_y == 1 || player_y == height - 2 || shift_pressed) {
      new_x -= player_speed;
      if (new_x < 1)
        new_x = 1;
      dx = -1;
      dy = 0;
    }
    break;
  case GDK_KEY_Right:
    if (player_y == 1 || player_y == height - 2 || shift_pressed) {
      new_x += player_speed;
      if (new_x > width - 2)
        new_x = width - 2;
      dx = 1;
      dy = 0;
    }
    break;
  case GDK_KEY_Up:
    if (player_x == 1 || player_x == width - 2 || shift_pressed) {
      new_y -= player_speed;
      if (new_y < 1)
        new_y = 1;
      dx = 0;
      dy = -1;
    }
    break;
  case GDK_KEY_Down:
    if (player_x == 1 || player_x == width - 2 || shift_pressed) {
      new_y += player_speed;
      if (new_y > height - 2)
        new_y = height - 2;
      dx = 0;
      dy = 1;
    }
    break;
  }

  // Update player position and check for direction change
  if (shift_pressed && !(new_x <= 1 || new_x >= width - 2 || new_y <= 1 || new_y >= height - 2)) {
    update_player_position(new_x, new_y);
  } else {
    player_x = new_x;
    player_y = new_y;
    last_player_x = new_x;
    last_player_y = new_y;
  }

  if ((dx != last_dx || dy != last_dy) && shift_pressed) {
    // Save point on direction change
    add_player_point(original_player_x, original_player_y); // (player_x, player_y);
    last_dx = dx;
    last_dy = dy;
  }

  // Check if player reached the boundary
  if (player_x == 0 || player_x == width || player_y == 0 || player_y == height) {
    if ((player_y == 0 || player_y == height) && drawing_complete == FALSE && shift_pressed) {
      // Complete shape vertically
      add_player_point(0.0, player_y);      // Adding the boundary point on the left
      add_player_point(0.0, last_player_y); // Adding the boundary point at the last known y position
    } else if ((player_x == 0 || player_x == width) && drawing_complete == FALSE && shift_pressed) {
      // Complete shape horizontally
      add_player_point(player_x, 0.0);      // Adding the boundary point on the top
      add_player_point(last_player_x, 0.0); // Adding the boundary point at the last known x position
    }
    drawing_complete = TRUE;
  }

  if (!shift_pressed && was_shift_pressed) {
    // Shift key was released, set drawing_complete flag
    drawing_complete = TRUE;
  }

  was_shift_pressed = shift_pressed;

  gtk_widget_queue_draw(GTK_WIDGET(user_data)); // Redraw the area
  return TRUE;
}

