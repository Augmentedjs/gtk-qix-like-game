#include "includes/globals.h"

int width = 640;
int height = 480;

double colors[COLOR_COUNT][3] = {
  {0.0, 0.0, 0.66},    // Blue
  {0.0, 0.66, 0.0},    // Green
  {0.0, 0.66, 0.66},   // Cyan
  {0.66, 0.0, 0.0},    // Red
  {0.66, 0.0, 0.66},   // Magenta
  {0.66, 0.33, 0.0},   // Brown
  {0.66, 0.66, 0.66},  // Light Gray
  {0.33, 0.33, 0.33},  // Dark Gray
  {0.66, 0.66, 1.0},   // Light Blue
  {0.66, 1.0, 0.66},   // Light Green
  {0.66, 1.0, 1.0},    // Light Cyan
  {1.0, 0.66, 0.66},   // Light Red
  {1.0, 0.66, 1.0},    // Light Magenta
  {1.0, 1.0, 0.66},    // Yellow
  {1.0, 1.0, 1.0},     // White
  {0.0, 0.0, 0.0}      // Black
};

double player_colors[4][3];

void initialize_player_colors() {
  player_colors[0][0] = colors[8][0];
  player_colors[0][1] = colors[8][1];
  player_colors[0][2] = colors[8][2];

  player_colors[1][0] = colors[15][0];
  player_colors[1][1] = colors[15][1];
  player_colors[1][2] = colors[15][2];

  player_colors[2][0] = colors[6][0];
  player_colors[2][1] = colors[6][1];
  player_colors[2][2] = colors[6][2];

  player_colors[3][0] = colors[7][0];
  player_colors[3][1] = colors[7][1];
  player_colors[3][2] = colors[7][2];
}

unsigned color_index = 0;

gboolean app_running = TRUE;
guint timeout_id = 0;
gboolean drawing_complete = FALSE;

double line_x = 320, line_y = 240;
double line_dx = 0, line_dy = 0;
double last_dx = 0, last_dy = 0;
double dx1, dy1, dx2, dy2;

Shape filled_shapes[MAX_SHAPES];
unsigned filled_shape_count = 0;

Line player_lines[MAX_LINES];
unsigned player_line_count = 0;

Point player_points[MAX_POINTS];

double line_x1, line_y1, line_x2, line_y2;

double offset = 5.0;

unsigned player_point_count = 0;

unsigned int shape_point_count = 0;
Point shape_points[MAX_POINTS];
