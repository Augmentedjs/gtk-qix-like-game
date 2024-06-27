#include "includes/qix_monster.h"

Trail trails[TRAIL_MAX];
unsigned int trail_count = 0;
const double TRAIL_OFFSET = 15.0;
unsigned int direction_change_interval = 75;
unsigned int update_counter = 0;
double speed = 0;
double last_x1 = -1, last_y1 = -1, last_x2 = -1, last_y2 = -1;
unsigned int stuck_counter1 = 0, stuck_counter2 = 0;
const double MAX_DISTANCE = 50;
const double MAX_SPEED = 2.0;
const double COLLISION_BUFFER = 3.0;
const unsigned int MAX_DIRECTION_CHANGE_INTERVAL = 50;
const unsigned int MAX_STUCK_COUNT = 2;

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
  const int x = (int)ceil(p.x);
  const int y = (int)ceil(p.y);
  int value = bitmap_get_value(x, y);
  return value == WALL || value == FILLED;
}

Point find_valid_position(double x, double y, double dx, double dy) {
  int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  for (int i = 0; i < 4; i++) {
    double new_x = x + directions[i][0];
    double new_y = y + directions[i][1];
    Point new_point = {new_x, new_y};
    if (!is_colliding_with_wall(new_point)) {
      return new_point;
    }
  }
  return (Point){x - dx, y - dy};
}

void handle_collision(double *x, double *y, double *dx, double *dy) {
  *dx = -*dx;
  *dy = -*dy;
  Point valid_position = find_valid_position(*x, *y, *dx, *dy);
  *x = valid_position.x;
  *y = valid_position.y;
}

void update_line_position(double *x, double *y, double *dx, double *dy, gboolean *bounced, double *last_x, double *last_y, unsigned int *stuck_counter) {
  *bounced = FALSE;
  const double next_x = *x + (*dx * speed);
  const double next_y = *y + (*dy * speed);

  const Point next_point = {ceil(next_x), ceil(next_y)};
  if (is_colliding_with_wall(next_point)) {
    handle_collision(x, y, dx, dy);
    *bounced = TRUE;
  } else {
    *x = clamp(next_x, 0, width - 1);
    *y = clamp(next_y, 0, height - 1);

    if (*x == *last_x && *y == *last_y) {
      (*stuck_counter)++;
      if (*stuck_counter >= MAX_STUCK_COUNT) {
        handle_collision(x, y, dx, dy);
        *bounced = TRUE;
      }
    } else {
      *stuck_counter = 0;
    }

    *last_x = *x;
    *last_y = *y;
  }

  if (*x <= 0 || *x >= width) {
    *dx = -*dx;
    *bounced = TRUE;
  }

  if (*y <= 0 || *y >= height) {
    *dy = -*dy;
    *bounced = TRUE;
  }
}

void randomize_direction_and_speed(double *dx, double *dy) {
  const double angle = (rand() % 360) * (M_PI / 180.0);
  speed = (rand() % (int)MAX_SPEED + 1);

  *dx = cos(angle) * speed;
  *dy = sin(angle) * speed;

  direction_change_interval = (rand() % 50 + MAX_DIRECTION_CHANGE_INTERVAL);
}

void update_positions_and_trails() {
  gboolean bounced1 = FALSE, bounced2 = FALSE;

  update_line_position(&qix_line_x1, &qix_line_y1, &dx1, &dy1, &bounced1, &last_x1, &last_y1, &stuck_counter1);
  update_line_position(&qix_line_x2, &qix_line_y2, &dx2, &dy2, &bounced2, &last_x2, &last_y2, &stuck_counter2);

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
