#ifndef PRINTER_H
#define PRINTER_H

#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

#define NEW_WIDTH 80
#define NEW_HEIGHT 60

extern const char density_chars[];

// Function prototypes
void shrink_bitmap(int **original, int reduced[NEW_HEIGHT][NEW_WIDTH]);
void print_bitmap(const int reduced[NEW_HEIGHT][NEW_WIDTH], const int x_ratio, const int y_ratio);
int reduce_and_print_bitmap(int **bitmap);

#endif // PRINTER_H
