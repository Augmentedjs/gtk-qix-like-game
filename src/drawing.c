#include "includes/drawing.h"
#include "includes/player.h"
#include "includes/lines.h"
#include "includes/qix_monster.h"
#include "includes/globals.h"
#include <stdio.h> // Include for debug prints

// This is for drawing the screen and objects

void drawDot(cairo_t *cr, const int x, const int y) {
  cairo_set_source_rgb(cr, colors[1][0], colors[1][1], colors[1][2]);
  cairo_arc(cr, x, y, 2, 0, 2 * M_PI);
  cairo_fill(cr);
}

static void draw_background(cairo_t *cr, const int width, const int height) {
  // Set the background color to black
  cairo_set_source_rgb(cr, colors[15][0], colors[15][1], colors[15][2]);
  cairo_paint(cr);

  // Draw the white border using VGA white color
  cairo_set_source_rgb(cr, colors[14][0], colors[14][1], colors[14][2]);
  cairo_set_line_width(cr, 2.0);
  cairo_rectangle(cr, 0.0, 0.0, width, height);
  cairo_stroke(cr);
}

static void draw_text(cairo_t *cr, const int width, const int height) {
  // Set the color for the text (dark gray)
  cairo_set_source_rgb(cr, colors[7][0], colors[7][1], colors[7][2]);
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
  cairo_set_font_size(cr, 48);

  // Get the text extents to center it
  cairo_text_extents_t extents;
  cairo_text_extents(cr, "QIX", &extents);

  // Calculate the position to center the text
  const double x = (width - extents.width) / 2 - extents.x_bearing;
  const double y = (height - extents.height) / 2 - extents.y_bearing;

  // Move to the position and show the text
  cairo_move_to(cr, x, y);
  cairo_show_text(cr, "QIX");
}

void draw_line(cairo_t *cr, const double x1, const double y1, const double x2, const double y2, const double opacity, const double width, const int color_index) {
  // Set the color for the line
  cairo_set_source_rgba(cr, colors[color_index][0], colors[color_index][1], colors[color_index][2], opacity);
  // Set the line width
  cairo_set_line_width(cr, width);

  // Move to the starting point of the line
  cairo_move_to(cr, x1, y1);
  // Draw a line to the end point
  cairo_line_to(cr, x2, y2);

  // Stroke the line to actually draw it
  cairo_stroke(cr);
}

void draw_points(cairo_t *cr) {
  if (shape_point_count > 0) {
    printf("Draw points %d\n", shape_point_count);
    for (size_t i = 0; i < shape_point_count; i++) {
      drawDot(cr, shape_points->x, shape_points->y);
      printf("X: %d Y:%d   \n", shape_points->x, shape_points->y);
    }
    printf("\n");
  }
}

void on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
  // Draw the background and the white border
  draw_background(cr, width, height);

  // Draw the text
  draw_text(cr, width, height);

  // Fill the shape if drawing is complete
  if (drawing_complete) {
    fill_shape(cr);
    drawing_complete = FALSE; // Reset the flag after filling
  }

  // Draw the filled shapes
  draw_filled_shapes(cr);

  // Draw the trails first
  for (size_t i = 0; i < TRAIL_COUNT; i++) {
    if (trails[i].opacity > 0) {
      draw_line(cr, trails[i].x1, trails[i].y1, trails[i].x2, trails[i].y2, trails[i].opacity, 1.0, color_index);
    }
  }
  // Draw the current line
  draw_line(cr, line_x1, line_y1, line_x2, line_y2, 1.0, 2.0, color_index);

  // Draw player-drawn lines
  draw_player_lines(cr);

  // Draw the player
  draw_player(cr);

  draw_points(cr);
}
