#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "rt_utility.h"
#include "scene_object.h"
#include "vect.h"

class sphere : public sceneObject {
  ray centerLocations;
  double radius;
  shared_ptr<material> mat;

public:
  sphere(const point3 &center, double radius, shared_ptr<material> mat)
      : centerLocations(center, vect(0, 0, 0)), radius(max(0.0, radius)),
        mat(mat) {}

  sphere(const point3 &center1, const point3 &center2, double radius,
         shared_ptr<material> mat)
      : centerLocations(center1, center2 - center1), radius(max(0.0, radius)),
        mat(mat) {}
  virtual bool isHit(const ray &r, interval ray_t,
                     hitRecord &record) const override;
};
#endif // !SPHERE_H
