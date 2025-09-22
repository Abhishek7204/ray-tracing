#ifndef SPHERE_H
#define SPHERE_H

#include "rt_utility.h"
#include "scene_object.h"
#include "vect.h"

class sphere : public sceneObject {
  point3 center;
  double radius;
  shared_ptr<material> mat;

public:
  sphere(const point3 &center, double radius, shared_ptr<material> mat)
      : center(center), radius(max(0.0, radius)), mat(mat) {}

  virtual bool isHit(const ray &r, interval ray_t,
                     hitRecord &record) const override;
};
#endif // !SPHERE_H
