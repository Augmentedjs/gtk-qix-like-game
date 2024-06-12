#ifndef PRINTER_H
#define PRINTER_H

#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

#define NEW_WIDTH 80
#define NEW_HEIGHT 60

// Function prototypes
void shrink_bitmap(int **original, int reduced[NEW_HEIGHT][NEW_WIDTH]);
void print_bitmap(int reduced[NEW_HEIGHT][NEW_WIDTH]);
int reduce_and_print_bitmap(int **bitmap);

#endif // PRINTER_H
