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

void mark_walls(const Point *points, const unsigned int point_count) {
  for (size_t i = 0; i < point_count; i++) {
    const int x = points[i].x;
    const int y = points[i].y;
    if (x >= 0 && x < width && y >= 0 && y < height) {
      bitmap[x][y] = WALL;
      printf("Marked wall at: (X: %d, Y: %d)\n", x, y); // Debug print
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
    printf("Filled point: (X: %d, Y: %d)\n", px, py); // Debug print

    Node *new_node = NULL;

    // Add (px + 1, py)
    if (px + 1 < width) {
      new_node = (Node *)malloc(sizeof(Node));
      if (!new_node) {
        printf("Memory allocation failed for flood fill deque\n");
        continue;
      }
      new_node->x = px + 1;
      new_node->y = py;
      new_node->next = NULL;
      if (rear)
        rear->next = new_node;
      rear = new_node;
    }

    // Add (px - 1, py)
    if (px - 1 >= 0) {
      new_node = (Node *)malloc(sizeof(Node));
      if (!new_node) {
        printf("Memory allocation failed for flood fill deque\n");
        continue;
      }
      new_node->x = px - 1;
      new_node->y = py;
      new_node->next = NULL;
      if (rear)
        rear->next = new_node;
      rear = new_node;
    }

    // Add (px, py + 1)
    if (py + 1 < height) {
      new_node = (Node *)malloc(sizeof(Node));
      if (!new_node) {
        printf("Memory allocation failed for flood fill deque\n");
        continue;
      }
      new_node->x = px;
      new_node->y = py + 1;
      new_node->next = NULL;
      if (rear)
        rear->next = new_node;
      rear = new_node;
    }

    // Add (px, py - 1)
    if (py - 1 >= 0) {
      new_node = (Node *)malloc(sizeof(Node));
      if (!new_node) {
        printf("Memory allocation failed for flood fill deque\n");
        continue;
      }
      new_node->x = px;
      new_node->y = py - 1;
      new_node->next = NULL;
      if (rear)
        rear->next = new_node;
      rear = new_node;
    }
  }
}

void convert_filled_area_to_points(Point *filled_points, int *point_count) {
  *point_count = 0;
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      if (bitmap[x][y] == FILLED) {
        filled_points[(*point_count)++] = (Point){x, y};
      }
    }
  }
}
