#include "includes/lines.h"

void add_player_line(const double x1, const double y1, const double x2, const double y2) {
  if (player_line_count < MAX_LINES) {
    player_lines[player_line_count].x1 = x1;
    player_lines[player_line_count].y1 = y1;
    player_lines[player_line_count].x2 = x2;
    player_lines[player_line_count].y2 = y2;
    player_line_count++;
  }
}

void add_filled_shape(const Point *points, const unsigned int point_count) {
  if (filled_shape_count < MAX_SHAPES) {
    filled_shapes[filled_shape_count].point_count = point_count;
    int min_x = points[0].x, min_y = points[0].y;
    int max_x = points[0].x, max_y = points[0].y;
    for (size_t i = 0; i < point_count; i++) {
      filled_shapes[filled_shape_count].points[i] = points[i];
      if (points[i].x < min_x)
        min_x = points[i].x;
      if (points[i].y < min_y)
        min_y = points[i].y;
      if (points[i].x > max_x)
        max_x = points[i].x;
      if (points[i].y > max_y)
        max_y = points[i].y;
    }
    filled_shapes[filled_shape_count].min_x = min_x;
    filled_shapes[filled_shape_count].min_y = min_y;
    filled_shapes[filled_shape_count].max_x = max_x;
    filled_shapes[filled_shape_count].max_y = max_y;
    filled_shape_count++;
  }
}

void add_player_point(const double x, const double y) {
  if (shape_point_count > 0 &&
      shape_points[shape_point_count - 1].x == x &&
      shape_points[shape_point_count - 1].y == y) {
    printf("Won't add duplicate point!\n");
  } else {
    shape_points[shape_point_count++] = (Point){x, y};
    printf("Shape Point added: (X: %.0f, Y: %.0f)\n", x, y); // Debug print
  }
}

void complete_shape_to_boundary() {
  printf("complete_shape_to_boundary Points: %d\n", shape_point_count); // Debug print
  if (shape_point_count < 2) {
    return;
  }

  Point last_point = shape_points[shape_point_count - 1];
  Point first_point = shape_points[0];

  if (last_point.x < 0)
    last_point.x = 0;
  if (last_point.x > width)
    last_point.x = width;
  if (last_point.y < 0)
    last_point.y = 0;
  if (last_point.y > height)
    last_point.y = height;

  if (last_point.x != 0 && last_point.x != width && last_point.y != 0 && last_point.y != height) {
    if (last_point.x < width / 2) {
      shape_points[shape_point_count++] = (Point){0, last_point.y};
      last_point.x = 0;
    } else {
      shape_points[shape_point_count++] = (Point){width, last_point.y};
      last_point.x = width;
    }
  }

  if (last_point.x == 0 || last_point.x == width) {
    if (last_point.y != first_point.y) {
      shape_points[shape_point_count++] = (Point){last_point.x, first_point.y};
    }
  } else if (last_point.y == 0 || last_point.y == height) {
    if (last_point.x != first_point.x) {
      shape_points[shape_point_count++] = (Point){first_point.x, last_point.y};
    }
  }

  if (shape_points[shape_point_count - 1].x != first_point.x || shape_points[shape_point_count - 1].y != first_point.y) {
    shape_points[shape_point_count++] = first_point;
  }

  printf("Completed shape to boundary - (%0.f, %0.f)\n", last_point.x, last_point.y);
}

double clamp(const double value, const double min, const double max) {
  return value < min ? min : (value > max ? max : value);
}

Point find_interior_point() {
  Point centroid = {0, 0};
  for (size_t i = 0; i < shape_point_count; i++) {
    centroid.x += shape_points[i].x;
    centroid.y += shape_points[i].y;
  }
  centroid.x /= shape_point_count;
  centroid.y /= shape_point_count;

  // Calculate the direction away from the QIX Monster
  double dx = centroid.x - qix_monster_x;
  double dy = centroid.y - qix_monster_y;
  const double length = sqrt(dx * dx + dy * dy);
  dx /= length;
  dy /= length;

  // Move the starting point slightly away from the QIX Monster
  centroid.x += dx;
  centroid.y += dy;

  return centroid;
}
