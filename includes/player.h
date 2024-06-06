#ifndef PLAYER_H
#define PLAYER_H

#include <gtk/gtk.h>
#include <cairo.h>
#include "globals.h"
#include "lines.h"

#define PLAYER_DOT_COUNT 20
#define PLAYER_DEFAULT_SPEED 5.0
#define PLAYER_SUPER_SPEED 8.0

void draw_player(cairo_t *cr);
void update_player_position(const double x, const double y);
void initialize_player_position(const int width, const int height);

extern double player_x, player_y;
extern double player_speed;
extern double last_player_x, last_player_y;

#endif
