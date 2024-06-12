#include "printer.h"

// Function to shrink the bitmap and summarize pixels
void shrink_bitmap(int **original, int reduced[NEW_HEIGHT][NEW_WIDTH]) {
  int x_ratio = width / NEW_WIDTH;
  int y_ratio = height / NEW_HEIGHT;

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
}

// Function to print the reduced bitmap using different characters for different densities
void print_bitmap(int reduced[NEW_HEIGHT][NEW_WIDTH]) {
  char density_chars[] = " .:-=+*#%@";

  for (int h = 0; h < NEW_HEIGHT; h++) {
    for (int w = 0; w < NEW_WIDTH; w++) {
      int index = (reduced[h][w] * 9) / 64; // Map sum to an index in the density_chars array
      printf("%c", density_chars[index]);
    }
    printf("\n");
  }
}

int reduce_and_print_bitmap(int **bitmap) {
  int reduced[NEW_HEIGHT][NEW_WIDTH] = {{0}};
  shrink_bitmap(bitmap, reduced);
  print_bitmap(reduced);
  return 0;
}
