#include "includes/globals.h"

int width = 640;
int height = 480;

double colors[COLOR_COUNT][3] = {
  {0.0, 0.0, 0.5},    // Blue
  {0.0, 0.5, 0.0},    // Green
  {0.0, 0.5, 0.5},    // Cyan
  {0.5, 0.0, 0.0},    // Red
  {0.5, 0.0, 0.5},    // Magenta
  {0.5, 0.5, 0.0},    // Brown
  {0.75, 0.75, 0.75}, // Light Gray
  {0.5, 0.5, 0.5},    // Dark Gray
  {0.5, 0.5, 1.0},    // Light Blue
  {0.5, 1.0, 0.5},    // Light Green
  {0.5, 1.0, 1.0},    // Light Cyan
  {1.0, 0.5, 0.5},    // Light Red
  {1.0, 0.5, 1.0},    // Light Magenta
  {1.0, 1.0, 0.5},    // Yellow
  {1.0, 1.0, 1.0},    // White
  {0.0, 0.0, 0.0}     // Black
};

int color_index = 0;

gboolean app_running = TRUE;
guint timeout_id = 0;
gboolean drawing_complete = FALSE;

double line_x = 320, line_y = 240;
double line_dx = 0, line_dy = 0;
double last_dx = 0, last_dy = 0;
double dx1, dy1, dx2, dy2;

Shape filled_shapes[MAX_SHAPES];
int filled_shape_count = 0;

Line player_lines[MAX_LINES];
int player_line_count = 0;
Point player_points[MAX_POINTS];

double line_x1, line_y1, line_x2, line_y2;

double offset = 5.0;

int player_point_count = 0;
