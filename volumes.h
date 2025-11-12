#ifndef VOLUMES_H
#define VOLUMES_H

#include "material.h"
#include "scene_object.h"
#include "texture.h"
#include <memory>

class volumes : public sceneObject {
  shared_ptr<sceneObject> boundary;
  double density;
  shared_ptr<material> phaseFunc;

public:
  volumes(shared_ptr<sceneObject> boundary, double density,
          shared_ptr<texture> tex)
      : boundary(boundary), density(density),
        phaseFunc(make_shared<isotropic>(tex)) {}

  volumes(shared_ptr<sceneObject> boundary, double density, const color &albedo)
      : boundary(boundary), density(density),
        phaseFunc(make_shared<isotropic>(albedo)) {}

  bool isHit(const ray &r, interval ray_t, hitRecord &record) const override;

  aabb boundingBox() const override { return boundary->boundingBox(); }
};

#endif // !VOLUMES_H
#define VOLUMES_H
