#ifndef LINES_H
#define LINES_H

#include <cairo.h>
#include "globals.h"

void add_player_point(const double x, const double y);
void add_player_line(const double x1, const double y1, const double x2, const double y2);
void draw_player_lines(cairo_t *cr);
void fill_shape(cairo_t *cr);
void add_filled_shape(const Point *points, const unsigned int point_count);
void draw_filled_shapes(cairo_t *cr);

#endif
