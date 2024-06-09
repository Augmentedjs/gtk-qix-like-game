#ifndef LINE_H
#define LINE_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "globals.h"
#include "drawing.h"
#include "player.h"

#define TRAIL_COUNT 5
#define TRAIL_MAX 1000

typedef struct {
  double x1, y1, x2, y2;
  double opacity;
} Trail;

extern Trail trails[TRAIL_MAX];
extern unsigned int trail_count;
extern const double TRAIL_OFFSET; // Offset for trail spread
extern unsigned int direction_change_interval; // Interval for direction change
extern unsigned int update_counter; // Counter for direction changes
extern double speed;
extern const double MAX_DISTANCE;

void add_trail_point(const double x, const double y);
void initialize_positions_and_directions(const int width, const int height);
void update_positions_and_trails(const int width, const int height);
void update_line_position(double *x, double *y, double *dx, double *dy, const int width, const int height, gboolean *bounced);
void randomize_direction_and_speed(double *dx, double *dy);

#endif
