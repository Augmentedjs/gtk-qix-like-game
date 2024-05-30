#ifndef LINES_H
#define LINES_H

#define MAX_LINES 1000

typedef struct {
    double x1, y1, x2, y2;
} Line;

extern Line player_lines[MAX_LINES];
extern int player_line_count;

void add_player_line(double x1, double y1, double x2, double y2);
void draw_player_lines(cairo_t *cr);

#endif
