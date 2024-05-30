#include <gtk/gtk.h>
#include <cairo.h>
#include "player.h"

static double player_x, player_y;
static double player_speed = 5.0;
static double player_colors[4][3] = {
    {0.6, 0.6, 0.6}, // Light Gray
    {0.5, 0.5, 0.5}, // Medium Gray
    {0.4, 0.4, 0.4}, // Dark Gray
    {0.3, 0.3, 0.3}  // Darker Gray
};

void draw_player(cairo_t *cr) {
    // Draw the player as a cloud of dots
    for (int i = 0; i < PLAYER_DOT_COUNT; i++) {
        double angle = 2 * M_PI * i / PLAYER_DOT_COUNT;
        double radius = 5 + 3 * (i % 4);
        double dot_x = player_x + radius * cos(angle);
        double dot_y = player_y + radius * sin(angle);
        int color_index = i % 4;

        // Set the color for the dot
        cairo_set_source_rgb(cr, player_colors[color_index][0], player_colors[color_index][1], player_colors[color_index][2]);
        cairo_arc(cr, dot_x, dot_y, 2, 0, 2 * M_PI);
        cairo_fill(cr);
    }
}

void update_player_position(double x, double y) {
    player_x = x;
    player_y = y;
}

void initialize_player_position(int width, int height) {
    player_x = width / 2;
    player_y = height;
}

