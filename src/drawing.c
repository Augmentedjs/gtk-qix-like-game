#include "includes/drawing.h"

// This is for drawing the screen and objects

void draw_background(cairo_t *cr) {
  // Set the background color to black
  cairo_set_source_rgb(cr, colors[BACKGROUND_COLOR][0], colors[BACKGROUND_COLOR][1], colors[BACKGROUND_COLOR][2]);
  cairo_paint(cr);

  // Draw the white border using VGA white color
  cairo_set_source_rgb(cr, colors[PLAYFIELD_BORDER_COLOR][0], colors[PLAYFIELD_BORDER_COLOR][1], colors[PLAYFIELD_BORDER_COLOR][2]);
  cairo_set_line_width(cr, 2.0);
  cairo_rectangle(cr, 0.0, 0.0, width, height);
  cairo_stroke(cr);
}

void draw_border(cairo_t *cr) {
  cairo_set_source_rgb(cr, colors[PLAYFIELD_BORDER_COLOR][0], colors[PLAYFIELD_BORDER_COLOR][1], colors[PLAYFIELD_BORDER_COLOR][2]);
  cairo_set_line_width(cr, 2.0);
  cairo_rectangle(cr, 0.0, 0.0, width, height);
  cairo_stroke(cr);
}

/* TODO: This will become the title area for score, etc. */
void draw_text(cairo_t *cr) {
  // Set the color for the text (dark gray)
  cairo_set_source_rgb(cr, colors[DARK_GRAY][0], colors[DARK_GRAY][1], colors[DARK_GRAY][2]);
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

void draw_QIX_line(cairo_t *cr, const double x1, const double y1, const double x2, const double y2, const double opacity, const double stroke, const int color_index) {
  // Set the color for the line
  cairo_set_source_rgba(cr, colors[color_index][0], colors[color_index][1], colors[color_index][2], opacity);
  // Set the line width
  cairo_set_line_width(cr, stroke);

  // Move to the starting point of the line
  cairo_move_to(cr, x1, y1);
  // Draw a line to the end point
  cairo_line_to(cr, x2, y2);

  // Stroke the line to actually draw it
  cairo_stroke(cr);
}

void draw_trails(cairo_t *cr) {
  for (size_t i = 0; i < TRAIL_COUNT; i++) {
    if (trails[i].opacity > 0) {
      draw_QIX_line(cr, trails[i].x1, trails[i].y1, trails[i].x2, trails[i].y2, trails[i].opacity, 1.0, qix_color_index);
    }
  }
}

void draw_player(cairo_t *cr) {
  // Draw the player as a cloud of dots
  for (size_t i = 0; i < PLAYER_DOT_COUNT; i++) {
    const double angle = 2 * M_PI * i / PLAYER_DOT_COUNT;
    const double radius = 5 + 3 * (i % 4);
    const double dot_x = player_x + radius * cos(angle);
    const double dot_y = player_y + radius * sin(angle);
    const int index = i % 4;

    // Set the color for the dot
    cairo_set_source_rgb(cr, player_colors[index][0], player_colors[index][1], player_colors[index][2]);
    cairo_arc(cr, dot_x, dot_y, 2, 0, 2 * M_PI);
    cairo_fill(cr);
  }
}

// Function to draw points from a bitmap
void draw_bitmap(cairo_t *cr, int **bitmap) {
  for (size_t y = 0; y < (size_t)height; y++) {
    size_t x = 0;
    while (x < (size_t)width) {
      const int pixel = bitmap[y][x];
      if (pixel != EMPTY) {
        // Find the length of the continuous segment of the same pixel type
        size_t segment_length = 1;
        while ((x + segment_length < (size_t)width) && (bitmap[y][x + segment_length] == pixel)) {
          segment_length++;
        }

        double stroke = 1.0;
        if (pixel == WALL) {
          cairo_set_source_rgb(cr, colors[FAST_FILL_BORDER_COLOR][0], colors[FAST_FILL_BORDER_COLOR][1], colors[FAST_FILL_BORDER_COLOR][2]);
          stroke = 2.0;
        } else if (pixel == FILLED) {
          cairo_set_source_rgb(cr, colors[FAST_FILL_COLOR][0], colors[FAST_FILL_COLOR][1], colors[FAST_FILL_COLOR][2]);
        }

        cairo_rectangle(cr, x, y, segment_length, stroke);
        cairo_fill(cr);

        // Move x to the end of the current segment
        x += segment_length;
      } else {
        // Skip clear areas
        x++;
      }
    }
  }
}

int fill_shape() {
  if (shape_point_count < 2) {
    return 1;
  }
  printf("Fill shape - %d\n", shape_point_count);
  if (shape_point_count < 3) {
    printf("Fill shape too small - %d\n", shape_point_count);
    return 1; // Not enough points to form a shape
  }

  // Complete the shape to the boundary
  // complete_shape_to_boundary();

  // Ensure the shape has at least 4 sides
  // if (shape_point_count < 4) {
  //   printf("Still fill shape too small - %d\n", shape_point_count);
  //   return;
  // }

  printf("Point Count - %d\n", shape_point_count);

  // Mark the walls in the bitmap (for fill)
  mark_walls(shape_points, shape_point_count);

  // Find a starting point for flood fill inside the shape
  const Point start = find_interior_point();

  // Perform flood fill
  flood_fill((int)start.x, (int)start.y);

  // Reset the points after filling
  shape_point_count = 0;
  player_line_count = 0;

  printf("Shape Points reset\n");
  return 0;
}

void draw_player_lines(cairo_t *cr) {
  cairo_set_source_rgb(cr, colors[FAST_LINE_COLOR][0], colors[FAST_LINE_COLOR][1], colors[FAST_LINE_COLOR][2]);
  cairo_set_line_width(cr, 2.0);
  for (size_t i = 0; i < player_line_count; i++) {
    cairo_move_to(cr, player_lines[i].x1, player_lines[i].y1);
    cairo_line_to(cr, player_lines[i].x2, player_lines[i].y2);
    cairo_stroke(cr);
  }
}

void draw_QIX_center(cairo_t *cr) {
  cairo_set_source_rgb(cr, colors[GREEN][0], colors[GREEN][1], colors[GREEN][2]);
  cairo_set_line_width(cr, 4.0);
  cairo_rectangle(cr, qix_monster_x - 2, qix_monster_y - 2, 2, 2);
  cairo_stroke(cr);
}

void on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
  (void)area;      // Mark area as unused
  (void)width;     // Mark width as unused
  (void)height;    // Mark height as unused
  (void)user_data; // Mark user_data as unused

  // Draw the background
  draw_background(cr);

  // Draw the text (will be a title area)
  draw_text(cr);

  // Fill the shape if drawing is complete
  if (drawing_complete) {
    const int ret = fill_shape();
    if (ret == 0) {
      // Print bitmap summary for debugging
      print_bitmap_summary();
    }
    drawing_complete = FALSE; // Reset the flag after filling
  }

  draw_bitmap(cr, bitmap);

  // Draw the trails first
  draw_trails(cr);

  // Draw the current line
  draw_QIX_line(cr, qix_line_x1, qix_line_y1, qix_line_x2, qix_line_y2, 1.0, 2.0, qix_color_index);

  draw_QIX_center(cr);

  if (!drawing_complete) {
    // Draw player-drawn lines
    draw_player_lines(cr);
  }

  // Draw the white border
  draw_border(cr);

  // Draw the player
  draw_player(cr);
}
