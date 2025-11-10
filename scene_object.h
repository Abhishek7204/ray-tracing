#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include "aabb.h"
#include "rt_utility.h"
#include "vect.h"
#include <memory>

class material;
class ray;

class hitRecord {
public:
  point3 contactPoint;
  vect hitNormal;
  double t;
  shared_ptr<material> hitMaterial;
  double u, v;
};

class sceneObject {
public:
  virtual ~sceneObject() = default;

  virtual bool isHit(const ray &r, interval ray_t, hitRecord &record) const = 0;

  virtual aabb boundingBox() const = 0;
};

class translate : public sceneObject {
  shared_ptr<sceneObject> object;
  vect offset;
  aabb bbox;

public:
  translate(shared_ptr<sceneObject> object, const vect &offset)
      : object(object), offset(offset) {
    bbox = object->boundingBox() + offset;
  }

  aabb boundingBox() const override { return bbox; }

  bool isHit(const ray &r, interval ray_t, hitRecord &record) const override;
};
#endif // !SCENE_OBJECT_H
