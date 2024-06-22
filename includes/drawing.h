#ifndef DRAWING_H
#define DRAWING_H

#include <gtk/gtk.h>
#include <cairo.h>
#include "globals.h"
#include "lines.h"
#include "player.h"
#include "qix_monster.h"
#include "bitmap.h"
#include <stdio.h> // Include for debug prints

void draw_QIX_line(cairo_t *cr, const double x1, const double y1, const double x2, const double y2, const double opacity, const double stroke, const int color_index);
void draw_trails(cairo_t *cr);
void draw_player(cairo_t *cr);
int fill_shape();
void draw_player_lines(cairo_t *cr);
void on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);

void draw_bitmap(cairo_t *cr, int **bitmap);
void draw_text(cairo_t *cr);
void draw_background(cairo_t *cr);
void draw_border(cairo_t *cr);
void draw_QIX_center(cairo_t *cr);

#endif // DRAWING_H
