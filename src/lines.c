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
  cairo_set_source_rgb(cr, colors[LIGHT_BLUE][0], colors[LIGHT_BLUE][1], colors[LIGHT_BLUE][2]); // VGA blue color
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
  shape_points[shape_point_count++] = (Point){x, y};
  printf("Shape Point added: (X: %.0f, Y: %.0f)\n", x, y); // Debug print
}

void complete_shape_to_boundary() {
  printf("complete_shape_to_boundary Points: %d\n", shape_point_count); // Debug print
  if (shape_point_count < 2) {
    return;
  }

  Point last_point = shape_points[shape_point_count - 1];
  Point first_point = shape_points[0];

  // Adjust the last point to ensure it aligns with the boundaries correctly
  if (last_point.x < 0) last_point.x = 0;
  if (last_point.x > width) last_point.x = width;
  if (last_point.y < 0) last_point.y = 0;
  if (last_point.y > height) last_point.y = height;

  // Move last point to the nearest boundary if it's not on one
  if (last_point.x != 0 && last_point.x != width && last_point.y != 0 && last_point.y != height) {
    if (last_point.x < width / 2) {
      shape_points[shape_point_count++] = (Point){0, last_point.y};
      last_point.x = 0;
    } else {
      shape_points[shape_point_count++] = (Point){width, last_point.y};
      last_point.x = width;
    }
  }

  // Add intermediate points to ensure right angles
  if (last_point.x == 0 || last_point.x == width) {
    if (last_point.y != first_point.y) {
      shape_points[shape_point_count++] = (Point){last_point.x, first_point.y};
    }
  } else if (last_point.y == 0 || last_point.y == height) {
    if (last_point.x != first_point.x) {
      shape_points[shape_point_count++] = (Point){first_point.x, last_point.y};
    }
  }

  // Ensure the shape is closed by adding the first point
  if (shape_points[shape_point_count - 1].x != first_point.x || shape_points[shape_point_count - 1].y != first_point.y) {
    shape_points[shape_point_count++] = first_point;
  }

  printf("Completed shape to boundary - (%0.f, %0.f)\n", last_point.x, last_point.y);
}

double clamp(double value, double min, double max) {
  return value < min ? min : (value > max ? max : value);
}


void fill_shape(cairo_t *cr) {
  if (shape_point_count < 2) {
    return;
  }
  printf("Fill shape - %d\n", shape_point_count);
  if (shape_point_count < 3) {
    printf("Fill shape too small - %d\n", shape_point_count);
    // return; // Not enough points to form a shape
  }



  // Complete the shape to the boundary
  complete_shape_to_boundary();

  // Ensure the shape has at least 4 sides
  if (shape_point_count < 4) {
    printf("Still fill shape too small - %d\n", shape_point_count);
    return;
  }

  printf("Point Count - %d\n", shape_point_count);

  // Store the filled shape before drawing it
  add_filled_shape(shape_points, shape_point_count);

  // Draw the filled shape
  // cairo_set_source_rgba(cr, colors[LIGHT_BLUE][0], colors[LIGHT_BLUE][1], colors[LIGHT_BLUE][2], 0.5); // Semi-transparent VGA blue color
  // cairo_move_to(cr, shape_points[0].x, shape_points[0].y);

  // for (size_t i = 1; i < shape_point_count; i++) {
  //   cairo_line_to(cr, shape_points[i].x, shape_points[i].y);
  // }

  // cairo_close_path(cr);
  // cairo_fill(cr);

  // Draw the border
  // cairo_set_source_rgb(cr, colors[LIGHT_CYAN][0], colors[LIGHT_CYAN][1], colors[LIGHT_CYAN][2]);
  // cairo_set_line_width(cr, 2.0);

  // cairo_move_to(cr, shape_points[0].x, shape_points[0].y);

  // for (size_t i = 1; i < shape_point_count; i++) {
  //   cairo_line_to(cr, shape_points[i].x, shape_points[i].y);
  // }

  // cairo_close_path(cr);
  // cairo_stroke(cr);

  // Reset the points after filling
  shape_point_count = 0;
  player_line_count = 0;

  printf("Shape Points reset\n");
}


void draw_filled_shapes(cairo_t *cr) {
  for (size_t i = 0; i < filled_shape_count; i++) {
    // Draw the filled shape
    cairo_set_source_rgba(cr, colors[BLUE][0], colors[BLUE][1], colors[BLUE][2], 0.5); // Semi-transparent VGA blue color
    cairo_move_to(cr, filled_shapes[i].points[0].x, filled_shapes[i].points[0].y);

    for (size_t j = 1; j < filled_shapes[i].point_count; j++) {
      cairo_line_to(cr, filled_shapes[i].points[j].x, filled_shapes[i].points[j].y);
    }

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

