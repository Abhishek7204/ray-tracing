#ifndef RAY_H
#define RAY_H

#include "color.h"
#include "rt_utility.h"
#include "scene_object_list.h"
#include "vect.h"

class ray {
  point3 org;
  vect dir;
  double time;

public:
  ray() {}
  ray(const point3 &origin, const vect &direction, double time = 0)
      : org(origin), dir(direction), time(time) {}

  const point3 &origin() const { return org; }
  const vect &direction() const { return dir; }
  double getTime() const { return time; }

  const point3 at(double t) const { return org + dir * t; }
};

color rayColor(const ray &r, int depthLeft, const sceneObjectList &world);

double hitSphere(const ray &r, const point3 &center, double radius);
#endif // !RAY_H
