#ifndef DRAWING_H
#define DRAWING_H

#define COLOR_COUNT 15

void draw_line(cairo_t *cr, double x1, double y1, double x2, double y2, double opacity, double width, int color_index);
void on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);

#endif
