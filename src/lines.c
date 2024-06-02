#include "includes/lines.h"
#include "includes/globals.h"
#include <cairo.h>
#include <stdio.h> // Include for debug prints

void add_player_line(double x1, double y1, double x2, double y2) {
  if (player_line_count < MAX_LINES) {
    player_lines[player_line_count].x1 = x1;
    player_lines[player_line_count].y1 = y1;
    player_lines[player_line_count].x2 = x2;
    player_lines[player_line_count].y2 = y2;
    player_line_count++;
  }
  if (player_point_count < MAX_POINTS) {
    player_points[player_point_count].x = x1;
    player_points[player_point_count].y = y1;
    player_point_count++;
  }
}

void draw_player_lines(cairo_t *cr) {
  cairo_set_source_rgb(cr, colors[8][0], colors[8][1], colors[8][2]); // VGA blue color
  cairo_set_line_width(cr, 2.0);
  for (int i = 0; i < player_line_count; i++) {
    cairo_move_to(cr, player_lines[i].x1, player_lines[i].y1);
    cairo_line_to(cr, player_lines[i].x2, player_lines[i].y2);
    cairo_stroke(cr);
  }
}

void add_filled_shape(Point *points, int point_count) {
  if (filled_shape_count < MAX_SHAPES) {
    filled_shapes[filled_shape_count].point_count = point_count;
    int min_x = points[0].x, min_y = points[0].y;
    int max_x = points[0].x, max_y = points[0].y;
    for (int i = 0; i < point_count; i++) {
      filled_shapes[filled_shape_count].points[i] = points[i];
      if (points[i].x < min_x) min_x = points[i].x;
      if (points[i].y < min_y) min_y = points[i].y;
      if (points[i].x > max_x) max_x = points[i].x;
      if (points[i].y > max_y) max_y = points[i].y;
    }
    filled_shapes[filled_shape_count].min_x = min_x;
    filled_shapes[filled_shape_count].min_y = min_y;
    filled_shapes[filled_shape_count].max_x = max_x;
    filled_shapes[filled_shape_count].max_y = max_y;
    filled_shape_count++;
  }
}

void complete_shape_to_boundary() {
  if (player_point_count < 3) {
    return;
  }

  Point last_point = player_points[player_point_count - 1];
  Point first_point = player_points[0];

  // Adjust the last point to ensure it aligns with the boundaries correctly
  if (last_point.x <= 0) last_point.x = 0;
  if (last_point.x >= width) last_point.x = width;
  if (last_point.y <= 0) last_point.y = 0;
  if (last_point.y >= height) last_point.y = height;

  // If the last point is already at the boundary, add points to close the shape
  if (last_point.x == 0 || last_point.x == width || last_point.y == 0 || last_point.y == height) {
    if (last_point.x == 0) {
      if (last_point.y != 0) {
        player_points[player_point_count++] = (Point){0, 0};
      }
      if (first_point.y != 0) {
        player_points[player_point_count++] = (Point){first_point.x, 0};
      }
    } else if (last_point.x == width) {
      if (last_point.y != height) {
        player_points[player_point_count++] = (Point){width, height};
      }
      if (first_point.y != height) {
        player_points[player_point_count++] = (Point){first_point.x, height};
      }
    } else if (last_point.y == 0) {
      if (last_point.x != width) {
        player_points[player_point_count++] = (Point){width, 0};
      }
      if (first_point.x != width) {
        player_points[player_point_count++] = (Point){width, first_point.y};
      }
    } else if (last_point.y == height) {
      if (last_point.x != 0) {
        player_points[player_point_count++] = (Point){0, height};
      }
      if (first_point.x != 0) {
        player_points[player_point_count++] = (Point){0, first_point.y};
      }
    }
  } else {
    // Add points to form right angles and close the shape
    if (last_point.x != first_point.x) {
      player_points[player_point_count++] = (Point){last_point.x, first_point.y};
    } else if (last_point.y != first_point.y) {
      player_points[player_point_count++] = (Point){first_point.x, last_point.y};
    }
  }

  // Ensure the last point closes the shape correctly
  if (player_points[player_point_count - 1].x != first_point.x || player_points[player_point_count - 1].y != first_point.y) {
    player_points[player_point_count++] = first_point;
  }
}

void fill_shape(cairo_t *cr) {
  if (player_point_count < 3) {
    return; // Not enough points to form a shape
  }

  // Complete the shape to the boundary
  complete_shape_to_boundary();

  // Ensure the shape has at least 4 sides
  if (player_point_count < 4) {
    return;
  }

  // Store the filled shape before drawing it
  add_filled_shape(player_points, player_point_count);

  // Draw the shape
  cairo_set_source_rgba(cr, colors[8][0], colors[8][1], colors[8][2], 0.5); // Semi-transparent VGA blue color
  cairo_move_to(cr, player_points[0].x, player_points[0].y);

  for (int i = 1; i < player_point_count; i++) {
    cairo_line_to(cr, player_points[i].x, player_points[i].y);
  }

  cairo_close_path(cr);
  cairo_fill(cr);

  // Reset the points after filling
  player_point_count = 0;
}

void draw_filled_shapes(cairo_t *cr) {
  for (int i = 0; i < filled_shape_count; i++) {
    cairo_set_source_rgba(cr, colors[0][0], colors[0][1], colors[0][2], 0.5); // Semi-transparent VGA blue color
    cairo_move_to(cr, filled_shapes[i].points[0].x, filled_shapes[i].points[0].y);

    for (int j = 1; j < filled_shapes[i].point_count; j++) {
      cairo_line_to(cr, filled_shapes[i].points[j].x, filled_shapes[i].points[j].y);
    }

    cairo_close_path(cr);
    cairo_fill(cr);
  }
}
