#ifndef DRAWING_H
#define DRAWING_H

#include <gtk/gtk.h>
#include <cairo.h>
#include "globals.h"
#include "lines.h"
#include "player.h"
#include "qix_monster.h"
#include <stdio.h> // Include for debug prints

void draw_QIX_line(cairo_t *cr, const double x1, const double y1, const double x2, const double y2, const double opacity, const double stroke, const int color_index);
void draw_trails(cairo_t *cr);
void draw_player(cairo_t *cr);
void fill_shape(cairo_t *cr);
void draw_filled_shapes(cairo_t *cr);
void draw_player_lines(cairo_t *cr);
void on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);

static void draw_text(cairo_t *cr);
static void draw_background(cairo_t *cr);

#endif
