#ifndef LINE_H
#define LINE_H

#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define TRAIL_COUNT 5
#define TRAIL_MAX 1000

typedef struct {
  double x1, y1, x2, y2;
  double opacity;
} Trail;

extern Trail trails[TRAIL_MAX];
extern int trail_count;

void add_trail_point(const double x, const double y);
void initialize_positions_and_directions(const int width, const int height);
void update_positions_and_trails(const int width, const int height);
void update_line_position(double *x, double *y, double *dx, double *dy, const int width, const int height, gboolean *bounced);
void update_bouncing_line_position();

#endif
