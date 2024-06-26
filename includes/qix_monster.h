#ifndef QIX_MONSTER_H
#define QIX_MONSTER_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "globals.h"
#include "drawing.h"
#include "player.h"
#include "bitmap.h" // Include bitmap.h for wall collision detection

#define TRAIL_COUNT 5
#define TRAIL_MAX 1000

typedef struct {
  double x1, y1, x2, y2;
  double opacity;
} Trail;

extern Trail trails[TRAIL_MAX];
extern unsigned int trail_count;
extern const double TRAIL_OFFSET;
extern unsigned int direction_change_interval;
extern unsigned int update_counter;
extern double speed;
extern const double MAX_DISTANCE;

// Function prototypes
double clamp(const double value, const double min, const double max);
double random_range(const int min, const int max);
void generate_random_line(const int max_distance, Point* p1, Point* p2);
void add_trail_point(const double x, const double y);
void initialize_positions_and_directions();
void update_positions_and_trails();
void update_line_position(double *x, double *y, double *dx, double *dy, gboolean *bounced);
void randomize_direction_and_speed(double *dx, double *dy);
int is_colliding_with_wall(const Point p); // New function for collision detection

#endif // QIX_MONSTER_H
