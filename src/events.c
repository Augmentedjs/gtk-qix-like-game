#include "includes/events.h"

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
  (void)controller; // Mark controller as unused
  (void)keycode;    // Mark keycode as unused
  gboolean shift_pressed = state & GDK_SHIFT_MASK;
  static gboolean was_shift_pressed = FALSE;

  double new_x = player_x;
  double new_y = player_y;
  int dx = 0, dy = 0;

  // Save original point for possible storage
  const double original_player_x = player_x;
  const double original_player_y = player_y;

  switch (keyval) {
    case GDK_KEY_Left:
      new_x -= player_speed;
      if (new_x < 0) new_x = 0;
      dx = -1;
      dy = 0;
      break;
    case GDK_KEY_Right:
      new_x += player_speed;
      if (new_x > width) new_x = width;
      dx = 1;
      dy = 0;
      break;
    case GDK_KEY_Up:
      new_y -= player_speed;
      if (new_y < 0) new_y = 0;
      dx = 0;
      dy = -1;
      break;
    case GDK_KEY_Down:
      new_y += player_speed;
      if (new_y > height) new_y = height;
      dx = 0;
      dy = 1;
      break;
  }

  // Update player position and check for direction change
  if (shift_pressed) {
    update_player_position(new_x, new_y);

    if ((dx != last_dx || dy != last_dy)) {
      // Save point on direction change
      add_player_point(original_player_x, original_player_y);
      last_dx = dx;
      last_dy = dy;
    }

    // Check if player reached the boundary or hit a wall
    if (player_x == 0 || player_x == width || player_y == 0 || player_y == height || is_wall(player_x, player_y)) {
      if ((player_y == 0 || player_y == height || is_wall(player_x, player_y)) && drawing_complete == FALSE) {
        // Complete shape vertically
        add_player_point(player_x, player_y);
        add_player_point(last_player_x, player_y);
      } else if ((player_x == 0 || player_x == width || is_wall(player_x, player_y)) && drawing_complete == FALSE) {
        // Complete shape horizontally
        add_player_point(player_x, player_y);
        add_player_point(player_x, last_player_y);
      }
      drawing_complete = TRUE;
    }
  } else {
    player_x = new_x;
    player_y = new_y;
    last_player_x = new_x;
    last_player_y = new_y;
  }

  if (!shift_pressed && was_shift_pressed) {
    // Shift key was released, set drawing_complete flag
    drawing_complete = TRUE;

    // When drawing is complete, mark walls and add lines to the bitmap
    mark_walls(shape_points, shape_point_count);
  }

  was_shift_pressed = shift_pressed;

  gtk_widget_queue_draw(GTK_WIDGET(user_data)); // Redraw the area
  return TRUE;
}
