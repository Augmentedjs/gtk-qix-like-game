#include "includes/bitmap.h"
#include <stdio.h>
#include <stdlib.h>

int **bitmap;

void initialize_bitmap() {
  bitmap = (int **)malloc(height * sizeof(int *));
  if (!bitmap) {
    printf("Memory allocation failed for bitmap rows\n");
    return;
  }
  for (size_t y = 0; y < (size_t)height; y++) {
    bitmap[y] = (int *)malloc(width * sizeof(int));
    if (!bitmap[y]) {
      printf("Memory allocation failed for bitmap columns at row %zu\n", y);
      return;
    }
    for (size_t x = 0; x < (size_t)width; x++) {
      bitmap[y][x] = EMPTY;
    }
  }
}

void free_bitmap() {
  if (bitmap) {
    for (size_t y = 0; y < (size_t)height; y++) {
      free(bitmap[y]);
    }
    free(bitmap);
  }
}

int bitmap_get_value(const int x, const int y) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return WALL; // Treat out-of-bounds as walls
  }
  return bitmap[y][x];
}

void draw_line(int x1, int y1, const int x2, const int y2) {
  const int dx = abs(x2 - x1);
  const int dy = abs(y2 - y1);
  const int sx = x1 < x2 ? 1 : -1;
  const int sy = y1 < y2 ? 1 : -1;
  int err = dx - dy;

  while (1) {
    if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
      bitmap[y1][x1] = WALL;
    }
    if (x1 == x2 && y1 == y2)
      break;
    const int e2 = err * 2;
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

void mark_walls(Point *points, const unsigned int point_count) {
  if (point_count == 0) {
    return;
  }
  for (size_t i = 0; i < point_count - 1; i++) {
    draw_line((int)points[i].x, (int)points[i].y, (int)points[i + 1].x, (int)points[i + 1].y);
  }
}

void flood_fill(const int x, const int y) {
  if (x < 0 || x >= width || y < 0 || y >= height) {
    return;
  }
  if (bitmap[y][x] != EMPTY) {
    return;
  }

  const int queue_size = (width * height) * 2 * sizeof(Point);
  Point *queue = (Point *)malloc(queue_size);
  if (!queue) {
    fprintf(stderr, "Memory allocation failed for queue\n");
    exit(EXIT_FAILURE);
  }
  int queue_start = 0, queue_end = 0;

  queue[queue_end++] = (Point){x, y};

  while (queue_start < queue_end) {
    const Point p = queue[queue_start++];
    const int px = p.x;
    const int py = p.y;

    if (px < 0 || px >= width || py < 0 || py >= height) {
      continue;
    }
    if (bitmap[py][px] != EMPTY) {
      continue;
    }

    bitmap[py][px] = FILLED;

    if (px + 1 < width && bitmap[py][px + 1] == EMPTY)
      queue[queue_end++] = (Point){px + 1, py};
    if (px - 1 >= 0 && bitmap[py][px - 1] == EMPTY)
      queue[queue_end++] = (Point){px - 1, py};
    if (py + 1 < height && bitmap[py + 1][px] == EMPTY)
      queue[queue_end++] = (Point){px, py + 1};
    if (py - 1 >= 0 && bitmap[py - 1][px] == EMPTY)
      queue[queue_end++] = (Point){px, py - 1};

    if (queue_end >= queue_size) {
      fprintf(stderr, "Queue overflow: queue_end (%d) exceeds maximum size (%d)\n", queue_end, width * height);
      break;
    }
  }

  free(queue);
  queue = NULL; // Ensure we don't double-free
  printf("Queue freed\n");
}

void print_bitmap_summary() {
  printf("Bitmap summary:\n");
  reduce_and_print_bitmap(bitmap);
}
