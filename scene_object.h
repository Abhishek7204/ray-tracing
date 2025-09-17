#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "rt_utility.h"
#include "vect.h"

class ray;

class hitRecord {
public:
  point3 contactPoint;
  vect hitNormal;
  double t;
};

class sceneObject {
public:
  virtual ~sceneObject() = default;
  virtual bool isHit(const ray &r, interval ray_t, hitRecord &record) const = 0;
};
#endif
