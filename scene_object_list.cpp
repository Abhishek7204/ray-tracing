#include "scene_object_list.h"

bool sceneObjectList::isHit(const ray &r, double tmin, double tmax,
                            hitRecord &record) const {
  hitRecord rec;
  bool hasHit = false;
  double closestHit = tmax;

  for (const auto &object : objectList) {
    if (object->isHit(r, tmin, closestHit, rec)) {
      hasHit = true;
      closestHit = rec.t;
      record = rec;
    }
  }

  return hasHit;
};
