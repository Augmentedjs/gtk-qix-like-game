#ifndef BITMAP_H
#define BITMAP_H

#include "globals.h"
#include "printer.h"
#include <stdio.h>
#include <stdlib.h>

#define EMPTY 0
#define WALL 1
#define FILLED 2

typedef struct {
  int x, y, direction;
} Span;

extern int **bitmap;

void initialize_bitmap();
void free_bitmap();
void mark_walls(Point *points, const unsigned int point_count);
void flood_fill(const int x, const int y);
void convert_filled_area_to_points(Point *filled_points, unsigned int *point_count);
void draw_line(int x1, int y1, const int x2, const int y2);

// for debug
void print_bitmap_summary();

#endif // BITMAP_H
