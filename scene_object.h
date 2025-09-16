#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

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
  virtual bool isHit(const ray &r, double tmin, double tmax,
                     hitRecord &record) const = 0;
};
#endif
