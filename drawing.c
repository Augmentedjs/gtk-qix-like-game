#include <gtk/gtk.h>
#include <cairo.h>
#include "drawing.h"
#include "trails.h"
#include "line.h"
#include "player.h"

static double colors[COLOR_COUNT][3] = {
    {0.0, 0.0, 0.5},    // Blue
    {0.0, 0.5, 0.0},    // Green
    {0.0, 0.5, 0.5},    // Cyan
    {0.5, 0.0, 0.0},    // Red
    {0.5, 0.0, 0.5},    // Magenta
    {0.5, 0.5, 0.0},    // Brown
    {0.75, 0.75, 0.75}, // Light Gray
    {0.5, 0.5, 0.5},    // Dark Gray
    {0.5, 0.5, 1.0},    // Light Blue
    {0.5, 1.0, 0.5},    // Light Green
    {0.5, 1.0, 1.0},    // Light Cyan
    {1.0, 0.5, 0.5},    // Light Red
    {1.0, 0.5, 1.0},    // Light Magenta
    {1.0, 1.0, 0.5},    // Yellow
    {1.0, 1.0, 1.0}     // White
};

static void draw_background(cairo_t *cr) {
    // Set the background color to black
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_paint(cr);
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
    // Draw the background
    draw_background(cr);

    // Draw the text
    draw_text(cr, width, height);

    // Draw the trails first
    for (int i = 0; i < TRAIL_COUNT; i++) {
        if (trails[i].opacity > 0) {
            draw_line(cr, trails[i].x1, trails[i].y1, trails[i].x2, trails[i].y2, trails[i].opacity, 1.0, color_index);
        }
    }
    // Draw the current line
    draw_line(cr, line_x1, line_y1, line_x2, line_y2, 1.0, 2.0, color_index);

    // Draw the player
    draw_player(cr);
}
