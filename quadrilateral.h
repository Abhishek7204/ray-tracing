#ifndef QUAD_H
#define QUAD_H

#include "scene_object.h"
#include "vect.h"
#include <memory>

class quadrilateral : public sceneObject {
  point3 Q;
  vect u, v;
  shared_ptr<material> mat;
  aabb bbox;
  vect normal;
  double D;
  vect W;

public:
  quadrilateral(const point3 &Q, const vect &u, const vect &v,
                shared_ptr<material> mat);

  virtual void setBoundingBox();

  aabb boundingBox() const override { return bbox; }

  virtual bool isHit(const ray &r, interval ray_t,
                     hitRecord &rec) const override;

  bool isInside(double alpha, double beta, hitRecord &record) const;
};
#endif // !QUAD_H
