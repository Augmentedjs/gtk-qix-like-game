#ifndef LINES_H
#define LINES_H

#include <cairo.h>

#define MAX_LINES 1000
#define MAX_POINTS 2000
#define MAX_SHAPES 100

typedef struct {
  double x1, y1, x2, y2;
} Line;

typedef struct {
  double x, y;
} Point;

typedef struct {
  Point points[MAX_POINTS];
  int point_count;
} Shape;

extern Line player_lines[MAX_LINES];
extern int player_line_count;
extern Point player_points[MAX_POINTS];
extern int player_point_count;
extern Shape filled_shapes[MAX_SHAPES];
extern int filled_shape_count;

void add_player_line(double x1, double y1, double x2, double y2);
void draw_player_lines(cairo_t *cr);
void fill_shape(cairo_t *cr);
void add_filled_shape(Point *points, int point_count);
void draw_filled_shapes(cairo_t *cr);

#endif
