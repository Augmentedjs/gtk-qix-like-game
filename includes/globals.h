#ifndef GLOBALS_H
#define GLOBALS_H

#include <gtk/gtk.h>
#include "colors.h"

#define MAX_LINES 1000
#define MAX_POINTS 2000
#define MAX_SHAPES 100
#define APPLICATION_NAME "com.augmentedjs.qixline"
#define APPLICATION_TITLE "QIX Style Line Drawing with Trails"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

extern int width;
extern int height;
extern gboolean app_running;
extern guint timeout_id;
extern gboolean drawing_complete;

extern double last_dx, last_dy;
extern double dx1, dy1, dx2, dy2;
extern double qix_line_x1, qix_line_y1, qix_line_x2, qix_line_y2;

extern unsigned int qix_color_index;

extern int qix_monster_x;
extern int qix_monster_y;

typedef struct {
  double x1, y1, x2, y2;
} Line;

typedef struct {
  double x, y;
} Point;

typedef struct {
  Point points[MAX_POINTS];
  unsigned int point_count;
  int min_x, min_y, max_x, max_y;
} Shape;

extern Shape filled_shapes[MAX_SHAPES];
extern unsigned int filled_shape_count;

extern Line player_lines[MAX_LINES];
extern unsigned int player_line_count;

extern unsigned int shape_point_count;
extern Point shape_points[MAX_POINTS];

extern double player_colors[4][3];
void initialize_player_colors(void);

#endif // GLOBALS_H
