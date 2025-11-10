#ifndef QUAD_H
#define QUAD_H

#include "scene_object.h"
#include "scene_object_list.h"
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

inline shared_ptr<sceneObjectList> box(const point3 &a, const point3 &b,
                                       shared_ptr<material> mat) {
  auto sides = make_shared<sceneObjectList>();
  point3 min = point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()),
                      std::fmin(a.z(), b.z()));
  point3 max = point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()),
                      std::fmax(a.z(), b.z()));

  vect xDist = vect(max.x() - min.x(), 0, 0);
  vect yDist = vect(0, max.y() - min.y(), 0);
  vect zDist = vect(0, 0, max.z() - min.z());
  sides->add(make_shared<quadrilateral>(point3(min.x(), min.y(), max.z()),
                                        xDist, yDist,
                                        mat)); // front
  sides->add(make_shared<quadrilateral>(point3(max.x(), min.y(), max.z()),
                                        -zDist, yDist,
                                        mat)); // right
  sides->add(make_shared<quadrilateral>(point3(max.x(), min.y(), min.z()),
                                        -xDist, yDist,
                                        mat)); // back
  sides->add(make_shared<quadrilateral>(point3(min.x(), min.y(), min.z()),
                                        zDist, yDist,
                                        mat)); // left
  sides->add(make_shared<quadrilateral>(point3(min.x(), max.y(), max.z()),
                                        xDist, -zDist,
                                        mat)); // top
  sides->add(make_shared<quadrilateral>(point3(min.x(), min.y(), min.z()),
                                        xDist, zDist,
                                        mat)); // bottom
  return sides;
}
#endif // !QUAD_H
