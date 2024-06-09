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

  Node *front = (Node *)malloc(sizeof(Node));
  if (!front) {
    printf("Memory allocation failed for flood fill deque\n");
    return;
  }
  front->x = x;
  front->y = y;
  front->next = NULL;

  Node *rear = front;

  int count = 0;
  const int max_fill_points = width * height;

  while (front != NULL) {
    Node *current = front;
    front = front->next;

    int px = current->x, py = current->y;
    free(current);

    if (px < 0 || px >= width || py < 0 || py >= height)
      continue;
    if (bitmap[px][py] != EMPTY)
      continue;

    bitmap[px][py] = FILLED;
    // printf("Filled point: (X: %d, Y: %d)\n", px, py); // Debug print

    count++;
    if (count > max_fill_points) {
      printf("Flood fill exceeded maximum points, stopping to prevent crash\n");
      break;
    }

    Node *new_node = NULL;

    // Add (px + 1, py)
    if (px + 1 < width && bitmap[px + 1][py] == EMPTY) {
      new_node = (Node *)malloc(sizeof(Node));
      if (!new_node) {
        printf("Memory allocation failed for flood fill deque\n");
        continue;
      }
      new_node->x = px + 1;
      new_node->y = py;
      new_node->next = front;
      front = new_node;
    }

    // Add (px - 1, py)
    if (px - 1 >= 0 && bitmap[px - 1][py] == EMPTY) {
      new_node = (Node *)malloc(sizeof(Node));
      if (!new_node) {
        printf("Memory allocation failed for flood fill deque\n");
        continue;
      }
      new_node->x = px - 1;
      new_node->y = py;
      new_node->next = front;
      front = new_node;
    }

    // Add (px, py + 1)
    if (py + 1 < height && bitmap[px][py + 1] == EMPTY) {
      new_node = (Node *)malloc(sizeof(Node));
      if (!new_node) {
        printf("Memory allocation failed for flood fill deque\n");
        continue;
      }
      new_node->x = px;
      new_node->y = py + 1;
      new_node->next = front;
      front = new_node;
    }

    // Add (px, py - 1)
    if (py - 1 >= 0 && bitmap[px][py - 1] == EMPTY) {
      new_node = (Node *)malloc(sizeof(Node));
      if (!new_node) {
        printf("Memory allocation failed for flood fill deque\n");
        continue;
      }
      new_node->x = px;
      new_node->y = py - 1;
      new_node->next = front;
      front = new_node;
    }
  }
}
