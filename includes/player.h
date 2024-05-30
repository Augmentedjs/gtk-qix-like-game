#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_DOT_COUNT 20

void draw_player(cairo_t *cr);
void update_player_position(double x, double y);
void initialize_player_position(int width, int height);

extern double player_x, player_y;
extern double player_speed;
extern double last_player_x, last_player_y;

#endif
