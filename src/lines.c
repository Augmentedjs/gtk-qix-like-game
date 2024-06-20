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

int is_point_inside_shape(const Point point) {
  int count = 0;
  for (size_t i = 0; i < shape_point_count; i++) {
    const Point p1 = shape_points[i];
    const Point p2 = shape_points[(i + 1) % shape_point_count];

    if (((p1.y > point.y) != (p2.y > point.y)) &&
        (point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x)) {
      count++;
    }
  }
  return count % 2;
}

double distance(const Point a, const Point b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double shape_perimeter() {
  double perimeter = 0;
  for (size_t i = 0; i < shape_point_count; i++) {
    const Point p1 = shape_points[i];
    const Point p2 = shape_points[(i + 1) % shape_point_count];
    perimeter += distance(p1, p2);
  }
  return perimeter;
}

Point find_interior_point() {
  printf("find_interior_point - %u\n", shape_point_count);

  Point centroid = {0, 0};
  for (size_t i = 0; i < shape_point_count; i++) {
    printf("  Shape Point %.0f, %.0f\n", shape_points[i].x, shape_points[i].y);
    centroid.x += shape_points[i].x;
    centroid.y += shape_points[i].y;
  }
  centroid.x /= shape_point_count;
  centroid.y /= shape_point_count;

  // Determine number of candidates based on shape perimeter
  const double perimeter = shape_perimeter();
  unsigned int num_candidates = (unsigned int)(perimeter / 10); // Adjust the divisor as needed
  if (num_candidates < 10)
    num_candidates = 10; // Ensure a minimum number of candidates

  Point candidates[num_candidates];
  for (size_t i = 0; i < num_candidates; i++) {
    candidates[i].x = centroid.x + (rand() % 20 - 10);
    candidates[i].y = centroid.y + (rand() % 20 - 10);
  }

  const Point qix_monster = {qix_monster_x, qix_monster_y};
  Point best_point = centroid;
  double max_distance = 0;

  for (size_t i = 0; i < num_candidates; i++) {
    if (is_point_inside_shape(candidates[i])) {
      const double dist = distance(candidates[i], qix_monster);
      if (dist > max_distance) {
        max_distance = dist;
        best_point = candidates[i];
      }
    }
  }

  printf("  QIX Monster %d, %d - Best Point %.0f, %.0f\n", qix_monster_x, qix_monster_y, best_point.x, best_point.y);

  return best_point;
}
