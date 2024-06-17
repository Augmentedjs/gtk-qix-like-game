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
void on_draw(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data);

static void draw_text(cairo_t *cr);
static void draw_background(cairo_t *cr);

double random_range(const int min, const int max);
void generate_random_line(const int max_distance, Point* p1, Point* p2);

#endif
