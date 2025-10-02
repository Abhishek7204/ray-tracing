#include "scene_object_list.h"

void sceneObjectList::add(shared_ptr<sceneObject> object) {
  objectList.push_back(object);
  bbox = aabb(bbox, object->boundingBox());
}

bool sceneObjectList::isHit(const ray &r, interval ray_t,
                            hitRecord &record) const {
  hitRecord rec;
  bool hasHit = false;
  double closestHit = ray_t.iMax;

  for (const auto &object : objectList) {
    if (object->isHit(r, interval(ray_t.iMin, closestHit), rec)) {
      hasHit = true;
      closestHit = rec.t;
      record = rec;
    }
  }

  return hasHit;
};
