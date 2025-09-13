#ifndef RAY_H
#define RAY_H

#include "color.h"
#include "vect.h"

class ray {
  point3 org;
  vect dir;

public:
  ray() {}
  ray(const point3 &origin, const vect &direction)
      : org(origin), dir(direction) {}

  const point3 &origin() const { return org; }
  const vect &direction() const { return dir; }

  point3 at(double t) { return org + dir * t; }
};

color rayColor(const ray &r);

#endif
