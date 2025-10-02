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
  aabb bbox;

public:
  sphere(const point3 &center, double radius, shared_ptr<material> mat)
      : centerLocations(center, vect(0, 0, 0)), radius(max(0.0, radius)),
        mat(mat) {
    vect rVec = vect(1, 1, 1) * radius;
    bbox = aabb(center - rVec, center + rVec);
  }

  sphere(const point3 &center1, const point3 &center2, double radius,
         shared_ptr<material> mat)
      : centerLocations(center1, center2 - center1), radius(max(0.0, radius)),
        mat(mat) {
    vect rVec = vect(1, 1, 1) * radius;
    aabb box1 = aabb(center1 - rVec, center1 + rVec);
    aabb box2 = aabb(center2 - rVec, center2 + rVec);
    bbox = aabb(box1, box2);
  }

  virtual bool isHit(const ray &r, interval ray_t,
                     hitRecord &record) const override;

  virtual aabb boundingBox() const override { return bbox; };
};
#endif // !SPHERE_H
