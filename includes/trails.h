#ifndef TRAILS_H
#define TRAILS_H

#define TRAIL_COUNT 5

typedef struct {
    double x1, y1, x2, y2;
    double opacity;
} Trail;

extern Trail trails[TRAIL_COUNT];

#endif
