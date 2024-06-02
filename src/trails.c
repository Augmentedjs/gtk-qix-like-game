#include "includes/trails.h"

#include <stdio.h> // Include for debug prints

Trail trails[TRAIL_MAX];
int trail_count = 0;

void add_point(double x, double y) {
  if (trail_count >= TRAIL_MAX) {
    // Handle max trail points
    return;
  }
  trails[trail_count].x1 = (trail_count == 0) ? x : trails[trail_count - 1].x2;
  trails[trail_count].y1 = (trail_count == 0) ? y : trails[trail_count - 1].y2;
  trails[trail_count].x2 = x;
  trails[trail_count].y2 = y;
  trails[trail_count].opacity = 1.0;
  trail_count++;

  //printf("Point added: (%.2f, %.2f)\n", x, y); // Debug print
}