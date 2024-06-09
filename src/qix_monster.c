#include "includes/qix_monster.h"

Trail trails[TRAIL_MAX];
unsigned int trail_count = 0;
const double TRAIL_OFFSET = 15.0; // Increased offset for more spread-out trails
unsigned int direction_change_interval = 75; // Interval for direction change
unsigned int update_counter = 0;
double speed = 0;
const double MAX_DISTANCE = 50;

void add_trail_point(const double x, const double y) {
  if (trail_count >= TRAIL_MAX) {
    // Handle max trail points
    return;
  }
  trails[trail_count].x1 = (trail_count == 0) ? x : trails[trail_count - 1].x2;
  trails[trail_count].y1 = (trail_count == 0) ? y : trails[trail_count - 1].y2;
  trails[trail_count].x2 = x;
  trails[trail_count].y2 = y;
  trails[trail_count].opacity = 1.0;
  trail_count++;

  //printf("Trail Point added: (X: %.2f, Y: %.2f)\n", x, y); // Debug print
}

void initialize_positions_and_directions(const int width, const int height) {
  // Seed the random number generator
  srand(time(NULL));

  // Initialize line positions randomly within the window bounds
  Point p1, p2;
  // Generate a random line
  generate_random_line(width, height, MAX_DISTANCE, &p1, &p2);
  qix_line_x1 = p1.x;
  qix_line_y1 = p1.y;
  qix_line_x2 = p2.x;
  qix_line_y2 = p2.y;

  // Initialize direction vectors randomly
  randomize_direction_and_speed(&dx1, &dy1);
  randomize_direction_and_speed(&dx2, &dy2);

  // Initialize trails
  for (size_t i = 0; i < TRAIL_COUNT; i++) {
    trails[i].x1 = qix_line_x1;
    trails[i].y1 = qix_line_y1;
    trails[i].x2 = qix_line_x2;
    trails[i].y2 = qix_line_x2;
    trails[i].opacity = 0.0;
  }
}

void update_line_position(double *x, double *y, double *dx, double *dy, const int width, const int height, gboolean *bounced) {
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

  qix_monster_x = (int)((double)((*x + *dx) / 2));
  qix_monster_y = (int)((double)((*x + *dx) / 2));
}

void randomize_direction_and_speed(double *dx, double *dy) {
  const double angle = (rand() % 360) * (M_PI / 180.0); // Random angle in radians
  speed = (rand() % 4 + 1); // Random speed between 1 and 4

  *dx = cos(angle) * speed;
  *dy = sin(angle) * speed;

  direction_change_interval = (rand() % 50 + 50);
  //printf("direction_change_interval %d\n", direction_change_interval);
}

void update_positions_and_trails(const int width, const int height) {
  gboolean bounced1 = FALSE, bounced2 = FALSE;

  // Update line position for both points
  update_line_position(&qix_line_x1, &qix_line_y1, &dx1, &dy1, width, height, &bounced1);
  update_line_position(&qix_line_x2, &qix_line_y2, &dx2, &dy2, width, height, &bounced2);

  // Increment the update counter
  update_counter++;

  // Randomize direction and speed at specific intervals
  if (update_counter >= direction_change_interval) {
    randomize_direction_and_speed(&dx1, &dy1);
    randomize_direction_and_speed(&dx2, &dy2);
    update_counter = 0; // Reset the counter
  }

  // Change color only if any point bounces off the boundary
  if (bounced1 || bounced2) {
    qix_color_index = (qix_color_index + 1) % (COLOR_COUNT - 1) + 1; // skip 'black'
  }

  // Update trails
  for (size_t i = TRAIL_COUNT - 1; i > 0; i--) {
    trails[i] = trails[i - 1];
    trails[i].opacity -= 1.0 / TRAIL_COUNT; // Decrease opacity for fading effect
  }

  // Set the position for the first trail
  trails[0].x1 = qix_line_x1;
  trails[0].y1 = qix_line_y1;
  trails[0].x2 = qix_line_x2;
  trails[0].y2 = qix_line_y2;
  trails[0].opacity = 1.0;

  // Apply offset to the remaining trails

  const double offset_modifier = TRAIL_OFFSET * speed;

  for (size_t i = 1; i < TRAIL_COUNT; i++) {
    const double trail_modifier = (offset_modifier * (0.25 * (i)));

    trails[i].x1 = trails[i - 1].x1 - trail_modifier * dx1 / sqrt(dx1 * dx1 + dy1 * dy1);
    trails[i].y1 = trails[i - 1].y1 - trail_modifier * dy1 / sqrt(dx1 * dx1 + dy1 * dy1);
    trails[i].x2 = trails[i - 1].x2 - trail_modifier * dx2 / sqrt(dx2 * dx2 + dy2 * dy2);
    trails[i].y2 = trails[i - 1].y2 - trail_modifier * dy2 / sqrt(dx2 * dx2 + dy2 * dy2);
  }
}
