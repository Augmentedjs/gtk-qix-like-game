#ifndef COLORS_H
#define COLORS_H

#define COLOR_COUNT 16

extern double colors[COLOR_COUNT][3];

// Matches CGA / EGA / VGA 16 Color Text
enum ColorIndex {
  BLACK,        // 0
  BLUE,         // 1
  GREEN,        // 2
  CYAN,         // 3
  RED,          // 4
  MAGENTA,      // 5
  BROWN,        // 6
  LIGHT_GRAY,   // 7
  DARK_GRAY,    // 8
  LIGHT_BLUE,   // 9
  LIGHT_GREEN,  // 10
  LIGHT_CYAN,   // 11
  LIGHT_RED,    // 12
  LIGHT_MAGENTA,// 13
  YELLOW,       // 14
  WHITE         // 15
};

enum LineFillColor {
  SLOW_LINE_COLOR = LIGHT_MAGENTA,
  SLOW_FILL_COLOR = RED,
  SLOW_FILL_BORDER_COLOR = LIGHT_RED,
  FAST_LINE_COLOR = LIGHT_CYAN,
  FAST_FILL_COLOR = BLUE,
  FAST_FILL_BORDER_COLOR = LIGHT_BLUE,
  BACKGROUND_COLOR = BLACK,
  PLAYFIELD_BORDER_COLOR = WHITE
};

#endif