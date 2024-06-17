#include "printer.h"

const char density_chars[] = " .:-=#@";

// Function to shrink the bitmap and summarize pixels
void shrink_bitmap(int **original, int reduced[NEW_HEIGHT][NEW_WIDTH]) {
  printf("Shrinking bitmap\n");
  const int x_ratio = width / NEW_WIDTH;
  const int y_ratio = height / NEW_HEIGHT;

  for (int h = 0; h < NEW_HEIGHT; h++) {
    for (int w = 0; w < NEW_WIDTH; w++) {
      int sum = 0;
      for (int y = 0; y < y_ratio; y++) {
        for (int x = 0; x < x_ratio; x++) {
          sum += original[h * y_ratio + y][w * x_ratio + x];
        }
      }
      reduced[h][w] = sum; // Store the sum to determine density
    }
  }
  printf("Bitmap shrunk\n");
}

// Function to print the reduced bitmap using different characters for different densities
void print_bitmap(int reduced[NEW_HEIGHT][NEW_WIDTH], int x_ratio, int y_ratio) {
  printf("Printing bitmap\n");
  for (int h = 0; h < NEW_HEIGHT; h++) {
    for (int w = 0; w < NEW_WIDTH; w++) {
      int sum = reduced[h][w];
      int index = (sum * (sizeof(density_chars) - 2)) / (x_ratio * y_ratio * 2);
      if (index < 0)
        index = 0;
      if (index >= (sizeof(density_chars) - 1))
        index = sizeof(density_chars) - 2;
      printf("%c", density_chars[index]);
    }
    printf("\n");
  }
}

int reduce_and_print_bitmap(int **bitmap) {
  printf("Reducing and printing bitmap\n");
  int reduced[NEW_HEIGHT][NEW_WIDTH];
  for (int i = 0; i < NEW_HEIGHT; i++) {
    for (int j = 0; j < NEW_WIDTH; j++) {
      reduced[i][j] = 0;
    }
  }
  const int x_ratio = width / NEW_WIDTH;
  const int y_ratio = height / NEW_HEIGHT;
  shrink_bitmap(bitmap, reduced);
  print_bitmap(reduced, x_ratio, y_ratio);
  return 0;
}
