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

void draw_player_lines(cairo_t *cr) {
  cairo_set_source_rgb(cr, colors[LIGHT_BLUE][0], colors[LIGHT_BLUE][1], colors[LIGHT_BLUE][2]);
  cairo_set_line_width(cr, 2.0);
  for (size_t i = 0; i < player_line_count; i++) {
    cairo_move_to(cr, player_lines[i].x1, player_lines[i].y1);
    cairo_line_to(cr, player_lines[i].x2, player_lines[i].y2);
    cairo_stroke(cr);
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

double clamp(double value, double min, double max) {
  return value < min ? min : (value > max ? max : value);
}

Point find_interior_point() {
  Point centroid = {0, 0};
  for (int i = 0; i < shape_point_count; i++) {
    centroid.x += shape_points[i].x;
    centroid.y += shape_points[i].y;
  }
  centroid.x /= shape_point_count;
  centroid.y /= shape_point_count;

  // Calculate the direction away from the QIX Monster
  double dx = centroid.x - qix_monster_x;
  double dy = centroid.y - qix_monster_y;
  double length = sqrt(dx * dx + dy * dy);
  dx /= length;
  dy /= length;

  // Move the starting point slightly away from the QIX Monster
  centroid.x += dx;
  centroid.y += dy;

  return centroid;
}

void fill_shape(cairo_t *cr) {
  if (shape_point_count < 2) {
    return;
  }
  printf("Fill shape - %d\n", shape_point_count);
  if (shape_point_count < 3) {
    printf("Fill shape too small - %d\n", shape_point_count);
    return; // Not enough points to form a shape
  }

  // Complete the shape to the boundary
  complete_shape_to_boundary();

  // Ensure the shape has at least 4 sides
  // if (shape_point_count < 4) {
  //   printf("Still fill shape too small - %d\n", shape_point_count);
  //   return;
  // }

  printf("Point Count - %d\n", shape_point_count);

  // Mark the walls in the bitmap (for fill)
  mark_walls(shape_points, shape_point_count);

  // Find a starting point for flood fill inside the shape
  Point start = find_interior_point();
  printf("Starting flood fill at: (X: %d, Y: %d)\n", (int)start.x, (int)start.y); // Debug print

  // Perform flood fill
  flood_fill((int)start.x, (int)start.y);

  // Convert the filled area to points
  Point *new_points = (Point *)malloc((width * height) * 2 * sizeof(Point));
  if (!new_points) {
    printf("Memory allocation failed for new_points\n");
    return;
  }
  unsigned int new_point_count = 0;
  convert_filled_area_to_points(new_points, &new_point_count);

  // printf("New points count: %d\n", new_point_count);
  // for (unsigned int i = 0; i < new_point_count; i++) {
  //   printf("New point %d: (X: %d, Y: %d)\n", i, (int)new_points[i].x, (int)new_points[i].y); // Debug print
  // }

  if (new_point_count == 0) {
    printf("No new points were filled.\n");
    free(new_points);
    shape_point_count = 0;
    return;
  }

  // Store the filled shape
  add_filled_shape(new_points, new_point_count);
  free(new_points);

  // Reset the points after filling
  shape_point_count = 0;
  player_line_count = 0;

  printf("Shape Points reset\n");
}

void draw_filled_shapes(cairo_t *cr) {
  // TODO: This is not drawing what was stored.  Maybe just draw the points in the bitmap?
  for (size_t i = 0; i < filled_shape_count; i++) {
    // Draw the filled shape
    cairo_set_source_rgba(cr, colors[BLUE][0], colors[BLUE][1], colors[BLUE][2], 0.5); // Semi-transparent VGA blue color
    cairo_move_to(cr, filled_shapes[i].points[0].x, filled_shapes[i].points[0].y);

    // for (size_t j = 1; j < filled_shapes[i].point_count; j++) {
    //   cairo_line_to(cr, filled_shapes[i].points[j].x, filled_shapes[i].points[j].y);
    // }

    cairo_close_path(cr);
    cairo_fill(cr);

    // Draw the border
    cairo_set_source_rgb(cr, colors[LIGHT_CYAN][0], colors[LIGHT_CYAN][1], colors[LIGHT_CYAN][2]);
    cairo_set_line_width(cr, 2.0);

    cairo_move_to(cr, filled_shapes[i].points[0].x, filled_shapes[i].points[0].y);

    for (size_t j = 1; j < filled_shapes[i].point_count; j++) {
      cairo_line_to(cr, filled_shapes[i].points[j].x, filled_shapes[i].points[j].y);
    }

    cairo_close_path(cr);
    cairo_stroke(cr);
  }
}
