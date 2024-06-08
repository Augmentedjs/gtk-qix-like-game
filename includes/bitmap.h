#ifndef BITMAP_H
#define BITMAP_H

#include "globals.h"

#define EMPTY 0
#define WALL 1
#define FILLED 2

typedef struct {
  int x, y;
} FloodFillPoint;

typedef struct Node {
  int x, y;
  struct Node* next;
} Node;

extern int **bitmap;

void initialize_bitmap();
void free_bitmap();
void mark_walls(const Point *points, const unsigned int point_count);
void flood_fill(const int x, const int y);
void convert_filled_area_to_points(Point *filled_points, int *point_count);

#endif // BITMAP_H
