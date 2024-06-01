#include "includes/globals.h"

int width = 640;
int height = 480;
gboolean app_running = TRUE;
guint timeout_id;
gboolean drawing_complete = FALSE;

double colors[COLOR_COUNT][3] = {
    {0.0, 0.0, 0.5},    // Blue
    {0.0, 0.5, 0.0},    // Green
    {0.0, 0.5, 0.5},    // Cyan
    {0.5, 0.0, 0.0},    // Red
    {0.5, 0.0, 0.5},    // Magenta
    {0.5, 0.5, 0.0},    // Brown
    {0.75, 0.75, 0.75}, // Light Gray
    {0.5, 0.5, 0.5},    // Dark Gray
    {0.5, 0.5, 1.0},    // Light Blue
    {0.5, 1.0, 0.5},    // Light Green
    {0.5, 1.0, 1.0},    // Light Cyan
    {1.0, 0.5, 0.5},    // Light Red
    {1.0, 0.5, 1.0},    // Light Magenta
    {1.0, 1.0, 0.5},    // Yellow
    {1.0, 1.0, 1.0}     // White
};

int last_dx = 0;
int last_dy = 0;
