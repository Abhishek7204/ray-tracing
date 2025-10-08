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
  sphere(const point3 &center, double radius, shared_ptr<material> mat);

  sphere(const point3 &center1, const point3 &center2, double radius,
         shared_ptr<material> mat);

  virtual bool isHit(const ray &r, interval ray_t,
                     hitRecord &record) const override;

  virtual aabb boundingBox() const override { return bbox; };

  void getUV(const point3 &p, hitRecord &record) const;
};
#endif // !SPHERE_H
