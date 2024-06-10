#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>

int **bitmap;

void initialize_bitmap() {
  bitmap = (int **)malloc(width * sizeof(int *));
  if (!bitmap) {
    printf("Memory allocation failed for bitmap columns\n");
    return;
  }
  for (int i = 0; i < width; i++) {
    bitmap[i] = (int *)malloc(height * sizeof(int));
    if (!bitmap[i]) {
      printf("Memory allocation failed for bitmap rows at column %d\n", i);
      return;
    }
    for (int j = 0; j < height; j++) {
      bitmap[i][j] = EMPTY;
    }
  }
}

void free_bitmap() {
  if (bitmap) {
    for (int i = 0; i < width; i++) {
      free(bitmap[i]);
    }
    free(bitmap);
  }
}

void draw_line(int x1, int y1, int x2, int y2) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = x1 < x2 ? 1 : -1;
  int sy = y1 < y2 ? 1 : -1;
  int err = dx - dy;

  while (1) {
    if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
      bitmap[x1][y1] = WALL;
      printf("Marked wall at: (X: %d, Y: %d)\n", x1, y1); // Debug print
    }
    if (x1 == x2 && y1 == y2)
      break;
    int e2 = err * 2;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}

void mark_walls(const Point *points, const unsigned int point_count) {
  for (size_t i = 0; i < point_count - 1; i++) {
    draw_line(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
  }
  // Draw line from the last point to the first to close the shape
  draw_line(points[point_count - 1].x, points[point_count - 1].y, points[0].x, points[0].y);
}

void convert_filled_area_to_points(Point *filled_points, unsigned int *point_count) {
  *point_count = 0;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (bitmap[x][y] == FILLED) {
        filled_points[(*point_count)++] = (Point){x, y};
      }
    }
  }
}

void flood_fill(const int x, const int y) {
  printf("Starting flood fill at: (X: %d, Y: %d)\n", x, y);
  if (x < 0 || x >= width || y < 0 || y >= height) {
    printf("Starting point is out of bounds\n");
    return;
  }
  if (bitmap[x][y] != EMPTY) {
    printf("Starting point is not empty\n");
    return;
  }

  Node *stack = (Node *)malloc(width * height * sizeof(Node));
  if (!stack) {
    printf("Memory allocation failed for flood fill stack\n");
    return;
  }

  int stack_size = 0;
  stack[stack_size++] = (Node){x, y, NULL};

  while (stack_size > 0) {
    Node current = stack[--stack_size];

    int x1 = current.x;
    while (x1 >= 0 && bitmap[x1][current.y] == EMPTY) {
      x1--;
    }
    x1++;

    int span_above = 0;
    int span_below = 0;
    while (x1 < width && bitmap[x1][current.y] == EMPTY) {
      bitmap[x1][current.y] = FILLED;
      printf("Filled point: (X: %d, Y: %d)\n", x1, current.y); // Debug print

      if (!span_above && current.y > 0 && bitmap[x1][current.y - 1] == EMPTY) {
        stack[stack_size++] = (Node){x1, current.y - 1, NULL};
        span_above = 1;
      } else if (span_above && current.y > 0 && bitmap[x1][current.y - 1] != EMPTY) {
        span_above = 0;
      }

      if (!span_below && current.y < height - 1 && bitmap[x1][current.y + 1] == EMPTY) {
        stack[stack_size++] = (Node){x1, current.y + 1, NULL};
        span_below = 1;
      } else if (span_below && current.y < height - 1 && bitmap[x1][current.y + 1] != EMPTY) {
        span_below = 0;
      }

      x1++;
    }
  }

  free(stack);
}
