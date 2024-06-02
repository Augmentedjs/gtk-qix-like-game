#include "includes/qix_monster.h"
#include "includes/drawing.h"
#include "includes/trails.h"
#include "includes/player.h"
#include "includes/globals.h"

void update_bouncing_line_position() {
  // Update line position
  line_x += line_dx;
  line_y += line_dy;

  // Check for collisions with window edges
  if (line_x <= 0 || line_x >= width) {
      line_dx = -line_dx;
  }
  if (line_y <= 0 || line_y >= height) {
      line_dy = -line_dy;
  }

  // Check for collisions with shape boundaries
  for (int i = 0; i < filled_shape_count; i++) {
    Shape *shape = &filled_shapes[i];
    if (line_x >= shape->min_x && line_x <= shape->max_x && line_y >= shape->min_y && line_y <= shape->max_y) {
      if (line_x <= shape->min_x || line_x >= shape->max_x) {
        line_dx = -line_dx;
      }
      if (line_y <= shape->min_y || line_y >= shape->max_y) {
        line_dy = -line_dy;
      }
    }
  }
}

void initialize_positions_and_directions(int width, int height) {
  // Seed the random number generator
  srand(time(NULL));

  // Initialize line positions randomly within the window bounds
  line_x1 = rand() % width;
  line_y1 = rand() % height;
  line_x2 = rand() % width;
  line_y2 = rand() % height;

  // Initialize direction vectors randomly (-2, -1, 1, 2)
  dx1 = (rand() % 3 + 1) * (rand() % 2 ? 1 : -1);
  dy1 = (rand() % 3 + 1) * (rand() % 2 ? 1 : -1);
  dx2 = (rand() % 3 + 1) * (rand() % 2 ? 1 : -1);
  dy2 = (rand() % 3 + 1) * (rand() % 2 ? 1 : -1);

  // Initialize trails
  for (int i = 0; i < TRAIL_COUNT; i++) {
    trails[i].x1 = line_x1;
    trails[i].y1 = line_y1;
    trails[i].x2 = line_x2;
    trails[i].y2 = line_y2;
    trails[i].opacity = 0.0;
  }

  // Initialize player position on the bottom border
  initialize_player_position(width, height);
}

void update_line_position(double *x, double *y, double *dx, double *dy, int width, int height, gboolean *bounced) {
  *bounced = FALSE;
  *x += *dx;
  *y += *dy;

  if (*x <= 0) {
    *x = 0;
    *dx = -*dx;
    *bounced = TRUE;
  } else if (*x >= width) {
    *x = width;
    *dx = -*dx;
    *bounced = TRUE;
  }

  if (*y <= 0) {
    *y = 0;
    *dy = -*dy;
    *bounced = TRUE;
  } else if (*y >= height) {
    *y = height;
    *dy = -*dy;
    *bounced = TRUE;
  }
}

void update_positions_and_trails(int width, int height) {
  gboolean bounced1 = FALSE, bounced2 = FALSE;

  // Update line position for both points
  update_line_position(&line_x1, &line_y1, &dx1, &dy1, width, height, &bounced1);
  update_line_position(&line_x2, &line_y2, &dx2, &dy2, width, height, &bounced2);

  // Change color only if any point bounces off the boundary
  if (bounced1 || bounced2) {
    color_index = (color_index + 1) % (COLOR_COUNT - 1); // skip 'black'
  }

  // Update trails
  for (int i = TRAIL_COUNT - 1; i > 0; i--) {
    trails[i] = trails[i - 1];
    trails[i].opacity -= 1.0 / TRAIL_COUNT; // Decrease opacity for fading effect
  }

  // Set the position for the first trail
  trails[0].x1 = line_x1;
  trails[0].y1 = line_y1;
  trails[0].x2 = line_x2;
  trails[0].y2 = line_y2;
  trails[0].opacity = 1.0;

  // Apply offset to the remaining trails
  for (int i = 1; i < TRAIL_COUNT; i++) {
    trails[i].x1 = trails[i - 1].x1 - offset * dx1 / sqrt(dx1 * dx1 + dy1 * dy1);
    trails[i].y1 = trails[i - 1].y1 - offset * dy1 / sqrt(dx1 * dx1 + dy1 * dy1);
    trails[i].x2 = trails[i - 1].x2 - offset * dx2 / sqrt(dx2 * dx2 + dy2 * dy2);
    trails[i].y2 = trails[i - 1].y2 - offset * dy2 / sqrt(dx2 * dx2 + dy2 * dy2);
  }
}
