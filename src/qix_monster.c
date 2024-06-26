#include "includes/qix_monster.h"
#include "includes/bitmap.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Trail trails[TRAIL_MAX];
unsigned int trail_count = 0;
const double TRAIL_OFFSET = 15.0;
unsigned int direction_change_interval = 75;
unsigned int update_counter = 0;
double speed = 0;
const double MAX_DISTANCE = 50;
const double MAX_SPEED = 4.0; // Define the maximum speed

double random_range(const int min, const int max) {
  return (double)(min + rand() % (max - min + 1));
}

double clamp(const double value, const double min, const double max) {
  return value < min ? min : (value > max ? max : value);
}

void generate_random_line(const int max_distance, Point *p1, Point *p2) {
  p1->x = random_range(0, width - 1);
  p1->y = random_range(0, height - 1);

  const double min_x2 = clamp(p1->x - max_distance, 0, width - 1);
  const double max_x2 = clamp(p1->x + max_distance, 0, width - 1);
  const double min_y2 = clamp(p1->y - max_distance, 0, height - 1);
  const double max_y2 = clamp(p1->y + max_distance, 0, height - 1);

  p2->x = random_range(min_x2, max_x2);
  p2->y = random_range(min_y2, max_y2);
}

void add_trail_point(const double x, const double y) {
  if (trail_count >= TRAIL_MAX) {
    return;
  }
  trails[trail_count].x1 = (trail_count == 0) ? x : trails[trail_count - 1].x2;
  trails[trail_count].y1 = (trail_count == 0) ? y : trails[trail_count - 1].y2;
  trails[trail_count].x2 = x;
  trails[trail_count].y2 = y;
  trails[trail_count].opacity = 1.0;
  trail_count++;
}

void initialize_positions_and_directions() {
  srand(time(NULL));

  Point p1, p2;
  generate_random_line(MAX_DISTANCE, &p1, &p2);
  qix_line_x1 = p1.x;
  qix_line_y1 = p1.y;
  qix_line_x2 = p2.x;
  qix_line_y2 = p2.y;

  randomize_direction_and_speed(&dx1, &dy1);
  randomize_direction_and_speed(&dx2, &dy2);

  for (size_t i = 0; i < TRAIL_COUNT; i++) {
    trails[i].x1 = qix_line_x1;
    trails[i].y1 = qix_line_y1;
    trails[i].x2 = qix_line_x2;
    trails[i].y2 = qix_line_x2;
    trails[i].opacity = 0.0;
  }
}

int is_colliding_with_wall(const Point p) {
  int x = (int)round(p.x);
  int y = (int)round(p.y);
  int value = bitmap_get_value(x, y);
  return value == WALL || value == FILLED;
}

void update_line_position(double *x, double *y, double *dx, double *dy, gboolean *bounced) {
  *bounced = FALSE;
  double next_x = *x;
  double next_y = *y;
  int steps = (int)ceil(speed);

  for (int step = 1; step <= steps; step++) {
    double check_x = *x + (*dx * step / steps);
    double check_y = *y + (*dy * step / steps);

    Point next_point = {round(check_x), round(check_y)};
    if (is_colliding_with_wall(next_point)) {
      randomize_direction_and_speed(dx, dy);
      *bounced = TRUE;
      return; // Exit the function early since we found a collision
    }
  }

  // No collision detected; update the position
  *x = next_x + (*dx * speed);
  *y = next_y + (*dy * speed);

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

void randomize_direction_and_speed(double *dx, double *dy) {
  const double angle = (rand() % 360) * (M_PI / 180.0);
  speed = (rand() % (int)MAX_SPEED + 1);

  *dx = cos(angle) * speed;
  *dy = sin(angle) * speed;

  direction_change_interval = (rand() % 50 + 50);
}

void update_positions_and_trails() {
  gboolean bounced1 = FALSE, bounced2 = FALSE;

  update_line_position(&qix_line_x1, &qix_line_y1, &dx1, &dy1, &bounced1);
  update_line_position(&qix_line_x2, &qix_line_y2, &dx2, &dy2, &bounced2);

  qix_monster_x = (int)((qix_line_x1 + qix_line_x2) / 2);
  qix_monster_y = (int)((qix_line_y1 + qix_line_y2) / 2);

  update_counter++;

  if (update_counter >= direction_change_interval) {
    randomize_direction_and_speed(&dx1, &dy1);
    randomize_direction_and_speed(&dx2, &dy2);
    update_counter = 0;
  }

  if (bounced1 || bounced2) {
    qix_color_index = (qix_color_index + 1) % (COLOR_COUNT - 1) + 1;
  }

  for (size_t i = TRAIL_COUNT - 1; i > 0; i--) {
    trails[i] = trails[i - 1];
    trails[i].opacity -= 1.0 / TRAIL_COUNT;
  }

  trails[0].x1 = qix_line_x1;
  trails[0].y1 = qix_line_y1;
  trails[0].x2 = qix_line_x2;
  trails[0].y2 = qix_line_y2;
  trails[0].opacity = 1.0;

  const double offset_modifier = TRAIL_OFFSET * speed;

  for (size_t i = 1; i < TRAIL_COUNT; i++) {
    const double trail_modifier = (offset_modifier * (0.25 * (i)));

    trails[i].x1 = trails[i - 1].x1 - trail_modifier * dx1 / sqrt(dx1 * dx1 + dy1 * dy1);
    trails[i].y1 = trails[i - 1].y1 - trail_modifier * dy1 / sqrt(dx1 * dx1 + dy1 * dy1);
    trails[i].x2 = trails[i - 1].x2 - trail_modifier * dx2 / sqrt(dx2 * dx2 + dy2 * dy2);
    trails[i].y2 = trails[i - 1].y2 - trail_modifier * dy2 / sqrt(dx2 * dx2 + dy2 * dy2);
  }
}
