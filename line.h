#ifndef LINE_H
#define LINE_H

#define TRAIL_COUNT 5

void initialize_positions_and_directions(int width, int height);
void update_positions_and_trails(int width, int height);
void update_line_position(double *x, double *y, double *dx, double *dy, int width, int height, gboolean *bounced);

extern double line_x1, line_y1, line_x2, line_y2;
extern int color_index;
extern double offset;

#endif

