#include "includes/player.h"
#include "includes/lines.h"
#include "includes/globals.h"

double player_x, player_y;
double player_speed = PLAYER_DEFAULT_SPEED;
double last_player_x, last_player_y;

void draw_player(cairo_t *cr) {
  // Draw the player as a cloud of dots
  for (size_t i = 0; i < PLAYER_DOT_COUNT; i++) {
    const double angle = 2 * M_PI * i / PLAYER_DOT_COUNT;
    const double radius = 5 + 3 * (i % 4);
    const double dot_x = player_x + radius * cos(angle);
    const double dot_y = player_y + radius * sin(angle);
    const int color_index = i % 4;

    // Set the color for the dot
    cairo_set_source_rgb(cr, player_colors[color_index][0], player_colors[color_index][1], player_colors[color_index][2]);
    cairo_arc(cr, dot_x, dot_y, 2, 0, 2 * M_PI);
    cairo_fill(cr);
  }
}

void update_player_position(const double x, const double y) {
  gboolean is_drawing = TRUE;

  // Check if the player is on the border
  if (x == 0 || x == width || y == 0 || y == height) {
    is_drawing = FALSE;
  }

  // Draw the line up to the boundary if moving and not on the border
  if (last_player_x != x || last_player_y != y) {
    if (is_drawing) {
      add_player_line(last_player_x, last_player_y, x, y);
    }
  }

  // Update player position
  player_x = x;
  player_y = y;
  last_player_x = x;
  last_player_y = y;
}

void initialize_player_position(const int width, const int height) {
  player_x = width / 2;
  player_y = height;
  last_player_x = player_x;
  last_player_y = player_y;
}
