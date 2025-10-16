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
      : axes{x, y, z} {
    paddingMinimum();
  };

  aabb(const point3 &a, const point3 &b);

  aabb(const aabb &boxA, const aabb &boxB);

  bool hit(const ray &r, interval ray_t) const;

  interval operator[](int axis) const { return axes[axis]; }

  int longestAxis();

  void paddingMinimum();

  static const aabb empty, universe;
};

#endif // !AABB_H
