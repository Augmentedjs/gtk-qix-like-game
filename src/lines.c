#include <cairo.h>
#include "includes/lines.h"

Line player_lines[MAX_LINES];
int player_line_count = 0;

void add_player_line(double x1, double y1, double x2, double y2) {
    if (player_line_count < MAX_LINES) {
        player_lines[player_line_count].x1 = x1;
        player_lines[player_line_count].y1 = y1;
        player_lines[player_line_count].x2 = x2;
        player_lines[player_line_count].y2 = y2;
        player_line_count++;
    }
}

void draw_player_lines(cairo_t *cr) {
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.5); // Blue color
    cairo_set_line_width(cr, 2.0);
    for (int i = 0; i < player_line_count; i++) {
        cairo_move_to(cr, player_lines[i].x1, player_lines[i].y1);
        cairo_line_to(cr, player_lines[i].x2, player_lines[i].y2);
        cairo_stroke(cr);
    }
}
