#ifndef LINES_H
#define LINES_H

#include <cairo.h>
#include "globals.h"
#include "bitmap.h"
#include <stdio.h> // Include for debug prints

void add_player_point(const double x, const double y);
void add_player_line(const double x1, const double y1, const double x2, const double y2);
void complete_shape_to_boundary();
Point find_interior_point();
double distance(const Point a, const Point b);
int is_point_inside_shape(const Point point);

#endif
