#include "includes/player.h"

double player_x, player_y;
double player_speed = PLAYER_DEFAULT_SPEED;
double last_player_x, last_player_y;

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

void initialize_player_position() {
  player_x = width / 2;
  player_y = height;
  last_player_x = player_x;
  last_player_y = player_y;
}
