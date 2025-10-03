#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "vect.h"

class ray;
class aabb {
  interval axes[3];

public:
  aabb() = default;

  aabb(const interval &x, const interval &y, const interval &z)
      : axes{x, y, z} {};

  aabb(const point3 &a, const point3 &b);

  aabb(const aabb &boxA, const aabb &boxB);

  bool hit(const ray &r, interval ray_t) const;

  interval operator[](int axis) const { return axes[axis]; }

  int longestAxis() {
    int maxSize = -1, maxAxis = 0;
    for (int i = 0; i < 3; i++) {
      if (axes[i].size() > maxSize) {
        maxSize = axes[i].size();
        maxAxis = i;
      }
    }
    return maxAxis;
  }

  static const aabb empty, universe;
};

#endif // !AABB_H
