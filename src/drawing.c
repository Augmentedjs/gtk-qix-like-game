#include <gtk/gtk.h>
#include <cairo.h>
#include "includes/drawing.h"
#include "includes/trails.h"
#include "includes/line.h"
#include "includes/player.h"
#include "includes/lines.h"
#include "includes/globals.h"

static void draw_background(cairo_t *cr, int width, int height) {
    // Set the background color to black
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_paint(cr);

    // Draw the white border using VGA white color
    cairo_set_source_rgb(cr, colors[14][0], colors[14][1], colors[14][2]);
    cairo_set_line_width(cr, 2.0);
    cairo_rectangle(cr, 1, 1, width - 2, height - 2);
    cairo_stroke(cr);
}

static void draw_text(cairo_t *cr, int width, int height) {
    // Set the color for the text (dark gray)
    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 48);

    // Get the text extents to center it
    cairo_text_extents_t extents;
    cairo_text_extents(cr, "QIX", &extents);

    // Calculate the position to center the text
    double x = (width - extents.width) / 2 - extents.x_bearing;
    double y = (height - extents.height) / 2 - extents.y_bearing;

    // Move to the position and show the text
    cairo_move_to(cr, x, y);
    cairo_show_text(cr, "QIX");
}

void draw_line(cairo_t *cr, double x1, double y1, double x2, double y2, double opacity, double width, int color_index) {
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
    for (int i = 0; i < TRAIL_COUNT; i++) {
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
}
