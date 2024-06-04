#ifndef DRAWING_H
#define DRAWING_H

#include <gtk/gtk.h>
#include <cairo.h>

void draw_line(cairo_t *cr, double x1, double y1, double x2, double y2, double opacity, double width, int color_index);
void on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);

static void draw_text(cairo_t *cr, int width, int height);
static void draw_background(cairo_t *cr, int width, int height);

#endif
