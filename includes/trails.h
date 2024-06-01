#ifndef TRAILS_H
#define TRAILS_H

#define TRAIL_MAX 1000

typedef struct {
  double x1, y1, x2, y2;
  double opacity;
} Trail;

extern Trail trails[TRAIL_MAX];
extern int trail_count;

void add_point(double x, double y);

#endif // TRAILS_H
