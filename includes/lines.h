#ifndef LINES_H
#define LINES_H

#include <cairo.h>
#include "globals.h"


void add_player_point(double x, double y);

void add_player_line(double x1, double y1, double x2, double y2);
void draw_player_lines(cairo_t *cr);
void fill_shape(cairo_t *cr);
void add_filled_shape(Point *points, int point_count);
void draw_filled_shapes(cairo_t *cr);

#endif
