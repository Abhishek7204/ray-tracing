#ifndef BVH_H
#define BVH_H

#include "scene_object.h"
#include "scene_object_list.h"
#include <algorithm>
#include <cstdlib>
#include <memory>

class bvh : public sceneObject {
  shared_ptr<sceneObject> left;
  shared_ptr<sceneObject> right;
  aabb bbox;

public:
  bvh(sceneObjectList list)
      : bvh(list.getObjects(), 0, list.getObjects().size()){};

  bvh(vector<shared_ptr<sceneObject>> &objects, size_t start, size_t end) {

    bbox = aabb::empty;
    for (size_t idx = start; idx < end; idx++)
      bbox = aabb(bbox, objects[idx]->boundingBox());
    size_t span = end - start;
    if (span == 1)
      left = right = objects[start];
    else if (span == 2) {
      left = objects[start];
      right = objects[start + 1];
    } else {
      int axis = bbox.longestAxis();
      sort(objects.begin() + start, objects.begin() + end,
           [=](shared_ptr<sceneObject> a, shared_ptr<sceneObject> b) {
             return a->boundingBox()[axis].iMin < b->boundingBox()[axis].iMin;
           });
      auto mid = start + span / 2;
      left = make_shared<bvh>(objects, start, mid);
      right = make_shared<bvh>(objects, mid, end);
    }
    bbox = aabb(left->boundingBox(), right->boundingBox());
  }

  virtual bool isHit(const ray &r, interval ray_t, hitRecord &record) const {
    if (!bbox.hit(r, ray_t))
      return false;

    bool hitLeft = left->isHit(r, ray_t, record);
    bool hitRight = right->isHit(
        r, interval(ray_t.iMin, hitLeft ? record.t : ray_t.iMax), record);
    return hitLeft || hitRight;
  };

  virtual aabb boundingBox() const { return bbox; }
};
#endif // !BVH_H
