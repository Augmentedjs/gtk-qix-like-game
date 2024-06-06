#ifndef DRAWING_H
#define DRAWING_H

#include <gtk/gtk.h>
#include <cairo.h>
#include "lines.h"

void draw_QIX_line(cairo_t *cr, const double x1, const double y1, const double x2, const double y2, const double opacity, const double width, const int color_index);
void draw_trails(cairo_t *cr);
void on_draw(GtkDrawingArea *area, cairo_t *cr, const int width, const int height, gpointer user_data);

static void draw_text(cairo_t *cr, const int width, const int height);
static void draw_background(cairo_t *cr, const int width, const int height);

double random_range(const int min, const int max);
void generate_random_line(const int width, const int height, const int max_distance, Point* p1, Point* p2);

#endif
