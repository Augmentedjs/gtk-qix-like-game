#ifndef GLOBALS_H
#define GLOBALS_H

#include <gtk/gtk.h>

#define COLOR_COUNT 15

extern int width;
extern int height;
extern gboolean app_running;
extern guint timeout_id;
extern gboolean drawing_complete;

extern double colors[COLOR_COUNT][3];

#endif