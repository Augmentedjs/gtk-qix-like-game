#include "includes/globals.h"

int width = SCREEN_WIDTH;
int height = SCREEN_HEIGHT;

double colors[COLOR_COUNT][3] = {
  {0.0, 0.0, 0.0},     // Black
  {0.0, 0.0, 0.667},   // Blue
  {0.0, 0.667, 0.0},   // Green
  {0.0, 0.667, 0.667}, // Cyan
  {0.667, 0.0, 0.0},   // Red
  {0.667, 0.0, 0.667}, // Magenta
  {0.667, 0.333, 0.0}, // Brown (dark yellow)
  {0.667, 0.667, 0.667}, // Light Gray
  {0.333, 0.333, 0.333}, // Dark Gray
  {0.333, 0.333, 1.0}, // Light Blue
  {0.333, 1.0, 0.333}, // Light Green
  {0.333, 1.0, 1.0},   // Light Cyan
  {1.0, 0.333, 0.333}, // Light Red
  {1.0, 0.333, 1.0},   // Light Magenta
  {1.0, 1.0, 0.333},   // Yellow
  {1.0, 1.0, 1.0}      // White
};

double player_colors[4][3];

void initialize_player_colors() {
  player_colors[0][0] = colors[LIGHT_CYAN][0];
  player_colors[0][1] = colors[LIGHT_CYAN][1];
  player_colors[0][2] = colors[LIGHT_CYAN][2];

  player_colors[1][0] = colors[WHITE][0];
  player_colors[1][1] = colors[WHITE][1];
  player_colors[1][2] = colors[WHITE][2];

  player_colors[2][0] = colors[LIGHT_GRAY][0];
  player_colors[2][1] = colors[LIGHT_GRAY][1];
  player_colors[2][2] = colors[LIGHT_GRAY][2];

  player_colors[3][0] = colors[DARK_GRAY][0];
  player_colors[3][1] = colors[DARK_GRAY][1];
  player_colors[3][2] = colors[DARK_GRAY][2];
}

// QIX Line start color
unsigned int qix_color_index = 1;

gboolean app_running = TRUE;
guint timeout_id = 0;
gboolean drawing_complete = TRUE;

double last_dx = 0, last_dy = 0;
double dx1, dy1, dx2, dy2;

Shape filled_shapes[MAX_SHAPES];
unsigned int filled_shape_count = 0;

Line player_lines[MAX_LINES];
unsigned int player_line_count = 0;

double qix_line_x1, qix_line_y1, qix_line_x2, qix_line_y2;

unsigned int shape_point_count = 0;
Point shape_points[MAX_POINTS];

int qix_monster_x = 0;
int qix_monster_y = 0;
